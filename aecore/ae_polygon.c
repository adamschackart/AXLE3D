/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

#ifndef AE_QUAD_EDGES_NUM_BLOCKS // 8 mb
#define AE_QUAD_EDGES_NUM_BLOCKS 131072
#endif

#ifndef AE_TRI_EDGES_NUM_BLOCKS // ~8 mb
#define AE_TRI_EDGES_NUM_BLOCKS 174762
#endif

static ae_memory_chunk_t ae_quad_winding_edge_planes;
static ae_memory_chunk_t ae_tri_winding_edge_planes;

void pw_alloc_edges(ae_plane_winding_t* winding, size_t count)
{
    ae_assert(winding->count == 0, "cannot reallocate plane winding edges!");
    ae_assert(count > 2, "invalid poly edge count: %u", (unsigned int)count);

    switch (count)
    {
        case 3:
        {
            winding->edges = (float*)ae_block_malloc(&ae_tri_winding_edge_planes);
        }
        break;

        case 4:
        {
            winding->edges = (float*)ae_block_malloc(&ae_quad_winding_edge_planes);
        }
        break;

        default:
        {
            winding->edges = (float*)ae_malloc(count * sizeof(float[4]));
        }
        break;
    }

    ae_assert(winding->edges, "memory allocation code is stubbed out");
    winding->count = count;
}

void pw_free_edges(ae_plane_winding_t* winding)
{
    switch (winding->count)
    {
        case 3:
        {
            ae_block_free(&ae_tri_winding_edge_planes, winding->edges);
        }
        break;

        case 4:
        {
            ae_block_free(&ae_quad_winding_edge_planes, winding->edges);
        }
        break;

        default:
        {
            ae_free(winding->edges);
        }
        break;
    }

    winding->edges = NULL;
    winding->count = 0;
}

void pw_copy_winding(ae_plane_winding_t* dst, const ae_plane_winding_t* const src)
{
    AE_PROFILE_ENTER();

    ae_assert(dst != src,  "source and destination windings alias in copy");
    ae_assert(dst->edges == NULL, "cannot reallocate plane winding edges!");

    if (src->edges != NULL)
    {
        pw_alloc_edges(dst, src->count); // allocate & copy edge planes
        memcpy(dst->edges, src->edges, src->count * sizeof(float[4]));
    }

    memcpy(dst->face, src->face, sizeof(float[4]));
    memcpy(dst->vert, src->vert, sizeof(float[3]));
    memcpy(dst->bmin, src->bmin, sizeof(float[3]));
    memcpy(dst->bmax, src->bmax, sizeof(float[3]));

    AE_PROFILE_LEAVE();
}

int pw_winding_cmp(const ae_plane_winding_t* const a, const ae_plane_winding_t* const b)
{
    float *a_ptr, *b_ptr, *end_p;

    if (a == b) return 1;

    if (a->count != b->count) return 0; // poly struct member comparisons
    if (!plane_cmp(a->face, b->face)) return 0;

    if (!vec3cmp(a->vert, b->vert)) return 0;
    if (!vec3cmp(a->bmin, b->bmin)) return 0;
    if (!vec3cmp(a->bmax, b->bmax)) return 0;

    for(a_ptr = a->edges, b_ptr = b->edges, end_p = a_ptr + a->count * 4;
        a_ptr < end_p; a_ptr += 4, b_ptr += 4) // polygon edge comparison
    {
        if (!plane_cmp(a_ptr, b_ptr)) return 0;
    }

    return 1;
}

int pw_winding_cmp_e(const ae_plane_winding_t* const a, // fuzzy comparison
                    const ae_plane_winding_t* const b, const float epsilon)
{
    float *a_ptr, *b_ptr, *end_p;

    if (a == b) return 1;

    if (a->count != b->count) return 0; // poly struct member comparisons
    if (!plane_cmp_e(a->face, b->face, epsilon)) return 0;

    if (!vec3cmp_e(a->vert, b->vert, epsilon)) return 0;
    if (!vec3cmp_e(a->bmin, b->bmin, epsilon)) return 0;
    if (!vec3cmp_e(a->bmax, b->bmax, epsilon)) return 0;

    for(a_ptr = a->edges, b_ptr = b->edges, end_p = a_ptr + a->count * 4;
        a_ptr < end_p; a_ptr += 4, b_ptr += 4) // polygon edge comparison
    {
        if (!plane_cmp_e(a_ptr, b_ptr, epsilon)) return 0;
    }

    return 1;
}

