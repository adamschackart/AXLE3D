/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void ae_search_init(int argc, char** argv) {}
void ae_search_quit(void) {}

void* ae_fsearch(const void* key, const void* base, size_t count, size_t size,
        int (*compare)(const void*, const void*, const void*), void* context)
{
    u8* end = (u8 *)base + count * size;
    u8* ptr = (u8 *)base;

    for (; ptr < end; ptr += size)
    {
        if (!compare(key, ptr, context)) return (void*)ptr;
    }

    return NULL;
}

void* ae_rsearch(const void* key, const void* base, size_t count, size_t size,
        int (*compare)(const void*, const void*, const void*), void* context)
{
    for (; count; count--)
    {
        u8 * ptr = (u8 *)base + (count - 1) * size; // elem
        if (!compare(key, ptr, context)) return (void*)ptr;
    }

    return NULL;
}

void* ae_bsearch(const void* key, const void* base, size_t count, size_t size,
        int (*compare)(const void*, const void*, const void*), void* context)
{
    ae_assert(ae_is_sorted((void*)base, count, size, compare, context), "non-sorted input");

    #if 0
        if (count)
        {
            const size_t mid_idx = count / 2; // halve the search window size
            const u8* const mid_ptr = (const u8* const)base + mid_idx * size;

            const int diff = compare(key, mid_ptr, context);

            if (diff == 0)
            {
                return (void*)mid_ptr;
            }
            else if (diff > 0)
            {
                return ae_bsearch(key, mid_ptr + size, mid_idx - 1, size, compare, context);
            }
            else
            {
                return ae_bsearch(key, base, mid_idx, size, compare, context);
            }
        }
    #else
        while (count)
        {
            const size_t mid_idx = count / 2; // halve the search window size
            const u8* const mid_ptr = (const u8* const)base + mid_idx * size;

            const int diff = compare(key, mid_ptr, context);

            if (diff == 0)
            {
                return (void*)mid_ptr;
            }

            if (diff > 0)
            {
                base = mid_ptr + size;
                count -= mid_idx + 1;
            }
            else
            {
                count = mid_idx;
            }
        }
    #endif
    return NULL;
}
