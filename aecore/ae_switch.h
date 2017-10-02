/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_SWITCH_H__
#define __AE_SWITCH_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_switch_init(int argc, char** argv);
void ae_switch_quit(void); // private init

/*
================================================================================
 * ~~ [ coverage tracker syntax ] ~~ *
--------------------------------------------------------------------------------
XXX: use this internally with great caution. ae_init might not be called yet,
or the thing you're marking up with ae_switch might be used to implement it...

XXX this cute little hack is not thread safe, or designed with speed in mind!
--------------------------------------------------------------------------------
*/

AE_DECL int AE_CALL
ae_switch_ex(int e_value, const char* type_name, const char** value_names,
    int enum_count, const char* function, const char* filename, int line);

#if defined(AE_SWITCH_COVERAGE)
    /*
        NOTE: This is designed to work with enums declared in our style, which
        have automatically-defined string tables & counts for easy debugging.

        (ae_)   (image_format)  (_t)

        /\      /\              /\
        prefix  name            suffix

        Type is the optional prefix and name joined together without the suffix.
        Caps is the "caps name" for values in the enum, ie. AE_IMAGE_FORMAT.
        Suffix mode (either a bare suffix or nosuffix identifier) appends a _t.
    */
    #define AE_SWITCH(value, type, suffix_type, caps) switch ((suffix_type) \
                                                                            \
            ae_switch_ex((int)(value), AE_STRINGIFY(suffix_type),           \
            (type ## _name), (int)(caps ## _COUNT), __FUNCTION__, __FILE__, __LINE__))

    #define ae_switch_suffix(value, type, caps) \
            AE_SWITCH((value), type, type ## _t, caps)

    #define ae_switch_nosuffix(value, type, caps) \
            AE_SWITCH((value), type, type, caps)

    #define ae_switch(value, type, caps, suffix_mode) \
            ae_switch_ ## suffix_mode((value), type, caps)
#else
    #define ae_switch(value, type, caps, suffix_mode) switch (value)
#endif

/*
================================================================================
 * ~~ [ global coverage report ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL int AE_CALL ae_switch_report_enabled(void);

typedef struct ae_switch_node_t
{
    const char** names;
    const char* type;

    const char* function;
    const char* filename;
    int line;

    double first;
    size_t total;
    size_t count;

    size_t* taken;
} \
    ae_switch_node_t;

typedef int (*AE_SWITCH_FILTER_FUNC)(ae_switch_node_t* node, void* data);

AE_DECL void AE_CALL ae_switch_report_clear_filter(AE_SWITCH_FILTER_FUNC, void*);
AE_DECL void AE_CALL ae_switch_report_clear(void);

AE_DECL void AE_CALL ae_switch_report_clear_function(const char* func);
AE_DECL void AE_CALL ae_switch_report_clear_file(const char* filename);

#define ae_switch_report_clear_this_function() \
        ae_switch_report_clear_function(__FUNCTION__)

#define ae_switch_report_clear_this_file() \
        ae_switch_report_clear_file(__FILE__)

AE_DECL void AE_CALL ae_switch_report_clear_type(const char* type_str);

typedef void (*AE_SWITCH_RENDER_FUNC)(ae_switch_node_t* node);

#define AE_SWITCH_SORT_N            \
                                    \
    /* sorting by count */          \
    N(AE_SWITCH_SORT_TOTAL),        \
                                    \
    /* sorting by cases covered */  \
    N(AE_SWITCH_SORT_SPREAD),       \
                                    \
    /* lexical sorting */           \
    N(AE_SWITCH_SORT_TYPENAME),     \
    N(AE_SWITCH_SORT_FUNCTION),     \
    N(AE_SWITCH_SORT_FILENAME),     \
                                    \
    /* chronological sorting */     \
    N(AE_SWITCH_SORT_FIRST_TEST),   \
    N(AE_SWITCH_SORT_LAST_TEST),    \
                                    \
    N(AE_SWITCH_SORT_COUNT)

typedef enum ae_switch_sort_t
{
    #define N(x) x
    AE_SWITCH_SORT_N
    #undef N
} \
    ae_switch_sort_t;

static const char* ae_switch_sort_name[] =
{
    #define N(x) #x
    AE_SWITCH_SORT_N
    #undef N
};

/* ===== [ abstract iteration ] ============================================= */

AE_DECL void AE_CALL
ae_switch_report_render_filter(AE_SWITCH_FILTER_FUNC func, void* context,
    AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items);

AE_DECL void AE_CALL ae_switch_report_render( // Display all statements.
    AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items);

AE_DECL void AE_CALL ae_switch_report_render_function(const char * func,
    AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items);

AE_DECL void AE_CALL ae_switch_report_render_file(const char * filename,
    AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items);

#define ae_switch_report_render_this_function(draw, sort, max_items) \
        ae_switch_report_render_function(__FUNCTION__, (draw), (sort), (max_items))

#define ae_switch_report_render_this_file(draw, sort, max_items) \
        ae_switch_report_render_file(__FILE__, (draw), (sort), (max_items))

AE_DECL void AE_CALL ae_switch_report_render_type(const char* type_name,
    AE_SWITCH_RENDER_FUNC draw, ae_switch_sort_t sort, size_t max_items);

/* ===== [ stdout reporting ] =============================================== */

AE_DECL void AE_CALL
ae_switch_report_print_filter(AE_SWITCH_FILTER_FUNC func, void* context,
                                ae_switch_sort_t sort, size_t max_items);

AE_DECL void AE_CALL ae_switch_report_print(
    ae_switch_sort_t sort, size_t max_items);

AE_DECL void AE_CALL ae_switch_report_print_function(const char* func_s,
                                ae_switch_sort_t sort, size_t max_items);

AE_DECL void AE_CALL ae_switch_report_print_file(const char* filename_s,
                                ae_switch_sort_t sort, size_t max_items);

#define ae_switch_report_print_this_function(sort, max_items) \
        ae_switch_report_print_function(__FUNCTION__, (sort), (max_items))

#define ae_switch_report_print_this_file(sort, max_items) \
        ae_switch_report_print_file(__FILE__, (sort), (max_items))

AE_DECL void AE_CALL ae_switch_report_print_type(const char* typename_s,
                                ae_switch_sort_t sort, size_t max_items);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_SWITCH_H__ */