int pw_create_from_tri(ae_plane_winding_t * dst, const float v0[3],
    const float v1[3], const float v2[3], const int fix_degenerate)
{
    AE_PROFILE_ENTER(); // track the number of these we build every frame

    float v_sub[3];
    float cross[3];

    if (!plane_from_points(dst->face, v0, v1, v2, fix_degenerate))
    {
        AE_PROFILE_LEAVE(); return 0;
    }

    pw_alloc_edges(dst, 3);

    vec3sub_vec(v_sub, v1, v0); vec3cross(cross, v_sub, dst->face); // v0
    if (!plane_from_point_vec(dst->edges + 0, v0, cross, fix_degenerate))
    {
        pw_free_edges(dst); // clear
        AE_PROFILE_LEAVE(); return 0;
    }

    vec3sub_vec(v_sub, v2, v1); vec3cross(cross, v_sub, dst->face); // v1
    if (!plane_from_point_vec(dst->edges + 4, v1, cross, fix_degenerate))
    {
        pw_free_edges(dst); // clear
        AE_PROFILE_LEAVE(); return 0;
    }

    vec3sub_vec(v_sub, v0, v2); vec3cross(cross, v_sub, dst->face); // v2
    if (!plane_from_point_vec(dst->edges + 8, v2, cross, fix_degenerate))
    {
        pw_free_edges(dst); // clear
        AE_PROFILE_LEAVE(); return 0;
    }

    vec3copy(dst->vert, v0);

    dst->bmin[0] = ae_fmin3f(v0[0], v1[0], v2[0]);
    dst->bmin[1] = ae_fmin3f(v0[1], v1[1], v2[1]);
    dst->bmin[2] = ae_fmin3f(v0[2], v1[2], v2[2]);

    dst->bmax[0] = ae_fmax3f(v0[0], v1[0], v2[0]);
    dst->bmax[1] = ae_fmax3f(v0[1], v1[1], v2[1]);
    dst->bmax[2] = ae_fmax3f(v0[2], v1[2], v2[2]);

    AE_PROFILE_LEAVE(); return 1;
}

/*
================================================================================
 * ~~ [ collision detection ] ~~ *
--------------------------------------------------------------------------------
*/

// toggle profiling these, as their average time is somewhere in the attoseconds
#if 0
    #define PW_PROFILE_ENTER AE_PROFILE_ENTER
    #define PW_PROFILE_LEAVE AE_PROFILE_LEAVE
#else
    static c_inline void* pw_profile_enter(void) { return NULL; }
    static c_inline void  pw_profile_leave(void* p) { /* nop */ }

    #define PW_PROFILE_ENTER() void* _pw_profile_context = pw_profile_enter()
    #define PW_PROFILE_LEAVE() pw_profile_leave(_pw_profile_context)
#endif

float pw_winding_vs_ellipsoid(const ae_plane_winding_t* const winding,
                    const float e_center[3], const float e_extent[3])
{
    PW_PROFILE_ENTER(); // track the number of intersection tests per frame

    float edge_r2, dd, sd;
    size_t i;

    const float d = vec3dot(e_center, winding->face) - winding->face[3];

    const float x = winding->face[0] * e_extent[0];
    const float y = winding->face[1] * e_extent[1];
    const float z = winding->face[2] * e_extent[2];

    const float r = x * x + y * y + z * z;
    const float s = d * d;

    if (s > r) { PW_PROFILE_LEAVE(); return 0.0f; }

    // get squared distance ellipsoid needs to be from edge plane - this is
    // an approximated sphere equation, and is a little too sensitive near the
    // vertices (collision planes extend infinitely, and the edge radius is
    // calculated incorrectly), but it's far cheaper than the exact solution
    // (which involves scale matrix transforms of the unit sphere and plane).
    edge_r2 = r - s;

    for (i = 0; i < winding->count; ++i)
    {
        dd = vec3dot(e_center, &winding->edges[i*4]) - winding->edges[i*4+3];
        sd = dd < 0.0f ? -1.0f : 1.0f; // keep sign after square (avoid sqrt)

        if (sd * dd * dd > edge_r2) { PW_PROFILE_LEAVE(); return 0.0f; }
    }

    PW_PROFILE_LEAVE(); return ae_sqrtf(r) - d;
}

int pw_winding_vs_line_type(float hit[3], const ae_plane_winding_t* const winding,
    const float line_point[3], const float line_vec[3], const ae_line_type_t type)
{
    PW_PROFILE_ENTER();

    if (plane_vs_line_type(hit, line_point, line_vec, type, winding->face))
    {
        size_t i = 0; // ensure hitpoint is inside all edges
        for (; i < winding->count; ++i)
        {
            const float* p = &winding->edges[i * 4];
            const float  d = vec3dot(hit, p) - p[3];

            if (d > 0.0f) { PW_PROFILE_LEAVE(); return 0; }
        }

        PW_PROFILE_LEAVE(); return 1;
    }
    else
    {
        PW_PROFILE_LEAVE(); return 0;
    }
}

