/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

/*
================================================================================
 * ~~ [ implementation files ] ~~ *
--------------------------------------------------------------------------------
*/

#if !defined(AE_NOBUILD_BRANCH)
    #if defined(AE_STUB_BRANCH)
    #include <stub/ae_branch.c>
    #else
    #include <ae_branch.c>
    #endif
#endif

#if !defined(AE_NOBUILD_COLOR)
    #if defined(AE_STUB_COLOR)
    #include <stub/ae_color.c>
    #else
    #include <ae_color.c>
    #endif
#endif

#if !defined(AE_NOBUILD_CPUINFO)
    #if defined(AE_STUB_CPUINFO)
    #include <stub/ae_cpuinfo.c>
    #else
    #include <ae_cpuinfo.c>
    #endif
#endif

#if !defined(AE_NOBUILD_EASING)
    #if defined(AE_STUB_EASING)
    #include <stub/ae_easing.c>
    #else
    #include <ae_easing.c>
    #endif
#endif

#if !defined(AE_NOBUILD_ERROR)
    #if defined(AE_STUB_ERROR)
    #include <stub/ae_error.c>
    #else
    #include <ae_error.c>
    #endif
#endif

#if !defined(AE_NOBUILD_FILE)
    #if defined(AE_STUB_FILE)
    #include <stub/ae_file.c>
    #else
    #include <ae_file.c>
    #endif
#endif

#if !defined(AE_NOBUILD_HASH)
    #if defined(AE_STUB_HASH)
    #include <stub/ae_hash.c>
    #else
    #include <ae_hash.c>
    #endif
#endif

#if !defined(AE_NOBUILD_IMAGE)
    #if defined(AE_STUB_IMAGE)
    #include <stub/ae_image.c>
    #else
    #include <ae_image.c>
    #endif
#endif

#if !defined(AE_NOBUILD_INDEX)
    #if defined(AE_STUB_INDEX)
    #include <stub/ae_index.c>
    #else
    #include <ae_index.c>
    #endif
#endif

#if !defined(AE_NOBUILD_LIBRARY)
    #if defined(AE_STUB_LIBRARY)
    #include <stub/ae_library.c>
    #else
    #include <ae_library.c>
    #endif
#endif

#if !defined(AE_NOBUILD_LOGGER)
    #if defined(AE_STUB_LOGGER)
    #include <stub/ae_logger.c>
    #else
    #include <ae_logger.c>
    #endif
#endif

#if !defined(AE_NOBUILD_MATH)
    #if defined(AE_STUB_MATH)
    #include <stub/ae_math.c>
    #else
    #include <ae_math.c>
    #endif
#endif

#if !defined(AE_NOBUILD_MEMORY)
    #if defined(AE_STUB_MEMORY)
    #include <stub/ae_memory.c>
    #else
    #include <ae_memory.c>
    #endif
#endif

#if !defined(AE_NOBUILD_MINIFONT)
    #if defined(AE_STUB_MINIFONT)
    #include <stub/ae_minifont.c>
    #else
    #include <ae_minifont.c>
    #endif
#endif

#if !defined(AE_NOBUILD_POLYGON)
    #if defined(AE_STUB_POLYGON)
    #include <stub/ae_polygon.c>
    #else
    #include <ae_polygon.c>
    #endif
#endif

#if !defined(AE_NOBUILD_RANDOM)
    #if defined(AE_STUB_RANDOM)
    #include <stub/ae_random.c>
    #else
    #include <ae_random.c>
    #endif
#endif

#if !defined(AE_NOBUILD_SEARCH)
    #if defined(AE_STUB_SEARCH)
    #include <stub/ae_search.c>
    #else
    #include <ae_search.c>
    #endif
#endif

#if !defined(AE_NOBUILD_SORT)
    #if defined(AE_STUB_SORT)
    #include <stub/ae_sort.c>
    #else
    #include <ae_sort.c>
    #endif
#endif

#if !defined(AE_NOBUILD_SWITCH)
    #if defined(AE_STUB_SWITCH)
    #include <stub/ae_switch.c>
    #else
    #include <ae_switch.c>
    #endif
#endif

#if !defined(AE_NOBUILD_TIME)
    #if defined(AE_STUB_TIME)
    #include <stub/ae_time.c>
    #else
    #include <ae_time.c>
    #endif
#endif

#if !defined(AE_NOBUILD_TRIANGLE)
    #if defined(AE_STUB_TRIANGLE)
    #include <stub/ae_triangle.c>
    #else
    #include <ae_triangle.c>
    #endif
#endif

#if !defined(AE_NOBUILD_VERTEX)
    #if defined(AE_STUB_VERTEX)
    #include <stub/ae_vertex.c>
    #else
    #include <ae_vertex.c>
    #endif
