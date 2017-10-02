/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void ae_random_gen_seed(void* buffer, size_t size)
{
    // TODO: read from /dev/urandom, RtlGenRandom, hash chain fallback
    // (generate one random number, hash it, hash the hash, and so on).
    AE_STUB();
}

u32 ae_random_gen_seed_u32(void)
{
    // TODO: use RDTSC on linux or microseconds from clock_gettime,
    // perhaps take the CRC of memory filled by ae_random_gen_seed.
    u32 value;

    #if defined(_WIN32)
    if (rand_s(&value))
    #endif

    // function addresses should be random on most modern systems.
    value = ae_hash_num((unsigned)time(NULL)) ^ (size_t) &printf;

    return value;
}

u32 ae_random_u32(void)
{
    #if defined(AE_RANDOM_USE_XORSHIFT128)
        return ae_random_xorshift128();

    #elif defined(AE_RANDOM_USE_XORSHIFT32)
        return ae_random_xorshift32();

    #else
        return ae_random_xorshift32();
    #endif
}

/*
================================================================================
 * ~~ [ generators - xorshift8 ] ~~ *
--------------------------------------------------------------------------------
*/

/* NOTE Unlike the original GBA version of this code, we don't shave a few bytes
 * by sharing one global state between all generators. Consider this "standard".
 */
static u8 ae_global_xorshift8_state;

void ae_random_xorshift8_seed(const u8 seed)
{
    ae_global_xorshift8_state = seed; // init generator state
    ae_log(RANDOM, "%s = %02X", __FUNCTION__, (unsigned)seed);
}

u8 ae_random_xorshift8_ex(u8* state)
{
    *state ^= (*state << 7);
    *state ^= (*state >> 5);
    *state ^= (*state << 3);

    return *state;
}

u8 ae_random_xorshift8(void)
{
    return ae_random_xorshift8_ex(&ae_global_xorshift8_state);
}

/*
================================================================================
 * ~~ [ generators - xorshift16 ] ~~ *
--------------------------------------------------------------------------------
*/

static u16 ae_global_xorshift16_state;

void ae_random_xorshift16_seed(const u16 seed)
{
    ae_global_xorshift16_state = seed; // init generator state
    ae_log(RANDOM, "%s = %04X", __FUNCTION__, (unsigned)seed);
}

u16 ae_random_xorshift16_ex(u16* state)
{
    *state ^= (*state << 13);
    *state ^= (*state >>  9);
    *state ^= (*state <<  7);

    return *state;
}

u16 ae_random_xorshift16(void)
{
    return ae_random_xorshift16_ex(&ae_global_xorshift16_state);
}

/*
================================================================================
 * ~~ [ generators - xorshift32 ] ~~ *
--------------------------------------------------------------------------------
*/

/* fast, good, simple PRNG. fails statistical tests, but 9999x better than rand,
 * also about 10x faster in my testing than VS2008's 32-bit rand implementation.
 * 32-bit xorshift will never return zero, or repeat a number until state wraps.
 */
static u32 ae_global_xorshift32_state;

void ae_random_xorshift32_seed(const u32 seed)
{
    ae_global_xorshift32_state = seed; /* state */
    ae_log(RANDOM, "%s = %X", __FUNCTION__, seed);
}

u32 ae_random_xorshift32_ex(u32* state)
{
    *state ^= (*state << 13);
    *state ^= (*state >> 17);
    *state ^= (*state <<  5);

    return *state;
}

u32 ae_random_xorshift32(void)
{
    return ae_random_xorshift32_ex(&ae_global_xorshift32_state);
}

/*
================================================================================
 * ~~ [ generators - xorshift64 ] ~~ *
--------------------------------------------------------------------------------
*/

static u64 ae_global_xorshift64_state;

void ae_random_xorshift64_seed(const u32 L, const u32 R)
{
    ae_global_xorshift64_state = ((u64)L << 32) | (u64)R;
    ae_log(RANDOM, "%s = %X %X", __FUNCTION__, L, R);
}

u64 ae_random_xorshift64_ex(u64* state)
{
    *state ^= (*state >> 12);
    *state ^= (*state << 25);
    *state ^= (*state >> 27);

    return *state;
}

u64 ae_random_xorshift64(void)
{
    return ae_random_xorshift64_ex(&ae_global_xorshift64_state);
}

/*
================================================================================
 * ~~ [ generators - xorshift128 ] ~~ *
--------------------------------------------------------------------------------
*/

/* passes diehard tests, but fails the MatrixRank and LinearComp bigcrush tests.
 * this is about 10% slower than xorshift32 according to 32-bit VS benchmarking.
 */
static u32 ae_global_xorshift128_state[4];

void ae_random_xorshift128_seed(const u32 x, const u32 y,
                                const u32 z, const u32 w)
{
    ae_global_xorshift128_state[0] = x;
    ae_global_xorshift128_state[1] = y;
    ae_global_xorshift128_state[2] = z;
    ae_global_xorshift128_state[3] = w;

    ae_log(RANDOM, "%s = %X %X %X %X", __FUNCTION__, x, y, z, w);
}

