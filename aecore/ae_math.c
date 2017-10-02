/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
--- TODO: "re-appropriate" HL2's 16-bit float and vector compression code from
--- github.com/ValveSoftware/source-sdk-2013/tree/master/sp/src/public/mathlib
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void ae_math_init(int argc, char** argv) {}
void ae_math_quit(void) {}

// disable things we've measured; most, if not everything here, is fixed-cost.
#if 0
    #define MT_PROFILE_ENTER AE_PROFILE_ENTER
    #define MT_PROFILE_LEAVE AE_PROFILE_LEAVE
#else
    static c_inline void* mt_profile_enter(void) { return NULL; }
    static c_inline void  mt_profile_leave(void* p) { /* nop */ }

    #define MT_PROFILE_ENTER() void* _mt_profile_context = mt_profile_enter()
    #define MT_PROFILE_LEAVE() mt_profile_leave(_mt_profile_context)
#endif

/*
================================================================================
 * ~~ [ integer math ] ~~ *
--------------------------------------------------------------------------------
*/

int ae_find_factors(int* factors, const int value)
{
    // TODO: use a more efficient algorithm for large values
    MT_PROFILE_ENTER();

    int *fac_ptr = factors, current = 1;
    const register int half = value / 2;

    if (value >= 0)
    {
        if (factors == NULL) // don't fill values, just count
        {
            for (; current <= half; ++current)
            {
                if (!(value % current)) fac_ptr++;
            }

            fac_ptr++;
        }
        else
        {
            for (; current <= half; ++current)
            {
                if (!(value % current)) *fac_ptr++ = current;
            }

            *fac_ptr++ = value;
        }
    }
    else
    {
        assert(!"TODO: factorization of negative integers");
    }

    MT_PROFILE_LEAVE(); return (int)(fac_ptr - factors);
}

void ae_closest_factors(int* a, int* b, const int value)
{
    MT_PROFILE_ENTER();

    int factors[512], num_factors = ae_find_factors(factors, value);

    *a = factors[num_factors / 2 - (num_factors & 1 ? 0 : 1)];
    *b = factors[num_factors / 2]; /* pick largest factors */

    ae_assert((*a) * (*b) == value, "%i * %i != %i", *a, *b, value);

    MT_PROFILE_LEAVE();
}

/*
================================================================================
 * ~~ [ vector math ] ~~ *
--------------------------------------------------------------------------------
*/

static int vec3fix_degenerate_normal_ex(float v[3])
{
    if (v[0] == 0.0f)
    {
        if (v[1] == 0.0f)
        {
            if (v[2] > 0.0f)
            {
                if (v[2] != 1.0f)
                {
                    v[2] = 1.0f; return 1;
                }
            }
            else
            {
                if (v[2] != -1.0f)
                {
                    v[2] = -1.0f; return 1;
                }
            }
            return 0;
        }
        else if (v[2] == 0.0f)
        {
            if (v[1] > 0.0f)
            {
                if (v[1] != 1.0f)
                {
                    v[1] = 1.0f; return 1;
                }
            }
            else
            {
                if (v[1] != -1.0f)
                {
                    v[1] = -1.0f; return 1;
                }
            }
            return 0;
        }
    }
    else if (v[1] == 0.0f)
    {
        if (v[2] == 0.0f)
        {
            if (v[0] > 0.0f)
            {
                if (v[0] != 1.0f)
                {
                    v[0] = 1.0f; return 1;
                }
            }
            else
            {
                if (v[0] != -1.0f)
                {
                    v[0] = -1.0f; return 1;
                }
            }
            return 0;
        }
    }

    if (ae_fabsf(v[0]) == 1.0f)
    {
        if (v[1] != 0.0f || v[2] != 0.0f)
        {
            v[1] = v[2] = 0.0f; return 1;
        }
        return 0;
    }
    else if (ae_fabsf(v[1]) == 1.0f)
    {
        if (v[0] != 0.0f || v[2] != 0.0f)
        {
            v[0] = v[2] = 0.0f; return 1;
        }
        return 0;
    }
    else if (ae_fabsf(v[2]) == 1.0f)
    {
        if (v[0] != 0.0f || v[1] != 0.0f)
        {
            v[0] = v[1] = 0.0f; return 1;
        }
        return 0;
    }

    return 0;
}

