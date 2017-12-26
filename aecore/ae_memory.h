/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_MEMORY_H__
#define __AE_MEMORY_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif
#ifndef __AE_ERROR_H__
#include <ae_error.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_memory_init(int argc, char** argv);
void ae_memory_quit(void); // private init

// we use size_t as a uintptr type throughout this entire codebase.
AE_STATIC_ASSERT(pointer_size_t, sizeof(size_t) == sizeof(void *));

// returns a pointer to a copy of argv we make at init time.
AE_DECL char** AE_CALL ae_argv(int* argc);

/*
================================================================================
 * ~~ [ fixed-size scalar types ] ~~ *
--------------------------------------------------------------------------------
*/

typedef   signed char       s8;
typedef unsigned char       u8;

typedef   signed short      s16;
typedef unsigned short      u16;

typedef   signed int        s32;
typedef unsigned int        u32;

typedef   signed long long  s64;
typedef unsigned long long  u64;

AE_STATIC_ASSERT(u8 , sizeof(u8 ) == 1);
AE_STATIC_ASSERT(u16, sizeof(u16) == 2);
AE_STATIC_ASSERT(u32, sizeof(u32) == 4);
AE_STATIC_ASSERT(u64, sizeof(u64) == 8);

/* ===== [ type value ranges ] ============================================== */

#define AE_U8_MAX  ((u8 )(-1))
#define AE_U16_MAX ((u16)(-1))
#define AE_U32_MAX ((u32)(-1))
#define AE_U64_MAX ((u64)(-1))

#define AE_S8_MIN  (-AE_S8_MAX  - 1)
#define AE_S16_MIN (-AE_S16_MAX - 1)
#define AE_S32_MIN (-AE_S32_MAX - 1)
#define AE_S64_MIN (-AE_S64_MAX - 1)

#define AE_S8_MAX  ((s8 )(0x7F))
#define AE_S16_MAX ((s16)(0x7FFF))
#define AE_S32_MAX ((s32)(0x7FFFFFFF))
#define AE_S64_MAX ((s64)(0x7FFFFFFFFFFFFFFF))

/* ===== [ integer unions ] ================================================= */

typedef union ae_integer16_t
{
    s8 as_s8[2];
    u8 as_u8[2];

    s16 as_s16[1];
    u16 as_u16[1];

    u16 u_value;
    s16 s_value;
} \
    ae_integer16_t;

AE_STATIC_ASSERT(ae_integer16_t, sizeof(ae_integer16_t) == sizeof(u16));

typedef union ae_integer32_t
{
    s8 as_s8[4];
    u8 as_u8[4];

    s16 as_s16[2];
    u16 as_u16[2];

    s32 as_s32[1];
    u32 as_u32[1];

    u32 u_value;
    s32 s_value;

    float as_flt[1];
    float f_value;
} \
    ae_integer32_t;

AE_STATIC_ASSERT(ae_integer32_t, sizeof(ae_integer32_t) == sizeof(u32));

typedef union ae_integer64_t
{
    s8 as_s8[8];
    u8 as_u8[8];

    s16 as_s16[4];
    u16 as_u16[4];

    s32 as_s32[2];
    u32 as_u32[2];

    s64 as_s64[1];
    u64 as_u64[1];

    u64 u_value;
    s64 s_value;

    float  as_flt[2];
    double as_dbl[1];
    double f_value;
} \
    ae_integer64_t;

AE_STATIC_ASSERT(ae_integer64_t, sizeof(ae_integer64_t) == sizeof(u64));

/*
================================================================================
 * ~~ [ misc. utilities ] ~~ *
--------------------------------------------------------------------------------
*/

#define AE_ARRAY_COUNT(a) (sizeof(a) / sizeof((a)[0]))

/* create an instance of an empty struct inline, so we don't need a memset() */
#if defined(_MSC_VER)
    #define AE_ZERO_STRUCT {0}
#else
    #define AE_ZERO_STRUCT {}
#endif

/* copies the contents of A into B and B into A (this is not an endian swap) */
AE_DECL void AE_CALL memswap(void* a_ptr, void* b_ptr, const size_t bytes);

/* return a true value if all bytes in a block of memory are cleared to zero */
AE_DECL int AE_CALL memiszero(const void* const ptr, const size_t bytes);

