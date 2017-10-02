/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------

                               .,,uod8B8bou,,.
                      ..,uod8BBBBBBBBBBBBBBBBRPFT?l!i:.
                 ,=m8BBBBBBBBBBBBBBBRPFT?!||||||||||||||
                 !...:!TVBBBRPFT||||||||||!!^^""'   ||||
                 !.......:!?|||||!!^^""'            ||||
                 !.........||||                     ||||
                 !.........||||  ##                 ||||
                 !.........||||                     ||||
                 !.........||||                     ||||
                 !.........||||                     ||||
                 !.........||||                     ||||
                 `.........||||                    ,||||
                  .;.......||||               _.-!!|||||
           .,uodWBBBBb.....||||       _.-!!|||||||||!:'
        !YBBBBBBBBBBBBBBb..!|||:..-!!|||||||!iof68BBBBBb....
        !..YBBBBBBBBBBBBBBb!!||||||||!iof68BBBBBBRPFT?!::   `.
        !....YBBBBBBBBBBBBBBbaaitf68BBBBBBRPFT?!:::::::::     `.
        !......YBBBBBBBBBBBBBBBBBBBRPFT?!::::::;:!^"`;:::       `.
        !........YBBBBBBBBBBRPFT?!::::::::::^''...::::::;         iBBbo.
        `..........YBRPFT?!::::::::::::::::::::::::;iof68bo.      WBBBBbo.
          `..........:::::::::::::::::::::::;iof688888888888b.     `YBBBP^'
            `........::::::::::::::::;iof688888888888888888888b.     `
              `......:::::::::;iof688888888888888888888888888888b.
                `....:::;iof688888888888888888888888888888888899fT!
                  `..::!8888888888888888888888888888888899fT|!^"'
                    `' !!988888888888888888888888899fT|!^"'
                        `!!8888888888888888899fT|!^"'
                          `!988888888899fT|!^"'
                            `!9899fT|!^"'
                              `!^"'

                                    --------

    This is the home of the lowest level of code, that we generally expect to
    work everywhere and live forever. Things like math, containers, timing,
    profiling, debugging, the filesystem, multithreading and multiprocessing,
    mesh vertex and index processing, memory, and more are handled here.

                                    --------

    Anything game-specific or subject to major changes (such as code that calls
    audio or graphics APIs) is forbidden. External libraries must be optional.

                                    --------

    By default, source files are built into one translation unit (unity build).
    However, there are a few compile-time options for custom build scenarios:

    #define AE_STUB_FOO to replace extern calls in the foo subsystem with stubs,
        for easier porting or binary size reduction for older gaming consoles.

    #define AE_NOBUILD_FOO to prevent ae_foo.c from being compiled at all. This
        can be used to delegate the functionality of a subsystem to a library.

    #define AECORE_EXTENDED to add additional functionality to initialization.
        This is for cases where we're building a DLL, and we must call another
        initialization function in some order (ie. before aecore is started).

----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#define __AE_CORE_H__

/*
================================================================================
 * ~~ [ platform-specific includes ] ~~ *
--------------------------------------------------------------------------------
*/

#if defined(_WIN32)
    #ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS
    #endif
    #ifndef _CRT_RAND_S
    #define _CRT_RAND_S
    #endif
    #ifndef _USE_MATH_DEFINES
    #define _USE_MATH_DEFINES
    #endif
    #ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
    #endif
    #ifndef VC_EXTRALEAN
    #define VC_EXTRALEAN
    #endif
    #include <windows.h>

    // NOTE: since we only use the CRT memory management functions to implement
    // our own (raw malloc & free are forbidden) _CRTDBG_MAP_ALLOC is not used.
    #if _DEBUG
    #include <crtdbg.h>
    #endif

    #if defined(_M_IX86) || defined(_M_X64)
    #ifndef __MMX__
    #define __MMX__
    #endif
    #ifndef __SSE__
    #define __SSE__
    #endif
    #ifndef __SSE2__
    #define __SSE2__
    #endif
    #ifndef __SSE3__
    #define __SSE3__
    #endif
    #ifndef __SSSE3__
    #define __SSSE3__
    #endif
    #ifndef __SSE4_1__
    #define __SSE4_1__
    #endif
    #ifndef __SSE4_2__
    #define __SSE4_2__
    #endif
    #ifndef __AVX__
    #define __AVX__
    #endif
    #ifndef __AVX2__
    #define __AVX2__
    #endif
    #include <intrin.h>
    #endif
