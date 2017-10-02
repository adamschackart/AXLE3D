/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_SEARCH_H__
#define __AE_SEARCH_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_search_init(int argc, char** argv);
void ae_search_quit(void); // private init

AE_DECL void* AE_CALL // Forward linear search for unsorted arrays.
ae_fsearch(const void * key, const void * base, size_t count, size_t size,
    int (*compare)(const void*, const void*, const void*), void* context);

AE_DECL void* AE_CALL // Reverse linear search for unsorted arrays.
ae_rsearch(const void * key, const void * base, size_t count, size_t size,
    int (*compare)(const void*, const void*, const void*), void* context);

/* Binary search for sorted arrays. Returns either the first match, or NULL
 * if nothing matched. Unlike stdlib bsearch, compare also takes user data.
 */
AE_DECL void* AE_CALL
ae_bsearch(const void * key, const void * base, size_t count, size_t size,
    int (*compare)(const void*, const void*, const void*), void* context);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_SEARCH_H__ */
