/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __GL_CORE_H__
#include <gl_core.h>
#endif

/*
================================================================================
 * ~~ [ subsystems ] ~~ *
--------------------------------------------------------------------------------
*/

extern void gl_wrapper_init(void);
extern void gl_wrapper_quit(void);

#if !defined(GL_NOBUILD_WRAPPER)
    #if defined(GL_STUB_WRAPPER)
    #include <stub/gl_wrapper.c>
    #else
    #include <gl_wrapper.cpp>
    #endif
#endif

extern void gl_utility_init(void);
extern void gl_utility_quit(void);

#if !defined(GL_NOBUILD_UTILITY)
    #if defined(GL_STUB_UTILITY)
    #include <stub/gl_utility.c>
    #else
    #include <gl_utility.cpp>
    #endif
#endif

extern void gl_objects_init(void);
extern void gl_objects_quit(void);

#if !defined(GL_NOBUILD_OBJECTS)
    #if defined(GL_STUB_OBJECTS)
    #include <stub/gl_objects.c>
    #else
    #include <gl_objects.cpp>
    #endif
#endif

/*
================================================================================
 * ~~ [ init & quit ] ~~ *
--------------------------------------------------------------------------------
*/

static int _gl_core_is_initialized;

int gl_is_init(void)
{
    return _gl_core_is_initialized;
}

void gl_init(void)
{
    if (!_gl_core_is_initialized)
    {
        const double init_time = ae_internal_seconds();

        if (!ae_is_init())
        {
            AE_WARN("initialize aecore before gl (command-line args ignored)");
            ae_init(0, NULL);
        }

        ae_atexit_ex(gl_quit);

        // command-line arguments are already copied in aecore (call ae_argv())
        gl_wrapper_init();
        gl_utility_init();
        gl_objects_init();

        ae_log( TIME, "gl_init done in %.2f milliseconds",
            (ae_internal_seconds() - init_time) * 1000.0);
    }

    _gl_core_is_initialized = 1;
}

void gl_quit(void)
{
    if (_gl_core_is_initialized)
    {
        const double quit_time = ae_seconds();

        // library startup and shutdown must always be stack-like
        ae_assert(ae_is_init(), "called gl_quit after ae_quit");

        // NULL out function pointers, delete GL objects, check for leaks, etc.
        gl_objects_quit();
        gl_utility_quit();
        gl_wrapper_quit();

        ae_log(TIME, "gl_quit done in %.2f milliseconds",
                    (ae_seconds() - quit_time) * 1000.0);
    }

    _gl_core_is_initialized = 0;
}
