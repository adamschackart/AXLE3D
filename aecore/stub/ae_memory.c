/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

char** ae_argv(int* argc)
{
    AE_STUB(); return NULL;
}

void memswap(void* a_ptr, void* b_ptr, const size_t bytes)
{
    AE_STUB();
}

int memiszero(const void * const ptr, const size_t bytes)
{
    AE_STUB(); return 0;
}

void memreverse(void* base, size_t e_count, size_t e_size)
{
    AE_STUB();
}

size_t memfilter(void* dst, const void* const src, size_t count, size_t size,
                int (*filter)(const void* item, const void* ctx), void* ctx)
{
    AE_STUB(); return 0;
}

int ae_vsnprintf(char* buf, size_t size, const char* fmt, va_list args)
{
    AE_STUB(); return -1;
}

int ae_snprintf(char* buf, size_t size, const char* fmt, ...)
{
    AE_STUB(); return -1;
}

void ae_strbounds(const char* str, size_t tab_width, size_t* cols, size_t* rows)
{
    AE_STUB();
}

void ae_strnbounds(const char* string, size_t length,
        size_t tab_width, size_t* cols, size_t* rows)
{
    AE_STUB();
}

void ae_heap_create_ex(ae_memory_heap_t * heap, const char* name, size_t reserve,
                    const char* filename, const char* funcname, const int lineno)
{
    AE_STUB();
}

void ae_heap_destroy_ex(ae_memory_heap_t* heap, const char* filename,
                            const char* funcname, const int lineno )
{
    AE_STUB();
}

void* ae_heap_malloc_ex(ae_memory_heap_t* heap, size_t size, // get block
            const char* filename, const char* funcname, const int lineno)
{
    AE_STUB(); return NULL;
}

void ae_heap_free_ex(ae_memory_heap_t* heap, void* ptr, // release block
            const char* filename, const char* funcname, const int lineno)
{
    AE_STUB();
}

void* ae_heap_calloc_ex(ae_memory_heap_t * heap, size_t num, size_t size,
            const char* filename, const char* funcname, const int lineno)
{
    AE_STUB(); return NULL;
}

void* ae_heap_realloc_ex(ae_memory_heap_t * heap, void* ptr, size_t size,
            const char* filename, const char* funcname, const int lineno)
{
    AE_STUB(); return NULL;
}

void* ae_malloc_ex(size_t size, const char* filename, const char* funcname, const int lineno)
{
    AE_STUB(); return NULL;
}

void ae_free_ex(void* ptr, const char* filename, const char* funcname, const int lineno)
{
    AE_STUB();
}

void* ae_calloc_ex(size_t num, size_t size, const char* filename,
                        const char* funcname, const int lineno )
{
    AE_STUB(); return NULL;
}

void* ae_realloc_ex(void* ptr, size_t size, const char* filename,
                        const char* funcname, const int lineno )
{
    AE_STUB(); return NULL;
}

void ae_array_append_ex(ae_array_t* array, void* data, size_t size,
    const char* filename, const char* funcname, const int lineno )
{
    AE_STUB();
}

void ae_array_resize_ex(ae_array_t * array, size_t size, const char * filename,
                                        const char* funcname, const int lineno)
{
    AE_STUB();
}

void ae_array_reserve_ex(ae_array_t * array, size_t size, const char * filename,
                                        const char * funcname, const int lineno)
{
    AE_STUB();
}

void ae_array_copy_ptr_ex(void* ptr, const ae_array_t* array, const char* filename,
                                            const char* funcname, const int lineno)
{
    AE_STUB();
}

void ae_array_copy_ex(ae_array_t* dst, const ae_array_t* src, const char* filename,
                                            const char* funcname, const int lineno)
{
    AE_STUB();
}

int ae_array_cmp_ex(const ae_array_t* a, const ae_array_t* b, const char* filename,
                                            const char* funcname, const int lineno)
{
    AE_STUB(); return 0;
}

int ae_array_is_zero_ex(ae_array_t* array, const char* filename,
                        const char* funcname, const int lineno)
{
    AE_STUB(); return 0;
}

void ae_array_zero_ex(ae_array_t * array, const char * filename,
                        const char * funcname, const int lineno)
{
    AE_STUB();
}

void ae_array_trim_ex(ae_array_t * array, const char * filename,
                        const char * funcname, const int lineno)
{
    AE_STUB();
}

void ae_array_free_ex(ae_array_t * array, const char * filename,
                        const char * funcname, const int lineno)
{
    AE_STUB();
}

void ae_chunk_create_ex(ae_memory_chunk_t* chunk, const char* name, u32 block_size,
    u32 num_blocks, const char* filename, const char* funcname, const int lineno)
{
    AE_STUB();
}

void ae_chunk_destroy_ex(ae_memory_chunk_t* chunk, const char* filename,
                                const char* funcname, const int lineno)
{
    AE_STUB();
}

void* ae_block_malloc_ex(ae_memory_chunk_t* chunk, const char* filename,
                                const char* funcname, const int lineno)
{
    AE_STUB(); return NULL;
}

void* ae_block_calloc_ex(ae_memory_chunk_t* chunk, const char* filename,
                                const char* funcname, const int lineno)
{
    AE_STUB(); return NULL;
}

void ae_block_free_ex(ae_memory_chunk_t* chunk, void* p, const char* filename,
                                    const char* funcname, const int lineno )
{
    AE_STUB();
}

ae_memory_stack_t* ae_global_stack(void)
{
    AE_STUB(); return NULL;
}

void ae_stack_create_ex(ae_memory_stack_t* stack, const char* name, u32 size,
                const char* filename, const char* funcname, const int lineno)
{
    AE_STUB();
}

void ae_stack_destroy_ex(ae_memory_stack_t* stack, const char* filename,
                                const char* funcname, const int lineno)
{
    AE_STUB();
}

void* ae_stack_malloc_ex( ae_memory_stack_t * stack, size_t size,
    const char* filename, const char* funcname, const int lineno)
{
    AE_STUB(); return NULL;
}

void* ae_stack_calloc_ex(ae_memory_stack_t* stack, size_t num, size_t size,
            const char* filename, const char* funcname, const int lineno )
{
    AE_STUB(); return NULL;
}

void ae_stack_free_ex(ae_memory_stack_t* stack, void* p, size_t size,
        const char* filename, const char* funcname, const int lineno)
{
    AE_STUB();
}

void* ae_string_malloc_ex(size_t length, // length is character count
        const char* filename, const char* funcname, const int lineno)
{
    AE_STUB(); return NULL;
}

void ae_string_free_ex(void* string, // free from string chunk or heap
        const char* filename, const char* funcname, const int lineno)
{
    AE_STUB();
}

char* ae_string_copy_sized_ex(char* string, size_t length, // n chars
        const char* filename, const char* funcname, const int lineno)
{
    AE_STUB(); return NULL;
}

char* ae_string_copy_ex(char* string, // copy string to alloced block
        const char* filename, const char* funcname, const int lineno)
{
    AE_STUB(); return NULL;
}

void ae_memory_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_memory_quit(void)
{
    // AE_STUB();
}
