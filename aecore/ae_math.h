/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
--- NOTE nothing in this library is guaranteed to deal with nan or inf properly
--- TODO C++ math wrapper classes; vectors and matrices templated on dimensions
----------------------------------------------------------------------------- */
#ifndef __AE_MATH_H__
#define __AE_MATH_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif
#ifndef __AE_ERROR_H__
#include <ae_error.h>
#endif
#ifndef __AE_LOGGER_H__
#include <ae_logger.h>
#endif
#ifndef __AE_MEMORY_H__
#include <ae_memory.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_math_init(int argc, char** argv);
void ae_math_quit(void); // private init

/*
================================================================================
 * ~~ [ integer math - base 2 logarithms ] ~~ *
--------------------------------------------------------------------------------
*/

// tricky use of a 4-bit table to identify 5 bit positions (note the '-1' here).
// 3-bit table would require another tree level; 5-bit table wouldn't save one.
static int ae_log2floor(unsigned int n)
{
    #if defined(_WIN32) && !defined(__MINGW32__) && !defined(__cplusplus)
        unsigned long i;

        _BitScanReverse(&i, n);
        return i != 0 ? i : -1;

    #elif defined(__GNUC__) && defined(__x86_64__)
        return n ? 31 - __builtin_clz(n) : -1;

    #else
        static signed char log2_4[16] = { -1,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3 };

        if          (n < (1U << 14)) /* 2 compares if n < 16, or 3 otherwise */
            if      (n < (1U <<  4)) return  0 + log2_4[n >>  0];
            else if (n < (1U <<  9)) return  5 + log2_4[n >>  5];
            else                     return 10 + log2_4[n >> 10];
        else if     (n < (1U << 24))
            if      (n < (1U << 19)) return 15 + log2_4[n >> 15];
            else                     return 20 + log2_4[n >> 20];
        else if     (n < (1U << 29)) return 25 + log2_4[n >> 25];
        else                         return 30 + log2_4[n >> 30];
    #endif
}

static int ae_log2ceil(unsigned int n)
{
    // if n is not a power of two, return ceil
    if (n & (n-1))  return ae_log2floor(n) + 1;
    else            return ae_log2floor(n);
}

/*
================================================================================
 * ~~ [ integer math - circular shifts ] ~~ *
--------------------------------------------------------------------------------
*/

#if defined(_MSC_VER)
    /*
     * Speed up bit sequence permutation for things like cryptography and hashing.
     */
    #define ae_barrel_rotate_right _rotr
    #define ae_barrel_rotate_left _rotl
#else
    /* According to the Wikipedia article I ganked this from, the compiler should
     * recognize this idiom and compile it down into one 32-bit rotate instruction.
     * Note that shift operations in C are only defined for shift values which are
     * not negative and smaller than sizeof(value) * CHAR_BIT, which is 32 here.
     * The mask, used with bitwise AND, prevents undefined behavior when the shift
     * count is zero or greater than or equal to the width of an unsigned integer.
     */
    static c_inline unsigned int
    ae_barrel_rotate_right(unsigned int value, unsigned int count)
    {
        const unsigned int mask = (CHAR_BIT * sizeof(value) - 1);
        count &= mask;
        return (value >> count) | (value << ((-count) & mask));
    }

    #define ae_barrel_rotate_left(value, shift) ((value << shift) + (value >> 25))
#endif

/*
================================================================================
 * ~~ [ integer math - factorization & primes ] ~~ *
--------------------------------------------------------------------------------
*/

/* Find all int factors, including 1 and the number itself for nonzero values.
 */
AE_DECL int AE_CALL ae_find_factors(int* factors, const int value);

/* Given the equation A*B=C, solve for A and B such that the difference between
 * them is minimal. Useful for even distribution of points across spheres, etc.
 */
AE_DECL void AE_CALL ae_closest_factors(int* a, int* b, const int value);

/* TODO: ae_find_primes function that finds all of the primes within a range.
 */
static c_inline int ae_is_prime(const int value)
{
    return value > 1 && ae_find_factors(NULL, value) == 2;
}

/*
================================================================================
 * ~~ [ integer math - miscellaneous ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline int ae_ipow(int base, int exp)
{
    int result = 1;
    while (exp)
    {
        if (exp & 1)
        {
            result *= base;
        }
        exp >>= 1;
        base *= base;
    }
    return result;
}

static c_inline int ae_iabs(const int x)
{
    #if 0
        if (x < 0) { return -x; } else { return x; }
    #elif 1
        return abs(x);
    #else
        // branchless implementation; int32 sign bit
        const int y = x >> 31; return (x ^ y) - y;
    #endif
}

static c_inline int ae_imin(const int x, const int y)
{
    if (x < y) { return x; } else { return y; }
}

static c_inline int ae_imax(const int x, const int y)
{
    if (x > y) { return x; } else { return y; }
}

static c_inline int ae_imin3(const int a, const int b, const int c)
{
    return ae_imin(a, ae_imin(b, c));
}

static c_inline int ae_imax3(const int a, const int b, const int c)
{
    return ae_imax(a, ae_imax(b, c));
}

static c_inline int ae_iclamp(const int x, const int mn, const int mx)
{
    return ae_imax(mn, ae_imin(mx, x));
}

/*
================================================================================
 * ~~ [ trig - radians to and from degrees ] ~~ *
--------------------------------------------------------------------------------
*/

#define ae_rad2degf(x) ((x) * (180.0f / 3.14159265358979323846f))
#define ae_deg2radf(x) ((x) * (3.14159265358979323846f / 180.0f))

#define ae_degreesf(x) ae_rad2degf(x) // from radians to degrees
#define ae_radiansf(x) ae_deg2radf(x) // from degrees to radians

/*
================================================================================
 * ~~ [ trig - radians to and from cartesian coordinates ] ~~ *
--------------------------------------------------------------------------------
*/

#define ae_atan2f atan2f

#define ae_cosf cosf
#define ae_sinf sinf
#define ae_tanf tanf

#define ae_acosf acosf
#define ae_asinf asinf
#define ae_atanf atanf

/* ===== [ sine and cosine within [0, 1] vertically ] ======================= */
/*
    NOTE: this is useful for flashing texture effects (scale theta for speed)
    NOTE: use 1.0 - ae_cosf_01 for waves that have an initial trough at zero
    TODO: a sawtooth wave function would give a smoother linear interpolation
*/

static c_inline float ae_sinf_01(const float theta)
{
    return ae_sinf(theta) * 0.5f + 0.5f;
}

static c_inline float ae_cosf_01(const float theta)
{
    return ae_cosf(theta) * 0.5f + 0.5f;
}

/* ===== [ sine and cosine within [0, period] horizontally ] ================ */

static c_inline float ae_sinf_period(const float theta, const float period)
{
    return ae_sinf(((2.0f * 3.14159265358979323846f) / period) * theta);
}

static c_inline float ae_cosf_period(const float theta, const float period)
{
    return ae_cosf(((2.0f * 3.14159265358979323846f) / period) * theta);
}

/* ===== [ combination of the above two methods ] =========================== */

static c_inline float ae_sinf_period_01(const float theta, const float period)
{
    return ae_sinf_period(theta, period) * 0.5f + 0.5f;
}

static c_inline float ae_cosf_period_01(const float theta, const float period)
{
    return ae_cosf_period(theta, period) * 0.5f + 0.5f;
}

/*
================================================================================
 * ~~ [ trig - viewing angles ] ~~ *
--------------------------------------------------------------------------------
NOTE: given a viewing frustum's Y field of view angle and aspect ratio (viewport
width / viewport height), compute the corresponding X angle of the view frustum.
--------------------------------------------------------------------------------
*/

static c_inline float ae_fovx_radiansf(const float fov_y, const float aspect)
{
    return 2.0f * ae_atanf(aspect * ae_tanf(fov_y / 2.0f));
}

static c_inline float ae_fovx_degreesf(const float fov_y, const float aspect)
{
    return ae_degreesf(ae_fovx_radiansf(ae_radiansf(fov_y), aspect));
}

/*
================================================================================
 * ~~ [ float math ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float ae_lerpf( const float a, const float b, const float t )
{
    return (1.0f - t) * a + t * b; // smoothly interpolate between two points
}

static c_inline float ae_unlerpf(const float a, const float b, const float t)
{
    return (t - a) / (b - a); // get the t value back from the output of lerp
}

#define ae_sqrtf sqrtf
#define ae_powf powf
#define ae_fabsf fabsf
#define ae_logf logf

static c_inline float ae_fminf(const float x, const float y)
{
    if (x < y) { return x; } else { return y; }
}

static c_inline float ae_fmaxf(const float x, const float y)
{
    if (x > y) { return x; } else { return y; }
}

static c_inline float ae_fmin3f(const float a, const float b, const float c)
{
    return ae_fminf(a, ae_fminf(b, c));
}

static c_inline float ae_fmax3f(const float a, const float b, const float c)
{
    return ae_fmaxf(a, ae_fmaxf(b, c));
}

static c_inline float ae_fmin4f(const float a, const float b,
                                const float c, const float d)
{
    return ae_fminf(a, ae_fmin3f(b, c, d));
}

static c_inline float ae_fmax4f(const float a, const float b,
                                const float c, const float d)
{
    return ae_fmaxf(a, ae_fmax3f(b, c, d));
}

static c_inline float ae_fclampf(const float x, const float mn, const float mx)
{
    return ae_fmaxf(mn, ae_fminf(mx, x));
}

/* TODO: round towards zero function (with associated ftoi mode)
 */
#define ae_floorf floorf
#define ae_ceilf ceilf

/* NOTE: not an alias for the POSIX math function roundf, which breaks
 * halfway ties by rounding towards zero instead of the nearest int.
 * this is basically an even less safe version of nearbyintf or rintf.
 */
static c_inline float ae_roundf(const float n)
{
    return ae_floorf(n + 0.5f);
}

typedef enum ae_ftoi_mode_t
{
    AE_FTOI_MODE_FLOOR,
    AE_FTOI_MODE_CEIL,
    AE_FTOI_MODE_ROUND,
    AE_FTOI_MODE_COUNT,
} \
    ae_ftoi_mode_t;

static c_inline int ae_ftoi_ex(float v, ae_ftoi_mode_t mode)
{
    switch (mode)
    {
        case AE_FTOI_MODE_FLOOR: return (int)ae_floorf(v);
        case AE_FTOI_MODE_CEIL:  return (int)ae_ceilf (v);
        case AE_FTOI_MODE_ROUND: return (int)ae_roundf(v);

        default: assert(0); return 0;
    }
}

static c_inline int ae_ftoi(float v)
{
    /* TODO optimize me! if i recall correctly, the c spec makes all sorts of
            expensive guarantees about what happens here (checks for nans etc).
            we can probably subvert those with some IEEE-754 bit munging >:)
     */
    assert((int)v == ae_ftoi_ex(v, AE_FTOI_MODE_FLOOR));
    return (int)v;
}

static c_inline float ae_normalize_deg_360f(float degrees)
{
    while (degrees <   0.0f) degrees += 360.0f;
    while (degrees > 360.0f) degrees -= 360.0f;

    return degrees;
}

static c_inline float ae_normalize_deg_180f(float degrees)
{
    while (degrees < -180.0f) degrees += 360.0f;
    while (degrees > +180.0f) degrees -= 360.0f;

    return degrees;
}

static c_inline float ae_normalize_rad_tauf(float radians)
{
    while (radians < 0.0f * (float)0.0f) radians += 2.0f * (float)M_PI;
    while (radians > 2.0f * (float)M_PI) radians -= 2.0f * (float)M_PI;

    return radians;
}

static c_inline float ae_normalize_rad_pif(float radians)
{
    while (radians < -(float)M_PI) radians += 2.0f * (float)M_PI;
    while (radians > +(float)M_PI) radians -= 2.0f * (float)M_PI;

    return radians;
}

/*
================================================================================
 * ~~ [ vector math ] ~~ *
--------------------------------------------------------------------------------
*/

#define ae_vector_alloc(n) ((float*)ae_calloc((n), sizeof(float)))
#define ae_vector_free(p, n) ae_free(p) // allocates n-dim vector

#define MAKE_ND_VECTOR_OP_FUNCS(NAME, OP)                                       \
    static c_inline void vec_ ## NAME ## _vec(                                  \
        float* out, const float* const a, const float* const b, const size_t n) \
    {                                                                           \
        size_t i = 0;                                                           \
        for (; i < n; i++) out[i] = a[i] OP b[i];                               \
    }                                                                           \
                                                                                \
    static c_inline void vec_ ## NAME ## _flt(                                  \
        float* out, const float* const v, const float f, const size_t n)        \
    {                                                                           \
        size_t i = 0;                                                           \
        for (; i < n; i++) out[i] = v[i] OP f;                                  \
    }                                                                           \

#define MAKE_VECTOR_OP_FUNC(NAME, OP)                                           \
    static c_inline void                                                        \
    vec2 ## NAME ## _vec(float out[2], const float a[2], const float b[2]) {    \
        out[0] = a[0] OP b[0]; out[1] = a[1] OP b[1];                           \
    }                                                                           \
    static c_inline void                                                        \
    vec3 ## NAME ## _vec(float out[3], const float a[3], const float b[3]) {    \
        out[0] = a[0] OP b[0]; out[1] = a[1] OP b[1]; out[2] = a[2] OP b[2];    \
    }                                                                           \
    static c_inline void                                                        \
    vec4 ## NAME ## _vec(float out[4], const float a[4], const float b[4]) {    \
        out[0] = a[0] OP b[0]; out[1] = a[1] OP b[1];                           \
        out[2] = a[2] OP b[2]; out[3] = a[3] OP b[3];                           \
    }                                                                           \

#define MAKE_SCALAR_OP_FUNC(NAME, OP)                                           \
    static c_inline void                                                        \
    vec2 ## NAME ## _flt(float out[2], const float v[2], const float f) {       \
        out[0] = v[0] OP f; out[1] = v[1] OP f;                                 \
    }                                                                           \
    static c_inline void                                                        \
    vec3 ## NAME ## _flt(float out[3], const float v[3], const float f) {       \
        out[0] = v[0] OP f; out[1] = v[1] OP f; out[2] = v[2] OP f;             \
    }                                                                           \
    static c_inline void                                                        \
    vec4 ## NAME ## _flt(float out[4], const float v[4], const float f) {       \
        out[0] = v[0] OP f; out[1] = v[1] OP f;                                 \
        out[2] = v[2] OP f; out[3] = v[3] OP f;                                 \
    }

MAKE_ND_VECTOR_OP_FUNCS(add, +);
MAKE_ND_VECTOR_OP_FUNCS(sub, -);
MAKE_ND_VECTOR_OP_FUNCS(mul, *);
MAKE_ND_VECTOR_OP_FUNCS(div, /);

#if 0
    #define vec2add_flt(out, v, f) vec_add_flt((out), (v), (f), 2)
    #define vec2add_vec(out, a, b) vec_add_vec((out), (a), (b), 2)
    #define vec3add_flt(out, v, f) vec_add_flt((out), (v), (f), 3)
    #define vec3add_vec(out, a, b) vec_add_vec((out), (a), (b), 3)
    #define vec4add_flt(out, v, f) vec_add_flt((out), (v), (f), 4)
    #define vec4add_vec(out, a, b) vec_add_vec((out), (a), (b), 4)

    #define vec2sub_flt(out, v, f) vec_sub_flt((out), (v), (f), 2)
    #define vec2sub_vec(out, a, b) vec_sub_vec((out), (a), (b), 2)
    #define vec3sub_flt(out, v, f) vec_sub_flt((out), (v), (f), 3)
    #define vec3sub_vec(out, a, b) vec_sub_vec((out), (a), (b), 3)
    #define vec4sub_flt(out, v, f) vec_sub_flt((out), (v), (f), 4)
    #define vec4sub_vec(out, a, b) vec_sub_vec((out), (a), (b), 4)

    #define vec2mul_flt(out, v, f) vec_mul_flt((out), (v), (f), 2)
    #define vec2mul_vec(out, a, b) vec_mul_vec((out), (a), (b), 2)
    #define vec3mul_flt(out, v, f) vec_mul_flt((out), (v), (f), 3)
    #define vec3mul_vec(out, a, b) vec_mul_vec((out), (a), (b), 3)
    #define vec4mul_flt(out, v, f) vec_mul_flt((out), (v), (f), 4)
    #define vec4mul_vec(out, a, b) vec_mul_vec((out), (a), (b), 4)

    #define vec2div_flt(out, v, f) vec_div_flt((out), (v), (f), 2)
    #define vec2div_vec(out, a, b) vec_div_vec((out), (a), (b), 2)
    #define vec3div_flt(out, v, f) vec_div_flt((out), (v), (f), 3)
    #define vec3div_vec(out, a, b) vec_div_vec((out), (a), (b), 3)
    #define vec4div_flt(out, v, f) vec_div_flt((out), (v), (f), 4)
    #define vec4div_vec(out, a, b) vec_div_vec((out), (a), (b), 4)
