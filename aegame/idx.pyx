# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
from mem cimport Array, ae_array_t

cdef extern from "ae_index.h":
    # ===== [ index type enum ] ================================================

    ctypedef enum ae_index_type_t:
        AE_INDEX_TYPE_U8
        AE_INDEX_TYPE_U16
        AE_INDEX_TYPE_U32
        AE_INDEX_TYPE_COUNT

    const char* ae_index_type_name[]
    size_t ae_index_type_bytes[]

    ae_index_type_t ae_index_bytes_to_type(const size_t bytes)

    # ===== [ generic indexing ] ===============================================

    size_t idx_value_get(void* indices, const ae_index_type_t type,
                            const size_t count, const size_t which)

    void idx_value_set(void* idx, const ae_index_type_t type, const size_t num,
                                        const size_t which, const size_t value)

    void idx_value_append(ae_array_t* inds, const ae_index_type_t type,
                                                    const size_t value)

    size_t idx_value_max(void* indices, const ae_index_type_t type,
                                                const size_t count)

    # ===== [ identity indices ] ===============================================

    void* idx_temp_identity_get(const ae_index_type_t type, const size_t i_count)
    void idx_temp_identity_free(void* p, const ae_index_type_t t, const size_t n)

    void idx_identity(void* idx, const ae_index_type_t type, const size_t count)
    int idx_is_identity(void* p, const ae_index_type_t type, const size_t count)

    # ===== [ scalar offsets ] =================================================

    void idx_add_offset(void* indices, const ae_index_type_t type,
                        const size_t count, const size_t offset_v)

    void idx_sub_offset(void* indices, const ae_index_type_t type,
                        const size_t count, const size_t offset_v)

    # ===== [ type conversion ] ================================================

    int idx_fits_type(const void * const idx, const size_t count,
        const ae_index_type_t src_t, const ae_index_type_t dst_t)

    ae_index_type_t idx_smallest_fitting_type(const void* const idx,
                    const ae_index_type_t type, const size_t count)

    void* idx_temp_type_converted_get(void * idx, const size_t count,
            const ae_index_type_t src_t, const ae_index_type_t dst_t)

    void idx_temp_type_converted_free(void* dst, # free temp
            const size_t count, const ae_index_type_t dst_t)

    void idx_type_convert(ae_array_t* indices, # grow or shrink index buffer
                    const ae_index_type_t src_t, const ae_index_type_t dst_t)

    void idx_type_convert_ex(void* indices, # doesn't reallocate index array
        const ae_index_type_t src, const ae_index_type_t dst, const size_t n)

    void idx_type_converted(ae_array_t* src, const ae_index_type_t src_t,
                            ae_array_t* dst, const ae_index_type_t dst_t)

    void idx_type_converted_ex( void* src, const ae_index_type_t src_t,
            void* dst, const ae_index_type_t dst_t, const size_t count)

    # ===== [ vertex cache optimization ] ======================================

    void idx_vertex_cache_optimize_ex(void* indices, const ae_index_type_t type,
                            const size_t index_count, const size_t vertex_count)

    void idx_vertex_cache_optimize(void* idx_buffer, const ae_index_type_t type,
                                                            const size_t count)

    double idx_vertex_cache_miss_ratio(const void* const, const ae_index_type_t,
                                                        const size_t, int, int)

# ==============================================================================
# ~ [ index types ]
# ==============================================================================

INDEX_TYPE_U8  = AE_INDEX_TYPE_U8
INDEX_TYPE_U16 = AE_INDEX_TYPE_U16
INDEX_TYPE_U32 = AE_INDEX_TYPE_U32

# ==============================================================================
# ~ [ generic indexing ]
# ==============================================================================

def get_value(Array a, ae_index_type_t t, size_t which):
    assert t < AE_INDEX_TYPE_COUNT, "invalid index type enum value: {}".format(t)

    return idx_value_get(a.array.data, t, a.array.size // \
                    ae_index_type_bytes[<size_t>t], which)

def set_value(Array a, ae_index_type_t t, size_t which, size_t value):
    assert t < AE_INDEX_TYPE_COUNT, "invalid index type enum value: {}".format(t)

    idx_value_set( a.array.data, t, a.array.size // \
        ae_index_type_bytes[<size_t>t], which, value)

    return a

