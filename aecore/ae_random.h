/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_RANDOM_H__
#define __AE_RANDOM_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif
#ifndef __AE_MEMORY_H__
#include <ae_memory.h>
#endif
#ifndef __AE_BRANCH_H__
#include <ae_branch.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_random_init(int argc, char** argv);
void ae_random_quit(void); // private init

/* Generate random bits for PRNG seeds. We try to generate different values
 * between calls, but it's not guaranteed. These tend to be extremely slow.
 */
AE_DECL void AE_CALL ae_random_gen_seed(void* buffer, size_t size);
AE_DECL u32  AE_CALL ae_random_gen_seed_u32(void);

/*
================================================================================
 * ~~ [ generators ] ~~ *
--------------------------------------------------------------------------------
We make all implementations available here for backwards compatibility reasons.
If the seed state used in a world generation algorithm must be stored in a saved
game, we want to be able to reproduce the same level consistently between runs.
--------------------------------------------------------------------------------
*/

/* Default global random for stuff like particle effects, gameplay procs, etc.
 * This should be the fastest generator we have, not necessarily the "best".
 * No seed function is available, as it would lock us into one method forever.
 */
AE_DECL u32 AE_CALL ae_random_u32(void);

/* ===== [ xorshift8 ] ====================================================== */

AE_DECL void AE_CALL ae_random_xorshift8_seed(const u8 seed);

AE_DECL u8   AE_CALL ae_random_xorshift8(void);
AE_DECL u8   AE_CALL ae_random_xorshift8_ex(u8* state);

/* ===== [ xorshift16 ] ===================================================== */

AE_DECL void AE_CALL ae_random_xorshift16_seed(const u16 seed);

AE_DECL u16  AE_CALL ae_random_xorshift16(void);
AE_DECL u16  AE_CALL ae_random_xorshift16_ex(u16* state);

/* ===== [ xorshift32 ] ===================================================== */

AE_DECL void AE_CALL ae_random_xorshift32_seed(const u32 seed);

AE_DECL u32  AE_CALL ae_random_xorshift32(void);
AE_DECL u32  AE_CALL ae_random_xorshift32_ex(u32* state);

/* ===== [ xorshift64 ] ===================================================== */

AE_DECL void AE_CALL ae_random_xorshift64_seed(const u32 L, const u32 R);

AE_DECL u64  AE_CALL ae_random_xorshift64(void);
AE_DECL u64  AE_CALL ae_random_xorshift64_ex(u64* state);

/* ===== [ xorshift128 ] ==================================================== */

AE_DECL void AE_CALL ae_random_xorshift128_seed(const u32 x, const u32 y,
                                                const u32 z, const u32 w);

AE_DECL u32  AE_CALL ae_random_xorshift128(void);
AE_DECL u32  AE_CALL ae_random_xorshift128_ex(u32 state[4]);

/*
================================================================================
 * ~~ [ integer distributions ] ~~ *
--------------------------------------------------------------------------------
*/

/* generate a uniformly distributed unsigned integer r, where 0 <= r < bound.
    [0, bound)
 */
static u32 ae_random_bounded_u32(u32 bound)
{
    #if defined(_MSC_VER)
        #pragma warning(push) // unary minus operator on unsigned type
        #pragma warning(disable: 4146)
    #endif

    /*  To avoid bias, we need to make the range of the RNG a multiple of bound,
        which we do by dropping output less than a threshold.
        A naive scheme to calculate the threshold would be to do:

            u32 threshold = 0x100000000ull % bound;

        but 64-bit div/mod is slower than 32-bit div/mod
        (especially on 32-bit platforms). In essence, we do:

            u32 threshold = (0x100000000ull-bound) % bound;

        because this version will calculate the same modulus,
        but the LHS value is less than 2^32.
    */
    u32 threshold = -bound % bound;

    /*  Uniformity guarantees that this loop will end. In practice, it should
        usually end quickly; on average (assuming all bounds are equally likely),
        82.25% of the time, we can expect it to require just one iteration. In
        the worst case, someone passes a bound of 2^31 + 1 (i.e., 2147483649),
        which invalidates almost 50% of the range. In practice, bounds
        are typically small and only a tiny amount of the range is eliminated.
    */
    for (;;)
    {
        const u32 r = ae_random_u32();
        if (ae_likely(r >= threshold)) return r % bound;
    }

    #if defined(_MSC_VER)
        #pragma warning(pop)
    #endif
}

