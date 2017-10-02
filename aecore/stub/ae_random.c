/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void ae_random_gen_seed(void* buffer, size_t size)
{
    AE_STUB();
}

u32 ae_random_gen_seed_u32(void)
{
    AE_STUB(); return 0;
}

u32 ae_random_u32(void)
{
    AE_STUB(); return 0;
}

void ae_random_xorshift8_seed(const u8 seed)
{
    AE_STUB();
}

u8 ae_random_xorshift8_ex(u8* state)
{
    AE_STUB(); return 0;
}

u8 ae_random_xorshift8(void)
{
    AE_STUB(); return 0;
}

void ae_random_xorshift16_seed(const u16 seed)
{
    AE_STUB();
}

u16 ae_random_xorshift16_ex(u16* state)
{
    AE_STUB(); return 0;
}

u16 ae_random_xorshift16(void)
{
    AE_STUB(); return 0;
}

void ae_random_xorshift32_seed(const u32 seed)
{
    AE_STUB();
}

u32 ae_random_xorshift32(void)
{
    AE_STUB(); return 0;
}

u32 ae_random_xorshift32_ex(u32* state)
{
    AE_STUB(); return 0;
}

void ae_random_xorshift64_seed(const u32 L, const u32 R)
{
    AE_STUB();
}

u64 ae_random_xorshift64_ex(u64* state)
{
    AE_STUB(); return 0;
}

u64 ae_random_xorshift64(void)
{
    AE_STUB(); return 0;
}

void ae_random_xorshift128_seed(const u32 x, const u32 y, const u32 z, const u32 w)
{
    AE_STUB();
}

u32 ae_random_xorshift128(void)
{
    AE_STUB(); return 0;
}

u32 ae_random_xorshift128_ex(u32 state[4])
{
    AE_STUB(); return 0;
}

void memshuffle(void* base, size_t count, size_t size)
{
    AE_STUB();
}

float ae_random_erlang_flt(int order, float mean)
{
    AE_STUB(); return 0.0f;
}

double ae_random_erlang_dbl(int order, double mean)
{
    AE_STUB(); return 0.0;
}

float ae_random_gauss_flt_ex(const float mu, const float sigma, float* next)
{
    AE_STUB(); return 0.0f;
}

double ae_random_gauss_dbl_ex(const double mu, const double sigma, double* next)
{
    AE_STUB(); return 0.0;
}

float ae_random_gauss_flt(const float mu, const float sigma)
{
    AE_STUB(); return 0.0f;
}

double ae_random_gauss_dbl(const double mu, const double sigma)
{
    AE_STUB(); return 0.0;
}

void ae_random_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_random_quit(void)
{
    // AE_STUB();
}
