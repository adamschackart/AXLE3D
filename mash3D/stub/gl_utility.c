/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __GL_CORE_H__
#include <gl_core.h>
#endif

/*
================================================================================
 * ~~ [ GL state logging ] ~~ *
--------------------------------------------------------------------------------
*/

void gl_log_error_state_ex(const char* filename, const char* funcname)
{
    AE_STUB();
}

void gl_log_texture_state_ex(const char* filename, const char* funcname)
{
    AE_STUB();
}

void gl_log_blend_state_ex(const char* filename, const char* funcname)
{
    AE_STUB();
}

void gl_log_material_state_ex(const char* filename, const char* funcname)
{
    AE_STUB();
}

void gl_log_light_state_ex(const char* filename, const char* funcname)
{
    AE_STUB();
}

void gl_log_internal_state_ex(const char* filename, const char* funcname)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ utilities ] ~~ *
--------------------------------------------------------------------------------
*/

void gl_enter3D(int width, int height, double FOV, double znear, double zfar)
{
    AE_STUB();
}

void gl_leave3D(void)
{
    AE_STUB();
}

void gl_enter2D(int width, int height)
{
    AE_STUB();
}

void gl_leave2D(void)
{
    AE_STUB();
}

/* ===== [ 2D shapes ] ====================================================== */

void gl_rect_ex(float* rect, const float* bl, const float* br, const float* tr,
                            const float* tl, int is_outline, float line_width)
{
    AE_STUB();
}

void gl_outline(float* rect, const float* rgba)
{
    AE_STUB();
}

void gl_rect(float* rect, const float* rgba)
{
    AE_STUB();
}

/* ===== [ 3D shapes ] ====================================================== */

void gl_aabbox_ex(float* minv, float* maxv, const float* rgba, float line_width)
{
    AE_STUB();
}

void gl_aabbox(float* minv, float* maxv, const float* rgba)
{
    AE_STUB();
}

void gl_ellipsoid(float* center, float* extent, const float* rgba)
{
    AE_STUB();
}

void gl_sphere(float* center, float radius, const float* rgba)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ coordinate system ] ~~ *
--------------------------------------------------------------------------------
*/

void gl_coord3D_apply_camera_transform_ex(gl_coord3D_t* coord, int absolute)
{
    AE_STUB();
}

void gl_coord3D_push_camera_transform_ex(gl_coord3D_t* coord, int absolute)
{
    AE_STUB();
}

void gl_coord3D_pop_camera_transform(void)
{
    AE_STUB();
}

void gl_coord3D_apply_object_transform_ex(gl_coord3D_t* coord, int absolute)
{
    AE_STUB();
}

void gl_coord3D_push_object_transform_ex(gl_coord3D_t* coord, int absolute)
{
    AE_STUB();
}

void gl_coord3D_pop_object_transform(void)
{
    AE_STUB();
}

void gl_coord3D_heading_towards_point(gl_coord3D_t* coord, float* p)
{
    AE_STUB();
}

void gl_coord3D_pitch_towards_point(gl_coord3D_t* coord, float* p)
{
    AE_STUB();
}

void gl_coord3D_face_towards_point(gl_coord3D_t* coord, float* p)
{
    AE_STUB();
}

void gl_coord3D_get_matrix(gl_coord3D_t* coord, float* matrix)
{
    AE_STUB();
}

void gl_coord3D_set_matrix(gl_coord3D_t* coord, float* matrix)
{
    AE_STUB();
}

void gl_coord3D_get_quat(gl_coord3D_t* coord, float* quat)
{
    AE_STUB();
}

void gl_coord3D_set_quat(gl_coord3D_t* coord, float* quat)
{
    AE_STUB();
}

void gl_coord3D_get_normal(gl_coord3D_t* coord, float* normal)
{
    AE_STUB();
}

void gl_coord3D_set_normal(gl_coord3D_t* coord, float* normal)
{
    AE_STUB();
}

void gl_coord3D_get_ray(gl_coord3D_t* coord, float* p, float* v)
{
    AE_STUB();
}

void gl_coord3D_set_ray(gl_coord3D_t* coord, float* p, float* v)
{
    AE_STUB();
}

void gl_coord3D_get_up_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_set_up_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_get_down_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_set_down_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_get_left_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_set_left_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_get_right_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_set_right_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ init & quit ] ~~ *
--------------------------------------------------------------------------------
*/

const char* gl_implementation(void)
{
    AE_STUB(); return "";
}

void gl_utility_init(void)
{
    // AE_STUB();
}

void gl_utility_quit(void)
{
    // AE_STUB();
}
