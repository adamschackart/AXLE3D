/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

size_t ae_file_bytes(const char* filename, int fatal)
{
    AE_STUB(); return (size_t)-1;
}

int ae_file_exists(const char* filename)
{
    AE_STUB(); return 0;
}

void ae_file_move(const char* oldname, const char* newname, int fatal)
{
    AE_STUB();
}

void ae_file_copy(const char* oldname, const char* newname, int fatal)
{
    AE_STUB();
}

void ae_file_remove(const char* filename, int fatal)
{
    AE_STUB();
}

const char* ae_conf_path(const char* org, const char* app)
{
    AE_STUB(); return "";
}

const char* ae_base_path(void)
{
    AE_STUB(); return "";
}

void ae_base_path_override(const char* name)
{
    AE_STUB();
}

const char* ae_filename_from_path_ex(const char* path, size_t* path_length)
{
    AE_STUB(); return NULL;
}

const char* ae_filename_from_path(const char* path)
{
    AE_STUB(); return NULL;
}

void ae_split_file_path(const char* path_and_name, char* path, char* name)
{
    AE_STUB();
}

const char* ae_extension_from_path_ex(const char* path, size_t* path_length)
{
    AE_STUB(); return NULL;
}

const char* ae_extension_from_path(const char* path)
{
    AE_STUB(); return NULL;
}

void ae_split_file_extension(const char* path, char* name, char* extn)
{
    AE_STUB();
}

int ae_file_read_array(ae_array_t* array, const char* filename, int fatal)
{
    AE_STUB(); return 0;
}

void* ae_file_read_stack(ae_memory_stack_t* stack, size_t* size, const char* filename, int fatal)
{
    AE_STUB(); return NULL;
}

void* ae_file_read_block(ae_memory_chunk_t* chunk, size_t* size, const char* filename, int fatal)
{
    AE_STUB(); return NULL;
}

void* ae_file_read_heap(ae_memory_heap_t* heap, size_t* size, const char* filename, int fatal)
{
    AE_STUB(); return NULL;
}

void* ae_file_read_string(size_t* size, const char* filename, int fatal)
{
    AE_STUB(); return NULL;
}

void* ae_file_read(size_t* size, const char* filename, int fatal)
{
    AE_STUB(); return NULL;
}

int ae_file_write(const char* filename, void* contents, size_t size, int fatal)
{
    AE_STUB(); return 0;
}

void ae_file_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_file_quit(void)
{
    // AE_STUB();
}
