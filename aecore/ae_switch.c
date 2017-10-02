/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

#if !defined(AE_SWITCH_COVERAGE)
    #include <stub/ae_switch.c>
#else

static ae_strmap_t ae_global_switch_coverage;

int ae_switch_ex(int e_value, const char* type_name, const char** value_names,
        int enum_count, const char* function, const char* filename, int line)
{
    ae_switch_node_t * node;
    char key_string[ 4096 ];

    ae_assert(e_value < enum_count, "%i >= %i (func \"%s\", file \"%s\", line %i)",
                                    e_value, enum_count, function, filename, line);

    if (ae_global_switch_coverage.table == NULL)
    {   /*
        AE_WARN("\"%s\" func \"%s\" file \"%s\" line %i before init or after quit",
                type_name, function, filename, line);
        */
        return e_value;
    }

    // Split data in hashtable key by an illegal identifier - the ascii delete char.
    // This also has the added benefit of printing as a space (at least on win32).
    // TODO common code for ae_switch and ae_branch that builds this string faster.

    if (AE_SNPRINTF(key_string, "%s\xFF%s\xFF%s\xFF%i",
            type_name, function, filename, line) < 0)
    {
        ae_error("ENUM \"%s\" FUNC \"%s\" FILE \"%s\" LINE %i TOO BIG!",
                                    type_name, function, filename, line);
    }

    node = (ae_switch_node_t*)ae_strmap_get(&ae_global_switch_coverage, key_string);

    if (node == NULL)
    {
        node = ae_create(ae_switch_node_t, clear);

        node->names = value_names;
        node->type = type_name;
        node->function = function;
        node->filename = filename;
        node->line = line;
        node->first = ae_seconds();
        node->count = enum_count;
        node->taken = (size_t*)ae_calloc(node->count, sizeof(size_t));

        ae_strmap_set(&ae_global_switch_coverage, key_string, node);
    }

    // TODO: overflow checks
    node->total++;
    node->taken[e_value]++;

    return e_value;
}

int ae_switch_report_enabled(void)
{
    return 1;
}

static int sc_filter_anything(ae_switch_node_t* node, void* data)
{
    return 1;
}

static int sc_filter_function(ae_switch_node_t* node, void* data)
{
    return strcmp(node->function, (const char*)data) == 0;
}

static int sc_filter_filename(ae_switch_node_t* node, void* data)
{
    return strstr(node->filename, (const char*)data) != NULL;
}

static int sc_filter_typename(ae_switch_node_t* node, void* data)
{
    return strcmp(node->type, (const char*)data) == 0;
}

void ae_switch_report_clear_filter(AE_SWITCH_FILTER_FUNC filter, void* context)
{
    char* k;
    void* v;

    int i;

    // TODO: because we're using a hashtable key removal call that potentially
    // reallocs the table, we restart the loop each time we free a key in order
    // to avoid touching potentially invalid memory. This is obviously O(N^2).
    begin:

    for (i = 0; i < ae_global_switch_coverage.limit ? // iterate switches
                k = ae_global_switch_coverage.table[i].key,
                v = ae_global_switch_coverage.table[i].val, 1 : 0; ++i)
    {
        if (k == NULL || k == (char*)1) {} else
        {
            ae_switch_node_t* node = (ae_switch_node_t*)v;

            if (filter(node, context))
            {
                ae_free(node->taken);
                ae_free(v);
                ae_strmap_remove(&ae_global_switch_coverage, k, NULL);

                goto begin;
            }
        }
    }
}

void ae_switch_report_clear(void)
{
    ae_switch_report_clear_filter(sc_filter_anything, NULL);
}

void ae_switch_report_clear_function(const char* function)
{
    ae_switch_report_clear_filter(sc_filter_function, (void*)function);
}

void ae_switch_report_clear_file(const char* filename)
{
    ae_switch_report_clear_filter(sc_filter_filename, (void*)filename);
}