#else
    MAKE_VECTOR_OP_FUNC(add, +);
    MAKE_VECTOR_OP_FUNC(sub, -);
    MAKE_VECTOR_OP_FUNC(mul, *);
    MAKE_VECTOR_OP_FUNC(div, /);

    MAKE_SCALAR_OP_FUNC(add, +);
    MAKE_SCALAR_OP_FUNC(sub, -);
    MAKE_SCALAR_OP_FUNC(mul, *);

    static c_inline void vec2div_flt(float out[2], const float v[2], const float f)
    {
        vec2mul_flt(out, v, 1.0f / f);
    }

    static c_inline void vec3div_flt(float out[3], const float v[3], const float f)
    {
        vec3mul_flt(out, v, 1.0f / f);
    }

    static c_inline void vec4div_flt(float out[4], const float v[4], const float f)
    {
        vec4mul_flt(out, v, 1.0f / f);
    }
#endif

#define vec2translate vec2add_vec
#define vec3translate vec3add_vec
#define vec4translate vec4add_vec

#define vec2scale vec2mul_flt
#define vec3scale vec3mul_flt
#define vec4scale vec4mul_flt

/* ===== [ other vector functions ] ========================================= */

#define vec_copy(dst, src, n) memcpy((dst), (src), (n) * sizeof(float))

#define vec2copy(dst, src) vec_copy((dst), (src), 2)
#define vec3copy(dst, src) vec_copy((dst), (src), 3)
#define vec4copy(dst, src) vec_copy((dst), (src), 4)

#define vec_zero(v, n) memset((v), 0, (n) * sizeof(float))

#define vec2zero(v) vec_zero((v), 2)
#define vec3zero(v) vec_zero((v), 3)
#define vec4zero(v) vec_zero((v), 4)

#define vec_cmp(a, b, n) (memcmp((a), (b), (n) * sizeof(float)) == 0)

#define vec2cmp(a, b) vec_cmp((a), (b), 2)
#define vec3cmp(a, b) vec_cmp((a), (b), 3)
#define vec4cmp(a, b) vec_cmp((a), (b), 4)

static c_inline int // compare function that tolerates difference between vecs
vec_cmp_e(const float* const a, const float* const b, float epsilon, size_t n)
{
    size_t i = 0;
    for (; i < n; i++)
    {
        if (ae_fabsf(a[i] - b[i]) > epsilon) return 0;
    }
    return 1;
}

#if 0
    #define vec2cmp_e(a, b, e) vec_cmp_e((a), (b), (e), 2)
    #define vec3cmp_e(a, b, e) vec_cmp_e((a), (b), (e), 3)
    #define vec4cmp_e(a, b, e) vec_cmp_e((a), (b), (e), 4)
#else
    static c_inline int
    vec2cmp_e(const float a[2], const float b[2], const float epsilon)
    {
        if (ae_fabsf(a[0] - b[0]) > epsilon) return 0;
        if (ae_fabsf(a[1] - b[1]) > epsilon) return 0;

        return 1;
    }

    static c_inline int
    vec3cmp_e(const float a[3], const float b[3], const float epsilon)
    {
        if (ae_fabsf(a[0] - b[0]) > epsilon) return 0;
        if (ae_fabsf(a[1] - b[1]) > epsilon) return 0;
        if (ae_fabsf(a[2] - b[2]) > epsilon) return 0;

        return 1;
    }

    static c_inline int
    vec4cmp_e(const float a[4], const float b[4], const float epsilon)
    {
        if (ae_fabsf(a[0] - b[0]) > epsilon) return 0;
        if (ae_fabsf(a[1] - b[1]) > epsilon) return 0;
        if (ae_fabsf(a[2] - b[2]) > epsilon) return 0;
        if (ae_fabsf(a[3] - b[3]) > epsilon) return 0;

        return 1;
    }
#endif

static c_inline int vec_is_zero(const float* const v, const size_t n)
{
    size_t i = 0;
    for (; i < n; i++)
    {
        if (v[i]) return 0;
    }
    return 1;
}

#if 0
    #define vec2is_zero(v) vec_is_zero((v), 2)
    #define vec3is_zero(v) vec_is_zero((v), 3)
    #define vec4is_zero(v) vec_is_zero((v), 4)
#else
    static c_inline int vec2is_zero(const float* const v)
    {
        return !(v[0] || v[1]);
    }

    static c_inline int vec3is_zero(const float* const v)
    {
        return !(v[0] || v[1] || v[2]);
    }

    static c_inline int vec4is_zero(const float* const v)
    {
        return !(v[0] || v[1] || v[2] || v[3]);
    }
#endif

static c_inline int
vec_is_zero_e(const float* const v, const float epsilon, const size_t n)
{
    size_t i = 0;
    for (; i < n; i++)
    {
        if (ae_fabsf(v[i]) > epsilon) return 0;
    }
    return 1;
}

#if 0
    #define vec2is_zero_e(v, e) vec_is_zero_e((v), (e), 2)
    #define vec3is_zero_e(v, e) vec_is_zero_e((v), (e), 3)
    #define vec4is_zero_e(v, e) vec_is_zero_e((v), (e), 4)
#else
    static c_inline int vec2is_zero_e(const float v[2], const float epsilon)
    {
        if (ae_fabsf(v[0]) > epsilon) return 0;
        if (ae_fabsf(v[1]) > epsilon) return 0;

        return 1;
    }

    static c_inline int vec3is_zero_e(const float v[3], const float epsilon)
    {
        if (ae_fabsf(v[0]) > epsilon) return 0;
        if (ae_fabsf(v[1]) > epsilon) return 0;
        if (ae_fabsf(v[2]) > epsilon) return 0;

        return 1;
    }

    static c_inline int vec4is_zero_e(const float v[4], const float epsilon)
    {
        if (ae_fabsf(v[0]) > epsilon) return 0;
        if (ae_fabsf(v[1]) > epsilon) return 0;
        if (ae_fabsf(v[2]) > epsilon) return 0;
        if (ae_fabsf(v[3]) > epsilon) return 0;

        return 1;
    }
#endif

// TODO: should we do a kahan sum here instead for better precision?
// check compiler options to make sure the optimizer won't invalidate
// it, or if we already have enough precision in our rounding mode.

static c_inline float vec_sum(const float* const v, const size_t n)
{
    float vsum = 0.0f;
    size_t i = 0;
    for (; i < n; i++)
    {
        vsum += v[i];
    }
    return vsum;
}

#if 0
    #define vec2sum(v) vec_sum((v), 2)
    #define vec3sum(v) vec_sum((v), 3)
    #define vec4sum(v) vec_sum((v), 4)
#else
    static c_inline float vec2sum(const float v[2])
    {
        return v[0] + v[1];
    }

    static c_inline float vec3sum(const float v[3])
    {
        return v[0] + v[1] + v[2];
    }

    static c_inline float vec4sum(const float v[4])
    {
        return v[0] + v[1] + v[2] + v[3];
    }
#endif

static c_inline float // compute n-dimensional vector inner product
vec_dot(const float* const a, const float* const b, const size_t n)
{
    float  v = 0.0f;
    size_t i = 0;
    for (; i < n; i++)
    {
        v += a[i] * b[i];
    }
    return v;
}

#if 0
    #define vec2dot(a, b) vec_dot((a), (b), 2)
    #define vec3dot(a, b) vec_dot((a), (b), 3)
    #define vec4dot(a, b) vec_dot((a), (b), 4)
#else
    static c_inline float vec2dot(const float a[2], const float b[2])
    {
        return a[0] * b[0] + a[1] * b[1];
    }

    static c_inline float vec3dot(const float a[3], const float b[3])
    {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
    }

    static c_inline float vec4dot(const float a[4], const float b[4])
    {
        return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
    }
#endif

static c_inline float vec_mag2(const float* const v, const size_t n)
{
    return vec_dot(v, v, n);
}

#if 0
    #define vec2mag2(v) vec_mag2((v), 2)
    #define vec3mag2(v) vec_mag2((v), 3)
    #define vec4mag2(v) vec_mag2((v), 4)
#else
    static c_inline float vec2mag2(const float v[2])
    {
        return vec2dot(v, v);
    }

    static c_inline float vec3mag2(const float v[3])
    {
        return vec3dot(v, v);
    }

    static c_inline float vec4mag2(const float v[4])
    {
        return vec4dot(v, v);
    }
#endif

static c_inline float vec_mag(const float* const v, const size_t n)
{
    return ae_sqrtf(vec_mag2(v, n));
}

#if 0
    #define vec2mag(v) vec_mag((v), 2)
    #define vec3mag(v) vec_mag((v), 3)
    #define vec4mag(v) vec_mag((v), 4)
#else
    static c_inline float vec2mag(const float v[2])
    {
        return ae_sqrtf(vec2mag2(v));
    }

    static c_inline float vec3mag(const float v[3])
    {
        return ae_sqrtf(vec3mag2(v));
    }

    static c_inline float vec4mag(const float v[4])
    {
        return ae_sqrtf(vec4mag2(v));
    }
#endif

static c_inline float
vec_dist2(const float* const a, const float* const b, const size_t n)
{
    #if 0
        float d[4];

        ae_assert(n <= sizeof(d) / sizeof(d[0]), "count %u > max %u",
                (unsigned)n, (unsigned)( sizeof(d) / sizeof(d[0]) ));

        vec_sub_vec(d, a, b, n);
        return vec_mag2( d, n );
    #else
        float mag2 = 0.0f;
        size_t i = 0;

        for (; i < n; i++)
        {
            const float d = a[i] - b[i];
            mag2 += d * d;
        }

        return mag2;
    #endif
}

#if 0
    #define vec2dist2(a, b) vec_dist2((a), (b), 2)
    #define vec3dist2(a, b) vec_dist2((a), (b), 3)
    #define vec4dist2(a, b) vec_dist2((a), (b), 4)
#else
    static c_inline float vec2dist2(const float a[2], const float b[2])
    {
        float d[2]; vec2sub_vec(d, a, b); return vec2mag2(d);
    }

    static c_inline float vec3dist2(const float a[3], const float b[3])
    {
        float d[3]; vec3sub_vec(d, a, b); return vec3mag2(d);
    }

    static c_inline float vec4dist2(const float a[4], const float b[4])
    {
        float d[4]; vec4sub_vec(d, a, b); return vec4mag2(d);
    }
#endif

static c_inline float
vec_dist(const float* const a, const float* const b, const size_t n)
{
    return ae_sqrtf(vec_dist2(a, b, n));
}

#if 0
    #define vec2dist(a, b) vec_dist((a), (b), 2)
    #define vec3dist(a, b) vec_dist((a), (b), 3)
    #define vec4dist(a, b) vec_dist((a), (b), 4)
#else
    static c_inline float vec2dist(const float a[2], const float b[2])
    {
        return ae_sqrtf(vec2dist2(a, b));
    }

    static c_inline float vec3dist(const float a[3], const float b[3])
    {
        return ae_sqrtf(vec3dist2(a, b));
    }

    static c_inline float vec4dist(const float a[4], const float b[4])
    {
        return ae_sqrtf(vec4dist2(a, b));
    }
#endif

static c_inline float
vec_angle(const float* const a, const float* const b, const size_t n)
{
    return ae_acosf(vec_dot(a, b, n) / (vec_mag(a, n) * vec_mag(b, n)));
}

#if 0
    #define vec2angle(a, b) vec_angle((a), (b), 2)
    #define vec3angle(a, b) vec_angle((a), (b), 3)
    #define vec4angle(a, b) vec_angle((a), (b), 4)
#else
    static c_inline float vec2angle(const float a[2], const float b[2])
    {
        return ae_acosf(vec2dot(a, b) / (vec2mag(a) * vec2mag(b)));
    }

    static c_inline float vec3angle(const float a[3], const float b[3])
    {
        return ae_acosf(vec3dot(a, b) / (vec3mag(a) * vec3mag(b)));
    }

    static c_inline float vec4angle(const float a[4], const float b[4])
    {
        return ae_acosf(vec4dot(a, b) / (vec4mag(a) * vec4mag(b)));
    }
#endif

static c_inline void // negate every element from v
vec_neg(float* out, const float* const v, size_t n)
{
    size_t i = 0;
    for (; i < n; i++) out[i] = -v[i];
}

#if 0
    #define vec2neg(out, v) vec_neg((out), (v), 2)
    #define vec3neg(out, v) vec_neg((out), (v), 3)
    #define vec4neg(out, v) vec_neg((out), (v), 4)
#else
    static c_inline void vec2neg(float dst[2], const float src[2])
    {
        dst[0] = -src[0]; dst[1] = -src[1];
    }

    static c_inline void vec3neg(float dst[3], const float src[3])
    {
        dst[0] = -src[0]; dst[1] = -src[1]; dst[2] = -src[2];
    }

    static c_inline void vec4neg(float dst[4], const float src[4])
    {
        dst[0] = -src[0]; dst[1] = -src[1];
        dst[2] = -src[2]; dst[3] = -src[3];
    }
#endif

static c_inline void vec_reciprocal(float* out, const float* const v, size_t n)
{
    size_t i = 0;
    for (; i < n; i++) out[i] = 1.0f / v[i];
}

#if 0
    #define vec2reciprocal(out, v) vec_reciprocal((out), (v), 2)
    #define vec3reciprocal(out, v) vec_reciprocal((out), (v), 3)
    #define vec4reciprocal(out, v) vec_reciprocal((out), (v), 4)
#else
    static c_inline void vec2reciprocal(float out[2], const float v[2])
    {
        out[0] = 1.0f / v[0];
        out[1] = 1.0f / v[1];
    }

    static c_inline void vec3reciprocal(float out[3], const float v[3])
    {
        out[0] = 1.0f / v[0];
        out[1] = 1.0f / v[1];
        out[2] = 1.0f / v[2];
    }

    static c_inline void vec4reciprocal(float out[4], const float v[4])
    {
        out[0] = 1.0f / v[0];
        out[1] = 1.0f / v[1];
        out[2] = 1.0f / v[2];
        out[3] = 1.0f / v[3];
    }
#endif

static c_inline float // set n-dimensional vector length to 1.0
vec_normalize(float* out, const float* const v, const size_t n)
{
    /*  there are faster ways to do a reciprocal square root (_mm_rsqrt_ss,
        "carmack's" famous quake 3 fast inverse square root trick), but
        they sacrifice an unacceptable level of floating point precision.
    */
    const float sqrlen = vec_mag2(v, n); // sqrlen * invlen = mag
    const float invlen = sqrlen ? 1.0f / ae_sqrtf(sqrlen) : 1.0f;

    size_t i = 0;
    for (; i < n; i++)
    {
        out[i] = v[i] * invlen;
    }

    return sqrlen * invlen;
}

#if 0
    #define vec2normalize(out, v) vec_normalize((out), (v), 2)
    #define vec3normalize(out, v) vec_normalize((out), (v), 3)
    #define vec4normalize(out, v) vec_normalize((out), (v), 4)
#else
    static c_inline float vec2normalize(float out[2], const float v[2])
    {
        const float sqrlen = vec2mag2(v); /* sqrlen * invlen = mag */
        const float invlen = sqrlen ? 1.0f / ae_sqrtf(sqrlen) : 1.0f;

        out[0] = v[0] * invlen;
        out[1] = v[1] * invlen;
        return sqrlen * invlen;
    }

    static c_inline float vec3normalize(float out[3], const float v[3])
    {
        const float sqrlen = vec3mag2(v); /* sqrlen * invlen = mag */
        const float invlen = sqrlen ? 1.0f / ae_sqrtf(sqrlen) : 1.0f;

        out[0] = v[0] * invlen;
        out[1] = v[1] * invlen;
        out[2] = v[2] * invlen;
        return sqrlen * invlen;
    }

    static c_inline float vec4normalize(float out[4], const float v[4])
    {
        const float sqrlen = vec4mag2(v); /* sqrlen * invlen = mag */
        const float invlen = sqrlen ? 1.0f / ae_sqrtf(sqrlen) : 1.0f;

        out[0] = v[0] * invlen;
        out[1] = v[1] * invlen;
        out[2] = v[2] * invlen;
        out[3] = v[3] * invlen;
        return sqrlen * invlen;
    }
#endif

static c_inline void
vec_min_vec(float* out, const float* const a, const float* const b, const size_t n)
{
    size_t i = 0;
    for (; i < n; i++) out[i] = ae_fminf(a[i], b[i]);
}

static c_inline void
vec_max_vec(float* out, const float* const a, const float* const b, const size_t n)
{
    size_t i = 0;
    for (; i < n; i++) out[i] = ae_fmaxf(a[i], b[i]);
}