u32 ae_random_xorshift128_ex(u32 state[4])
{
    u32 t = state[0];

    t ^= t << 11;
    t ^= t >>  8;

    state[0] = state[1];
    state[1] = state[2];
    state[2] = state[3];

    state[3] ^= state[3] >> 19;
    state[3] ^= t;

    return state[3];
}

u32 ae_random_xorshift128(void)
{
    return ae_random_xorshift128_ex(ae_global_xorshift128_state);
}

/*
================================================================================
 * ~~ [ array shuffling (permutation) ] ~~ *
--------------------------------------------------------------------------------
*/

void memshuffle(void* base, size_t count, size_t size)
{
    size_t i;

    for (i = 0; i < count; i++)
    {
        const size_t j = ae_random_bounded_u32(count);

        u8* a = (u8*)base + i * size;
        u8* b = (u8*)base + j * size;

        memswap(a, b, size);
    }
}

/*
================================================================================
 * ~~ [ float distributions ] ~~ *
--------------------------------------------------------------------------------
*/

float ae_random_erlang_flt(int order, float mean)
{
    float rvalue = ae_random_flt_oc();
    int p;

    if (order <= 1)
        order = 1;

    for (p = 1; p < order; p++) // decimate
    {
        rvalue *= ae_random_flt_oc();
    }

    return -mean * ae_logf(rvalue) / order;
}

double ae_random_erlang_dbl(int order, double mean)
{
    double rvalue = ae_random_dbl_oc();
    int p;

    if (order <= 1)
        order = 1;

    for (p = 1; p < order; p++) // decimate
    {
        rvalue *= ae_random_dbl_oc();
    }

    return -mean * log(rvalue) / order;
}

float ae_random_gauss_flt_ex(const float mu, const float sigma, float* next)
{
    float cur_z = *next;
    *next = 0.0f;

    if (cur_z == 0.0f)
    {
        float xtau = ae_random_flt_oc() * 2.0f * M_PI; // rad angle
        float grad = ae_sqrtf(-2.0f * ae_logf(ae_random_flt_oc()));

        cur_z = ae_cosf(xtau) * grad;
        *next = ae_sinf(xtau) * grad;
    }

    return mu + cur_z * sigma;
}

double ae_random_gauss_dbl_ex(const double mu, const double sigma, double* next)
{
    double cur_z = *next;
    *next = 0.0;

    if (cur_z == 0.0)
    {
        const double xtau = ae_random_dbl_oc() * 2.0 * M_PI; // rad
        const double grad = sqrt( -2.0 * log(ae_random_dbl_oc()) );

        cur_z = cos(xtau) * grad;
        *next = sin(xtau) * grad;
    }

    return mu + cur_z * sigma;
}

static float  __ae_gaussf_next;
static double __ae_gaussd_next;

float ae_random_gauss_flt(const float mu, const float sigma)
{
    return ae_random_gauss_flt_ex(mu, sigma, &__ae_gaussf_next);
}

double ae_random_gauss_dbl(const double mu, const double sigma)
{
    return ae_random_gauss_dbl_ex(mu, sigma, &__ae_gaussd_next);
}

/*
================================================================================
 * ~~ [ initialization ] ~~ *
--------------------------------------------------------------------------------
*/

/* TODO:
    - AE_RANDOM_TRACE
    - mersenne twister + tinymt version
    - SFMT (SIMD mersenne twister)
    - linear congruential generator
    - xorshift*, xorshift+
    - pcg32, pcg32f
    - hash standard library rand?
*/

void ae_random_init(int argc, char** argv)
{
    ae_random_xorshift32_seed(ae_random_gen_seed_u32());

    ae_random_xorshift8_seed (ae_random_xorshift32() & 0x000000FF);
    ae_random_xorshift16_seed(ae_random_xorshift32() & 0x0000FFFF);

    ae_random_xorshift64_seed(  ae_random_xorshift32(),
                                ae_random_xorshift32());

    ae_random_xorshift128_seed( ae_random_xorshift32(),
                                ae_random_xorshift32(),
                                ae_random_xorshift32(),
                                ae_random_xorshift32());
}

void ae_random_quit(void)
{
    // reset the gaussian random distribution to its original state
    __ae_gaussd_next = 0.0; __ae_gaussf_next = 0.0f;

    // dumb hack to log these values on program exit for debugging
    ae_random_xorshift128_seed( ae_global_xorshift128_state[0],
                                ae_global_xorshift128_state[1],
                                ae_global_xorshift128_state[2],
                                ae_global_xorshift128_state[3] );

    ae_random_xorshift64_seed(
        (u32)((ae_global_xorshift64_state & 0xFFFFFFFF00000000) >> 32),
        (u32)(ae_global_xorshift64_state & 0x00000000FFFFFFFF));

    ae_random_xorshift16_seed(ae_global_xorshift16_state);
    ae_random_xorshift8_seed(ae_global_xorshift8_state);

    ae_random_xorshift32_seed(ae_global_xorshift32_state);
}
