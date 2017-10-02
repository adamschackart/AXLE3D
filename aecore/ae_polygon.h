/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_POLYGON_H__
#define __AE_POLYGON_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_polygon_init(int argc, char** argv);
void ae_polygon_quit(void); // private init

/*
================================================================================
 * ~~ [ collision polygons ] ~~ *
--------------------------------------------------------------------------------
*/

typedef struct ae_plane_winding_t
{
    float* edges; // array of edge planes
    size_t count; // count of edge planes

    float face[4]; // polygon face plane
    float vert[3]; // point on polygon
    float bmin[3]; // aabbox bounds min
    float bmax[3]; // aabbox bounds max
} \
    ae_plane_winding_t;

/* ===== [ construction ] =================================================== */

AE_DECL void AE_CALL pw_alloc_edges(ae_plane_winding_t* winding, size_t count);
AE_DECL void AE_CALL pw_free_edges(ae_plane_winding_t* winding);

AE_DECL void AE_CALL
pw_copy_winding(ae_plane_winding_t* dst, const ae_plane_winding_t* const src);

AE_DECL int AE_CALL
pw_winding_cmp(const ae_plane_winding_t* const a, const ae_plane_winding_t* const b);

AE_DECL int AE_CALL
pw_winding_cmp_e(const ae_plane_winding_t* const a, // fuzzy comparison
                const ae_plane_winding_t* const b, const float epsilon);

AE_DECL int AE_CALL // create new collision polygon from the points in a triangle
pw_create_from_tri(ae_plane_winding_t* dst, const float v0[3], const float v1[3],
                                    const float v2[3], const int fix_degenerate);

/* ===== [ collision detection ] ============================================ */

AE_DECL float AE_CALL // penetration through plane along normal (0 if no hit)
pw_winding_vs_ellipsoid( const ae_plane_winding_t * const winding,
                const float e_center[3], const float e_extent[3]);

AE_DECL int AE_CALL // contact point stored in hit (returns 0 if nothing hit)
pw_winding_vs_line_type(float hit[3], const ae_plane_winding_t* const winding,
    const float line_p[3], const float line_v[3], const ae_line_type_t type);

AE_DECL int AE_CALL
pw_winding_vs_line(float hit[3], const ae_plane_winding_t* const winding,
                    const float line_point[3], const float line_vec[3]);

AE_DECL int AE_CALL
pw_winding_vs_line_segment(float hit[3], const ae_plane_winding_t* const winding,
                            const float line_point[3], const float line_vec[3]);

AE_DECL int AE_CALL
pw_winding_vs_ray(float hit[3], const ae_plane_winding_t* const winding,
                    const float line_point[3], const float line_vec[3]);

/* ===== [ serialization ] ================================================== */

AE_DECL size_t AE_CALL
pw_buffer_size(const ae_plane_winding_t* const winding);

AE_DECL void AE_CALL
pw_buffer_save(u8* out, const ae_plane_winding_t* const winding);

AE_DECL void AE_CALL
pw_buffer_load(ae_plane_winding_t* winding, u8* buf, size_t len);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_POLYGON_H__ */
