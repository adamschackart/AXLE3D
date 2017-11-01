/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

/* TODO: tessellation (use mikko mononen's "libtess2" GLUtess optimization)
 * TODO: reweld - pack raw vertices with an already generated index buffer
 * TODO: Sven Forstmann's quadric-based edge collapse mesh simplification
 * TODO: half-edge meshes (unknown at this time if they would belong here)
 * TODO: linear interpolation with bounding box recalculation (for culling)
 */
void ae_vertex_init(int argc, char** argv) {}
void ae_vertex_quit(void) {}

/*
================================================================================
 * ~~ [ vertex format descriptors ] ~~ *
--------------------------------------------------------------------------------
*/

size_t ae_vertex_format_element_offset(const ae_vertex_format_t fmt, const char* elem)
{
    ae_assert(elem && elem[0], "invalid element string \"%s\" for vertex format %s",
                                elem, ae_vertex_format_name[fmt]); // validate input

    switch (toupper(elem[0]))
    {
        case 'T': return ae_vertex_format_texcoord_offset[fmt];
        case 'C': return ae_vertex_format_color_offset[fmt];
        case 'N': return ae_vertex_format_normal_offset[fmt];
        case 'V': return ae_vertex_format_position_offset[fmt];

        default: break;
    }

    ae_assert(0, "invalid element string \"%s\" for vertex format %s",
                        elem, ae_vertex_format_name[fmt]); return 0;
}

size_t ae_vertex_format_element_size(const ae_vertex_format_t fmt, const char* elem)
{
    ae_assert(elem && elem[0], "invalid element string \"%s\" for vertex format %s",
                                elem, ae_vertex_format_name[fmt]); // validate input

    switch (toupper(elem[0]))
    {
        case 'T': return ae_vertex_format_texcoord_size[fmt];
        case 'C': return ae_vertex_format_color_size[fmt];
        case 'N': return ae_vertex_format_normal_size[fmt];
        case 'V': return ae_vertex_format_position_size[fmt];

        default: break;
    }

    ae_assert(0, "invalid element string \"%s\" for vertex format %s",
                        elem, ae_vertex_format_name[fmt]); return 0;
}

ae_vertex_format_t
ae_vertex_format_from_sizes(const size_t tex_coord, const size_t color,
                            const size_t normal, const size_t position)
{
    size_t i = 0, n = AE_VERTEX_FORMAT_COUNT;

    for (; i < n; i++)
    {
        if (tex_coord == ae_vertex_format_texcoord_size[i] &&
            color == ae_vertex_format_color_size[i] &&
            normal == ae_vertex_format_normal_size[i] &&
            position == ae_vertex_format_position_size[i])
        {
            return (ae_vertex_format_t)i;
        }
    }

    ae_assert(0, "tex = %u col = %u nrm = %u pos = %u!", (u32)tex_coord,
                                (u32)color, (u32)normal, (u32)position);

    return AE_VERTEX_FORMAT_COUNT;
}

/*
================================================================================
 * ~~ [ bounding volume construction ] ~~ *
--------------------------------------------------------------------------------
*/

/*
    floats - the number of floats in the vertex buffer
    offset - position of the component in the vertex
    stride - the total length of the vertex (in floats)
    length - length of the component (3 for rgb etc.)
*/

void vtx_aabbox_ex(float* minv, float* maxv, const float* const vertices,
                                const size_t floats, const size_t offset,
                                const size_t stride, const size_t length)
{
    AE_PROFILE_ENTER();

    ae_assert(offset + length <= stride,"offset %u, length %u, stride %u",
        (unsigned int)offset, (unsigned int)length, (unsigned int)stride);

    ae_assert(floats % stride == 0, "%u %u", (u32)floats, (u32)stride);

    ae_if (floats >= stride)
    {
        size_t i;

        vec_copy(minv, vertices + offset, length);
        vec_copy(maxv, vertices + offset, length);

        for (i = offset; i < floats; i += stride)
        {
            vec_min_vec(minv, minv, vertices + i, length);
            vec_max_vec(maxv, maxv, vertices + i, length);
        }
    }
    else
    {
        vec_zero(minv, length);
        vec_zero(maxv, length);
    }

    AE_PROFILE_LEAVE();
}

void vtx_aabbox_flt_ex(float * minv, float* maxv, const float* const vertices,
                const size_t floats, const size_t offset, const size_t stride)
{
    vtx_aabbox_ex(minv, maxv, vertices, floats, offset, stride, 1);
}

void vtx_aabbox2_ex(float minv[2], float maxv[2], const float* const vertices,
                const size_t floats, const size_t offset, const size_t stride)
{
    vtx_aabbox_ex(minv, maxv, vertices, floats, offset, stride, 2);
}

void vtx_aabbox3_ex(float minv[3], float maxv[3], const float* const vertices,
                const size_t floats, const size_t offset, const size_t stride)
{
    AE_PROFILE_ENTER(); // loop unrolled and most writes are eliminated
    ae_assert(floats % stride == 0, "%u %u", (u32)floats, (u32)stride);

    ae_assert(offset + 3 <= stride, "offset %u, stride %u",
            (unsigned int) offset, (unsigned int) stride );

    /* NOTE: offset 5, stride 8 (GL_T2F_N3F_V3F) is a common case. call
     * into aabbox4_ex that uses _mm_(min/max)_ps & keep the top 3 vals.
     *
     * NOTE: win32 profiling shows the "scalar" SSE path is the fastest,
     * probably because of data dependencies and out of order execution.
     */
    if (offset == 5 && stride == 8 && 0)
    {
        float min4[4];
        float max4[4];

        vtx_aabbox4_ex(min4, max4, vertices, floats, 4, 8);

        vec3copy(minv, min4 + 1);
        vec3copy(maxv, max4 + 1);

        AE_PROFILE_LEAVE(); return;
    }

    ae_if (floats >= stride)
    {
        float* vertex = (float*)vertices + offset;
        float* endptr = (float*)vertices + floats;

        #if defined(__SSE__) && !defined(AE_BRANCH_COVERAGE)
        if (ae_cpuinfo_sse())
        {
            __m128 min_x = _mm_load_ss(vertex + 0);
            __m128 min_y = _mm_load_ss(vertex + 1);
            __m128 min_z = _mm_load_ss(vertex + 2);

            __m128 max_x = _mm_load_ss(vertex + 0);
            __m128 max_y = _mm_load_ss(vertex + 1);
            __m128 max_z = _mm_load_ss(vertex + 2);

            for (; vertex < endptr; vertex += stride)
            {
                const __m128 vtx_x = _mm_load_ss(vertex + 0);
                const __m128 vtx_y = _mm_load_ss(vertex + 1);
                const __m128 vtx_z = _mm_load_ss(vertex + 2);

                min_x = _mm_min_ss(min_x, vtx_x);
                min_y = _mm_min_ss(min_y, vtx_y);
                min_z = _mm_min_ss(min_z, vtx_z);

                max_x = _mm_max_ss(max_x, vtx_x);
                max_y = _mm_max_ss(max_y, vtx_y);
                max_z = _mm_max_ss(max_z, vtx_z);
            }

            _mm_store_ss(minv + 0, min_x);
            _mm_store_ss(minv + 1, min_y);
            _mm_store_ss(minv + 2, min_z);

            _mm_store_ss(maxv + 0, max_x);
            _mm_store_ss(maxv + 1, max_y);
            _mm_store_ss(maxv + 2, max_z);
        }
        else
        #endif
        {
            // initialize the bounding box with the first mesh vertex
            vec3copy(minv, vertex);
            vec3copy(maxv, vertex);

            for (; vertex < endptr; vertex += stride)
            {
                ae_if (vertex[0] < minv[0]) { minv[0] = vertex[0]; }
                ae_if (vertex[1] < minv[1]) { minv[1] = vertex[1]; }
                ae_if (vertex[2] < minv[2]) { minv[2] = vertex[2]; }

                ae_if (vertex[0] > maxv[0]) { maxv[0] = vertex[0]; }
                ae_if (vertex[1] > maxv[1]) { maxv[1] = vertex[1]; }
                ae_if (vertex[2] > maxv[2]) { maxv[2] = vertex[2]; }
            }
        }
    }
    else
    {
        vec3zero(minv);
        vec3zero(maxv);
    }

    AE_PROFILE_LEAVE();
}