#if 0
    #define vec2min_vec(out, a, b) vec_min_vec((out), (a), (b), 2)
    #define vec3min_vec(out, a, b) vec_min_vec((out), (a), (b), 3)
    #define vec4min_vec(out, a, b) vec_min_vec((out), (a), (b), 4)

    #define vec2max_vec(out, a, b) vec_max_vec((out), (a), (b), 2)
    #define vec3max_vec(out, a, b) vec_max_vec((out), (a), (b), 3)
    #define vec4max_vec(out, a, b) vec_max_vec((out), (a), (b), 4)
#else
    static c_inline void vec2min_vec(float out[2], const float a[2], const float b[2])
    {
        out[0] = ae_fminf(a[0], b[0]);
        out[1] = ae_fminf(a[1], b[1]);
    }

    static c_inline void vec3min_vec(float out[3], const float a[3], const float b[3])
    {
        out[0] = ae_fminf(a[0], b[0]);
        out[1] = ae_fminf(a[1], b[1]);
        out[2] = ae_fminf(a[2], b[2]);
    }

    static c_inline void vec4min_vec(float out[4], const float a[4], const float b[4])
    {
        out[0] = ae_fminf(a[0], b[0]);
        out[1] = ae_fminf(a[1], b[1]);
        out[2] = ae_fminf(a[2], b[2]);
        out[3] = ae_fminf(a[3], b[3]);
    }

    static c_inline void vec2max_vec(float out[2], const float a[2], const float b[2])
    {
        out[0] = ae_fmaxf(a[0], b[0]);
        out[1] = ae_fmaxf(a[1], b[1]);
    }

    static c_inline void vec3max_vec(float out[3], const float a[3], const float b[3])
    {
        out[0] = ae_fmaxf(a[0], b[0]);
        out[1] = ae_fmaxf(a[1], b[1]);
        out[2] = ae_fmaxf(a[2], b[2]);
    }

    static c_inline void vec4max_vec(float out[4], const float a[4], const float b[4])
    {
        out[0] = ae_fmaxf(a[0], b[0]);
        out[1] = ae_fmaxf(a[1], b[1]);
        out[2] = ae_fmaxf(a[2], b[2]);
        out[3] = ae_fmaxf(a[3], b[3]);
    }
#endif

static c_inline void
vec_min_flt(float* out, const float* const vec, const float fv, const size_t n)
{
    size_t i = 0;
    for (; i < n; i++) out[i] = ae_fminf(vec[i], fv);
}

static c_inline void
vec_max_flt(float* out, const float* const vec, const float fv, const size_t n)
{
    size_t i = 0;
    for (; i < n; i++) out[i] = ae_fmaxf(vec[i], fv);
}

#if 0
    #define vec2min_flt(out, v, f) vec_min_flt((out), (v), (f), 2)
    #define vec3min_flt(out, v, f) vec_min_flt((out), (v), (f), 3)
    #define vec4min_flt(out, v, f) vec_min_flt((out), (v), (f), 4)

    #define vec2max_flt(out, v, f) vec_max_flt((out), (v), (f), 2)
    #define vec3max_flt(out, v, f) vec_max_flt((out), (v), (f), 3)
    #define vec4max_flt(out, v, f) vec_max_flt((out), (v), (f), 4)
#else
    static c_inline void vec2min_flt(float out[2], const float v[2], const float f)
    {
        out[0] = ae_fminf(v[0], f);
        out[1] = ae_fminf(v[1], f);
    }

    static c_inline void vec3min_flt(float out[3], const float v[3], const float f)
    {
        out[0] = ae_fminf(v[0], f);
        out[1] = ae_fminf(v[1], f);
        out[2] = ae_fminf(v[2], f);
    }

    static c_inline void vec4min_flt(float out[4], const float v[4], const float f)
    {
        out[0] = ae_fminf(v[0], f);
        out[1] = ae_fminf(v[1], f);
        out[2] = ae_fminf(v[2], f);
        out[3] = ae_fminf(v[3], f);
    }

    static c_inline void vec2max_flt(float out[2], const float v[2], const float f)
    {
        out[0] = ae_fmaxf(v[0], f);
        out[1] = ae_fmaxf(v[1], f);
    }

    static c_inline void vec3max_flt(float out[3], const float v[3], const float f)
    {
        out[0] = ae_fmaxf(v[0], f);
        out[1] = ae_fmaxf(v[1], f);
        out[2] = ae_fmaxf(v[2], f);
    }

    static c_inline void vec4max_flt(float out[4], const float v[4], const float f)
    {
        out[0] = ae_fmaxf(v[0], f);
        out[1] = ae_fmaxf(v[1], f);
        out[2] = ae_fmaxf(v[2], f);
        out[3] = ae_fmaxf(v[3], f);
    }
#endif

static c_inline void vec_clamp_vec( float* out, const float* const v,
        const float* const mn, const float* const mx, const size_t n)
{
    size_t i = 0;
    for (; i < n; i++) out[i] = ae_fclampf(v[i], mn[i], mx[i]);
}

#if 0
    #define vec2clamp_vec(out, v, mn, mx) vec_clamp_vec((out), (v), (mn), (mx), 2)
    #define vec3clamp_vec(out, v, mn, mx) vec_clamp_vec((out), (v), (mn), (mx), 3)
    #define vec4clamp_vec(out, v, mn, mx) vec_clamp_vec((out), (v), (mn), (mx), 4)
#else
    static c_inline void vec2clamp_vec(float out[2], const float v[2],
                                const float mn[2], const float mx[2])
    {
        out[0] = ae_fclampf(v[0], mn[0], mx[0]);
        out[1] = ae_fclampf(v[1], mn[1], mx[1]);
    }

    static c_inline void vec3clamp_vec(float out[3], const float v[3],
                                const float mn[3], const float mx[3])
    {
        out[0] = ae_fclampf(v[0], mn[0], mx[0]);
        out[1] = ae_fclampf(v[1], mn[1], mx[1]);
        out[2] = ae_fclampf(v[2], mn[2], mx[2]);
    }

    static c_inline void vec4clamp_vec(float out[4], const float v[4],
                                const float mn[4], const float mx[4])
    {
        out[0] = ae_fclampf(v[0], mn[0], mx[0]);
        out[1] = ae_fclampf(v[1], mn[1], mx[1]);
        out[2] = ae_fclampf(v[2], mn[2], mx[2]);
        out[3] = ae_fclampf(v[3], mn[3], mx[3]);
    }
#endif

static c_inline void vec_clamp_flt( float * out, const float * const v,
                        const float mn, const float mx, const size_t n)
{
    size_t i = 0;
    for (; i < n; i++) out[i] = ae_fclampf(v[i], mn, mx);
}

#if 0
    #define vec2clamp_flt(out, v, mn, mx) vec_clamp_flt((out), (v), (mn), (mx), 2)
    #define vec3clamp_flt(out, v, mn, mx) vec_clamp_flt((out), (v), (mn), (mx), 3)
    #define vec4clamp_flt(out, v, mn, mx) vec_clamp_flt((out), (v), (mn), (mx), 4)
#else
    static c_inline void vec2clamp_flt(float out[2], const float v[2],
                                const float min_v, const float max_v)
    {
        out[0] = ae_fclampf(v[0], min_v, max_v);
        out[1] = ae_fclampf(v[1], min_v, max_v);
    }

    static c_inline void vec3clamp_flt(float out[3], const float v[3],
                                const float min_v, const float max_v)
    {
        out[0] = ae_fclampf(v[0], min_v, max_v);
        out[1] = ae_fclampf(v[1], min_v, max_v);
        out[2] = ae_fclampf(v[2], min_v, max_v);
    }

    static c_inline void vec4clamp_flt(float out[4], const float v[4],
                                const float min_v, const float max_v)
    {
        out[0] = ae_fclampf(v[0], min_v, max_v);
        out[1] = ae_fclampf(v[1], min_v, max_v);
        out[2] = ae_fclampf(v[2], min_v, max_v);
        out[3] = ae_fclampf(v[3], min_v, max_v);
    }
#endif

static c_inline int vec_is_unit(const float* const v, const size_t n)
{
    return vec_mag2(v, n) == 1.0f;
}

#if 0
    #define vec2is_unit(v) vec_is_unit((v), 2)
    #define vec3is_unit(v) vec_is_unit((v), 3)
    #define vec4is_unit(v) vec_is_unit((v), 4)
#else
    static c_inline int vec2is_unit(const float v[2])
    {
        return vec2mag2(v) == 1.0f;
    }

    static c_inline int vec3is_unit(const float v[3])
    {
        return vec3mag2(v) == 1.0f;
    }

    static c_inline int vec4is_unit(const float v[4])
    {
        return vec4mag2(v) == 1.0f;
    }
#endif

static c_inline int
vec_is_unit_e(const float* const v, const float epsilon, const size_t n)
{
    return ae_fabsf(vec_mag2(v, n) - 1.0f) < epsilon;
}

#if 0
    #define vec2is_unit_e(v, e) vec_is_unit_e((v), (e), 2)
    #define vec3is_unit_e(v, e) vec_is_unit_e((v), (e), 3)
    #define vec4is_unit_e(v, e) vec_is_unit_e((v), (e), 4)
#else
    static c_inline int vec2is_unit_e(const float v[2], const float epsilon)
    {
        return ae_fabsf(vec2mag2(v) - 1.0f) < epsilon;
    }

    static c_inline int vec3is_unit_e(const float v[3], const float epsilon)
    {
        return ae_fabsf(vec3mag2(v) - 1.0f) < epsilon;
    }

    static c_inline int vec4is_unit_e(const float v[4], const float epsilon)
    {
        return ae_fabsf(vec4mag2(v) - 1.0f) < epsilon;
    }
#endif

static c_inline int vec_fix_denormals_ex(float* v, const float denormal, const size_t n)
{
    int has_denormal = 0;

    size_t i = 0;
    for (; i < n; i++)
    {
        // floor tiny vals in the vec to zero to avoid performance issues
        if (ae_fabsf(v[i]) < denormal) { v[i] = 0.0f; has_denormal = 1; }
    }

    return has_denormal;
}

static c_inline int vec_fix_denormals(float* v, const size_t n)
{
    return vec_fix_denormals_ex(v, 1e-30f, n);
}

#if 0
    #define vec2fix_denormals_ex(v, d) vec_fix_denormals_ex((v), (d), 2)
    #define vec3fix_denormals_ex(v, d) vec_fix_denormals_ex((v), (d), 3)
    #define vec4fix_denormals_ex(v, d) vec_fix_denormals_ex((v), (d), 4)
#else
    static c_inline int vec2fix_denormals_ex(float v[2], const float denormal)
    {
        int has_denormal = 0;

        if (ae_fabsf(v[0]) < denormal) { v[0] = 0.0f; has_denormal = 1; }
        if (ae_fabsf(v[1]) < denormal) { v[1] = 0.0f; has_denormal = 1; }

        return has_denormal;
    }

    static c_inline int vec3fix_denormals_ex(float v[3], const float denormal)
    {
        int has_denormal = 0;

        if (ae_fabsf(v[0]) < denormal) { v[0] = 0.0f; has_denormal = 1; }
        if (ae_fabsf(v[1]) < denormal) { v[1] = 0.0f; has_denormal = 1; }
        if (ae_fabsf(v[2]) < denormal) { v[2] = 0.0f; has_denormal = 1; }

        return has_denormal;
    }

    static c_inline int vec4fix_denormals_ex(float v[4], const float denormal)
    {
        int has_denormal = 0;

        if (ae_fabsf(v[0]) < denormal) { v[0] = 0.0f; has_denormal = 1; }
        if (ae_fabsf(v[1]) < denormal) { v[1] = 0.0f; has_denormal = 1; }
        if (ae_fabsf(v[2]) < denormal) { v[2] = 0.0f; has_denormal = 1; }
        if (ae_fabsf(v[3]) < denormal) { v[3] = 0.0f; has_denormal = 1; }

        return has_denormal;
    }
#endif

#if 0
    #define vec2fix_denormals(v) vec_fix_denormals((v), 2)
    #define vec3fix_denormals(v) vec_fix_denormals((v), 3)
    #define vec4fix_denormals(v) vec_fix_denormals((v), 4)
#else
    #define vec2fix_denormals(v) vec2fix_denormals_ex((v), 1e-30f)
    #define vec3fix_denormals(v) vec3fix_denormals_ex((v), 1e-30f)
    #define vec4fix_denormals(v) vec4fix_denormals_ex((v), 1e-30f)
#endif

static c_inline void vec_reflect(float* out, const float* const v,
                        const float* const normal, const size_t n)
{
    const float d = 2.0f * vec_dot(v, normal, n);

    size_t i = 0; // reflect vector about the normal
    for (; i < n; i++) out[i] = v[i] - d * normal[i];
}

#if 0
    #define vec2reflect(out, v, n) vec_reflect((out), (v), (n), 2)
    #define vec3reflect(out, v, n) vec_reflect((out), (v), (n), 3)
    #define vec4reflect(out, v, n) vec_reflect((out), (v), (n), 4)
#else
    static c_inline void vec2reflect(float v_out[2],
            const float v[2], const float normal[2])
    {
        const float d = 2.0f * vec2dot(v, normal);

        v_out[0] = v[0] - d * normal[0];
        v_out[1] = v[1] - d * normal[1];
    }

    static c_inline void vec3reflect(float v_out[3],
            const float v[3], const float normal[3])
    {
        const float d = 2.0f * vec3dot(v, normal);

        v_out[0] = v[0] - d * normal[0];
        v_out[1] = v[1] - d * normal[1];
        v_out[2] = v[2] - d * normal[2];
    }

    static c_inline void vec4reflect(float v_out[4],
            const float v[4], const float normal[4])
    {
        const float d = 2.0f * vec4dot(v, normal);

        v_out[0] = v[0] - d * normal[0];
        v_out[1] = v[1] - d * normal[1];
        v_out[2] = v[2] - d * normal[2];
        v_out[3] = v[3] - d * normal[3];
    }
#endif

static c_inline void vec_madd(float * out, const float * const a,
            const float * const b, const float s, const size_t n)
{
    size_t i = 0; // TODO: try using c stdlib fma
    for (; i < n; i++) out[i] = a[i] + s * b[i];
}

#if 0
    #define vec2madd(out, a, b, s) vec_madd((out), (a), (b), (s), 2)
    #define vec3madd(out, a, b, s) vec_madd((out), (a), (b), (s), 3)
    #define vec4madd(out, a, b, s) vec_madd((out), (a), (b), (s), 4)
#else
    static c_inline void vec2madd(float out[2], const float a[2],
                            const float b[2], const float scale)
    {
        out[0] = a[0] + scale * b[0];
        out[1] = a[1] + scale * b[1];
    }

    static c_inline void vec3madd(float out[3], const float a[3],
                            const float b[3], const float scale)
    {
        out[0] = a[0] + scale * b[0];
        out[1] = a[1] + scale * b[1];
        out[2] = a[2] + scale * b[2];
    }

    static c_inline void vec4madd(float out[4], const float a[4],
                            const float b[4], const float scale)
    {
        out[0] = a[0] + scale * b[0];
        out[1] = a[1] + scale * b[1];
        out[2] = a[2] + scale * b[2];
        out[3] = a[3] + scale * b[3];
    }
#endif

static c_inline void vec_lerp(float * out, const float * const a,
            const float * const b, const float t, const size_t n)
{
    const register float s = 1.0f - t;

    size_t i = 0;
    for (; i < n; i++) out[i] = s * a[i] + t * b[i];
}

#if 0
    #define vec2lerp(out, a, b, t) vec_lerp((out), (a), (b), (t), 2)
    #define vec3lerp(out, a, b, t) vec_lerp((out), (a), (b), (t), 3)
    #define vec4lerp(out, a, b, t) vec_lerp((out), (a), (b), (t), 4)
#else
    static c_inline void vec2lerp(float out[2], const float a[2],
                                const float b[2], const float t)
    {
        const register float s = 1.0f - t;

        out[0] = s * a[0] + t * b[0];
        out[1] = s * a[1] + t * b[1];
    }

    static c_inline void vec3lerp(float out[3], const float a[3],
                                const float b[3], const float t)
    {
        const register float s = 1.0f - t;

        out[0] = s * a[0] + t * b[0];
        out[1] = s * a[1] + t * b[1];
        out[2] = s * a[2] + t * b[2];
    }

    static c_inline void vec4lerp(float out[4], const float a[4],
                                const float b[4], const float t)
    {
        const register float s = 1.0f - t;

        out[0] = s * a[0] + t * b[0];
        out[1] = s * a[1] + t * b[1];
        out[2] = s * a[2] + t * b[2];
        out[3] = s * a[3] + t * b[3];
    }
#endif

static c_inline void vec_floor(float* out, const float* const v, const size_t n)
{
    size_t i = 0;
    for (; i < n; i++) out[i] = ae_floorf(v[i]);
}

#if 0
    #define vec2floor(out, v) vec_floor((out), (v), 2)
    #define vec3floor(out, v) vec_floor((out), (v), 3)
    #define vec4floor(out, v) vec_floor((out), (v), 4)
