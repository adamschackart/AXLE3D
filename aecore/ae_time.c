/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
--- TODO: void ae_sleep(double seconds); will actually be tough to implement!!!
--- TODO: ae_datetime: fill struct with ints - year, month, day, hour, min, sec
--- TODO: ae_current_united_states_holiday and ae_current_international_holiday
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

/*
================================================================================
 * ~~ [ timer ] ~~ *
--------------------------------------------------------------------------------
*/

#if defined(_WIN32)

// this differs from ae_seconds in that it doesn't require init, and fails gracefully
double ae_internal_seconds(void)
{
    LARGE_INTEGER freq, perf;

    if (QueryPerformanceFrequency(&freq) && QueryPerformanceCounter(&perf))
    {
        return (double)perf.QuadPart / (double)freq.QuadPart;
    }
    else
    {
        return 0.0;
    }
}

static LARGE_INTEGER ae_performance_frequency;
static LARGE_INTEGER ae_initial_perf_counter;

double ae_seconds(void)
{
    // by this point, we've cached all possible system information (for maximum perf)
    LARGE_INTEGER performance_counter;

    ae_assert( ae_performance_frequency.QuadPart, "call ae_init before ae_seconds!");
    QueryPerformanceCounter(&performance_counter);

    return (double)(performance_counter.QuadPart - ae_initial_perf_counter.QuadPart) \
                                        / (double)ae_performance_frequency.QuadPart;
}

static void ae_time_counter_init(int argc, char** argv)
{
    if (!QueryPerformanceFrequency( &ae_performance_frequency ))
        ae_error("failed to query system performance frequency");

    if( !QueryPerformanceCounter( &ae_initial_perf_counter ) )
        ae_error("failed to query system performance counter");

    ae_log(TIME, "performance frequency is %u counts per second",
                        (u32)ae_performance_frequency.QuadPart);
}

static void ae_time_counter_quit(void) {}

#elif defined(__APPLE__)

// TODO: github.com/ThomasHabets/monotonic_clock/blob/master/src/monotonic_mach.c
#error TODO high-precision timer using mach_timebase_info and mach_absolute_time

#elif defined(unix) || defined(__unix__) || defined(__unix)

/*
XXX This may not be available on older systems (it's part of the
    POSIX 2008 standard) or non-GNU standard builds (-std=cXXX).
*/
#if defined(CLOCK_MONOTONIC_RAW)
    #define AE_MONOTONIC_CLOCK CLOCK_MONOTONIC_RAW
#else
    #define AE_MONOTONIC_CLOCK CLOCK_MONOTONIC
#endif

