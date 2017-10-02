/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_BRANCH_H__
#define __AE_BRANCH_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_branch_init(int argc, char** argv);
void ae_branch_quit(void); // private init

/*
================================================================================
 * ~~ [ coverage tracker syntax ] ~~ *
--------------------------------------------------------------------------------
XXX use this internally with great caution. ae_init might not be called yet, or
~~~ the thing you're marking up with ae_if might be used to implement ae_branch.

XXX this cute little hack is not thread safe, or designed with speed in mind!!!
--------------------------------------------------------------------------------
*/

AE_DECL int AE_CALL ae_branch_ex(int conditional, const char* conditional_string,
                            const char* function, const char* filename, int lin);

#if defined(AE_BRANCH_COVERAGE)
    #define ae_branch(cnd) \
            ae_branch_ex((cnd), AE_STRINGIFY(cnd), __FUNCTION__, __FILE__, __LINE__)

    #define ae_unlikely ae_branch
    #define ae_likely ae_branch

    #define ae_if(cnd) if (ae_branch(cnd))
#else
    #define ae_branch(cnd) (cnd)

    #if defined(__GNUC__)
        #define ae_unlikely(x) __builtin_expect(!!(x), 0)
        #define ae_likely(x)   __builtin_expect(!!(x), 1)
    #else
        #define ae_unlikely(cnd) (cnd)
        #define ae_likely(cnd) (cnd)
    #endif

    #define ae_if if
#endif

/*
================================================================================
 * ~~ [ global coverage report ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL int AE_CALL ae_branch_report_enabled(void);

typedef struct ae_branch_node_t
{
    const char* conditional;
    const char* function;
    const char* filename;
    int line;

    double first;
    size_t total;
    size_t taken;
} \
    ae_branch_node_t;

typedef int (*AE_BRANCH_FILTER_FUNC)(ae_branch_node_t* node, void* data);

AE_DECL void AE_CALL ae_branch_report_clear_filter(AE_BRANCH_FILTER_FUNC, void*);
AE_DECL void AE_CALL ae_branch_report_clear(void);

AE_DECL void AE_CALL ae_branch_report_clear_file(const char* filename);
AE_DECL void AE_CALL ae_branch_report_clear_function(const char* func);

#define ae_branch_report_clear_this_function() \
        ae_branch_report_clear_function(__FUNCTION__)

#define ae_branch_report_clear_this_file() \
        ae_branch_report_clear_file(__FILE__)

AE_DECL void AE_CALL ae_branch_report_clear_extremes(const double epsilon);
AE_DECL void AE_CALL ae_branch_report_clear_median(const double epsilon);

typedef void (*AE_BRANCH_RENDER_FUNC)(ae_branch_node_t* node);

#define AE_BRANCH_SORT_N            \
                                    \
    /* sorting by count */          \
    N(AE_BRANCH_SORT_TOTAL),        \
    N(AE_BRANCH_SORT_TRUE_COUNT),   \
    N(AE_BRANCH_SORT_FALSE_COUNT),  \
                                    \
    /* sorting by margins */        \
    N(AE_BRANCH_SORT_TRUE_RATIO),   \
    N(AE_BRANCH_SORT_FALSE_RATIO),  \
    N(AE_BRANCH_SORT_HALFWAYNESS),  \
                                    \
    /* lexical sorting */           \
    N(AE_BRANCH_SORT_CONDITIONAL),  \
    N(AE_BRANCH_SORT_FUNCTION),     \
    N(AE_BRANCH_SORT_FILENAME),     \
                                    \
    /* chronological sorting */     \
    N(AE_BRANCH_SORT_FIRST_TEST),   \
    N(AE_BRANCH_SORT_LAST_TEST),    \
                                    \
    N(AE_BRANCH_SORT_COUNT)

typedef enum ae_branch_sort_t
{
    #define N(x) x
    AE_BRANCH_SORT_N
    #undef N
} \
    ae_branch_sort_t;

static const char* ae_branch_sort_name[] =
{
    #define N(x) #x
    AE_BRANCH_SORT_N
    #undef N
};

/* ===== [ abstract iteration ] ============================================= */

AE_DECL void AE_CALL
ae_branch_report_render_filter(AE_BRANCH_FILTER_FUNC func, void* context,
    AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items);

AE_DECL void AE_CALL ae_branch_report_render( // Display all branches.
    AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items);

/* Display all branches within any path that contains the given filename.
 */
AE_DECL void AE_CALL ae_branch_report_render_file(const char * filename,
    AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items);

/* Display all branches within a function. Very useful for printf debugging.
 */
AE_DECL void AE_CALL ae_branch_report_render_function(const char * func,
    AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items);

#define ae_branch_report_render_this_function(draw, sort, max_items) \
        ae_branch_report_render_function(__FUNCTION__, (draw), (sort), (max_items))

#define ae_branch_report_render_this_file(draw, sort, max_items) \
        ae_branch_report_render_file(__FILE__, (draw), (sort), (max_items))

/* Display "easy" branches. Shows vals in [0, epsilon] and [100 - epsilon, 100].
 * 1.0 is a good margin here for deciding where to put ae_likely or ae_unlikely.
 */
AE_DECL void AE_CALL ae_branch_report_render_extremes(const double epsilon,
    AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items);

/* Display "hard" branches. Shows values within [epsilon%, 100% - epsilon].
 * 1.0 is a good margin here (knocks out most loop counters and safety checks).
 */
AE_DECL void AE_CALL ae_branch_report_render_median(const double epsilon,
    AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items);

/* ===== [ stdout reporting ] =============================================== */

AE_DECL void AE_CALL
ae_branch_report_print_filter(AE_BRANCH_FILTER_FUNC func, void* context,
                                ae_branch_sort_t sort, size_t max_items);

AE_DECL void AE_CALL ae_branch_report_print(
    ae_branch_sort_t sort, size_t max_items);

AE_DECL void AE_CALL ae_branch_report_print_file(const char* filename_s,
                                ae_branch_sort_t sort, size_t max_items);

AE_DECL void AE_CALL ae_branch_report_print_function(const char* func_s,
                                ae_branch_sort_t sort, size_t max_items);

#define ae_branch_report_print_this_function(sort, max_items) \
        ae_branch_report_print_function(__FUNCTION__, (sort), (max_items))

#define ae_branch_report_print_this_file(sort, max_items) \
        ae_branch_report_print_file(__FILE__, (sort), (max_items))

AE_DECL void AE_CALL ae_branch_report_print_extremes(const double epsilon,
                                ae_branch_sort_t sort, size_t max_items);

AE_DECL void AE_CALL ae_branch_report_print_median(const double epsilon,
                                ae_branch_sort_t sort, size_t max_items);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_BRANCH_H__ */
