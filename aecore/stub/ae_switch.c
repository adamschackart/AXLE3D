/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

int ae_switch_ex(int e_value, const char* type_name, const char** value_names,
        int enum_count, const char* function, const char* filename, int line)
{
    AE_STUB(); return e_value;
}

int ae_switch_report_enabled(void)
{
    /*AE_STUB();*/ return 0;
}

void ae_switch_report_clear_filter(AE_SWITCH_FILTER_FUNC filter, void* context)
{
    // AE_STUB();
}

void ae_switch_report_clear(void)
{
    // AE_STUB();
}

void ae_switch_report_clear_function(const char* function)
{
    // AE_STUB();
}

void ae_switch_report_clear_file(const char* filename)
{
    // AE_STUB();
}

void ae_switch_report_clear_type(const char* type_name)
{
    // AE_STUB();
}

void ae_switch_report_render_filter(AE_SWITCH_FILTER_FUNC filter, void* context,
            AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_switch_report_render(AE_SWITCH_RENDER_FUNC draw,
                ae_switch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_switch_report_render_function(const char* function, // func filter
    AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_switch_report_render_file(const char* filename, // filter by file
    AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_switch_report_render_type(const char* type_name, // filter by type
    AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_switch_report_print_filter(AE_SWITCH_FILTER_FUNC func, void* ctx,
                                ae_switch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_switch_report_print(ae_switch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_switch_report_print_function(const char* function_s,
                    ae_switch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_switch_report_print_file(const char* filename_s,
                ae_switch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_switch_report_print_type(const char* typename_s,
                ae_switch_sort_t sort, size_t max_items)
{
    // AE_STUB();
}

void ae_switch_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_switch_quit(void)
{
    // AE_STUB();
}