def append_value(Array a, ae_index_type_t t, size_t value):
    assert t < AE_INDEX_TYPE_COUNT, "invalid index type enum value: {}".format(t)
    idx_value_append(&a.array, t, value); return a

def max_value(Array a, ae_index_type_t t):
    assert t < AE_INDEX_TYPE_COUNT, "invalid index type enum value: {}".format(t)

    return idx_value_max(a.array.data, t, a.array.size // \
                            ae_index_type_bytes[<size_t>t])

# ==============================================================================
# ~ [ identity indices ]
# ==============================================================================

def identity(Array a, ae_index_type_t t):
    assert t < AE_INDEX_TYPE_COUNT, "invalid index type enum value: {}".format(t)
    idx_identity(a.array.data, t, a.array.size // ae_index_type_bytes[<size_t>t])

    return a

def is_identity(Array a, ae_index_type_t t):
    cdef void* ip = a.array.data

    assert t < AE_INDEX_TYPE_COUNT, "invalid index type enum value: {}".format(t)
    return idx_is_identity(ip, t, a.array.size // ae_index_type_bytes[<size_t>t])

# ==============================================================================
# ~ [ scalar offsets ]
# ==============================================================================

def add_offset(Array a, ae_index_type_t t, size_t offset):
    assert t < AE_INDEX_TYPE_COUNT, "invalid index type enum value: {}".format(t)

    idx_add_offset(a.array.data, t, a.array.size // \
            ae_index_type_bytes[ <size_t>t ], offset)

    return a

def sub_offset(Array a, ae_index_type_t t, size_t offset):
    assert t < AE_INDEX_TYPE_COUNT, "invalid index type enum value: {}".format(t)

    idx_sub_offset(a.array.data, t, a.array.size // \
            ae_index_type_bytes[ <size_t>t ], offset)

    return a

# ==============================================================================
# ~ [ type conversion ]
# ==============================================================================

def fits_type(Array a, ae_index_type_t src, ae_index_type_t dst):
    assert src < AE_INDEX_TYPE_COUNT, "invalid index type enum v: {}".format(src)
    assert dst < AE_INDEX_TYPE_COUNT, "invalid index type enum v: {}".format(dst)

    return( idx_fits_type(a.array.data, a.array.size // \
            ae_index_type_bytes[<size_t>src], src, dst) )

def smallest_fitting_type(Array a, ae_index_type_t t):
    cdef void * p = a.array.data
    cdef size_t s = a.array.size

    assert t < AE_INDEX_TYPE_COUNT, "invalid index type enum value: {}".format(t)
    return idx_smallest_fitting_type(p, t, s // ae_index_type_bytes[ <size_t>t ])

def type_convert(Array array, ae_index_type_t src, ae_index_type_t dst):
    assert src < AE_INDEX_TYPE_COUNT, "invalid index type enum v: {}".format(src)
    assert dst < AE_INDEX_TYPE_COUNT, "invalid index type enum v: {}".format(dst)

    idx_type_convert(&array.array, src, dst); return array

def type_converted(Array a, ae_index_type_t src, ae_index_type_t dst, out_t=Array):
    cdef Array out = out_t()

    assert src < AE_INDEX_TYPE_COUNT, "invalid index type enum v: {}".format(src)
    assert dst < AE_INDEX_TYPE_COUNT, "invalid index type enum v: {}".format(dst)

    idx_type_converted(&a.array, src, &out.array, dst); return out

# ==============================================================================
# ~ [ vertex cache optimization ]
# ==============================================================================

def vertex_cache_optimize_ex(Array a, ae_index_type_t t, size_t vertex_count):
    """
    Re-order indices for more efficient use of graphics hardware vertex caches.
    """
    idx_vertex_cache_optimize_ex(a.array.data, t, a.array.size // \
                    ae_index_type_bytes[ <size_t> t], vertex_count)
    return a

def vertex_cache_optimize(Array a, ae_index_type_t t):
    idx_vertex_cache_optimize(a.array.data, t, a.array.size // \
                                ae_index_type_bytes[ <size_t> t])
    return a

def vertex_cache_miss_ratio(Array a, ae_index_type_t t,
                    int cache_size=16, bint FIFO=True):
    """
    Measure cache efficiency (and compressibility, kinda) of this index buffer.
    """
    return idx_vertex_cache_miss_ratio(a.array.data, t, a.array.size // \
                        ae_index_type_bytes[<size_t>t], cache_size, FIFO)

# ==============================================================================
# ~ [ wrapper class ]
# ==============================================================================

cdef class IndexArray(Array):
    cdef public ae_index_type_t index_type

    TYPE_U8  = AE_INDEX_TYPE_U8
    TYPE_U16 = AE_INDEX_TYPE_U16
    TYPE_U32 = AE_INDEX_TYPE_U32

    def __init__(self, ae_index_type_t index_type):
        self.index_type = index_type

    def __repr__(self):
        cdef size_t index = <size_t>self.index_type # casting for cython

        return "{}({} size, {} cap, {})".format(self.__class__.__name__,
            self.array.size, self.array._cap, ae_index_type_name[index])

    def get_subview(self, size_t offset, size_t length, object array_t=IndexArray):
        return Array.get_subview(self, offset, length, array_t)

    def chunk(self, size_t chunk_size, object chunk_t=IndexArray):
        return Array.chunk(self, chunk_size, chunk_t)

    def get_region(self, size_t offset, size_t length, object array_t=IndexArray):
        return Array.get_region(self, offset, length, array_t)

    property ctor_args:
        def __get__(self): return (self.index_type,) # copy

    def type_name(self):
        return ae_index_type_name[<size_t>self.index_type]

    def type_bytes(self):
        return ae_index_type_bytes[<size_t>self.index_type]

    cpdef size_t count(self):
        return self.array.size // ae_index_type_bytes[<size_t>self.index_type]

    cpdef size_t triangle_count(self):
        return self.count() // 3

    cpdef size_t quad_count(self):
        return self.count() // 4

    def get_value(self, size_t which):
        return idx_value_get(self.array.data, self.index_type, self.count(), which)

    def set_value(self, size_t which, size_t value):
        idx_value_set(self.array.data, self.index_type, self.count(), which, value)
        return self

    def append_value(self, size_t value):
        idx_value_append(&self.array, self.index_type, value); return self

    def max_value(self):
        return idx_value_max(self.array.data, self.index_type, self.count())

    def identity(self):
        idx_identity(self.array.data, self.index_type, self.count()); return self

    def is_identity(self):
        return idx_is_identity(self.array.data, self.index_type, self.count())

    def add_offset(self, size_t v):
        idx_add_offset(self.array.data, self.index_type, self.count(), v); return self

    def sub_offset(self, size_t v):
        idx_sub_offset(self.array.data, self.index_type, self.count(), v); return self

    def fits_type(self, ae_index_type_t idx_type):
        return idx_fits_type(self.array.data, self.count(), self.index_type, idx_type)

    def smallest_fitting_type(self):
        return idx_smallest_fitting_type(self.array.data, self.index_type, self.count())

    def type_convert(self, ae_index_type_t idx_type):
        idx_type_convert(&self.array, self.index_type, idx_type)
        self.index_type = idx_type; return self

    def type_converted(self, ae_index_type_t idx_type):
        cdef Array other = self.__class__(idx_type)
        idx_type_converted(&self.array, self.index_type, &other.array, idx_type)

        return other

    def vertex_cache_optimize_ex(self, size_t vertex_count):
        """
        Re-order indices for more efficient use of graphics hardware vertex caches.
        """
        idx_vertex_cache_optimize_ex( self.array.data, self.index_type,
                                            self.count(), vertex_count)
        return self

    def vertex_cache_optimize(self):
        idx_vertex_cache_optimize(self.array.data, self.index_type, self.count())
        return self

    def vertex_cache_miss_ratio(self, int cache_size=16, bint FIFO=True):
        """
        Measure cache efficiency (and compressibility, kinda) of this index buffer.
        """
        return idx_vertex_cache_miss_ratio(self.array.data, self.index_type,
                                            self.count(), cache_size, FIFO)