void vtx_aabbox4_ex(float minv[4], float maxv[4], const float* const vertices,
                const size_t floats, const size_t offset, const size_t stride)
{
    AE_PROFILE_ENTER(); // loop unrolled and most writes are eliminated
    ae_assert(floats % stride == 0, "%u %u", (u32)floats, (u32)stride);

    ae_assert(offset + 4 <= stride, "offset %u, stride %u",
            (unsigned int) offset, (unsigned int) stride );

    ae_if (floats >= stride)
    {
        float* vertex = (float*)vertices + offset;
        float* endptr = (float*)vertices + floats;

        #if defined(__SSE__) && !defined(AE_BRANCH_COVERAGE)
        if (ae_cpuinfo_sse() && num_is_aligned((size_t)vertices, 16) &&
            /*
                common case - min & max of pos at the end of a vertex
            */
            num_is_aligned(offset, 4) &&
            num_is_aligned(stride, 4))
        {
            __m128 min4 = _mm_load_ps(vertex);
            __m128 max4 = _mm_load_ps(vertex);

            for (; vertex < endptr; vertex += stride)
            {
                const __m128 v = _mm_load_ps(vertex);

                min4 = _mm_min_ps(min4, v);
                max4 = _mm_max_ps(max4, v);
            }

            _mm_storeu_ps(minv, min4);
            _mm_storeu_ps(maxv, max4);
        }
        else
        #endif
        {
            // initialize the bounding box with the first mesh vertex
            vec4copy(minv, vertex);
            vec4copy(maxv, vertex);

            for (; vertex < endptr; vertex += stride)
            {
                ae_if (vertex[0] < minv[0]) { minv[0] = vertex[0]; }
                ae_if (vertex[1] < minv[1]) { minv[1] = vertex[1]; }
                ae_if (vertex[2] < minv[2]) { minv[2] = vertex[2]; }
                ae_if (vertex[3] < minv[3]) { minv[3] = vertex[3]; }

                ae_if (vertex[0] > maxv[0]) { maxv[0] = vertex[0]; }
                ae_if (vertex[1] > maxv[1]) { maxv[1] = vertex[1]; }
                ae_if (vertex[2] > maxv[2]) { maxv[2] = vertex[2]; }
                ae_if (vertex[3] > maxv[3]) { maxv[3] = vertex[3]; }
            }
        }
    }
    else
    {
        vec4zero(minv);
        vec4zero(maxv);
    }

    AE_PROFILE_LEAVE();
}

void vtx_aabbox_flt(float* minv, float* maxv, // packed
        const float* const verts, const size_t floats)
{
    vtx_aabbox_flt_ex(minv, maxv, verts, floats, 0, 1);
}

void vtx_aabbox2(float minv[2], float maxv[2], // packed fast path
                const float * const vertices, const size_t floats)
{
    vtx_aabbox2_ex(minv, maxv, vertices, floats, 0, 2);
}

void vtx_aabbox3(float minv[3], float maxv[3], // packed fast path
                const float * const vertices, const size_t floats)
{
    // TODO use _mm_(min/max)_ps on 12 floats at a time
    vtx_aabbox3_ex(minv, maxv, vertices, floats, 0, 3);
}

void vtx_aabbox4(float minv[4], float maxv[4], // packed fast path
                const float * const vertices, const size_t floats)
{
    vtx_aabbox4_ex(minv, maxv, vertices, floats, 0, 4);
}

/*
================================================================================
 * ~~ [ vector copy ] ~~ *
--------------------------------------------------------------------------------
*/

void vtx_vec_copy_ex(float* vertices, const float* const val,
                    const size_t floats, const size_t offset,
                    const size_t stride, const size_t length)
{
    AE_PROFILE_ENTER();

    float* vertex = vertices + offset;
    float* endptr = vertices + floats;

    ae_assert(offset + length <= stride,"offset %u, length %u, stride %u",
        (unsigned int)offset, (unsigned int)length, (unsigned int)stride);

    ae_assert(floats % stride == 0, "%u %u", (u32)floats, (u32)stride);

    for (; vertex < endptr; vertex += stride)
    {
        vec_copy(vertex, val, length);
    }

    AE_PROFILE_LEAVE();
}

void vtx_flt_copy_ex(float * vertices, const float val, // write value to pos
                const size_t floats, const size_t offset, const size_t stride)
{
    vtx_vec_copy_ex(vertices, &val, floats, offset, stride, 1);
}

void vtx_vec2copy_ex(float* vertices, const float val[2], // write val to pos
                const size_t floats, const size_t offset, const size_t stride)
{
    vtx_vec_copy_ex(vertices, val, floats, offset, stride, 2);
}

void vtx_vec3copy_ex(float* vertices, const float val[3], // write val to pos
                const size_t floats, const size_t offset, const size_t stride)
{
    vtx_vec_copy_ex(vertices, val, floats, offset, stride, 3);
}

void vtx_vec4copy_ex(float* vertices, const float val[4], // write val to pos
                const size_t floats, const size_t offset, const size_t stride)
{
    vtx_vec_copy_ex(vertices, val, floats, offset, stride, 4);
}

void vtx_flt_copy(float* vertices, const float fvalue, const size_t floats)
{
    vtx_flt_copy_ex(vertices, fvalue, floats, 0, 1);
}

void vtx_vec2copy(float* vertices, const float val[2], const size_t floats)
{
    vtx_vec2copy_ex(vertices, val, floats, 0, 2);
}

void vtx_vec3copy(float* vertices, const float val[3], const size_t floats)
{
    vtx_vec3copy_ex(vertices, val, floats, 0, 3);
}

void vtx_vec4copy(float* vertices, const float val[4], const size_t floats)
{
    vtx_vec4copy_ex(vertices, val, floats, 0, 4);
}

/*
================================================================================
 * ~~ [ vector epsilon compare ] ~~ *
--------------------------------------------------------------------------------
NOTE: helps find "uniform" mesh channels (same values repeated in every vertex)
--------------------------------------------------------------------------------
*/

int vtx_vec_cmp_e_ex(
    const float* const vertices, const float* const value, const float epsilon,
    const size_t floats, const size_t offset,
    const size_t stride, const size_t length)
{
    AE_PROFILE_ENTER();

    float* vertex = (float*)vertices + offset;
    float* endptr = (float*)vertices + floats;

    ae_assert(offset + length <= stride,"offset %u, length %u, stride %u",
        (unsigned int)offset, (unsigned int)length, (unsigned int)stride);

    ae_assert(floats % stride == 0, "%u %u", (u32)floats, (u32)stride);

    for (; vertex < endptr; vertex += stride)
    {
        if (!vec_cmp_e(vertex, value, epsilon, length))
        {
            AE_PROFILE_LEAVE(); return 0;
        }
    }

    // NOTE: empty arrays should never compare as true
    AE_PROFILE_LEAVE(); return floats != 0;
}

int vtx_flt_cmp_e_ex(const float* const vertices, const float value, // float comparison
    const float epsilon, const size_t floats, const size_t offset, const size_t stride)
{
    return vtx_vec_cmp_e_ex(vertices, &value, epsilon, floats, offset, stride, 1);
}