double ae_internal_seconds(void)
{
    struct timespec ts; // safer for timing init

    memset(&ts, 0, sizeof(ts)); // zero out
    clock_gettime(AE_MONOTONIC_CLOCK, &ts);

    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

double ae_seconds(void)
{
    struct timespec ts; // TODO: gettimeofday fallback?
    clock_gettime(AE_MONOTONIC_CLOCK, &ts);

    // TODO: offset by the initial time (like in win32)
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

static void ae_time_counter_init(int argc, char** argv)
{
    struct timespec time_spec;

    if (clock_gettime(AE_MONOTONIC_CLOCK, &time_spec))
        ae_error("failed to create a monotonic timer");
}

static void ae_time_counter_quit(void) {}

#else

static void ae_time_counter_init(int argc, char** argv) {}
static void ae_time_counter_quit(void) {}

double ae_seconds(void) { AE_STUB(); return 0.0; }
double ae_internal_seconds(void) { return 0.0; }

#endif

/* ===== [ frame timer ] ==================================================== */

typedef struct ae_frame_callback_data_t
{
    char name[128];
    ae_frame_callback_t function;
    void * context;
} \
    ae_frame_callback_data_t;

static void ae_frame_callback_init(int argc, char** argv) {}
static ae_frame_callback_data_t ae_frame_callbacks[ 128 ];

static void ae_frame_callback_quit(void)
{
    memset(ae_frame_callbacks, 0, sizeof(ae_frame_callbacks));
}

static void ae_frame_callback_update(double dt)
{
    AE_PROFILE_ENTER();

    size_t i = 0, n = AE_ARRAY_COUNT(ae_frame_callbacks);
    for (; i < n; i++)
    {
        ae_frame_callback_data_t* data = ae_frame_callbacks + i;

        if (data->function)
        {
            data->function(data->name, dt, data->context);
        }
    }

    AE_PROFILE_LEAVE();
}

void
ae_frame_callback_register(const char* name, ae_frame_callback_t func, void* ctx)
{
    size_t i = 0, n = AE_ARRAY_COUNT(ae_frame_callbacks);
    for (; i < n; i++)
    {
        ae_frame_callback_data_t* data = ae_frame_callbacks + i;

        if (!strncmp(data->name, name, sizeof(data->name) - 1))
        {
            ae_frame_callback_unregister(name);
        }

        if (data->function == NULL)
        {
            ae_log(TIME, "registered frame callback \"%s\"", name);

            strncpy(data->name, name, sizeof(data->name) - 1);
            data->function = func;
            data->context = ctx;

            return;  // found our slot, no need to keep iterating
        }
    }

    ae_error("failed to register frame callback \"%s\"!", name);
}

void
ae_frame_callback_unregister(const char* name)
{
    size_t i = 0, n = AE_ARRAY_COUNT(ae_frame_callbacks); // safe O(n)
    for (; i < n; i++)
    {
        ae_frame_callback_data_t* data = ae_frame_callbacks + i;

        if (!strncmp(data->name, name, sizeof(data->name) - 1))
        {
            ae_log(TIME, "unregistered frame callback \"%s\"", name);

            data->name[0] = '\0';
            data->function = NULL;
            data->context = NULL;

            return;  // found our slot, no need to keep iterating
        }
    }
}

void
ae_frame_callback_get(const char* name, ae_frame_callback_t* func, void** ctx)
{
    size_t i = 0, n = AE_ARRAY_COUNT(ae_frame_callbacks); // safe O(n)

    if (func) *func = NULL;
    if (ctx ) *ctx  = NULL;

    for (; i < n; i++)
    {
        ae_frame_callback_data_t* data = ae_frame_callbacks + i;

        if (!strncmp(data->name, name, sizeof(data->name) - 1))
        {
            if (func) *func = data->function;
            if (ctx ) *ctx  = data->context;

            return;  // found our slot, no need to keep iterating
        }
    }
}

typedef struct ae_timer_callback_data_t
{
    char name[128];
    ae_timer_callback_t function;
    void * context;

    double current;
    double seconds;

    int repeat;
} \
    ae_timer_callback_data_t;

static void ae_timer_callback_init(int argc, char** argv) {}
static ae_timer_callback_data_t ae_timer_callbacks[ 128 ];

static void ae_timer_callback_quit(void)
{
    memset(ae_timer_callbacks, 0, sizeof(ae_timer_callbacks));
}

static void ae_timer_callback_update(double dt)
{
    AE_PROFILE_ENTER();

    size_t i = 0, n = AE_ARRAY_COUNT(ae_timer_callbacks);
    for (; i < n; i++)
    {
        ae_timer_callback_data_t* data = ae_timer_callbacks + i;

        ae_if (data->function)
        {
            data->current += dt;

            ae_if (data->current >= data->seconds) // timer fired - wrap or unregister
            {
                data->function(data->name, data->seconds, data->repeat, data->context);

                ae_if (data->repeat)
                {
                    data->current -= data->seconds;
                }
                else
                {
                    ae_timer_callback_unregister(data->name);
                }
            }
        }
    }

    AE_PROFILE_LEAVE();
}

void ae_timer_callback_register(const char* name, ae_timer_callback_t func,
                                double seconds, int repeat, void* context)
{
    size_t i = 0, n = AE_ARRAY_COUNT(ae_timer_callbacks);
    for (; i < n; i++)
    {
        ae_timer_callback_data_t* data = ae_timer_callbacks + i;

        if (!strncmp(data->name, name, sizeof(data->name) - 1))
        {
            ae_timer_callback_unregister(name);
        }

        if (data->function == NULL)
        {
            ae_log(TIME, "registered timer callback \"%s\"", name);

            strncpy(data->name, name, sizeof(data->name) - 1);
            data->function = func;
            data->context = context;
            data->current = 0.0;
            data->seconds = seconds;
            data->repeat = repeat;

            return; // found our slot, no need to keep iterating
        }
    }

    ae_error("failed to register timer callback \"%s\"!", name);
}

void ae_timer_callback_unregister(const char* name)
{
    size_t i = 0, n = AE_ARRAY_COUNT(ae_timer_callbacks); // safe O(n)
    for (; i < n; i++)
    {
        ae_timer_callback_data_t* data = ae_timer_callbacks + i;

        if (!strncmp(data->name, name, sizeof(data->name) - 1))
        {
            ae_log(TIME, "unregistered timer callback \"%s\"", name);

            data->name[0] = '\0';
            data->function = NULL;
            data->context = NULL;
            data->current = 0.0;
            data->seconds = 0.0;
            data->repeat = 0;

            return; // found our slot, no need to keep iterating
        }
    }
}

void ae_timer_callback_get(const char* name, ae_timer_callback_t* function,
                            double* seconds, int* repeat, void ** context)
{
    if (function) *function = NULL;
    if (seconds) *seconds = 0.0;
    if (repeat) *repeat = 0;
    if (context) *context = NULL;

    size_t i = 0, n = AE_ARRAY_COUNT(ae_timer_callbacks);
    for (; i < n; i++)
    {
        ae_timer_callback_data_t* data = ae_timer_callbacks + i;

        if (!strncmp(data->name, name, sizeof(data->name) - 1))
        {
            if (function) *function = data->function;
            if (seconds) *seconds = data->seconds;
            if (repeat) *repeat = data->repeat;
            if (context) *context = data->context;

            return; // found our slot, no need to keep iterating
        }
    }
}

static double ae_previous_frame_time;

double ae_frame_delta(void)
{
    const double current = ae_seconds(); // seconds elapsed
    const double delta_t = current - ae_previous_frame_time;

    ae_previous_frame_time = current;

    ae_timer_callback_update(delta_t);
    ae_frame_callback_update(delta_t);

    return delta_t;
}

/*
================================================================================
 * ~~ [ profiler ] ~~ *
--------------------------------------------------------------------------------
TODO: each thread needs to have its own profiler (this isn't threadsafe at all)
TODO: we should track each function/block's minimum and maximum execution times
TODO: could handle C++ overloading easier if we took line numbers (big change!)
TODO: track depths of functions in the profiler callstack (min, max, & average)
--------------------------------------------------------------------------------
*/

#if defined(AE_PROFILE_ENABLE)

static ae_strmap_t ae_global_profile; // string function name -> profile node.
static ae_memory_chunk_t ae_profile_node_chunk; // profile node memory blocks.

typedef struct ae_profile_context_t
{
    // file and function names. these may or may not be dynamically allocated,
    // so to be safe we still allocate and copy them into a normal string map.
    const char* filename;
    const char* funcname;

    // used for computing the time delta taken up by the function's execution.
    double start_seconds;
} \
    ae_profile_context_t;

static void ae_profile_free_nodes(void)
{
    int i = 0;

    char* k;
    void* v;

    for ( ; i < ae_global_profile.limit ? // iterate profile
            k = ae_global_profile.table[i].key,
            v = ae_global_profile.table[i].val, 1 : 0; ++i )
    {
        if (k == NULL || k == (char*)1) {} else
        {
            ae_block_free(&ae_profile_node_chunk, v);
        }
    }
}

/* ===== [ public API ] ===================================================== */

void* ae_profile_enter(const char* filename, const char* funcname)
{
    /* NOTE we can always use the block allocator here if we encounter issues,
     * or make another stack just for the profiler so nothing else interferes.
     */
    ae_profile_context_t* context = (ae_profile_context_t*)
        ae_stack_malloc(ae_global_stack(), sizeof(ae_profile_context_t));

    /* TODO: Modern compilers can fold const expressions like strlen(__FILE__).
     * Use this in our macro to take a faster string hash and allocation path.
     * We also don't need to store these, we should take args in profile_leave.
     */
    context->filename = filename;
    context->funcname = funcname;

    /* TODO: should we call rdtsc after this? we have to measure its cost on
     * a bunch of different platforms to make sure it isn't altering results.
     */
    context->start_seconds = ae_seconds();

    return (void*)context;
}

void ae_profile_leave(void* _context)
{
    ae_profile_context_t * context = (ae_profile_context_t *)_context;
    double functime = ae_seconds() - context->start_seconds; // get dt

    char * funcname = (char *)context->funcname;
    char * filename = (char *)context->filename;

    // get func info if it's been called before, otherwise create a new node.
    u32 funchash = ae_hash_str(funcname);

    ae_profile_node_t* node = (ae_profile_node_t*)ae_strmap_get_ex(
                            &ae_global_profile, funcname, funchash);
    if (node == NULL)
    {
        // fringe case, but this will leak if the table has no implementation
        ae_assert(ae_global_profile.table, "hash table code is stubbed out");

        node = (ae_profile_node_t*)ae_block_malloc(&ae_profile_node_chunk);
        /*
         * NOTE: we could save time, memory, and a little safety here by just
         * assigning the strmap key (funcname) to a char* instead of copying.
         * however, we can safely copy and store nodes indefinitely this way.
         */
        ae_assert(strlen(funcname) < sizeof(node->funcname), "%s", funcname);
        strncpy(node->funcname, funcname, sizeof(node->funcname)-1);

        ae_assert(strlen(filename) < sizeof(node->filename), "%s", filename);
        strncpy(node->filename, filename, sizeof(node->filename)-1);

        node->first_call = context->start_seconds;
        node->time_taken = 0;
        node->call_count = 0;

        if (!ae_strmap_add_ex(&ae_global_profile, funcname, node, funchash))
        {
            ae_error("failed to add profile node (is table code stubbed?");
        }
    }

    ae_assert(!strcmp(filename, node->filename), "file collision for func \"%s"
        "\": expected \"%s\", got \"%s\"", funcname, node->filename, filename);

    node->time_taken += functime;
    node->call_count += 1;

    ae_stack_free(ae_global_stack(), _context, sizeof(ae_profile_context_t));
}

int ae_profile_enabled(void)
{
    return 1;
}

void ae_profile_clear(void)
{
    ae_profile_free_nodes(); // reset profile

    ae_strmap_free(&ae_global_profile);
    ae_strmap_init(&ae_global_profile, 1024);
}

/* ===== [ rendering ] ====================================================== */

static int ae_profile_sort_total_time(const void* a, const void* b)
{
    const double a_time = (*(ae_profile_node_t**)a)->time_taken;
    const double b_time = (*(ae_profile_node_t**)b)->time_taken;

    if (a_time < b_time) return +1;
    if (a_time > b_time) return -1;

    return 0;
}

static int ae_profile_sort_average_time(const void* a, const void* b)
{
    const double a_time = (*(ae_profile_node_t**)a)->time_taken /
                ( double )(*(ae_profile_node_t**)a)->call_count;

    const double b_time = (*(ae_profile_node_t**)b)->time_taken /
                ( double )(*(ae_profile_node_t**)b)->call_count;

    if (a_time < b_time) return +1;
    if (a_time > b_time) return -1;

    return ae_profile_sort_total_time(a, b);
}

static int ae_profile_sort_call_count(const void* a, const void* b)
{
    const size_t a_call = (*(ae_profile_node_t**)a)->call_count;
    const size_t b_call = (*(ae_profile_node_t**)b)->call_count;

    if (a_call < b_call) return +1;
    if (a_call > b_call) return -1;

    return ae_profile_sort_total_time(a, b);
}

static int ae_profile_sort_funcname(const void* a, const void* b)
{
    const char* a_name = (*(ae_profile_node_t**)a)->funcname;
    const char* b_name = (*(ae_profile_node_t**)b)->funcname;

    return strcmp(a_name, b_name);
}

static int ae_profile_sort_filename(const void* a, const void* b)
{
    const char* a_name = (*(ae_profile_node_t**)a)->filename;
    const char* b_name = (*(ae_profile_node_t**)b)->filename;

    const int name_cmp = strcmp(a_name, b_name);

    if (name_cmp < 0) return -1;
    if (name_cmp > 0) return +1;

    return ae_profile_sort_funcname(a, b);
}

static int ae_profile_sort_first_call(const void* a, const void* b)
{
    const double a_time = (*(ae_profile_node_t**)a)->first_call;
    const double b_time = (*(ae_profile_node_t**)b)->first_call;

    if (a_time < b_time) return -1;
    if (a_time > b_time) return +1;

    return 0;
}

void ae_profile_render( AE_PROFILE_RENDER_FUNC draw, ae_profile_sort_t sort,
                        size_t max_items, double dt) // abstract sort & draw
{
    int (*compare)(const void*, const void*) = NULL;
    ae_profile_node_t** items;

    items = (ae_profile_node_t**)ae_stack_malloc(ae_global_stack(),
            ae_global_profile.count * sizeof(ae_profile_node_t *));
    {
        int i = 0, j = 0;

        char* k;
        void* v;

        for ( ; i < ae_global_profile.limit ? // iterate profile
                k = ae_global_profile.table[i].key,
                v = ae_global_profile.table[i].val, 1 : 0; ++i )
        {
            if (k == NULL || k == (char*)1) {} else
            {
                items[j++] = (ae_profile_node_t*)v;
            }
        }

        assert(ae_global_profile.count == j);
    }

    switch (sort)
    {
        case AE_PROFILE_SORT_TOTAL_TIME:
            compare = ae_profile_sort_total_time; break;

        case AE_PROFILE_SORT_AVERAGE_TIME:
            compare = ae_profile_sort_average_time; break;

        case AE_PROFILE_SORT_CALL_COUNT:
            compare = ae_profile_sort_call_count; break;

        case AE_PROFILE_SORT_FUNCNAME:
            compare = ae_profile_sort_funcname; break;

        case AE_PROFILE_SORT_FILENAME:
            compare = ae_profile_sort_filename; break;

        case AE_PROFILE_SORT_FIRST_CALL:
            compare = ae_profile_sort_first_call; break;

        default: assert(0); break;
    }

    qsort(items, ae_global_profile.count, sizeof(ae_profile_node_t*), compare);
    {
        int i = 0; // iterate the sorted profile nodes and render them
        for (; i < ae_global_profile.count; i++)
        {
            // TODO: profile "render" func should take a context param
            if ((size_t)i == max_items) break; else draw(items[i]);
        }
    }

    ae_stack_free(ae_global_stack(), items, ae_global_profile.count *
                                        sizeof(ae_profile_node_t *));
}

static void ae_profile_node_print(ae_profile_node_t* node)
{
    printf("%s\n\t(time %f, avg %f, calls %u, file \"%s\")\n", node->funcname, node->time_taken,
            node->time_taken / (double)node->call_count, (u32)node->call_count, node->filename);
}

void ae_profile_print(ae_profile_sort_t sort, size_t max_items, double dt)
{
    int i;

    for(i = 0; i < 80; ++i) printf("-"); printf("\n");

    printf("\t%s (dt %f, fps %f)\n", // show framerate
            ae_profile_sort_name[sort], dt, 1.0 / dt);

    for(i = 0; i < 80; ++i) printf("-"); printf("\n");

    ae_profile_render(ae_profile_node_print, sort, max_items, dt);
}

/* ===== [ initialization ] ================================================= */

static void ae_time_profile_init(int argc, char** argv)
{
    if (!ae_cpuinfo_rdtsc())
    {
        ae_error("processor doesn't support the timestamp counter instruction");
    }

    ae_chunk_create(&ae_profile_node_chunk, "profile_node",
                    sizeof(ae_profile_node_t), 1024);

    ae_profile_clear();
}

static void ae_time_profile_quit(void)
{
    ae_profile_free_nodes(); // free the nodes
    ae_strmap_free(&ae_global_profile);

    ae_chunk_destroy(&ae_profile_node_chunk);
}

#else

static void ae_time_profile_init(int argc, char** argv) {} // profile disabled
static void ae_time_profile_quit(void) {}

void* ae_profile_enter(const char* file_n, const char* func_n) { return NULL; }
void ae_profile_leave(void* ctx) {}

int ae_profile_enabled(void) { return 0; }
void ae_profile_clear(void) {}

void ae_profile_render( AE_PROFILE_RENDER_FUNC draw , ae_profile_sort_t sort,
                                            size_t max_items, double dt) {}

void ae_profile_print(ae_profile_sort_t sort, size_t max_items, double dt) {}

#endif

/*
================================================================================
 * ~~ [ initialization ] ~~ *
--------------------------------------------------------------------------------
*/

void ae_time_init(int argc, char** argv)
{
    if (ae_log_is_enabled(AE_LOG_CATEGORY_TIME))
    {
        struct tm* time_info;
        time_t raw_time;
        char init_date[1024];

        time(&raw_time); // convert time into the proper format for strftime
        time_info = localtime(&raw_time);

        strftime(init_date, sizeof(init_date), "%b %d %Y at %X", time_info);
        if (init_date[4] == '0') init_date[4] = ' '; // HACK standin C89 %e

        ae_log(TIME, "aecore build date is %s at %s", __DATE__, __TIME__);
        ae_log(TIME, "aecore start date is %s", init_date); // build & init
    }

    ae_time_counter_init(argc, argv);
    ae_time_profile_init(argc, argv);

    /* prevent big first frame delta if ae_seconds begins with a large value.
     * this still doesn't prevent library initialization from spiking deltas.
     */
    ae_previous_frame_time = ae_seconds();

    ae_frame_callback_init(argc, argv);
    ae_timer_callback_init(argc, argv);
}

void ae_time_quit(void)
{
    if (ae_log_is_enabled(AE_LOG_CATEGORY_TIME))
    {
        struct tm* time_info;
        time_t raw_time;
        char quit_date[1024];

        time(&raw_time); // convert time into the proper format for strftime
        time_info = localtime(&raw_time);

        strftime(quit_date, sizeof(quit_date), "%b %d %Y at %X", time_info);
        if (quit_date[4] == '0') quit_date[4] = ' '; // HACK standin C89 %e

        ae_log(TIME, "aecore end date is %s", quit_date);
    }

    ae_timer_callback_quit();
    ae_frame_callback_quit();

    ae_time_profile_quit();
    ae_time_counter_quit();
}
