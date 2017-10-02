/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

/* TODO: triangle stripper (define mesh structure with inter-connectivity data?)
 * TODO: polygon and quad triangulation (allow selection of polygon algorithm)
 * TODO: triangle mesh generation API with spheres, cubes, toroidal shapes, etc.
 * TODO: smooth and faceted normal generation (facet requires raw tri vertices)
 */
void ae_triangle_init(int argc, char** argv) {}
void ae_triangle_quit(void) {}

/*
================================================================================
 * ~~ [ depth sorting ] ~~ *
--------------------------------------------------------------------------------
TODO: use std::sort in C++ builds for inlining, eventually use some custom sort
--------------------------------------------------------------------------------
*/

static float* g_depthsort_point;
static float* g_depthsort_vertices;
static size_t g_depthsort_offset;
static size_t g_depthsort_stride;

#define TRI3_MESH_DEPTHSORT(TYPE) do                                            \
{                                                                               \
    /* TODO: would vertex 0 work as well as the tri center for a sort point? */ \
    const TYPE* const a = (const TYPE* const)p1;                                \
    const TYPE* const b = (const TYPE* const)p2;                                \
                                                                                \
    float a_distance, b_distance, t[3];                                         \
                                                                                \
    tri3centroid(t,                                                             \
        &g_depthsort_vertices[a[0] * g_depthsort_stride + g_depthsort_offset],  \
        &g_depthsort_vertices[a[1] * g_depthsort_stride + g_depthsort_offset],  \
        &g_depthsort_vertices[a[2] * g_depthsort_stride + g_depthsort_offset]); \
                                                                                \
    vec3sub_vec(t, t, g_depthsort_point); a_distance = vec3mag2(t);             \
                                                                                \
    tri3centroid(t,                                                             \
        &g_depthsort_vertices[b[0] * g_depthsort_stride + g_depthsort_offset],  \
        &g_depthsort_vertices[b[1] * g_depthsort_stride + g_depthsort_offset],  \
        &g_depthsort_vertices[b[2] * g_depthsort_stride + g_depthsort_offset]); \
                                                                                \
    vec3sub_vec(t, t, g_depthsort_point); b_distance = vec3mag2(t);             \
                                                                                \
    /* order values in reverse to sort tris from back to front */               \
    return (b_distance > a_distance) - (b_distance < a_distance);               \
}                                                                               \
while (AE_NULL_WHILE_LOOP_CONDITION)

static int tri3_mesh_depthsort_u8 (const void* const p1, const void* const p2)
{
    TRI3_MESH_DEPTHSORT(u8 );
}

static int tri3_mesh_depthsort_u16(const void* const p1, const void* const p2)
{
    TRI3_MESH_DEPTHSORT(u16);
}

static int tri3_mesh_depthsort_u32(const void* const p1, const void* const p2)
{
    TRI3_MESH_DEPTHSORT(u32);
}

static void tri3_mesh_depthsort_idx(const float point[3],

                                    float* vertices,
                                    size_t floats,
                                    size_t offset,
                                    size_t stride,

                                    void * indices,
                                    ae_index_type_t type,
                                    size_t i_count)
{
    AE_PROFILE_ENTER();

    int (* tri_compare)(const void*, const void*);
    size_t tri_bytes;

    /* setup global variables for the qsort comparison function
     * (TODO: custom qsort_r/qsort_s replacement with context).
     */
    g_depthsort_point = (float*)point;
    g_depthsort_vertices = vertices;
    g_depthsort_offset = offset;
    g_depthsort_stride = stride;

    ae_assert(i_count % 3 == 0, "non-triangle index count: %u", (u32)i_count);

    ae_switch (type, ae_index_type, AE_INDEX_TYPE, suffix)
    {
        case AE_INDEX_TYPE_U8:
        {
            tri_compare = tri3_mesh_depthsort_u8;
            tri_bytes = sizeof(u8[3]);
        }
        break;

        case AE_INDEX_TYPE_U16:
        {
            tri_compare = tri3_mesh_depthsort_u16;
            tri_bytes = sizeof(u16[3]);
        }
        break;

        case AE_INDEX_TYPE_U32:
        {
            tri_compare = tri3_mesh_depthsort_u32;
            tri_bytes = sizeof(u32[3]);
        }
        break;

        default:
        {
            ae_assert(0, "%u", (u32)type);

            // keep GCC uninitialized variable warnings quiet
            tri_compare = NULL; tri_bytes = 0;
        }
        break;
    }

    ae_if (!ae_is_sorted_ex(indices, i_count / 3, tri_bytes, tri_compare))
    {
        qsort(indices, i_count / 3, tri_bytes, tri_compare);
    }

    AE_PROFILE_LEAVE();
}

