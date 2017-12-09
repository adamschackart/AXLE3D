/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_SORT_H__
#define __AE_SORT_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_sort_init(int argc, char** argv);
void ae_sort_quit(void); // private init

/* Allows the use of context-free stdlib qsort or bsearch comparison functions
 * with our sorting and searching routines (pass the old function as context).
 * TODO: move to ae_compare.h and define a bunch of integral type comparators.
 */
static int ae_call_comparator(const void* a, const void* b, const void* data)
{
    return ((int(*)(const void*, const void*))data)(a, b);
}

AE_DECL int AE_CALL ae_is_sorted(void *base, size_t num, size_t size, // O(n)
        int (*compare)(const void*, const void*, const void*), void* context);

AE_DECL int AE_CALL ae_is_sorted_ex(void *e_base, size_t e_num, size_t e_size,
                                    int (*compare)(const void*, const void*));

AE_DECL void AE_CALL ae_bubblesort(void *e_base, size_t e_num, size_t e_size,
        int (*compare)(const void*, const void*, const void*), void* context);

AE_DECL void AE_CALL ae_combsort(void* e_base, size_t e_count, size_t e_size,
        int (*compare)(const void*, const void*, const void*), void* context);

AE_DECL void AE_CALL ae_sort(void* base, size_t count, size_t size, // fastest
        int (*compare)(const void*, const void*, const void*), void* context);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_SORT_H__ */
