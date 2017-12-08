/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_ERROR_H__
#define __AE_ERROR_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_error_init(int argc, char** argv);
void ae_error_quit(void); // private init

// display a formatted error message to the user and terminate the process.
AE_DECL void AE_CALL ae_error(const char* fmt, ...) AE_PRINTF_ARGS(0, 1);

/*
================================================================================
 * ~~ [ debugger breakpoints ] ~~ *
--------------------------------------------------------------------------------
*/

#if defined(_MSC_VER)
    /*
     * NOTE we don't include intrin.h here, as it apparently contains c++ code.
     */
    extern void __cdecl __debugbreak(void);
    #define ae_breakpoint() __debugbreak()

#elif defined(__GNUC__)
    #include <signal.h>
    /*
     * NOTE __builtin_trap doesn't work with GDB for whatever reason, so we use
     *      this. __builtin_unreachable would help eliminate a bunch of static
     *      analysis warnings, but halts continued execution in the debugger...
     */
    #define ae_breakpoint() raise(SIGTRAP)

#else
    #define ae_breakpoint() *((int*)0) = 0xDEADFEED
#endif

/*
================================================================================
 * ~~ [ printf-style asserts ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL ae_assert_message( const char* conditional,
                                        const char* funcname,
                                        const char* filename,
                                        const int   lineno,
                                        const char* fmt, ...)
                                        AE_PRINTF_ARGS(4, 5);

/* TODO: test the conditional in a while loop so we can implement abort, retry,
 * break, & ignore (ae_assert_option_t enum return val from ae_assert_message).
 * see SDL_enabled_assert for an example of this kind of assert implementation.
 */
#if defined(AE_DEBUG)
    #define ae_assert(cnd, ...) do { if (!(cnd))            \
    {                                                       \
        ae_assert_message(AE_STRINGIFY(cnd), __FUNCTION__,  \
                        __FILE__, __LINE__, __VA_ARGS__ );  \
        ae_breakpoint();                                    \
    }                                                       \
    } while (AE_NULL_WHILE_LOOP_CONDITION)
#else
    #define ae_assert(cnd, ...) (void)sizeof(cnd)
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_ERROR_H__ */
