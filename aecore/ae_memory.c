/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
TODO: macro-level reflection for mem profiling - register heaps, chunks, stacks,
etc. in global hashtables by name, so we can iterate over all active structures.
(P.S. from the future - this proposed "object model" can be seen in xl_core.h).
TODO: build & print allocation reports similar to branch/switch/profile tracker.
TODO: ae_virtual_malloc/free for pages & ae_virtual_mprotect for executable bit.
TODO: single function for locking and unlocking heap, chunk, and stack mutexes.
TODO: endian swapping & read/write funcs (i.e. ae_read_be_u16, ae_write_le_flt).
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

#if !defined(_WIN32)
    #define USE_DL_PREFIX 1
    #define MSPACES 1
    #if defined(AE_DEBUG)
        #define DEBUG 1
        #define FOOTERS 1
    #else
        #define INSECURE 1
    #endif
    #include <dlmalloc.c>
#endif

/*
================================================================================
 * ~~ [ command-line arguments ] ~~ *
--------------------------------------------------------------------------------
*/

static char** ae_argv_copy;
static int    ae_argc_copy;

char** ae_argv(int* argc)
{
    *argc = ae_argc_copy;
    return(ae_argv_copy);
}

static void ae_argv_copy_init(int argc, char** argv)
{
    int i; // allocate with room for a terminating null pointer
    assert(!ae_argc_copy && !ae_argv_copy);

    ae_argv_copy = (char**)ae_calloc(argc + 1, sizeof(char*));
    ae_argc_copy = argc;

    for (i = 0; i < argc; i++)
    {
        ae_argv_copy[i] = ae_string_copy(argv[i]);
    }
}

static void ae_argv_copy_quit(void)
{
    int i;

    for (i = 0; i < ae_argc_copy; i++)
    {
        ae_string_free(ae_argv_copy[i]);
    }

    ae_free(ae_argv_copy);

    ae_argc_copy = 0;
    ae_argv_copy = NULL;
}

/*
================================================================================
 * ~~ [ misc. utilities ] ~~ *
--------------------------------------------------------------------------------
TODO: early fast path for pointer-sized memswap and memiszero; benchmark changes
--------------------------------------------------------------------------------
*/

void memswap(void* a_ptr, void* b_ptr, const size_t bytes)
{
    u8* a = (u8*)a_ptr, *end = a + bytes; // int register size
    u8* b = (u8*)b_ptr, *unr = end - (bytes % sizeof(size_t));

    #if defined(__SSE2__)
    if(ae_cpuinfo_sse2())
    {
        u8* ur2 = end - (bytes % 16); // unroll by vector size

        if (num_is_aligned((size_t)a, 16) &&
            num_is_aligned((size_t)b, 16) )
        {
            for (; a < ur2; a += 16, b += 16)
            {
                __m128i a_vec = _mm_load_si128((__m128i*)a);
                __m128i b_vec = _mm_load_si128((__m128i*)b);

                _mm_store_si128((__m128i*)a, b_vec);
                _mm_store_si128((__m128i*)b, a_vec);
            }
        }
        /*else
        {
            for (; a < ur2; a += 16, b += 16)
            {
                __m128i a_vec = _mm_loadu_si128((__m128i*)a);
                __m128i b_vec = _mm_loadu_si128((__m128i*)b);

                _mm_storeu_si128((__m128i*)a, b_vec);
                _mm_storeu_si128((__m128i*)b, a_vec);
            }
        }*/
    }
    #endif

    for (; a < unr; a += sizeof(size_t), b += sizeof(size_t))
    {
        size_t temp = *(size_t*)a;

        *(size_t*)a = *(size_t*)b;
        *(size_t*)b = temp;
    }

    for (; a < end; a++, b++)
    {
        u8 t = *a; *a = *b; *b = t;
    }
}

int memiszero(const void * const ptr, const size_t bytes)
{
    register size_t i = 0, // unroll by int register size
                    j = bytes - (bytes % sizeof(size_t));

    ae_assert(bytes ? ptr != NULL : 1, "%u-byte nullptr",
                                        (unsigned)bytes);

    for (; i < j; i += sizeof(size_t))
    {
        const u8* const p = ( (const u8* const)ptr ) + i;
        if (*((const size_t* const)p)) { return 0; }
    }

    for (; i < bytes; ++i)
    {
        const u8* const p = ( (const u8* const)ptr ) + i;
        if (*p) { return 0; }
    }

    return 1;
}