int vtx_vec2cmp_e_ex(const float* const vertices, const float value[2], // vec2 compare
    const float epsilon, const size_t floats, const size_t offset, const size_t stride)
{
    return vtx_vec_cmp_e_ex(vertices, value, epsilon, floats, offset, stride, 2);
}

int vtx_vec3cmp_e_ex(const float* const vertices, const float value[3], // vec3 compare
    const float epsilon, const size_t floats, const size_t offset, const size_t stride)
{
    return vtx_vec_cmp_e_ex(vertices, value, epsilon, floats, offset, stride, 3);
}

int vtx_vec4cmp_e_ex(const float* const vertices, const float value[4], // vec4 compare
    const float epsilon, const size_t floats, const size_t offset, const size_t stride)
{
    return vtx_vec_cmp_e_ex(vertices, value, epsilon, floats, offset, stride, 4);
}

int vtx_flt_cmp_e(const float* const vertices, const float value,
                        const float epsilon, const size_t floats)
{
    return vtx_flt_cmp_e_ex(vertices, value, epsilon, floats, 0, 1);
}

int vtx_vec2cmp_e(const float* const vertices, const float value[2],
                        const float epsilon, const size_t floats)
{
    return vtx_vec2cmp_e_ex(vertices, value, epsilon, floats, 0, 2);
}

int vtx_vec3cmp_e(const float* const vertices, const float value[3],
                        const float epsilon, const size_t floats)
{
    return vtx_vec3cmp_e_ex(vertices, value, epsilon, floats, 0, 3);
}

int vtx_vec4cmp_e(const float* const vertices, const float value[4],
                        const float epsilon, const size_t floats)
{
    return vtx_vec4cmp_e_ex(vertices, value, epsilon, floats, 0, 4);
}

/*
================================================================================
 * ~~ [ vector add ] ~~ *
--------------------------------------------------------------------------------
*/

void vtx_vec_add_ex(float * vertices, const float* const val,
                    const size_t floats, const size_t offset,
                    const size_t stride, const size_t length)
{
    AE_PROFILE_ENTER();

    float* vertex = vertices + offset;
    float* endptr = vertices + floats;

    ae_assert(offset + length <= stride,"offset %u, length %u, stride %u",
        (unsigned int)offset, (unsigned int)length, (unsigned int)stride);

    ae_assert(floats % stride == 0, "%u %u", (u32)floats, (u32)stride);

    for (; vertex < endptr; vertex += stride)
    {
        vec_add_vec(vertex, vertex, val, length);
    }

    AE_PROFILE_LEAVE();
}

void vtx_flt_add_ex(float* vertices, const float fvalue, const size_t floats,
                                    const size_t offset, const size_t stride)
{
    vtx_vec_add_ex(vertices, &fvalue, floats, offset, stride, 1);
}

void vtx_vec2add_ex(float* vertices, const float val[2], const size_t floats,
                                    const size_t offset, const size_t stride)
{
    vtx_vec_add_ex(vertices, val, floats, offset, stride, 2);
}

void vtx_vec3add_ex(float* vertices, const float val[3], const size_t floats,
                                    const size_t offset, const size_t stride)
{
    AE_PROFILE_ENTER();

    float* ptr = vertices + offset;
    float* end = vertices + floats;

    ae_assert(floats % stride == 0, "%u %u", (u32)floats, (u32)stride);

    ae_assert(offset + 3 <= stride, "offset %u, stride %u",
            (unsigned int) offset, (unsigned int) stride );

    #if defined(__SSE__)
    ae_if (stride == 3 && ae_cpuinfo_sse() && num_is_aligned((size_t)vertices, 16))
    {
        float* unr = ptr + (floats - (floats % 12)); // unrolled sentinel

        const __m128 v0 = { val[0], val[1], val[2], val[0] };
        const __m128 v1 = { val[1], val[2], val[0], val[1] };
        const __m128 v2 = { val[2], val[0], val[1], val[2] };

        for (; ptr < unr; ptr += 12)
        {
            _mm_store_ps(ptr + 0, _mm_add_ps(_mm_load_ps(ptr + 0), v0));
            _mm_store_ps(ptr + 4, _mm_add_ps(_mm_load_ps(ptr + 4), v1));
            _mm_store_ps(ptr + 8, _mm_add_ps(_mm_load_ps(ptr + 8), v2));
        }
    }
    #endif

    for (; ptr < end; ptr += stride) { vec3add_vec(ptr, ptr, val); }
    AE_PROFILE_LEAVE();
}

void vtx_vec4add_ex(float* vertices, const float val[4], const size_t floats,
                                    const size_t offset, const size_t stride)
{
    vtx_vec_add_ex(vertices, val, floats, offset, stride, 4);
}

void vtx_flt_add(float* vertices, const float fvalue, const size_t floats)
{
    vtx_flt_add_ex(vertices, fvalue, floats, 0, 1);
}

void vtx_vec2add(float* vertices, const float val[2], const size_t floats)
{
    vtx_vec2add_ex(vertices, val, floats, 0, 2);
}

void vtx_vec3add(float* vertices, const float val[3], const size_t floats)
{
    vtx_vec3add_ex(vertices, val, floats, 0, 3);
}

void vtx_vec4add(float* vertices, const float val[4], const size_t floats)
{
    vtx_vec4add_ex(vertices, val, floats, 0, 4);
}

/*
================================================================================
 * ~~ [ vector mul ] ~~ *
--------------------------------------------------------------------------------
*/

void vtx_vec_mul_ex(float * vertices, const float* const val,
                    const size_t floats, const size_t offset,
                    const size_t stride, const size_t length)
{
    AE_PROFILE_ENTER();

    float* vertex = vertices + offset;
    float* endptr = vertices + floats;

    ae_assert(offset + length <= stride,"offset %u, length %u, stride %u",
        (unsigned int)offset, (unsigned int)length, (unsigned int)stride);

    ae_assert(floats % stride == 0, "%u %u", (u32)floats, (u32)stride);

    for (; vertex < endptr; vertex += stride)
    {
        vec_mul_vec(vertex, vertex, val, length);
    }

    AE_PROFILE_LEAVE();
}

void vtx_flt_mul_ex(float* vertices, const float fvalue, const size_t floats,
                                    const size_t offset, const size_t stride)
{
    vtx_vec_mul_ex(vertices, &fvalue, floats, offset, stride, 1);
}

void vtx_vec2mul_ex(float* vertices, const float val[2], const size_t floats,
                                    const size_t offset, const size_t stride)
{
    vtx_vec_mul_ex(vertices, val, floats, offset, stride, 2);
}

void vtx_vec3mul_ex(float* vertices, const float val[3], const size_t floats,
                                    const size_t offset, const size_t stride)
{
    AE_PROFILE_ENTER();

    float* ptr = vertices + offset;
    float* end = vertices + floats;

    ae_assert(floats % stride == 0, "%u %u", (u32)floats, (u32)stride);

    ae_assert(offset + 3 <= stride, "offset %u, stride %u",
            (unsigned int) offset, (unsigned int) stride );

    #if defined(__SSE__)
    ae_if (stride == 3 && ae_cpuinfo_sse() && num_is_aligned((size_t)vertices, 16))
    {
        float* unr = ptr + (floats - (floats % 12)); // unrolled sentinel

        const __m128 v0 = { val[0], val[1], val[2], val[0] };
        const __m128 v1 = { val[1], val[2], val[0], val[1] };
        const __m128 v2 = { val[2], val[0], val[1], val[2] };

        for (; ptr < unr; ptr += 12)
        {
            _mm_store_ps(ptr + 0, _mm_mul_ps(_mm_load_ps(ptr + 0), v0));
            _mm_store_ps(ptr + 4, _mm_mul_ps(_mm_load_ps(ptr + 4), v1));
            _mm_store_ps(ptr + 8, _mm_mul_ps(_mm_load_ps(ptr + 8), v2));
        }
    }
    #endif

    for (; ptr < end; ptr += stride) { vec3mul_vec(ptr, ptr, val); }
    AE_PROFILE_LEAVE();
}

