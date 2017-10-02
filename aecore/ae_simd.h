/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
--- This file is intended to fill a few gaps in some old SIMD instruction sets.
----------------------------------------------------------------------------- */
#ifndef __AE_SIMD_H__
#define __AE_SIMD_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

// http://jrfonseca.blogspot.com/2008/09/fast-sse2-pow-tables-or-polynomials.html
#if defined(__SSE2__)

static c_inline __m128 _mm_clamp_ps(__m128 x, __m128 mn, __m128 mx)
{
    return _mm_max_ps(mn, _mm_min_ps(mx, x));
}

#define _MM_POLY0(x, c0) \
    _mm_set1_ps(c0)

#define _MM_POLY1(x, c0, c1) \
    _mm_add_ps(_mm_mul_ps(_MM_POLY0(x, c1), x), _mm_set1_ps(c0))

#define _MM_POLY2(x, c0, c1, c2) \
    _mm_add_ps(_mm_mul_ps(_MM_POLY1(x, c1, c2), x), _mm_set1_ps(c0))

#define _MM_POLY3(x, c0, c1, c2, c3) \
    _mm_add_ps(_mm_mul_ps(_MM_POLY2(x, c1, c2, c3), x), _mm_set1_ps(c0))

#define _MM_POLY4(x, c0, c1, c2, c3, c4) \
    _mm_add_ps(_mm_mul_ps(_MM_POLY3(x, c1, c2, c3, c4), x), _mm_set1_ps(c0))

#define _MM_POLY5(x, c0, c1, c2, c3, c4, c5) \
    _mm_add_ps(_mm_mul_ps(_MM_POLY4(x, c1, c2, c3, c4, c5), x), _mm_set1_ps(c0))

static c_inline __m128 _mm_exp2_ps(__m128 x)
{
    #define _MM_EXP_POLY_DEGREE 3

    __m128i ipart; /* integer part */
    __m128 fpart, expipart, expfpart;

    x = _mm_min_ps(x, _mm_set1_ps( 129.00000f));
    x = _mm_max_ps(x, _mm_set1_ps(-126.99999f));

    /* ipart = int(x - 0.5) */
    ipart = _mm_cvtps_epi32(_mm_sub_ps(x, _mm_set1_ps(0.5f)));

    /* fpart = x - ipart */
    fpart = _mm_sub_ps(x, _mm_cvtepi32_ps(ipart));

    /* expipart = (float) (1 << ipart) */
    expipart = _mm_castsi128_ps(_mm_slli_epi32(_mm_add_epi32(ipart, _mm_set1_epi32(127)), 23));

    /* minimax polynomial fit of 2 ** x, in range [-0.5, 0.5] */
    #if _MM_EXP_POLY_DEGREE == 5
        expfpart = _MM_POLY5(fpart, 9.9999994e-1f, 6.9315308e-1f, 2.4015361e-1f, 5.5826318e-2f, 8.9893397e-3f, 1.8775767e-3f);
    #elif _MM_EXP_POLY_DEGREE == 4
        expfpart = _MM_POLY4(fpart, 1.0000026f, 6.9300383e-1f, 2.4144275e-1f, 5.2011464e-2f, 1.3534167e-2f);
    #elif _MM_EXP_POLY_DEGREE == 3
        expfpart = _MM_POLY3(fpart, 9.9992520e-1f, 6.9583356e-1f, 2.2606716e-1f, 7.8024521e-2f);
    #elif _MM_EXP_POLY_DEGREE == 2
        expfpart = _MM_POLY2(fpart, 1.0017247f, 6.5763628e-1f, 3.3718944e-1f);
    #else
        #error
    #endif

    return _mm_mul_ps(expipart, expfpart);
}

static c_inline __m128 _mm_log2_ps(__m128 x)
{
    #define _MM_LOG_POLY_DEGREE 5

    __m128i exp = _mm_set1_epi32(0x7F800000);
    __m128i mant = _mm_set1_epi32(0x007FFFFF);
    __m128 one = _mm_set1_ps( 1.0f);
    __m128i i = _mm_castps_si128(x);
    __m128 e = _mm_cvtepi32_ps(_mm_sub_epi32(_mm_srli_epi32(_mm_and_si128(i, exp), 23), _mm_set1_epi32(127)));
    __m128 m = _mm_or_ps(_mm_castsi128_ps(_mm_and_si128(i, mant)), one);
    __m128 p;

    /* Minimax polynomial fit of log2(x) / (x - 1), for x in range [1, 2] */
    #if _MM_LOG_POLY_DEGREE == 6
        p = _MM_POLY5( m, 3.1157899f, -3.3241990f, 2.5988452f, -1.2315303f,  3.1821337e-1f, -3.4436006e-2f);
    #elif _MM_LOG_POLY_DEGREE == 5
        p = _MM_POLY4(m, 2.8882704548164776201f, -2.52074962577807006663f, 1.48116647521213171641f, -0.465725644288844778798f, 0.0596515482674574969533f);
    #elif _MM_LOG_POLY_DEGREE == 4
        p = _MM_POLY3(m, 2.61761038894603480148f, -1.75647175389045657003f, 0.688243882994381274313f, -0.107254423828329604454f);
    #elif _MM_LOG_POLY_DEGREE == 3
        p = _MM_POLY2(m, 2.28330284476918490682f, -1.04913055217340124191f, 0.204446009836232697516f);
    #else
        #error
    #endif

    /* This effectively increases the polynomial degree by one, but ensures that log2(1) == 0 */
    p = _mm_mul_ps(p, _mm_sub_ps(m, one));

    return _mm_add_ps(p, e);
}

static c_inline __m128 _mm_pow_ps(__m128 a, __m128 b)
{
    return _mm_exp2_ps(_mm_mul_ps(_mm_log2_ps(a), b));
}

#endif /* __SSE2__ */

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_SIMD_H__ */