/* reverse the elements of an array in-place. element size must not be zero. */
AE_DECL void AE_CALL memreverse(void* base, size_t e_count, size_t e_size);

/* element passes filter function? copy into dst, and increment return value */
AE_DECL size_t AE_CALL memfilter(void* dst, const void* const src, size_t num,
            size_t size, int (*filter)(const void*, const void*), void* user);

static c_inline int /* determine if pointer overlaps memory block or pointer */
ptr_in_array(const void* const ptr, const void* const array, const size_t size)
{
    return size ? ptr >= array && (const char* const)ptr
        < (const char* const)array + size : ptr == array;
}

#define zero_byte_in_s32 zero_byte_in_u32 /* via paul hsieh's assembly lab */
#define zero_byte_in_u32(x) (((x) - 0x01010101) & (~(x) & 0x80808080))

/* what we should expect pointers returned by malloc etc. to be aligned by */
#define AE_HEAP_ALIGNMENT (sizeof(void*) * 2)

/* determine if a size (or pointer address) is evenly divisible by a value */
#define num_is_aligned(number, alignment) ( !((number) & ((alignment) - 1)) )

/* round up to the nearest multiple of align (which must be a power of 2!) */
#define num_pow2_align(num, align) (((num) + ((align) - 1)) & ~((align) - 1))

/*
================================================================================
 * ~~ [ string formatting ] ~~ *
--------------------------------------------------------------------------------
TODO: create ae_string.(h/c) and move this stuff into it, along with allocation
--------------------------------------------------------------------------------
*/

#define AE_SNPRINTF(buf, fmt, ...) ae_snprintf((buf), sizeof(buf), (fmt), __VA_ARGS__)
#define AE_VSNPRINTF(buf, fmt, a) ae_vsnprintf((buf), sizeof(buf), (fmt), (a))

/* MSVC doesn't provide a standard version of these, so we must define our own.
 * TODO: proper GCC/Clang attributes (do we need to #define AE_SPRINTF_ARGS?)
 */
AE_DECL int AE_CALL ae_vsnprintf(char* buf, size_t size, const char* fmt, va_list args);
AE_DECL int AE_CALL ae_snprintf(char* buf, size_t size, const char* fmt, ...);

/* Not all sized string copy implementations write a terminating null char
 * in case of truncation - they also don't check for NULL string pointers.
 * snprintf is the superset of these functions, and should be used instead.
 */
#define AE_STRNCPY(dst, src) ae_strncpy((dst), (src), (sizeof(dst) - 1))
AE_DECL char* AE_CALL ae_strncpy(char* dst, const char* src, size_t num);

#define AE_STRNCAT(dst, src) ae_strncat((dst), (src), (sizeof(src) - 1))
AE_DECL char* AE_CALL ae_strncat(char* dst, const char* src, size_t num);

/* Compute the size (in chars) of a string to be displayed. Non-printable
 * characters are ignored. Useful for rendering font strings to bitmaps.
 */
AE_DECL void AE_CALL ae_strnbounds( const char* string, size_t length,
                        size_t tab_width, size_t* cols, size_t* rows);

AE_DECL void AE_CALL
ae_strbounds(const char* str, size_t tab_width, size_t* cols, size_t* rows);

/*
================================================================================
 * ~~ [ powers of 2 ] ~~ *
--------------------------------------------------------------------------------

In 12 operations, this code computes the next highest power of 2 for a 32-bit
integer. The result may be expressed by the formula 1U << (lg(v - 1) + 1).

Note that in the edge case where v is 0, it returns 0, which isn't a power of 2;
you might append the expression v += (v == 0) to remedy this if it matters.

It would be faster by 2 operations to use the formula and the log base 2 method
that uses a lookup table, but in some situations, lookup tables are not suitable,
so the above code may be best. (On an Athlon XP 2100+ I've found the above
shift-left and then OR code is as fast as using a single BSR assembly language
instruction, which scans in reverse to find the highest set bit.) It works by
copying the highest set bit to all of the lower bits, and then adding one, which
results in carries that set all of the lower bits to 0 and one bit beyond the
highest set bit to 1. If the original number was a power of 2, then the decrement
will reduce it to one less, so that we round up to the same original value.

You might alternatively compute the next higher power of 2 in only 8 or 9 operations
using a lookup table for floor(lg(v)) and then evaluating 1<<(1+floor(lg(v))).

    - https://graphics.stanford.edu/~seander/bithacks.html

--------------------------------------------------------------------------------
*/

