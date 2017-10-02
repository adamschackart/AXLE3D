/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

/* TODO:
    - ae_heapsort
    - ae_insertionsort
    - ae_mergesort
    - ae_quicksort
    - ae_selectionsort
    - ae_shellsort
    - ae_smoothsort
    - ae_timsort
 */
void ae_sort_init(int argc, char** argv) {}
void ae_sort_quit(void) {}

int ae_is_sorted(void *base, size_t num, size_t size, // is array already sorted?
            int (*compare)(const void*, const void*, const void*), void* context)
{
    size_t i;

    for (i = 1; i < num; i++) // user comparison data
    {
        char* b = (char*)base + i * size;
        char* a = b - size;

        if (compare(a, b, context) > 0) { return 0; }
    }

    return 1;
}

int ae_is_sorted_ex(void *e_base, size_t e_num, size_t e_size,
                    int (*compare)(const void*, const void*))
{
    size_t i;

    for (i = 1; i < e_num; i++) // libc style
    {
        char* b = (char*)e_base + i * e_size;
        char* a = b - e_size;

        if (compare(a, b) > 0) { return 0; }
    }

    return 1;
}

void ae_bubblesort(void *base, size_t num, size_t size, // a really terrible sort
            int (*compare)(const void*, const void*, const void*), void* context)
{
    size_t i, j;

    for (i = 0; i < num; i++) // O(n ^ 2)
    for (j = 0; j < num - 1; j++)
    {
        char* a = (char*)base + j * size;
        char* b = a + size;

        if (compare(a, b, context) > 0)
        {
            memswap(a, b, size);
        }
    }

    ae_assert(ae_is_sorted(base, num, size, compare, context), "bad sort");
}

void ae_combsort(void *base, size_t num, size_t size, // a simple O(n log n) sort
            int (*compare)(const void*, const void*, const void*), void* context)
{
    size_t gap = num;
    size_t i, j;

    char *p1, *p2;
    int swapped;

    if (!num) return;

    do
    {
        /* deal with turtles in the list by using a shrinking swap window */
        gap = (gap * 10) / 13;

        if (gap == 9 || gap == 10) gap = 11;
        if (gap < 1) gap = 1;

        swapped = 0;

        for (i = 0, p1 = (char *)base; i < num - gap; i++, p1 += size)
        {
            j = i + gap;

            if (compare(p1, p2 = (char *)base + j * size, context) > 0)
            {
                memswap(p1, p2, size);
                swapped = 1;
            }
        }
    }
    while (gap > 1 || swapped);

    ae_assert(ae_is_sorted(base, num, size, compare, context), "bad sort");
}