void memreverse(void* base, size_t e_count, size_t e_size)
{
    if (e_count) // TODO: could probably handle this without a branch
    {
        char* t = (char*)base + (e_count - 1) * e_size;
        char* s = (char*)base;
        ae_assert(e_size, "%p %u", base, (u32)e_count);

        for (; s < t; s += e_size, t -= e_size) memswap(s, t, e_size);
    }
}

size_t memfilter(void* dst, const void* const src, size_t count, size_t size,
                int (*filter)(const void* item, const void* ctx), void* ctx)
{
    u8* sp = (u8*)src;
    u8* ep = (u8*)src + count * size;
    u8* dp = (u8*)dst;

    assert(size);

    if (dst) // pass NULL to count elements
    {
        for (; sp < ep; sp += size)
        {
            if (filter(sp, ctx))
            {
                memcpy(dp, sp, size);
                dp += size;
            }
        }
    }
    else
    {
        for (; sp < ep; sp += size)
        {
            if (filter(sp, ctx)) dp += size;
        }
    }

    return (dp - (u8*)dst) / size;
}

/*
================================================================================
 * ~~ [ string formatting ] ~~ *
--------------------------------------------------------------------------------
TODO: ae_str(const char* s, int index) - if index is out of bounds, return '\0'
TODO: ANSI to Unicode conversion and vice versa (code should be in ae_string.h)
--------------------------------------------------------------------------------
*/

#define STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_STATIC
#include <stb_sprintf.h>

int ae_vsnprintf(char* buf, size_t size, const char* fmt, va_list args)
{
    #if defined(STB_SPRINTF_IMPLEMENTATION)
        int result = stbsp_vsnprintf(buf, size, fmt, args);
    #elif _MSC_VER >= 1400
        int result = _vsnprintf_s(buf, size, _TRUNCATE, fmt, args);
    #else
        int result = vsnprintf(buf, size, fmt, args);
    #endif

    if (result < 0 || (size_t)result >= size)
    {
        buf[size - 1] = '\0'; return -1;
    }

    return result;
}

int ae_snprintf(char* buf, size_t size, const char* fmt, ...)
{
    va_list args;
    int result;

    va_start(args, fmt);
    result = ae_vsnprintf(buf, size, fmt, args);
    va_end(args);

    return result;
}

void ae_strbounds(const char* str, size_t tab_width, size_t* cols, size_t* rows)
{
    ae_strnbounds(str, (size_t)-1, tab_width, cols, rows);
}

void ae_strnbounds(const char* string, size_t length,
        size_t tab_width, size_t* cols, size_t* rows)
{
    size_t x = 0;

    *cols = 0;
    *rows = 1;

    for (; length-- && *string != '\0'; string++)
    {
        switch (*string)
        {
            case '\n':
            {
                if (x > *cols) *cols = x;
                x = 0;
                *rows += 1;
            }
            break;

            case '\t':
            {
                x += tab_width;
            }
            break;

            default:
            {
                if (isprint(*string)) x++;
            }
            break;
        }
    }

    if (x > *cols) *cols = x;
}

/*
================================================================================
 * ~~ [ local heap allocator ] ~~ *
--------------------------------------------------------------------------------
TODO: guard bytes, double free checks, free from out of heap checks, size maxes
TODO: debug features we can share between the heap, block, and stack allocators
TODO: on linux we can mmap and fault smallblock pages (create_mspace_with_base)
TODO: linux usually uses glibc's tcmalloc, which allocates faster than dlmalloc
TODO: allocation counter for leak checking (realloc with a NULL arg only bumps)
--------------------------------------------------------------------------------
*/

void ae_heap_create_ex(ae_memory_heap_t * heap, const char* name, size_t reserve,
                    const char* filename, const char* funcname, const int lineno)
{
    ae_heap_destroy(heap);

    ae_assert( strlen(name) < sizeof(heap->name), "heap name \"%s\" is too long", name );
    strncpy(heap->name, name, sizeof(heap->name));

    #if defined(_WIN32)
        heap->heap = HeapCreate(0, reserve, 0);
    #else
        heap->heap = create_mspace(reserve, 0);
    #endif

    ae_log(HEAP, "created \"%s\" (%.2fmb reserved)", name, (reserve / 1024.0) / 1024.0);
    if (heap->heap == NULL)
    {
        AE_WARN("heap \"%s\" creation failed! using global fallback", heap->name);
    }
}