int vec3fix_degenerate_normal(float v[3])
{
    MT_PROFILE_ENTER(); int r = vec3fix_degenerate_normal_ex(v);
    MT_PROFILE_LEAVE(); return r; // track calls in planar code
}

void vec3rotate_ex( float out[3], const float s[3], const float axis[3],
                    const float sin_theta, const float cos_theta )
{
    MT_PROFILE_ENTER();

    // copy the axis and self in case of aliasing with our result
    // vector - don't bother with a vec3cross-style assert here.
    const float r2 = vec3mag2(axis);

    if (r2)
    {
        const float st = sin_theta / ae_sqrtf(r2); // no origin used!
        const float dt = vec3dot(axis, s) * (1.0f - cos_theta) / r2;

        const float x = s[0];
        const float y = s[1];
        const float z = s[2];

        const float u = axis[0];
        const float v = axis[1];
        const float w = axis[2];

        out[0] = u * dt + x * cos_theta + (-w * y + v * z) * st;
        out[1] = v * dt + y * cos_theta + ( w * x - u * z) * st;
        out[2] = w * dt + z * cos_theta + (-v * x + u * y) * st;
    }
    else
    {
        vec3copy(out, s);
    }

    MT_PROFILE_LEAVE();
}

void vec3rotate(float out[3], const float s[3], const float axis[3], const float theta)
{
    // cached vs. not; loops that call this would probably be better
    // off writing a custom inlined version of this function anyways.
    MT_PROFILE_ENTER();

    vec3rotate_ex(out, s, axis, ae_sinf(theta), ae_cosf(theta));
    MT_PROFILE_LEAVE();
}

/*
================================================================================
 * ~~ [ quaternion math ] ~~ *
--------------------------------------------------------------------------------
*/

void quat_to_row_mat3x3(float m[ 9], const float q[4])
{
    MT_PROFILE_ENTER();

    const float x2 = q[0] + q[0];
    const float y2 = q[1] + q[1];
    const float z2 = q[2] + q[2];

    const float xx = q[0] * x2;
    const float xy = q[0] * y2;
    const float xz = q[0] * z2;

    const float yy = q[1] * y2;
    const float yz = q[1] * z2;
    const float zz = q[2] * z2;

    const float wx = q[3] * x2;
    const float wy = q[3] * y2;
    const float wz = q[3] * z2;

    m[0] = 1.0f - ( yy + zz );
    m[1] = xy + wz;
    m[2] = xz - wy;

    m[3] = xy - wz;
    m[4] = 1.0f - ( xx + zz );
    m[5] = yz + wx;

    m[6] = xz + wy;
    m[7] = yz - wx;
    m[8] = 1.0f - ( xx + yy );

    MT_PROFILE_LEAVE();
}

void quat_to_col_mat3x3(float m[ 9], const float q[4])
{
    MT_PROFILE_ENTER();

    quat_to_row_mat3x3(m, q);
    mat3x3transpose(m);

    MT_PROFILE_LEAVE();
}