#else
    static c_inline void vec2floor(float out[2], const float v[2])
    {
        out[0] = ae_floorf(v[0]);
        out[1] = ae_floorf(v[1]);
    }

    static c_inline void vec3floor(float out[3], const float v[3])
    {
        out[0] = ae_floorf(v[0]);
        out[1] = ae_floorf(v[1]);
        out[2] = ae_floorf(v[2]);
    }

    static c_inline void vec4floor(float out[4], const float v[4])
    {
        out[0] = ae_floorf(v[0]);
        out[1] = ae_floorf(v[1]);
        out[2] = ae_floorf(v[2]);
        out[3] = ae_floorf(v[3]);
    }
#endif

static c_inline void vec_ceil(float* out, const float* const v, const size_t n)
{
    size_t i = 0;
    for (; i < n; i++) out[i] = ae_ceilf(v[i]);
}

#if 0
    #define vec2ceil(out, v) vec_ceil((out), (v), 2)
    #define vec3ceil(out, v) vec_ceil((out), (v), 3)
    #define vec4ceil(out, v) vec_ceil((out), (v), 4)
#else
    static c_inline void vec2ceil(float out[2], const float v[2])
    {
        out[0] = ae_ceilf(v[0]);
        out[1] = ae_ceilf(v[1]);
    }

    static c_inline void vec3ceil(float out[3], const float v[3])
    {
        out[0] = ae_ceilf(v[0]);
        out[1] = ae_ceilf(v[1]);
        out[2] = ae_ceilf(v[2]);
    }

    static c_inline void vec4ceil(float out[4], const float v[4])
    {
        out[0] = ae_ceilf(v[0]);
        out[1] = ae_ceilf(v[1]);
        out[2] = ae_ceilf(v[2]);
        out[3] = ae_ceilf(v[3]);
    }
#endif

static c_inline void vec_round(float* out, const float* const v, const size_t n)
{
    size_t i = 0;
    for (; i < n; i++) out[i] = ae_roundf(v[i]);
}

#if 0
    #define vec2round(out, v) vec_round((out), (v), 2)
    #define vec3round(out, v) vec_round((out), (v), 3)
    #define vec4round(out, v) vec_round((out), (v), 4)
#else
    static c_inline void vec2round(float out[2], const float v[2])
    {
        out[0] = ae_roundf(v[0]);
        out[1] = ae_roundf(v[1]);
    }

    static c_inline void vec3round(float out[3], const float v[3])
    {
        out[0] = ae_roundf(v[0]);
        out[1] = ae_roundf(v[1]);
        out[2] = ae_roundf(v[2]);
    }

    static c_inline void vec4round(float out[4], const float v[4])
    {
        out[0] = ae_roundf(v[0]);
        out[1] = ae_roundf(v[1]);
        out[2] = ae_roundf(v[2]);
        out[3] = ae_roundf(v[3]);
    }
#endif

/* ===== [ single-dimensional kludges ] ===================================== */

/* rotate one point about another (usually the center of an object).
 * this is somewhat of a mathematical hack - use quat/matrix for 3D.
 */
static c_inline void rotate_point2_ex(float out[2], const float point[2],
    const float center[2], const float sin_theta, const float cos_theta)
{
    // copy the point to rotate so that in and out points can alias.
    const float px = point[0];
    const float py = point[1];

    out[0] = center[0] + (px - center[0]) * cos_theta - (py - center[1]) * sin_theta;
    out[1] = center[1] + (px - center[0]) * sin_theta + (py - center[1]) * cos_theta;
}

static c_inline void rotate_point2(float out[2], const float point[2],
                            const float center[2], const float theta)
{
    rotate_point2_ex(out, point, center, ae_sinf(theta), ae_cosf(theta));
}

static c_inline void vec2rotate_ex(float out[2], const float v[2],
                    const float sin_theta, const float cos_theta)
{
    const float origin[2] = { 0.0f, 0.0f };
    rotate_point2_ex(out, v, origin, sin_theta, cos_theta);
}

static c_inline void vec2rotate(float out[2], const float v[2], const float theta)
{
    vec2rotate_ex(out, v, ae_sinf(theta), ae_cosf(theta));
}

