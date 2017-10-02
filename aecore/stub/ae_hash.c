/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

u32 ae_hash_combine(u32 lhs, u32 rhs)
{
    AE_STUB(); return 0;
}

u32 ae_hash_str(char *str)
{
    AE_STUB(); return 0;
}

u32 ae_hash_len(char *str, int len)
{
    AE_STUB(); return 0;
}

void ae_hash64_str(char* str, u32 hash[2])
{
    AE_STUB();
}

void ae_hash64_len(char* str, int len, u32 hash[2])
{
    AE_STUB();
}

u32 ae_hash_ptr(void *p)
{
    AE_STUB(); return 0;
}

u32 ae_rehash_new(u32 v)
{
    AE_STUB(); return 0;
}

u32 ae_hash_num(u32 num)
{
    AE_STUB(); return 0;
}

u32 ae_hash_mem(void* p, int len)
{
    AE_STUB(); return 0;
}

u32 ae_crc32_block(u32 crc, const void* const buffer, const size_t len)
{
    AE_STUB(); return 0;
}

u32 ae_crc32(const void* const buffer, const size_t len)
{
    AE_STUB(); return 0;
}

u32 ae_adler32_block(u32 adler32, const void* const buffer, size_t len)
{
    AE_STUB(); return 0;
}

u32 ae_adler32(const void* const buffer, size_t len)
{
    AE_STUB(); return 0;
}

void ae_strmap_init(ae_strmap_t* h, int count)
{
    AE_STUB();
}

ae_strmap_t* ae_strmap_create(void)
{
    AE_STUB(); return NULL;
}

void ae_strmap_free(ae_strmap_t* a)
{
    AE_STUB();
}

int ae_strmap_set_ex(ae_strmap_t* a, char* k, void* v, u32 h)
{
    AE_STUB(); return 0;
}

int ae_strmap_add_ex(ae_strmap_t* a, char* k, void* v, u32 h)
{
    AE_STUB(); return 0;
}

int ae_strmap_update_ex(ae_strmap_t* a, char* k, void* v, u32 h)
{
    AE_STUB(); return 0;
}

int ae_strmap_set(ae_strmap_t* a, char* k, void* v)
{
    AE_STUB(); return 0;
}

int ae_strmap_add(ae_strmap_t* a, char* k, void* v)
{
    AE_STUB(); return 0;
}

int ae_strmap_update(ae_strmap_t* a, char* k, void* v)
{
    AE_STUB(); return 0;
}

int ae_strmap_remove_ex(ae_strmap_t* a, char* k, void** v, u32 h)
{
    AE_STUB(); return 0;
}

int ae_strmap_remove(ae_strmap_t* a, char* k, void** v)
{
    AE_STUB(); return 0;
}

ae_strmap_t* ae_strmap_copy(ae_strmap_t* a)
{
    AE_STUB(); return NULL;
}

int ae_strmap_get_flag_ex(ae_strmap_t* a, char* k, void** v, u32 h)
{
    AE_STUB(); return 0;
}

int ae_strmap_get_flag(ae_strmap_t* a, char* k, void** v)
{
    AE_STUB(); return 0;
}

void* ae_strmap_get_ex(ae_strmap_t* a, char* k, u32 h)
{
    AE_STUB(); return NULL;
}

void* ae_strmap_get(ae_strmap_t* a, char* k)
{
    AE_STUB(); return NULL;
}

int ae_strmap_get_key_ex(ae_strmap_t* a, char* k, char** kout, u32 h)
{
    AE_STUB(); return 0;
}

int ae_strmap_get_key(ae_strmap_t* a, char* k, char** kout)
{
    AE_STUB(); return 0;
}

void ae_ptrmap_init(ae_ptrmap_t* h, int count)
{
    AE_STUB();
}

ae_ptrmap_t* ae_ptrmap_create(void)
{
    AE_STUB(); return NULL;
}

void ae_ptrmap_free(ae_ptrmap_t* a)
{
    AE_STUB();
}

int ae_ptrmap_set_ex(ae_ptrmap_t* a, void* k, void* v, u32 h)
{
    AE_STUB(); return 0;
}

int ae_ptrmap_add_ex(ae_ptrmap_t* a, void* k, void* v, u32 h)
{
    AE_STUB(); return 0;
}

int ae_ptrmap_update_ex(ae_ptrmap_t* a, void* k, void* v, u32 h)
{
    AE_STUB(); return 0;
}

int ae_ptrmap_set(ae_ptrmap_t* a, void* k, void* v)
{
    AE_STUB(); return 0;
}

int ae_ptrmap_add(ae_ptrmap_t* a, void* k, void* v)
{
    AE_STUB(); return 0;
}

int ae_ptrmap_update(ae_ptrmap_t* a, void* k, void* v)
{
    AE_STUB(); return 0;
}

int ae_ptrmap_remove_ex(ae_ptrmap_t* a, void* k, void** v, u32 h)
{
    AE_STUB(); return 0;
}

int ae_ptrmap_remove(ae_ptrmap_t* a, void* k, void** v)
{
    AE_STUB(); return 0;
}

ae_ptrmap_t* ae_ptrmap_copy(ae_ptrmap_t* a)
{
    AE_STUB(); return NULL;
}

int ae_ptrmap_get_flag_ex(ae_ptrmap_t* a, void* k, void** v, u32 h)
{
    AE_STUB(); return 0;
}

int ae_ptrmap_get_flag(ae_ptrmap_t* a, void* k, void** v)
{
    AE_STUB(); return 0;
}

void* ae_ptrmap_get_ex(ae_ptrmap_t* a, void* k, u32 h)
{
    AE_STUB(); return NULL;
}

void* ae_ptrmap_get(ae_ptrmap_t* a, void* k)
{
    AE_STUB(); return NULL;
}

int ae_ptrmap_get_key_ex(ae_ptrmap_t* a, void* k, void** kout, u32 h)
{
    AE_STUB(); return 0;
}

int ae_ptrmap_get_key(ae_ptrmap_t* a, void* k, void** kout)
{
    AE_STUB(); return 0;
}

void ae_hash_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_hash_quit(void)
{
    // AE_STUB();
}