static c_inline int num_is_pow2(u32 n) { return (n & (n - 1)) == 0; }

static c_inline u32 num_prev_pow2(u32 x)
{
    ae_assert(x != 0, "zero num_prev_pow2 val");

    x |= x >> 0x00000001;
    x |= x >> 0x00000002;
    x |= x >> 0x00000004;
    x |= x >> 0x00000008;
    x |= x >> 0x00000010;

    x -= (x >> 1);
    ae_assert(num_is_pow2(x), "non-pow2 %u", x);

    return x;
}

static c_inline u32 num_next_pow2(u32 x)
{
    ae_assert(x != 0, "zero num_next_pow2 val");
    x--;

    x |= x >> 0x00000001;
    x |= x >> 0x00000002;
    x |= x >> 0x00000004;
    x |= x >> 0x00000008;
    x |= x >> 0x00000010;

    x++;
    ae_assert(num_is_pow2(x), "non-pow2 %u", x);

    return x;
}

/*
================================================================================
 * ~~ [ bits ] ~~ *
--------------------------------------------------------------------------------
*/

// Fill the N bottom bits of an integer with ones, and the rest with zeroes.
// This can be used to AND a range of bits, ignoring all other values as 0.
#define AE_FILL_MASK(bits) (~(~0 << (bits)))

// Given integer N, return an integer with just bit N set to one.
#define AE_IDX2BIT(x) (1 << (x))

static c_inline size_t ae_idx2bit(const size_t X) { return AE_IDX2BIT(X); }
static c_inline size_t ae_bit2idx(const size_t X)
{
    /* This function returns the shift value from a power-of-two integer.
     */
    #if defined(_MSC_VER) && _MSC_VER >= 1300 && 0
        if (X)
        {
            DWORD J;
            _BitScanForward(&J, X);
            return (size_t)J;
        }
        else return 0;

    #elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
        if (X)
        {
            return (size_t)__builtin_ctz(X);
        }
        else return 0;

    #else
        size_t K, N = 0, Y = X - 1;

        N += K = Y >> (16 - 4) & 16; Y >>= K;
        N += K = Y >> ( 8 - 3) &  8; Y >>= K;
        N += K = Y >> ( 4 - 2) &  4; Y >>= K;
        N += K = Y >> ( 2 - 1) &  2; Y >>= K;
        N += K = Y >> ( 1 - 0) &  1; Y >>= K;

        return N + Y;
    #endif
}

/*  Returns a tribool (-1, 0, or +1) depending on the state of some bits.
    Looks at the plus and minus bits of flags, and subtracts their status
    to give a +1, -1 or zero result. Useful for directional button flags.

    flags:  Value with bit mask.
    plus :  Bit index for positive result.
    minus:  Bit index for negative result.

    returns +1 if plus bit is set but minus bit isn't,
            -1 if minus bit is set and plus bit isn't,
            or 0 if neither or both are set.
*/
static c_inline int ae_tribool(size_t flags, size_t negative, size_t positive)
{
    return ((flags >> positive) & 1) - ((flags >> negative) & 1);
}

/* These functions manage arrays of bits, which are divided into byte-sized chunks.
 * The compiler should fold these pow2 divisions and modulos into shifts and masks.
 */
static c_inline size_t ae_bitvector_bytes(const size_t num_bits)
{
    return (num_bits + 7) / 8;
}

static c_inline int ae_bitvector_get(u8* bits, const size_t bit)
{
    return (bits[bit / 8] & AE_IDX2BIT(bit % 8)) != 0;
}

static c_inline void ae_bitvector_set(u8* bits, const size_t bit, const int value)
{
    if (value)
    {
        bits[bit / 8] |=  AE_IDX2BIT(bit % 8);
    }
    else
    {
        bits[bit / 8] &= ~AE_IDX2BIT(bit % 8);
    }
}

/*
================================================================================
 * ~~ [ local heap allocator ] ~~ *
--------------------------------------------------------------------------------
TODO: active alloc counter (even with realloc) - would be a huge debugging boon
--------------------------------------------------------------------------------
*/

typedef struct ae_memory_heap_t
{
    char name[16];
    void* heap;
} \
    ae_memory_heap_t;