static int tri3_mesh_depthsort_cmp(const void* const p1, const void* const p2)
{
    const float* const a = (const float* const)p1;
    const float* const b = (const float* const)p2;

    float a_distance, b_distance, t[3];

    tri3centroid(t, a + g_depthsort_stride * 0 + g_depthsort_offset,
                    a + g_depthsort_stride * 1 + g_depthsort_offset,
                    a + g_depthsort_stride * 2 + g_depthsort_offset);

    vec3sub_vec(t, t, g_depthsort_point); a_distance = vec3mag2(t);

    tri3centroid(t, b + g_depthsort_stride * 0 + g_depthsort_offset,
                    b + g_depthsort_stride * 1 + g_depthsort_offset,
                    b + g_depthsort_stride * 2 + g_depthsort_offset);

    vec3sub_vec(t, t, g_depthsort_point); b_distance = vec3mag2(t);

    /* order values in reverse to sort tris from back to front */
    return (b_distance > a_distance) - (b_distance < a_distance);
}

static void tri3_mesh_depthsort_raw(const float point[3],
                                    float* vertices,
                                    size_t floats,
                                    size_t offset,
                                    size_t stride)
{
    AE_PROFILE_ENTER();

    const size_t tri_stride = stride * 3; // no indices
    const size_t num_tris = floats / tri_stride;
    const size_t tri_bytes = tri_stride * sizeof(float);

    g_depthsort_point = (float*)point;
    g_depthsort_offset = offset;
    g_depthsort_stride = stride;

    ae_if (!ae_is_sorted_ex(vertices, num_tris, tri_bytes, tri3_mesh_depthsort_cmp))
    {
        qsort(vertices, num_tris, tri_bytes, tri3_mesh_depthsort_cmp);
    }

    AE_PROFILE_LEAVE();
}

void tri3_mesh_depthsort_ex(const float point[3],

                            float* vertices,
                            size_t floats,
                            size_t offset,
                            size_t stride,

                            void * indices,
                            ae_index_type_t type,
                            size_t i_count)
{
    ae_if (i_count != 0)
    {
        tri3_mesh_depthsort_idx(point, vertices, floats, offset, stride, indices, type, i_count);
    }
    else
    {
        tri3_mesh_depthsort_raw(point, vertices, floats, offset, stride);
    }
}

void tri3_mesh_depthsort(   const float point[3],

                            float* vertices,
                            size_t floats,

                            void * indices,
                            ae_index_type_t type,
                            size_t i_count)
{
    tri3_mesh_depthsort_ex(point, vertices, floats, 0, 3, indices, type, i_count);
}

/*
================================================================================
 * ~~ [ raycasting (collision detection) ] ~~ *
--------------------------------------------------------------------------------
*/

int tri3_vs_ray(float ray_pos[3],
                float ray_dir[3],
                float vert0[3],
                float vert1[3],
                float vert2[3],
                float *t,
                float *u,
                float *v,
                float epsilon)
{
    float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3], det, inv_det;

    // find vectors for the two triangle edges sharing the first vertex
    vec3sub_vec(edge1, vert1, vert0);
    vec3sub_vec(edge2, vert2, vert0);

    // begin calculating determinant - also used to calculate U parameter
    vec3cross(pvec, ray_dir, edge2);

    // if determinant is near zero, the ray lies in the triangle's plane
    det = vec3dot(edge1, pvec);

    #if 0
        ae_if (det > -epsilon && det < epsilon) return 0;
    #else
        ae_if (det < epsilon) return 0; // cull back face
    #endif

    inv_det = 1.0f / det;

    // calculate the distance from the first vertex to the ray origin
    vec3sub_vec(tvec, ray_pos, vert0);

    // calculate the U parameter and use it to test ray hit bounds
    *u = vec3dot(tvec, pvec) * inv_det;
    ae_if (*u < 0.0f || *u > 1.0f) return 0;

    // prepare to test the V parameter
    vec3cross(qvec, tvec, edge1);

    // calculate the V parameter and use it to test ray hit bounds
    *v = vec3dot(ray_dir, qvec) * inv_det;
    ae_if (*v < 0.0f || *u + *v > 1.0f) return 0;

    // calculate the T parameter, the ray intersects the triangle
    *t = vec3dot(edge2, qvec) * inv_det;

    // AJS: added to make this an actual ray test, not a line test
    return ae_branch(*t > 0.0f);
}

