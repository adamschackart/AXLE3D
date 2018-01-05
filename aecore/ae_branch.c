/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
--- TODO: function to write total report (all categories) to a file in home dir
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

#if !defined(AE_BRANCH_COVERAGE)
    #include <stub/ae_branch.c>
#else

static ae_strmap_t ae_global_branch_coverage;

int ae_branch_ex(int condition, const char* condition_string,
        const char* function, const char* filename, int line)
{
    ae_branch_node_t * node;
    char key_string[ 4096 ];

    if (ae_global_branch_coverage.table == NULL)
    {   /*
        AE_WARN("\"%s\" func \"%s\" file \"%s\" line %i before init or after quit",
                condition_string, function, filename, line);
        */
        return condition;
    }

    // Split data in hashtable key by an illegal identifier - the ascii delete char.
    // This also has the added benefit of printing as a space (at least on win32).
    // TODO common code for ae_switch and ae_branch that builds this string faster.

    if (AE_SNPRINTF(key_string, "%s\xFF%s\xFF%s\xFF%i",
        condition_string, function, filename, line) < 0)
    {
        ae_error("BOOL \"%s\" FUNC \"%s\" FILE \"%s\" LINE %i TOO BIG!",
                            condition_string, function, filename, line);
    }

    node = (ae_branch_node_t*)ae_strmap_get(&ae_global_branch_coverage, key_string);

    if (node == NULL)
    {
        node = ae_create(ae_branch_node_t, clear);

        node->conditional = condition_string;
        node->function = function;
        node->filename = filename;
        node->line = line;
        node->first = ae_seconds();

        ae_strmap_set(&ae_global_branch_coverage, key_string, node);
    }

    if (node->total == (size_t)-1)
    {
        AE_WARN("\"%s\" func \"%s\" file \"%s\" line %i overflowed",
                        condition_string, function, filename, line);
    }

    node->total++;
    node->taken += (condition != 0);

    return condition;
}

int ae_branch_report_enabled(void)
{
    return 1;
}

static int bc_filter_anything(ae_branch_node_t* node, void* data)
{
    return 1;
}

static int bc_filter_function(ae_branch_node_t* node, void* data)
{
    return strcmp(node->function, (const char*)data) == 0;
}

static int bc_filter_filename(ae_branch_node_t* node, void* data)
{
    return strstr(node->filename, (const char*)data) != NULL;
}

static int bc_filter_extremes(ae_branch_node_t* node, void* data)
{
    const double t_ratio = ((double)node->taken / (double)node->total) * 100.0;
    const double epsilon = *(double*)data;

    if (t_ratio < (100.0 - epsilon)) return 0;
    if (t_ratio > epsilon) return 0;

    return 1;
}

static int bc_filter_median(ae_branch_node_t* node, void* data)
{
    const double t_ratio = ((double)node->taken / (double)node->total) * 100.0;
    const double epsilon = *(double*)data;

    if (t_ratio > (100.0 - epsilon)) return 0;
    if (t_ratio < epsilon) return 0;

    return 1;
}

void ae_branch_report_clear_filter(AE_BRANCH_FILTER_FUNC filter, void* context)
{
    char* k;
    void* v;

    int i;

    // TODO: because we're using a hashtable key removal call that potentially
    // reallocs the table, we restart the loop each time we free a key in order
    // to avoid touching potentially invalid memory. This is obviously O(N^2).
    begin:

    for (i = 0; i < ae_global_branch_coverage.limit ? // iterate branches
                k = ae_global_branch_coverage.table[i].key,
                v = ae_global_branch_coverage.table[i].val, 1 : 0; ++i)
    {
        if (k == NULL || k == (char*)1) {} else
        {
            ae_branch_node_t* node = (ae_branch_node_t*)v;

            if (filter(node, context))
            {
                ae_free(v);
                ae_strmap_remove(&ae_global_branch_coverage, k, NULL);

                goto begin;
            }
        }
    }
}

void ae_branch_report_clear(void)
{
    ae_branch_report_clear_filter(bc_filter_anything, NULL);
}

void ae_branch_report_clear_file(const char* filename)
{
    ae_branch_report_clear_filter(bc_filter_filename, (void*)filename);
}

void ae_branch_report_clear_function(const char* function)
{
    ae_branch_report_clear_filter(bc_filter_function, (void*)function);
}

void ae_branch_report_clear_extremes(const double epsilon)
{
    ae_branch_report_clear_filter(bc_filter_extremes, (void*)&epsilon);
}

void ae_branch_report_clear_median(const double epsilon)
{
    ae_branch_report_clear_filter(bc_filter_median, (void*)&epsilon);
}