#define ae_heap_create(h, n, r) ae_heap_create_ex((h), (n), (r), __FILE__, __FUNCTION__, __LINE__)
#define ae_heap_destroy(h) ae_heap_destroy_ex((h), __FILE__, __FUNCTION__, __LINE__)

AE_DECL void AE_CALL ae_heap_create_ex(ae_memory_heap_t* heap, const char* name, size_t reserve,
                const char* filename, const char* funcname, const int lineno);

AE_DECL void AE_CALL ae_heap_destroy_ex(ae_memory_heap_t* heap,
                const char* filename, const char* funcname, const int lineno);

#define ae_heap_malloc(h, s) ae_heap_malloc_ex((h), (s), __FILE__, __FUNCTION__, __LINE__)
#define ae_heap_free(h, p) ae_heap_free_ex((h), (p), __FILE__, __FUNCTION__, __LINE__)
#define ae_heap_calloc(h, n, s) ae_heap_calloc_ex((h), (n), (s), __FILE__, __FUNCTION__, __LINE__)
#define ae_heap_realloc(h, p, s) ae_heap_realloc_ex((h), (p), (s), __FILE__, __FUNCTION__, __LINE__)

AE_DECL void* AE_CALL ae_heap_malloc_ex(ae_memory_heap_t* heap, size_t size,
                const char* filename, const char* funcname, const int lineno);

AE_DECL void  AE_CALL ae_heap_free_ex(ae_memory_heap_t* heap, void* ptr,
                const char* filename, const char* funcname, const int lineno);

AE_DECL void* AE_CALL ae_heap_calloc_ex(ae_memory_heap_t* heap, size_t num, size_t size,
                const char* filename, const char* funcname, const int lineno);

AE_DECL void* AE_CALL ae_heap_realloc_ex(ae_memory_heap_t* heap, void* ptr, size_t size,
                const char* filename, const char* funcname, const int lineno);

/*
================================================================================
 * ~~ [ global heap allocator ] ~~ *
--------------------------------------------------------------------------------
*/

#define _ae_clear(p, type) memset((p), 0, sizeof(type))
#define _ae_noclear(p, type) (p)

#define ae_create(type, mode) ((type*)_ae_ ## mode(ae_malloc(sizeof(type)), type))

#define ae_malloc(s) ae_malloc_ex((s), __FILE__, __FUNCTION__, __LINE__)
#define ae_free(p) ae_free_ex((p), __FILE__, __FUNCTION__, __LINE__)
#define ae_calloc(n, s) ae_calloc_ex((n), (s), __FILE__, __FUNCTION__, __LINE__)
#define ae_realloc(p, s) ae_realloc_ex((p), (s), __FILE__, __FUNCTION__, __LINE__)

AE_DECL void* AE_CALL ae_malloc_ex(size_t size,
                const char* filename, const char* funcname, const int lineno);

AE_DECL void  AE_CALL ae_free_ex(void* ptr,
                const char* filename, const char* funcname, const int lineno);

AE_DECL void* AE_CALL ae_calloc_ex(size_t num, size_t size,
                const char* filename, const char* funcname, const int lineno);

AE_DECL void* AE_CALL ae_realloc_ex(void* ptr, size_t size,
                const char* filename, const char* funcname, const int lineno);

/*
================================================================================
 * ~~ [ dynamic arrays ] ~~ *
--------------------------------------------------------------------------------
TODO: give arrays an "owned" member that prevents freeing or resizing operations
--------------------------------------------------------------------------------
*/

typedef struct ae_array_t
{
    void * data; // memory address
    size_t size; // num used bytes
    size_t _cap; // reserved bytes
} \
    ae_array_t;

#define ae_array_append(a, d, s) ae_array_append_ex((a), (d), (s), __FILE__, __FUNCTION__, __LINE__)
#define ae_array_expand(a, s) ae_array_expand_ex((a), (s), __FILE__, __FUNCTION__, __LINE__)

#define ae_array_resize(a, s) ae_array_resize_ex((a), (s), __FILE__, __FUNCTION__, __LINE__)
#define ae_array_reserve(a, s) ae_array_reserve_ex((a), (s), __FILE__, __FUNCTION__, __LINE__)