#else
    #include <dlfcn.h>
    #include <sys/utsname.h>
    #include <unistd.h>
    #include <x86intrin.h>
#endif

/*
================================================================================
 * ~~ [ c standard library includes ] ~~ *
--------------------------------------------------------------------------------
*/

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
================================================================================
 * ~~ [ c++ standard library includes ] ~~ *
--------------------------------------------------------------------------------
NOTE: no STL containers, typeinfo, streams, std::string, or C++11 & above here!
NOTE: disabling these for now, as their templates massively increase build time
--------------------------------------------------------------------------------
*/

#if defined(__cplusplus) && 0
    #include <algorithm>
    #include <bitset>
    #include <functional>
    #include <memory>
    #include <new>
    #include <numeric>
    #include <utility>
#endif

/*
================================================================================
 * ~~ [ aecore header file includes ] ~~ *
--------------------------------------------------------------------------------
*/

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif
#ifndef __AE_BRANCH_H__
#include <ae_branch.h>
#endif
#ifndef __AE_CPUINFO_H__
#include <ae_cpuinfo.h>
#endif
#ifndef __AE_EASING_H__
#include <ae_easing.h>
#endif
#ifndef __AE_ERROR_H__
#include <ae_error.h>
#endif
#ifndef __AE_FILE_H__
#include <ae_file.h>
#endif
#ifndef __AE_HASH_H__
#include <ae_hash.h>
#endif
#ifndef __AE_IMAGE_H__
#include <ae_image.h>
#endif
#ifndef __AE_INDEX_H__
#include <ae_index.h>
#endif
#ifndef __AE_LIBRARY_H__
#include <ae_library.h>
#endif
#ifndef __AE_LOGGER_H__
#include <ae_logger.h>
#endif
#ifndef __AE_MATH_H__
#include <ae_math.h>
#endif
#ifndef __AE_MEMORY_H__
#include <ae_memory.h>
#endif
#ifndef __AE_MINIFONT_H__
#include <ae_minifont.h>
#endif
#ifndef __AE_POLYGON_H__
#include <ae_polygon.h>
#endif
#ifndef __AE_RANDOM_H__
#include <ae_random.h>
#endif
#ifndef __AE_SEARCH_H__
#include <ae_search.h>
#endif
#ifndef __AE_SIMD_H__
#include <ae_simd.h>
#endif
#ifndef __AE_SORT_H__
#include <ae_sort.h>
#endif
#ifndef __AE_SWITCH_H__
#include <ae_switch.h>
#endif
#ifndef __AE_TIME_H__
#include <ae_time.h>
#endif
#ifndef __AE_TRIANGLE_H__
#include <ae_triangle.h>
#endif
#ifndef __AE_VERTEX_H__
#include <ae_vertex.h>
#endif

/*
================================================================================
 * ~~ [ main api ] ~~ *
--------------------------------------------------------------------------------
                            Command Line Arguments:

    -log-FOO
    -nolog-FOO

        Enables or disables writing messages in the logging category FOO.

    --log-all
    --log-none

        Enables or disables all logging categories, including warnings.

--------------------------------------------------------------------------------
*/

#ifdef __cplusplus
    extern "C" {
#endif

AE_DECL void AE_CALL ae_init(int argc, char** argv);
AE_DECL void AE_CALL ae_quit(void);

AE_DECL int AE_CALL ae_is_init(void);

typedef void (*ae_atexit_func_t)(void* context); // called at ae_quit
AE_DECL void AE_CALL ae_atexit(ae_atexit_func_t func, void* context);

static void ae_call_void_function(void* ptr)
{
    ((void(*)(void))ptr)(); // apply no arguments
}

static void ae_atexit_ex(void (*func)(void))
{
    ae_atexit(ae_call_void_function, (void*)func);
}

// NOTE avoiding linker issues when changing build configurations,
// in case somebody accidentally types this into a header file...
#undef AE_DECL

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_CORE_H__ */