void ae_heap_destroy_ex(ae_memory_heap_t* heap, const char* filename,
                            const char* funcname, const int lineno )
{
    if (heap->heap)
    {
        ae_log(HEAP, "destroying heap \"%s\"", heap->name);

        #if defined(_WIN32)
            #if defined(AE_DEBUG)
            if (HeapValidate(heap->heap, 0, NULL) == FALSE)
            {
                AE_WARN("heap \"%s\" validation failed!", heap->name);
            }
            #endif
            HeapDestroy(heap->heap);
        #else
            // TODO log mspace_mallinfo
            destroy_mspace(heap->heap);
        #endif
    }

    heap->name[0] = 0;
    heap->heap = NULL;
}

#define AE_HEAP_NULLCHECK(p) if ((p) == NULL) /* check alloc success */ \
    {                                                                   \
        /* TODO: localize err string, maybe free some stuff & retry? */ \
        ae_error("OUT OF MEMORY: %s "                                   \
                "from heap \"%s\" failed in %s (file \"%s\" line %i)",  \
                __FUNCTION__, heap->name, funcname, filename, lineno);  \
    }

void* ae_heap_malloc_ex(ae_memory_heap_t* heap, size_t size, // get block
            const char* filename, const char* funcname, const int lineno)
{
    void* p;

    if (heap && heap->heap)
    {
        #if defined(_WIN32)
            p = HeapAlloc(heap->heap, 0, size);
        #else
            p = mspace_malloc(heap->heap, size);
        #endif
    }
    else
    {
        p = malloc(size);
    }

    #if defined(AE_HEAP_TRACE)
    ae_log(HEAP,
        "%s (%u bytes at %p) from heap \"%s\" in %s (file \"%s\" line %i)",
        __FUNCTION__, (u32)size, p,
        heap ? heap->name : "(NULL)", funcname, filename, lineno);
    #endif

    AE_HEAP_NULLCHECK(p);
    return p;
}

void ae_heap_free_ex(ae_memory_heap_t* heap, void* ptr, // release block
            const char* filename, const char* funcname, const int lineno)
{
    // XXX: python is freeing objects after the interpreter atexit runs.
    // this pointer comes from a heap that has already been destroyed.
    // TODO move this branch to the fallback path? (only place it runs)
    if (!ae_is_init())
    {
    //  AE_WARN("%p leaked (freed before init or after shutdown)", ptr);
        return;
    }

    if (heap && heap->heap)
    {
        #if defined(_WIN32)
            HeapFree(heap->heap, 0, ptr);
        #else
            mspace_free(heap->heap, ptr);
        #endif
    }
    else
    {
        free(ptr);
    }

    #if defined(AE_HEAP_TRACE)
    if (ptr) // don't clutter the log with null frees from the heap

    ae_log(HEAP,
        "%s at %p from heap \"%s\" in %s (file \"%s\" line %i)",
        __FUNCTION__, ptr,
        heap ? heap->name : "(NULL)", funcname, filename, lineno);
    #endif
}

void* ae_heap_calloc_ex(ae_memory_heap_t * heap, size_t num, size_t size,
            const char* filename, const char* funcname, const int lineno)
{
    void* p;

    if (heap && heap->heap)
    {
        #if defined(_WIN32)
            p = HeapAlloc(heap->heap, HEAP_ZERO_MEMORY, num * size);
        #else
            p = mspace_calloc(heap->heap, num, size);
        #endif
    }
    else
    {
        p = calloc(num, size);
    }

    #if defined(AE_HEAP_TRACE)
    ae_log(HEAP,
        "%s (%u * %u = %u bytes at %p) from heap \"%s\" in %s (file \"%s\" line %i)",
        __FUNCTION__, (u32)num, (u32)size, (u32)(num * size), p,
        heap ? heap->name : "(NULL)", funcname, filename, lineno);
    #endif

    AE_HEAP_NULLCHECK(p);
    return p;
}

void* ae_heap_realloc_ex(ae_memory_heap_t * heap, void* ptr, size_t size,
            const char* filename, const char* funcname, const int lineno)
{
    void* p;

    if (heap && heap->heap)
    {
        #if defined(_WIN32)
            if (ptr)
            {
                p = HeapReAlloc(heap->heap, 0, ptr, size);
            }
            else
            {
                p = HeapAlloc(heap->heap, 0, size);
            }
        #else
            p = mspace_realloc(heap->heap, ptr, size);
        #endif
    }
    else
    {
        p = realloc(ptr, size);
    }

    #if defined(AE_HEAP_TRACE)
    ae_log(HEAP,
        "%s (%p to %u bytes at %p) from heap \"%s\" in %s (file \"%s\" line %i)",
        __FUNCTION__, ptr, (u32)size, p,
        heap ? heap->name : "(NULL)", funcname, filename, lineno);
    #endif

    /* FIXME: realloc(p, 0) is different in C11 than in prior versions.
     * The standard says (7.20.3.1 for C11, 7.22.3.1 for C1x):
     *
     * "If the size of the space requested is zero, the behavior is
     * implementation-defined: either a null pointer is returned, or the
     * behavior is as if the size were some nonzero value, except
     * that the returned pointer shall not be used to access an object."
     */
    AE_HEAP_NULLCHECK(p);
    return p;
}

