# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
from utl cimport ae_profile_enter, ae_profile_leave

from idx cimport IndexArray, ae_index_type_t
from mem cimport Array, ae_array_t

from vec cimport Vec2, Vec3, Vec4
from mat cimport Mat4x4

import sys # version info

cdef extern from "ae_vertex.h":
    # ===== [ vertex format descriptors ] ======================================

    ctypedef enum ae_vertex_format_t:
        AE_T2F
        AE_T3F
        AE_T4F
        AE_C3F
        AE_C4F
        AE_N3F
        AE_V2F
        AE_V3F
        AE_V4F
        AE_T2F_V3F
        AE_C3F_V3F
        AE_N3F_V3F
        AE_T4F_V4F
        AE_T2F_C3F_V3F
        AE_T2F_N3F_V3F
        AE_C4F_N3F_V3F
        AE_T2F_C4F_N3F_V3F
        AE_T4F_C4F_N3F_V3F

    const char * ae_vertex_format_name[]
    const size_t ae_vertex_format_size[]

    const size_t ae_vertex_format_texcoord_offset[]
    const size_t ae_vertex_format_texcoord_size[]
    const size_t ae_vertex_format_color_offset[]
    const size_t ae_vertex_format_color_size[]
    const size_t ae_vertex_format_normal_offset[]
    const size_t ae_vertex_format_normal_size[]
    const size_t ae_vertex_format_position_offset[]
    const size_t ae_vertex_format_position_size[]

    int ae_vertex_format_has_texcoord(ae_vertex_format_t fmt)
    int ae_vertex_format_has_color(ae_vertex_format_t fmt)
    int ae_vertex_format_has_normal(ae_vertex_format_t fmt)
    int ae_vertex_format_has_position(ae_vertex_format_t fmt)

    size_t ae_vertex_format_element_offset(ae_vertex_format_t fmt, const char* e)
    size_t ae_vertex_format_element_size(ae_vertex_format_t fmt, const char* e)

    int ae_vertex_format_has_element(ae_vertex_format_t fmt, const char* e)

    ae_vertex_format_t ae_vertex_format_from_sizes( const size_t tex_coord,
                                                    const size_t color,
                                                    const size_t normal,
                                                    const size_t position)

    # ===== [ bounding volume construction ] ===================================

    void vtx_aabbox_ex(float* minv, float* maxv, const float* const vertices,
                        const size_t floats, const size_t offset,
                        const size_t stride, const size_t length)

    void vtx_aabbox_flt_ex(float * minv, float* maxv, const float* const vertices,
                    const size_t floats, const size_t offset, const size_t stride)

    void vtx_aabbox2_ex(float minv[2], float maxv[2], const float* const vertices,
                    const size_t floats, const size_t offset, const size_t stride)

    void vtx_aabbox3_ex(float minv[3], float maxv[3], const float* const vertices,
                    const size_t floats, const size_t offset, const size_t stride)

    void vtx_aabbox4_ex(float minv[4], float maxv[4], const float* const vertices,
                    const size_t floats, const size_t offset, const size_t stride)

    void vtx_aabbox_flt( float* minv, float* maxv,
        const float* const v, const size_t floats)

    void vtx_aabbox2(float minv[2], float maxv[2],
        const float* const v, const size_t floats)

    void vtx_aabbox3(float minv[3], float maxv[3],
        const float* const v, const size_t floats)

    void vtx_aabbox4(float minv[4], float maxv[4],
        const float* const v, const size_t floats)

    # ===== [ vector copy ] ====================================================

    void vtx_vec_copy_ex(float* vertices, const float* const val,
                        const size_t floats, const size_t offset,
                        const size_t stride, const size_t length)

    void vtx_flt_copy_ex(float * vertices, const float val, # write the val to pos
                    const size_t floats, const size_t offset, const size_t stride)

    void vtx_vec2copy_ex(float* vertices, const float val[2], # write value to pos
                    const size_t floats, const size_t offset, const size_t stride)

    void vtx_vec3copy_ex(float* vertices, const float val[3], # write value to pos
                    const size_t floats, const size_t offset, const size_t stride)

    void vtx_vec4copy_ex(float* vertices, const float val[4], # write value to pos
                    const size_t floats, const size_t offset, const size_t stride)

    void vtx_flt_copy(float* vertices, const float fvalue, const size_t floats)
    void vtx_vec2copy(float* vertices, const float val[2], const size_t floats)
    void vtx_vec3copy(float* vertices, const float val[3], const size_t floats)
    void vtx_vec4copy(float* vertices, const float val[4], const size_t floats)

    # ===== [ vector epsilon compare ] =========================================

    int vtx_vec_cmp_e_ex(
        const float* const vertices, const float* const value, const float epsilon,
        const size_t floats, const size_t offset,
        const size_t stride, const size_t length)

    int vtx_flt_cmp_e_ex(const float* const vertices, const float value, # float comparison
        const float epsilon, const size_t floats, const size_t offset, const size_t stride)

    int vtx_vec2cmp_e_ex(const float* const vertices, const float value[2], # vec2 compare
        const float epsilon, const size_t floats, const size_t offset, const size_t stride)

    int vtx_vec3cmp_e_ex(const float* const vertices, const float value[3], # vec3 compare
        const float epsilon, const size_t floats, const size_t offset, const size_t stride)

    int vtx_vec4cmp_e_ex(const float* const vertices, const float value[4], # vec4 compare
        const float epsilon, const size_t floats, const size_t offset, const size_t stride)

    int vtx_flt_cmp_e(const float* const vertices, const float value,
                            const float epsilon, const size_t floats)

    int vtx_vec2cmp_e(const float* const vertices, const float value[2],
                            const float epsilon, const size_t floats)

    int vtx_vec3cmp_e(const float* const vertices, const float value[3],
                            const float epsilon, const size_t floats)

    int vtx_vec4cmp_e(const float* const vertices, const float value[4],
                            const float epsilon, const size_t floats)

    # ===== [ vector add ] =====================================================

    void vtx_vec_add_ex(float * vertices, const float* const val,
                        const size_t floats, const size_t offset,
                        const size_t stride, const size_t length)

    void vtx_flt_add_ex(float* vertices, const float fvalue, const size_t floats,
                                        const size_t offset, const size_t stride)

    void vtx_vec2add_ex(float* vertices, const float val[2], const size_t floats,
                                        const size_t offset, const size_t stride)

    void vtx_vec3add_ex(float* vertices, const float val[3], const size_t floats,
                                        const size_t offset, const size_t stride)

    void vtx_vec4add_ex(float* vertices, const float val[4], const size_t floats,
                                        const size_t offset, const size_t stride)

    void vtx_flt_add(float* vertices, const float fvalue, const size_t floats)
    void vtx_vec2add(float* vertices, const float val[2], const size_t floats)
    void vtx_vec3add(float* vertices, const float val[3], const size_t floats)
    void vtx_vec4add(float* vertices, const float val[4], const size_t floats)

    # ===== [ vector mul ] =====================================================

    void vtx_vec_mul_ex(float * vertices, const float* const val,
                        const size_t floats, const size_t offset,
                        const size_t stride, const size_t length)

    void vtx_flt_mul_ex(float* vertices, const float fvalue, const size_t floats,
                                        const size_t offset, const size_t stride)

    void vtx_vec2mul_ex(float* vertices, const float val[2], const size_t floats,
                                        const size_t offset, const size_t stride)

    void vtx_vec3mul_ex(float* vertices, const float val[3], const size_t floats,
                                        const size_t offset, const size_t stride)

    void vtx_vec4mul_ex(float* vertices, const float val[4], const size_t floats,
                                        const size_t offset, const size_t stride)

    void vtx_flt_mul(float* vertices, const float fvalue, const size_t floats)
    void vtx_vec2mul(float* vertices, const float val[2], const size_t floats)
    void vtx_vec3mul(float* vertices, const float val[3], const size_t floats)
    void vtx_vec4mul(float* vertices, const float val[4], const size_t floats)

    # ===== [ vector lerp ] ====================================================

    void vtx_vec_lerp_ex(float* verts, const float* const val, const float t,
                                    const size_t floats, const size_t offset,
                                    const size_t stride, const size_t length)

    void vtx_flt_lerp_ex(float* vertices, const float fval, const float t,
            const size_t floats, const size_t offset, const size_t stride)

    void vtx_vec2lerp_ex(float* vertices, const float v[2], const float t,
            const size_t floats, const size_t offset, const size_t stride)

    void vtx_vec3lerp_ex(float* vertices, const float v[3], const float t,
            const size_t floats, const size_t offset, const size_t stride)

    void vtx_vec4lerp_ex(float* vertices, const float v[4], const float t,
            const size_t floats, const size_t offset, const size_t stride)

    void vtx_flt_lerp(float* verts, const float fval, const float t, const size_t floats)
    void vtx_vec2lerp(float* verts, const float v[2], const float t, const size_t floats)
    void vtx_vec3lerp(float* verts, const float v[3], const float t, const size_t floats)
    void vtx_vec4lerp(float* verts, const float v[4], const float t, const size_t floats)

    # ===== [ array copy ] =====================================================

    void vtx_copy_ex(float* dst, const float* const src, # single-elem copy
        const size_t d_floats, const size_t d_offset, const size_t d_stride,
        const size_t s_floats, const size_t s_offset, const size_t s_stride,
        const size_t length)

    void vtx_copy(float* dst, const float* const src, const size_t floats)

    # ===== [ array epsilon compare ] ==========================================

    int vtx_cmp_e_ex(const float* const a, const float* const b, const float ev,
            const size_t a_floats, const size_t a_offset, const size_t a_stride,
            const size_t b_floats, const size_t b_offset, const size_t b_stride,
            const size_t length)

    int vtx_cmp_e(const float * const a, const float* const b,
                    const float epsilon, const size_t floats)

    # ===== [ array add ] ======================================================

    void vtx_add_ex( float* d, const float * const a, const float * const b,
        const size_t d_floats, const size_t d_offset, const size_t d_stride,
        const size_t a_floats, const size_t a_offset, const size_t a_stride,
        const size_t b_floats, const size_t b_offset, const size_t b_stride,
        const size_t length)

    void vtx_add(float* out, const float* const a,
        const float* const b, const size_t floats)

    # ===== [ array mul ] ======================================================

    void vtx_mul_ex( float* d, const float * const a, const float * const b,
        const size_t d_floats, const size_t d_offset, const size_t d_stride,
        const size_t a_floats, const size_t a_offset, const size_t a_stride,
        const size_t b_floats, const size_t b_offset, const size_t b_stride,
        const size_t length)

    void vtx_mul(float* out, const float* const a,
        const float* const b, const size_t floats)

    # ===== [ array mul-add ] ==================================================

    void vtx_madd_ex(
        float* d, const float* const a, const float* const b, const float s,
        const size_t d_floats, const size_t d_offset, const size_t d_stride,
        const size_t a_floats, const size_t a_offset, const size_t a_stride,
        const size_t b_floats, const size_t b_offset, const size_t b_stride,
        const size_t length)

    void vtx_madd(float* out, const float* const a, const float* const b,
                                    const float s, const size_t floats)

    # ===== [ array lerp ] =====================================================

    void vtx_lerp_ex(
        float* d, const float* const a, const float* const b, const float t,
        const size_t d_floats, const size_t d_offset, const size_t d_stride,
        const size_t a_floats, const size_t a_offset, const size_t a_stride,
        const size_t b_floats, const size_t b_offset, const size_t b_stride,
        const size_t length)

    void vtx_lerp(float* out, const float* const a, const float* const b,
                                    const float t, const size_t floats)

    # ===== [ mat4x4 mul ] =====================================================

    void vtx_mat4x4row_mul_point3_ex(float* vertices, const float mat[16],
            const size_t floats, const size_t offset, const size_t stride)

    void vtx_mat4x4row_mul_point3(float * vertices, const float m[16], const size_t n)

    void vtx_mat4x4row_mul_vector3_ex(float* v, const float m[16], const int normalize,
                        const size_t floats, const size_t offset, const size_t stride)

    void vtx_mat4x4row_mul_vector3(float * vertices, const float mat[16],
                                const int normalize, const size_t floats)

    # ===== [ vertex welding ] =================================================

    ctypedef int (*vtx_weld_func_t)(float* a, float* b, void* data)

    void vtx_weld_func(ae_array_t* vertices, ae_vertex_format_t vtx_format, # shrinks
        ae_array_t* indices, ae_index_type_t idx_type, vtx_weld_func_t F, void* data)

    size_t vtx_weld_func_ex(float* vertices, ae_vertex_format_t vertex_format,
                void* indices, ae_index_type_t index_type, size_t index_count,
                vtx_weld_func_t func, void * data) # returns new vertex count

    void vtx_weld(ae_array_t* vertices, ae_vertex_format_t vtx_format,
        ae_array_t* indices, ae_index_type_t idx_type, float epsilon)

    size_t vtx_weld_ex(float* vertices, ae_vertex_format_t vtx_format, void* indices,
                        ae_index_type_t idx_type, size_t index_count, float epsilon)

    void vtx_weld_dist2(ae_array_t* vertices, ae_vertex_format_t vtx_format,
            ae_array_t* indices, ae_index_type_t idx_type, float threshold2)

    size_t vtx_weld_dist2_ex(float * vertices, ae_vertex_format_t vertex_format,
        void* idx, ae_index_type_t idx_type, size_t idx_count, float threshold2)

    # ===== [ vertex unwelding ] ===============================================

    void vtx_unweld(ae_array_t* vertices, ae_vertex_format_t format,
                    ae_array_t* indices, ae_index_type_t index_type)

    void vtx_unweld_ex(float* pack, float* unpack, ae_vertex_format_t fmt,
                        void* idx, ae_index_type_t index_type, size_t num)