void quat_to_row_mat4x4(float m[16], const float q[4])
{
    MT_PROFILE_ENTER();

    const float xx = q[0] * q[0];
    const float xy = q[0] * q[1];
    const float xz = q[0] * q[2];
    const float xw = q[0] * q[3];
    const float yy = q[1] * q[1];
    const float yz = q[1] * q[2];
    const float yw = q[1] * q[3];
    const float zz = q[2] * q[2];
    const float zw = q[2] * q[3];

    // TODO: if this assert fires, make this func quat_to_row_mat4x4_ex, where
    // quat_to_row_mat4x4 writes the proper right column and bottom row values.
    ae_assert(mat4x4is_identity(m), "garbage or zero values in output matrix");

    m[ 0] = 1.0f - 2.0f * ( yy + zz );
    m[ 1] = 2.0f * ( xy - zw );
    m[ 2] = 2.0f * ( xz + yw );

    m[ 4] = 2.0f * ( xy + zw );
    m[ 5] = 1.0f - 2.0f * ( xx + zz );
    m[ 6] = 2.0f * ( yz - xw );

    m[ 8] = 2.0f * ( xz - yw );
    m[ 9] = 2.0f * ( yz + xw );
    m[10] = 1.0f - 2.0f * ( xx + yy );

    MT_PROFILE_LEAVE();
}

void quat_to_col_mat4x4(float m[16], const float q[4])
{
    MT_PROFILE_ENTER();

    quat_to_row_mat4x4(m, q);
    mat4x4transpose(m);

    MT_PROFILE_LEAVE();
}

/*
================================================================================
 * ~~ [ matrix math ] ~~ *
--------------------------------------------------------------------------------
*/

/* TODO:
    Linear solvers:
        Upper and Lower Triangular Inverse
        LU Decomposition
        LU Reduction
        Block LU Decomposition
        Rank Factorization
        Cholesky Decomposition
        LDLT Decomposition
        QR Decomposition
        RRQR Factorization
        Interpolative Decomposition
        Gaussian Elimination
        Tridiagonal Inverse

    Eigenvalue solvers:
        Eigendecomposition
        Jordan Decomposition
        Hessenberg Reduction
        Schur Decomposition
        QZ Decomposition
        Takagi Factorization
        Singular Value Decomposition (SVD)

    Matrix is:
        P matrix
        Z matrix
        Q matrix
        M matrix

        Orthogonal
        Orthonormal

        Rotated
        Translated
        Scaled

        Positive Definite
        Symmetric Positive Definite
        Positive Semi Definite
        Symmetric Positive Semi Definite
*/

// TODO: move these tables to the header file in case anyone else might need them
static const size_t mat2x2_col_indices[ 4] =
{
    0, 2,
    1, 3
};

static const size_t mat3x3_col_indices[ 9] =
{
    0, 3, 6,
    1, 4, 7,
    2, 5, 8
};

static const size_t mat4x4_col_indices[16] =
{
    0, 4,  8, 12,
    1, 5,  9, 13,
    2, 6, 10, 14,
    3, 7, 11, 15
};

static const size_t matNxN_row_indices[16] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
};

static void mat4x4to_quat(float q[4], const float m[16], const size_t* const idx)
{
    MT_PROFILE_ENTER();

    // TODO: use reciprocal square mul instead of division?
    float sqrv, tracev = m[0] + m[5] + m[10];

    if (tracev > 0.00000001f)
    {
        sqrv = ae_sqrtf(1.0f + tracev) * 2.0f;
        q[0] = (m[idx[9]] - m[idx[6]]) / sqrv;
        q[1] = (m[idx[2]] - m[idx[8]]) / sqrv;
        q[2] = (m[idx[4]] - m[idx[1]]) / sqrv;
        q[3] = 0.25f * sqrv;
    }
    else if (m[0] > m[5] && m[0] > m[10])
    {
        sqrv = ae_sqrtf(1.0f + m[0] - m[5] - m[10]) * 2.0f;
        q[0] = 0.25f * sqrv;
        q[1] = (m[idx[4]] + m[idx[1]]) / sqrv;
        q[2] = (m[idx[2]] + m[idx[8]]) / sqrv;
        q[3] = (m[idx[9]] - m[idx[6]]) / sqrv;
    }
    else if (m[5] > m[10])
    {
        sqrv = ae_sqrtf(1.0f + m[5] - m[0] - m[10]) * 2.0f;
        q[0] = (m[idx[4]] + m[idx[1]]) / sqrv;
        q[1] = 0.25f * sqrv;
        q[2] = (m[idx[9]] + m[idx[6]]) / sqrv;
        q[3] = (m[idx[2]] - m[idx[8]]) / sqrv;
    }
    else
    {
        sqrv = ae_sqrtf(1.0f + m[10] - m[0] - m[5]) * 2.0f;
        q[0] = (m[idx[2]] + m[idx[8]]) / sqrv;
        q[1] = (m[idx[9]] + m[idx[6]]) / sqrv;
        q[2] = 0.25f * sqrv;
        q[3] = (m[idx[4]] - m[idx[1]]) / sqrv;
    }

    MT_PROFILE_LEAVE();
}