/*
================================================================================
 * ~~ [ global heap allocator ] ~~ *
--------------------------------------------------------------------------------
*/

// we don't expose the global heap as part of the public API, as ae_malloc(s)
// may not be equivalent to / implemented via ae_heap_malloc(&global_heap, s).
static ae_memory_heap_t global_heap;

// TODO: put heap tracing code into macros that we can easily re-use in debug
#if defined(_WIN32) && defined(_DEBUG) && !defined(AE_HEAP_TRACE)

// NOTE: the only real reason we use a global heap is to catch accidental use
// of malloc and free, which use the CRT heap. debug heap does the same thing.
#undef  AE_GLOBAL_HEAP_RESERVE
#define AE_GLOBAL_HEAP_RESERVE 0

void* ae_malloc_ex(size_t size, const char* filename, const char* funcname, const int lineno)
{
    void* p = _malloc_dbg(size, _NORMAL_BLOCK, filename, lineno);

    assert(p);
    return(p);
}

void ae_free_ex(void* ptr, const char* filename, const char* funcname, const int lineno)
{
    _free_dbg(ptr, _NORMAL_BLOCK);
}

void* ae_calloc_ex(size_t num, size_t size, const char* filename,
                        const char* funcname, const int lineno )
{
    void* p = _calloc_dbg(num, size, _NORMAL_BLOCK, filename, lineno);

    assert(p);
    return(p);
}

void* ae_realloc_ex(void* ptr, size_t size, const char* filename,
                        const char* funcname, const int lineno )
{
    void* p = _realloc_dbg(ptr, size, _NORMAL_BLOCK, filename, lineno);

    // always return a valid address, even when shrinking to size zero
    if (size == 0 && p == NULL)
    {
        p = _malloc_dbg(0, _NORMAL_BLOCK, filename, lineno);
    }

    assert(p);
    return(p);
}

#else

void* ae_malloc_ex(size_t size, const char* filename, const char* funcname, const int lineno)
{
    return ae_heap_malloc_ex(&global_heap, size, filename, funcname, lineno);
}

void ae_free_ex(void* ptr, const char* filename, const char* funcname, const int lineno)
{
    ae_heap_free_ex(&global_heap, ptr, filename, funcname, lineno);
}

void* ae_calloc_ex(size_t num, size_t size, const char* filename,
                        const char* funcname, const int lineno )
{
    return ae_heap_calloc_ex(&global_heap, num, size, filename, funcname, lineno);
}

void* ae_realloc_ex(void* ptr, size_t size, const char* filename,
                        const char* funcname, const int lineno )
{
    return ae_heap_realloc_ex(&global_heap, ptr, size, filename, funcname, lineno);
}

#endif

/*
================================================================================
 * ~~ [ dynamic arrays ] ~~ *
--------------------------------------------------------------------------------
TODO: AE_ARRAY_TRACE, which would require the addition of an ARRAY log category
--------------------------------------------------------------------------------
*/

/* NOTE: if we point an array to global stack memory for some hack, we want to
 * ensure that the array is cleared before being passed into funcs that resize.
 */
#define AE_ARRAY_STACK_CHECK() \
    \
    ae_assert( ae_global_stack()->size ? !ptr_in_array(array->data, \
        ae_global_stack()->mem_stack, ae_global_stack()->size) : 1, \
    \
    "stack free in \"%s\" (file \"%s\" line %i)", funcname, filename, lineno)

/* ===== [ external functions ] ============================================= */

void ae_array_append_ex(ae_array_t* array, void* data, size_t size,
    const char* filename, const char* funcname, const int lineno )
{
    AE_ARRAY_STACK_CHECK();

    ae_array_resize(array, array->size + size); // expand to fit
    memmove((u8*)array->data + (array->size - size), data, size);
}