void ae_switch_report_clear_type(const char* type_name)
{
    ae_switch_report_clear_filter(sc_filter_typename, (void*)type_name);
}

static int sc_sort_compare_total(const void* a, const void* b)
{
    ae_switch_node_t* a_node = *(ae_switch_node_t**)a;
    ae_switch_node_t* b_node = *(ae_switch_node_t**)b;

    const size_t a_total = a_node->total;
    const size_t b_total = b_node->total;

    if (a_total < b_total) return +1;
    if (a_total > b_total) return -1;

    // the same total count might mean the same file and function,
    // so do a cheap hack thing and break the tie by line number.

    if (a_node->line < b_node->line) return -1;
    if (a_node->line > b_node->line) return +1;

    return 0;
}

static size_t sc_count_cases_covered(const ae_switch_node_t* const node)
{
    size_t i = 0, j = 0;

    for (; i < node->count; i++)
        if (node->taken[i]) j++;

    return j;
}

static int sc_sort_compare_spread(const void* a, const void* b)
{
    ae_switch_node_t* a_node = *(ae_switch_node_t**)a;
    ae_switch_node_t* b_node = *(ae_switch_node_t**)b;

    const size_t a_total = sc_count_cases_covered(a_node);
    const size_t b_total = sc_count_cases_covered(b_node);

    if (a_total < b_total) return +1;
    if (a_total > b_total) return -1;

    return sc_sort_compare_total(a, b);
}

static int sc_sort_compare_typename(const void* a, const void* b)
{
    ae_switch_node_t* a_node = *(ae_switch_node_t**)a;
    ae_switch_node_t* b_node = *(ae_switch_node_t**)b;

    return strcmp(a_node->type, b_node->type);
}

static int sc_sort_compare_function(const void* a, const void* b)
{
    ae_switch_node_t* a_node = *(ae_switch_node_t**)a;
    ae_switch_node_t* b_node = *(ae_switch_node_t**)b;

    // TODO: fall back to line compare in case of tie
    return strcmp(a_node->function, b_node->function);
}

static int sc_sort_compare_filename(const void* a, const void* b)
{
    ae_switch_node_t* a_node = *(ae_switch_node_t**)a;
    ae_switch_node_t* b_node = *(ae_switch_node_t**)b;

    // TODO: fall back to func compare in case of tie
    return strcmp(a_node->filename, b_node->filename);
}

static int sc_sort_compare_first_test(const void* a, const void* b)
{
    ae_switch_node_t* a_node = *(ae_switch_node_t**)a;
    ae_switch_node_t* b_node = *(ae_switch_node_t**)b;

    if (a_node->first < b_node->first) return -1;
    if (a_node->first > b_node->first) return +1;

    return 0;
}

static int sc_sort_compare_last_test(const void* a, const void* b)
{
    return -sc_sort_compare_first_test(a, b);
}