# ==============================================================================
# ~ [ bounding volume construction ]
# ==============================================================================

def aabbox_flt_ex(Array array, size_t offset=0, size_t stride=1):
    cdef float minv
    cdef float maxv

    vtx_aabbox_flt_ex(&minv,&maxv, <const float*>array.array.data,
                array.array.size / sizeof(float), offset, stride)

    return (minv, maxv)

def aabbox2_ex(Array array, size_t offset=0, size_t stride=2):
    cdef Vec2 minv = Vec2()
    cdef Vec2 maxv = Vec2()

    vtx_aabbox2_ex(minv.v, maxv.v, <const float*>array.array.data,
                array.array.size / sizeof(float), offset, stride)

    return (minv, maxv)

def aabbox3_ex(Array array, size_t offset=0, size_t stride=3):
    cdef Vec3 minv = Vec3()
    cdef Vec3 maxv = Vec3()

    vtx_aabbox3_ex(minv.v, maxv.v, <const float*>array.array.data,
                array.array.size / sizeof(float), offset, stride)

    return (minv, maxv)

def aabbox4_ex(Array array, size_t offset=0, size_t stride=4):
    cdef Vec4 minv = Vec4()
    cdef Vec4 maxv = Vec4()

    vtx_aabbox4_ex(minv.v, maxv.v, <const float*>array.array.data,
                array.array.size / sizeof(float), offset, stride)

    return (minv, maxv)