/* generate uniformly distributed signed integer r, where min_v <= r < max_v.
    [min_v, max_v)
 */
static s32 ae_random_bounded_s32(s32 min_v, s32 max_v)
{
    // TODO: abs(max_v - min_v) doesn't fix this. what does?
    ae_assert(min_v <= max_v, "%i > %i", min_v, max_v);

    return min_v + (s32)ae_random_bounded_u32(max_v - min_v);
}

/*
================================================================================
 * ~~ [ array shuffling (permutation) ] ~~ *
--------------------------------------------------------------------------------
*/

// randomly shuffle array elements in-place. uses the global generator.
AE_DECL void AE_CALL memshuffle(void* base, size_t count, size_t size);

/*
================================================================================
 * ~~ [ lower-level random float api ] ~~ *
--------------------------------------------------------------------------------
TODO: fight bias with "rejection sampling" (do { v = rand } while (v is valid))?
--------------------------------------------------------------------------------
*/

static c_inline float ae_random_flt_cc(void) // [0, 1]
{
    return (float)ae_random_u32() / AE_U32_MAX;
}

static c_inline double ae_random_dbl_cc(void) // [0, 1]
{
    return (double)ae_random_u32() / AE_U32_MAX;
}

static c_inline float ae_random_flt_co(void) // [0, 1)
{
    return (float)ae_random_u32() / (AE_U32_MAX + 1.0f);
}

static c_inline double ae_random_dbl_co(void) // [0, 1)
{
    return (double)ae_random_u32() / (AE_U32_MAX + 1.0);
}

static c_inline float ae_random_flt_oc(void) // (0, 1]
{
    return 1.0f - ae_random_flt_co();
}

static c_inline double ae_random_dbl_oc(void) // (0, 1]
{
    return 1.0 - ae_random_dbl_co();
}

static c_inline float ae_random_flt_oo(void) // (0, 1)
{
    return ((float)ae_random_u32() + 0.5f) / (AE_U32_MAX + 1.0f);
}

static c_inline double ae_random_dbl_oo(void) // (0, 1)
{
    return ((double)ae_random_u32() + 0.5) / (AE_U32_MAX + 1.0);
}

/*
================================================================================
 * ~~ [ higher-level random float api ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float ae_random_flt(void) // [0, 1]
{
    return ae_random_flt_cc();
}

static c_inline double ae_random_dbl(void) // [0, 1]
{
    return ae_random_dbl_cc();
}

#if 1
    /* this version has one less multiply, as the compiler can fold the divisor.
     */
    static c_inline float ae_random_bi_flt(void) // [-1, 1]
    {
        return (float)ae_random_u32() / (AE_U32_MAX * 0.5f) - 1.0f;
    }

    static c_inline double ae_random_bi_dbl(void) // [-1, 1]
    {
        return (double)ae_random_u32() / (AE_U32_MAX * 0.5) - 1.0;
    }
#else
    static c_inline float ae_random_bi_flt(void) // [-1, 1]
    {
        return ae_random_flt() * 2.0f - 1.0f;
    }

    static c_inline double ae_random_bi_dbl(void) // [-1, 1]
    {
        return ae_random_dbl() * 2.0 - 1.0;
    }
#endif

#if 1
    static c_inline float ae_random_bounded_flt(const float a, const float b)
    {
        return a + (b - a) * ae_random_flt();
    }

    static c_inline double ae_random_bounded_dbl(const double a, const double b)
    {
        return a + (b - a) * ae_random_dbl();
    }
#else
    static c_inline float ae_random_bounded_flt(const float a, const float b)
    {
        const float t = ae_random_flt();
        return (1.0f - t) * a + t * b;
    }

    static c_inline double ae_random_bounded_dbl(const double a, const double b)
    {
        const double t = ae_random_dbl();
        return (1.0 - t) * a + t * b;
    }
#endif

/* ===== [ random booleans ] ================================================ */

static c_inline int ae_random_bool(void)
{
    return (ae_random_u32() & 1); // int is odd
}

static c_inline float ae_random_flt_sign(void)
{
    return (ae_random_u32() & 1) ? 1.0f : -1.0f;
}

static c_inline double ae_random_dbl_sign(void)
{
    return (ae_random_u32() & 1) ? 1.0 : -1.0;
}

/*
================================================================================
 * ~~ [ float distributions ] ~~ *
--------------------------------------------------------------------------------
*/