void ae_switch_report_render_filter(AE_SWITCH_FILTER_FUNC filter, void* context,
            AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items)
{
    int (*compare)(const void*, const void*) = NULL;
    int i = 0, j = 0;

    char* k;
    void* v;

    const size_t bytes = ae_global_switch_coverage.count * sizeof(ae_switch_node_t*);
    ae_switch_node_t** items;

    items = (ae_switch_node_t**)ae_stack_malloc(ae_global_stack(), bytes);

    for ( ; i < ae_global_switch_coverage.limit ? // fill ptr array
            k = ae_global_switch_coverage.table[i].key,
            v = ae_global_switch_coverage.table[i].val, 1 : 0; ++i)
    {
        if (k == NULL || k == (char*)1) {} else
        {
            items[j++] = (ae_switch_node_t*)v;
        }
    }

    assert(ae_global_switch_coverage.count == j);

    switch (sort)
    {
        case AE_SWITCH_SORT_TOTAL:
            compare = sc_sort_compare_total; break;

        case AE_SWITCH_SORT_SPREAD:
            compare = sc_sort_compare_spread; break;

        case AE_SWITCH_SORT_TYPENAME:
            compare = sc_sort_compare_typename; break;

        case AE_SWITCH_SORT_FUNCTION:
            compare = sc_sort_compare_function; break;

        case AE_SWITCH_SORT_FILENAME:
            compare = sc_sort_compare_filename; break;

        case AE_SWITCH_SORT_FIRST_TEST:
            compare = sc_sort_compare_first_test; break;

        case AE_SWITCH_SORT_LAST_TEST:
            compare = sc_sort_compare_last_test; break;

        default: assert(0); break;
    }

    qsort(items, ae_global_switch_coverage.count, sizeof(ae_switch_node_t*), compare);

    for (i = 0; i < ae_imin(ae_global_switch_coverage.count, max_items); i++)
    {
        if (filter(items[i], context)) draw(items[i]);
    }

    ae_stack_free(ae_global_stack(), items, bytes);
}

void ae_switch_report_render(AE_SWITCH_RENDER_FUNC draw,
                ae_switch_sort_t sort, size_t max_items)
{
    ae_switch_report_render_filter(sc_filter_anything, NULL, draw, sort, max_items);
}

void ae_switch_report_render_function(const char* function, // func filter
    AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items)
{
    ae_switch_report_render_filter(sc_filter_function, (void*)function, draw, sort, max_items);
}

void ae_switch_report_render_file(const char* filename, // filter by file
    AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items)
{
    ae_switch_report_render_filter(sc_filter_filename, (void*)filename, draw, sort, max_items);
}

void ae_switch_report_render_type(const char* type_name, // filter by type
    AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items)
{
    ae_switch_report_render_filter(sc_filter_typename, (void*)type_name, draw, sort, max_items);
}

static void sc_print_node(ae_switch_node_t* node)
{
    size_t i;

    printf("type \"%s\" in func \"%s\"\n", node->type, node->function);
    printf("\t(file \"%s\", line %i)\n", node->filename, node->line);
    printf("\ttotal = %u\n", (u32)node->total);

    for (i = 0; i < node->count; i++)
    {
        if (node->taken[i])
        {
            printf("\t%s = %u\n", node->names[i], (u32)node->taken[i]);
        }
    }

    printf("\n");
}

void ae_switch_report_print_filter(AE_SWITCH_FILTER_FUNC func, void* ctx,
                                ae_switch_sort_t sort, size_t max_items)
{
    // TODO: print an info banner first, similar to the one in profiler print
    ae_switch_report_render_filter(func, ctx, sc_print_node, sort, max_items);
}

void ae_switch_report_print(ae_switch_sort_t sort, size_t max_items)
{
    ae_switch_report_print_filter(sc_filter_anything, NULL, sort, max_items);
}

void ae_switch_report_print_function(const char* function_s,
                    ae_switch_sort_t sort, size_t max_items)
{
    ae_switch_report_print_filter(sc_filter_function, (void*)function_s, sort, max_items);
}

void ae_switch_report_print_file(const char* filename_s,
                ae_switch_sort_t sort, size_t max_items)
{
    ae_switch_report_print_filter(sc_filter_filename, (void*)filename_s, sort, max_items);
}

void ae_switch_report_print_type(const char* typename_s,
                ae_switch_sort_t sort, size_t max_items)
{
    ae_switch_report_print_filter(sc_filter_typename, (void*)typename_s, sort, max_items);
}

void ae_switch_init(int argc, char** argv)
{
    ae_strmap_init(&ae_global_switch_coverage, 1024);
}

void ae_switch_quit(void)
{
    ae_switch_report_clear();

    ae_strmap_free(&ae_global_switch_coverage); // set table to initial state
    memset(&ae_global_switch_coverage, 0, sizeof(ae_global_switch_coverage));
}

#endif