void ae_array_resize_ex(ae_array_t * array, size_t size, const char * filename,
                                        const char* funcname, const int lineno)
{
    #if defined(AE_DEBUG) // exact resizing in case of ae_malloc guard bytes.
        AE_ARRAY_STACK_CHECK();

        array->data = ae_realloc(array->data, size);
        array->size = array->_cap = size;
    #else
        if (size > array->_cap)
        {
            // the first array allocation is always exactly sized, plus the
            // alignment we apply later to give more even growth patterns.
            if (array->_cap == 0) array->_cap = size;

            // expand the array capacity by 50% until it exceeds the new size.
            while (array->_cap < size) array->_cap = (array->_cap * 3) >> 1;

            // enforcing min size also avoids issues resizing 1-byte arrays.
            array->_cap = num_pow2_align(array->_cap, 16);
            array->data = ae_realloc(array->data, array->_cap);
        }

        array->size = size;
    #endif
}

void ae_array_reserve_ex(ae_array_t * array, size_t size, const char * filename,
                                        const char * funcname, const int lineno)
{
    #if defined(AE_DEBUG) // debug resize is exact
        AE_ARRAY_STACK_CHECK();
    #else
        const size_t old_array_size = array->size;

        ae_array_resize(array, size);
        array->size = old_array_size;
    #endif
}

void ae_array_copy_ptr_ex(void* ptr, const ae_array_t* array, const char* filename,
                                            const char* funcname, const int lineno)
{
    memmove(ptr, array->data, array->size);
}

void ae_array_copy_ex(ae_array_t* dst, const ae_array_t* src, const char* filename,
                                            const char* funcname, const int lineno)
{
    ae_array_resize  (dst, src->size);
    ae_array_copy_ptr(dst->data, src);
}

int ae_array_cmp_ex(const ae_array_t* a, const ae_array_t* b, const char* filename,
                                            const char* funcname, const int lineno)
{
    // XXX: could check for equality or take the min of both sizes, or do this...
    ae_assert(a->size == b->size, "A=%u != B=%u", (u32)a->size, (u32)b->size);

    // TODO: do most libc memcmp implementations return early if ptr A == ptr B?
    return (/*a->size == b->size &&*/ memcmp(a->data, b->data, a->size) == 0);
}

int ae_array_is_zero_ex(ae_array_t* array, const char* filename,
                        const char* funcname, const int lineno)
{
    return memiszero(array->data, array->size);
}

void ae_array_zero_ex(ae_array_t * array, const char * filename,
                        const char * funcname, const int lineno)
{
    memset(array->data, 0, array->size);
}

void ae_array_trim_ex(ae_array_t * array, const char * filename,
                        const char * funcname, const int lineno)
{
    #if defined(AE_DEBUG) // debug array resizing is always exact
        AE_ARRAY_STACK_CHECK();
    #else
        if (array->size)
        {
            array->_cap = num_pow2_align(array->size, 16); // SSE
            array->data = ae_realloc( array->data, array->_cap );
        }
        else
        {
            ae_array_free(array);
        }
    #endif
}

void ae_array_free_ex(ae_array_t * array, const char * filename,
                        const char * funcname, const int lineno)
{
    AE_ARRAY_STACK_CHECK();

    ae_free(array->data); // zero for reuse
    memset( array, 0, sizeof(ae_array_t) );
}

/*
================================================================================
 * ~~ [ fixed-size block allocator ] ~~ *
--------------------------------------------------------------------------------
This is taken from the paper "Fast Efficient Fixed-Size Memory Pool: No Loops
and No Overhead" by Ben Kenwright. It stores information about free blocks in
the free blocks themselves, rather than keeping a large separate list of them.
--------------------------------------------------------------------------------
TODO: pass filename, funcname, and lineno through to fallback heap allocation
TODO: micro-optimize this! reorder struct members, use size_t for 64bit, etc.
TODO: show memory profiling info at exit; max number of blocks allocated etc.
TODO: function to free all blocks in a chunk at once rather than individually
--------------------------------------------------------------------------------
*/

