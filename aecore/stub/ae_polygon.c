/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void pw_alloc_edges(ae_plane_winding_t* winding, size_t count)
{
    AE_STUB();
}

void pw_free_edges(ae_plane_winding_t* winding)
{
    AE_STUB();
}

void pw_copy_winding(ae_plane_winding_t* dst, const ae_plane_winding_t* const src)
{
    AE_STUB();
}

int pw_winding_cmp(const ae_plane_winding_t* const a, const ae_plane_winding_t* const b)
{
    AE_STUB(); return 0;
}

int pw_winding_cmp_e(const ae_plane_winding_t* const a, // fuzzy comparison
                    const ae_plane_winding_t* const b, const float epsilon)
{
    AE_STUB(); return 0;
}

int pw_create_from_tri(ae_plane_winding_t * dst, const float v0[3],
    const float v1[3], const float v2[3], const int fix_degenerate)
{
    AE_STUB(); return 0;
}

float pw_winding_vs_ellipsoid(const ae_plane_winding_t* const winding,
                    const float e_center[3], const float e_extent[3])
{
    AE_STUB(); return 0.0f;
}

int pw_winding_vs_line_type(float hit[3], const ae_plane_winding_t* const winding,
    const float line_point[3], const float line_vec[3], const ae_line_type_t type)
{
    AE_STUB(); return 0;
}

int pw_winding_vs_line(float hit[3], const ae_plane_winding_t* const winding,
                        const float line_point[3], const float line_vec[3])
{
    AE_STUB(); return 0;
}

int pw_winding_vs_line_segment(float hit[3], const ae_plane_winding_t* const winding,
                                const float line_point[3], const float line_vec[3])
{
    AE_STUB(); return 0;
}

int pw_winding_vs_ray(float hit[3], const ae_plane_winding_t* const winding,
                        const float line_point[3], const float line_vec[3])
{
    AE_STUB(); return 0;
}

size_t pw_buffer_size(const ae_plane_winding_t* const winding)
{
    AE_STUB(); return 0;
}

void pw_buffer_save(u8* out, const ae_plane_winding_t* const winding)
{
    AE_STUB();
}

void pw_buffer_load(ae_plane_winding_t* winding, u8* buf, size_t len)
{
    AE_STUB();
}

void ae_polygon_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_polygon_quit(void)
{
    // AE_STUB();
}