#define TRI3_MESH_VS_RAY_EX(TYPE) do                                    \
{                                                                       \
    AE_PROFILE_ENTER();                                                 \
                                                                        \
    TYPE* end = indices + i_count;                                      \
    int r_hit = 0;                                                      \
                                                                        \
    for (; indices < end; indices += 3)                                 \
    {                                                                   \
        float temp_t, *v0 = &vertices[indices[0] * stride + offset];    \
        float temp_u, *v1 = &vertices[indices[1] * stride + offset];    \
        float temp_v, *v2 = &vertices[indices[2] * stride + offset];    \
                                                                        \
        if (tri3_vs_ray(ray_pos,                                        \
                        ray_dir,                                        \
                        v0, v1, v2,                                     \
                        &temp_t,                                        \
                        &temp_u,                                        \
                        &temp_v,                                        \
                        epsilon))                                       \
        {                                                               \
            ae_if (!r_hit || temp_t < *t)                               \
            {                                                           \
                r_hit = 1;                                              \
                                                                        \
                hit_indices[0] = (u32)indices[0];                       \
                hit_indices[1] = (u32)indices[1];                       \
                hit_indices[2] = (u32)indices[2];                       \
                                                                        \
                *t = temp_t;                                            \
                *u = temp_u;                                            \
                *v = temp_v;                                            \
            }                                                           \
        }                                                               \
    }                                                                   \
                                                                        \
    AE_PROFILE_LEAVE(); return r_hit;                                   \
}                                                                       \
while (AE_NULL_WHILE_LOOP_CONDITION)

static int tri3_mesh_vs_ray_ex_u8(  float  ray_pos[3],
                                    float  ray_dir[3],
                                    float* vertices,
                                    size_t floats,
                                    size_t offset,
                                    size_t stride,
                                    u8*    indices,
                                    size_t i_count,
                                    u32    hit_indices[3],
                                    float* t,
                                    float* u,
                                    float* v,
                                    float  epsilon)
{
    TRI3_MESH_VS_RAY_EX(u8);
}

static int tri3_mesh_vs_ray_ex_u16( float  ray_pos[3],
                                    float  ray_dir[3],
                                    float* vertices,
                                    size_t floats,
                                    size_t offset,
                                    size_t stride,
                                    u16*   indices,
                                    size_t i_count,
                                    u32    hit_indices[3],
                                    float* t,
                                    float* u,
                                    float* v,
                                    float  epsilon)
{
    TRI3_MESH_VS_RAY_EX(u16);
}

static int tri3_mesh_vs_ray_ex_u32( float  ray_pos[3],
                                    float  ray_dir[3],
                                    float* vertices,
                                    size_t floats,
                                    size_t offset,
                                    size_t stride,
                                    u32*   indices,
                                    size_t i_count,
                                    u32    hit_indices[3],
                                    float* t,
                                    float* u,
                                    float* v,
                                    float  epsilon)
{
    TRI3_MESH_VS_RAY_EX(u32);
}

static int tri3_mesh_vs_ray_ex_raw( float  ray_pos[3],
                                    float  ray_dir[3],
                                    float* vertices,
                                    size_t floats,
                                    size_t offset,
                                    size_t stride,
                                    u32    hit_indices[3],
                                    float* t,
                                    float* u,
                                    float* v,
                                    float  epsilon)
{
    AE_PROFILE_ENTER();

    int r_hit = 0;
    size_t ix = 0;

    for (; ix < floats; ix += stride * 3)
    {
        float temp_t, *v0 = &vertices[ix + stride * 0 + offset];
        float temp_u, *v1 = &vertices[ix + stride * 1 + offset];
        float temp_v, *v2 = &vertices[ix + stride * 2 + offset];

        if (tri3_vs_ray(ray_pos, ray_dir, v0, v1, v2,
                &temp_t, &temp_u, &temp_v, epsilon))
        {
            ae_if (!r_hit || temp_t < *t)
            {
                r_hit = 1;

                hit_indices[0] = ix / stride; // vert
                hit_indices[1] = hit_indices[0] + 1;
                hit_indices[2] = hit_indices[0] + 2;

                *t = temp_t;
                *u = temp_u;
                *v = temp_v;
            }
        }
    }

    AE_PROFILE_LEAVE();
    return r_hit;
}