void ae_chunk_create_ex(ae_memory_chunk_t* chunk, const char* name, u32 block_size,
    u32 num_blocks, const char* filename, const char* funcname, const int lineno)
{
    ae_chunk_destroy(chunk);

    ae_assert(strlen(name) < sizeof(chunk->name), "chunk name \"%s\" is too long", name);
    strncpy(chunk->name, name, sizeof(chunk->name));

    ae_assert(block_size >= 4, "block size %u can't fit free block indices", block_size);

    chunk->num_blocks = num_blocks;
    chunk->block_size = block_size;
    chunk->num_free = num_blocks;

    /* TODO: do this allocation (and the subsequent free in ae_chunk_destroy_ex)
     * through an internal API we can share with the stack that calls VirtualAlloc
     * or mmap. the alloc func would take a bool arg that triggers page faulting.
     * can we fix OOM errors when using more than ~1.5gb on 32-bit windows builds?
     */
    chunk->mem_chunk = (u8*)ae_malloc(block_size * num_blocks);
    chunk->next_block = chunk->mem_chunk;

    /* this is mostly to generate page faults, so we pay for the cost of accessing
     * this memory up front, rather than in the middle of a game frame. it also has
     * the benefit of keeping us from relying on implicit initialization to zero.
     */
    memset(chunk->mem_chunk, 0xFFFFFFFF, block_size * num_blocks);

    ae_log( BLOCK, "created \"%s\" (%u bytes per block * %u blocks = %.2fmb reserved)", name,
            block_size, num_blocks, (((double)(block_size * num_blocks)) / 1024.0) / 1024.0);
}

void ae_chunk_destroy_ex(ae_memory_chunk_t* chunk, const char* filename,
                                const char* funcname, const int lineno)
{
    if (chunk->mem_chunk)
    {
        ae_log(BLOCK, "destroying chunk \"%s\"", chunk->name);
        ae_free(chunk->mem_chunk);
    }

    memset(chunk, 0, sizeof(ae_memory_chunk_t));
}

static c_inline u8* ae_block_address_from_index(ae_memory_chunk_t* chunk, u32 i)
{
    return chunk->mem_chunk + (chunk->block_size * i);
}

static c_inline u32 ae_block_index_from_address(ae_memory_chunk_t* chunk, u8* p)
{
    return ((u32)(p - chunk->mem_chunk)) / chunk->block_size;
}

#define AE_CHECK_MEMORY_CHUNK() ae_assert(chunk && chunk->mem_chunk, \
    "invalid chunk \"%s\"! struct pointer at %p chunk memory at %p", \
    chunk ? chunk->name : "(NULL)", chunk, chunk ? chunk->mem_chunk : NULL)

void* ae_block_malloc_ex(ae_memory_chunk_t* chunk, const char* filename,
                                const char* funcname, const int lineno)
{
    AE_CHECK_MEMORY_CHUNK(); // ensure that the chunk and all its data are valid
    if (chunk->num_init < chunk->num_blocks)
    {
        u32* p = (u32*)ae_block_address_from_index(chunk, chunk->num_init);
        *p = ++chunk->num_init;
    }

    // TODO: alternate fallback mode (controllable by boolean), where instead
    // of falling back to malloc, another chunk of equal size is created and
    // appended to the end of a linked list (chunk also keeps a next pointer).

    if (chunk->num_free)
    {
        void* ret_p = (void*)chunk->next_block;

        if (--chunk->num_free)
        {
            chunk->next_block = ae_block_address_from_index(chunk,
                                    *((u32 *) chunk->next_block));
        }
        else
        {
            chunk->next_block = NULL;
        }

        #if defined(AE_BLOCK_TRACE)
        ae_log(BLOCK,
            "%s (%u bytes at %p) from chunk \"%s\" in %s (file \"%s\" line %i)",
            __FUNCTION__,
            chunk->block_size, ret_p, chunk->name, funcname, filename, lineno);
        #endif
        return ret_p;
    }

    /* TODO: keep count of these "overrun" bytes we alloc outside the chunk
     */
    AE_WARN("chunk \"%s\" is full! using ae_malloc fallback", chunk->name);
    return ae_malloc(chunk->block_size);
}

void* ae_block_calloc_ex(ae_memory_chunk_t* chunk, const char* filename,
                                const char* funcname, const int lineno)
{
    AE_CHECK_MEMORY_CHUNK();

    return memset(ae_block_malloc_ex(chunk, filename,
            funcname, lineno), 0, chunk->block_size);
}