void vtx_vec4mul_ex(float* vertices, const float val[4], const size_t floats,
                                    const size_t offset, const size_t stride)
{
    vtx_vec_mul_ex(vertices, val, floats, offset, stride, 4);
}

void vtx_flt_mul(float* vertices, const float fvalue, const size_t floats)
{
    vtx_flt_mul_ex(vertices, fvalue, floats, 0, 1);
}

void vtx_vec2mul(float* vertices, const float val[2], const size_t floats)
{
    vtx_vec2mul_ex(vertices, val, floats, 0, 2);
}

void vtx_vec3mul(float* vertices, const float val[3], const size_t floats)
{
    vtx_vec3mul_ex(vertices, val, floats, 0, 3);
}

void vtx_vec4mul(float* vertices, const float val[4], const size_t floats)
{
    vtx_vec4mul_ex(vertices, val, floats, 0, 4);
}

/*
================================================================================
 * ~~ [ vector lerp ] ~~ *
--------------------------------------------------------------------------------
*/

void vtx_vec_lerp_ex(float* verts, const float* const val, const float t,
                                const size_t floats, const size_t offset,
                                const size_t stride, const size_t length)
{
    AE_PROFILE_ENTER();

    float* vertex = verts + offset;
    float* endptr = verts + floats;

    ae_assert(offset + length <= stride,"offset %u, length %u, stride %u",
        (unsigned int)offset, (unsigned int)length, (unsigned int)stride);

    ae_assert(floats % stride == 0, "%u %u", (u32)floats, (u32)stride);

    for (; vertex < endptr; vertex += stride)
    {
        vec_lerp(vertex, vertex, val, t, length);
    }

    AE_PROFILE_LEAVE();
}

void vtx_flt_lerp_ex(float* vertices, const float fval, const float t,
        const size_t floats, const size_t offset, const size_t stride)
{
    vtx_vec_lerp_ex(vertices, &fval, t, floats, offset, stride, 1);
}

void vtx_vec2lerp_ex(float* vertices, const float v[2], const float t,
        const size_t floats, const size_t offset, const size_t stride)
{
    vtx_vec_lerp_ex(vertices, v, t, floats, offset, stride, 2);
}

void vtx_vec3lerp_ex(float* vertices, const float v[3], const float t,
        const size_t floats, const size_t offset, const size_t stride)
{
    vtx_vec_lerp_ex(vertices, v, t, floats, offset, stride, 3);
}

void vtx_vec4lerp_ex(float* vertices, const float v[4], const float t,
        const size_t floats, const size_t offset, const size_t stride)
{
    vtx_vec_lerp_ex(vertices, v, t, floats, offset, stride, 4);
}

void vtx_flt_lerp(float* verts, const float fval, const float t, const size_t floats)
{
    vtx_flt_lerp_ex(verts, fval, t, floats, 0, 1);
}

void vtx_vec2lerp(float* verts, const float v[2], const float t, const size_t floats)
{
    vtx_vec2lerp_ex(verts, v, t, floats, 0, 2);
}

void vtx_vec3lerp(float* verts, const float v[3], const float t, const size_t floats)
{
    vtx_vec3lerp_ex(verts, v, t, floats, 0, 3);
}

void vtx_vec4lerp(float* verts, const float v[4], const float t, const size_t floats)
{
    vtx_vec4lerp_ex(verts, v, t, floats, 0, 4);
}

/*
================================================================================
 * ~~ [ array copy ] ~~ *
--------------------------------------------------------------------------------
*/

void vtx_copy_ex(float* dst, const float * const src, // strided element copy
    const size_t dst_floats, const size_t dst_offset, const size_t dst_stride,
    const size_t src_floats, const size_t src_offset, const size_t src_stride,
    const size_t length)
{
    AE_PROFILE_ENTER();

    float* d_ptr = (float*)dst + dst_offset;
    float* s_ptr = (float*)src + src_offset;
    float* end_p = (float*)dst + dst_floats;

    ae_assert(dst_offset + length <= dst_stride,"d offset %u, length %u, d stride %u",
            (unsigned int)dst_offset, (unsigned int)length, (unsigned int)dst_stride);

    ae_assert(src_offset + length <= src_stride,"s offset %u, length %u, s stride %u",
            (unsigned int)src_offset, (unsigned int)length, (unsigned int)src_stride);

    ae_assert(dst_floats % dst_stride == 0, "dst count %u dst stride %u",
                    (unsigned int)dst_floats, (unsigned int)dst_stride);

    ae_assert(src_floats % src_stride == 0, "src count %u src stride %u",
                    (unsigned int)src_floats, (unsigned int)src_stride);

    // TODO: break asserts out into macro (shared with others), fast paths
    #if defined(AE_DEBUG)
    {
        const size_t d_count = dst_floats / dst_stride;
        const size_t s_count = src_floats / src_stride;

        ae_assert(d_count == s_count, "%u %u", (u32)d_count, (u32)s_count);
    }
    #endif

    for (; d_ptr < end_p; d_ptr += dst_stride, s_ptr += src_stride)
    {
        vec_copy(d_ptr, s_ptr, length);
    }

    AE_PROFILE_LEAVE();
}

void vtx_copy(float* dst, const float* const src, const size_t floats)
{
    AE_PROFILE_ENTER(); vec_copy(dst, src, floats);
    AE_PROFILE_LEAVE();
}

/*
================================================================================
 * ~~ [ array epsilon compare ] ~~ *
--------------------------------------------------------------------------------
*/

int vtx_cmp_e_ex(const float* const a, const float* const b, const float ev,
        const size_t a_floats, const size_t a_offset, const size_t a_stride,
        const size_t b_floats, const size_t b_offset, const size_t b_stride,
        const size_t length)
{
    AE_PROFILE_ENTER();

    float* a_ptr = (float*)a + a_offset;
    float* b_ptr = (float*)b + b_offset;
    float* end_p = (float*)a + a_floats;

    ae_assert(a_offset + length <= a_stride, "a offset %u, length %u, a stride %u",
            (unsigned int)a_offset, (unsigned int)length, (unsigned int)a_stride);

    ae_assert(b_offset + length <= b_stride, "b offset %u, length %u, b stride %u",
            (unsigned int)b_offset, (unsigned int)length, (unsigned int)b_stride);

    ae_assert(a_floats % a_stride == 0, "a count %u a stride %u",
                (unsigned int)a_floats, (unsigned int)a_stride);

    ae_assert(b_floats % b_stride == 0, "b count %u b stride %u",
                (unsigned int)b_floats, (unsigned int)b_stride);

    // TODO: break asserts out into macro (shared with others), fast paths
    #if defined(AE_DEBUG)
    {
        const size_t a_count = a_floats / a_stride;
        const size_t b_count = b_floats / b_stride;

        ae_assert(a_count == b_count, "%u %u", (u32)a_count, (u32)b_count);
    }
    #endif

    for (; a_ptr < end_p; a_ptr += a_stride, b_ptr += b_stride)
    {
        if (!vec_cmp_e(a_ptr, b_ptr, ev, length))
        {
            AE_PROFILE_LEAVE(); return 0;
        }
    }

    AE_PROFILE_LEAVE(); return 1;
}

