/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_TIME_H__
#define __AE_TIME_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_time_init(int argc, char** argv);
void ae_time_quit(void); // private init

/*
================================================================================
 * ~~ [ timer ] ~~ *
--------------------------------------------------------------------------------
*/

// get the number of seconds elapsed since some arbitrary epoch. to compute
// time elapsed between points (time delta), use ae_seconds() - start_time.
AE_DECL double AE_CALL ae_seconds(void);

// internal-only clock for timing init (slower to call, but fails gracefully).
AE_DECL double AE_CALL ae_internal_seconds(void);

// get the processor timestamp counter. has the same signature everywhere.
#define ae_rdtsc() ((u64)__rdtsc())

/* generate a temporal sine wave with a given period and useless initial offset.
 * this provides an easy way to create flashing animations (texture color etc.)
 */
static c_inline double ae_time_wave_ex(double time, double rate, double offset)
{
    // NOTE: this function was hacked out in a graphing calculator, and seemed
    // correct, but apparently this is necessary to make rate scale properly...
    rate = 1.0 / rate;

    return -cos(time * ((2.0 * M_PI) / rate) - offset) * 0.5 + 0.5;
}

static c_inline double ae_time_wave(double rate)
{
    return ae_time_wave_ex(ae_seconds(), rate, 0.0);
}

/* ===== [ frame timer ] ==================================================== */

// a function to run every frame - only one function can be registered per name.
typedef void (*ae_frame_callback_t)(const char* name, double dt, void* context);

// allow alternate old-style func naming convention
typedef ae_frame_callback_t AE_FRAME_CALLBACK_FUNC;

AE_DECL void AE_CALL
ae_frame_callback_register(const char* name, ae_frame_callback_t fn, void* ctx);

AE_DECL void AE_CALL
ae_frame_callback_unregister(const char* name);

AE_DECL int AE_CALL
ae_frame_callback_get(const char* name, ae_frame_callback_t* fn, void** ctx);

/* function called after a set amount of time - unregisters if repeat is false.
 * otherwise, repeat reports the number of times the callback has been called.
 * t is the actual amount of time elapsed before the timer function was called.
 */
typedef void (*ae_timer_callback_t)(const char * name, double t,
                                    int repeats, void* context);

// allow alternate old-style func naming convention
typedef ae_timer_callback_t AE_TIMER_CALLBACK_FUNC;

AE_DECL void AE_CALL ae_timer_callback_register(const char* name, ae_timer_callback_t func,
                                                double seconds, int repeat, void* context);

AE_DECL void AE_CALL ae_timer_callback_unregister(const char* name);

AE_DECL int AE_CALL ae_timer_callback_get( const char* name, ae_timer_callback_t* function,
                            double* current, double* seconds, int* repeat, void** context);

// reserve these easier short names for user convenience
#define ae_timer_register   ae_timer_callback_register
#define ae_timer_unregister ae_timer_callback_unregister
#define ae_timer_get        ae_timer_callback_get

/* Tick the global game timer and run all scheduled frame callback functions.
 * Note that the time delta is not capped (necessary for smooth physics etc).
 */
AE_DECL double AE_CALL ae_frame_delta(void);

/*
================================================================================
 * ~~ [ profiler ] ~~ *
--------------------------------------------------------------------------------
~ NOTE: The original idea behind this was that we would just accumulate timing
        information every frame (hence the vestigial dt argument in render),
        display it, and clear the state of the profiler before the next frame.
        While that works just fine, there are other ways to use the profiler.

        Engines can display long frame reports for individual frames that don't
        hit some target dt, as well as adding profile nodes to some hash table
        each frame to show sum total times taken across the run of a program.

        Engines can also filter file and function names to find performance data
        relevant to a specific subsystem, or print reports detailing execution
        during a loading screen (especially when combined with branch tracking).
--------------------------------------------------------------------------------
*/

// place these at the beginning and ending of funcs to track performance metrics
#define AE_PROFILE_ENTER() void* _ae_profile_context = \
                ae_profile_enter(__FILE__, __FUNCTION__)

#define AE_PROFILE_LEAVE() ae_profile_leave(_ae_profile_context)

AE_DECL void* AE_CALL ae_profile_enter(const char* file_n, const char* func_n);
AE_DECL void  AE_CALL ae_profile_leave(void* ctx); // custom section profiling

AE_DECL int AE_CALL ae_profile_enabled(void); // get profiler status at runtime
AE_DECL void AE_CALL ae_profile_clear(void); // call at the end of each frame

/* ===== [ c++ macro ] ====================================================== */

#if defined(__cplusplus)
    /*
        This leaves the profile block automatically at scope exit. Note that on
        many C++ compilers, the constructor is always executed at scope entry,
        not where you actually declare the object (use funcs for custom blocks).
    */
    #define AE_PROFILE_SCOPE() \
        aeProfileScope _ae_profile_context(__FILE__, __FUNCTION__)

    #define AE_PROFILE_NAMED(func_name) \
        aeProfileScope _ae_profile_context(__FILE__, #func_name)

    struct aeProfileScope
    {
        void* ctx;

        aeProfileScope(const char* filename, const char* funcname)
        {
            ctx = ae_profile_enter(filename, funcname);
        }

        ~aeProfileScope()
        {
            ae_profile_leave(ctx);
        }
    };
#endif

/* ===== [ rendering ] ====================================================== */

typedef struct ae_profile_node_t
{
    char funcname[96];
    char filename[96];

    double time_taken;
    double first_call;
    size_t call_count;
} \
    ae_profile_node_t;

/* TODO: user data pointer, node filter callback for this and ae_profile_clear
 * (see the implementation of the branch coverage tracker for a good example).
 */
typedef void (*AE_PROFILE_RENDER_FUNC)(ae_profile_node_t* node);

#define AE_PROFILE_SORT_N \
    N(AE_PROFILE_SORT_TOTAL_TIME), \
    N(AE_PROFILE_SORT_AVERAGE_TIME), \
    N(AE_PROFILE_SORT_CALL_COUNT), \
    N(AE_PROFILE_SORT_FUNCNAME), \
    N(AE_PROFILE_SORT_FILENAME), \
    N(AE_PROFILE_SORT_FIRST_CALL), \
    N(AE_PROFILE_SORT_COUNT)

typedef enum ae_profile_sort_t
{
    #define N(x) x
    AE_PROFILE_SORT_N
    #undef N
} \
    ae_profile_sort_t;

static const char* ae_profile_sort_name[] =
{
    #define N(x) #x
    AE_PROFILE_SORT_N
    #undef N
};

/* display the profile, sorted by some criteria. takes the frame's delta.
 * this can also be used simply to gather all sorted profile information.
 */
AE_DECL void AE_CALL ae_profile_render( AE_PROFILE_RENDER_FUNC draw ,
                ae_profile_sort_t sort, size_t max_items, double dt);

AE_DECL void AE_CALL ae_profile_print(ae_profile_sort_t sort,
                                size_t max_items, double dt);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_TIME_H__ */