void ae_block_free_ex(ae_memory_chunk_t * chunk, void* p, const char* filename,
                                        const char* funcname, const int lineno)
{
    // XXX: python is freeing objects after the interpreter atexit runs.
    // this block comes from a chunk that has already been destroyed.
    // TODO move this branch to the fallback path? (only place it runs)
    if (!ae_is_init())
    {
    //  AE_WARN("%p leaked (freed before init or after shutdown)", p);
        return;
    }

    AE_CHECK_MEMORY_CHUNK(); // ensure that the chunk and all its data are valid
    if (ptr_in_array(p, chunk->mem_chunk, chunk->num_blocks * chunk->block_size))
    {
        if (chunk->next_block)
        {
            *(u32*)p = ae_block_index_from_address(chunk, chunk->next_block);
        }
        else
        {
            *(u32*)p = chunk->num_blocks;
        }

        #if defined(AE_BLOCK_TRACE)
        if (p) // don't clutter the log with null frees from the chunk

        ae_log(BLOCK,
            "%u-byte %s at %p from chunk \"%s\" in %s (file \"%s\" line %i)",
            chunk->block_size,
            __FUNCTION__, p, chunk->name, funcname, filename, lineno);
        #endif

        chunk->next_block = (u8*)p;
        chunk->num_free++;
    }
    else
    {
        ae_free(p);
    }
}

/*
================================================================================
 * ~~ [ stack allocator ] ~~ *
--------------------------------------------------------------------------------
TODO: micro-optimize this! reorder struct members, use size_t for 64bit, etc.
TODO: stack realloc could be really easy to implement here (just change size)
TODO: pass filename, funcname, and lineno through to fallback heap allocation
TODO: show memory profiling info at exit - max number of bytes allocated etc.
TODO: function to free all stack memory at once rather than individual allocs
--------------------------------------------------------------------------------
*/

ae_memory_stack_t* ae_global_stack(void)
{
    static ae_memory_stack_t global_stack;
    return &global_stack;
}

void ae_stack_create_ex(ae_memory_stack_t* stack, const char* name, u32 size,
                const char* filename, const char* funcname, const int lineno)
{
    ae_stack_destroy(stack);

    ae_assert(strlen(name) < sizeof(stack->name), "stack name \"%s\" is too long", name);
    strncpy(stack->name, name, sizeof(stack->name));

    stack->mem_stack = (u8*)ae_malloc(size);
    stack->size = size;
    stack->alignment = 16;

    /* this is mostly to generate page faults, so we pay for the cost of accessing
     * this memory up front, rather than in the middle of a game frame. it also has
     * the benefit of keeping us from relying on implicit initialization to zero.
     */
    memset(stack->mem_stack, 0xFFFFFFFF, size);

    ae_log(STACK, "created \"%s\" (%.2fmb reserved)",
            name, ((double)size / 1024.0) / 1024.0);
}

void ae_stack_destroy_ex(ae_memory_stack_t* stack, const char* filename,
                                const char* funcname, const int lineno)
{
    /* TODO assert that zero bytes are left allocated on free?
     */
    if (stack->mem_stack)
    {
        ae_log(STACK, "destroying stack \"%s\"", stack->name);
        ae_free(stack->mem_stack);
    }

    memset(stack, 0, sizeof(ae_memory_stack_t));
}

#define AE_CHECK_MEMORY_STACK() ae_assert(stack && stack->mem_stack, \
    "invalid stack \"%s\"! struct pointer at %p stack memory at %p", \
    stack ? stack->name : "(NULL)", stack, stack ? stack->mem_stack : NULL)

void* ae_stack_malloc_ex( ae_memory_stack_t * stack, size_t size,
    const char* filename, const char* funcname, const int lineno)
{
    void* p; u32 old_used, new_used;
    AE_CHECK_MEMORY_STACK();

    old_used = stack->used; // keep the old size around for new offset
    new_used = old_used + num_pow2_align(size, stack->alignment);

    if (new_used > stack->size)
    {
        /* TODO: count the "overrun" bytes allocated outside the stack
         */
        AE_WARN("stack \"%s\" is full! using fallback", stack->name);
        return ae_malloc(size);
    }

    p = stack->mem_stack + old_used;
    stack->used = new_used;

    #if defined(AE_STACK_TRACE)
    ae_log(STACK,
        "%s (%u bytes at %p) from stack \"%s\" in %s (file \"%s\" line %i)",
        __FUNCTION__,(u32)size, p, stack->name, funcname, filename, lineno);
    #endif
    return p;
}

void* ae_stack_calloc_ex(ae_memory_stack_t* stack, size_t num, size_t size,
            const char* filename, const char* funcname, const int lineno )
{
    AE_CHECK_MEMORY_STACK();

    /* NOTE: if we need this to be faster for whatever reason, we could re-clear
     * stack pages during initialization and track of the maximum number of bytes
     * the stack has allocated. if memory hasn't been used yet, it will be zeros.
     */
    return memset(ae_stack_malloc_ex(stack, num * size ,
            filename, funcname, lineno), 0, num * size);
}

