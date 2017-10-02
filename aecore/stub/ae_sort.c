/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

int ae_is_sorted(void *base, size_t num, size_t size, // is array already sorted?
            int (*compare)(const void*, const void*, const void*), void* context)
{
    AE_STUB(); return 0;
}

int ae_is_sorted_ex(void *e_base, size_t e_num, size_t e_size,
                    int (*compare)(const void*, const void*))
{
    AE_STUB(); return 0;
}

void ae_bubblesort(void *base, size_t num, size_t size, // a really terrible sort
            int (*compare)(const void*, const void*, const void*), void* context)
{
    AE_STUB();
}

void ae_combsort(void *base, size_t num, size_t size, // a simple O(n log n) sort
            int (*compare)(const void*, const void*, const void*), void* context)
{
    AE_STUB();
}

void ae_sort_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_sort_quit(void)
{
    // AE_STUB();
}
