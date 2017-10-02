/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

int ae_branch_ex(int condition, const char* condition_string,
        const char* function, const char* filename, int line)
{
    AE_STUB(); return condition;
}

int ae_branch_report_enabled(void)
{
    /*AE_STUB();*/ return 0;
}

void ae_branch_report_clear_filter(AE_BRANCH_FILTER_FUNC filter, void* context)
{
    // AE_STUB();
}

void ae_branch_report_clear(void)
{
    // AE_STUB();
}

void ae_branch_report_clear_file(const char* filename)
{
    // AE_STUB();
}

void ae_branch_report_clear_function(const char* function)
{
    // AE_STUB();
}

void ae_branch_report_clear_extremes(const double epsilon)
{
    // AE_STUB();
}

void ae_branch_report_clear_median(const double epsilon)
{
    // AE_STUB();
}

void ae_branch_report_render_filter(AE_BRANCH_FILTER_FUNC filter, void* context,
            AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_branch_report_render(AE_BRANCH_RENDER_FUNC draw,
                ae_branch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_branch_report_render_file(const char* filename, // filter by file
    AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_branch_report_render_function(const char* function, // func filter
    AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_branch_report_render_extremes(const double epsilon, // outer ratios
    AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_branch_report_render_median(const double epsilon, // inner ratios
    AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_branch_report_print_filter(AE_BRANCH_FILTER_FUNC func, void* ctx,
                                ae_branch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_branch_report_print(ae_branch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_branch_report_print_file(const char* filename,
            ae_branch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_branch_report_print_function(const char* function,
                ae_branch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_branch_report_print_extremes(const double epsilon,
                ae_branch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_branch_report_print_median(const double epsilon,
                ae_branch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_branch_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_branch_quit(void)
{
    // AE_STUB();
}