# ===== [ packed ] =============================================================

def aabbox_flt(Array array):
    cdef float minv
    cdef float maxv

    vtx_aabbox_flt(&minv, &maxv, <const float*>array.array.data,
                array.array.size / sizeof(float))

    return (minv, maxv)

def aabbox2(Array array):
    cdef Vec2 minv = Vec2()
    cdef Vec2 maxv = Vec2()

    vtx_aabbox2(minv.v, maxv.v, <const float*>array.array.data,
                array.array.size / sizeof(float))

    return (minv, maxv)

def aabbox3(Array array):
    cdef Vec3 minv = Vec3()
    cdef Vec3 maxv = Vec3()

    vtx_aabbox3(minv.v, maxv.v, <const float*>array.array.data,
                array.array.size / sizeof(float))

    return (minv, maxv)

def aabbox4(Array array):
    cdef Vec4 minv = Vec4()
    cdef Vec4 maxv = Vec4()

    vtx_aabbox4(minv.v, maxv.v, <const float*>array.array.data,
                array.array.size / sizeof(float))

    return (minv, maxv)

# ==============================================================================
# ~ [ vector copy ]
# ==============================================================================

def flt_copy_ex(Array array, float value, size_t offset=0, size_t stride=1):
    vtx_flt_copy_ex(<float *>array.array.data, value,
                    array.array.size / sizeof(float), offset, stride)

def vec2copy_ex(Array array, Vec2 val, size_t offset=0, size_t stride=2):
    vtx_vec2copy_ex(<float *>array.array.data, val.v,
                    array.array.size / sizeof(float), offset, stride)

def vec3copy_ex(Array array, Vec3 val, size_t offset=0, size_t stride=3):
    vtx_vec3copy_ex(<float *>array.array.data, val.v,
                    array.array.size / sizeof(float), offset, stride)

def vec4copy_ex(Array array, Vec4 val, size_t offset=0, size_t stride=4):
    vtx_vec4copy_ex(<float *>array.array.data, val.v,
                    array.array.size / sizeof(float), offset, stride)

# ===== [ packed ] =============================================================

def flt_copy(Array array, float val):
    vtx_flt_copy(<float*>array.array.data, val, array.array.size / sizeof(float))

def vec2copy(Array array, Vec2 v):
    vtx_vec2copy(<float*>array.array.data, v.v, array.array.size / sizeof(float))

def vec3copy(Array array, Vec3 v):
    vtx_vec3copy(<float*>array.array.data, v.v, array.array.size / sizeof(float))

def vec4copy(Array array, Vec4 v):
    vtx_vec4copy(<float*>array.array.data, v.v, array.array.size / sizeof(float))

# ==============================================================================
# ~ [ vector epsilon compare ]
# ==============================================================================

# TODO: flt_cmp_e_ex
# TODO: vec2cmp_e_ex
# TODO: vec3cmp_e_ex
# TODO: vec4cmp_e_ex

# ===== [ packed ] =============================================================

# TODO: flt_cmp_e
# TODO: vec2cmp_e
# TODO: vec3cmp_e
# TODO: vec4cmp_e

# ==============================================================================
# ~ [ vector add ]
# ==============================================================================

def flt_add_ex(Array array, float val, size_t offset=0, size_t stride=1):
    vtx_flt_add_ex(<float*>array.array.data, val,
                    array.array.size / sizeof(float), offset, stride)

def vec2add_ex(Array array, Vec2 val, size_t offset=0, size_t stride=2):
    vtx_vec2add_ex(<float*>array.array.data, val.v,
                    array.array.size / sizeof(float), offset, stride)

def vec3add_ex(Array array, Vec3 val, size_t offset=0, size_t stride=3):
    vtx_vec3add_ex(<float*>array.array.data, val.v,
                    array.array.size / sizeof(float), offset, stride)

