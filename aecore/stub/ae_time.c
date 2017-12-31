/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

double ae_internal_seconds(void)
{
    return 0.0; // don't stub this! the logger might not be initialized yet!
}

double ae_seconds(void)
{
    AE_STUB(); return 0.0;
}

void
ae_frame_callback_register(const char* name, ae_frame_callback_t func, void* ctx)
{
    AE_STUB();
}

void
ae_frame_callback_unregister(const char* name)
{
    AE_STUB();
}

int
ae_frame_callback_get(const char* name, ae_frame_callback_t* func, void** ctx)
{
    AE_STUB(); return 0;
}

void ae_timer_callback_register(const char* name, ae_timer_callback_t func,
                                double seconds, int repeat, void* context)
{
    AE_STUB();
}

void ae_timer_callback_unregister(const char* name)
{
    AE_STUB();
}

int ae_timer_callback_get( const char* name, ae_timer_callback_t* function,
            double* current, double* seconds, int* repeat, void** context)
{
    AE_STUB(); return 0;
}

void ae_timer_callback_set_repeat(const char* name, int repeat)
{
    AE_STUB();
}

double ae_frame_delta(void)
{
    AE_STUB(); return 0.0;
}

void ae_seconds_to_display(double t, int* hours, int* minutes, int* seconds)
{
    AE_STUB();
}

double ae_display_to_seconds(int hours, int minutes, int seconds)
{
    AE_STUB(); return 0.0;
}

const char* ae_seconds_to_string(double t, int show_hours)
{
    AE_STUB(); return "";
}

double ae_string_to_seconds(const char* string)
{
    AE_STUB(); return 0.0;
}

void* ae_profile_enter(const char* file_n, const char* func_n)
{
    /*AE_STUB();*/ return NULL;
}

void ae_profile_leave(void* ctx)
{
    /*AE_STUB();*/
}

int ae_profile_enabled(void)
{
    /*AE_STUB();*/ return 0;
}

void ae_profile_clear(void)
{
    AE_STUB();
}

void ae_profile_render(AE_PROFILE_RENDER_FUNC render, ae_profile_sort_t sort,
                                                size_t max_items, double dt)
{
    AE_STUB();
}

void ae_profile_print(ae_profile_sort_t sort, size_t max_items, double dt)
{
    AE_STUB();
}

void ae_time_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_time_quit(void)
{
    // AE_STUB();
}