void ae_stack_free_ex(ae_memory_stack_t* stack, void* p, size_t size,
        const char* filename, const char* funcname, const int lineno)
{
    // TODO: avoid this align in release builds by just setting an end pointer
    const size_t asize = num_pow2_align(size, stack->alignment);

    AE_CHECK_MEMORY_STACK();

    if (ptr_in_array(p, stack->mem_stack, stack->size))
    {
        // ensure most recent stack free (TODO: show useful information here)
        ae_assert(p == stack->mem_stack + (stack->used - asize), "bad free");

        #if defined(AE_STACK_TRACE)
        if (p) // don't clutter the log file with null frees from the stack

        ae_log(STACK,
            "%u-byte %s at %p from stack \"%s\" in %s (file \"%s\" line %i)",
        (u32)size, __FUNCTION__, p, stack->name, funcname, filename, lineno);
        #endif

        stack->used -= asize;
    }
    else
    {
        ae_free(p);
    }
}

/*
================================================================================
 * ~~ [ string allocator ] ~~ *
--------------------------------------------------------------------------------
TODO: what do most c std libs do to optimize strdup, and can we apply that here?
TODO: we can copy small strings without taking their length first (single pass).
if the copy loop overruns block size, we can discard the block and use the heap.
--------------------------------------------------------------------------------
TODO: AE_STRING_TRACE, which requires the addition of a STRING logging category.
TODO: the empty string "" is a very common case - handle without wasting blocks.
TODO: should we pass file, func, and line through to the lower-level allocators?
TODO: constant string pool allocator - keep reference count in string hash table
--------------------------------------------------------------------------------
*/

// reserve 8 mb (for smaller dict strings)
#ifndef AE_SMALL_STRING_NUM_BLOCKS
#define AE_SMALL_STRING_NUM_BLOCKS 262144
#endif

#ifndef AE_SMALL_STRING_BLOCK_SIZE
#define AE_SMALL_STRING_BLOCK_SIZE 32
#endif

static ae_memory_chunk_t ae_small_strings;

/* ===== [ implementation ] ================================================= */

void* ae_string_malloc_ex(size_t length, // length is character count
        const char* filename, const char* funcname, const int lineno)
{
    char* s; // malloc and stamp down null terminator
    if (length < AE_SMALL_STRING_BLOCK_SIZE)
    {
        s = (char*)ae_block_malloc(&ae_small_strings);
    }
    else
    {
        s = (char*)ae_malloc(length + 1); // fallback
    }

    s[length] = '\0';
    return s;
}

void ae_string_free_ex(void* string, // free from string chunk or heap
        const char* filename, const char* funcname, const int lineno)
{
    ae_block_free(&ae_small_strings, string); // might call ae_free
}

char* ae_string_copy_sized_ex(char* string, size_t len, // char count
        const char* filename, const char* funcname, const int lineno)
{
    assert(string); return (char*)memcpy(ae_string_malloc(len), string, len);
}

char* ae_string_copy_ex(char* string, // copy string to alloced block
        const char* filename, const char* funcname, const int lineno)
{
    assert(string); return ae_string_copy_sized(string, strlen(string));
}

/*
================================================================================
 * ~~ [ initialization ] ~~ *
--------------------------------------------------------------------------------
TODO: make global heap & stack sizes optionally configurable on the command line
--------------------------------------------------------------------------------
*/

void ae_memory_init(int argc, char** argv)
{
    #ifndef AE_GLOBAL_HEAP_RESERVE // reserve 8 mb for small blocks first
    #define AE_GLOBAL_HEAP_RESERVE (1024 * 1024 * 8)
    #endif
    ae_heap_create(&global_heap, "global", AE_GLOBAL_HEAP_RESERVE);

    #ifndef AE_GLOBAL_STACK_RESERVE // reserve 8 mb for global stack alloc
    #define AE_GLOBAL_STACK_RESERVE (1024 * 1024 * 8)
    #endif
    ae_stack_create(ae_global_stack(), "global", AE_GLOBAL_STACK_RESERVE);

    ae_chunk_create(&ae_small_strings, "strings", // strmap symbols
            AE_SMALL_STRING_BLOCK_SIZE, AE_SMALL_STRING_NUM_BLOCKS);

    ae_argv_copy_init(argc, argv);
}

void ae_memory_quit(void)
{
    ae_argv_copy_quit();

    ae_chunk_destroy(&ae_small_strings);
    ae_stack_destroy(ae_global_stack());

    ae_heap_destroy(&global_heap);
}
