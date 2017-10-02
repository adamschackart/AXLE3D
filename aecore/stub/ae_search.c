/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void* ae_fsearch(const void* key, const void* base, size_t count, size_t size,
        int (*compare)(const void*, const void*, const void*), void* context)
{
    AE_STUB(); return NULL;
}

void* ae_rsearch(const void* key, const void* base, size_t count, size_t size,
        int (*compare)(const void*, const void*, const void*), void* context)
{
    AE_STUB(); return NULL;
}

void* ae_bsearch(const void* key, const void* base, size_t count, size_t size,
        int (*compare)(const void*, const void*, const void*), void* context)
{
    AE_STUB(); return NULL;
}

void ae_search_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_search_quit(void)
{
    // AE_STUB();
}
