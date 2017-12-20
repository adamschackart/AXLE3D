# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
from vec cimport Vector, Vec2, Vec3, Vec4, vec_copy, vec2copy, vec3copy, vec4copy
from libc.string cimport memcpy, memset

import sys # version info

cdef extern from "ae_memory.h":
    # ===== [ fixed-size scalar types ] ========================================

    ctypedef   signed char      s8
    ctypedef unsigned char      u8

    ctypedef   signed short     s16
    ctypedef unsigned short     u16

    ctypedef   signed int       s32
    ctypedef unsigned int       u32

    ctypedef   signed long long s64
    ctypedef unsigned long long u64

    enum: AE_U8_MAX, AE_U16_MAX, AE_U32_MAX, AE_U64_MAX
    enum: AE_S8_MAX, AE_S16_MAX, AE_S32_MAX, AE_S64_MAX
    enum: AE_S8_MIN, AE_S16_MIN, AE_S32_MIN, AE_S64_MIN

    # ===== [ integer unions ] =================================================

    ctypedef union ae_integer16_t:
        s8 as_s8[2]
        u8 as_u8[2]

        s16 as_s16[1]
        u16 as_u16[1]

        u16 u_value
        s16 s_value

    ctypedef union ae_integer32_t:
        s8 as_s8[4]
        u8 as_u8[4]

        s16 as_s16[2]
        u16 as_u16[2]

        s32 as_s32[1]
        u32 as_u32[1]

        u32 u_value
        s32 s_value

        float as_flt[1]
        float f_value

    ctypedef union ae_integer64_t:
        s8 as_s8[8]
        u8 as_u8[8]

        s16 as_s16[4]
        u16 as_u16[4]

        s32 as_s32[2]
        u32 as_u32[2]

        s64 as_s64[1]
        u64 as_u64[1]

        u64 u_value
        s64 s_value

        float  as_flt[2]
        double as_dbl[1]
        double f_value

    # ===== [ misc. utilities ] ================================================

    char** ae_argv(int* argc)

    void memswap(void* a_ptr, void* b_ptr, const size_t bytes)
    int memiszero(void* ptr, size_t bytes)
    void memreverse(void* base, size_t e_count, size_t e_size)

    size_t memfilter(void* dst, const void* const src, size_t count, size_t size,
                    int (*filter)(const void* item, const void* ctx), void* ctx)

    int ptr_in_array(void* ptr, void* array, size_t size)
    int num_is_aligned(size_t number, size_t alignment)

    int zero_byte_in_u32(u32 x)
    int zero_byte_in_s32(s32 x)

    enum: AE_HEAP_ALIGNMENT # round up to multiple of align
    size_t num_pow2_align(size_t number, size_t alignment)

    # ===== [ string formatting ] ==============================================

    # int ae_vsnprintf(char* buf, size_t size, const char* fmt, va_list args)
    int ae_snprintf(char* buf, size_t size, const char* fmt, ...)

    # NOTE: remember that num is sizeof(dst) - 1 for fixed-length dst buffers
    char* ae_strncpy(char* dst, const char* src, size_t num_chars)

    # NOTE: remember that num is sizeof(src) - 1 for fixed-length src buffers
    char* ae_strncat(char* dst, const char* src, size_t num_chars)

    void ae_strbounds(const char* s, size_t tab_width, size_t* cols, size_t* rows)

    void ae_strnbounds(const char* string, size_t length,
            size_t tab_width, size_t* cols, size_t* rows)

    # ===== [ powers of 2 ] ====================================================

    int num_is_pow2(u32 num)

    u32 num_prev_pow2(u32 x)
    u32 num_next_pow2(u32 x)

    # ===== [ bits ] ===========================================================

    size_t ae_idx2bit(const size_t X)
    size_t ae_bit2idx(const size_t X)

    int ae_tribool(size_t mask, size_t neg, size_t pos)

    size_t ae_bitvector_bytes(const size_t num_bits) # large bit arrays

    int  ae_bitvector_get(u8* bits, const size_t bit)
    void ae_bitvector_set(u8* bits, const size_t bit, const int value)

    # ===== [ local heap allocator ] ===========================================

    ctypedef struct ae_memory_heap_t:
        pass

    void ae_heap_create(ae_memory_heap_t* heap, const char* name, size_t reserve)
    void ae_heap_destroy(ae_memory_heap_t* heap)

    void* ae_heap_malloc(ae_memory_heap_t* heap, size_t size)
    void ae_heap_free(ae_memory_heap_t* heap, void* ptr)

    void* ae_heap_calloc(ae_memory_heap_t* heap, size_t num, size_t size)
    void* ae_heap_realloc(ae_memory_heap_t* heap, void* ptr, size_t size)

    # ===== [ global heap allocator ] ==========================================

    void* ae_malloc(size_t size)
    void  ae_free(void* memptr)

    void* ae_calloc(size_t num, size_t size)
    void* ae_realloc(void* ptr, size_t size)

    # ===== [ dynamic arrays ] =================================================

    ctypedef struct ae_array_t:
        void * data # memory address
        size_t size # num used bytes
        size_t _cap # reserved bytes

    void ae_array_append(ae_array_t* array, void* data, size_t size)

    void ae_array_resize (ae_array_t* array, size_t size)
    void ae_array_reserve(ae_array_t* array, size_t size)

    void ae_array_copy_ptr(void* ptr, const ae_array_t* array)
    void ae_array_copy(ae_array_t* dst, const ae_array_t* src)

    int ae_array_cmp(const ae_array_t* a, const ae_array_t* b)
    int ae_array_is_zero(ae_array_t* arr) # array bytes are 0

    void ae_array_zero(ae_array_t* array) # set all bits to 0
    void ae_array_trim(ae_array_t* array) # shrink cap to size
    void ae_array_free(ae_array_t* array) # free array data

    # ===== [ fixed-size block allocator ] =====================================

    ctypedef struct ae_memory_chunk_t:
        pass

    void ae_chunk_create(ae_memory_chunk_t* chunk, const char* name,
                                    u32 block_size, u32 num_blocks)
    void ae_chunk_destroy(ae_memory_chunk_t* chunk)

    void* ae_block_malloc(ae_memory_chunk_t* chunk)
    void* ae_block_calloc(ae_memory_chunk_t* chunk)

    void ae_block_free(ae_memory_chunk_t* chunk, void* p)

    # ===== [ stack allocator ] ================================================

    ctypedef struct ae_memory_stack_t:
        pass

    ae_memory_stack_t* ae_global_stack()

    void ae_stack_create(ae_memory_stack_t* stack, const char* name, u32 size)
    void ae_stack_destroy(ae_memory_stack_t* stack)

    void* ae_stack_malloc(ae_memory_stack_t* stack, size_t size)
    void* ae_stack_calloc(ae_memory_stack_t* stack, size_t num, size_t size)

    void ae_stack_free(ae_memory_stack_t* stack, void* p, size_t size)

    # ===== [ string allocator ] ===============================================

    void* ae_string_malloc(size_t length) # length is character count
    void  ae_string_free(void* string) # free from str chunk or heap

    char* ae_string_copy_sized(char* string, size_t length) # n chars
    char* ae_string_copy(char* string) # copy string to alloced block

    const char* AE_STRING_COPY_SIZED(const char* strn, size_t length)
    const char* AE_STRING_COPY(const char* strn) # fix type annoyance