#define ae_array_copy_ptr(p, a) ae_array_copy_ptr_ex((p), (a), __FILE__, __FUNCTION__, __LINE__)
#define ae_array_copy(d, s) ae_array_copy_ex((d), (s), __FILE__, __FUNCTION__, __LINE__)

#define ae_array_cmp(a, b) ae_array_cmp_ex((a), (b), __FILE__, __FUNCTION__, __LINE__)
#define ae_array_is_zero(a) ae_array_is_zero_ex((a), __FILE__, __FUNCTION__, __LINE__)

#define ae_array_zero(a) ae_array_zero_ex((a), __FILE__, __FUNCTION__, __LINE__)
#define ae_array_trim(a) ae_array_trim_ex((a), __FILE__, __FUNCTION__, __LINE__)
#define ae_array_free(a) ae_array_free_ex((a), __FILE__, __FUNCTION__, __LINE__)

AE_DECL void AE_CALL ae_array_append_ex(ae_array_t* array, void* data, size_t size,
                const char* filename, const char* funcname, const int lineno);

AE_DECL void* AE_CALL ae_array_expand_ex(ae_array_t* array, size_t size,
                const char* filename, const char* funcname, const int lineno);

AE_DECL void AE_CALL ae_array_resize_ex(ae_array_t* array, size_t size,
                const char* filename, const char* funcname, const int lineno);

AE_DECL void AE_CALL ae_array_reserve_ex(ae_array_t* array, size_t size,
                const char* filename, const char* funcname, const int lineno);

AE_DECL void AE_CALL ae_array_copy_ptr_ex(void* ptr, const ae_array_t* array,
                const char* filename, const char* funcname, const int lineno);

AE_DECL void AE_CALL ae_array_copy_ex(ae_array_t* dst, const ae_array_t* src,
                const char* filename, const char* funcname, const int lineno);

AE_DECL int AE_CALL ae_array_cmp_ex(const ae_array_t* a, const ae_array_t* b,
                const char* filename, const char* funcname, const int lineno);

AE_DECL int AE_CALL ae_array_is_zero_ex(ae_array_t* array, /* all bytes are 0 */
                const char* filename, const char* funcname, const int lineno);

AE_DECL void AE_CALL ae_array_zero_ex(ae_array_t* array, /* set all bits to 0 */
                const char* filename, const char* funcname, const int lineno);

AE_DECL void AE_CALL ae_array_trim_ex(ae_array_t* array, /* shrink cap to size */
                const char* filename, const char* funcname, const int lineno);

AE_DECL void AE_CALL ae_array_free_ex(ae_array_t* array, /* free array data */
                const char* filename, const char* funcname, const int lineno);

/*
================================================================================
 * ~~ [ fixed-size block allocator ] ~~ *
--------------------------------------------------------------------------------
*/

typedef struct ae_memory_chunk_t
{
    char name[16];
    u32 num_blocks;
    u32 block_size;
    u32 num_free;
    u32 num_init;
    u8* mem_chunk;
    u8* next_block;
} \
    ae_memory_chunk_t;

#define ae_chunk_create(mc, nm, bs, nb) ae_chunk_create_ex((mc), (nm), (bs), (nb), __FILE__, __FUNCTION__, __LINE__)
#define ae_chunk_destroy(mc) ae_chunk_destroy_ex((mc), __FILE__, __FUNCTION__, __LINE__)

AE_DECL void AE_CALL ae_chunk_create_ex(ae_memory_chunk_t* chunk, const char* name, u32 block_size,
                    u32 num_blocks, const char* filename, const char* funcname, const int lineno);

AE_DECL void AE_CALL ae_chunk_destroy_ex(ae_memory_chunk_t* chunk,
    const char* filename, const char* funcname, const int lineno);

#define ae_block_malloc(mc) ae_block_malloc_ex((mc), __FILE__, __FUNCTION__, __LINE__)
#define ae_block_calloc(mc) ae_block_calloc_ex((mc), __FILE__, __FUNCTION__, __LINE__)
#define ae_block_free(mc, p) ae_block_free_ex((mc), (p), __FILE__, __FUNCTION__, __LINE__)

AE_DECL void* AE_CALL ae_block_malloc_ex(ae_memory_chunk_t* chunk,
    const char* filename, const char* funcname, const int lineno);

AE_DECL void* AE_CALL ae_block_calloc_ex(ae_memory_chunk_t* chunk,
    const char* filename, const char* funcname, const int lineno);

