/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_LOGGER_H__
#define __AE_LOGGER_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_logger_init(int argc, char** argv);
void ae_logger_quit(void); // private init

// clear the terminal window and reset the cursor
AE_DECL void AE_CALL ae_log_clear_stdout(void);

/*
================================================================================
 * ~~ [ logging categories ] ~~ *
--------------------------------------------------------------------------------
*/

#define AE_LOG_CATEGORIES_N     \
                                \
    /* external libraries */    \
                                \
    N(AE_LOG_CATEGORY_PYTHON),  \
    N(AE_LOG_CATEGORY_LUA),     \
    N(AE_LOG_CATEGORY_RECAST),  \
    N(AE_LOG_CATEGORY_DETOUR),  \
    N(AE_LOG_CATEGORY_BULLET),  \
    N(AE_LOG_CATEGORY_GLFW),    \
    N(AE_LOG_CATEGORY_SDL),     \
    N(AE_LOG_CATEGORY_OPENGL),  \
    N(AE_LOG_CATEGORY_VULKAN),  \
    N(AE_LOG_CATEGORY_D3D),     \
    N(AE_LOG_CATEGORY_OPENAL),  \
                                \
    /* aecore categories */     \
                                \
    N(AE_LOG_CATEGORY_CPUINFO), \
    N(AE_LOG_CATEGORY_LIBRARY), \
    N(AE_LOG_CATEGORY_FILE),    \
    N(AE_LOG_CATEGORY_IMAGE),   \
    N(AE_LOG_CATEGORY_POLYGON), \
    N(AE_LOG_CATEGORY_RANDOM),  \
    N(AE_LOG_CATEGORY_TIME),    \
    N(AE_LOG_CATEGORY_STACK),   \
    N(AE_LOG_CATEGORY_HEAP),    \
    N(AE_LOG_CATEGORY_BLOCK),   \
    N(AE_LOG_CATEGORY_WARNING), \
    N(AE_LOG_CATEGORY_MISC),    \
    N(AE_LOG_CATEGORY_COUNT)

typedef enum ae_log_category_t
{
    #define N(x) x
    AE_LOG_CATEGORIES_N
    #undef N
} \
    ae_log_category_t;

static const char* ae_log_category_name[] =
{
    #define N(x) #x
    AE_LOG_CATEGORIES_N
    #undef N
};

/*
================================================================================
 * ~~ [ logging functions ] ~~ *
--------------------------------------------------------------------------------
TODO: it should be easy to add logging levels (needs a new interface though...)
TODO: move fatal warnings to the C file, to accommodate differing build configs
--------------------------------------------------------------------------------
*/

#define ae_log(c, ...) ae_log_ex(AE_LOG_CATEGORY_ ## c, __VA_ARGS__) // full enum

// log a message (append to log file, display in terminal window, ignore it, etc)
AE_DECL void AE_CALL ae_log_ex(ae_log_category_t category, const char* fmt, ...)
                                                            AE_PRINTF_ARGS(1, 2);

// log with a truncated string category, eg. ae_log_str("MISC", "Hello %s", name)
AE_DECL void AE_CALL ae_log_str(const char* category, const char* fmt, ...)
                                                            AE_PRINTF_ARGS(1, 2);

/* ===== [ category-specific helper functions ] ============================= */

#if defined(AE_FATAL_WARNINGS)

// issue a fatal warning regardless of the status of the log, for testing.
#define AE_WARN(...) ae_error(__VA_ARGS__)

#else

// issue a non-fatal warning (the warning category is enabled by default).
#define AE_WARN(...) ae_log(WARNING, __VA_ARGS__)

#endif

/* this warns the user about a function call that hasn't been implemented.
 */
#define AE_STUB() AE_WARN("stub function \"%s\" (file \"%s\" line %i)", \
                                        __FUNCTION__, __FILE__, __LINE__)

/* warning for a switch case that has yet to be implemented - this is designed
 * to work with enums declared in our style, which have automatically-defined
 * string tables for easy debugging, rather than printing integral enum values.
 *
 *      (ae_)   (image_format)  (_t)
 *
 *      /\      /\              /\
 *      prefix  name            suffix
 *
 *      type is the optional prefix and name joined together without the suffix.
 *      caps is the "caps name" for values in the enum, ie. AE_IMAGE_FORMAT.
 *      suffix mode (either a bare suffix or nosuffix identifier) appends a _t.
 */
#define AE_CASE_STUB(value, type, caps, suffix_mode)                        \
        ae_case_stub_ ## suffix_mode((value), type, caps)

#define ae_case_stub_suffix(value, type, caps)                              \
        ae_case_stub_ex((value), type, type ## _t, caps)

#define ae_case_stub_nosuffix(value, type, caps)                            \
        ae_case_stub_ex((value), type, type, caps)

#define ae_case_stub_ex(value, type, suffix_type, caps)                     \
                                                                            \
        /* NOTE: caps is passed to keep compatibility with ae_switch */     \
        AE_WARN("stub \"%s\" case %s (func \"%s\" file \"%s\" line %i)",    \
                                                                            \
                AE_STRINGIFY(suffix_type), type ## _name[(int) (value)],    \
                                (__FUNCTION__), (__FILE__), (__LINE__))

/* Indicates that a function is barely implemented or suboptimal in some way.
 * An example would be while porting memory allocation to a new platform,
 * implementing stack allocation and freeing as simple malloc / free calls.
 */
#define AE_TEMP() ae_log(MISC, "temp function \"%s\" (file \"%s\" line %i)", \
                                    (__FUNCTION__), (__FILE__), (__LINE__))

/*
================================================================================
 * ~~ [ logging control ] ~~ *
--------------------------------------------------------------------------------
*/

/* if writing the log to a file, synchronize the log's contents with the file.
 * this is automatically invoked on app shutdown, but may be needed elsewhere.
 */
AE_DECL void AE_CALL ae_log_flush(void);

AE_DECL void AE_CALL ae_log_enable(ae_log_category_t category, int enabled);
AE_DECL void AE_CALL ae_log_enable_str(const char* category, int enabled);

AE_DECL void AE_CALL ae_log_enable_all(int enabled);

AE_DECL int AE_CALL ae_log_is_enabled(ae_log_category_t category);
AE_DECL int AE_CALL ae_log_is_enabled_str(const char* category);

/* set formatted message handling and log flush callback functions. if either
 * function pointer is NULL, the default stdout print handler is re-installed.
 *
 * please take care not to call any functions in your handler that might log,
 * or disable all log categories on entry and restore values on function exit.
 */
AE_DECL void AE_CALL ae_log_handler(void (*log_func)(ae_log_category_t
                category, const char* msg), void (*flush_func)(void));

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_LOGGER_H__ */