#endif

/*
================================================================================
 * ~~ [ initialization ] ~~ *
--------------------------------------------------------------------------------
*/

#ifndef AE_ATEXIT_MAX_FUNCTIONS
#define AE_ATEXIT_MAX_FUNCTIONS 32
#endif

static ae_atexit_func_t ae_global_atexit_funcs[AE_ATEXIT_MAX_FUNCTIONS];
static void *           ae_global_atexit_param[AE_ATEXIT_MAX_FUNCTIONS];

void ae_atexit(ae_atexit_func_t function, void* context)
{
    size_t i;

    for (i = 0; i < AE_ATEXIT_MAX_FUNCTIONS; i++)
    {
        if (ae_global_atexit_funcs[i] == NULL)
        {
            ae_global_atexit_funcs[i] = function;
            ae_global_atexit_param[i] = context;

            return;
        }
    }

    ae_error("ae_atexit max func limit (%u) exceeded",
                        (u32)AE_ATEXIT_MAX_FUNCTIONS);
}

/* these allow us to hook in other init code. they may or may not be defined.
 * if they are, make sure to log in them so devs know this is taking place.
 */
void ae_pre_init (int argc, char** argv);
void ae_post_init(int argc, char** argv);

void ae_pre_quit (void);
void ae_post_quit(void);

static int aecore_is_initialized = 0; // XXX: refcount?
int ae_is_init(void) { return aecore_is_initialized; }

void ae_init(int argc, char** argv)
{
    if (!aecore_is_initialized)
    {
        const double init_time = ae_internal_seconds();

        /* TODO: this call can fail, so any cleanup work done by extensions
         * or in ae_atexit should probably not be a necessary operation.
         * maybe later we can get the warning call working before startup.
         */
        #if !defined(AE_NOQUIT_ATEXIT)
            atexit(ae_quit);
        #endif

        #if defined(AECORE_EXTENDED)
            ae_pre_init(argc, argv);
        #endif

        ae_error_init(argc, argv);
        ae_logger_init(argc, argv);
        ae_cpuinfo_init(argc, argv);
        ae_library_init(argc, argv);
        ae_math_init(argc, argv);
        ae_easing_init(argc, argv);
        ae_memory_init(argc, argv);
        ae_hash_init(argc, argv);
        ae_branch_init(argc, argv);
        ae_switch_init(argc, argv);
        ae_random_init(argc, argv);
        ae_search_init(argc, argv);
        ae_sort_init(argc, argv);
        ae_time_init(argc, argv);
        ae_file_init(argc, argv);
        ae_index_init(argc, argv);
        ae_vertex_init(argc, argv);
        ae_triangle_init(argc, argv);
        ae_color_init(argc, argv);
        ae_image_init(argc, argv);
        ae_minifont_init(argc, argv);
        ae_polygon_init(argc, argv);

        #if defined(AECORE_EXTENDED)
            ae_post_init(argc, argv);
        #endif

        ae_log( TIME, "ae_init done in %.2f milliseconds",
            (ae_internal_seconds() - init_time) * 1000.0);
    }

    aecore_is_initialized = 1;
}

void ae_quit(void)
{
    if (aecore_is_initialized)
    {
        const double quit_time = ae_internal_seconds();
        int i;

        /* run user exit functions before shutting anything else down, so
         * cleanup code has as much valid state to work with as possible.
         */
        for (i = AE_ATEXIT_MAX_FUNCTIONS - 1; i >= 0; i--)
        {
            if (ae_global_atexit_funcs[i])
            {
                ae_global_atexit_funcs[i](ae_global_atexit_param[i]);
                ae_global_atexit_funcs[i] = NULL;
            }
        }

        assert( memiszero(ae_global_atexit_funcs,
                sizeof(ae_global_atexit_funcs)));

        #if defined(AECORE_EXTENDED)
            ae_pre_quit();
        #endif

        ae_polygon_quit();
        ae_minifont_quit();
        ae_image_quit();
        ae_color_quit();
        ae_triangle_quit();
        ae_vertex_quit();
        ae_index_quit();
        ae_file_quit();
        ae_time_quit();
        ae_sort_quit();
        ae_search_quit();
        ae_random_quit();
        ae_switch_quit();
        ae_branch_quit();
        ae_hash_quit();
        ae_memory_quit();
        ae_easing_quit();
        ae_math_quit();
        ae_library_quit();
        ae_cpuinfo_quit();
        ae_logger_quit();
        ae_error_quit();

        #if defined(AECORE_EXTENDED)
            ae_post_quit();
        #endif

        ae_log( TIME, "ae_quit done in %.2f milliseconds",
            (ae_internal_seconds() - quit_time) * 1000.0);
    }

    aecore_is_initialized = 0;
}