int vtx_cmp_e(const float * const a, const float* const b,
                const float epsilon, const size_t floats)
{
    AE_PROFILE_ENTER();
#if 0
    if (!vec_cmp_e(a, b, epsilon, floats))
    {
        AE_PROFILE_LEAVE(); return 0;
    }
#else
    float* end_p = (float*)a + floats;
    float* a_ptr = (float*)a;
    float* b_ptr = (float*)b;

    #if defined(__SSE__)
    if(ae_cpuinfo_sse())
    {
        static const __m128 neg_sign_mask = { -0.0f, -0.0f, -0.0f, -0.0f };

        float* u_ptr = a_ptr + (floats - (floats % 4));
        const __m128 e4 = _mm_set1_ps(epsilon);

        if (num_is_aligned((size_t)a, 16) && num_is_aligned((size_t)b, 16))
        {
            for (; a_ptr < u_ptr; a_ptr += 4, b_ptr += 4)
            {
                const __m128 a4 = _mm_load_ps(a_ptr);
                const __m128 b4 = _mm_load_ps(b_ptr);
                const __m128 s4 = _mm_sub_ps(a4, b4);

                const __m128 n4 = _mm_andnot_ps(neg_sign_mask, s4);
                const __m128 c4 = _mm_cmpgt_ps(n4, e4);

                if (_mm_movemask_ps(c4)) { AE_PROFILE_LEAVE(); return 0; }
            }
        }
        else
        {
            for (; a_ptr < u_ptr; a_ptr += 4, b_ptr += 4)
            {
                const __m128 a4 = _mm_loadu_ps(a_ptr);
                const __m128 b4 = _mm_loadu_ps(b_ptr);
                const __m128 s4 = _mm_sub_ps(a4, b4);

                const __m128 n4 = _mm_andnot_ps(neg_sign_mask, s4);
                const __m128 c4 = _mm_cmpgt_ps(n4, e4);

                if (_mm_movemask_ps(c4)) { AE_PROFILE_LEAVE(); return 0; }
            }
        }
    }
    #endif

    while (a_ptr < end_p)
    {
        if (ae_fabsf(*a_ptr++ - *b_ptr++) > epsilon)
        {
            AE_PROFILE_LEAVE(); return 0;
        }
    }
#endif
    AE_PROFILE_LEAVE(); return 1;
}

/*
================================================================================
 * ~~ [ array add ] ~~ *
--------------------------------------------------------------------------------
*/

void vtx_add_ex( float* d, const float * const a, const float * const b,
    const size_t d_floats, const size_t d_offset, const size_t d_stride,
    const size_t a_floats, const size_t a_offset, const size_t a_stride,
    const size_t b_floats, const size_t b_offset, const size_t b_stride,
    const size_t length)
{
    AE_PROFILE_ENTER();

    float* a_ptr = (float*)a + a_offset;
    float* b_ptr = (float*)b + b_offset;

    float* d_ptr = (float*)d + d_offset;
    float* end_p = (float*)d + d_floats;

    ae_assert(d_offset + length <= d_stride,"d offset %u, length %u, d stride %u",
            (unsigned int)d_offset, (unsigned int)length, (unsigned int)d_stride);

    ae_assert(a_offset + length <= a_stride,"a offset %u, length %u, a stride %u",
            (unsigned int)a_offset, (unsigned int)length, (unsigned int)a_stride);

    ae_assert(b_offset + length <= b_stride,"b offset %u, length %u, b stride %u",
            (unsigned int)b_offset, (unsigned int)length, (unsigned int)b_stride);

    ae_assert(d_floats % d_stride == 0, "%u %u", (u32)d_floats, (u32)d_stride);
    ae_assert(a_floats % a_stride == 0, "%u %u", (u32)a_floats, (u32)a_stride);
    ae_assert(b_floats % b_stride == 0, "%u %u", (u32)b_floats, (u32)b_stride);

    // TODO: break asserts out into macro (shared with others), fast length cases
    #if defined(AE_DEBUG)
    {
        const size_t d_count = d_floats / d_stride;
        const size_t a_count = a_floats / a_stride;
        const size_t b_count = b_floats / b_stride;

        ae_assert(d_count == a_count && a_count == b_count, "D: %u A: %u B: %u!",
            (unsigned int)d_count, (unsigned int)a_count, (unsigned int)b_count);
    }
    #endif

    for (; d_ptr < end_p; d_ptr += d_stride, a_ptr += a_stride, b_ptr += b_stride)
    {
        vec_add_vec(d_ptr, a_ptr, b_ptr, length);
    }

    AE_PROFILE_LEAVE();
}

void vtx_add(float* out, const float* const a,
    const float* const b, const size_t floats)
{
    AE_PROFILE_ENTER();
#if 0
    vec_add_vec(out, a, b, floats);
#else
    float * dp = (float *)out;
    float * ap = (float *)a;
    float * bp = (float *)b;
    float * end = dp + floats;

    #if defined(__SSE__)
    if (ae_cpuinfo_sse() && num_is_aligned((size_t)dp, 16) &&
                            num_is_aligned((size_t)ap, 16) &&
                            num_is_aligned((size_t)bp, 16))
    {
        float* ur = dp + (floats - (floats % 4));

        for (; dp < ur; dp += 4, ap += 4, bp += 4)
        {
            const __m128 a4 = _mm_load_ps(ap);
            const __m128 b4 = _mm_load_ps(bp);

            const __m128 rs = _mm_add_ps(a4, b4);
            _mm_store_ps(dp, rs);
        }
    }
    #endif
    while (dp < end) { *dp++ = *ap++ + *bp++; }
#endif
    AE_PROFILE_LEAVE();
}

/*
================================================================================
 * ~~ [ array mul ] ~~ *
--------------------------------------------------------------------------------
*/

void vtx_mul_ex( float* d, const float * const a, const float * const b,
    const size_t d_floats, const size_t d_offset, const size_t d_stride,
    const size_t a_floats, const size_t a_offset, const size_t a_stride,
    const size_t b_floats, const size_t b_offset, const size_t b_stride,
    const size_t length)
{
    AE_PROFILE_ENTER();

    float* a_ptr = (float*)a + a_offset;
    float* b_ptr = (float*)b + b_offset;

    float* d_ptr = (float*)d + d_offset;
    float* end_p = (float*)d + d_floats;

    ae_assert(d_offset + length <= d_stride,"d offset %u, length %u, d stride %u",
            (unsigned int)d_offset, (unsigned int)length, (unsigned int)d_stride);

    ae_assert(a_offset + length <= a_stride,"a offset %u, length %u, a stride %u",
            (unsigned int)a_offset, (unsigned int)length, (unsigned int)a_stride);

    ae_assert(b_offset + length <= b_stride,"b offset %u, length %u, b stride %u",
            (unsigned int)b_offset, (unsigned int)length, (unsigned int)b_stride);

    ae_assert(d_floats % d_stride == 0, "%u %u", (u32)d_floats, (u32)d_stride);
    ae_assert(a_floats % a_stride == 0, "%u %u", (u32)a_floats, (u32)a_stride);
    ae_assert(b_floats % b_stride == 0, "%u %u", (u32)b_floats, (u32)b_stride);

    // TODO: break asserts out into macro (shared with others), fast length cases
    #if defined(AE_DEBUG)
    {
        const size_t d_count = d_floats / d_stride;
        const size_t a_count = a_floats / a_stride;
        const size_t b_count = b_floats / b_stride;

        ae_assert(d_count == a_count && a_count == b_count, "D: %u A: %u B: %u!",
            (unsigned int)d_count, (unsigned int)a_count, (unsigned int)b_count);
    }
    #endif

    for (; d_ptr < end_p; d_ptr += d_stride, a_ptr += a_stride, b_ptr += b_stride)
    {
        vec_mul_vec(d_ptr, a_ptr, b_ptr, length);
    }

    AE_PROFILE_LEAVE();
}

