/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
--- stb.h v2.27 - Sean's Tool Box -- public domain -- http://nothings.org/stb.h
----------------------------------------------------------------------------- */
#ifndef __AE_HASH_H__
#define __AE_HASH_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif
#ifndef __AE_MEMORY_H__
#include <ae_memory.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_hash_init(int argc, char** argv);
void ae_hash_quit(void); // private init

/*
================================================================================
 * ~~ [ hash functions ] ~~ *
--------------------------------------------------------------------------------

The typical use for these functions are to implement power-of-two hash tables.

    let N = size of table (2^n)
    let H = ae_hash_str(str)
    let S = ae_rehash_old(H) | 1

    then hash probe sequence P(i) for i=0..N-1
        P(i) = (H + S*i) & (N-1)

The idea is that H has 32 bits of hash information, but the table has only,
say, 2^20 entries so only uses 20 of the bits. Then by rehashing the original
H we get 2^12 different probe sequences for a given initial probe location.
(So it's optimal for 64K tables and its optimality decreases past that.)

OK, so I've added something that generates _two separate_ 32-bit hashes
simultaneously which should scale better to very large tables. - STB

--------------------------------------------------------------------------------
*/

// combine two hash values in a way that deals well with poor hashes
AE_DECL u32 AE_CALL ae_hash_combine(u32 lhs, u32 rhs);

#define ae_rehash_old(x) ((x) + ((x) >> 6) + ((x) >> 19))

AE_DECL u32 AE_CALL ae_hash_str(char *str); // condense string to int
AE_DECL u32 AE_CALL ae_hash_len(char *str, int len);

AE_DECL void AE_CALL ae_hash64_str(char* str, u32 hash[2]); // 64-bit
AE_DECL void AE_CALL ae_hash64_len(char* str, int len, u32 hash[2]);

AE_DECL u32 AE_CALL ae_hash_ptr(void *p);
AE_DECL u32 AE_CALL ae_rehash_new(u32 v);
AE_DECL u32 AE_CALL ae_hash_num(u32 num);

AE_DECL u32 AE_CALL ae_hash_mem(void* p, int len);

/*
================================================================================
 * ~~ [ streaming block checksums ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL u32 AE_CALL ae_crc32_block(u32 crc, const void* const buffer, const size_t len);
AE_DECL u32 AE_CALL ae_crc32(const void* const buffer, const size_t len);

AE_DECL u32 AE_CALL ae_adler32_block(u32 adler32, const void* const buffer, size_t len);
AE_DECL u32 AE_CALL ae_adler32(const void* const buffer, size_t len);

/*
================================================================================
 * ~~ [ hash table template ] ~~ *
--------------------------------------------------------------------------------
TODO: abstract table walker interface; iterate by calling a function repeatedly
TODO: iteration with function pointer callbacks; call func on each key/val pair
TODO: can we make a generic "for" loop macro? (see original stb_sdict_for macro)
TODO: implement VAL_COPY and test to ensure it's called everywhere it's needed
TODO: sorted table iteration; iterator takes a key compare function for sorting
--------------------------------------------------------------------------------

NAME, INIT_NAME, COPY_NAME
    Type name. INIT_NAME and COPY_NAME are to generate custom implementations:

    ae_strmap_t* ae_strmap_copy(ae_strmap_t* d)
    {
        // strmap_internal was passed as COPY_NAME...
        ae_strmap_t* s = ae_strmap_internal_copy(d);

        // do special custom stuff here...
        return s;
    }

KEY_TYPE, VAL_TYPE
    Types for keys and values. For example, (void*, void*) for ae_ptrmap_t.

EMPTY
    Marker for uninitialized items. It should be something that won't ever be
    inserted into the table normally, and it must also be different from DEL.

DEL
    The tombstone marker for deleted items. It should be something that won't
    ever be inserted into the table normally (like a pointer to 0xFFFFFFFF).

EXTRA_FIELDS
    Any extra struct members needed by hashtable implementations.

KEY_HASH
    The hashing function for table key slot or bucket selection.

V_COMPARE
    Expensive dereferencing "value" comparison function for keys (like strcmp).
    If you ever call V_COMPARE with EMPTY or DEL, the app will likely crash.

C_COMPARE
    Cheap pointer comparison function for keys.

SAFE
    Used to suppress V_COMPAREs if the entry in the table is DEL.

KEY_COPY
    The function to copy a key into the table, and possibly allocate space.
    Default is AE_no_copy, which just does assignment with the = operator.

MALLOC, FREE
    Allocator for table and possibly struct memory.

KEY_FREE, VAL_FREE
    Functions to delete keys and values on rehashing or deletion.

LOAD_FACTOR
    From the Java 6 HashMap documentation:

    "The load factor is a measure of how full the hash table is allowed to get
    before its capacity is automatically increased. When the number of entries in
    the hash table exceeds the product of the load factor and the current capacity,
    the hash table is rehashed (that is, internal data structures are rebuilt)
    so that the hash table has approximately twice the number of buckets.

    As a general rule, the default load factor (.75) offers a good tradeoff
    between time and space costs. Higher values decrease the space overhead but
    increase the lookup cost (reflected in most of the operations of the
    HashMap class, including get and put). The expected number of entries in the
    map and its load factor should be taken into account when setting its
    initial capacity, so as to minimize the number of rehash operations. If the
    initial capacity is greater than the maximum number of entries divided by
    the load factor, no rehash operations will ever occur."

HAS_VNULL, VNULL
    If HAS_VNULL is AE_has_vnull, the get() function is defined, which
    may return VNULL if the requested key is not in the hash table.

H_DECL, H_CALL
    Prefix and calling convention for declaration (header file).

C_DECL, C_CALL
    Prefix and calling convention for implementation (c file).

--------------------------------------------------------------------------------
*/