int pw_winding_vs_line(float hit[3], const ae_plane_winding_t* const winding,
                        const float line_point[3], const float line_vec[3])
{
    return pw_winding_vs_line_type(hit, winding, line_point, line_vec, AE_LINE_TYPE_LINE);
}

int pw_winding_vs_line_segment(float hit[3], const ae_plane_winding_t* const winding,
                                const float line_point[3], const float line_vec[3])
{
    #if 0
    return pw_winding_vs_line_type(hit, winding, line_point, line_vec, AE_LINE_TYPE_LINE_SEGMENT);
    #else
    PW_PROFILE_ENTER();

    if (plane_vs_line_segment(hit, line_point, line_vec, winding->face))
    {
        size_t i = 0; // ensure hitpoint is inside all edges
        for (; i < winding->count; ++i)
        {
            const float* p = &winding->edges[i * 4];
            const float  d = vec3dot(hit, p) - p[3];

            if (d > 0.0f) { PW_PROFILE_LEAVE(); return 0; }
        }

        PW_PROFILE_LEAVE(); return 1;
    }
    else
    {
        PW_PROFILE_LEAVE(); return 0;
    }
    #endif
}

int pw_winding_vs_ray(float hit[3], const ae_plane_winding_t* const winding,
                        const float line_point[3], const float line_vec[3])
{
    return pw_winding_vs_line_type(hit, winding, line_point, line_vec, AE_LINE_TYPE_RAY);
}

/*
================================================================================
 * ~~ [ serialization ] ~~ *
--------------------------------------------------------------------------------
FIXME: this code is not endian safe! always store data in little endian format!
--------------------------------------------------------------------------------
*/

size_t pw_buffer_size(const ae_plane_winding_t* const winding)
{
    return (sizeof(float[4]) * winding->count +
            sizeof(float[4]) + // face
            sizeof(float[3]) + // vert
            sizeof(float[3]) + // bmin
            sizeof(float[3])); // bmax
}

void pw_buffer_save(u8* out, const ae_plane_winding_t* const winding)
{
    const size_t plane_bytes = sizeof(float[4]) * winding->count;
    memcpy(out, winding->edges, plane_bytes); out += plane_bytes;

    memcpy(out, winding->face, sizeof(float[4])); out += sizeof(float[4]);
    memcpy(out, winding->vert, sizeof(float[3])); out += sizeof(float[3]);
    memcpy(out, winding->bmin, sizeof(float[3])); out += sizeof(float[3]);
    memcpy(out, winding->bmax, sizeof(float[3])); out += sizeof(float[3]);
}

void pw_buffer_load(ae_plane_winding_t* winding, u8* buf, size_t len)
{
    const size_t edge_size = len - (sizeof(float[4]) + // face
                                    sizeof(float[3]) + // vert
                                    sizeof(float[3]) + // bmin
                                    sizeof(float[3])); // bmax

    const size_t num_edges = edge_size / sizeof(float[4]);
    if (num_edges) { pw_alloc_edges(winding, num_edges); }

    memcpy(winding->edges, buf, edge_size); buf += edge_size; // edge planes
    memcpy(winding->face , buf, sizeof(float[4])); buf += sizeof(float[4]);
    memcpy(winding->vert , buf, sizeof(float[3])); buf += sizeof(float[3]);
    memcpy(winding->bmin , buf, sizeof(float[3])); buf += sizeof(float[3]);
    memcpy(winding->bmax , buf, sizeof(float[3])); buf += sizeof(float[3]);
}

/*
================================================================================
 * ~~ [ initialization ] ~~ *
--------------------------------------------------------------------------------
*/

void ae_polygon_init(int argc, char** argv)
{
    ae_chunk_create(&ae_quad_winding_edge_planes, "quad_edges",
                4 * sizeof(float[4]), AE_QUAD_EDGES_NUM_BLOCKS);

    ae_chunk_create(&ae_tri_winding_edge_planes , "tri_edges",
                3 * sizeof(float[4]), AE_TRI_EDGES_NUM_BLOCKS);
}

void ae_polygon_quit(void)
{
    ae_chunk_destroy(&ae_tri_winding_edge_planes);
    ae_chunk_destroy(&ae_quad_winding_edge_planes);
}