void vtx_mul(float* out, const float* const a,
    const float* const b, const size_t floats)
{
    AE_PROFILE_ENTER();
#if 0
    vec_mul_vec(out, a, b, floats);
#else
    float * dp = (float *)out;
    float * ap = (float *)a;
    float * bp = (float *)b;
    float * end = dp + floats;

    #if defined(__SSE__)
    if (ae_cpuinfo_sse() && num_is_aligned((size_t)dp, 16) &&
                            num_is_aligned((size_t)ap, 16) &&
                            num_is_aligned((size_t)bp, 16))
    {
        float* ur = dp + (floats - (floats % 4));

        for (; dp < ur; dp += 4, ap += 4, bp += 4)
        {
            const __m128 a4 = _mm_load_ps(ap);
            const __m128 b4 = _mm_load_ps(bp);

            const __m128 rs = _mm_mul_ps(a4, b4);
            _mm_store_ps(dp, rs);
        }
    }
    #endif
    while (dp < end) { *dp++ = *ap++ * *bp++; }
#endif
    AE_PROFILE_LEAVE();
}

/*
================================================================================
 * ~~ [ array mul-add ] ~~ *
--------------------------------------------------------------------------------
*/

void vtx_madd_ex(
    float* d, const float* const a, const float* const b, const float s,
    const size_t d_floats, const size_t d_offset, const size_t d_stride,
    const size_t a_floats, const size_t a_offset, const size_t a_stride,
    const size_t b_floats, const size_t b_offset, const size_t b_stride,
    const size_t length)
{
    AE_PROFILE_ENTER();

    float* a_ptr = (float*)a + a_offset;
    float* b_ptr = (float*)b + b_offset;

    float* d_ptr = (float*)d + d_offset;
    float* end_p = (float*)d + d_floats;

    ae_assert(d_offset + length <= d_stride,"d offset %u, length %u, d stride %u",
            (unsigned int)d_offset, (unsigned int)length, (unsigned int)d_stride);

    ae_assert(a_offset + length <= a_stride,"a offset %u, length %u, a stride %u",
            (unsigned int)a_offset, (unsigned int)length, (unsigned int)a_stride);

    ae_assert(b_offset + length <= b_stride,"b offset %u, length %u, b stride %u",
            (unsigned int)b_offset, (unsigned int)length, (unsigned int)b_stride);

    ae_assert(d_floats % d_stride == 0, "%u %u", (u32)d_floats, (u32)d_stride);
    ae_assert(a_floats % a_stride == 0, "%u %u", (u32)a_floats, (u32)a_stride);
    ae_assert(b_floats % b_stride == 0, "%u %u", (u32)b_floats, (u32)b_stride);

    // TODO: break asserts out into macro (shared with others), fast length cases
    #if defined(AE_DEBUG)
    {
        const size_t d_count = d_floats / d_stride;
        const size_t a_count = a_floats / a_stride;
        const size_t b_count = b_floats / b_stride;

        ae_assert(d_count == a_count && a_count == b_count, "D: %u A: %u B: %u!",
            (unsigned int)d_count, (unsigned int)a_count, (unsigned int)b_count);
    }
    #endif

    for (; d_ptr < end_p; d_ptr += d_stride, a_ptr += a_stride, b_ptr += b_stride)
    {
        vec_madd(d_ptr, a_ptr, b_ptr, s, length);
    }

    AE_PROFILE_LEAVE();
}

void vtx_madd(float* out, const float* const a, const float* const b,
                                const float s, const size_t floats)
{
    AE_PROFILE_ENTER();
#if 0
    vec_madd(out, a, b, s, floats);
#else
    float * dp = (float *)out;
    float * ap = (float *)a;
    float * bp = (float *)b;
    float * end = dp + floats;

    #if defined(__SSE__)
    if (ae_cpuinfo_sse() && num_is_aligned((size_t)dp, 16) &&
                            num_is_aligned((size_t)ap, 16) &&
                            num_is_aligned((size_t)bp, 16))
    {
        float * ur = dp + (floats - (floats % 4));
        const __m128 s4 = _mm_set1_ps(s);

        for (; dp < ur; dp += 4, ap += 4, bp += 4)
        {
            const __m128 a4 = _mm_load_ps(ap);
            const __m128 b4 = _mm_load_ps(bp);

            const __m128 c4 = _mm_mul_ps(s4, b4);
            const __m128 d4 = _mm_add_ps(a4, c4);

            _mm_store_ps(dp, d4);
        }
    }
    #endif
    while (dp < end) { *dp++ = *ap++ + s * *bp++; }
#endif
    AE_PROFILE_LEAVE();
}

/*
================================================================================
 * ~~ [ array lerp ] ~~ *
--------------------------------------------------------------------------------
*/

void vtx_lerp_ex(
    float* d, const float* const a, const float* const b, const float t,
    const size_t d_floats, const size_t d_offset, const size_t d_stride,
    const size_t a_floats, const size_t a_offset, const size_t a_stride,
    const size_t b_floats, const size_t b_offset, const size_t b_stride,
    const size_t length)
{
    AE_PROFILE_ENTER();

    float* a_ptr = (float*)a + a_offset;
    float* b_ptr = (float*)b + b_offset;

    float* d_ptr = (float*)d + d_offset;
    float* end_p = (float*)d + d_floats;

    ae_assert(d_offset + length <= d_stride,"d offset %u, length %u, d stride %u",
            (unsigned int)d_offset, (unsigned int)length, (unsigned int)d_stride);

    ae_assert(a_offset + length <= a_stride,"a offset %u, length %u, a stride %u",
            (unsigned int)a_offset, (unsigned int)length, (unsigned int)a_stride);

    ae_assert(b_offset + length <= b_stride,"b offset %u, length %u, b stride %u",
            (unsigned int)b_offset, (unsigned int)length, (unsigned int)b_stride);

    ae_assert(d_floats % d_stride == 0, "%u %u", (u32)d_floats, (u32)d_stride);
    ae_assert(a_floats % a_stride == 0, "%u %u", (u32)a_floats, (u32)a_stride);
    ae_assert(b_floats % b_stride == 0, "%u %u", (u32)b_floats, (u32)b_stride);

    // TODO: break asserts out into macro (shared with others), fast length cases
    #if defined(AE_DEBUG)
    {
        const size_t d_count = d_floats / d_stride;
        const size_t a_count = a_floats / a_stride;
        const size_t b_count = b_floats / b_stride;

        ae_assert(d_count == a_count && a_count == b_count, "D: %u A: %u B: %u!",
            (unsigned int)d_count, (unsigned int)a_count, (unsigned int)b_count);
    }
    #endif

    for (; d_ptr < end_p; d_ptr += d_stride, a_ptr += a_stride, b_ptr += b_stride)
    {
        vec_lerp(d_ptr, a_ptr, b_ptr, t, length);
    }

    AE_PROFILE_LEAVE();
}

void vtx_lerp(float* out, const float* const a, const float* const b,
                                const float t, const size_t floats)
{
    AE_PROFILE_ENTER();
#if 0
    vec_lerp(out, a, b, t, floats);
#else
    const float s = 1.0f - t;

    float * dp = (float *)out;
    float * ap = (float *)a;
    float * bp = (float *)b;
    float * end = dp + floats;

    #if defined(__SSE__)
    if (ae_cpuinfo_sse() && num_is_aligned((size_t)dp, 16) &&
                            num_is_aligned((size_t)ap, 16) &&
                            num_is_aligned((size_t)bp, 16))
    {
        float* ur = dp + (floats - (floats % 4));

        const __m128 s4 = _mm_set1_ps(s);
        const __m128 t4 = _mm_set1_ps(t);

        for (; dp < ur; dp += 4, ap += 4, bp += 4)
        {
            const __m128 a4 = _mm_load_ps(ap);
            const __m128 b4 = _mm_load_ps(bp);

            const __m128 l4 = _mm_mul_ps(s4, a4);
            const __m128 r4 = _mm_mul_ps(t4, b4);

            const __m128 rs = _mm_add_ps(l4, r4);
            _mm_store_ps(dp, rs);
        }
    }
    #endif
    while (dp < end) { *dp++ = s * *ap++ + t * *bp++; }
#endif
    AE_PROFILE_LEAVE();
}

