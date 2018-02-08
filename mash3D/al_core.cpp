/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AL_CORE_H__
#include <al_core.h>
#endif

/*
================================================================================
 * ~~ [ subsystems ] ~~ *
--------------------------------------------------------------------------------
*/

extern void al_wrapper_init(void);
extern void al_wrapper_quit(void);

#if !defined(AL_NOBUILD_WRAPPER)
    #if defined(AL_STUB_WRAPPER)
    #include <stub/al_wrapper.c>
    #else
    #include <al_wrapper.cpp>
    #endif
#endif

extern void al_utility_init(void);
extern void al_utility_quit(void);

#if !defined(AL_NOBUILD_UTILITY)
    #if defined(AL_STUB_UTILITY)
    #include <stub/al_utility.c>
    #else
    #include <al_utility.cpp>
    #endif
#endif

extern void al_objects_init(void);
extern void al_objects_quit(void);

#if !defined(AL_NOBUILD_OBJECTS)
    #if defined(AL_STUB_OBJECTS)
    #include <stub/al_objects.c>
    #else
    #include <al_objects.cpp>
    #endif
#endif

/*
================================================================================
 * ~~ [ init & quit ] ~~ *
--------------------------------------------------------------------------------
*/

static int _al_core_is_initialized;

int al_is_init(void)
{
    return _al_core_is_initialized;
}

void al_init(void)
{
    if (!_al_core_is_initialized)
    {
        const double init_time = ae_internal_seconds();

        if (!ae_is_init())
        {
            AE_WARN("initialize aecore before al (command-line args ignored)");
            ae_init(0, NULL);
        }

        ae_atexit_ex(al_quit);

        // command-line arguments are already copied in aecore (call ae_argv())
        al_wrapper_init();
        al_utility_init();
        al_objects_init();

        ae_log( TIME, "al_init done in %.2f milliseconds",
            (ae_internal_seconds() - init_time) * 1000.0);
    }

    _al_core_is_initialized = 1;
}

void al_quit(void)
{
    if (_al_core_is_initialized)
    {
        const double quit_time = ae_seconds();

        // library startup and shutdown must always be stack-like
        ae_assert(ae_is_init(), "called al_quit after ae_quit");

        // NULL out function pointers, delete AL objects, check for leaks, etc.
        al_objects_quit();
        al_utility_quit();
        al_wrapper_quit();

        ae_log(TIME, "al_quit done in %.2f milliseconds",
                    (ae_seconds() - quit_time) * 1000.0);
    }

    _al_core_is_initialized = 0;
}