def vec4add_ex(Array array, Vec4 val, size_t offset=0, size_t stride=4):
    vtx_vec4add_ex(<float*>array.array.data, val.v,
                    array.array.size / sizeof(float), offset, stride)

# ===== [ packed ] =============================================================

def flt_add(Array array, float val):
    vtx_flt_add(<float*>array.array.data, val, array.array.size / sizeof(float))

def vec2add(Array array, Vec2 v):
    vtx_vec2add(<float*>array.array.data, v.v, array.array.size / sizeof(float))

def vec3add(Array array, Vec3 v):
    vtx_vec3add(<float*>array.array.data, v.v, array.array.size / sizeof(float))

def vec4add(Array array, Vec4 v):
    vtx_vec4add(<float*>array.array.data, v.v, array.array.size / sizeof(float))

# ==============================================================================
# ~ [ vector mul ]
# ==============================================================================

def flt_mul_ex(Array array, float val, size_t offset=0, size_t stride=1):
    vtx_flt_mul_ex(<float*>array.array.data, val,
                    array.array.size / sizeof(float), offset, stride)

def vec2mul_ex(Array array, Vec2 val, size_t offset=0, size_t stride=2):
    vtx_vec2mul_ex(<float*>array.array.data, val.v,
                    array.array.size / sizeof(float), offset, stride)

def vec3mul_ex(Array array, Vec3 val, size_t offset=0, size_t stride=3):
    vtx_vec3mul_ex(<float*>array.array.data, val.v,
                    array.array.size / sizeof(float), offset, stride)

def vec4mul_ex(Array array, Vec4 val, size_t offset=0, size_t stride=4):
    vtx_vec4mul_ex(<float*>array.array.data, val.v,
                    array.array.size / sizeof(float), offset, stride)

# ===== [ packed ] =============================================================

def flt_mul(Array array, float val):
    vtx_flt_mul(<float*>array.array.data, val, array.array.size / sizeof(float))

def vec2mul(Array array, Vec2 v):
    vtx_vec2mul(<float*>array.array.data, v.v, array.array.size / sizeof(float))

def vec3mul(Array array, Vec3 v):
    vtx_vec3mul(<float*>array.array.data, v.v, array.array.size / sizeof(float))

def vec4mul(Array array, Vec4 v):
    vtx_vec4mul(<float*>array.array.data, v.v, array.array.size / sizeof(float))

# ==============================================================================
# ~ [ vector lerp ]
# ==============================================================================

def flt_lerp_ex(Array array, float val, float t, size_t offset=0, size_t stride=1):
    vtx_flt_lerp_ex(<float*>array.array.data, val, t,
                    array.array.size / sizeof(float), offset, stride)

def vec2lerp_ex(Array array, Vec2 v, float t, size_t offset=0, size_t stride=2):
    vtx_vec2lerp_ex(<float*>array.array.data, v.v, t,
                    array.array.size / sizeof(float), offset, stride)

def vec3lerp_ex(Array array, Vec3 v, float t, size_t offset=0, size_t stride=3):
    vtx_vec3lerp_ex(<float*>array.array.data, v.v, t,
                    array.array.size / sizeof(float), offset, stride)

def vec4lerp_ex(Array array, Vec4 v, float t, size_t offset=0, size_t stride=4):
    vtx_vec4lerp_ex(<float*>array.array.data, v.v, t,
                    array.array.size / sizeof(float), offset, stride)

# ===== [ packed ] =============================================================

def flt_lerp(Array array, float val, float t):
    vtx_flt_lerp(<float*>array.array.data, val, t, array.array.size / sizeof(float))

def vec2lerp(Array array, Vec2 v, float t):
    vtx_vec2lerp(<float*>array.array.data, v.v, t, array.array.size / sizeof(float))

def vec3lerp(Array array, Vec3 v, float t):
    vtx_vec3lerp(<float*>array.array.data, v.v, t, array.array.size / sizeof(float))

def vec4lerp(Array array, Vec4 v, float t):
    vtx_vec4lerp(<float*>array.array.data, v.v, t, array.array.size / sizeof(float))

# ==============================================================================
# ~ [ array copy ]
# ==============================================================================

# TODO: copy_ex
# TODO: copy

# ==============================================================================
# ~ [ array epsilon compare ]
# ==============================================================================

# TODO: cmp_e_ex
# TODO: cmp_e

# ==============================================================================
# ~ [ array add ]
# ==============================================================================

# TODO: add_ex
# TODO: add

# ==============================================================================
# ~ [ array mul ]
# ==============================================================================

# TODO: mul_ex
# TODO: mul

# ==============================================================================
# ~ [ array mul-add ]
# ==============================================================================

# TODO: madd_ex
# TODO: madd

# ==============================================================================
# ~ [ array lerp ]
# ==============================================================================

# TODO: lerp_ex
# TODO: lerp

# ==============================================================================
# ~ [ mat4x4 mul ]
# ==============================================================================

def mat4x4row_mul_point3_ex(Array array, Mat4x4 m, size_t offset=0, size_t stride=3):
    """
    Transform a strided array of points. Use vec mul/add for faster scale/translate.
    """
    vtx_mat4x4row_mul_point3_ex(<float*>array.array.data, m.m,
            array.array.size / sizeof(float), offset, stride)

def mat4x4row_mul_point3(Array a, Mat4x4 m):
    """Transform a packed point array. Use vec mul/add for faster scale/translate."""
    vtx_mat4x4row_mul_point3(<float*>a.array.data, m.m, a.array.size / sizeof(float))

# ===== [ vectors ] ============================================================

def mat4x4row_mul_vector3_ex(Array array, Mat4x4 m, bint normalize,
                                size_t offset=0, size_t stride=3 ):
    """
    Transform a strided array of vectors, optionally re-normalizing them.
    """
    vtx_mat4x4row_mul_vector3_ex(<float*>array.array.data, m.m, normalize,
                        array.array.size / sizeof(float), offset, stride)

def mat4x4row_mul_vector3(Array array, Mat4x4 m, bint normalize):
    """
    Transform a packed array of vectors, optionally re-normalizing them.
    """
    vtx_mat4x4row_mul_vector3(<float*>array.array.data, m.m,
                normalize, array.array.size / sizeof(float))