/*
================================================================================
 * ~~ [ mat4x4 mul ] ~~ *
--------------------------------------------------------------------------------
*/

void vtx_mat4x4row_mul_point3_ex(float* vertices, const float mat[16],
        const size_t floats, const size_t offset, const size_t stride)
{
    AE_PROFILE_ENTER(); // TODO: this is a first draft. optimize it!!!

    float* ptr = vertices + offset;
    float* end = vertices + floats;

    ae_assert(floats % stride == 0, "%u %u", (u32)floats, (u32)stride);

    ae_assert(offset + 3 <= stride, "offset %u, stride %u",
            (unsigned int) offset, (unsigned int) stride );

    for (; ptr < end; ptr += stride)
    {
        mat4x4row_mul_point3_safe(ptr, mat, ptr);
    }

    AE_PROFILE_LEAVE();
}

void vtx_mat4x4row_mul_point3(float* v, const float m[16], const size_t n)
{
    vtx_mat4x4row_mul_point3_ex(v, m, n, 0, 3);
}

void vtx_mat4x4row_mul_vector3_ex( float * vertices, const float mat[16],
                                const int normalize, const size_t floats,
                                const size_t offset, const size_t stride)
{
    AE_PROFILE_ENTER(); // TODO: this is a first draft. optimize it!!!

    float* ptr = vertices + offset;
    float* end = vertices + floats;

    // NOTE: for mesh normals, we must take the transpose of the inverse
    float inv[16];

    if (mat4x4inverse(inv, mat)) { mat4x4transpose(inv); } else
    {
        // TODO the information in the null space is lost (zero eigenvalues),
        // can we at least recover info in co-image space by pseudo-inverse?
        AE_WARN("singular 4x4 xform matrix"); AE_PROFILE_LEAVE(); return;
    }

    ae_assert(floats % stride == 0, "%u %u", (u32)floats, (u32)stride);

    ae_assert(offset + 3 <= stride, "offset %u, stride %u",
            (unsigned int) offset, (unsigned int) stride );

    ae_if (normalize)
    {
        for (; ptr < end; ptr += stride)
        {
            mat4x4row_mul_vector3_safe(ptr, inv, ptr);
            vec3normalize(ptr, ptr);
        }
    }
    else
    {
        for (; ptr < end; ptr += stride)
        {
            mat4x4row_mul_vector3_safe(ptr, inv, ptr);
        }
    }

    AE_PROFILE_LEAVE();
}

void vtx_mat4x4row_mul_vector3( float* vertices, const float mat[16],
                            const int normalize, const size_t floats)
{
    vtx_mat4x4row_mul_vector3_ex(vertices, mat, normalize, floats, 0, 3);
}

/*
================================================================================
 * ~~ [ vertex welding ] ~~ *
--------------------------------------------------------------------------------
NOTE: Based on NVIDIA Mesh Tools welder (nvmesh/weld/Weld.h) by Ignacio Castaño.
--------------------------------------------------------------------------------
*/

void vtx_weld_func(ae_array_t* vertices, ae_vertex_format_t vtx_format, // shrinks
    ae_array_t* indices, ae_index_type_t idx_type, vtx_weld_func_t fn, void* data)
{
    AE_PROFILE_ENTER();

    const size_t vertex_size = ae_vertex_format_size[vtx_format]; // v floats
    const size_t index_count = vertices->size / (sizeof(float) * vertex_size);

    size_t new_vertex_count;

    ae_assert(vertices->size != 0, "vert array can't be empty for vertex welding");
    ae_assert(indices->size == 0, "index buffer must be empty for vertex welding");

    // the new index buffer takes the size of the old unpacked vertex buffer
    ae_array_resize(indices, index_count * ae_index_type_bytes[idx_type]);

    new_vertex_count = vtx_weld_func_ex((float *)vertices->data, vtx_format,
                            indices->data, idx_type, index_count, fn, data);

    ae_array_resize(vertices, new_vertex_count * vertex_size * sizeof(float));
    ae_array_trim(vertices);

    AE_PROFILE_LEAVE();
}

static size_t vtx_weld_func_u32(float * vertices, ae_vertex_format_t vertex_format,
                u32* indices, size_t index_count, vtx_weld_func_t func, void* data)
{
    AE_PROFILE_ENTER(); size_t new_vertex_count = 0;
    #define NIL 0xFFFFFFFF // empty hash table slot

    const size_t vertex_size = ae_vertex_format_size[vertex_format];
    const size_t vertex_bytes = vertex_size * sizeof(float);

    // space for the hash table, and linked list of visited on bottom
    const size_t hash_size = num_next_pow2(index_count);
    const size_t hash_bytes = (hash_size + index_count) * sizeof(u32);

    u32 *hash_table, *next;
    size_t i;

    hash_table = (u32*)ae_stack_malloc(ae_global_stack(), hash_bytes);

    next = hash_table + hash_size; // init hash table
    memset(hash_table, NIL, hash_size * sizeof(u32));

    for (i = 0; i < index_count; i++)
    {
        float* vertex = vertices + i * vertex_size;

        u32 v_hash = ae_hash_mem(vertex, vertex_bytes) & (hash_size - 1);
        u32 offset = hash_table[v_hash];

        while (offset != NIL && !func(vertices + offset * vertex_size, vertex, data))
        {
            offset = next[offset]; // traverse list of visited verts
        }

        indices[i] = offset;

        ae_if (offset == NIL) // no match, copy vertex and add to hash
        {
            // save xref
            indices[i] = new_vertex_count;

            // copy vertex
            memcpy(vertices + new_vertex_count * vertex_size, vertex, vertex_bytes);

            // link to hash table
            next[new_vertex_count] = hash_table[v_hash];

            // update hash heads and increase output counter
            hash_table[v_hash] = new_vertex_count++;
        }
    }

    ae_stack_free(ae_global_stack(), hash_table, hash_bytes);
    AE_PROFILE_LEAVE(); return new_vertex_count;

    #undef NIL
}

static size_t vtx_weld_func_u16(float * vertices, ae_vertex_format_t vertex_format,
                u16* indices, size_t index_count, vtx_weld_func_t func, void* data)
{
    AE_PROFILE_ENTER(); // weld into u32 temp buffer, convert into indices, and free
    u32* temp = (u32*)ae_stack_malloc(ae_global_stack(), index_count * sizeof(u32));

    size_t ret = vtx_weld_func_u32(vertices, vertex_format,
                            temp, index_count, func, data);

    idx_type_converted_ex( temp, AE_INDEX_TYPE_U32, indices,
                            AE_INDEX_TYPE_U16, index_count);

    ae_stack_free(ae_global_stack(), temp, index_count * sizeof(u32));
    AE_PROFILE_LEAVE(); return ret;
}

static size_t vtx_weld_func_u8(float * vertices, ae_vertex_format_t vertex_format,
                u8* indices, size_t index_count, vtx_weld_func_t func, void* data)
{
    AE_PROFILE_ENTER(); // weld into u32 temp buffer, convert into indices, and free
    u32* temp = (u32*)ae_stack_malloc(ae_global_stack(), index_count * sizeof(u32));

    size_t ret = vtx_weld_func_u32(vertices, vertex_format,
                            temp, index_count, func, data);

    idx_type_converted_ex(temp, AE_INDEX_TYPE_U32, indices,
                            AE_INDEX_TYPE_U8, index_count);

    ae_stack_free(ae_global_stack(), temp, index_count * sizeof(u32));
    AE_PROFILE_LEAVE(); return ret;
}