void mat4x4row_to_quat(float q[4], const float m[16])
{
    mat4x4to_quat(q, m, matNxN_row_indices);
}

void mat4x4col_to_quat(float q[4], const float m[16])
{
    mat4x4to_quat(q, m, mat4x4_col_indices);
}

int mat_inverse(float* out, const float* const m, const size_t n)
{
    AE_STUB(); return 0;
}

int mat2x2inverse(float out[ 4], const float mat[ 4])
{
    AE_STUB(); return 0;
}

int mat3x3inverse(float out[ 9], const float mat[ 9])
{
    AE_STUB(); return 0;
}

int mat4x4inverse(float out[16], const float mat[16])
{
    mat4x4copy(out, mat); return mat4x4invert(out);
}

int mat_invert(float* m, const size_t n)
{
    AE_STUB(); return 0;
}

int mat2x2invert(float mat[ 4])
{
    AE_STUB(); return 0;
}

int mat3x3invert(float mat[ 9])
{
    AE_STUB(); return 0;
}

int mat4x4invert(float mat[16])
{
    MT_PROFILE_ENTER();

    const float det2_01_01=(mat[0 * 4 + 0] * mat[1 * 4 + 1] -
                            mat[0 * 4 + 1] * mat[1 * 4 + 0]);
    const float det2_01_02=(mat[0 * 4 + 0] * mat[1 * 4 + 2] -
                            mat[0 * 4 + 2] * mat[1 * 4 + 0]);
    const float det2_01_03=(mat[0 * 4 + 0] * mat[1 * 4 + 3] -
                            mat[0 * 4 + 3] * mat[1 * 4 + 0]);
    const float det2_01_12=(mat[0 * 4 + 1] * mat[1 * 4 + 2] -
                            mat[0 * 4 + 2] * mat[1 * 4 + 1]);
    const float det2_01_13=(mat[0 * 4 + 1] * mat[1 * 4 + 3] -
                            mat[0 * 4 + 3] * mat[1 * 4 + 1]);
    const float det2_01_23=(mat[0 * 4 + 2] * mat[1 * 4 + 3] -
                            mat[0 * 4 + 3] * mat[1 * 4 + 2]);

    const float det3_201_012 = (mat[2 * 4 + 0] * det2_01_12 -
                                mat[2 * 4 + 1] * det2_01_02 +
                                mat[2 * 4 + 2] * det2_01_01);
    const float det3_201_013 = (mat[2 * 4 + 0] * det2_01_13 -
                                mat[2 * 4 + 1] * det2_01_03 +
                                mat[2 * 4 + 3] * det2_01_01);
    const float det3_201_023 = (mat[2 * 4 + 0] * det2_01_23 -
                                mat[2 * 4 + 2] * det2_01_03 +
                                mat[2 * 4 + 3] * det2_01_02);
    const float det3_201_123 = (mat[2 * 4 + 1] * det2_01_23 -
                                mat[2 * 4 + 2] * det2_01_13 +
                                mat[2 * 4 + 3] * det2_01_12);

    const float det = (-det3_201_123 * mat[3 * 4 + 0] +
                        det3_201_023 * mat[3 * 4 + 1] -
                        det3_201_013 * mat[3 * 4 + 2] +
                        det3_201_012 * mat[3 * 4 + 3]);

    if (ae_fabsf(det) < 1e-14) { MT_PROFILE_LEAVE(); return 0; } else
    {
        const float inv_det = 1.0f / det;

        const float det2_03_01=(mat[0 * 4 + 0] * mat[3 * 4 + 1] -
                                mat[0 * 4 + 1] * mat[3 * 4 + 0]);
        const float det2_03_02=(mat[0 * 4 + 0] * mat[3 * 4 + 2] -
                                mat[0 * 4 + 2] * mat[3 * 4 + 0]);
        const float det2_03_03=(mat[0 * 4 + 0] * mat[3 * 4 + 3] -
                                mat[0 * 4 + 3] * mat[3 * 4 + 0]);
        const float det2_03_12=(mat[0 * 4 + 1] * mat[3 * 4 + 2] -
                                mat[0 * 4 + 2] * mat[3 * 4 + 1]);
        const float det2_03_13=(mat[0 * 4 + 1] * mat[3 * 4 + 3] -
                                mat[0 * 4 + 3] * mat[3 * 4 + 1]);
        const float det2_03_23=(mat[0 * 4 + 2] * mat[3 * 4 + 3] -
                                mat[0 * 4 + 3] * mat[3 * 4 + 2]);

        const float det2_13_01=(mat[1 * 4 + 0] * mat[3 * 4 + 1] -
                                mat[1 * 4 + 1] * mat[3 * 4 + 0]);
        const float det2_13_02=(mat[1 * 4 + 0] * mat[3 * 4 + 2] -
                                mat[1 * 4 + 2] * mat[3 * 4 + 0]);
        const float det2_13_03=(mat[1 * 4 + 0] * mat[3 * 4 + 3] -
                                mat[1 * 4 + 3] * mat[3 * 4 + 0]);
        const float det2_13_12=(mat[1 * 4 + 1] * mat[3 * 4 + 2] -
                                mat[1 * 4 + 2] * mat[3 * 4 + 1]);
        const float det2_13_13=(mat[1 * 4 + 1] * mat[3 * 4 + 3] -
                                mat[1 * 4 + 3] * mat[3 * 4 + 1]);
        const float det2_13_23=(mat[1 * 4 + 2] * mat[3 * 4 + 3] -
                                mat[1 * 4 + 3] * mat[3 * 4 + 2]);

        const float det3_203_012 = (mat[2 * 4 + 0] * det2_03_12 -
                                    mat[2 * 4 + 1] * det2_03_02 +
                                    mat[2 * 4 + 2] * det2_03_01);
        const float det3_203_013 = (mat[2 * 4 + 0] * det2_03_13 -
                                    mat[2 * 4 + 1] * det2_03_03 +
                                    mat[2 * 4 + 3] * det2_03_01);
        const float det3_203_023 = (mat[2 * 4 + 0] * det2_03_23 -
                                    mat[2 * 4 + 2] * det2_03_03 +
                                    mat[2 * 4 + 3] * det2_03_02);
        const float det3_203_123 = (mat[2 * 4 + 1] * det2_03_23 -
                                    mat[2 * 4 + 2] * det2_03_13 +
                                    mat[2 * 4 + 3] * det2_03_12);

        const float det3_213_012 = (mat[2 * 4 + 0] * det2_13_12 -
                                    mat[2 * 4 + 1] * det2_13_02 +
                                    mat[2 * 4 + 2] * det2_13_01);
        const float det3_213_013 = (mat[2 * 4 + 0] * det2_13_13 -
                                    mat[2 * 4 + 1] * det2_13_03 +
                                    mat[2 * 4 + 3] * det2_13_01);
        const float det3_213_023 = (mat[2 * 4 + 0] * det2_13_23 -
                                    mat[2 * 4 + 2] * det2_13_03 +
                                    mat[2 * 4 + 3] * det2_13_02);
        const float det3_213_123 = (mat[2 * 4 + 1] * det2_13_23 -
                                    mat[2 * 4 + 2] * det2_13_13 +
                                    mat[2 * 4 + 3] * det2_13_12);

        const float det3_301_012 = (mat[3 * 4 + 0] * det2_01_12 -
                                    mat[3 * 4 + 1] * det2_01_02 +
                                    mat[3 * 4 + 2] * det2_01_01);
        const float det3_301_013 = (mat[3 * 4 + 0] * det2_01_13 -
                                    mat[3 * 4 + 1] * det2_01_03 +
                                    mat[3 * 4 + 3] * det2_01_01);
        const float det3_301_023 = (mat[3 * 4 + 0] * det2_01_23 -
                                    mat[3 * 4 + 2] * det2_01_03 +
                                    mat[3 * 4 + 3] * det2_01_02);
        const float det3_301_123 = (mat[3 * 4 + 1] * det2_01_23 -
                                    mat[3 * 4 + 2] * det2_01_13 +
                                    mat[3 * 4 + 3] * det2_01_12);

        mat[0 * 4 + 0] = - det3_213_123 * inv_det;
        mat[1 * 4 + 0] = + det3_213_023 * inv_det;
        mat[2 * 4 + 0] = - det3_213_013 * inv_det;
        mat[3 * 4 + 0] = + det3_213_012 * inv_det;

        mat[0 * 4 + 1] = + det3_203_123 * inv_det;
        mat[1 * 4 + 1] = - det3_203_023 * inv_det;
        mat[2 * 4 + 1] = + det3_203_013 * inv_det;
        mat[3 * 4 + 1] = - det3_203_012 * inv_det;

        mat[0 * 4 + 2] = + det3_301_123 * inv_det;
        mat[1 * 4 + 2] = - det3_301_023 * inv_det;
        mat[2 * 4 + 2] = + det3_301_013 * inv_det;
        mat[3 * 4 + 2] = - det3_301_012 * inv_det;

        mat[0 * 4 + 3] = - det3_201_123 * inv_det;
        mat[1 * 4 + 3] = + det3_201_023 * inv_det;
        mat[2 * 4 + 3] = - det3_201_013 * inv_det;
        mat[3 * 4 + 3] = + det3_201_012 * inv_det;

        MT_PROFILE_LEAVE(); return 1;
    }
}