# ==============================================================================
# ~ [ vertex welding ]
# ==============================================================================

def weld(Array vertices, ae_vertex_format_t vertex_format,
    Array indices, ae_index_type_t index_type, float tol):
    """
        Replace duplicate adjacent verts with int offsets (builds idx buffer).
    """
    vtx_weld(&vertices.array, vertex_format, &indices.array, index_type, tol)

def weld_dist2(Array vertices, ae_vertex_format_t vertex_format,
        Array indices, ae_index_type_t index_type, float dist2):
    """
        Replace nearby verts (distance^2 compare that ignores texcoords etc).
    """
    vtx_weld_dist2(&vertices.array, vertex_format,
                &indices.array, index_type, dist2)

# ==============================================================================
# ~ [ vertex unwelding ]
# ==============================================================================

def unweld(Array verts, ae_vertex_format_t v_format,
            Array inds, ae_index_type_t index_type):
    vtx_unweld(&verts.array, v_format, &inds.array, index_type)

# ==============================================================================
# ~ [ wrapper class ]
# ==============================================================================

cdef class VertexArray(Array):
    cdef public ae_vertex_format_t vertex_format

    T2F = AE_T2F
    T3F = AE_T3F
    T4F = AE_T4F
    C3F = AE_C3F
    C4F = AE_C4F
    N3F = AE_N3F
    V2F = AE_V2F
    V3F = AE_V3F
    V4F = AE_V4F

    T2F_V3F = AE_T2F_V3F
    C3F_V3F = AE_C3F_V3F
    N3F_V3F = AE_N3F_V3F
    T4F_V4F = AE_T4F_V4F

    T2F_C3F_V3F = AE_T2F_C3F_V3F
    T2F_N3F_V3F = AE_T2F_N3F_V3F
    C4F_N3F_V3F = AE_C4F_N3F_V3F

    T2F_C4F_N3F_V3F = AE_T2F_C4F_N3F_V3F
    T4F_C4F_N3F_V3F = AE_T4F_C4F_N3F_V3F

    def __init__(self, ae_vertex_format_t vertex_format):
        self.vertex_format = vertex_format

    def __repr__(self):
        return "{}({} size, {} cap, {})".format( self.__class__.__name__,
                    self.array.size, self.array._cap, self.format_name())

    def get_subview(self, size_t offset, size_t length, object array_t=VertexArray):
        return Array.get_subview(self, offset, length, array_t)

    def chunk(self, size_t chunk_size=0, object chunk_t=VertexArray):
        """Splits self into equally-sized chunks. Defaults to vertex size."""

        cdef size_t size = chunk_size if chunk_size != 0 else \
            ae_vertex_format_size[<size_t>self.vertex_format] * sizeof(float)

        return Array.chunk(self, size, chunk_t)

    def get_region(self, size_t offset, size_t length, object array_t=VertexArray):
        return Array.get_region(self, offset, length, array_t)

    property ctor_args:
        def __get__(self): return (self.vertex_format,) # for copying & pickling

    def format_name(self):
        """
        Get the name string of this array's vertex format (without the AE prefix).
        """
        if sys.version_info.major > 2:
            return ae_vertex_format_name[<size_t>self.vertex_format][3:].decode()
        else:
            return ae_vertex_format_name[<size_t>self.vertex_format][3:]

    def format_size(self):
        return ae_vertex_format_size[<size_t>self.vertex_format]

    def format_bytes(self):
        return ae_vertex_format_size[<size_t>self.vertex_format] * sizeof(float)

    cpdef size_t count(self):
        return (self.array.size // ( sizeof(float) * # div by bytes
                ae_vertex_format_size[<size_t>self.vertex_format]))

    cpdef size_t triangle_count(self):
        return self.count() // 3

    cpdef size_t quad_count(self):
        return self.count() // 4

    cpdef size_t texcoord_offset(self):
        return ae_vertex_format_texcoord_offset[<size_t>self.vertex_format]

    cpdef size_t texcoord_size(self):
        return ae_vertex_format_texcoord_size[<size_t>self.vertex_format]

    cpdef size_t color_offset(self):
        return ae_vertex_format_color_offset[<size_t>self.vertex_format]

    cpdef size_t color_size(self):
        return ae_vertex_format_color_size[<size_t>self.vertex_format]

    cpdef size_t normal_offset(self):
        return ae_vertex_format_normal_offset[<size_t>self.vertex_format]

    cpdef size_t normal_size(self):
        return ae_vertex_format_normal_size[<size_t>self.vertex_format]

    cpdef size_t position_offset(self):
        return ae_vertex_format_position_offset[<size_t>self.vertex_format]

    cpdef size_t position_size(self):
        return ae_vertex_format_position_size[<size_t>self.vertex_format]

    def has_texcoord(self):
        return ae_vertex_format_has_texcoord(self.vertex_format)

    def has_color(self):
        return ae_vertex_format_has_color(self.vertex_format)

    def has_normal(self):
        return ae_vertex_format_has_normal(self.vertex_format)

    def has_position(self):
        return ae_vertex_format_has_position(self.vertex_format)

    def element_offset(self, str element):
        """
        Get the float offset of a given vertex element (color, normal, etc).
        For example, the `v` (vertex pos) offset of T2F_N3F_V3F would be 5.
        """
        cdef bytes e

        if sys.version_info.major > 2:
            e = <bytes>element.encode('utf-8')
        else:
            e = <bytes>element

        return ae_vertex_format_element_offset(self.vertex_format, <char*>e)

    def element_size(self, str element):
        """
        Get the float count of a given vertex element (color, normal, etc).
        For example, the `t` (texture coords) size of T2F_V3F would be 2.
        """
        cdef bytes e

        if sys.version_info.major > 2:
            e = <bytes>element.encode('utf-8')
        else:
            e = <bytes>element

        return ae_vertex_format_element_size(self.vertex_format, <char*>e)

    def has_element(self, str element):
        """
        Returns true if the vertex format of self contains a given element.
        """
        cdef bytes e

        if sys.version_info.major > 2:
            e = <bytes>element.encode('utf-8')
        else:
            e = <bytes>element

        return ae_vertex_format_has_element(self.vertex_format, <char*>e)

    @staticmethod
    def format_from_sizes(size_t tex, size_t col, size_t norm, size_t pos):
        """
        Given a vertex `descriptor` consisting of vertex element size information, this
        function returns a vertex format enumerant (i.e. f(2, 0, 3, 3) -> T2F_N3F_V3F).
        """
        return ae_vertex_format_from_sizes(tex, col, norm, pos)

    def apply_to_element(self, str element, func, *a, **k):
        """
        Apply a function to each vertex element in an array (i.e. every normal).
        Other methods may do what you want much faster! This is extremely slow!
        """
        cdef bytes e

        cdef void* ctx = ae_profile_enter("aegame/vtx.pyx",
                            "VertexArray.apply_to_element")
        cdef size_t e_off
        cdef size_t e_num

        cdef size_t v_len = ae_vertex_format_size[<size_t>self.vertex_format]
        cdef size_t count = self.array.size // sizeof(float)

        if sys.version_info.major > 2:
            e = <bytes>element.encode('utf-8')
        else:
            e = <bytes>element

        e_off = ae_vertex_format_element_offset(self.vertex_format, <char*>e)
        e_num = ae_vertex_format_element_size(self.vertex_format, <char*>e)

        if (e_num == 2): v_get, v_set = self.get_vec2, self.set_vec2
        elif e_num == 3: v_get, v_set = self.get_vec3, self.set_vec3
        elif e_num == 4: v_get, v_set = self.get_vec4, self.set_vec4
        else: assert 0, "element \"{}\" size == {}".format(e, e_num)

        for i in range(e_off, count, v_len): v_set(i, func(v_get(i), *a, **k))
        ae_profile_leave(ctx); return self # apply func to elem in each vertex

    def aabbox2(self):
        cdef const float* d = <const float *>(self.array.data)
        cdef size_t nfloats = self.array.size // sizeof(float)

        cdef Vec2 minv = Vec2()
        cdef Vec2 maxv = Vec2()

        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.position_size() == 2, "bad vert format: {}".format(self)

        if size == 2:
            vtx_aabbox2(minv.v, maxv.v, d, nfloats) # maybe calls a faster simd path
        else:
            vtx_aabbox2_ex(minv.v, maxv.v, d, nfloats, self.position_offset(), size)

        return (minv, maxv)

    def aabbox3(self):
        cdef const float* d = <const float *>(self.array.data)
        cdef size_t nfloats = self.array.size // sizeof(float)

        cdef Vec3 minv = Vec3()
        cdef Vec3 maxv = Vec3()

        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.position_size() == 3, "bad vert format: {}".format(self)

        if size == 3:
            vtx_aabbox3(minv.v, maxv.v, d, nfloats) # maybe calls a faster simd path
        else:
            vtx_aabbox3_ex(minv.v, maxv.v, d, nfloats, self.position_offset(), size)

        return (minv, maxv)

    def aabbox4(self):
        cdef const float* d = <const float *>(self.array.data)
        cdef size_t nfloats = self.array.size // sizeof(float)

        cdef Vec4 minv = Vec4()
        cdef Vec4 maxv = Vec4()

        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.position_size() == 4, "bad vert format: {}".format(self)

        if size == 4:
            vtx_aabbox4(minv.v, maxv.v, d, nfloats) # maybe calls a faster simd path
        else:
            vtx_aabbox4_ex(minv.v, maxv.v, d, nfloats, self.position_offset(), size)

        return (minv, maxv)

    def vec2copy(self, str elem, Vec2 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 2, "wrong format: {}".format(self)

        if size == 2:
            vtx_vec2copy( <float*> (self.array.data), v.v, self.array.size // sizeof(float))
        else:
            vtx_vec2copy_ex( <float*> self.array.data, v.v, self.array.size // sizeof(float),
                                                            self.element_offset(elem), size)

        return self

    def vec3copy(self, str elem, Vec3 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 3, "wrong format: {}".format(self)

        if size == 3:
            vtx_vec3copy( <float*> (self.array.data), v.v, self.array.size // sizeof(float))
        else:
            vtx_vec3copy_ex( <float*> self.array.data, v.v, self.array.size // sizeof(float),
                                                            self.element_offset(elem), size)

        return self

    def vec4copy(self, str elem, Vec4 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 4, "wrong format: {}".format(self)

        if size == 4:
            vtx_vec4copy( <float*> (self.array.data), v.v, self.array.size // sizeof(float))
        else:
            vtx_vec4copy_ex( <float*> self.array.data, v.v, self.array.size // sizeof(float),
                                                            self.element_offset(elem), size)

        return self

    def vec2cmp_e(self, str elem, Vec2 v, float epsilon):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 2, "wrong format: {}".format(self)

        if size == 2:
            return vtx_vec2cmp_e(<const float* const>self.array.data, v.v,
                                epsilon, self.array.size // sizeof(float))

        return vtx_vec2cmp_e_ex(<const float * const>self.array.data, v.v,
                                epsilon, self.array.size // sizeof(float),

                                # TODO: remove this function call overhead
                                self.element_offset(elem), size)

    def vec3cmp_e(self, str elem, Vec3 v, float epsilon):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 3, "wrong format: {}".format(self)

        if size == 3:
            return vtx_vec3cmp_e(<const float* const>self.array.data, v.v,
                                epsilon, self.array.size // sizeof(float))

        return vtx_vec3cmp_e_ex(<const float * const>self.array.data, v.v,
                                epsilon, self.array.size // sizeof(float),

                                # TODO: remove this function call overhead
                                self.element_offset(elem), size)

    def vec4cmp_e(self, str elem, Vec4 v, float epsilon):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 4, "wrong format: {}".format(self)

        if size == 4:
            return vtx_vec4cmp_e(<const float* const>self.array.data, v.v,
                                epsilon, self.array.size // sizeof(float))

        return vtx_vec4cmp_e_ex(<const float * const>self.array.data, v.v,
                                epsilon, self.array.size // sizeof(float),

                                # TODO: remove this function call overhead
                                self.element_offset(elem), size)

    def vec2add(self, str elem, Vec2 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 2, "wrong format: {}".format(self)

        if size == 2:
            vtx_vec2add(<float*>self.array.data, v.v, self.array.size // sizeof(float))
        else:
            vtx_vec2add_ex(<float*>self.array.data, # TODO avoid elem offset method call
                v.v, self.array.size // sizeof(float), self.element_offset(elem), size)

        return self

    def vec3add(self, str elem, Vec3 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 3, "wrong format: {}".format(self)

        if size == 3:
            vtx_vec3add(<float*>self.array.data, v.v, self.array.size // sizeof(float))
        else:
            vtx_vec3add_ex(<float*>self.array.data, # TODO avoid elem offset method call
                v.v, self.array.size // sizeof(float), self.element_offset(elem), size)

        return self

    def vec4add(self, str elem, Vec4 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 4, "wrong format: {}".format(self)

        if size == 4:
            vtx_vec4add(<float*>self.array.data, v.v, self.array.size // sizeof(float))
        else:
            vtx_vec4add_ex(<float*>self.array.data, # TODO avoid elem offset method call
                v.v, self.array.size // sizeof(float), self.element_offset(elem), size)

        return self

    def vec2translate(self, Vec2 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.position_size() == 2, "bad vert format: {}".format(self)

        if size == 2:
            vtx_vec2add(<float*>self.array.data, v.v,
                    self.array.size // sizeof(float))
        else:
            vtx_vec2add_ex( <float *>self.array.data, v.v, self.array.size \
                            // sizeof(float), self.position_offset(), size)

        return self

    def vec3translate(self, Vec3 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.position_size() == 3, "bad vert format: {}".format(self)

        if size == 3:
            vtx_vec3add(<float*>self.array.data, v.v,
                    self.array.size // sizeof(float))
        else:
            vtx_vec3add_ex( <float *>self.array.data, v.v, self.array.size \
                            // sizeof(float), self.position_offset(), size)

        return self

    def vec4translate(self, Vec4 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.position_size() == 4, "bad vert format: {}".format(self)

        if size == 4:
            vtx_vec4add(<float*>self.array.data, v.v,
                    self.array.size // sizeof(float))
        else:
            vtx_vec4add_ex( <float *>self.array.data, v.v, self.array.size \
                            // sizeof(float), self.position_offset(), size)

        return self

    def vec2mul(self, str elem, Vec2 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 2, "wrong format: {}".format(self)

        if size == 2:
            vtx_vec2mul(<float*>self.array.data, v.v, self.array.size // sizeof(float))
        else:
            vtx_vec2mul_ex(<float*>self.array.data, # TODO avoid elem offset method call
                v.v, self.array.size // sizeof(float), self.element_offset(elem), size)

        return self

    def vec3mul(self, str elem, Vec3 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 3, "wrong format: {}".format(self)

        if size == 3:
            vtx_vec3mul(<float*>self.array.data, v.v, self.array.size // sizeof(float))
        else:
            vtx_vec3mul_ex(<float*>self.array.data, # TODO avoid elem offset method call
                v.v, self.array.size // sizeof(float), self.element_offset(elem), size)

        return self

    def vec4mul(self, str elem, Vec4 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 4, "wrong format: {}".format(self)

        if size == 4:
            vtx_vec4mul(<float*>self.array.data, v.v, self.array.size // sizeof(float))
        else:
            vtx_vec4mul_ex(<float*>self.array.data, # TODO avoid elem offset method call
                v.v, self.array.size // sizeof(float), self.element_offset(elem), size)

        return self

    def vec2scale(self, Vec2 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.position_size() == 2, "bad vert format: {}".format(self)

        if size == 2:
            vtx_vec2mul(<float*>self.array.data, v.v,
                    self.array.size // sizeof(float))
        else:
            vtx_vec2mul_ex( <float *>self.array.data, v.v, self.array.size \
                            // sizeof(float), self.position_offset(), size)

        return self

    def vec3scale(self, Vec3 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.position_size() == 3, "bad vert format: {}".format(self)

        if size == 3:
            vtx_vec3mul(<float*>self.array.data, v.v,
                    self.array.size // sizeof(float))
        else:
            vtx_vec3mul_ex( <float *>self.array.data, v.v, self.array.size \
                            // sizeof(float), self.position_offset(), size)

        return self

    def vec4scale(self, Vec4 v):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.position_size() == 4, "bad vert format: {}".format(self)

        if size == 4:
            vtx_vec4mul(<float*>self.array.data, v.v,
                    self.array.size // sizeof(float))
        else:
            vtx_vec4mul_ex( <float *>self.array.data, v.v, self.array.size \
                            // sizeof(float), self.position_offset(), size)

        return self

    def vec2lerp(self, bytes elem, Vec2 v, float t):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 2, "wrong format: {}".format(self)

        if size == 2:
            vtx_vec2lerp(<float*>self.array.data, v.v, t, self.array.size // sizeof(float))
        else:
            vtx_vec2lerp_ex(<float*>self.array.data, v.v, t, self.array.size // sizeof(float),
                    ae_vertex_format_element_offset(self.vertex_format, <char*>elem), size)

        return self

    def vec3lerp(self, bytes elem, Vec3 v, float t):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 3, "wrong format: {}".format(self)

        if size == 3:
            vtx_vec3lerp(<float*>self.array.data, v.v, t, self.array.size // sizeof(float))
        else:
            vtx_vec3lerp_ex(<float*>self.array.data, v.v, t, self.array.size // sizeof(float),
                    ae_vertex_format_element_offset(self.vertex_format, <char*>elem), size)

        return self

    def vec4lerp(self, bytes elem, Vec4 v, float t):
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.element_size(elem) == 4, "wrong format: {}".format(self)

        if size == 4:
            vtx_vec4lerp(<float*>self.array.data, v.v, t, self.array.size // sizeof(float))
        else:
            vtx_vec4lerp_ex(<float*>self.array.data, v.v, t, self.array.size // sizeof(float),
                    ae_vertex_format_element_offset(self.vertex_format, <char*>elem), size)

        return self

    def copy_ex(self, bytes elem, VertexArray src):
        assert self.element_size(elem) == src.element_size(elem), "dst: "\
            "{} src: {} element: {} doesn't match".format(self, src, elem)

        vtx_copy_ex( <float *> self.array.data,
            <const float* const>src.array.data,

            self.array.size / sizeof(float),
            ae_vertex_format_element_offset(self.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>self.vertex_format],

            src.array.size / sizeof(float),
            ae_vertex_format_element_offset(src.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>src.vertex_format],

            ae_vertex_format_element_size(self.vertex_format, <char*>elem))

        return self

    def cmp_e_ex(self, bytes elem, VertexArray other, float epsilon):
        assert self.element_size(elem) == other.element_size(elem), "self: " \
            "{} other: {} element: {} doesn't match".format(self, other, elem)

        return vtx_cmp_e_ex(
            <const float* const>self.array.data,
            <const float* const>other.array.data,
            epsilon,

            self.array.size / sizeof(float),
            ae_vertex_format_element_offset(self.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>self.vertex_format],

            other.array.size / sizeof(float),
            ae_vertex_format_element_offset(other.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>other.vertex_format],

            ae_vertex_format_element_size(self.vertex_format, <char*>elem))

    def cmp_e(self, VertexArray other, float epsilon):
        assert len(self) == len(other), "{} {}".format(self, other)

        return vtx_cmp_e(<const float* const>self.array.data,
                        <const float* const>other.array.data,
                        epsilon,
                        self.array.size / sizeof(float))

    def add_ex(self, bytes elem, VertexArray a, VertexArray b):
        vtx_add_ex( <float*> self.array.data,
            <const float* const>a.array.data,
            <const float* const>b.array.data,

            self.array.size / sizeof(float),
            ae_vertex_format_element_offset(self.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>self.vertex_format],

            a.array.size / sizeof(float),
            ae_vertex_format_element_offset(a.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>a.vertex_format],

            b.array.size / sizeof(float),
            ae_vertex_format_element_offset(b.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>b.vertex_format],

            # TODO: we should probably assert that A and B sizes match this
            ae_vertex_format_element_size(self.vertex_format, <char*>elem))

        return self

    def add(self, VertexArray a, VertexArray b):
        assert len(self) == len(a) == len(b), "{} {} {}".format(self, a, b)

        vtx_add( < float* >  self.array.data,
            <const float* const>a.array.data,
            <const float* const>b.array.data,
            self.array.size / sizeof(float))

        return self

    def mul_ex(self, bytes elem, VertexArray a, VertexArray b):
        vtx_mul_ex( <float*> self.array.data,
            <const float* const>a.array.data,
            <const float* const>b.array.data,

            self.array.size / sizeof(float),
            ae_vertex_format_element_offset(self.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>self.vertex_format],

            a.array.size / sizeof(float),
            ae_vertex_format_element_offset(a.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>a.vertex_format],

            b.array.size / sizeof(float),
            ae_vertex_format_element_offset(b.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>b.vertex_format],

            # TODO: we should probably assert that A and B sizes match this
            ae_vertex_format_element_size(self.vertex_format, <char*>elem))

        return self

    def mul(self, VertexArray a, VertexArray b):
        assert len(self) == len(a) == len(b), "{} {} {}".format(self, a, b)

        vtx_mul( < float* >  self.array.data,
            <const float* const>a.array.data,
            <const float* const>b.array.data,
            self.array.size / sizeof(float))

        return self

    def madd_ex(self, bytes elem, VertexArray a, VertexArray b, float s):
        vtx_madd_ex( <float*>self.array.data,
            <const float* const>a.array.data,
            <const float* const>b.array.data,
            s,

            self.array.size / sizeof(float),
            ae_vertex_format_element_offset(self.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>self.vertex_format],

            a.array.size / sizeof(float),
            ae_vertex_format_element_offset(a.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>a.vertex_format],

            b.array.size / sizeof(float),
            ae_vertex_format_element_offset(b.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>b.vertex_format],

            # TODO: we should probably assert that A and B sizes match this
            ae_vertex_format_element_size(self.vertex_format, <char*>elem))

        return self

    def madd(self, VertexArray a, VertexArray b, float s):
        assert len(self) == len(a) == len(b), "{} {} {}".format(self, a, b)

        vtx_madd(
            <float*>self.array.data,
            <const float* const>a.array.data,
            <const float* const>b.array.data,
            s,
            self.array.size / sizeof(float))

        return self

    def lerp_ex(self, bytes elem, VertexArray a, VertexArray b, float t):
        vtx_lerp_ex( <float*>self.array.data,
            <const float* const>a.array.data,
            <const float* const>b.array.data,
            t,

            self.array.size / sizeof(float),
            ae_vertex_format_element_offset(self.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>self.vertex_format],

            a.array.size / sizeof(float),
            ae_vertex_format_element_offset(a.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>a.vertex_format],

            b.array.size / sizeof(float),
            ae_vertex_format_element_offset(b.vertex_format, <char*>elem),
            ae_vertex_format_size[<size_t>b.vertex_format],

            # TODO: we should probably assert that A and B sizes match this
            ae_vertex_format_element_size(self.vertex_format, <char*>elem))

        return self

    def lerp(self, VertexArray a, VertexArray b, float t):
        assert len(self) == len(a) == len(b), "{} {} {}".format(self, a, b)

        vtx_lerp(
            <float*>self.array.data,
            <const float* const>a.array.data,
            <const float* const>b.array.data,
            t,
            self.array.size / sizeof(float))

        return self

    def mat4x4row_transform(self, Mat4x4 m, bint normalize):
        # TODO: transform normals and positions in one pass for better cache use
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]

        if ae_vertex_format_has_normal(self.vertex_format):
            assert self.normal_size() == 3, "invalid format: {}".format(self)

            if size == 3:
                vtx_mat4x4row_mul_vector3( <float*>self.array.data, m.m,
                            normalize, self.array.size // sizeof(float))
            else:
                vtx_mat4x4row_mul_vector3_ex(<float*>self.array.data, m.m, normalize,
                        self.array.size // sizeof(float), self.normal_offset(), size)

        if ae_vertex_format_has_position(self.vertex_format):
            assert self.position_size() == 3, "invalid format: {}".format(self)

            if size == 3:
                vtx_mat4x4row_mul_point3(<float*>self.array.data, m.m,
                    self.array.size // sizeof(float)) # sse fast path?
            else:
                vtx_mat4x4row_mul_point3_ex( <float*>self.array.data, m.m, # points
                    self.array.size // sizeof(float), self.position_offset(), size)

        return self

    def weld(self, IndexArray indices, float epsilon):
        vtx_weld(&self.array, self.vertex_format, &indices.array,
                                    indices.index_type, epsilon)
        return self

    def weld_dist2(self, IndexArray indices, float threshold2):
        vtx_weld_dist2(&self.array, self.vertex_format, &indices.array,
                                        indices.index_type, threshold2)
        return self

    def unweld(self, IndexArray idx):
        vtx_unweld(&self.array, self.vertex_format, &idx.array, idx.index_type)
        return self # remap indexed vert buffer into "raw" (non-indexed) verts