int tri3_mesh_vs_ray_ex(float ray_pos[3],
                        float ray_dir[3],

                        float* vertices,
                        size_t floats,
                        size_t offset,
                        size_t stride,

                        void * indices,
                        ae_index_type_t type,
                        size_t i_count,

                        u32 hit_indices[3],
                        float *t,
                        float *u,
                        float *v,

                        float epsilon)
{
    ae_if (i_count != 0)
    {
        // the vertices are indexed, so we dispatch to the proper method by index int size.
        ae_assert(i_count % 3 == 0, "got a non-triangular index count: %u", (u32)i_count);

        ae_switch (type, ae_index_type, AE_INDEX_TYPE, suffix)
        {
            case AE_INDEX_TYPE_U8:
            {
                return tri3_mesh_vs_ray_ex_u8 (ray_pos, ray_dir, vertices, floats, offset,
                            stride, (u8 *)indices, i_count, hit_indices, t, u, v, epsilon);
            }
            case AE_INDEX_TYPE_U16:
            {
                return tri3_mesh_vs_ray_ex_u16(ray_pos, ray_dir, vertices, floats, offset,
                            stride, (u16*)indices, i_count, hit_indices, t, u, v, epsilon);
            }
            case AE_INDEX_TYPE_U32:
            {
                return tri3_mesh_vs_ray_ex_u32(ray_pos, ray_dir, vertices, floats, offset,
                            stride, (u32*)indices, i_count, hit_indices, t, u, v, epsilon);
            }

            default: ae_assert(0, "%u", (u32)type); return 0;
        }
    }

    ae_assert((floats / stride) % 3 == 0, "num %u offset %u stride %u",
                                (u32)floats, (u32)offset, (u32)stride);

    return tri3_mesh_vs_ray_ex_raw(ray_pos, ray_dir, vertices, floats,
                        offset, stride, hit_indices, t, u, v, epsilon);
}

int tri3_mesh_vs_ray(   float ray_pos[3],
                        float ray_dir[3],

                        float* vertices,
                        size_t floats,

                        void * indices,
                        ae_index_type_t type,
                        size_t i_count,

                        u32 hit_indices[3],
                        float *t,
                        float *u,
                        float *v,

                        float epsilon)
{
    return tri3_mesh_vs_ray_ex(ray_pos, ray_dir, vertices, floats, 0, 3,
                indices, type, i_count, hit_indices, t, u, v, epsilon );
}

/*
================================================================================
 * ~~ [ inversion (face swapping) ] ~~ *
--------------------------------------------------------------------------------
*/

void tri3_mesh_invert_ex(   float* vertices,

                            size_t floats,
                            size_t stride,

                            void * indices,
                            ae_index_type_t type,
                            size_t i_count)
{
    AE_PROFILE_ENTER(); size_t i = 0;

    /* TODO: move indexed path type dispatch outside of the loop
     */
    ae_if (i_count != 0)
    {
        for (; i < i_count; i += 3)
        {
            size_t a = idx_value_get(indices, type, i_count, i + 0);
            size_t c = idx_value_get(indices, type, i_count, i + 2);

            idx_value_set(indices, type, i_count, i + 0, c);
            idx_value_set(indices, type, i_count, i + 2, a);
        }
    }
    else
    {
        /* TODO: would it be better to call memswap here instead?
         */
        size_t vert_bytes = sizeof(float) * stride;
        float* temp_v = (float*)alloca(vert_bytes);

        for (; i < floats; i += stride * 3)
        {
            float* a = vertices + i + stride * 0;
            float* c = vertices + i + stride * 2;

            memcpy(temp_v, a, vert_bytes);
            memcpy(a, c, vert_bytes);
            memcpy(c, temp_v, vert_bytes);
        }
    }

    AE_PROFILE_LEAVE();
}

void tri3_mesh_invert(  float* vertices,
                        size_t floats,

                        void * indices,
                        ae_index_type_t type,
                        size_t i_count)
{
    tri3_mesh_invert_ex(vertices, floats, 3, indices, type, i_count);
}