cdef extern from "ae_hash.h":
    # ===== [ hash functions ] =================================================

    # combine two hash values in a way that deals well with poor hash functions
    u32 ae_hash_combine(u32 lhs, u32 rhs)

    u32 ae_rehash_old(u32 v)
    u32 ae_rehash_new(u32 v)

    u32 ae_hash_str(char *str) # condense string to int
    u32 ae_hash_len(char *str, int len)

    void ae_hash64_str(char* str, u32 hash[2]) # 64-bit
    void ae_hash64_len(char* str, int len, u32 hash[2])

    u32 ae_hash_ptr(void *p)
    u32 ae_hash_num(u32 num)

    u32 ae_hash_mem(void* p, int len)

    # ===== [ streaming block checksums ] ======================================

    u32 ae_crc32_block(u32 crc, const void* const buffer, const size_t len)
    u32 ae_crc32(const void* const buffer, const size_t len)

    u32 ae_adler32_block(u32 adler32, const void* const buffer, size_t len)
    u32 ae_adler32(const void* const buffer, size_t len)

cdef extern from "ae_file.h":
    # ===== [ filesystem utilities ] ===========================================

    size_t ae_file_bytes(const char* filename, int fatal)
    int ae_file_exists(const char* filename)

    void ae_file_move(const char* oldname, const char* newname, int fatal)
    void ae_file_copy(const char* oldname, const char* newname, int fatal)

    void ae_file_remove(const char* filename, int fatal)

    const char* ae_conf_path(const char* org, const char* app)
    const char* ae_base_path()

    void ae_base_path_override(const char* name)

    # ===== [ path string utilities ] ==========================================

    const char* ae_filename_from_path_ex(const char* path, size_t* path_length)
    const char* ae_filename_from_path(const char* path) # "C:\User\x.y" => "x.y"

    void ae_split_file_path(const char* path_and_name, char* path, char* name)

    const char* ae_extension_from_path_ex(const char* path, size_t* path_length)
    const char* ae_extension_from_path(const char* path) # "C:\User\x.y" => "y"

    void ae_split_file_extension(const char* path, char* name, char* extn)

    # ===== [ reading whole binary files ] =====================================

    int ae_file_read_array(ae_array_t* array, const char* filename, int fatal)

    void* ae_file_read_stack(ae_memory_stack_t* stack,
                                size_t* size, const char* filename, int fatal)

    void* ae_file_read_block(ae_memory_chunk_t* chunk,
                                size_t* size, const char* filename, int fatal)

    void* ae_file_read_heap(ae_memory_heap_t* heap,
                                size_t* size, const char* filename, int fatal)

    void* ae_file_read_string(size_t* size, const char* filename, int fatal)

    void* ae_file_read(size_t* size, const char* filename, int fatal)

    # ===== [ writing whole binary files ] =====================================

    int ae_file_write(const char* filename, void* ptr, size_t size, int fatal)