#define ae_random_uniform_flt ae_random_bounded_flt
#define ae_random_uniform_dbl ae_random_bounded_dbl

static c_inline float ae_random_exponential_flt(const float mean)
{
    // negative exponential (p-erlang order 1)
    return -mean * ae_logf(ae_random_flt_oc());
}

static c_inline double ae_random_exponential_dbl(const double mean)
{
    return -mean * log(ae_random_dbl_oc());
}

AE_DECL float  AE_CALL ae_random_erlang_flt(int order, float  mean);
AE_DECL double AE_CALL ae_random_erlang_dbl(int order, double mean);

AE_DECL float  AE_CALL
ae_random_gauss_flt_ex(const float  mu, const float  sigma, float * next);

AE_DECL double AE_CALL
ae_random_gauss_dbl_ex(const double mu, const double sigma, double* next);

AE_DECL float  AE_CALL
ae_random_gauss_flt(const float  mu, const float  sigma);

AE_DECL double AE_CALL
ae_random_gauss_dbl(const double mu, const double sigma);

/*
================================================================================
 * ~~ [ random angles ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline float ae_random_bi_radian_flt(void) // [-pi, pi]
{
    return ae_random_bi_flt() * M_PI;
}

static c_inline double ae_random_bi_radian_dbl(void) // [-pi, pi]
{
    return ae_random_bi_dbl() * M_PI;
}

static c_inline float ae_random_radian_flt(void) // [0, 2 * pi]
{
    return ae_random_flt() * 2.0f * M_PI;
}

static c_inline double ae_random_radian_dbl(void) // [0, 2 * pi]
{
    return ae_random_dbl() * 2.0 * M_PI;
}

static c_inline float ae_random_bi_degree_flt(void) // [-180, 180]
{
    return ae_random_bi_flt() * 180.0f;
}

static c_inline double ae_random_bi_degree_dbl(void) // [-180, 180]
{
    return ae_random_bi_dbl() * 180.0;
}

static c_inline float ae_random_degree_flt(void) // [0, 360]
{
    return ae_random_flt() * 360.0f;
}

static c_inline double ae_random_degree_dbl(void) // [0, 360]
{
    return ae_random_dbl() * 360.0;
}

/*
================================================================================
 * ~~ [ random vectors ] ~~ *
--------------------------------------------------------------------------------
FIXME: random vectors in R^3 exhibit a slight bias towards the edges of a cube -
still not as objectionable as artifacts from random angles converted to vectors!
--------------------------------------------------------------------------------
*/

static c_inline void ae_random_unit_vec(float* v, size_t n)
{
    size_t i = 0;
    for (; i < n; i++) v[i] = ae_random_bi_flt();

    vec_normalize(v, v, n);
}

#if 0
    #define ae_random_unit_vec2(v) ae_random_unit_vec((v), 2)
    #define ae_random_unit_vec3(v) ae_random_unit_vec((v), 3)
    #define ae_random_unit_vec4(v) ae_random_unit_vec((v), 4)
#else
    static c_inline void ae_random_unit_vec2(float v[2])
    {
        v[0] = ae_random_bi_flt();
        v[1] = ae_random_bi_flt();

        vec2normalize(v, v);
    }

    static c_inline void ae_random_unit_vec3(float v[3])
    {
        v[0] = ae_random_bi_flt();
        v[1] = ae_random_bi_flt();
        v[2] = ae_random_bi_flt();

        vec3normalize(v, v);
    }

    static c_inline void ae_random_unit_vec4(float v[4])
    {
        v[0] = ae_random_bi_flt();
        v[1] = ae_random_bi_flt();
        v[2] = ae_random_bi_flt();
        v[3] = ae_random_bi_flt();

        vec4normalize(v, v);
    }
#endif

/*
================================================================================
 * ~~ [ point on line segment ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline void
ae_random_seg_point(float* out, const float* a, const float* b, size_t n)
{
    vec_lerp(out, a, b, ae_random_flt(), n);
}

#if 0
    #define ae_random_seg2_point(d, a, b) ae_random_seg_point((d), (a), (b), 2)
    #define ae_random_seg3_point(d, a, b) ae_random_seg_point((d), (a), (b), 3)
#else
    static c_inline void ae_random_seg2_point(float out[2],
                        const float a[2], const float b[2])
    {
        vec2lerp(out, a, b, ae_random_flt());
    }

    static c_inline void ae_random_seg3_point(float out[3],
                        const float a[3], const float b[3])
    {
        vec3lerp(out, a, b, ae_random_flt());
    }
#endif

/*
================================================================================
 * ~~ [ point on triangle edge ] ~~ *
--------------------------------------------------------------------------------
*/