#define AE_SUBTYPE_NAME(N, S) ae_ ## N ## _ ## S ## _t
#define AE_TYPE_NAME(N) ae_ ## N ## _t
#define AE_FUNC_NAME(N, F) ae_ ## N ## _ ## F

#define AE_no_extra_fields
#define AE_no_free(x)
#define AE_equals(a, b) ((a) == (b))
#define AE_no_copy(x) (x)

#define AE_has_vnull(x) x
#define AE_no_vnull(x)

#define AE_no_safe_compare(x)
#define AE_safe_compare(x) x

#define AE_no_decl
#define AE_no_call

#define AE_HASH_DECL(NAME, INIT_NAME, COPY_NAME, KEY_TYPE, VAL_TYPE, EMPTY,     \
            DEL, EXTRA_FIELDS, KEY_HASH, V_COMPARE, C_COMPARE, SAFE, KEY_COPY,  \
            MALLOC, FREE, KEY_FREE, VAL_FREE, LOAD_FACTOR, HAS_VNULL, VNULL,    \
            H_DECL, H_CALL, C_DECL, C_CALL)                                     \
                                                                                \
    typedef struct AE_SUBTYPE_NAME(NAME, pair)                                  \
    {                                                                           \
        KEY_TYPE key;                                                           \
        VAL_TYPE val;                                                           \
    }                                                                           \
        AE_SUBTYPE_NAME(NAME, pair);                                            \
                                                                                \
    typedef struct AE_TYPE_NAME(NAME)                                           \
    {                                                                           \
        EXTRA_FIELDS                                                            \
        AE_SUBTYPE_NAME(NAME, pair) *table;                                     \
                                                                                \
        int count, limit, deleted;                                              \
        unsigned int mask;                                                      \
                                                                                \
        int delete_threshhold;                                                  \
        int grow_threshhold;                                                    \
        int shrink_threshhold;                                                  \
                                                                                \
        unsigned char alloced, has_empty, has_del;                              \
        VAL_TYPE ev; VAL_TYPE dv;                                               \
    }                                                                           \
        AE_TYPE_NAME(NAME);                                                     \
                                                                                \
    H_DECL void H_CALL AE_FUNC_NAME(NAME, init)(AE_TYPE_NAME(NAME)* h, int count);  \
    H_DECL AE_TYPE_NAME(NAME)* H_CALL AE_FUNC_NAME(NAME, create)(void);         \
    H_DECL void H_CALL AE_FUNC_NAME(NAME, free)(AE_TYPE_NAME(NAME)* a);         \
                                                                                \
    H_DECL int H_CALL AE_FUNC_NAME(NAME, set_ex)                                \
                    ( AE_TYPE_NAME(NAME)* a, KEY_TYPE k, VAL_TYPE  v, u32 h);   \
    H_DECL int H_CALL AE_FUNC_NAME(NAME, add_ex)                                \
                    ( AE_TYPE_NAME(NAME)* a, KEY_TYPE k, VAL_TYPE  v, u32 h);   \
    H_DECL int H_CALL AE_FUNC_NAME(NAME, update_ex)                             \
                    ( AE_TYPE_NAME(NAME)* a, KEY_TYPE k, VAL_TYPE  v, u32 h);   \
                                                                                \
    H_DECL int H_CALL AE_FUNC_NAME(NAME, set)                                   \
                    ( AE_TYPE_NAME(NAME)* a, KEY_TYPE k, VAL_TYPE  v);          \
    H_DECL int H_CALL AE_FUNC_NAME(NAME, add)                                   \
                    ( AE_TYPE_NAME(NAME)* a, KEY_TYPE k, VAL_TYPE  v);          \
    H_DECL int H_CALL AE_FUNC_NAME(NAME, update)                                \
                    ( AE_TYPE_NAME(NAME)* a, KEY_TYPE k, VAL_TYPE  v);          \
                                                                                \
    H_DECL int H_CALL AE_FUNC_NAME(NAME, remove_ex)                             \
                    ( AE_TYPE_NAME(NAME)* a, KEY_TYPE k, VAL_TYPE* v, u32 h);   \
    H_DECL int H_CALL AE_FUNC_NAME(NAME, remove)                                \
                    ( AE_TYPE_NAME(NAME)* a, KEY_TYPE k, VAL_TYPE* v);          \
                                                                                \
    H_DECL AE_TYPE_NAME(NAME)* H_CALL AE_FUNC_NAME(NAME, copy)(AE_TYPE_NAME(NAME)* a);  \
                                                                                \
    H_DECL int H_CALL AE_FUNC_NAME(NAME, get_flag_ex)                           \
                    ( AE_TYPE_NAME(NAME)* a, KEY_TYPE k, VAL_TYPE *v, u32 h);   \
    H_DECL int H_CALL AE_FUNC_NAME(NAME, get_flag)                              \
                    ( AE_TYPE_NAME(NAME)* a, KEY_TYPE k, VAL_TYPE *v);          \
                                                                                \
    H_DECL VAL_TYPE H_CALL AE_FUNC_NAME(NAME, get_ex)                           \
                        (  AE_TYPE_NAME(NAME)* a, KEY_TYPE k, u32 h);           \
    H_DECL VAL_TYPE H_CALL AE_FUNC_NAME(NAME, get)                              \
                        (  AE_TYPE_NAME(NAME)* a, KEY_TYPE k);                  \
                                                                                \
    H_DECL int H_CALL AE_FUNC_NAME(NAME, get_key_ex)                            \
                    ( AE_TYPE_NAME(NAME)* a, KEY_TYPE k, KEY_TYPE* kout, u32 h);\
    H_DECL int H_CALL AE_FUNC_NAME(NAME, get_key)                               \
                    ( AE_TYPE_NAME(NAME)* a, KEY_TYPE k, KEY_TYPE* kout);       \