cdef extern from "ae_random.h":
    #
    # redundantly declared here to avoid a circular dependency between rng & mem
    #
    void memshuffle(void* base, size_t count, size_t size)

# double check these to make sure typedefs written here match the header file's
assert sizeof(u8 ) == 1, "{}".format(sizeof(u8 ))
assert sizeof(u16) == 2, "{}".format(sizeof(u16))
assert sizeof(u32) == 4, "{}".format(sizeof(u32))
assert sizeof(u64) == 8, "{}".format(sizeof(u64))

# ==============================================================================
# ~ [ dynamic arrays ]
# ==============================================================================
# NOTE: aecore calls will be used on these, so overriding resize etc. is futile
# (unless i decide someday to put a vtable pointer into the array structure...)
# ------------------------------------------------------------------------------

cdef class Array:
    cdef ae_array_t array

    # TODO: cpython does this automatically to ext. types, does pypy?
    # def __cinit__(self): memset(&self.array, 0, sizeof(ae_array_t))

    def __dealloc__(self):
        ae_array_free(&self.array)

    def __repr__(self):
        return "{}({} size, {} cap)".format( # report total array size
            self.__class__.__name__, self.array.size, self.array._cap)

    def __nonzero__(self):
        return self.array.size != 0

    def __copy__(self):
        cdef Array v = self.__class__(*self.ctor_args)
        ae_array_copy(&v.array, &self.array); return v

    copy = __copy__

    def __len__(self):
        return self.array.size

    def __getitem__(self, size_t index):
        if index >= self.array.size: raise IndexError
        return(<u8*>self.array.data)[index]

    def __setitem__(self, size_t index, u8 value):
        if index >= self.array.size: raise IndexError
        (<u8*>self.array.data)[index] = value

    def __reduce__(self):
        cdef bytes b = b'\0' * self.array.size
        cdef char* p = <char *>b

        ae_array_copy_ptr(p, &self.array) # pickle
        return (self.__class__, self.ctor_args, b)

    def __setstate__(self, bytes state):
        cdef ae_array_t array

        array.size = array._cap = len(state)
        array.data = <char*>state
        ae_array_copy(&self.array, &array)

    property ctor_args: # subclasses
        def __get__(self): return ()

    def address(self):
        return <size_t>self.array.data

    def capacity(self):
        return self.array._cap

    def contains_ptr(self, size_t ptr):
        return ptr_in_array(<void*>ptr, self.array.data, self.array.size)

    def cmp(self, Array other):
        return ae_array_cmp(&self.array, &other.array) # do bytes match?

    def is_zero(self):
        return ae_array_is_zero(&self.array) # all array bytes are zero?

    def hash(self):
        return ae_hash_mem(self.array.data, <int>self.array.size)

    def adler32(self, u32 seed=1):
        return ae_adler32_block(seed, self.array.data, self.array.size)

    def crc32(self, u32 seed=0):
        return ae_crc32_block(seed, self.array.data, self.array.size)

    def append_file(self, str filename, bint fatal=True):
        """
        Write the contents of a file to the end of this array (in binary mode).
        """
        cdef bytes b_filename # convert potentially unicode filename to ascii

        if sys.version_info.major > 2:
            b_filename = <bytes>filename.encode('utf-8')
        else:
            b_filename = <bytes>filename

        if not ae_file_read_array(&self.array, <char*>b_filename, 0) and fatal:
            raise IOError("failed to read file \"{}\"".format(filename))

        return self

    def write_file(self, str filename, bint fatal=True):
        """
        Write this array to a raw binary file (no newline conversion, etc). If
        the file already existed, its contents will be overwritten (truncated).
        """
        cdef bytes b_filename # convert potentially unicode filename to ascii

        if sys.version_info.major > 2:
            b_filename = <bytes>filename.encode('utf-8')
        else:
            b_filename = <bytes>filename

        if not ae_file_write(< char* >b_filename, self.array.data,
                                    self.array.size, 0) and fatal:
            raise IOError("failed to write file \"{}\"".format(filename))

        return self

    def get_subview(self, size_t offset, size_t length, object array_t=Array):
        """
        The in-place, unsafe version of get_region. Returns an array pointing
        directly into a region of self, which must be freed with del_subview.
        """
        cdef Array subview = array_t(*self.ctor_args)

        assert offset + length <= self.array.size, '{} subview (offset'\
            ' {} length {}) out of bounds'.format(self, offset, length)

        subview.array.data = &( <u8 *> self.array.data )[offset]
        subview.array.size = subview.array._cap = <size_t>length

        return subview

    def del_subview(self):
        """
        Safely deletes a buffer returned by get_subview without freeing memory.
        """
        memset(&self.array, 0, sizeof(ae_array_t)); return self

    def chunk(self, size_t chunk_size, object chunk_t=Array):
        """
        Make a new copy of self broken up into a list of equally sized regions.
        """
        assert (chunk_size and self.array.size % chunk_size == 0, # check size
                "{} is an invalid chunk size for {}".format(chunk_size, self))

        return [self.get_region(i, chunk_size, chunk_t) \
            for i in range(0, self.array.size, chunk_size)]

    @classmethod
    def merge(cls, list arrays):
        """
        Join a list of arrays into one array. The functional opposite of chunk.
        """
        cdef Array a, self = cls(*arrays[0].ctor_args)

        assert len(set(_.ctor_args for _ in arrays)) == 1, "heterogenous array " \
            "constructor arguments: {}".format(set([_.ctor_args for _ in arrays]))

        # TODO: verify that the sum loop is properly translated to C
        ae_array_reserve(&self.array, sum(map(len, arrays)))

        for a in arrays:
            ae_array_append(&self.array, a.array.data, a.array.size)

        return self

    def shuffle(self, size_t elem_size):
        """
        Randomly shuffle all n-byte elements around inside a homogenous array.
        """
        assert ( elem_size <= self.array.size ) if self else True, \
            "{} size is not divisible by {}".format(self, elem_size)

        assert not self.array.size % elem_size, \
                "{} {}".format(self, elem_size)

        memshuffle(self.array.data, self.array.size // elem_size, elem_size)
        return self

    def reverse(self, size_t elem_size):
        """
        Reverse array contents. Be careful not to call this instead of reserve!
        """
        assert ( elem_size <= self.array.size ) if self else True, \
            "{} size is not divisible by {}".format(self, elem_size)

        assert not self.array.size % elem_size, \
                "{} {}".format(self, elem_size)

        memreverse(self.array.data, self.array.size // elem_size, elem_size)
        return self

    def append(self, size_t ptr, size_t size):
        ae_array_append(&self.array, <void*>ptr, size); return self

    def append_vector(self, Vector v):
        ae_array_append(&self.array, v.v, sizeof(float) * v.n); return self

    def append_vec2(self, Vec2 v):
        ae_array_append(&self.array, v.v, sizeof(float[2])); return self

    def append_vec3(self, Vec3 v):
        ae_array_append(&self.array, v.v, sizeof(float[3])); return self

    def append_vec4(self, Vec4 v):
        ae_array_append(&self.array, v.v, sizeof(float[4])); return self

    def append_flt(self, float v):
        ae_array_append(&self.array, &v, sizeof(v)); return self

    def append_dbl(self, double v):
        ae_array_append(&self.array, &v, sizeof(v)); return self

    def append_u8(self, u8 v):
        ae_array_append(&self.array, &v, sizeof(v)); return self

    def append_s8(self, s8 v):
        ae_array_append(&self.array, &v, sizeof(v)); return self

    def append_u16(self, u16 v):
        ae_array_append(&self.array, &v, sizeof(v)); return self

    def append_s16(self, s16 v):
        ae_array_append(&self.array, &v, sizeof(v)); return self

    def append_u32(self, u32 v):
        ae_array_append(&self.array, &v, sizeof(v)); return self

    def append_s32(self, s32 v):
        ae_array_append(&self.array, &v, sizeof(v)); return self

    def append_u64(self, u64 v):
        ae_array_append(&self.array, &v, sizeof(v)); return self

    def append_s64(self, s64 v):
        ae_array_append(&self.array, &v, sizeof(v)); return self

    def append_array_list(self, list v):
        cdef Array i # TODO: reserve sum? use c call?

        for i in v: self.append(i.address(), len(i))
        return self

    def append_vector_list(self, list v):
        cdef Vector i # TODO: reserve sum?

        for i in v: self.append_vector(i)
        return self

    def append_vec2_list(self, list v):
        cdef Vec2 i
        ae_array_reserve(&self.array, self.array.size + len(v) * sizeof(float[2]))

        for i in v: self.append_vec2(i)
        return self

    def append_vec3_list(self, list v):
        cdef Vec3 i
        ae_array_reserve(&self.array, self.array.size + len(v) * sizeof(float[3]))

        for i in v: self.append_vec3(i)
        return self

    def append_vec4_list(self, list v):
        cdef Vec4 i
        ae_array_reserve(&self.array, self.array.size + len(v) * sizeof(float[4]))

        for i in v: self.append_vec4(i)
        return self

    def append_flt_list(self, list v):
        cdef float i
        ae_array_reserve(&self.array, self.array.size + len(v) * sizeof(float))

        for i in v: self.append_flt(i)
        return self

    def append_dbl_list(self, list v):
        cdef double i
        ae_array_reserve(&self.array, self.array.size + len(v) * sizeof(double))

        for i in v: self.append_dbl(i)
        return self

    def append_u8_list(self, list v):
        cdef u8 i
        ae_array_reserve(&self.array, self.array.size + len(v) * sizeof(u8))

        for i in v: self.append_u8(i)
        return self

    def append_s8_list(self, list v):
        cdef s8 i
        ae_array_reserve(&self.array, self.array.size + len(v) * sizeof(s8))

        for i in v: self.append_s8(i)
        return self

    def append_u16_list(self, list v):
        cdef u16 i
        ae_array_reserve(&self.array, self.array.size + len(v) * sizeof(u16))

        for i in v: self.append_u16(i)
        return self

    def append_s16_list(self, list v):
        cdef s16 i
        ae_array_reserve(&self.array, self.array.size + len(v) * sizeof(s16))

        for i in v: self.append_s16(i)
        return self

    def append_u32_list(self, list v):
        cdef u32 i
        ae_array_reserve(&self.array, self.array.size + len(v) * sizeof(u32))

        for i in v: self.append_u32(i)
        return self

    def append_s32_list(self, list v):
        cdef s32 i
        ae_array_reserve(&self.array, self.array.size + len(v) * sizeof(s32))

        for i in v: self.append_s32(i)
        return self

    def append_u64_list(self, list v):
        cdef u64 i
        ae_array_reserve(&self.array, self.array.size + len(v) * sizeof(u64))

        for i in v: self.append_u64(i)
        return self

    def append_s64_list(self, list v):
        cdef s64 i
        ae_array_reserve(&self.array, self.array.size + len(v) * sizeof(s64))

        for i in v: self.append_s64(i)
        return self

    # NOTE: vecs are single float addressed, other types are addressed by size

    def get_vector(self, size_t index, size_t length):
        assert index + length <= self.array.size // sizeof(float), index

        cdef Vector v = Vector(length)
        vec_copy(v.v, &(<float *>self.array.data)[index], length)

        return v

    def get_vec2(self, size_t index):
        assert index + 2 <= self.array.size // sizeof(float), index

        cdef Vec2 v = Vec2() # get vec at a scalar offset
        vec2copy(v.v, &(<float *>self.array.data)[index])

        return v

    def get_vec3(self, size_t index):
        assert index + 3 <= self.array.size // sizeof(float), index

        cdef Vec3 v = Vec3() # get vec at a scalar offset
        vec3copy(v.v, &(<float *>self.array.data)[index])

        return v

    def get_vec4(self, size_t index):
        assert index + 4 <= self.array.size // sizeof(float), index

        cdef Vec4 v = Vec4() # get vec at a scalar offset
        vec4copy(v.v, &(<float *>self.array.data)[index])

        return v

    def get_flt(self, size_t index):
        assert index < self.array.size // sizeof(float), index
        return (<float*>self.array.data)[index]

    def get_dbl(self, size_t index):
        assert index < self.array.size // sizeof(double), index
        return (<double*>self.array.data)[index]

    def get_bit(self, size_t index):
        assert index < self.array.size * 8, "{} >= {}" \
                    .format(index, self.array.size * 8)

        return ae_bitvector_get(<u8*>self.array.data, index)

    def get_u8(self, size_t index):
        assert index < self.array.size // sizeof(u8), index
        return (<u8 *>self.array.data)[index]

    def get_s8(self, size_t index):
        assert index < self.array.size // sizeof(s8), index
        return (<s8 *>self.array.data)[index]

    def get_u16(self, size_t index):
        assert index < self.array.size // sizeof(u16), index
        return (<u16*>self.array.data)[index]

    def get_s16(self, size_t index):
        assert index < self.array.size // sizeof(s16), index
        return (<s16*>self.array.data)[index]

    def get_u32(self, size_t index):
        assert index < self.array.size // sizeof(u32), index
        return (<u32*>self.array.data)[index]

    def get_s32(self, size_t index):
        assert index < self.array.size // sizeof(s32), index
        return (<s32*>self.array.data)[index]

    def get_u64(self, size_t index):
        assert index < self.array.size // sizeof(u64), index
        return (<u64*>self.array.data)[index]

    def get_s64(self, size_t index):
        assert index < self.array.size // sizeof(s64), index
        return (<s64*>self.array.data)[index]

    def get_region(self, size_t offset, size_t length, object array_t=Array):
        """
        Allocate and copy a subsection of this array's contents. If you need
        to manipulate a specific section in-place, call get_subview instead.
        """
        cdef Array region = array_t(*self.ctor_args)

        assert offset + length <= self.array.size, '{} region (offset'\
            ' {} length {}) out of bounds'.format(self, offset, length)

        ae_array_resize(&region.array, length)
        memcpy(region.array.data, &(<u8*>self.array.data)[offset], length)

        return region

    def get_bytes(self, size_t offset, size_t length):
        """
        Copy a sized subsection of this array into a new python bytes object.
        """
        cdef bytes b = b'\0' * length

        assert offset + length <= self.array.size, '{} bytes (offset' \
            ' {} length {}) out of bounds'.format(self, offset, length)

        memcpy(<char*>b, &(<u8*>self.array.data)[offset], length); return b

    def set_vector(self, size_t index, Vector v):
        assert ( index + v.n ) <= self.array.size // sizeof(float), index
        vec_copy(&(<float *>self.array.data)[index], v.v, v.n); return self

    def set_vec2(self, size_t index, Vec2 v):
        assert ( index + 2 ) <= self.array.size // sizeof(float), index
        vec2copy(&(<float *>self.array.data)[index], v.v); return self

    def set_vec3(self, size_t index, Vec3 v):
        assert ( index + 3 ) <= self.array.size // sizeof(float), index
        vec3copy(&(<float *>self.array.data)[index], v.v); return self

    def set_vec4(self, size_t index, Vec4 v):
        assert ( index + 4 ) <= self.array.size // sizeof(float), index
        vec4copy(&(<float *>self.array.data)[index], v.v); return self

    def set_flt(self, size_t index, float v):
        assert index < self.array.size // sizeof(float), index
        (<float*>self.array.data)[index] = v; return self

    def set_dbl(self, size_t index, double v):
        assert index < self.array.size // sizeof(double), index
        (<double*>self.array.data)[index] = v; return self

    def set_bit(self, size_t index, bint v):
        assert index < self.array.size * 8, "{} >= {}" \
                    .format(index, self.array.size * 8)

        ae_bitvector_set(<u8*>self.array.data, index, v)
        return self

    def set_u8(self, size_t index, u8 v):
        assert index < self.array.size // sizeof(u8), index
        (<u8*>self.array.data)[index] = v; return self

    def set_s8(self, size_t index, s8 v):
        assert index < self.array.size // sizeof(s8), index
        (<s8*>self.array.data)[index] = v; return self

    def set_u16(self, size_t index, u16 v):
        assert index < self.array.size // sizeof(u16), index
        (<u16*>self.array.data)[index] = v; return self

    def set_s16(self, size_t index, s16 v):
        assert index < self.array.size // sizeof(s16), index
        (<s16*>self.array.data)[index] = v; return self

    def set_u32(self, size_t index, u32 v):
        assert index < self.array.size // sizeof(u32), index
        (<u32*>self.array.data)[index] = v; return self

    def set_s32(self, size_t index, s32 v):
        assert index < self.array.size // sizeof(s32), index
        (<s32*>self.array.data)[index] = v; return self

    def set_u64(self, size_t index, u64 v):
        assert index < self.array.size // sizeof(u64), index
        (<u64*>self.array.data)[index] = v; return self

    def set_s64(self, size_t index, s64 v):
        assert index < self.array.size // sizeof(s64), index
        (<s64*>self.array.data)[index] = v; return self

    def set_region(self, size_t offset, Array region):
        """
        Copy another array into a subsection of this array at a given byte offset.
        """
        assert offset + region.array.size <= self.array.size, '{} region (offset'\
            ' {} length {}) out of bounds'.format(self, offset, region.array.size)

        assert not ptr_in_array(region.array.data, self.array.data, # no subviews!
            self.array.size), '{} is copying {} into itself'.format(self, region)

        memcpy(&( <u8 *>self.array.data )[offset],
            region.array.data, region.array.size)

        return self

    def set_bytes(self, size_t offset, bytes source):
        """
        Copy a python bytes object into a region of self at a given byte offset.
        """
        assert offset + len(source) <= self.array.size, '{} bytes (offset' \
            ' {} length {}) out of bounds'.format(self, offset, len(source))

        memcpy(&(<u8 *>self.array.data)[offset],
                    <char *>source, len(source))

        return self

    @staticmethod
    def bitvector_bytes(size_t count):
        """Compute the number of bytes required to store a given number of bits."""
        return ae_bitvector_bytes(count)

    def resize(self, size_t size):
        """Change the size of the array to N bytes, either growing or shrinking."""
        ae_array_resize(&self.array, size); return self

    def reserve(self, size_t size):
        """Ensure that N bytes are allocated behind the scenes, for performance."""
        ae_array_reserve(&self.array, size); return self

    def zero(self):
        """Set the value of every byte in the array to zero (memset equivalent)."""
        ae_array_zero(&self.array); return self

    def trim(self):
        """Remove any excess extra memory capacity allocated behind the scenes."""
        ae_array_trim(&self.array); return self
