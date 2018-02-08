/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_TRIANGLE_H__
#define __AE_TRIANGLE_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif
#ifndef __AE_INDEX_H__
#include <ae_index.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_triangle_init(int argc, char** argv);
void ae_triangle_quit(void); // private init

/*
================================================================================
 * ~~ [ misc. utils ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float tri_area( const float* v0, const float* v1,
                                const float* v2, const size_t n)
{
    float s; // allocate temporary space for results

    float a, *av = (float*)alloca(sizeof(float) * n);
    float b, *bv = (float*)alloca(sizeof(float) * n);
    float c, *cv = (float*)alloca(sizeof(float) * n);

    vec_sub_vec(av, v1, v0, n); a = vec_mag(av, n);
    vec_sub_vec(bv, v2, v0, n); b = vec_mag(bv, n);
    vec_sub_vec(cv, v2, v1, n); c = vec_mag(cv, n);

    s = (a + b + c) / 2.0f; /* precise single side */
    return ae_sqrtf(s * (s - a) * (s - b) * (s - c));
}

#if 0
    #define tri2area(v0, v1, v2) tri_area((v0), (v1), (v2), 2)
    #define tri3area(v0, v1, v2) tri_area((v0), (v1), (v2), 3)
#else
    static c_inline float tri2area(const float v0[2],
                const float v1[2], const float v2[2])
    {
        float a, b, c, s, av[2], bv[2], cv[2]; /* temp */

        vec2sub_vec(av, v1, v0); a = vec2mag(av);
        vec2sub_vec(bv, v2, v0); b = vec2mag(bv);
        vec2sub_vec(cv, v2, v1); c = vec2mag(cv);

        s = (a + b + c) / 2.0f; /* precise single side */
        return ae_sqrtf(s * (s - a) * (s - b) * (s - c));
    }

    static c_inline float tri3area(const float v0[3],
                const float v1[3], const float v2[3])
    {
        float a, b, c, s, av[3], bv[3], cv[3]; /* temp */

        vec3sub_vec(av, v1, v0); a = vec3mag(av);
        vec3sub_vec(bv, v2, v0); b = vec3mag(bv);
        vec3sub_vec(cv, v2, v1); c = vec3mag(cv);

        s = (a + b + c) / 2.0f; /* precise single side */
        return ae_sqrtf(s * (s - a) * (s - b) * (s - c));
    }
#endif

static c_inline void tri_centroid( float* center, const float* v0,
                const float* v1, const float* v2, const size_t n)
{
    size_t i = 0; // compute 1/3 of the sum of all vertices
    for (; i < n; i++)
    {
        center[i] = (v0[i] + v1[i] + v2[i]) * (1.0f / 3.0f);
    }
}

#if 0
    #define tri2centroid(d, a, b, c) tri_centroid((d), (a), (b), (c), 2)
    #define tri3centroid(d, a, b, c) tri_centroid((d), (a), (b), (c), 3)
#else
    static c_inline void tri2centroid(float center[2], const float v0[2],
                                    const float v1[2], const float v2[2])
    {
        center[0] = (v0[0] + v1[0] + v2[0]) * (1.0f / 3.0f);
        center[1] = (v0[1] + v1[1] + v2[1]) * (1.0f / 3.0f);
    }

    static c_inline void tri3centroid(float center[3], const float v0[3],
                                    const float v1[3], const float v2[3])
    {
        center[0] = (v0[0] + v1[0] + v2[0]) * (1.0f / 3.0f);
        center[1] = (v0[1] + v1[1] + v2[1]) * (1.0f / 3.0f);
        center[2] = (v0[2] + v1[2] + v2[2]) * (1.0f / 3.0f);
    }
#endif

static c_inline void tri3face_normal(float out[3], const float v0[3],
                                const float v1[3], const float v2[3])
{
    /* NOTE: could use plane_from_points, but this is slightly faster
     * because we don't need to compute distance scalar (dot product)
     */
    float u[3];
    float v[3];

    /* this isn't a crasher bug, but it generates meaningless results
     */
    ae_assert(tri3area(v0, v1, v2) > 0.00000001f,
            "got degenerate (0-area) triangle!");

    vec3sub_vec(u, v1, v0);
    vec3sub_vec(v, v2, v0);

    vec3cross(out, u, v);
    vec3normalize(out, out);
}