size_t vtx_weld_func_ex(float* vertices, ae_vertex_format_t vertex_format,
            void* indices, ae_index_type_t index_type, size_t index_count,
            vtx_weld_func_t func, void* data) // returns new vertex count
{
    // idx_identity(indices, index_type, index_count); return index_count;

    ae_switch (index_type, ae_index_type, AE_INDEX_TYPE, suffix)
    {
        case AE_INDEX_TYPE_U8:  return vtx_weld_func_u8 ( vertices,
            vertex_format, (u8 *)indices, index_count, func, data);

        case AE_INDEX_TYPE_U16: return vtx_weld_func_u16( vertices,
            vertex_format, (u16*)indices, index_count, func, data);

        case AE_INDEX_TYPE_U32: return vtx_weld_func_u32( vertices,
            vertex_format, (u32*)indices, index_count, func, data);

        default: break;
    }

    assert(0); return 0;
}

/* ===== [ epsilon comparison ] ============================================= */

typedef struct vtx_weld_args_t
{
    size_t vertex_size;
    float epsilon;
} \
    vtx_weld_args_t;

static int vtx_weld_cmp_e(float* a, float* b, void* data)
{
    /* NOTE using vec_cmp_e here instead of the identically-behaving vtx_cmp_e,
     * as the latter trades higher latency (per-call overhead) for throughput.
     * I also don't want to see nine hundred zillion tiny calls in the profile.
     */
    vtx_weld_args_t * args = (vtx_weld_args_t *)data; // args
    return vec_cmp_e(a, b, args->epsilon, args->vertex_size);
}

void vtx_weld(ae_array_t* vertices, ae_vertex_format_t vtx_format,
    ae_array_t* indices, ae_index_type_t idx_type, float epsilon)
{
    AE_PROFILE_ENTER();

    vtx_weld_args_t args = { ae_vertex_format_size[vtx_format], epsilon }; // data
    vtx_weld_func(vertices, vtx_format, indices, idx_type, vtx_weld_cmp_e, &args);

    AE_PROFILE_LEAVE();
}

size_t vtx_weld_ex(float* vertices, ae_vertex_format_t vtx_format, void* indices,
                    ae_index_type_t idx_type, size_t index_count, float epsilon)
{
    AE_PROFILE_ENTER(); // weld with sized buffers, returns new vert count
    vtx_weld_args_t args = { ae_vertex_format_size[vtx_format], epsilon };

    size_t ret_v = vtx_weld_func_ex(vertices, vtx_format, indices,
                    idx_type, index_count, vtx_weld_cmp_e, &args);

    AE_PROFILE_LEAVE(); return ret_v;
}

/* ===== [ distance comparison ] ============================================ */

typedef struct vtx_weld_dist2_args_t
{
    size_t offset; // position index
    float threshold; // dist squared
} \
    vtx_weld_dist2_args_t;

static int vtx_weld_cmp_dist2(float* a, float* b, void* data)
{
    vtx_weld_dist2_args_t* args = (vtx_weld_dist2_args_t*)data;
    const size_t offset = args->offset;

    return vec3dist2(&a[offset], &b[offset]) < args->threshold;
}

void vtx_weld_dist2(ae_array_t* vertices, ae_vertex_format_t vtx_format,
        ae_array_t* indices, ae_index_type_t idx_type, float threshold2)
{
    AE_PROFILE_ENTER();

    vtx_weld_dist2_args_t args =
    {
        ae_vertex_format_position_offset[vtx_format], threshold2
    };

    ae_assert(ae_vertex_format_position_size[vtx_format] == 3,
        "bad format: %s!", ae_vertex_format_name[vtx_format]);

    vtx_weld_func( vertices, vtx_format, indices,
            idx_type, vtx_weld_cmp_dist2, &args);

    AE_PROFILE_LEAVE();
}

size_t vtx_weld_dist2_ex(float * vertices, ae_vertex_format_t vertex_format,
    void* idx, ae_index_type_t idx_type, size_t idx_count, float threshold2)
{
    AE_PROFILE_ENTER(); size_t retv;

    vtx_weld_dist2_args_t args =
    {
        ae_vertex_format_position_offset[vertex_format], threshold2
    };

    ae_assert(ae_vertex_format_position_size[vertex_format] == 3,
        "bad format: %s!", ae_vertex_format_name[vertex_format]);

    retv = vtx_weld_func_ex(vertices, vertex_format, idx, idx_type,
                            idx_count, vtx_weld_cmp_dist2, &args);

    AE_PROFILE_LEAVE(); return retv;
}

/*
================================================================================
 * ~~ [ vertex unwelding ] ~~ *
--------------------------------------------------------------------------------
*/

void vtx_unweld(ae_array_t* vertices, ae_vertex_format_t format,
                ae_array_t* indices, ae_index_type_t index_type)
{
    AE_PROFILE_ENTER();

    const size_t idx_count = indices->size / ae_index_type_bytes[index_type];
    const size_t flt_count = idx_count * ae_vertex_format_size[format];

    ae_array_t unpacked;
    memset(&unpacked, 0, sizeof(ae_array_t));

    ae_assert(indices->size != 0, "index buffer must have data for unweld!");
    ae_array_resize(&unpacked, flt_count * sizeof(float));

    vtx_unweld_ex((float *)vertices->data, (float *)unpacked.data,
                    format, indices->data, index_type, idx_count);

    ae_array_free(vertices); ae_array_free(indices);
    *vertices = unpacked; // free and swap contents

    AE_PROFILE_LEAVE();
}

#define VTX_UNWELD(IDX)                                                     \
{                                                                           \
    AE_PROFILE_ENTER();                                                     \
                                                                            \
    const size_t vertex_size = ae_vertex_format_size[vertex_format];        \
    const size_t vertex_bytes = vertex_size * sizeof(float);                \
                                                                            \
    float* end = unpacked + index_count * vertex_size; /* sentinel */       \
    for (; unpacked < end; unpacked += vertex_size, indices++)              \
    {                                                                       \
        const IDX i = *indices;                                             \
        ae_assert(i < index_count, "index value out of range: %u", (u32)i); \
                                                                            \
        memcpy(unpacked, packed + i * vertex_size, vertex_bytes);           \
    }                                                                       \
                                                                            \
    AE_PROFILE_LEAVE();                                                     \
}

static void vtx_unweld_u32(float* packed, float* unpacked, // u32 unpacking
        ae_vertex_format_t vertex_format, u32* indices, size_t index_count)
{
    VTX_UNWELD(u32);
}

static void vtx_unweld_u16(float* packed, float* unpacked, // u16 unpacking
        ae_vertex_format_t vertex_format, u16* indices, size_t index_count)
{
    VTX_UNWELD(u16);
}

static void vtx_unweld_u8 (float* packed, float* unpacked, // u8 unpacking
        ae_vertex_format_t vertex_format, u8* indices, size_t index_count)
{
    VTX_UNWELD(u8);
}

void vtx_unweld_ex(float* packed, float* unpacked, ae_vertex_format_t vtx_format,
                    void* indices, ae_index_type_t index_type, size_t idx_count)
{
    AE_PROFILE_ENTER();

    ae_switch (index_type, ae_index_type, AE_INDEX_TYPE, suffix)
    {
        case AE_INDEX_TYPE_U8:
        {
            vtx_unweld_u8 (packed, unpacked, vtx_format, (u8 *)indices, idx_count);
        }
        break;

        case AE_INDEX_TYPE_U16:
        {
            vtx_unweld_u16(packed, unpacked, vtx_format, (u16*)indices, idx_count);
        }
        break;

        case AE_INDEX_TYPE_U32:
        {
            vtx_unweld_u32(packed, unpacked, vtx_format, (u32*)indices, idx_count);
        }
        break;

        default: assert(0); break;
    }

    AE_PROFILE_LEAVE();
}