static c_inline void vec3cross(float out[3], const float a[3], const float b[3])
{
    ae_assert(out != a, "out and in (a) vecs alias (gives incorrect results!)");
    ae_assert(out != b, "out and in (b) vecs alias (gives incorrect results!)");

    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

// returns 1 if v is a degenerate axial case, after fixing it
AE_DECL int AE_CALL vec3fix_degenerate_normal(float v[3]);

AE_DECL void AE_CALL vec3rotate_ex(float out[3], const float s[3], // rotate about axis
                    const float axis[3], const float sin_theta, const float cos_theta);

AE_DECL void AE_CALL vec3rotate( float out[3], const float s[3],
                        const float axis[3], const float theta);

/* Given an absolute world up vector (like (0, 1, 0) for an OpenGL coordinate
 * system) and an object's direction vector, compute the object's up vector.
 * To compute the right vector, just call vec3cross(out, obj_forward, abs_up).
 */
static c_inline void vec3up_from_forward(float out_vec[3],
        const float abs_up[3], const float obj_forward[3])
{
    float cross_product[3]; // TODO: avoid temp use

    vec3cross(cross_product, obj_forward, abs_up);
    vec3cross(out_vec, cross_product, obj_forward);
}

/* Given 2 of an object's 3 euler angles (yaw and attitude without roll/bank),
 * convert them to a direction vector using the OpenGL world coordinate system
 * (which is a right handed system, so +Z is right, +Y is up, -Z is forwards).
 */
static c_inline void euler_radians_to_gl_normal(float gl_vec[3],
                        const float heading, const float pitch)
{
    const float sh = ae_sinf(heading), sp = ae_sinf(pitch);
    const float ch = ae_cosf(heading), cp = ae_cosf(pitch);

    gl_vec[0] = +(cp * sh);
    gl_vec[1] = +(sp);
    gl_vec[2] = -(cp * ch);
}

static c_inline void euler_degrees_to_gl_normal(float gl_vec[3],
                        const float heading, const float pitch)
{
    euler_radians_to_gl_normal(gl_vec, ae_radiansf(heading), ae_radiansf(pitch));
}

static c_inline float gl_normal_to_heading_radians(const float v[3])
{
    return ae_atan2f(v[0], -v[2]);
}

static c_inline float gl_normal_to_heading_degrees(const float v[3])
{
    return ae_degreesf(gl_normal_to_heading_radians(v));
}

static c_inline float gl_normal_to_pitch_radians(const float v[3])
{
    return ae_atanf(v[1] / ae_sqrtf(v[0] * v[0] + v[2] * v[2]));
}

static c_inline float gl_normal_to_pitch_degrees(const float v[3])
{
    return ae_degreesf(gl_normal_to_pitch_radians(v));
}

/*
================================================================================
 * ~~ [ quaternion math ] ~~ *
--------------------------------------------------------------------------------
*/

#define quat_fix_denormals_ex vec4fix_denormals_ex
#define quat_fix_denormals vec4fix_denormals

#define quat_lerp vec4lerp

#define quat_mag vec4mag
#define quat_mag2 vec4mag2

#define quat_copy vec4copy
#define quat_cmp vec4cmp
#define quat_cmp_e vec4cmp_e

#define quat_zero vec4zero
#define quat_is_zero vec4is_zero
#define quat_is_zero_e vec4is_zero_e

static const float quaternion_identity[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // mag=1

static c_inline void quat_identity(float q[4])
{
    quat_copy(q, quaternion_identity);
}

static c_inline int quat_is_identity(const float q[4])
{
    return quat_cmp(q, quaternion_identity);
}

static c_inline int quat_is_identity_e(const float q[4], const float epsilon)
{
    return quat_cmp_e(q, quaternion_identity, epsilon);
}

static c_inline void quat_conjugated(float out[4], const float q[4])
{
    vec3neg(out, q); out[3] = q[3]; // keep w component
}

static c_inline void quat_conjugate(float q[4])
{
    vec3neg(q, q);
}

static c_inline void quat_normalize(float out[4], const float q[4])
{
    float d = quat_mag(q); // TODO: should we return old quaternion magnitude?
    if (d >= 0.00001f) { vec4div_flt(out, q, d); } else { quat_identity(out); }
}

static c_inline void quat_mul_quat(float out[4], const float a[4], const float b[4])
{
    // XXX: we just use a temp here to avoid issues if a or b = out. if this is used in
    // a performance-critical path, it might be better to inline it and maybe use SIMD.
    // it's also important to remember that quat mul is not commutative (order matters).
    float temp[4];

    temp[0] = a[3] * b[0] + a[0] * b[3] + a[1] * b[2] - a[2] * b[1];
    temp[1] = a[3] * b[1] - a[0] * b[2] + a[1] * b[3] + a[2] * b[0];
    temp[2] = a[3] * b[2] + a[0] * b[1] - a[1] * b[0] + a[2] * b[3];
    temp[3] = a[3] * b[3] - a[0] * b[0] - a[1] * b[1] - a[2] * b[2];

    quat_copy(out, temp);
}

// since quaternions only represent rotation (not scale and translation), the
// distinction between points and vectors is irrelevant. this is to keep the
// naming convention consistent with matrix code, and maybe for perf someday?
#define quat_mul_vector3 quat_mul_point3
#define quat_mul_vector3_safe quat_mul_point3_safe

static void quat_mul_point3(float outp[3], const float q[4], const float p[3])
{
    const float xw2 = q[0] * q[3] * 2.0f;
    const float xy2 = q[0] * q[1] * 2.0f;
    const float xz2 = q[0] * q[2] * 2.0f;
    const float yw2 = q[1] * q[3] * 2.0f;
    const float yz2 = q[1] * q[2] * 2.0f;
    const float zw2 = q[2] * q[3] * 2.0f;

    const float x_2 = q[0] * q[0];
    const float y_2 = q[1] * q[1];
    const float z_2 = q[2] * q[2];
    const float w_2 = q[3] * q[3];

    ae_assert(p != outp, "in and out vectors alias (use safe quat multiply)");

    outp[0] = w_2 * p[0] + yw2 * p[2] - zw2 * p[1] + x_2 * p[0]
            + xy2 * p[1] + xz2 * p[2] - z_2 * p[0] - y_2 * p[0];

    outp[1] = xy2 * p[0] + y_2 * p[1] + yz2 * p[2] + zw2 * p[0]
            - z_2 * p[1] + w_2 * p[1] - xw2 * p[2] - x_2 * p[1];

    outp[2] = xz2 * p[0] + yz2 * p[1] + z_2 * p[2] - yw2 * p[0]
            - y_2 * p[2] + xw2 * p[1] - x_2 * p[2] + w_2 * p[2];
}

static void quat_mul_point3_safe(float out[3], const float q[4], const float p[3])
{
    // "unsafe" quat mul reads out values after writing them
    float v[3]; quat_mul_point3(v, q, p); vec3copy(out, v);
}

AE_DECL void AE_CALL quat_to_row_mat3x3(float m[ 9], const float q[4]);
AE_DECL void AE_CALL quat_to_col_mat3x3(float m[ 9], const float q[4]);

AE_DECL void AE_CALL quat_to_row_mat4x4(float m[16], const float q[4]);
AE_DECL void AE_CALL quat_to_col_mat4x4(float m[16], const float q[4]);

/* ===== [ construction & conversion (angles in radians) ] ================== */

static void
quat_new_rotate_axis(float rq[4], const float angle, const float axis[3])
{
    const float half_rad = angle / 2.0f;

    const float half_cos = ae_cosf(half_rad);
    const float half_sin = ae_sinf(half_rad);

    float normalized_axis[3]; // TODO assert?

    vec3normalize(normalized_axis, axis);
    vec3scale(rq, normalized_axis, half_sin);

    rq[3] = half_cos;
}

static void
quat_new_rotate_euler(float q[4], float heading, float attitude, float bank)
{
    const float half_heading = heading / 2.0f;
    const float half_attitude = attitude / 2.0f;
    const float half_bank = bank / 2.0f;

    const float c1 = ae_cosf(half_heading);
    const float s1 = ae_sinf(half_heading);
    const float c2 = ae_cosf(half_attitude);
    const float s2 = ae_sinf(half_attitude);
    const float c3 = ae_cosf(half_bank);
    const float s3 = ae_sinf(half_bank);

    q[0] = s1 * s2 * c3 + c1 * c2 * s3;
    q[1] = s1 * c2 * c3 + c1 * s2 * s3;
    q[2] = c1 * s2 * c3 - s1 * c2 * s3;
    q[3] = c1 * c2 * c3 - s1 * s2 * s3;
}

static void quat_to_angle_axis(float* angle, float axis[3], const float q[4])
{
    float self[4], s; // TODO: avoid quaternion copy?

    if (q[3] > 1.0f)
    {
        quat_normalize(self, q);
    }
    else
    {
        quat_copy(self, q);
    }

    s = ae_sqrtf(1.0f - self[3] * self[3]);
    *angle = 2.0f * ae_acosf(self[3]);

    if (s < 0.001f)
    {
        axis[0] = 1.0f; axis[1] = 0.0f; axis[2] = 0.0f;
    }
    else
    {
        vec3div_flt(axis, self, s);
    }
}

static void
quat_to_euler(float* heading, float* attitude, float* bank, const float q[4])
{
    const float t = q[0] * q[1] + q[2] * q[3];

    if (t > 0.4999f)
    {
        *heading = 2.0f * ae_atan2f(q[0], q[3]);
        *attitude = 3.14159265358979323846f / 2.0f;
        *bank = 0.0f;
    }
    else if (t < -0.4999f)
    {
        *heading = -2.0f * ae_atan2f(q[0], q[3]);
        *attitude = -3.14159265358979323846f / 2.0f;
        *bank = 0.0f;
    }
    else
    {
        const float x = q[0] * q[0];
        const float y = q[1] * q[1];
        const float z = q[2] * q[2];

        const float hy = 2.0f * q[1] * q[3] - 2.0f * q[0] * q[2];
        const float hx = 1.0f - 2.0f * y - 2.0f * z;

        const float by = 2.0f * q[0] * q[3] - 2.0f * q[1] * q[2];
        const float bx = 1.0f - 2.0f * x - 2.0f * z;

        *heading = ae_atan2f(hy, hx);
        *attitude = ae_asinf(2.0f * t);
        *bank = ae_atan2f(by, bx);
    }
}

/*
================================================================================
 * ~~ [ matrix math ] ~~ *
--------------------------------------------------------------------------------
*/

#define ae_matrix_alloc(r, c) ((float*)ae_calloc((r) * (c), sizeof(float)))
#define ae_matrix_free(m, r, c) ae_free(m) // allocate n-dimensional matrix

#define mat_copy(d, s, r, c) memcpy((d), (s), (r) * (c) * sizeof(float))

#define mat2x2copy(dst, src) mat_copy((dst), (src), 2, 2)
#define mat3x3copy(dst, src) mat_copy((dst), (src), 3, 3)
#define mat4x4copy(dst, src) mat_copy((dst), (src), 4, 4)

#define mat_cmp(a, b, r, c) (memcmp((a), (b), (r) * (c) * sizeof(float)) == 0)

#define mat2x2cmp(a, b) mat_cmp((a), (b), 2, 2)
#define mat3x3cmp(a, b) mat_cmp((a), (b), 3, 3)
#define mat4x4cmp(a, b) mat_cmp((a), (b), 4, 4)

static c_inline int mat_cmp_e(const float * const a, const float * const b,
                const float epsilon, const size_t rows, const size_t cols)
{
    size_t row, col; // mtx compare with epsilon

    for (row = 0; row < rows; row++)
    for (col = 0; col < cols; col++)
    {
        const float A = a[row * cols + col];
        const float B = b[row * cols + col];

        if (ae_fabsf(A - B) > epsilon) return 0;
    }

    return 1;
}

#if 0
    #define mat2x2cmp_e(a, b, e) mat_cmp_e((a), (b), (e), 2, 2)
    #define mat3x3cmp_e(a, b, e) mat_cmp_e((a), (b), (e), 3, 3)
    #define mat4x4cmp_e(a, b, e) mat_cmp_e((a), (b), (e), 4, 4)
#else
    static c_inline int
    mat2x2cmp_e(const float* const a, const float* const b, const float epsilon)
    {
        return( vec2cmp_e(a + 0, b + 0, epsilon) &&
                vec2cmp_e(a + 2, b + 2, epsilon) );
    }

    static c_inline int
    mat3x3cmp_e(const float* const a, const float* const b, const float epsilon)
    {
        return( vec3cmp_e(a + 0, b + 0, epsilon) &&
                vec3cmp_e(a + 3, b + 3, epsilon) &&
                vec3cmp_e(a + 6, b + 6, epsilon) );
    }

    static c_inline int
    mat4x4cmp_e(const float* const a, const float* const b, const float epsilon)
    {
        return( vec4cmp_e(a +  0, b +  0, epsilon) &&
                vec4cmp_e(a +  4, b +  4, epsilon) &&
                vec4cmp_e(a +  8, b +  8, epsilon) &&
                vec4cmp_e(a + 12, b + 12, epsilon) );
    }
#endif

#define mat_zero(m, r, c) memset((m), 0, (r) * (c) * sizeof(float))

#define mat2x2zero(m) mat_zero((m), 2, 2)
#define mat3x3zero(m) mat_zero((m), 3, 3)
#define mat4x4zero(m) mat_zero((m), 4, 4)

static c_inline int mat_is_zero(const float* const m,
                const size_t rows, const size_t cols)
{
    size_t row, col;

    for (row = 0; row < rows; row++)
    for (col = 0; col < cols; col++)
    {
        if (m[row * cols + col]) return 0;
    }

    return 1;
}

#if 1
    #define mat2x2is_zero(m) mat_is_zero((m), 2, 2)
    #define mat3x3is_zero(m) mat_is_zero((m), 3, 3)
    #define mat4x4is_zero(m) mat_is_zero((m), 4, 4)
#else
    #error TODO
#endif

static c_inline int mat_is_zero_e(const float * const m, const float epsilon,
                                        const size_t rows, const size_t cols)
{
    size_t row, col; // fuzzy check if the entire matrix is 0

    for (row = 0; row < rows; row++)
    for (col = 0; col < cols; col++)
    {
        if (ae_fabsf(m[row * cols + col]) > epsilon) return 0;
    }

    return 1;
}

#if 1
    #define mat2x2is_zero_e(m, e) mat_is_zero_e((m), (e), 2, 2)
    #define mat3x3is_zero_e(m, e) mat_is_zero_e((m), (e), 3, 3)
    #define mat4x4is_zero_e(m, e) mat_is_zero_e((m), (e), 4, 4)
#else
    #error TODO
#endif

#define mat_sum(m, r, c) vec_sum((m), (r) * (c)) // add all elements together

#if 1
    #define mat2x2sum(m) mat_sum((m), 2, 2)
    #define mat3x3sum(m) mat_sum((m), 3, 3)
    #define mat4x4sum(m) mat_sum((m), 4, 4)
#else
    #error TODO
#endif

static const float mat2x2_identity[4] =
{
    1.0f, 0.0f,
    0.0f, 1.0f
};

static const float mat3x3_identity[9] =
{
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

static const float mat4x4_identity[16] =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f,
};

static c_inline void mat_identity_ex(float* m, const size_t n)
{
    size_t i = 0;
    for (; i < n; i++) m[i * n + i] = 1.0f;
}

#if 0
    #define mat2x2identity_ex(m) mat_identity_ex((m), 2)
    #define mat3x3identity_ex(m) mat_identity_ex((m), 3)
    #define mat4x4identity_ex(m) mat_identity_ex((m), 4)
#else
    static c_inline void mat2x2identity_ex(float* m)
    {
        m[0] = m[3] = 1.0f;
    }

    static c_inline void mat3x3identity_ex(float* m)
    {
        m[0] = m[4] = m[8] = 1.0f;
    }

    static c_inline void mat4x4identity_ex(float* m)
    {
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }
#endif

static c_inline void mat_identity(float* m, const size_t n)
{
    #if 0
        // NOTE: only square matrices (row count = column count) have an identity
        size_t row, col, i = 0;

        for (row = 0; row < n; row++)
        for (col = 0; col < n; col++)
        {
            if (row == i && col == i)
            {
                m[row * n + col] = 1.0f; i++;
            }
            else
            {
                m[row * n + col] = 0.0f;
            }
        }
    #else
        mat_zero(m, n, n); mat_identity_ex(m, n);
    #endif
}

#if 0
    #define mat2x2identity(m) mat_identity((m), 2)
    #define mat3x3identity(m) mat_identity((m), 3)
    #define mat4x4identity(m) mat_identity((m), 4)
#else
    #define mat2x2identity(m) mat2x2copy((m), mat2x2_identity)
    #define mat3x3identity(m) mat3x3copy((m), mat3x3_identity)
    #define mat4x4identity(m) mat4x4copy((m), mat4x4_identity)
#endif

static c_inline int mat_is_identity(const float* const m, const size_t n)
{
    size_t row, col, i = 0;

    for (row = 0; row < n; row++)
    for (col = 0; col < n; col++)
    {
        if (row == i && col == i)
        {
            if (m[row * n + col] != 1.0f) return 0;
            i++;
        }
        else
        {
            if (m[row * n + col] != 0.0f) return 0;
        }
    }

    return 1;
}

#if 0
    #define mat2x2is_identity(m) mat_is_identity((m), 2)
    #define mat3x3is_identity(m) mat_is_identity((m), 3)
    #define mat4x4is_identity(m) mat_is_identity((m), 4)
#else
    #define mat2x2is_identity(m) mat2x2cmp((m), mat2x2_identity)
    #define mat3x3is_identity(m) mat3x3cmp((m), mat3x3_identity)
    #define mat4x4is_identity(m) mat4x4cmp((m), mat4x4_identity)
#endif

static c_inline int
mat_is_identity_e(const float* const m, const float epsilon, const size_t n)
{
    size_t row, col, i = 0;

    for (row = 0; row < n; row++)
    for (col = 0; col < n; col++)
    {
        if (row == i && col == i)
        {
            if (ae_fabsf(m[row * n + col] - 1.0f) > epsilon) return 0;
            i++;
        }
        else
        {
            if (ae_fabsf(m[row * n + col] - 0.0f) > epsilon) return 0;
        }
    }

    return 1;
}

#if 0
    #define mat2x2is_identity_e(m, e) mat_is_identity_e((m), (e), 2)
    #define mat3x3is_identity_e(m, e) mat_is_identity_e((m), (e), 3)
    #define mat4x4is_identity_e(m, e) mat_is_identity_e((m), (e), 4)
#else
    #define mat2x2is_identity_e(m, e) mat2x2cmp_e((m), mat2x2_identity, (e))
    #define mat3x3is_identity_e(m, e) mat3x3cmp_e((m), mat3x3_identity, (e))
    #define mat4x4is_identity_e(m, e) mat4x4cmp_e((m), mat4x4_identity, (e))
#endif

static c_inline int mat_is_symmetric(const float* const m, const size_t n)
{
    size_t row, col;

    for (row = 0; row < n; row++)
    for (col = 0; col < n; col++)
    {
        if (m[row * n + col] != m[col * n + row]) return 0;
    }

    return 1;
}

#if 0
    #define mat2x2is_symmetric(m) mat_is_symmetric((m), 2)
    #define mat3x3is_symmetric(m) mat_is_symmetric((m), 3)
    #define mat4x4is_symmetric(m) mat_is_symmetric((m), 4)
#else
    static c_inline int mat2x2is_symmetric(const float m[ 4])
    {
        return (m[0 * 2 + 1] == m[1 * 2 + 0]);
    }

    static c_inline int mat3x3is_symmetric(const float m[ 9])
    {
        return (m[0 * 3 + 1] == m[1 * 3 + 0] &&
                m[0 * 3 + 2] == m[2 * 3 + 0] &&
                m[1 * 3 + 2] == m[2 * 3 + 1] );
    }

    static c_inline int mat4x4is_symmetric(const float m[16])
    {
        return (m[0 * 4 + 1] == m[1 * 4 + 0] &&
                m[0 * 4 + 2] == m[2 * 4 + 0] &&
                m[0 * 4 + 3] == m[3 * 4 + 0] &&
                m[1 * 4 + 2] == m[2 * 4 + 1] &&
                m[1 * 4 + 3] == m[3 * 4 + 1] &&
                m[2 * 4 + 3] == m[3 * 4 + 2] );
    }
#endif

static c_inline int // if matrix M is symmetric, then the transpose of M = M
mat_is_symmetric_e(const float* const m, const float epsilon, const size_t n)
{
    size_t row, col;

    for (row = 0; row < n; row++)
    for (col = 0; col < n; col++)
    {
        if (ae_fabsf(m[row * n + col] - m[col * n + row]) > epsilon) return 0;
    }

    return 1;
}

#if 0
    #define mat2x2is_symmetric_e(m, e) mat_is_symmetric_e((m), (e), 2)
    #define mat3x3is_symmetric_e(m, e) mat_is_symmetric_e((m), (e), 3)
    #define mat4x4is_symmetric_e(m, e) mat_is_symmetric_e((m), (e), 4)
#else
    static c_inline int mat2x2is_symmetric_e(const float m[ 4], const float epsilon)
    {
        return (ae_fabsf(m[0 * 2 + 1] - m[1 * 2 + 0]) > epsilon);
    }

    static c_inline int mat3x3is_symmetric_e(const float m[ 9], const float epsilon)
    {
        return (ae_fabsf(m[0 * 3 + 1] - m[1 * 3 + 0]) > epsilon &&
                ae_fabsf(m[0 * 3 + 2] - m[2 * 3 + 0]) > epsilon &&
                ae_fabsf(m[1 * 3 + 2] - m[2 * 3 + 1]) > epsilon );
    }

    static c_inline int mat4x4is_symmetric_e(const float m[16], const float epsilon)
    {
        return (ae_fabsf(m[0 * 4 + 1] - m[1 * 4 + 0]) > epsilon &&
                ae_fabsf(m[0 * 4 + 2] - m[2 * 4 + 0]) > epsilon &&
                ae_fabsf(m[0 * 4 + 3] - m[3 * 4 + 0]) > epsilon &&
                ae_fabsf(m[1 * 4 + 2] - m[2 * 4 + 1]) > epsilon &&
                ae_fabsf(m[1 * 4 + 3] - m[3 * 4 + 1]) > epsilon &&
                ae_fabsf(m[2 * 4 + 3] - m[3 * 4 + 2]) > epsilon );
    }
#endif

static c_inline void mat_transposed(float* out, const float* const m,
                                const size_t rows, const size_t cols)
{
    size_t row, col; // switch between row/col major

    ae_assert(out != m, "%ux%u aliased",
        (unsigned)rows, (unsigned)cols);

    for (row = 0; row < rows; row++)
    for (col = 0; col < cols; col++)
    {
        out[col * cols + row] = m[row * cols + col];
    }
}

#if 1
    #define mat2x2transposed(out, m) mat_transposed((out), (m), 2, 2)
    #define mat3x3transposed(out, m) mat_transposed((out), (m), 3, 3)
    #define mat4x4transposed(out, m) mat_transposed((out), (m), 4, 4)
#else
    #error TODO
#endif

static c_inline void mat_transpose(float* m, const size_t n)
{
    size_t size = n * n * sizeof(float); // don't write over values
    float* temp = (float*)ae_stack_malloc(ae_global_stack(), size);

    mat_transposed(temp, m, n, n);
    mat_copy(m, temp, n, n);

    ae_stack_free(ae_global_stack(), temp, size);
}

#if 0
    #define mat2x2transpose(m) mat_transpose((m), 2)
    #define mat3x3transpose(m) mat_transpose((m), 3)
    #define mat4x4transpose(m) mat_transpose((m), 4)
#else
    static c_inline void mat2x2transpose(float m[ 4])
    {
        float temp; // 1 swap
        temp = m[ 1]; m[ 1] = m[ 2]; m[ 2] = temp;
    }

    static c_inline void mat3x3transpose(float m[ 9])
    {
        float temp; // 3 swaps
        temp = m[ 1]; m[ 1] = m[ 3]; m[ 3] = temp;
        temp = m[ 2]; m[ 2] = m[ 6]; m[ 6] = temp;
        temp = m[ 5]; m[ 5] = m[ 7]; m[ 7] = temp;
    }

    static c_inline void mat4x4transpose(float m[16])
    {
        float temp; // 6 swaps
        temp = m[ 1]; m[ 1] = m[ 4]; m[ 4] = temp;
        temp = m[ 2]; m[ 2] = m[ 8]; m[ 8] = temp;
        temp = m[ 3]; m[ 3] = m[12]; m[12] = temp;
        temp = m[ 6]; m[ 6] = m[ 9]; m[ 9] = temp;
        temp = m[ 7]; m[ 7] = m[13]; m[13] = temp;
        temp = m[11]; m[11] = m[14]; m[14] = temp;
    }
#endif

static c_inline float mat_trace(const float* const m, const size_t n)
{
    float trace = 0.0f;
    size_t i = 0;

    for (; i < n; i++)
    {
        trace += m[i * n + i];
    }

    return trace;
}

#if 0
    #define mat2x2trace(m) mat_trace((m), 2)
    #define mat3x3trace(m) mat_trace((m), 3)
    #define mat4x4trace(m) mat_trace((m), 4)
#else
    static c_inline float mat2x2trace(const float m[ 4])
    {
        return m[0 * 2 + 0] + m[1 * 2 + 1];
    }

    static c_inline float mat3x3trace(const float m[ 9])
    {
        return m[0 * 3 + 0] + m[1 * 3 + 1] + m[2 * 3 + 2];
    }

    static c_inline float mat4x4trace(const float m[16])
    {
        return m[0 * 4 + 0] + m[1 * 4 + 1] + m[2 * 4 + 2] + m[3 * 4 + 3];
    }
#endif

static c_inline void mat_get_diag(float* out, const float* const m, const size_t n)
{
    size_t i = 0; // get elements on diagonal
    for (; i < n; i++) out[i] = m[i * n + i];
}

#if 1
    #define mat2x2get_diag(v, m) mat_get_diag((v), (m), 2)
    #define mat3x3get_diag(v, m) mat_get_diag((v), (m), 3)
    #define mat4x4get_diag(v, m) mat_get_diag((v), (m), 4)
#else
    #error TODO
#endif

static c_inline void mat_set_diag(float* mat, const float* const v, const size_t n)
{
    size_t i = 0; // set elements on diagonal
    for (; i < n; i++) mat[i * n + i] = v[i];
}

#if 1
    #define mat2x2set_diag(m, v) mat_set_diag((m), (v), 2)
    #define mat3x3set_diag(m, v) mat_set_diag((m), (v), 3)
    #define mat4x4set_diag(m, v) mat_set_diag((m), (v), 4)
#else
    #error TODO
#endif

static c_inline void mat_get_row(float * out, const float * const m,
        const size_t row, const size_t n_rows, const size_t n_cols)
{
    ae_assert(row < n_rows, "bad row index %u", (u32)row);
    memcpy(out, &m[row * n_cols], n_cols * sizeof(float));
}

#if 1
    #define mat2x2get_row(out, m, row) mat_get_row((out), (m), (row), 2, 2)
    #define mat3x3get_row(out, m, row) mat_get_row((out), (m), (row), 3, 3)
    #define mat4x4get_row(out, m, row) mat_get_row((out), (m), (row), 4, 4)
#else
    #error TODO
#endif

static c_inline void mat_get_col(float * out, const float * const m,
        const size_t col, const size_t n_rows, const size_t n_cols)
{
    size_t i = 0;
    ae_assert(col < n_cols, "bad col index %u", (u32)col);

    for (; i < n_rows; i++)
    {
        out[i] = m[i * n_cols + col];
    }
}

#if 1
    #define mat2x2get_col(out, m, col) mat_get_col((out), (m), (col), 2, 2)
    #define mat3x3get_col(out, m, col) mat_get_col((out), (m), (col), 3, 3)
    #define mat4x4get_col(out, m, col) mat_get_col((out), (m), (col), 4, 4)
#else
    #error TODO
#endif

static c_inline void mat_set_row(float * m, const float * const vec,
        const size_t row, const size_t n_rows, const size_t n_cols)
{
    ae_assert(row < n_rows, "bad row index %u", (u32)row);
    memcpy(&m[row * n_cols], vec, n_cols * sizeof(float));
}

#if 1
    #define mat2x2set_row(mat, v, row) mat_set_row((mat), (v), (row), 2, 2)
    #define mat3x3set_row(mat, v, row) mat_set_row((mat), (v), (row), 3, 3)
    #define mat4x4set_row(mat, v, row) mat_set_row((mat), (v), (row), 4, 4)
#else
    #error TODO
#endif

static c_inline void mat_set_col(float * m, const float * const vec,
        const size_t col, const size_t n_rows, const size_t n_cols)
{
    size_t i = 0;
    ae_assert(col < n_cols, "bad col index %u", (u32)col);

    for (; i < n_rows; i++)
    {
        m[i * n_cols + col] = vec[i];
    }
}

#if 1
    #define mat2x2set_col(mat, v, col) mat_set_col((mat), (v), (col), 2, 2)
    #define mat3x3set_col(mat, v, col) mat_set_col((mat), (v), (col), 3, 3)
    #define mat4x4set_col(mat, v, col) mat_set_col((mat), (v), (col), 4, 4)
#else
    #error TODO
#endif

static c_inline void mat_swap_rows(float* mat, const size_t r1,
    const size_t r2, const size_t n_rows, const size_t n_cols)
{
    ae_assert(r1 < n_rows, "bad row index %u", (u32)r1);
    ae_assert(r2 < n_rows, "bad row index %u", (u32)r2);

    if (r1 != r2)
    {
        size_t col = 0; // pivot values for inverse

        for (; col < n_cols; col++)
        {
            const float a = mat[r1 * n_cols + col];
            const float b = mat[r2 * n_cols + col];

            mat[r1 * n_cols + col] = b;
            mat[r2 * n_cols + col] = a;
        }
    }
}

#if 1
    #define mat2x2swap_rows(m, r1, r2) mat_swap_rows((m), (r1), (r2), 2, 2)
    #define mat3x3swap_rows(m, r1, r2) mat_swap_rows((m), (r1), (r2), 3, 3)
    #define mat4x4swap_rows(m, r1, r2) mat_swap_rows((m), (r1), (r2), 4, 4)
#else
    #error TODO
#endif

static c_inline void mat_swap_cols(float* mat, const size_t c1,
    const size_t c2, const size_t n_rows, const size_t n_cols)
{
    ae_assert(c1 < n_cols, "bad col index %u", (u32)c1);
    ae_assert(c2 < n_cols, "bad col index %u", (u32)c2);

    if (c1 != c2)
    {
        size_t row = 0; // iterate through mat cols

        for (; row < n_rows; row++)
        {
            const float a = mat[row * n_cols + c1];
            const float b = mat[row * n_cols + c2];

            mat[row * n_cols + c1] = b;
            mat[row * n_cols + c2] = a;
        }
    }
}

#if 1
    #define mat2x2swap_cols(m, c1, c2) mat_swap_cols((m), (c1), (c2), 2, 2)
    #define mat3x3swap_cols(m, c1, c2) mat_swap_cols((m), (c1), (c2), 3, 3)
    #define mat4x4swap_cols(m, c1, c2) mat_swap_cols((m), (c1), (c2), 4, 4)
#else
    #error TODO
#endif

static void mat_get_mat(float* dst, const size_t dst_rows, const size_t dst_cols,
            const float* const src, const size_t src_rows, const size_t src_cols,
            const size_t src_row_offset, const size_t src_col_offset)
{
    size_t d_row, d_col, s_row, s_col;

    ae_assert(src_row_offset + dst_rows <= src_rows, "off %u dst %u src %u",
                        (u32)src_row_offset, (u32)dst_rows, (u32)src_rows);

    ae_assert(src_col_offset + dst_cols <= src_cols, "off %u dst %u src %u",
                        (u32)src_col_offset, (u32)dst_cols, (u32)src_cols);

    for (d_row = 0, s_row = src_row_offset; d_row < dst_rows; d_row++, s_row++)
    for (d_col = 0, s_col = src_col_offset; d_col < dst_cols; d_col++, s_col++)
    {
        dst[d_row * dst_cols + d_col] = src[s_row * src_cols + s_col];
    }
}

static void mat_set_mat(float* dst, const size_t dst_rows, const size_t dst_cols,
            const float* const src, const size_t src_rows, const size_t src_cols,
            const size_t dst_row_offset, const size_t dst_col_offset)
{
    size_t d_row, d_col, s_row, s_col;

    ae_assert(dst_row_offset + src_rows <= dst_rows, "off %u src %u dst %u",
                        (u32)dst_row_offset, (u32)src_rows, (u32)dst_rows);

    ae_assert(dst_col_offset + src_cols <= dst_cols, "off %u src %u dst %u",
                        (u32)dst_col_offset, (u32)src_cols, (u32)dst_cols);

    for (d_row = dst_row_offset, s_row = 0; s_row < src_rows; d_row++, s_row++)
    for (d_col = dst_col_offset, s_col = 0; s_col < src_cols; d_col++, s_col++)
    {
        dst[d_row * dst_cols + d_col] = src[s_row * src_cols + s_col];
    }
}

AE_DECL int AE_CALL mat_inverse(float* out, const float* const m, const size_t n);

AE_DECL int AE_CALL mat2x2inverse(float out[ 4], const float m[ 4]);
AE_DECL int AE_CALL mat3x3inverse(float out[ 9], const float m[ 9]);
AE_DECL int AE_CALL mat4x4inverse(float out[16], const float m[16]);

AE_DECL int AE_CALL mat_invert(float* m, const size_t n);

AE_DECL int AE_CALL mat2x2invert(float m[ 4]);
AE_DECL int AE_CALL mat3x3invert(float m[ 9]);
AE_DECL int AE_CALL mat4x4invert(float m[16]);

AE_DECL float AE_CALL mat_determinant(const float* const m, const size_t n);

AE_DECL float AE_CALL mat2x2determinant(const float m[ 4]);
AE_DECL float AE_CALL mat3x3determinant(const float m[ 9]);
AE_DECL float AE_CALL mat4x4determinant(const float m[16]);

#define AE_MATRIX_MUL_CHECK() /* don't romp over values we need! */ \
    \
    ae_assert(out != a, "matrix multiplication out and lhs alias"); \
    ae_assert(out != b, "matrix multiplication out and rhs alias");

static c_inline void mat_mul_mat(float * out, const float * const a,
        const float * const b, const size_t rows, const size_t cols)
{
    ae_assert(0, "TODO");
}

#if 0
    #define mat2x2mul_mat(out, a, b) mat_mul_mat((out), (a), (b), 2, 2)
    #define mat3x3mul_mat(out, a, b) mat_mul_mat((out), (a), (b), 3, 3)
    #define mat4x4mul_mat(out, a, b) mat_mul_mat((out), (a), (b), 4, 4)
#else
    static c_inline void
    mat2x2mul_mat(float out[ 4], const float a[ 4], const float b[ 4])
    {
        AE_MATRIX_MUL_CHECK();

        out[0] = a[0] * b[0] + a[1] * b[2];
        out[1] = a[0] * b[1] + a[1] * b[3];
        out[2] = a[2] * b[0] + a[3] * b[2];
        out[3] = a[2] * b[1] + a[3] * b[3];
    }

    static c_inline void
    mat3x3mul_mat(float out[ 9], const float a[ 9], const float b[ 9])
    {
        size_t i, j, op = 0, d = 0;
        AE_MATRIX_MUL_CHECK();

        for (i = 0; i < 3; ++i, d += 3)
        for (j = 0; j < 3; ++j, ++op)
        {
            out[op] = a[d + 0] * b[0 * 3 + j]
                    + a[d + 1] * b[1 * 3 + j]
                    + a[d + 2] * b[2 * 3 + j];
        }
    }

    static c_inline void
    mat4x4mul_mat(float out[16], const float a[16], const float b[16])
    {
        size_t i, j, op = 0, d = 0;
        AE_MATRIX_MUL_CHECK();

        for (i = 0; i < 4; ++i, d += 4)
        for (j = 0; j < 4; ++j, ++op)
        {
            out[op] = a[d + 0] * b[0 * 4 + j]
                    + a[d + 1] * b[1 * 4 + j]
                    + a[d + 2] * b[2 * 4 + j]
                    + a[d + 3] * b[3 * 4 + j];
        }
    }
#endif

static c_inline void mat_mul_mat_safe(float* out, const float* const a,
            const float* const b, const size_t rows, const size_t cols)
{
    ae_assert(0, "TODO");
}

#if 0
    #define mat2x2mul_mat_safe(out, a, b) mat_mul_mat((out), (a), (b), 2, 2)
    #define mat3x3mul_mat_safe(out, a, b) mat_mul_mat((out), (a), (b), 3, 3)
    #define mat4x4mul_mat_safe(out, a, b) mat_mul_mat((out), (a), (b), 4, 4)
#else
    static c_inline void
    mat2x2mul_mat_safe(float out[ 4], const float a[ 4], const float b[ 4])
    {
        float temp[ 4]; mat2x2mul_mat(temp, a, b); mat2x2copy(out, temp);
    }

    static c_inline void
    mat3x3mul_mat_safe(float out[ 9], const float a[ 9], const float b[ 9])
    {
        float temp[ 9]; mat3x3mul_mat(temp, a, b); mat3x3copy(out, temp);
    }

    static c_inline void
    mat4x4mul_mat_safe(float out[16], const float a[16], const float b[16])
    {
        float temp[16]; mat4x4mul_mat(temp, a, b); mat4x4copy(out, temp);
    }
#endif

#define mat_mul_flt(out, m, f, r, c) vec_mul_flt((out), (m), (f), (r) * (c))

#define mat2x2mul_flt(out, m, f) mat_mul_flt((out), (m), (f), 2, 2)
#define mat3x3mul_flt(out, m, f) mat_mul_flt((out), (m), (f), 3, 3)
#define mat4x4mul_flt(out, m, f) mat_mul_flt((out), (m), (f), 4, 4)

static c_inline void
mat4x4row_mul_point3(float outp[3], const float mat[16], const float p[3])
{
    float s = (mat[4 * 3 + 0] * p[0] + mat[4 * 3 + 1] *
        p[1] + mat[4 * 3 + 2] * p[2] + mat[4 * 3 + 3]);

    ae_assert(outp != p, "in and out vectors alias (values overwritten)");

    if (s == 0.0f)
    {
        vec3zero(outp); return;
    }

    outp[0] = (mat[4 * 0 + 0] * p[0] + mat[4 * 0 + 1] *
        p[1] + mat[4 * 0 + 2] * p[2] + mat[4 * 0 + 3]);

    outp[1] = (mat[4 * 1 + 0] * p[0] + mat[4 * 1 + 1] *
        p[1] + mat[4 * 1 + 2] * p[2] + mat[4 * 1 + 3]);

    outp[2] = (mat[4 * 2 + 0] * p[0] + mat[4 * 2 + 1] *
        p[1] + mat[4 * 2 + 2] * p[2] + mat[4 * 2 + 3]);

    // NOTE: this is really mat * vec4, so normalize to keep point w = 1.0
    vec3div_flt(outp, outp, s);
}

static c_inline void
mat4x4col_mul_point3(float out[3], const float mat[16], const float p[3])
{
    float m[16]; mat4x4transposed(m, mat); mat4x4row_mul_point3(out, m, p);
}

static c_inline void
mat4x4row_mul_point3_safe(float out[3], const float mat[16], const float p[3])
{
    float t[3]; mat4x4row_mul_point3(t, mat, p); vec3copy(out, t);
}

static c_inline void
mat4x4col_mul_point3_safe(float out[3], const float mat[16], const float p[3])
{
    float m[16]; mat4x4transposed(m, mat); mat4x4row_mul_point3_safe(out, m, p);
}

static c_inline void // NOTE: doesn't automatically re-normalize the output vector
mat4x4row_mul_vector3(float out[3], const float mat[16], const float v[3])
{
    ae_assert(out != v, "in and out vectors alias (values overwritten)");

    out[0] = mat[4 * 0 + 0] * v[0] + mat[4 * 0 + 1] * v[1] + mat[4 * 0 + 2] * v[2];
    out[1] = mat[4 * 1 + 0] * v[0] + mat[4 * 1 + 1] * v[1] + mat[4 * 1 + 2] * v[2];
    out[2] = mat[4 * 2 + 0] * v[0] + mat[4 * 2 + 1] * v[1] + mat[4 * 2 + 2] * v[2];
}

static c_inline void
mat4x4col_mul_vector3(float out[3], const float mat[16], const float v[3])
{
    float m[16]; mat4x4transposed(m, mat); mat4x4row_mul_vector3(out, m, v);
}

static c_inline void
mat4x4row_mul_vector3_safe(float out[3], const float mat[16], const float v[3])
{
    float t[3]; mat4x4row_mul_vector3(t, mat, v); vec3copy(out, t);
}

static c_inline void
mat4x4col_mul_vector3_safe(float out[3], const float mat[16], const float v[3])
{
    float m[16]; mat4x4transposed(m, mat); mat4x4row_mul_vector3_safe(out, m, v);
}

AE_DECL void AE_CALL mat4x4row_to_quat(float q[4], const float m[16]);
AE_DECL void AE_CALL mat4x4col_to_quat(float q[4], const float m[16]);

/* ===== [ construction (angles in radians) ] =============================== */

static void mat4x4row_translate(float out[16], const float xyz[3])
{
    ae_assert(mat4x4is_identity(out), "non-identity!");
    out[3] = xyz[0]; out[7] = xyz[1]; out[11] = xyz[2];
}

static void mat4x4col_translate(float out[16], const float xyz[3])
{
    mat4x4row_translate(out, xyz); mat4x4transpose(out);
}

static void mat4x4row_scale(float out[16], const float xyz[3])
{
    ae_assert(mat4x4is_identity(out), "non-identity!");
    out[0] = xyz[0]; out[5] = xyz[1]; out[10] = xyz[2];
}

static void mat4x4col_scale(float out[16], const float xyz[3])
{
    mat4x4row_scale(out, xyz); mat4x4transpose(out);
}

static void mat4x4row_rotate_x(float out[16], const float angle)
{
    float s = ae_sinf(angle);
    float c = ae_cosf(angle);

    ae_assert(mat4x4is_identity(out), "non-identity!");
    out[5] = out[10] = c; out[6] = -s; out[9] = s;
}

static void mat4x4col_rotate_x(float out[16], const float angle)
{
    mat4x4row_rotate_x(out, angle); mat4x4transpose(out);
}

static void mat4x4row_rotate_y(float out[16], const float angle)
{
    float s = ae_sinf(angle);
    float c = ae_cosf(angle);

    ae_assert(mat4x4is_identity(out), "non-identity!");
    out[0] = out[10] = c; out[2] = s; out[8] = -s;
}

static void mat4x4col_rotate_y(float out[16], const float angle)
{
    mat4x4row_rotate_y(out, angle); mat4x4transpose(out);
}

static void mat4x4row_rotate_z(float out[16], const float angle)
{
    float s = ae_sinf(angle);
    float c = ae_cosf(angle);

    ae_assert(mat4x4is_identity(out), "non-identity!");
    out[0] = out[5] = c; out[1] = -s; out[4] = s;
}

static void mat4x4col_rotate_z(float out[16], const float angle)
{
    mat4x4row_rotate_z(out, angle); mat4x4transpose(out);
}

static void
mat4x4row_rotate_axis(float out[16], const float angle, const float axis[3])
{
    float s = ae_sinf(angle);
    float c = ae_cosf(angle);

    float n[3];
    float i = 1.0f - c;

    ae_assert(mat4x4is_identity(out), "non-identity!");
    vec3normalize(n, axis);

    out[ 0] = n[0] * n[0] * i + c;
    out[ 1] = n[0] * n[1] * i - n[2] * s;
    out[ 2] = n[0] * n[2] * i + n[1] * s;

    out[ 4] = n[1] * n[0] * i + n[2] * s;
    out[ 5] = n[1] * n[1] * i + c;
    out[ 6] = n[1] * n[2] * i - n[0] * s;

    out[ 8] = n[0] * n[2] * i - n[1] * s;
    out[ 9] = n[1] * n[2] * i + n[0] * s;
    out[10] = n[2] * n[2] * i + c;
}

static void
mat4x4col_rotate_axis(float out[16], const float angle, const float axis[3])
{
    mat4x4row_rotate_axis(out, angle, axis); mat4x4transpose(out);
}

static void
mat4x4row_rotate_euler(float out[16], float heading, float attitude, float bank)
{
    float ch = ae_cosf(heading);
    float sh = ae_sinf(heading);

    float ca = ae_cosf(attitude);
    float sa = ae_sinf(attitude);

    float cb = ae_cosf(bank);
    float sb = ae_sinf(bank);

    ae_assert(mat4x4is_identity(out), "non-identity!");

    out[ 0] = +ch * ca;
    out[ 1] = +sh * sb - ch * sa * cb;
    out[ 2] = +ch * sa * sb + sh * cb;

    out[ 4] = +sa;
    out[ 5] = +ca * cb;
    out[ 6] = -ca * sb;

    out[ 8] = -sh * ca;
    out[ 9] = +sh * sa * cb + ch * sb;
    out[10] = -sh * sa * sb + ch * cb;
}

static void
mat4x4col_rotate_euler(float out[16], float heading, float attitude, float bank)
{
    mat4x4row_rotate_euler(out, heading, attitude, bank); mat4x4transpose(out);
}

static void
mat4x4row_rotate_triple_axis(float out[16], float x[3], float y[3], float z[3])
{
    ae_assert(mat4x4is_identity(out), "non-identity!");

    out[ 0] = x[0]; out[ 1] = y[0]; out[ 2] = z[0];
    out[ 4] = x[1]; out[ 5] = y[1]; out[ 6] = z[1];
    out[ 8] = x[2]; out[ 9] = y[2]; out[10] = z[2];
}

static void
mat4x4col_rotate_triple_axis(float out[16], float x[3], float y[3], float z[3])
{
    mat4x4row_rotate_triple_axis(out, x, y, z); mat4x4transpose(out);
}

static void mat4x4col_perspective(float out_m[16], const float fovy,
            const float aspect, const float znear, const float zfar)
{
    float radians = fovy / 2.0f * M_PI / 180.0f;
    float delta_z = zfar - znear;

    // get y cartesian coord of the y field of view
    float sin_v = ae_sinf(radians);

    ae_assert(mat4x4is_identity(out_m), "non-identity!");

    if (delta_z && sin_v && aspect)
    {
        float cotangent = ae_cosf(radians) / sin_v;

        out_m[ 0] = cotangent / aspect;
        out_m[ 5] = cotangent;
        out_m[10] = -(zfar + znear) / delta_z;

        out_m[11] = -1.0f;
        out_m[14] = -2.0f * znear * zfar / delta_z;
        out_m[15] = +0.0f;
    }
    else
    {
        AE_WARN("fovy %f aspect %f near %f far %f",
                        fovy, aspect, znear, zfar);
    }
}

static void mat4x4row_perspective(float out_m[16], const float fovy,
            const float aspect, const float znear, const float zfar)
{
    mat4x4col_perspective(out_m, fovy, aspect, znear, zfar); mat4x4transpose(out_m);
}

/*
================================================================================
 * ~~ [ quadric shapes ] ~~ *
--------------------------------------------------------------------------------
TODO compute the shortest possible line segment (& distance) between two shapes
--------------------------------------------------------------------------------
*/

static c_inline int ellipse_vs_ellipse( const float a_c[2], const float a_v[2],
                                        const float b_c[2], const float b_v[2])
{
    // TODO: ellipse_vs_ellipse_ex that stores the contact points on collisions
    float v[2], n[2];
    float a[2], b[2];

    vec2sub_vec(v, a_c, b_c);
    vec2normalize(n, v);

    vec2mul_vec(a, a_v, n);
    vec2mul_vec(b, b_v, n);
    vec2neg(a, a);

    return vec2mag2(v) < (vec2mag2(a) + vec2mag2(b));
}

static c_inline int ellipsoid_vs_ellipsoid( const float a_c[3],
    const float a_v[3], const float b_c[3], const float b_v[3])
{
    float v[3], n[3];
    float a[3], b[3];

    vec3sub_vec(v, a_c, b_c);
    vec3normalize(n, v);

    vec3mul_vec(a, a_v, n);
    vec3mul_vec(b, b_v, n);
    vec3neg(a, a);

    return vec3mag2(v) < (vec3mag2(a) + vec3mag2(b));
}

AE_DECL int AE_CALL // if ray hit, distance to point is stored in out parameter
ellipsoid_vs_ray(float* h_distance, const float e_pos[3], const float e_ext[3],
                                const float ray_pos[3], const float ray_dir[3]);

/*
================================================================================
 * ~~ [ line math ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline int // u lies along [ -inf, +inf ]
u_in_line(const float u) { return 1; }

static c_inline int // u lies along [ -0.0, +1.0 ]
u_in_line_segment(const float u) { return u >= 0.0f && u <= 1.0f; }

static c_inline int // u lies along [ -0.0, +inf ]
u_in_ray(const float u) { return u >= 0.0f; }

#define AE_LINE_TYPE_N \
    N(AE_LINE_TYPE_LINE), \
    N(AE_LINE_TYPE_LINE_SEGMENT), \
    N(AE_LINE_TYPE_RAY), \
    N(AE_LINE_TYPE_COUNT)

typedef enum ae_line_type_t
{
    #define N(x) x
    AE_LINE_TYPE_N
    #undef N
} \
    ae_line_type_t;

static const char* ae_line_type_name[] =
{
    #define N(x) #x
    AE_LINE_TYPE_N
    #undef N
};

static c_inline int u_in_line_type(const float u, const ae_line_type_t type)
{
    switch (type)
    {
        case AE_LINE_TYPE_LINE:         return u_in_line(u);
        case AE_LINE_TYPE_LINE_SEGMENT: return u_in_line_segment(u);
        case AE_LINE_TYPE_RAY:          return u_in_ray(u);

        default: ae_assert(0, "%u", (unsigned)type); return 0;
    }
}

static c_inline int plane_vs_line_type(float hit[3], const float line_p[3],
    const float line_v[3], const ae_line_type_t type, const float plane[4])
{
    float u, d = vec3dot(plane, line_v);

    // line is parallel to plane (TODO: epsilon?)
    if (d == 0.0f) return 0;

    u = (plane[3] - vec3dot(plane, line_p)) / d;
    if (!u_in_line_type(u, type)) return 0;

    hit[0] = line_p[0] + u * line_v[0];
    hit[1] = line_p[1] + u * line_v[1];
    hit[2] = line_p[2] + u * line_v[2];

    return 1;
}

#define plane_vs_line(hp, lp, lv, p) \
        plane_vs_line_type(hp, lp, lv, AE_LINE_TYPE_LINE, p)

// #define plane_vs_line_segment(hp, lp, lv, p) \
//         plane_vs_line_type(hp, lp, lv, AE_LINE_TYPE_LINE_SEGMENT, p)

#define plane_vs_ray(hp, lp, lv, p) \
        plane_vs_line_type(hp, lp, lv, AE_LINE_TYPE_RAY, p)

static c_inline int plane_vs_line_segment(float hit[3], const float line_p[3],
                                const float line_vec[3], const float plane[4])
{
    float u, d = vec3dot(plane, line_vec);

    // line is parallel to plane (TODO: epsilon?)
    if (d == 0.0f) return 0;

    u = (plane[3] - vec3dot(plane, line_p)) / d;
    if (!u_in_line_segment(u)) return 0;

    hit[0] = line_p[0] + u * line_vec[0];
    hit[1] = line_p[1] + u * line_vec[1];
    hit[2] = line_p[2] + u * line_vec[2];

    return 1;
}

/*
================================================================================
 * ~~ [ planar math ] ~~ *
--------------------------------------------------------------------------------
*/

#define plane_copy vec4copy
#define plane_cmp vec4cmp
#define plane_cmp_e vec4cmp_e

#define plane_zero vec4zero
#define plane_is_zero vec4is_zero
#define plane_is_zero_e vec4is_zero_e

#define plane_neg vec4neg

static c_inline void plane_translate(float p[4], const float vec[3])
{
    p[3] += vec3dot(vec, p); // just set a new plane distance scalar value
}

static c_inline float plane_distance(const float p[4], const float vec[3])
{
    return vec3dot(p, vec) - p[3]; // get the distance from plane to point
}

static c_inline void point_on_plane(float out[3], const float p[4])
{
    ae_assert(vec3is_unit_e(p, 0.0001f), "plane { %f, %f, %f }.p = %f has" \
        " non-unit normal (length %f)", p[0], p[1], p[2], p[3], vec3mag(p));

    vec3scale(out, p, p[3]);
}

/* ===== [ classification ] ================================================= */

#define AE_PLANE_SIDE_N \
    N(AE_PLANE_SIDE_FRONT), \
    N(AE_PLANE_SIDE_BACK), \
    N(AE_PLANE_SIDE_ON), \
    N(AE_PLANE_SIDE_COUNT)

typedef enum ae_plane_side_t
{
    #define N(x) x
    AE_PLANE_SIDE_N
    #undef N
} \
    ae_plane_side_t;

static const char* ae_plane_side_name[] =
{
    #define N(x) #x
    AE_PLANE_SIDE_N
    #undef N
};

static ae_plane_side_t plane_side( const float p[4],
            const float vec[3], const float epsilon)
{
    const float dist = plane_distance(p, vec);

    if (dist > epsilon) return AE_PLANE_SIDE_FRONT;
    if (dist < -epsilon) return AE_PLANE_SIDE_BACK;

    return AE_PLANE_SIDE_ON;
}

#define AE_PLANE_TYPE_TRUE_AXIAL 6 // all plane types < 6 are true axial cases

#define AE_PLANE_TYPE_N \
    N(AE_PLANE_TYPE_X), \
    N(AE_PLANE_TYPE_Y), \
    N(AE_PLANE_TYPE_Z), \
    N(AE_PLANE_TYPE_NEG_X), \
    N(AE_PLANE_TYPE_NEG_Y), \
    N(AE_PLANE_TYPE_NEG_Z), \
    N(AE_PLANE_TYPE_ZERO_X), \
    N(AE_PLANE_TYPE_ZERO_Y), \
    N(AE_PLANE_TYPE_ZERO_Z), \
    N(AE_PLANE_TYPE_NON_AXIAL), \
    N(AE_PLANE_TYPE_COUNT)

typedef enum ae_plane_type_t
{
    #define N(x) x
    AE_PLANE_TYPE_N
    #undef N
} \
    ae_plane_type_t;

static const char* ae_plane_type_name[] =
{
    #define N(x) #x
    AE_PLANE_TYPE_N
    #undef N
};

static ae_plane_type_t plane_type(const float plane[4])
{
    if (plane[0] == 0.0f)
    {
        if (plane[1] == 0.0f)
        {
            return plane[2] > 0.0f ? AE_PLANE_TYPE_Z : AE_PLANE_TYPE_NEG_Z;
        }

        if (plane[2] == 0.0f)
        {
            return plane[1] > 0.0f ? AE_PLANE_TYPE_Y : AE_PLANE_TYPE_NEG_Y;
        }

        return AE_PLANE_TYPE_ZERO_X;
    }

    if (plane[1] == 0.0f)
    {
        if (plane[2] == 0.0f)
        {
            return plane[0] > 0.0f ? AE_PLANE_TYPE_X : AE_PLANE_TYPE_NEG_X;
        }

        return AE_PLANE_TYPE_ZERO_Y;
    }

    if (plane[2] == 0.0f)
    {
        return AE_PLANE_TYPE_ZERO_Z;
    }

    return AE_PLANE_TYPE_NON_AXIAL;
}

/* ===== [ construction ] =================================================== */

static c_inline float plane_normalize(float out[4],
    const float plane[4], const int fix_degenerate)
{
    const float length = vec3normalize(out, plane);

    if (fix_degenerate && vec3fix_degenerate_normal(out))
        return 1.0f; // fix degenerate axial vector cases

    return length;
}

static c_inline int plane_from_point_vec(float plane[4], const float p[3],
                            const float v[3], const int fix_degenerate)
{
    if (plane_normalize(plane, v, fix_degenerate) == 0.0f)
        return 0; // the points on the plane are collinear

    // the plane is valid, set distance scalar
    plane[3] = vec3dot(plane, p); return 1;
}

static c_inline int plane_from_vecs(float plane[4], const float dir1[3],
        const float dir2[3], const float p[3], const int fix_degenerate)
{
    vec3cross(plane, dir1, dir2); // calculate the facing normal
    return plane_from_point_vec(plane, p, plane, fix_degenerate);
}

static c_inline int plane_from_points(float plane[4], const float p1[3],
        const float p2[3], const float p3[3], const int fix_degenerate)
{
    float dir_one[3], dir_two[3];

    vec3sub_vec(dir_one, p2, p1);
    vec3sub_vec(dir_two, p3, p1);

    return plane_from_vecs(plane, dir_one, dir_two, p2, fix_degenerate);
}

/*
================================================================================
 * ~~ [ plane frustum math ] ~~ *
--------------------------------------------------------------------------------
*/

#define plane_frustum_copy(dst, src) vec_copy((dst), (src), 24)
#define plane_frustum_cmp(a, b) vec_cmp((a), (b), 24)
#define plane_frustum_cmp_e(a, b, e) vec_cmp_e((a), (b), (e), 24)

#define plane_frustum_zero(p) vec_zero((p), 24)
#define plane_frustum_is_zero(p) vec_is_zero((p), 24)
#define plane_frustum_is_zero_e(p, e) vec_is_zero_e((p), (e), 24)

/* Setup viewing frustum planes from row-major model * view * projection matrix.
 * Use glGetFloatv with GL_PROJECTION_MATRIX and GL_MODELVIEW_MATRIX to get the
 * current column-major matrices in deprecated OpenGL. Doesn't normalize planes.
 */
static c_inline void
plane_frustum_from_model_view_projection_matrix(float planes[24], float mvp[16])
{
    vec4sub_vec(&planes[0 * 4], &mvp[3 * 4], &mvp[0 * 4]); // right
    vec4add_vec(&planes[1 * 4], &mvp[3 * 4], &mvp[0 * 4]); // left
    vec4sub_vec(&planes[2 * 4], &mvp[3 * 4], &mvp[1 * 4]); // top
    vec4add_vec(&planes[3 * 4], &mvp[3 * 4], &mvp[1 * 4]); // bottom
    vec4sub_vec(&planes[4 * 4], &mvp[3 * 4], &mvp[2 * 4]); // far
    vec4add_vec(&planes[5 * 4], &mvp[3 * 4], &mvp[2 * 4]); // near
}

/* Normalize viewing frustum planes. Allows non-scaled distances to be returned
 * from plane intersection tests, which is needed for sphere collision testing.
 * Note that after normalization, plane normals might still be non-unit length.
 */
static c_inline void plane_frustum_normalize(float planes[24])
{
    vec4div_flt(&planes[0 * 4], &planes[0 * 4], vec3mag2(&planes[0 * 4]));
    vec4div_flt(&planes[1 * 4], &planes[1 * 4], vec3mag2(&planes[1 * 4]));
    vec4div_flt(&planes[2 * 4], &planes[2 * 4], vec3mag2(&planes[2 * 4]));
    vec4div_flt(&planes[3 * 4], &planes[3 * 4], vec3mag2(&planes[3 * 4]));
    vec4div_flt(&planes[4 * 4], &planes[4 * 4], vec3mag2(&planes[4 * 4]));
    vec4div_flt(&planes[5 * 4], &planes[5 * 4], vec3mag2(&planes[5 * 4]));
}

/* ===== [ culling ] ======================================================== */

static c_inline int plane_frustum_contains(const float planes[24],
                        const float center[3], const float radius)
{
    size_t i = 0; // point test becomes sphere test if pos radius
    for (; i < 24; i += 4)
    {
        if (planes[i + 0] * center[0] + planes[i + 1] * center[1] +
            planes[i + 2] * center[2] + planes[i + 3] <= -radius)
        {
            return 0;
        }
    }

    return 1;
}

static c_inline float plane_frustum_distance(const float planes[24],
                        const float center[3], const float radius)
{
    float d; // the last dot product test is against the near plane

    size_t i = 0;
    for (; i < 24; i += 4)
    {
        d = planes[i + 0] * center[0] + planes[i + 1] * center[1] +
            planes[i + 2] * center[2] + planes[i + 3]; // vec dot

        if (d <= -radius) return 0.0f;
    }

    return d - radius;
}

static c_inline int plane_frustum_contains_aabbox(const float planes[24],
                                    const float p1[3], const float p2[3])
{
    size_t i = 0; // test the 8 box points against 6 frustum planes
    for (; i < 24; i += 4)
    {
        if (planes[i + 0] * p1[0] + planes[i + 1] * p1[1] +
            planes[i + 2] * p1[2] + planes[i + 3] > 0.0f) continue;
        if (planes[i + 0] * p2[0] + planes[i + 1] * p1[1] +
            planes[i + 2] * p1[2] + planes[i + 3] > 0.0f) continue;
        if (planes[i + 0] * p1[0] + planes[i + 1] * p2[1] +
            planes[i + 2] * p1[2] + planes[i + 3] > 0.0f) continue;
        if (planes[i + 0] * p2[0] + planes[i + 1] * p2[1] +
            planes[i + 2] * p1[2] + planes[i + 3] > 0.0f) continue;
        if (planes[i + 0] * p1[0] + planes[i + 1] * p1[1] +
            planes[i + 2] * p2[2] + planes[i + 3] > 0.0f) continue;
        if (planes[i + 0] * p2[0] + planes[i + 1] * p1[1] +
            planes[i + 2] * p2[2] + planes[i + 3] > 0.0f) continue;
        if (planes[i + 0] * p1[0] + planes[i + 1] * p2[1] +
            planes[i + 2] * p2[2] + planes[i + 3] > 0.0f) continue;
        if (planes[i + 0] * p2[0] + planes[i + 1] * p2[1] +
            planes[i + 2] * p2[2] + planes[i + 3] > 0.0f) continue;

        return 0;
    }

    return 1;
}

/*
================================================================================
 * ~~ [ aabbox math ] ~~ *
--------------------------------------------------------------------------------
XXX: two boxes touching but not overlapping ("tangent") registers as a collision
--------------------------------------------------------------------------------
*/

static c_inline void aabbox2_get_center(float out[2],
            const float minv[2], const float maxv[2])
{
    out[0] = (minv[0] + maxv[0]) / 2.0f;
    out[1] = (minv[1] + maxv[1]) / 2.0f;
}

static c_inline void aabbox3_get_center(float out[3],
            const float minv[3], const float maxv[3])
{
    out[0] = (minv[0] + maxv[0]) / 2.0f;
    out[1] = (minv[1] + maxv[1]) / 2.0f;
    out[2] = (minv[2] + maxv[2]) / 2.0f;
}

static c_inline void
aabbox2_set_center(float minv[2], float maxv[2], const float center[2])
{
    const float half_w = (maxv[0] - minv[0]) / 2.0f;
    const float half_h = (maxv[1] - minv[1]) / 2.0f;

    minv[0] = center[0] - half_w;
    minv[1] = center[1] - half_h;

    maxv[0] = center[0] + half_w;
    maxv[1] = center[1] + half_h;
}

static c_inline void
aabbox3_set_center(float minv[3], float maxv[3], const float center[3])
{
    const float half_w = (maxv[0] - minv[0]) / 2.0f;
    const float half_h = (maxv[1] - minv[1]) / 2.0f;
    const float half_d = (maxv[2] - minv[2]) / 2.0f;

    minv[0] = center[0] - half_w;
    minv[1] = center[1] - half_h;
    minv[2] = center[2] - half_d;

    maxv[0] = center[0] + half_w;
    maxv[1] = center[1] + half_h;
    maxv[2] = center[2] + half_d;
}

static c_inline void aabbox2_center_in( float minv[2], float maxv[2],
                    const float dst_minv[2], const float dst_maxv[2])
{
    float center[2];

    aabbox2_get_center(center, dst_minv, dst_maxv);
    aabbox2_set_center(minv, maxv, center);
}

static c_inline void aabbox3_center_in( float minv[3], float maxv[3],
                    const float dst_minv[3], const float dst_maxv[3])
{
    float center[3];

    aabbox3_get_center(center, dst_minv, dst_maxv);
    aabbox3_set_center(minv, maxv, center);
}

static c_inline float aabbox2_area(const float minv[2], const float maxv[2])
{
    return (maxv[0] - minv[0]) * (maxv[1] - minv[1]);
}

static c_inline float aabbox3_volume(const float minv[3], const float maxv[3])
{
    return (maxv[0] - minv[0]) * (maxv[1] - minv[1]) * (maxv[2] - minv[2]);
}

static c_inline void ellipse_get_outer_aabbox2( float minv[2], float maxv[2],
                            const float e_center[2], const float e_extent[2])
{
    vec2sub_vec(minv, e_center, e_extent);
    vec2add_vec(maxv, e_center, e_extent);
}

static c_inline void ellipsoid_get_outer_aabbox3(float minv[3], float maxv[3],
                            const float e_center[3], const float e_extent[3])
{
    vec3sub_vec(minv, e_center, e_extent);
    vec3add_vec(maxv, e_center, e_extent);
}

static c_inline void aabbox2_get_inner_ellipse( float e_center[2],
        float e_extent[2], const float mnv[2], const float mxv[2])
{
    aabbox2_get_center(e_center, mnv, mxv);

    e_extent[0] = ae_fabsf(mxv[0] - e_center[0]);
    e_extent[1] = ae_fabsf(mxv[1] - e_center[1]);
}

static c_inline void aabbox3_get_inner_ellipsoid( float e_center[3],
        float e_extent[3], const float minv[3], const float maxv[3])
{
    aabbox3_get_center(e_center, minv, maxv);

    e_extent[0] = ae_fabsf(maxv[0] - e_center[0]);
    e_extent[1] = ae_fabsf(maxv[1] - e_center[1]);
    e_extent[2] = ae_fabsf(maxv[2] - e_center[2]);
}

static c_inline int // returns true if box a intersects with b
aabbox2_vs_aabbox2( const float amin[2], const float amax[2],
                    const float bmin[2], const float bmax[2])
{
    return  ! ( amax[0] < bmin[0] ||
                amax[1] < bmin[1] ||
                amin[0] > bmax[0] ||
                amin[1] > bmax[1] );
}

static c_inline int // returns true if box a intersects with b
aabbox3_vs_aabbox3( const float amin[3], const float amax[3],
                    const float bmin[3], const float bmax[3])
{
    return  ! ( amax[0] < bmin[0] ||
                amax[1] < bmin[1] ||
                amax[2] < bmin[2] ||
                amin[0] > bmax[0] ||
                amin[1] > bmax[1] ||
                amin[2] > bmax[2] );
}

static c_inline int
aabbox2_vs_point(const float minv[2], const float maxv[2], const float point[2])
{
    return aabbox2_vs_aabbox2(minv, maxv, point, point);
}

static c_inline int
aabbox3_vs_point(const float minv[3], const float maxv[3], const float point[3])
{
    return aabbox3_vs_aabbox3(minv, maxv, point, point);
}

static c_inline void aabbox2_points(float out[8],
        const float minv[2], const float maxv[2])
{
    out[0] = minv[0]; out[1] = minv[1]; // btm lf
    out[2] = maxv[0]; out[3] = minv[1]; // btm rt
    out[4] = maxv[0]; out[5] = maxv[1]; // top rt
    out[6] = minv[0]; out[7] = maxv[1]; // top lf
}

static c_inline void aabbox3_points(float out[24],
        const float minv[3], const float maxv[3])
{
    // btm (assuming +y is up) for a counter-clockwise winding
    out[ 0] = minv[0]; out[ 1] = minv[1]; out[ 2] = minv[2];
    out[ 3] = maxv[0]; out[ 4] = minv[1]; out[ 5] = minv[2];
    out[ 6] = maxv[0]; out[ 7] = minv[1]; out[ 8] = maxv[2];
    out[ 9] = minv[0]; out[10] = minv[1]; out[11] = maxv[2];

    // top (assuming +y is up) for a counter-clockwise winding
    out[12] = minv[0]; out[13] = maxv[1]; out[14] = minv[2];
    out[15] = maxv[0]; out[16] = maxv[1]; out[17] = minv[2];
    out[18] = maxv[0]; out[19] = maxv[1]; out[20] = maxv[2];
    out[21] = minv[0]; out[22] = maxv[1]; out[23] = maxv[2];
}

/* get the axis-aligned bounding box of a rotated (oriented) axis-aligned box.
 * if NULL is passed to center, then the rotation will be about aabbox center.
 */
static void aabbox2_rotated_bounds_ex(  float min_out[2],
                                        float max_out[2],

                                        const float min_val[2],
                                        const float max_val[2],

                                        const float center[2],
                                        const float sin_theta,
                                        const float cos_theta)
{
    float* real_center = (float *)center, points[8];

    if (real_center == NULL)
    {
        real_center = (float *)alloca(sizeof( float[2] ));
        aabbox2_get_center(real_center, min_val, max_val);
    }

    aabbox2_points(points, min_val, max_val);

    rotate_point2_ex(points + 0, points + 0, real_center, sin_theta, cos_theta);
    rotate_point2_ex(points + 2, points + 2, real_center, sin_theta, cos_theta);
    rotate_point2_ex(points + 4, points + 4, real_center, sin_theta, cos_theta);
    rotate_point2_ex(points + 6, points + 6, real_center, sin_theta, cos_theta);

    // get the bounding box of rotated points
    vec2copy(min_out, points);
    vec2copy(max_out, points);

    vec2min_vec(min_out, min_out, points + 2);
    vec2max_vec(max_out, max_out, points + 2);

    vec2min_vec(min_out, min_out, points + 4);
    vec2max_vec(max_out, max_out, points + 4);

    vec2min_vec(min_out, min_out, points + 6);
    vec2max_vec(max_out, max_out, points + 6);
}

static c_inline void aabbox2_rotated_bounds(float min_out[2],
                                            float max_out[2],

                                            const float min_val[2],
                                            const float max_val[2],

                                            const float center[2],
                                            const float theta)
{
    aabbox2_rotated_bounds_ex(min_out, max_out, min_val, max_val, center,
                                        ae_sinf(theta), ae_cosf(theta) );
}

/*
================================================================================
 * ~~ [ rectangle math ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline void flt_rect_copy(float* dst, const float* const src)
{
    memcpy(dst, src, sizeof(float[4]));
}

static c_inline void int_rect_copy(int* dst, const int* const src)
{
    memcpy(dst, src, sizeof(int[4]));
}

static c_inline int flt_rect_cmp(const float* const a, const float* const b)
{
    return memcmp(a, b, sizeof(float[4])) == 0;
}

static c_inline int int_rect_cmp(const int* const a, const int* const b)
{
    return memcmp(a, b, sizeof(int[4])) == 0;
}

#define flt_rect_cmp_e vec4cmp_e

static c_inline void flt_rect_zero(float rect[4])
{
    memset(rect, 0, sizeof(float[4]));
}

static c_inline void int_rect_zero(int rect[4])
{
    memset(rect, 0, sizeof(int[4]));
}

static c_inline int flt_rect_is_zero(const float rect[4])
{
    return !(rect[0] || rect[1] || rect[2] || rect[3]);
}

static c_inline int int_rect_is_zero(const int rect[4])
{
    return !(rect[0] || rect[1] || rect[2] || rect[3]);
}

#define flt_rect_is_zero_e vec4is_zero_e

/* ===== [ center ] ========================================================= */

#define RECT_GET_CENTER(center, rect) do        \
{                                               \
    (center)[0] = (rect)[0] + ((rect)[2] / 2);  \
    (center)[1] = (rect)[1] + ((rect)[3] / 2);  \
}                                               \
while (AE_NULL_WHILE_LOOP_CONDITION)

static c_inline void flt_rect_get_center(float center[2], const float rect[4])
{
    RECT_GET_CENTER(center, rect);
}

static c_inline void int_rect_get_center(int center[2], const int rect[4])
{
    RECT_GET_CENTER(center, rect);
}

#define RECT_SET_CENTER(rect, center) do        \
{                                               \
    (rect)[0] = (center)[0] - ((rect)[2] / 2);  \
    (rect)[1] = (center)[1] - ((rect)[3] / 2);  \
}                                               \
while (AE_NULL_WHILE_LOOP_CONDITION)

static c_inline void flt_rect_set_center(float rect[4], const float center[2])
{
    RECT_SET_CENTER(rect, center);
}

static c_inline void int_rect_set_center(int rect[4], const int center[2])
{
    RECT_SET_CENTER(rect, center);
}

#define RECT_CENTER_IN(type, rect, target) do   \
{                                               \
    type center[2];                             \
                                                \
    RECT_GET_CENTER(center, target);            \
    RECT_SET_CENTER(rect, center);              \
}                                               \
while (AE_NULL_WHILE_LOOP_CONDITION)

static c_inline void flt_rect_center_in(float rect[4], const float target[4])
{
    RECT_CENTER_IN(float, rect, target);
}

static c_inline void int_rect_center_in(int rect[4], const int target[4])
{
    RECT_CENTER_IN(int, rect, target);
}

/* ===== [ area ] =========================================================== */

static c_inline float flt_rect_area(const float rect[4])
{
    return ae_fabsf(rect[2] * rect[3]);
}

static c_inline int int_rect_area(const int rect[4])
{
    return ae_iabs(rect[2] * rect[3]);
}

/* ===== [ aabbox conversion ] ============================================== */

#define RECT_TO_AABBOX(mn, mx, rect) do \
{                                       \
    (mn)[0] = (rect)[0];                \
    (mx)[0] = (rect)[0] + (rect)[2];    \
    (mn)[1] = (rect)[1];                \
    (mx)[1] = (rect)[1] + (rect)[3];    \
}                                       \
while (AE_NULL_WHILE_LOOP_CONDITION)

static c_inline void
flt_rect_to_aabbox(float mn[2], float mx[2], const float rect[4])
{
    RECT_TO_AABBOX(mn, mx, rect);
}

static c_inline void
int_rect_to_aabbox(int mn[2], int mx[2], const int rect[4])
{
    RECT_TO_AABBOX(mn, mx, rect);
}

#define RECT_FROM_AABBOX(rect, mn, mx) do   \
{                                           \
    (rect)[0] = (mn)[0];                    \
    (rect)[2] = (mx)[0] - (mn)[0];          \
    (rect)[1] = (mn)[1];                    \
    (rect)[3] = (mx)[1] - (mn)[1];          \
}                                           \
while (AE_NULL_WHILE_LOOP_CONDITION)

static c_inline void
flt_rect_from_aabbox(float rect[4], const float mn[2], const float mx[2])
{
    RECT_FROM_AABBOX(rect, mn, mx);
}

static c_inline void
int_rect_from_aabbox(int rect[4], const int mn[2], const int mx[2])
{
    RECT_FROM_AABBOX(rect, mn, mx);
}

/* ===== [ collision detection ] ============================================ */

static c_inline int flt_rect_vs_rect(const float a[4], const float b[4])
{
    // TODO would probably be cleaner to do this inline
    float amin[2], amax[2];
    float bmin[2], bmax[2];

    flt_rect_to_aabbox(amin, amax, a);
    flt_rect_to_aabbox(bmin, bmax, b);

    return aabbox2_vs_aabbox2(amin, amax, bmin, bmax);
}

static c_inline int int_rect_vs_rect(const int a[4], const int b[4])
{
    return  ! ( (a[0] + a[2]) < b[0] ||
                (a[1] + a[3]) < b[1] ||
                a[0] > (b[0] + b[2]) ||
                a[1] > (b[1] + b[3]) );
}

static c_inline int flt_rect_vs_point(const float rect[4], const float point[2])
{
    const float r[4] = { point[0], point[1], 0.0f, 0.0f };
    return flt_rect_vs_rect(rect, r);
}

static c_inline int int_rect_vs_point(const int rect[4], const int point[2])
{
    const int r[4] = { point[0], point[1], 0, 0 };
    return int_rect_vs_rect(rect, r);
}

/* ===== [ normalized coordinates ] ========================================= */

static c_inline void flt_rect_point_to_normalized(float out[2],
                    const float rect[4], const float rpoint[2])
{
    ae_assert(rect[2] && rect[3], "zero size rect: %f %f %f %f",
                            rect[0], rect[1], rect[2], rect[3]);

    out[0] = (rpoint[0] - rect[0]) / rect[2];
    out[1] = (rpoint[1] - rect[1]) / rect[3];
}

static c_inline void flt_rect_normalized_to_point(float out[2],
                    const float rect[4], const float normal[2])
{
    out[0] = (normal[0] * rect[2]) + rect[0];
    out[1] = (normal[1] * rect[3]) + rect[1];
}

/* ===== [ type conversion ] ================================================ */

static c_inline void
flt_rect_to_int_ex(int out[4], const float rect[4], const ae_ftoi_mode_t mode)
{
#if 0
    size_t i = 0;
    for (; i < 4; i++) out[i] = ae_ftoi_ex(rect[i], mode);
#else
    switch (mode)
    {
        case AE_FTOI_MODE_FLOOR:
        {
            // floor is the default rounding mode.
            out[0] = ae_ftoi(rect[0]);
            out[1] = ae_ftoi(rect[1]);
            out[2] = ae_ftoi(rect[2]);
            out[3] = ae_ftoi(rect[3]);
        }
        break;

        case AE_FTOI_MODE_CEIL:
        {
            out[0] = ae_ftoi(ae_ceilf(rect[0]));
            out[1] = ae_ftoi(ae_ceilf(rect[1]));
            out[2] = ae_ftoi(ae_ceilf(rect[2]));
            out[3] = ae_ftoi(ae_ceilf(rect[3]));
        }
        break;

        case AE_FTOI_MODE_ROUND:
        {
            out[0] = ae_ftoi(ae_roundf(rect[0]));
            out[1] = ae_ftoi(ae_roundf(rect[1]));
            out[2] = ae_ftoi(ae_roundf(rect[2]));
            out[3] = ae_ftoi(ae_roundf(rect[3]));
        }
        break;

        default: assert(0); break;
    }
#endif
}

static c_inline void flt_rect_to_int(int out[4], const float rect[4])
{
#if 0
    flt_rect_to_int_ex(out, rect, AE_FTOI_MODE_FLOOR);
#else
    out[0] = ae_ftoi(rect[0]);
    out[1] = ae_ftoi(rect[1]);
    out[2] = ae_ftoi(rect[2]);
    out[3] = ae_ftoi(rect[3]);
#endif
}

static c_inline void int_rect_to_flt(float out[4], const int rect[4])
{
    out[0] = (float)rect[0];
    out[1] = (float)rect[1];
    out[2] = (float)rect[2];
    out[3] = (float)rect[3];
}

/* ===== [ rotated bounds ] ================================================= */

static c_inline void flt_rect_rotated_bounds_ex(float out[4], const float rect[4],
                                                const float center[2],
                                                const float sin_theta,
                                                const float cos_theta)
{
    // TODO: optimize this by just doing all this stuff inline (call into macro?)
    float minv[2];
    float maxv[2];

    flt_rect_to_aabbox(minv, maxv, rect);

    aabbox2_rotated_bounds_ex(minv, maxv, minv, maxv, center, sin_theta, cos_theta);
    flt_rect_from_aabbox(out, minv, maxv);
}

static c_inline void flt_rect_rotated_bounds(float out[4], const float rect[4],
                                    const float center[2], const float theta)
{
    flt_rect_rotated_bounds_ex(out, rect, center, ae_sinf(theta), ae_cosf(theta));
}

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_MATH_H__ */