AE_DECL void AE_CALL ae_block_free_ex(ae_memory_chunk_t* chunk, void* p,
        const char* filename, const char* funcname, const int lineno );

/*
================================================================================
 * ~~ [ stack allocator ] ~~ *
--------------------------------------------------------------------------------
*/

#if defined(_MSC_VER) && !defined(alloca)
#define alloca _alloca // XXX: this is a wonderful avenue for mysterious crashes!
#endif

typedef struct ae_memory_stack_t
{
    char name[16];
    u8* mem_stack;

    u32 size;
    u32 used;

    u32 alignment;
} \
    ae_memory_stack_t;

// use instead of alloca for better debugging, persistence across func calls, etc.
AE_DECL ae_memory_stack_t* AE_CALL ae_global_stack(void);

#define ae_stack_create(ms, nm, sz) ae_stack_create_ex((ms), (nm), (sz), __FILE__, __FUNCTION__, __LINE__)
#define ae_stack_destroy(ms) ae_stack_destroy_ex((ms), __FILE__, __FUNCTION__, __LINE__)

AE_DECL void AE_CALL ae_stack_create_ex(ae_memory_stack_t* stack, const char* name, u32 size,
                                const char* filename, const char* funcname, const int lineno);

AE_DECL void AE_CALL ae_stack_destroy_ex(ae_memory_stack_t* stack, const char* filename,
                                                const char* funcname, const int lineno);

#define ae_stack_malloc(ms, sz) ae_stack_malloc_ex((ms), (sz), __FILE__, __FUNCTION__, __LINE__)
#define ae_stack_calloc(ms, n, s) ae_stack_calloc_ex((ms), (n), (s), __FILE__, __FUNCTION__, __LINE__)
#define ae_stack_free(ms, p, sz) ae_stack_free_ex((ms), (p), (sz), __FILE__, __FUNCTION__, __LINE__)

AE_DECL void* AE_CALL ae_stack_malloc_ex( ae_memory_stack_t * stack, size_t size,
                    const char* filename, const char* funcname, const int lineno);

AE_DECL void* AE_CALL ae_stack_calloc_ex(ae_memory_stack_t* stack, size_t num, size_t size,
                            const char* filename, const char* funcname, const int lineno);

AE_DECL void AE_CALL ae_stack_free_ex(ae_memory_stack_t* stack, void* p, size_t size,
                        const char* filename, const char* funcname, const int lineno);

/*
================================================================================
 * ~~ [ string allocator ] ~~ *
--------------------------------------------------------------------------------
*/

#define ae_string_malloc(n) ae_string_malloc_ex((n), __FILE__, __FUNCTION__, __LINE__)
#define ae_string_free(s) ae_string_free_ex((s), __FILE__, __FUNCTION__, __LINE__)

AE_DECL void* AE_CALL ae_string_malloc_ex(size_t length, // num chars
        const char* filename, const char* funcname, const int lineno);

AE_DECL void  AE_CALL ae_string_free_ex(void * str, // release string
        const char* filename, const char* funcname, const int lineno);

#define ae_string_copy_sized(s, n) ae_string_copy_sized_ex((s), (n), __FILE__, __FUNCTION__, __LINE__)
#define ae_string_copy(s) ae_string_copy_ex((s), __FILE__, __FUNCTION__, __LINE__)

AE_DECL char* AE_CALL ae_string_copy_sized_ex(char* s, size_t length,
        const char* filename, const char* funcname, const int lineno);

AE_DECL char* AE_CALL ae_string_copy_ex(char* string, // copy to block
        const char* filename, const char* funcname, const int lineno);

/* NOTE: this code was originally designed for use with STB's hashtable code,
 * which also annoyingly uses char* and I haven't gotten around to fixing it.
 */
#define AE_STRING_COPY_SIZED(s, n) ((const char*)ae_string_copy_sized((const char*)(s), (n)))
#define AE_STRING_COPY(s) ((const char*)ae_string_copy((const char*)(s)))

/*
================================================================================
 * ~~ [ C++ array classes ] ~~ *
--------------------------------------------------------------------------------
TODO: all the methods, typedefs, and iterator stuff we need to be STL compliant
TODO: this throws away file & line information - declare these through a macro?
TODO: const ASCII string class that stores length and hash - overload hashtable
TODO: aeBlockAllocator<class T, size_t N> - compile-time version of block alloc
--------------------------------------------------------------------------------
*/