float mat_determinant(const float* const mat, const size_t n)
{
    AE_STUB(); return 0.0f;
}

float mat2x2determinant(const float mat[ 4])
{
    return (mat[0 * 2 + 0] * mat[1 * 2 + 1] -
            mat[0 * 2 + 1] * mat[1 * 2 + 0]);
}

float mat3x3determinant(const float mat[ 9])
{
    // compute 2x2 matrix sub-determinants

    const float det2_12_01=(mat[1 * 3 + 0] * mat[2 * 3 + 1] -
                            mat[1 * 3 + 1] * mat[2 * 3 + 0]);
    const float det2_12_02=(mat[1 * 3 + 0] * mat[2 * 3 + 2] -
                            mat[1 * 3 + 2] * mat[2 * 3 + 0]);
    const float det2_12_12=(mat[1 * 3 + 1] * mat[2 * 3 + 2] -
                            mat[1 * 3 + 2] * mat[2 * 3 + 1]);

    return (det2_12_12 * mat[0 * 3 + 0] -
            det2_12_02 * mat[0 * 3 + 1] +
            det2_12_01 * mat[0 * 3 + 2]);
}

float mat4x4determinant(const float mat[16])
{
    // compute 2x2 matrix sub-determinants

    const float det2_01_01=(mat[0 * 4 + 0] * mat[1 * 4 + 1] -
                            mat[0 * 4 + 1] * mat[1 * 4 + 0]);
    const float det2_01_02=(mat[0 * 4 + 0] * mat[1 * 4 + 2] -
                            mat[0 * 4 + 2] * mat[1 * 4 + 0]);
    const float det2_01_03=(mat[0 * 4 + 0] * mat[1 * 4 + 3] -
                            mat[0 * 4 + 3] * mat[1 * 4 + 0]);
    const float det2_01_12=(mat[0 * 4 + 1] * mat[1 * 4 + 2] -
                            mat[0 * 4 + 2] * mat[1 * 4 + 1]);
    const float det2_01_13=(mat[0 * 4 + 1] * mat[1 * 4 + 3] -
                            mat[0 * 4 + 3] * mat[1 * 4 + 1]);
    const float det2_01_23=(mat[0 * 4 + 2] * mat[1 * 4 + 3] -
                            mat[0 * 4 + 3] * mat[1 * 4 + 2]);

    // compute 3x3 matrix sub-determinants

    const float det3_201_012 = (mat[2 * 4 + 0] * det2_01_12 -
                                mat[2 * 4 + 1] * det2_01_02 +
                                mat[2 * 4 + 2] * det2_01_01);
    const float det3_201_013 = (mat[2 * 4 + 0] * det2_01_13 -
                                mat[2 * 4 + 1] * det2_01_03 +
                                mat[2 * 4 + 3] * det2_01_01);
    const float det3_201_023 = (mat[2 * 4 + 0] * det2_01_23 -
                                mat[2 * 4 + 2] * det2_01_03 +
                                mat[2 * 4 + 3] * det2_01_02);
    const float det3_201_123 = (mat[2 * 4 + 1] * det2_01_23 -
                                mat[2 * 4 + 2] * det2_01_13 +
                                mat[2 * 4 + 3] * det2_01_12);

    return(-det3_201_123 * mat[3 * 4 + 0] +
            det3_201_023 * mat[3 * 4 + 1] -
            det3_201_013 * mat[3 * 4 + 2] +
            det3_201_012 * mat[3 * 4 + 3] );
}