#define AE_HASH_IMPL(NAME, INIT_NAME, COPY_NAME, KEY_TYPE, VAL_TYPE, EMPTY,     \
            DEL, EXTRA_FIELDS, KEY_HASH, V_COMPARE, C_COMPARE, SAFE, KEY_COPY,  \
            MALLOC, FREE, KEY_FREE, VAL_FREE, LOAD_FACTOR, HAS_VNULL, VNULL,    \
            H_DECL, H_CALL, C_DECL, C_CALL)                                     \
                                                                                \
    C_DECL void C_CALL AE_FUNC_NAME(INIT_NAME, init)(AE_TYPE_NAME(NAME)* h, int count)  \
    {                                                                           \
        int i; void* t;                                                         \
        if (count < 4) count = 4;                                               \
                                                                                \
        h->limit = count;                                                       \
        h->count = 0;                                                           \
        h->mask = count - 1;                                                    \
        h->deleted = 0;                                                         \
                                                                                \
        h->grow_threshhold = (int)(count * LOAD_FACTOR);                        \
        h->alloced = h->has_empty = h->has_del = 0;                             \
        h->delete_threshhold = (int)(count * (1 - LOAD_FACTOR) / 2);            \
                                                                                \
        if (count <= 64)                                                        \
            h->shrink_threshhold = 0;                                           \
        else                                                                    \
            h->shrink_threshhold = (int)(count * (LOAD_FACTOR / 2.25));         \
                                                                                \
        t = MALLOC (sizeof( h->table[0] ) * count);                             \
        h->table = (AE_SUBTYPE_NAME(NAME, pair)*)t;                             \
                                                                                \
        /* likely place to hit this in an unfinished platform port */           \
        ae_assert(t != NULL, "memory allocation code is stubbed out");          \
                                                                                \
        for (i = 0; i < count; ++i) { h->table[i].key = EMPTY; }                \
    }                                                                           \
                                                                                \
    C_DECL AE_TYPE_NAME(NAME)* C_CALL AE_FUNC_NAME(INIT_NAME, create)(void)     \
    {                                                                           \
        void* v = MALLOC( sizeof(AE_TYPE_NAME(NAME)) );                         \
        AE_TYPE_NAME(NAME)* h = (AE_TYPE_NAME(NAME)*)v;                         \
                                                                                \
        AE_FUNC_NAME(INIT_NAME, init)(h, 16);                                   \
        h->alloced = 1;                                                         \
                                                                                \
        return h;                                                               \
    }                                                                           \
                                                                                \
    C_DECL void C_CALL AE_FUNC_NAME(INIT_NAME, free)(AE_TYPE_NAME(NAME)* a)     \
    {                                                                           \
        int i = 0;                                                              \
        for (; i < a->limit; ++i)                                               \
        {                                                                       \
            if(!C_COMPARE(a->table[i].key, EMPTY)                               \
            && !C_COMPARE(a->table[i].key, DEL ))                               \
            {                                                                   \
                KEY_FREE(a->table[i].key);                                      \
                VAL_FREE(a->table[i].val);                                      \
            }                                                                   \
        }                                                                       \
        FREE(a->table);                                                         \
        if (a->alloced) FREE(a);                                                \
    }                                                                           \
                                                                                \
    static void AE_FUNC_NAME(NAME, rehash)(AE_TYPE_NAME(NAME)* a, int count);   \
                                                                                \
    static int                                                                  \
    AE_FUNC_NAME(NAME, add_set_ex)(AE_TYPE_NAME(NAME) * a, KEY_TYPE k,          \
            VAL_TYPE v, int allow_new, int allow_old, int copy, u32 h)          \
    {                                                                           \
        unsigned int n = h & a->mask;                                           \
        int b = -1;                                                             \
                                                                                \
        ae_assert(a->table, "uninitialized hashtable access (insertion)");      \
                                                                                \
        if (C_COMPARE(k, EMPTY))                                                \
        {                                                                       \
            if (a->has_empty ? allow_old : allow_new)                           \
            {                                                                   \
                n=a->has_empty;                                                 \
                a->ev = v;                                                      \
                a->has_empty = 1;                                               \
                return !n;                                                      \
            }                                                                   \
            else                                                                \
            {                                                                   \
                return 0;                                                       \
            }                                                                   \
        }                                                                       \
        if (C_COMPARE(k, DEL))                                                  \
        {                                                                       \
            if (a->has_del ? allow_old : allow_new)                             \
            {                                                                   \
                n=a->has_del;                                                   \
                a->dv = v;                                                      \
                a->has_del = 1;                                                 \
                return !n;                                                      \
            }                                                                   \
            else                                                                \
            {                                                                   \
                return 0;                                                       \
            }                                                                   \
        }                                                                       \
                                                                                \
        if (!C_COMPARE(a->table[n].key, EMPTY))                                 \
        {                                                                       \
            unsigned int s;                                                     \
                                                                                \
            if (C_COMPARE(a->table[n].key, DEL)) { b = n; }                     \
            else if (V_COMPARE(a->table[n].key, k))                             \
            {                                                                   \
                if (allow_old)                                                  \
                {                                                               \
                    a->table[n].val = v;                                        \
                }                                                               \
                else                                                            \
                {                                                               \
                    return !allow_new;                                          \
                }                                                               \
            }                                                                   \
            s = ae_rehash_old(h) | 1;                                           \
                                                                                \
            for (;;)                                                            \
            {                                                                   \
                n = (n + s) & a->mask;                                          \
                                                                                \
                if (C_COMPARE(a->table[n].key, EMPTY)) break;                   \
                if (C_COMPARE(a->table[n].key, DEL  ))                          \
                {                                                               \
                    if (b < 0) b = n;                                           \
                }                                                               \
                else if (V_COMPARE(a->table[n].key, k))                         \
                {                                                               \
                    if (allow_old)                                              \
                    {                                                           \
                        a->table[n].val = v;                                    \
                    }                                                           \
                    else                                                        \
                    {                                                           \
                        return !allow_new;                                      \
                    }                                                           \
                }                                                               \
            }                                                                   \
        }                                                                       \
                                                                                \
        if (!allow_new) return 0;                                               \
        if (b < 0) b = n; else --a->deleted;                                    \
                                                                                \
        a->table[b].key = copy ? KEY_COPY(k) : k;                               \
        a->table[b].val = v;                                                    \
                                                                                \
        if (++a->count > a->grow_threshhold)                                    \
        {                                                                       \
            AE_FUNC_NAME(NAME, rehash)(a, a->limit * 2);                        \
        }                                                                       \
                                                                                \
        return 1;                                                               \
    }                                                                           \
                                                                                \
    static int                                                                  \
    AE_FUNC_NAME(NAME, add_set)(AE_TYPE_NAME(NAME) * a, KEY_TYPE k,             \
                VAL_TYPE v, int allow_new, int allow_old, int copy)             \
    {                                                                           \
        return AE_FUNC_NAME(NAME, add_set_ex)(a, k, v,                          \
            allow_new, allow_old, copy, KEY_HASH(k) );                          \
    }                                                                           \
                                                                                \
    static void AE_FUNC_NAME(NAME, rehash)(AE_TYPE_NAME(NAME)* a, int count)    \
    {                                                                           \
        int i = 0;                                                              \
                                                                                \
        AE_TYPE_NAME(NAME) b; /* could we avoid a temp copy? */                 \
        AE_FUNC_NAME(NAME, init)(&b, count);                                    \
                                                                                \
        for (; i < a->limit; ++i)                                               \
        {                                                                       \
            if(!C_COMPARE(a->table[i].key, EMPTY)                               \
            && !C_COMPARE(a->table[i].key, DEL ))                               \
            {                                                                   \
                AE_FUNC_NAME(NAME, add_set)(&b, a->table[i].key,                \
                                    a->table[i].val, 1, 1, 0 );                 \
            }                                                                   \
        }                                                                       \
                                                                                \
        FREE(a->table);                                                         \
                                                                                \
        a->table = b.table;                                                     \
        a->mask = b.mask;                                                       \
        a->count = b.count;                                                     \
        a->limit = b.limit;                                                     \
        a->deleted = b.deleted;                                                 \
                                                                                \
        a->delete_threshhold = b.delete_threshhold;                             \
        a->grow_threshhold = b.grow_threshhold;                                 \
        a->shrink_threshhold = b.shrink_threshhold;                             \
    }                                                                           \
                                                                                \
    C_DECL int C_CALL AE_FUNC_NAME(NAME, set_ex)(AE_TYPE_NAME(NAME)* a,         \
                                        KEY_TYPE k, VAL_TYPE v, u32 h )         \
    {                                                                           \
        return AE_FUNC_NAME(NAME, add_set_ex)(a, k, v, 1, 1, 1, h);             \
    }                                                                           \
                                                                                \
    C_DECL int C_CALL AE_FUNC_NAME(NAME, add_ex)(AE_TYPE_NAME(NAME)* a,         \
                                        KEY_TYPE k, VAL_TYPE v, u32 h )         \
    {                                                                           \
        return AE_FUNC_NAME(NAME, add_set_ex)(a, k, v, 1, 0, 1, h);             \
    }                                                                           \
                                                                                \
    C_DECL int C_CALL AE_FUNC_NAME(NAME, update_ex)(AE_TYPE_NAME(NAME)* a,      \
                                            KEY_TYPE k,VAL_TYPE v,u32 h )       \
    {                                                                           \
        return AE_FUNC_NAME(NAME, add_set_ex)(a, k, v, 0, 1, 1, h);             \
    }                                                                           \
                                                                                \
    C_DECL int C_CALL AE_FUNC_NAME(NAME, set)(AE_TYPE_NAME(NAME)* a,            \
                                            KEY_TYPE k, VAL_TYPE v )            \
    {                                                                           \
        return AE_FUNC_NAME(NAME, add_set)(a, k, v, 1, 1, 1);                   \
    }                                                                           \
                                                                                \
    C_DECL int C_CALL AE_FUNC_NAME(NAME, add)(AE_TYPE_NAME(NAME)* a,            \
                                            KEY_TYPE k, VAL_TYPE v )            \
    {                                                                           \
        return AE_FUNC_NAME(NAME, add_set)(a, k, v, 1, 0, 1);                   \
    }                                                                           \
                                                                                \
    C_DECL int C_CALL AE_FUNC_NAME(NAME, update)(AE_TYPE_NAME(NAME)* a,         \
                                                KEY_TYPE k,VAL_TYPE v )         \
    {                                                                           \
        return AE_FUNC_NAME(NAME, add_set)(a, k, v, 0, 1, 1);                   \
    }                                                                           \
                                                                                \
    C_DECL int C_CALL AE_FUNC_NAME(NAME, remove_ex)(AE_TYPE_NAME(NAME) * a,     \
                                            KEY_TYPE k, VAL_TYPE* v, u32 h)     \
    {                                                                           \
        unsigned int n = h & a->mask, s; /* make sure users call init */        \
        ae_assert(a->table, "uninitialized hashtable access (removal)");        \
                                                                                \
        if (C_COMPARE(k, EMPTY))                                                \
        {                                                                       \
            if (a->has_empty)                                                   \
            {                                                                   \
                if (v) *v = a->ev; a->has_empty = 0; return 1;                  \
            }                                                                   \
            return 0;                                                           \
        }                                                                       \
        if (C_COMPARE(k, DEL))                                                  \
        {                                                                       \
            if (a->has_del)                                                     \
            {                                                                   \
                if (v) *v = a->dv; a->has_del = 0; return 1;                    \
            }                                                                   \
            return 0;                                                           \
        }                                                                       \
        if (C_COMPARE(a->table[n].key, EMPTY)) { return 0; }                    \
                                                                                \
        if (SAFE(C_COMPARE(a->table[n].key, DEL) ||) !V_COMPARE(a->table[n].key, k))\
        {                                                                       \
            s = ae_rehash_old(h) | 1;                                           \
            for(;;)                                                             \
            {                                                                   \
                n = (n + s) & a->mask; /* scan to resolve collisions */         \
                if (C_COMPARE(a->table[n].key, EMPTY)) return 0;                \
                                                                                \
                SAFE(if (C_COMPARE(a->table[n].key, DEL)) continue;)            \
                if (V_COMPARE(a->table[n].key, k)) break;                       \
            }                                                                   \
        }                                                                       \
                                                                                \
        KEY_FREE(a->table[n].key);                                              \
        a->table[n].key = DEL;                                                  \
                                                                                \
        --a->count;                                                             \
        ++a->deleted;                                                           \
                                                                                \
        if (v != NULL) *v = a->table[n].val;                                    \
        if (a->count < a->shrink_threshhold)                                    \
        {                                                                       \
            AE_FUNC_NAME(NAME, rehash)(a, a->limit >> 1);                       \
        }                                                                       \
        else if (a->deleted > a->delete_threshhold)                             \
        {                                                                       \
            AE_FUNC_NAME(NAME, rehash)(a, a->limit);                            \
        }                                                                       \
        return 1;                                                               \
    }                                                                           \
                                                                                \
    C_DECL int C_CALL AE_FUNC_NAME(NAME, remove)(AE_TYPE_NAME(NAME) * a,        \
                                                KEY_TYPE k, VAL_TYPE* v)        \
    {                                                                           \
        return AE_FUNC_NAME(NAME, remove_ex)(a, k, v, KEY_HASH(k));             \
    }                                                                           \
                                                                                \
    C_DECL AE_TYPE_NAME(NAME)* C_CALL AE_FUNC_NAME(COPY_NAME, copy)(AE_TYPE_NAME(NAME)* a)  \
    {                                                                           \
        AE_TYPE_NAME(NAME) *h = (AE_TYPE_NAME(NAME)*)MALLOC(sizeof(*h));        \
        int i = 0;                                                              \
        AE_FUNC_NAME(NAME, init)(h, a->limit);                                  \
                                                                                \
        h->count = a->count;                                                    \
        h->deleted = a->deleted;                                                \
        h->alloced = 1;                                                         \
        h->ev = a->ev;                                                          \
        h->dv = a->dv;                                                          \
        h->has_empty = a->has_empty;                                            \
        h->has_del = a->has_del;                                                \
                                                                                \
        memcpy(h->table, a->table, h->limit * sizeof(h->table[0]));             \
        for (; i < a->limit; ++i)                                               \
        {                                                                       \
            if(!C_COMPARE(h->table[i].key, EMPTY)                               \
            && !C_COMPARE(h->table[i].key, DEL ))                               \
            {                                                                   \
                h->table[i].key = KEY_COPY(h->table[i].key);                    \
                                                                                \
            }                                                                   \
        }                                                                       \
        return h;                                                               \
    }                                                                           \
                                                                                \
    C_DECL int C_CALL AE_FUNC_NAME(NAME, get_flag_ex)(AE_TYPE_NAME(NAME) *a,    \
                                            KEY_TYPE k, VAL_TYPE *v, u32 h )    \
    {                                                                           \
        unsigned int n = h & a->mask, s; /* make sure user calls init */        \
        ae_assert(a->table, "uninitialized hashtable access (get val)");        \
                                                                                \
        if (C_COMPARE(k, EMPTY))                                                \
        {                                                                       \
            if (a->has_empty) *v = a->ev; return a->has_empty;                  \
        }                                                                       \
        if (C_COMPARE(k, DEL))                                                  \
        {                                                                       \
            if (a->has_del) *v = a->dv; return a->has_del;                      \
        }                                                                       \
                                                                                \
        if (C_COMPARE(a->table[n].key, EMPTY)) return 0;                        \
                                                                                \
        SAFE(if (!C_COMPARE(a->table[n].key, DEL)))                             \
        if (V_COMPARE(a->table[n].key, k))                                      \
        {                                                                       \
            *v = a->table[n].val; return 1;                                     \
        }                                                                       \
                                                                                \
        s = ae_rehash_old(h) | 1;                                               \
        for (;;)                                                                \
        {                                                                       \
            n = (n + s) & a->mask; /* scan to resolve collisions */             \
            if (C_COMPARE(a->table[n].key, EMPTY)) return 0;                    \
                                                                                \
            SAFE(if (C_COMPARE(a->table[n].key, DEL)) continue;)                \
            if (V_COMPARE(a->table[n].key, k))                                  \
            {                                                                   \
                *v = a->table[n].val; return 1;                                 \
            }                                                                   \
        }                                                                       \
    }                                                                           \
                                                                                \
    C_DECL int C_CALL AE_FUNC_NAME(NAME, get_flag)(AE_TYPE_NAME(NAME) *a,       \
                                                KEY_TYPE k, VAL_TYPE *v )       \
    {                                                                           \
        return AE_FUNC_NAME(NAME, get_flag_ex)(a, k, v, KEY_HASH(k));           \
    }                                                                           \
                                                                                \
    HAS_VNULL(                                                                  \
        C_DECL VAL_TYPE C_CALL AE_FUNC_NAME(NAME, get_ex)(AE_TYPE_NAME(NAME)* a,\
                                                            KEY_TYPE k, u32 h ) \
        {                                                                       \
            VAL_TYPE v;                                                         \
            if (AE_FUNC_NAME(NAME, get_flag_ex)(a,k,&v, h)) return v;           \
            else                                            return VNULL;       \
        }                                                                       \
                                                                                \
        C_DECL VAL_TYPE C_CALL AE_FUNC_NAME(NAME, get)(AE_TYPE_NAME(NAME)* a,   \
                                                                KEY_TYPE k )    \
        {                                                                       \
            VAL_TYPE v;                                                         \
            if (AE_FUNC_NAME(NAME, get_flag)(a, k, &v)) return v;               \
            else                                        return VNULL;           \
        }                                                                       \
    );                                                                          \
                                                                                \
    C_DECL int C_CALL AE_FUNC_NAME(NAME, get_key_ex)(AE_TYPE_NAME(NAME)* a,     \
                                        KEY_TYPE k, KEY_TYPE* kout, u32 h )     \
    {                                                                           \
        unsigned int n = h & a->mask, s; /* make sure user calls init */        \
        ae_assert(a->table, "uninitialized hashtable access (get key)");        \
                                                                                \
        if (C_COMPARE(k, EMPTY) || C_COMPARE(k, DEL)) return 0;                 \
        if (C_COMPARE(a->table[n].key, EMPTY)) return 0;                        \
                                                                                \
        SAFE(if (!C_COMPARE(a->table[n].key, DEL)))                             \
        if (V_COMPARE(a->table[n].key, k))                                      \
        {                                                                       \
            *kout = a->table[n].key; return 1;                                  \
        }                                                                       \
                                                                                \
        s = ae_rehash_old(h) | 1;                                               \
        for (;;)                                                                \
        {                                                                       \
            n = (n + s) & a->mask; /* scan to resolve collisions */             \
            if (C_COMPARE(a->table[n].key, EMPTY)) return 0;                    \
                                                                                \
            SAFE(if (C_COMPARE(a->table[n].key, DEL)) continue;)                \
            if (V_COMPARE(a->table[n].key, k))                                  \
            {                                                                   \
                *kout = a->table[n].key; return 1;                              \
            }                                                                   \
        }                                                                       \
    }                                                                           \
                                                                                \
    C_DECL int C_CALL AE_FUNC_NAME(NAME, get_key)(AE_TYPE_NAME(NAME)* a,        \
                                            KEY_TYPE k, KEY_TYPE* kout)         \
    {                                                                           \
        return AE_FUNC_NAME(NAME, get_key_ex)(a, k, kout, KEY_HASH(k));         \
    }                                                                           \

