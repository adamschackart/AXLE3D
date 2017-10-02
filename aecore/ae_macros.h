/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_MACROS_H__
#define __AE_MACROS_H__

#ifdef __cplusplus
    extern "C" {
#endif

// this allows us to convert the result of macro expansion to a string literal.
#define _AE_STR_IMPL(s) #s
#define AE_STRINGIFY(s) _AE_STR_IMPL(s)

// check a condition at compile time (and fail to build if condition is false).
#define AE_STATIC_ASSERT(msg, cnd) typedef int _assert_ ## msg[(cnd) ? 1 : -1]

/*
================================================================================
 * ~~ [ ugly function decoration ] ~~ *
--------------------------------------------------------------------------------
*/

// the export macro allows our symbols to be visible when linking with a lib.
// the import macro enables a more efficient calling convention when linking.
#if defined(_WIN32)
    #define AE_EXPORT __declspec(dllexport)
    #define AE_IMPORT __declspec(dllimport)
#else
    #define AE_EXPORT extern
    #define AE_IMPORT extern
#endif

// don't specify DLL linkage when building an app (avoid tons of warning C4273).
#if defined(AE_COMPILING_EXE)
    #define AE_DECL
    #define AE_CALL
#endif

#if !defined(AE_DECL)
    #if defined(AE_COMPILING_DLL)
        #define AE_DECL AE_EXPORT
    #else
        #define AE_DECL AE_IMPORT
    #endif
#endif

// we generally use cdecl in this lib, as we define variable argument functions.
// fastcall benchmarks show no difference, and it doesn't matter on x64 anyways.
#if !defined(AE_CALL)
    #if (defined(__WIN32__) || defined(__WINRT__)) && !defined(__GNUC__)
        #define AE_CALL __cdecl
    #else
        #define AE_CALL
    #endif
#endif

// this is for printf-like functions that take a format string and variable args.
// F is the index of the format string param, and A is the index of the "...".
#if defined(__GNUC__)
    #define AE_PRINTF_ARGS(F, A) __attribute__((format (printf, F + 1, A + 1)))
#else
    #define AE_PRINTF_ARGS(F, A)
#endif

// NOTE: __inline works on all compilers i've tested, for both C and C++ builds.
// this just lets us turn inline hints off, to see what happens in the profiler.
#if !defined(c_inline)
    #if defined(AE_NO_INLINING)
        #define c_inline
    #elif defined(__cplusplus)
        #define c_inline inline
    #else
        #define c_inline __inline
    #endif
#endif

/*
================================================================================
 * ~~ [ various extra miscellany ] ~~ *
--------------------------------------------------------------------------------
*/

#ifdef _MSC_VER
//  #define AE_NULL_WHILE_LOOP_CONDITION (-1 == __LINE__)
    #define AE_NULL_WHILE_LOOP_CONDITION (0,0)
#else
    #define AE_NULL_WHILE_LOOP_CONDITION (0) /* for /W4 */
#endif

#if defined(__GNUC__)
    #define AE_DECL_ALIGN(s) __attribute__((aligned(s)))
#elif defined(_WIN32)
    #define AE_DECL_ALIGN(s) __declspec(align(s))
#else
    #error no alignment macro defined for this platform
#endif

#define AE_ALIGN16 AE_DECL_ALIGN(16)
#define AE_ALIGN32 AE_DECL_ALIGN(32)
#define AE_ALIGN64 AE_DECL_ALIGN(64)

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_MACROS_H__ */