/*
================================================================================
 * ~~ [ quadric shapes ] ~~ *
--------------------------------------------------------------------------------
*/

// TODO: it would be trivial to generalize this out to ellipsoid_vs_line_type
// TODO: version that stores both contact points for FPS entity exit wounds
// TODO: version that takes prenormalized ray normal and asserts unit length

int ellipsoid_vs_ray(float* h_dist, const float e_pos[3], const float e_ext[3],
                                const float ray_pos[3], const float ray_dir[3])
{
    float ray_origin[3], ray_normal[3], a, b, c, d;
    MT_PROFILE_ENTER();

    vec3sub_vec(ray_origin, ray_pos, e_pos);
    vec3normalize(ray_normal, ray_dir);

    // combine formulas for ray & ellipsoid, and solve with quadratic formula

    a = ((ray_normal[0] * ray_normal[0]) / (e_ext[0] * e_ext[0])) +
        ((ray_normal[1] * ray_normal[1]) / (e_ext[1] * e_ext[1])) +
        ((ray_normal[2] * ray_normal[2]) / (e_ext[2] * e_ext[2]));

    b = ((2.0f * ray_origin[0] * ray_normal[0]) / (e_ext[0] * e_ext[0])) +
        ((2.0f * ray_origin[1] * ray_normal[1]) / (e_ext[1] * e_ext[1])) +
        ((2.0f * ray_origin[2] * ray_normal[2]) / (e_ext[2] * e_ext[2]));

    c = ((ray_origin[0] * ray_origin[0]) / (e_ext[0] * e_ext[0])) +
        ((ray_origin[1] * ray_origin[1]) / (e_ext[1] * e_ext[1])) +
        ((ray_origin[2] * ray_origin[2]) / (e_ext[2] * e_ext[2])) - 1.0f;

    d = ((b * b) - (4.0f * a * c));

    // squared dist to closest hit; if it's negative, hitpoint is behind ray
    if (d < 0) { MT_PROFILE_LEAVE(); return 0; }

    if (h_dist != NULL)
    {
        d = ae_sqrtf(d); // keep the closest hit, either the first or second
        *h_dist = ae_fminf((-b + d) / (2.0f * a), (-b - d) / (2.0f * a));
    }

    MT_PROFILE_LEAVE(); return 1;
}