#ifdef __cplusplus
} /* extern "C" */

template <class type> struct aeTempArray
{
    aeTempArray(const size_t num, bool clear=false)
    {
        init(num, ae_global_stack(), clear);
    }

    aeTempArray(const size_t num, ae_memory_stack_t* stack, bool clear=false)
    {
        init(num, stack, clear);
    }

    aeTempArray(const aeTempArray<type>& other)
    {
        init(other.m_num, other.m_stack, false);

        type* ptr = m_ptr; // copy constructors
        type* obj = other.m_ptr;
        type* end = ptr + m_num;

        while (ptr < end) { *ptr++ = *obj++; }
    }

    ~aeTempArray()
    {
        ae_stack_free(m_stack, m_ptr, bytes());
    }

    type& operator[](size_t index)
    {
        ae_assert(index < m_num, "idx %u >= num %u", (u32)index, (u32)m_num);
        return m_ptr[index];
    }

    const type& operator[](size_t index) const
    {
        ae_assert(index < m_num, "idx %u >= num %u", (u32)index, (u32)m_num);
        return m_ptr[index];
    }

    const bool is_stack_resident(ae_memory_stack_t* stack) const
    {
        return ptr_in_array(m_ptr, stack->mem_stack, stack->size) != 0;
    }

    const bool is_stack_resident() const
    {
        return is_stack_resident(m_stack);
    }

    size_t bytes()              { return m_num * sizeof(type); }
    const size_t bytes() const  { return m_num * sizeof(type); }

    type* data()                { return m_ptr; }
    const type* data() const    { return m_ptr; }

    size_t size()               { return m_num; }
    const size_t size() const   { return m_num; }

    bool empty()                { return m_num == 0; }
    const bool empty() const    { return m_num == 0; }

protected:
    void init(const size_t num, ae_memory_stack_t* stack, bool clear)
    {
        if (clear)
        {
            m_ptr = static_cast<type*>(ae_stack_calloc(stack, num , sizeof(type)));
        }
        else
        {
            m_ptr = static_cast<type*>(ae_stack_malloc(stack, num * sizeof(type)));
        }

        m_num = num; m_stack = stack;
    }

    type * m_ptr;
    size_t m_num;

    ae_memory_stack_t* m_stack;
};

template <class type> struct aeArray
{
    /* XXX NOTE this prevents definition of the copy constructor.
     * if this becomes an issue, remove the default owned value.
     */
    aeArray(ae_array_t* array, const bool owned=true)
    {
        m_array = array;
        m_owned = owned;
    }

    aeArray()
    {
        memset(&m_struct, 0, sizeof(m_struct));
        m_owned = false;
        m_array = &m_struct;
    }

    ~aeArray()
    {
        if (!m_owned) { ae_array_free(m_array); }
    }

    type& operator[](size_t index)
    {
        ae_assert(index < size(), "index %u >= count %u",
                                (u32)index, (u32)size());

        return static_cast<type *>(m_array->data)[index];
    }

    const type& operator[](size_t index) const
    {
        ae_assert(index < size(), "index %u >= count %u",
                                (u32)index, (u32)size());

        return static_cast<type *>(m_array->data)[index];
    }

    size_t bytes()              { return m_array->size; }
    const size_t bytes() const  { return m_array->size; }

    type* data()                { return (      type*)m_array->data; }
    const type* data() const    { return (const type*)m_array->data; }

    size_t size()               { return bytes() / sizeof(type); }
    const size_t size() const   { return bytes() / sizeof(type); }

    bool empty()                { return bytes() == 0; }
    const bool empty() const    { return bytes() == 0; }

    void append(const type& value)
    {
        ae_array_append(m_array, (void*)&value, sizeof(type));
    }

    void resize(const size_t count)
    {
        ae_array_resize(m_array, count * sizeof(type));
    }

    void reserve(const size_t count)
    {
        ae_array_reserve(m_array, count * sizeof(type));
    }

    void trim()
    {
        ae_array_trim(m_array);
    }

protected:
    ae_array_t *m_array;
    ae_array_t m_struct;

    bool m_owned;
};

#endif
#endif /* __AE_MEMORY_H__ */