/*
================================================================================
 * ~~ [ depth sorting ] ~~ *
--------------------------------------------------------------------------------
*/

/* Sort the triangles from back to front, based on distance from a viewpoint.
 * This is useful for correctly rendering scenes with translucent surfaces.
 */
AE_DECL void AE_CALL tri3_mesh_depthsort_ex(const float point[3],

                                            float* vertices,
                                            size_t floats,
                                            size_t offset,
                                            size_t stride,

                                            void * indices,
                                            ae_index_type_t type,
                                            size_t i_count);

AE_DECL void AE_CALL tri3_mesh_depthsort(   const float point[3],

                                            float* vertices,
                                            size_t floats,

                                            void * indices,
                                            ae_index_type_t type,
                                            size_t i_count);

/*
================================================================================
 * ~~ [ raycasting (collision detection) ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline void tri3_ray_hit_point(float point[3], // out
                                        const float ray_pos[3],
                                        const float ray_dir[3],
                                        const float vert0[3],
                                        const float vert1[3],
                                        const float vert2[3],
                                        const float t,
                                        const float u,
                                        const float v)
{
    // TODO: try getting the point from (u, v) just for the fun of it.
    // i can't imagine it would be any faster (3 adds and 3 muls)...
#if 0
    ae_assert(vec3is_unit_e(ray_dir, 0.00000001f), "(%f, %f, %f), (%f, %f, %f)",
        ray_pos[0], ray_pos[1], ray_pos[2], ray_dir[0], ray_dir[1], ray_dir[2]);

    vec3scale(point, ray_dir, t);
    vec3add_vec(point, point, ray_pos);
#else
    float n[3]; // normalize for safety

    vec3normalize(n, ray_dir);
    vec3scale(point, n, t);
    vec3add_vec(point, point, ray_pos);
#endif
}

/* Moller-Trumbore ray-triangle intersection. If the ray hits the triangle,
 * T is the distance from the ray origin (position) to the collision point.
 * U and V are the barycentric coordinates along the triangle the ray hit.
 */
AE_DECL int AE_CALL tri3_vs_ray(float ray_pos[3],
                                float ray_dir[3],
                                float vert0[3],
                                float vert1[3],
                                float vert2[3],
                                float *t,
                                float *u,
                                float *v,
                                float epsilon);

/* Tests a ray against a triangle mesh. Input vertices can be "raw" with no
 * attached index buffer, or indexed by standard byte, short, and int types.
 *
 * If the ray hits, the closest hit is stored. In addition to the TUV coords
 * mentioned above, the indices of the hit tri are also passed to the caller.
 *
 * formula to compute point: hit_point = ray_pos + ray_dir.normalized() * t
 *                          ------------------------------------------------
 *  (this is being safe and not assuming the ray's normal is unit length).
 */
AE_DECL int AE_CALL tri3_mesh_vs_ray_ex(float ray_pos[3],
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

                                        float epsilon);

/* Tests a ray against a AE_V3F triangle mesh. Might use SIMD or something.
 */
AE_DECL int AE_CALL tri3_mesh_vs_ray(   float ray_pos[3],
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

                                        float epsilon);

/*
================================================================================
 * ~~ [ inversion (face swapping) ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL tri3_mesh_invert_ex(float *vertices,

                                        size_t floats,
                                        size_t stride,

                                        void * indices,
                                        ae_index_type_t type,
                                        size_t i_count);

/* Each triangle is made to face the opposite direction. This is potentially
 * useful for renderers that expect tris to be specified in clockwise order.
 */
AE_DECL void AE_CALL tri3_mesh_invert(  float* vertices,
                                        size_t floats,

                                        void * indices,
                                        ae_index_type_t type,
                                        size_t i_count);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_TRIANGLE_H__ */