/*
================================================================================
 * ~~ [ string dictionaries (symbol tables) ] ~~ *
--------------------------------------------------------------------------------
*/

#define AE_STRMAP_APPLY_ARGS(F) F( \
    strmap, \
    strmap, \
    strmap, \
    char*, \
    void*, \
    NULL, \
    ((char*)1), \
    AE_no_extra_fields, \
    ae_hash_str, \
    !strcmp, \
    AE_equals, \
    AE_safe_compare, \
    ae_string_copy, \
    ae_malloc, \
    ae_free, \
    ae_string_free, \
    AE_no_free, \
    0.85f, \
    AE_has_vnull, \
    NULL, \
    AE_DECL, \
    AE_CALL, \
    AE_no_decl, \
    AE_no_call);

AE_STRMAP_APPLY_ARGS(AE_HASH_DECL);

/*
================================================================================
 * ~~ [ pointer -> pointer mappings ] ~~ *
--------------------------------------------------------------------------------
TODO: allow the user to set key/val deletion callbacks at runtime (extra fields)
--------------------------------------------------------------------------------
*/

#define AE_PTRMAP_APPLY_ARGS(F) F( \
    ptrmap, \
    ptrmap, \
    ptrmap, \
    void*, \
    void*, \
    ((void*)2), \
    ((void*)6), \
    AE_no_extra_fields, \
    ae_hash_ptr, \
    AE_equals, \
    AE_equals, \
    AE_no_safe_compare, \
    AE_no_copy, \
    ae_malloc, \
    ae_free, \
    AE_no_free, \
    AE_no_free, \
    0.85f, \
    AE_has_vnull, \
    NULL, \
    AE_DECL, \
    AE_CALL, \
    AE_no_decl, \
    AE_no_call);

AE_PTRMAP_APPLY_ARGS(AE_HASH_DECL);

/*
================================================================================
 * ~~ [ pointer set ] ~~ *
--------------------------------------------------------------------------------
*/

typedef ae_ptrmap_t ae_ptrset_t; // TODO: make automatically inside macro

#define ae_ptrset_init ae_ptrmap_init
#define ae_ptrset_create ae_ptrmap_create
#define ae_ptrset_free ae_ptrmap_free

#define ae_ptrset_add(set, ptr) ae_ptrmap_add((set), (ptr), ((void*)1))
#define ae_ptrset_remove(set, ptr) ae_ptrmap_remove((set), (ptr), NULL)

#define ae_ptrset_copy ae_ptrmap_copy
#define ae_ptrset_contains(set, ptr) (ae_ptrmap_get((set), (ptr)) != NULL)

static void ae_ptrset_list(ae_ptrset_t* set, void** list)
{
    int i, j;
    void * k;

    for (i = 0, j = 0; i < set->limit ? k = set->table[i].key, 1 : 0; i++)
    {
        if (k > (void*)6) { list[j++] = k; }
    }

    ae_assert(j == set->count, "%i %i", j, set->count);
}

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_HASH_H__ */