static int bc_sort_compare_total(const void* a, const void* b)
{
    ae_branch_node_t* a_node = *(ae_branch_node_t**)a;
    ae_branch_node_t* b_node = *(ae_branch_node_t**)b;

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

static int bc_sort_compare_true_count(const void* a, const void* b)
{
    ae_branch_node_t* a_node = *(ae_branch_node_t**)a;
    ae_branch_node_t* b_node = *(ae_branch_node_t**)b;

    const size_t a_true = a_node->taken;
    const size_t b_true = b_node->taken;

    if (a_true < b_true) return +1;
    if (a_true > b_true) return -1;

    return bc_sort_compare_total(a, b);
}

static int bc_sort_compare_false_count(const void* a, const void* b)
{
    ae_branch_node_t* a_node = *(ae_branch_node_t**)a;
    ae_branch_node_t* b_node = *(ae_branch_node_t**)b;

    const size_t a_false = a_node->total - a_node->taken;
    const size_t b_false = b_node->total - b_node->taken;

    if (a_false < b_false) return +1;
    if (a_false > b_false) return -1;

    return bc_sort_compare_total(a, b);
}

static int bc_sort_compare_true_ratio(const void* a, const void* b)
{
    ae_branch_node_t* a_node = *(ae_branch_node_t**)a;
    ae_branch_node_t* b_node = *(ae_branch_node_t**)b;

    double a_ratio = (double)a_node->taken / (double)a_node->total;
    double b_ratio = (double)b_node->taken / (double)b_node->total;

    if (a_ratio < b_ratio) return +1;
    if (a_ratio > b_ratio) return -1;

    return 0;
}

static int bc_sort_compare_false_ratio(const void* a, const void* b)
{
    ae_branch_node_t* a_node = *(ae_branch_node_t**)a;
    ae_branch_node_t* b_node = *(ae_branch_node_t**)b;

    double a_ratio = 1.0 - ((double)a_node->taken / (double)a_node->total);
    double b_ratio = 1.0 - ((double)b_node->taken / (double)b_node->total);

    if (a_ratio < b_ratio) return +1;
    if (a_ratio > b_ratio) return -1;

    return 0;
}

static int bc_sort_compare_halfwayness(const void* a, const void* b)
{
    ae_branch_node_t* a_node = *(ae_branch_node_t**)a;
    ae_branch_node_t* b_node = *(ae_branch_node_t**)b;

    double a_ratio = (double)a_node->taken / (double)a_node->total;
    double b_ratio = (double)b_node->taken / (double)b_node->total;

    double a_dist = fabs(a_ratio - 0.5);
    double b_dist = fabs(b_ratio - 0.5);

    if (a_dist < b_dist) return -1;
    if (a_dist > b_dist) return +1;

    return bc_sort_compare_false_ratio(a, b);
}

static int bc_sort_compare_conditional(const void* a, const void* b)
{
    ae_branch_node_t* a_node = *(ae_branch_node_t**)a;
    ae_branch_node_t* b_node = *(ae_branch_node_t**)b;

    return strcmp(a_node->conditional, b_node->conditional);
}

static int bc_sort_compare_function(const void* a, const void* b)
{
    ae_branch_node_t* a_node = *(ae_branch_node_t**)a;
    ae_branch_node_t* b_node = *(ae_branch_node_t**)b;

    // TODO: fall back to line compare in case of tie
    return strcmp(a_node->function, b_node->function);
}

static int bc_sort_compare_filename(const void* a, const void* b)
{
    ae_branch_node_t* a_node = *(ae_branch_node_t**)a;
    ae_branch_node_t* b_node = *(ae_branch_node_t**)b;

    // TODO: fall back to func compare in case of tie
    return strcmp(a_node->filename, b_node->filename);
}

static int bc_sort_compare_first_test(const void* a, const void* b)
{
    ae_branch_node_t* a_node = *(ae_branch_node_t**)a;
    ae_branch_node_t* b_node = *(ae_branch_node_t**)b;

    if (a_node->first < b_node->first) return -1;
    if (a_node->first > b_node->first) return +1;

    return 0;
}

static int bc_sort_compare_last_test(const void* a, const void* b)
{
    return -bc_sort_compare_first_test(a, b);
}

void ae_branch_report_render_filter(AE_BRANCH_FILTER_FUNC filter, void* context,
            AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)
{
    int (*compare)(const void*, const void*) = NULL;
    int i = 0, j = 0;

    char* k;
    void* v;

    const size_t bytes = ae_global_branch_coverage.count * sizeof(ae_branch_node_t*);
    ae_branch_node_t** items;

    items = (ae_branch_node_t**)ae_stack_malloc(ae_global_stack(), bytes);

    for ( ; i < ae_global_branch_coverage.limit ? // fill ptr array
            k = ae_global_branch_coverage.table[i].key,
            v = ae_global_branch_coverage.table[i].val, 1 : 0; ++i)
    {
        if (k == NULL || k == (char*)1) {} else
        {
            items[j++] = (ae_branch_node_t*)v;
        }
    }

    assert(ae_global_branch_coverage.count == j);

    switch (sort)
    {
        case AE_BRANCH_SORT_TOTAL:
            compare = bc_sort_compare_total; break;

        case AE_BRANCH_SORT_TRUE_COUNT:
            compare = bc_sort_compare_true_count; break;

        case AE_BRANCH_SORT_FALSE_COUNT:
            compare = bc_sort_compare_false_count; break;

        case AE_BRANCH_SORT_TRUE_RATIO:
            compare = bc_sort_compare_true_ratio; break;

        case AE_BRANCH_SORT_FALSE_RATIO:
            compare = bc_sort_compare_false_ratio; break;

        case AE_BRANCH_SORT_HALFWAYNESS:
            compare = bc_sort_compare_halfwayness; break;

        case AE_BRANCH_SORT_CONDITIONAL:
            compare = bc_sort_compare_conditional; break;

        case AE_BRANCH_SORT_FUNCTION:
            compare = bc_sort_compare_function; break;

        case AE_BRANCH_SORT_FILENAME:
            compare = bc_sort_compare_filename; break;

        case AE_BRANCH_SORT_FIRST_TEST:
            compare = bc_sort_compare_first_test; break;

        case AE_BRANCH_SORT_LAST_TEST:
            compare = bc_sort_compare_last_test; break;

        default: assert(0); break;
    }

    qsort(items, ae_global_branch_coverage.count, sizeof(ae_branch_node_t*), compare);

    for (i = 0; i < ae_imin(ae_global_branch_coverage.count, max_items); i++)
    {
        if (filter(items[i], context)) draw(items[i]);
    }

    ae_stack_free(ae_global_stack(), items, bytes);
}

void ae_branch_report_render(AE_BRANCH_RENDER_FUNC draw,
                ae_branch_sort_t sort, size_t max_items)
{
    ae_branch_report_render_filter(bc_filter_anything, NULL, draw, sort, max_items);
}

void ae_branch_report_render_file(const char* filename, // filter by file
    AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)
{
    ae_branch_report_render_filter(bc_filter_filename, (void*)filename, draw, sort, max_items);
}

void ae_branch_report_render_function(const char* function, // func filter
    AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)
{
    ae_branch_report_render_filter(bc_filter_function, (void*)function, draw, sort, max_items);
}

void ae_branch_report_render_extremes(const double epsilon, // outer ratios
    AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)
{
    ae_branch_report_render_filter(bc_filter_extremes, (void*)&epsilon, draw, sort, max_items);
}

void ae_branch_report_render_median(const double epsilon, // inner ratios
    AE_BRANCH_RENDER_FUNC draw, ae_branch_sort_t sort, size_t max_items)
{
    ae_branch_report_render_filter(bc_filter_median, (void*)&epsilon, draw, sort, max_items);
}

static void bc_print_node(ae_branch_node_t* node)
{
    // TODO: strip newlines and whitespace from huge multi-line bools
    printf( "bool \"%s\" in func \"%s\"\n"
            "\t(file \"%s\", line %i)\n"
            "\ttotal = %u, true = %u, false = %u (%.2f%% true)\n\n",
            node->conditional,
            node->function,
            node->filename,
            node->line,
            (u32)node->total,
            (u32)node->taken,
            (u32)(node->total - node->taken),
            ((double)node->taken / (double)node->total) * 100.0);
}

void ae_branch_report_print_filter(AE_BRANCH_FILTER_FUNC func, void* ctx,
                                ae_branch_sort_t sort, size_t max_items)
{
    // TODO: print an info banner first, similar to the one in profiler print
    ae_branch_report_render_filter(func, ctx, bc_print_node, sort, max_items);
}

void ae_branch_report_print(ae_branch_sort_t sort, size_t max_items)
{
    ae_branch_report_print_filter(bc_filter_anything, NULL, sort, max_items);
}

void ae_branch_report_print_file(const char* filename,
            ae_branch_sort_t sort, size_t max_items)
{
    ae_branch_report_print_filter(bc_filter_filename, (void*)filename, sort, max_items);
}

void ae_branch_report_print_function(const char* function,
                ae_branch_sort_t sort, size_t max_items)
{
    ae_branch_report_print_filter(bc_filter_function, (void*)function, sort, max_items);
}

void ae_branch_report_print_extremes(const double epsilon,
                ae_branch_sort_t sort, size_t max_items)
{
    ae_branch_report_print_filter(bc_filter_extremes, (void*)&epsilon, sort, max_items);
}

void ae_branch_report_print_median(const double epsilon,
                ae_branch_sort_t sort, size_t max_items)
{
    ae_branch_report_print_filter(bc_filter_median, (void*)&epsilon, sort, max_items);
}

void ae_branch_init(int argc, char** argv)
{
    ae_strmap_init(&ae_global_branch_coverage, 1024);
}

void ae_branch_quit(void)
{
    ae_branch_report_clear();

    ae_strmap_free(&ae_global_branch_coverage); // set table to initial state
    memset(&ae_global_branch_coverage, 0, sizeof(ae_global_branch_coverage));
}

#endif