static c_inline void
ae_random_tri_edge_index( float* out, const float* v0, const float* v1,
                    const float* v2, const size_t idx, const size_t n)
{
    switch (idx) {
        case  0: { ae_random_seg_point(out, v0, v1, n); } break;
        case  1: { ae_random_seg_point(out, v1, v2, n); } break;
        default: { ae_random_seg_point(out, v2, v0, n); } break;
    }
}

#if 0
    #define ae_random_tri2_edge_index(out, v0, v1, v2, idx) /* 2D tri edge */ \
            ae_random_tri_edge_index((out), (v0), (v1), (v2), (idx), 2)

    #define ae_random_tri3_edge_index(out, v0, v1, v2, idx) /* 3D tri edge */ \
            ae_random_tri_edge_index((out), (v0), (v1), (v2), (idx), 3)
#else
    static c_inline void
    ae_random_tri2_edge_index(float out[2], const float v0[2], const float v1[2],
                                            const float v2[2], const size_t idx)
    {
        switch (idx) {
            case  0: { ae_random_seg2_point(out, v0, v1); } break;
            case  1: { ae_random_seg2_point(out, v1, v2); } break;
            default: { ae_random_seg2_point(out, v2, v0); } break;
        }
    }

    static c_inline void
    ae_random_tri3_edge_index(float out[3], const float v0[3], const float v1[3],
                                            const float v2[3], const size_t idx)
    {
        switch (idx) {
            case  0: { ae_random_seg3_point(out, v0, v1); } break;
            case  1: { ae_random_seg3_point(out, v1, v2); } break;
            default: { ae_random_seg3_point(out, v2, v0); } break;
        }
    }
#endif

static c_inline void ae_random_tri_edge(float* out, const float* v0,
                        const float* v1, const float* v2, size_t n)
{
    ae_random_tri_edge_index(out, v0, v1, v2, ae_random_bounded_u32(3), n);
}

#if 0
    #define ae_random_tri2_edge(out, v0, v1, v2) /* 2D tri edge */ \
            ae_random_tri_edge((out), (v0), (v1), (v2), 2)

    #define ae_random_tri3_edge(out, v0, v1, v2) /* 3D tri edge */ \
            ae_random_tri_edge((out), (v0), (v1), (v2), 3)
#else
    static c_inline void ae_random_tri2_edge(float out[2], const float v0[2],
                                        const float v1[2], const float v2[2])
    {
        ae_random_tri2_edge_index(out, v0, v1, v2, ae_random_bounded_u32(3));
    }

    static c_inline void ae_random_tri3_edge(float out[3], const float v0[3],
                                        const float v1[3], const float v2[3])
    {
        ae_random_tri3_edge_index(out, v0, v1, v2, ae_random_bounded_u32(3));
    }
#endif

/*
================================================================================
 * ~~ [ point on triangle face ] ~~ *
--------------------------------------------------------------------------------
FIXME: selected points cluster slightly (but still noticeably) around one vertex
--------------------------------------------------------------------------------
*/

static c_inline void ae_random_tri_point(float* pt, const float* v0,
                        const float* v1, const float* v2, size_t n)
{
    ae_random_seg_point(pt, v1, v2, n); /* edge point */
    ae_random_seg_point(pt, v0, pt, n); /* and median */
}

#if 0
    #define ae_random_tri2_point(pt, v0, v1, v2) /* 2D tri face */ \
            ae_random_tri_point((pt), (v0), (v1), (v2), 2)

    #define ae_random_tri3_point(pt, v0, v1, v2) /* 3D tri face */ \
            ae_random_tri_point((pt), (v0), (v1), (v2), 3)
#else
    static c_inline void ae_random_tri2_point(float pt[2], const float v0[2],
                                        const float v1[2], const float v2[2])
    {
        ae_random_seg2_point(pt, v1, v2); /* edge point */
        ae_random_seg2_point(pt, v0, pt); /* and median */
    }

    static c_inline void ae_random_tri3_point(float pt[3], const float v0[3],
                                        const float v1[3], const float v2[3])
    {
        ae_random_seg3_point(pt, v1, v2); /* edge point */
        ae_random_seg3_point(pt, v0, pt); /* and median */
    }
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_RANDOM_H__ */
