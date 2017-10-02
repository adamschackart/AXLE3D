/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CPUINFO_H__
#define __AE_CPUINFO_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_cpuinfo_init(int argc, char** argv);
void ae_cpuinfo_quit(void); // private init

// NOTE: endianness must always be detected at runtime, as many of the more
// exotic chips (powerpc, arm, alpha, sparc, mips, itanium) are bi-endian.
// Some embedded chips even allow endianness to be set on a per-page basis.
AE_STATIC_ASSERT(integer_union_size, sizeof(int) == sizeof(char[4]));

static c_inline int ae_cpuinfo_big_endian(void) { // microcontroller
    const union { int i; char c[4]; } one = { 1 }; return one.c[3]; }

static c_inline int ae_cpuinfo_lil_endian(void) { // modern pc chip
    const union { int i; char c[4]; } one = { 1 }; return one.c[0]; }

/*
================================================================================
 * ~~ [ platform info strings ] ~~ *
--------------------------------------------------------------------------------
*/

// get the human-readable name and version of the compiler used to build aecore
AE_DECL const char* AE_CALL ae_compiler_name(void);

// get the human-readable name of the language used to build aecore (C or C++)
AE_DECL const char* AE_CALL ae_language_name(void);

// get the human-readable build mode used to build aecore ("debug" or "release")
AE_DECL const char* AE_CALL ae_buildmode_name(void);

// get the human-readable name of the operating system ("Windows 7 SP1" etc.)
AE_DECL const char* AE_CALL ae_platform_name(void);

// get the human-readable cpu string (not shorthand "GenuineIntel" etc. strings)
AE_DECL const char* AE_CALL ae_cpuinfo_name(void);

/*
================================================================================
 * ~~ [ username and hostname ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL const char* AE_CALL ae_username(void); // name of the user: "Adam"
AE_DECL const char* AE_CALL ae_hostname(void); // net node name: "AdamsPC"

/*
================================================================================
 * ~~ [ cpu feature detection ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL int AE_CALL ae_cpuinfo_rdtsc(void);
AE_DECL int AE_CALL ae_cpuinfo_sse(void);
AE_DECL int AE_CALL ae_cpuinfo_sse2(void);
AE_DECL int AE_CALL ae_cpuinfo_sse3(void);
AE_DECL int AE_CALL ae_cpuinfo_sse41(void);
AE_DECL int AE_CALL ae_cpuinfo_sse42(void);
AE_DECL int AE_CALL ae_cpuinfo_avx(void);
AE_DECL int AE_CALL ae_cpuinfo_avx2(void);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_CPUINFO_H__ */
