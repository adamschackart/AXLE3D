/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

int ae_find_factors(int* factors, const int value)
{
    AE_STUB(); return 0;
}

void ae_closest_factors(int* a, int* b, const int value)
{
    AE_STUB();
}

int vec3fix_degenerate_normal(float v[3])
{
    AE_STUB(); return 0;
}

void vec3rotate_ex( float out[3], const float s[3], const float axis[3],
                    const float sin_theta, const float cos_theta )
{
    AE_STUB();
}

void vec3rotate(float out[3], const float s[3], const float axis[3], const float theta)
{
    AE_STUB();
}

void quat_to_row_mat3x3(float m[ 9], const float q[4])
{
    AE_STUB();
}

void quat_to_col_mat3x3(float m[ 9], const float q[4])
{
    AE_STUB();
}

void quat_to_row_mat4x4(float m[16], const float q[4])
{
    AE_STUB();
}

void quat_to_col_mat4x4(float m[16], const float q[4])
{
    AE_STUB();
}

void mat4x4row_to_quat(float q[4], const float m[16])
{
    AE_STUB();
}

void mat4x4col_to_quat(float q[4], const float m[16])
{
    AE_STUB();
}

int mat_inverse(float* out, const float* const m, const size_t n)
{
    AE_STUB(); return 0;
}

int mat2x2inverse(float out[ 4], const float m[ 4])
{
    AE_STUB(); return 0;
}

int mat3x3inverse(float out[ 9], const float m[ 9])
{
    AE_STUB(); return 0;
}

int mat4x4inverse(float out[16], const float m[16])
{
    AE_STUB(); return 0;
}

int mat_invert(float* m, const size_t n)
{
    AE_STUB(); return 0;
}

int mat2x2invert(float m[ 4])
{
    AE_STUB(); return 0;
}

int mat3x3invert(float m[ 9])
{
    AE_STUB(); return 0;
}

int mat4x4invert(float m[16])
{
    AE_STUB(); return 0;
}

float mat_determinant(const float* const m, const size_t n)
{
    AE_STUB(); return 0.0f;
}

float mat2x2determinant(const float m[ 4])
{
    AE_STUB(); return 0.0f;
}

float mat3x3determinant(const float m[ 9])
{
    AE_STUB(); return 0.0f;
}

float mat4x4determinant(const float m[16])
{
    AE_STUB(); return 0.0f;
}

int ellipsoid_vs_ray(float* h_dist, const float e_pos[3], const float e_ext[3],
                                const float ray_pos[3], const float ray_dir[3])
{
    AE_STUB(); return 0;
}

void ae_math_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_math_quit(void)
{
    // AE_STUB();
}
