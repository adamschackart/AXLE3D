/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

size_t ae_vertex_format_element_offset(const ae_vertex_format_t fmt, const char* elem)
{
    AE_STUB(); return 0;
}

size_t ae_vertex_format_element_size(const ae_vertex_format_t fmt, const char* elem)
{
    AE_STUB(); return 0;
}

ae_vertex_format_t
ae_vertex_format_from_sizes(const size_t tex_coord, const size_t color,
                            const size_t normal, const size_t position)
{
    AE_STUB(); return AE_VERTEX_FORMAT_COUNT;
}

void vtx_aabbox_ex(float* minv, float* maxv, const float* const vertices,
                                const size_t floats, const size_t offset,
                                const size_t stride, const size_t length)
{
    AE_STUB();
}

void vtx_aabbox_flt_ex(float * minv, float* maxv, const float* const vertices,
                const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_aabbox2_ex(float minv[2], float maxv[2], const float* const vertices,
                const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_aabbox3_ex(float minv[3], float maxv[3], const float* const vertices,
                const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_aabbox4_ex(float minv[4], float maxv[4], const float* const vertices,
                const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_aabbox_flt(float* minv, float* maxv, // packed
        const float* const verts, const size_t floats)
{
    AE_STUB();
}

void vtx_aabbox2(float minv[2], float maxv[2], // packed fast path
                const float * const vertices, const size_t floats)
{
    AE_STUB();
}

void vtx_aabbox3(float minv[3], float maxv[3], // packed fast path
                const float * const vertices, const size_t floats)
{
    AE_STUB();
}

void vtx_aabbox4(float minv[4], float maxv[4], // packed fast path
                const float * const vertices, const size_t floats)
{
    AE_STUB();
}

void vtx_vec_copy_ex(float* vertices, const float* const val,
                    const size_t floats, const size_t offset,
                    const size_t stride, const size_t length)
{
    AE_STUB();
}

void vtx_flt_copy_ex(float * vertices, const float val, // write value to pos
                const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_vec2copy_ex(float* vertices, const float val[2], // write val to pos
                const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_vec3copy_ex(float* vertices, const float val[3], // write val to pos
                const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_vec4copy_ex(float* vertices, const float val[4], // write val to pos
                const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_flt_copy(float* vertices, const float fvalue, const size_t floats)
{
    AE_STUB();
}

void vtx_vec2copy(float* vertices, const float val[2], const size_t floats)
{
    AE_STUB();
}

void vtx_vec3copy(float* vertices, const float val[3], const size_t floats)
{
    AE_STUB();
}

void vtx_vec4copy(float* vertices, const float val[4], const size_t floats)
{
    AE_STUB();
}

int vtx_vec_cmp_e_ex(
    const float* const vertices, const float* const value, const float epsilon,
    const size_t floats, const size_t offset,
    const size_t stride, const size_t length)
{
    AE_STUB(); return 0;
}

int vtx_flt_cmp_e_ex(const float* const vertices, const float value, // float comparison
    const float epsilon, const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB(); return 0;
}

int vtx_vec2cmp_e_ex(const float* const vertices, const float value[2], // vec2 compare
    const float epsilon, const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB(); return 0;
}

int vtx_vec3cmp_e_ex(const float* const vertices, const float value[3], // vec3 compare
    const float epsilon, const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB(); return 0;
}

int vtx_vec4cmp_e_ex(const float* const vertices, const float value[4], // vec4 compare
    const float epsilon, const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB(); return 0;
}

int vtx_flt_cmp_e(const float* const vertices, const float value,
                        const float epsilon, const size_t floats)
{
    AE_STUB(); return 0;
}

int vtx_vec2cmp_e(const float* const vertices, const float value[2],
                        const float epsilon, const size_t floats)
{
    AE_STUB(); return 0;
}

int vtx_vec3cmp_e(const float* const vertices, const float value[3],
                        const float epsilon, const size_t floats)
{
    AE_STUB(); return 0;
}

int vtx_vec4cmp_e(const float* const vertices, const float value[4],
                        const float epsilon, const size_t floats)
{
    AE_STUB(); return 0;
}

void vtx_vec_add_ex(float * vertices, const float* const val,
                    const size_t floats, const size_t offset,
                    const size_t stride, const size_t length)
{
    AE_STUB();
}

void vtx_flt_add_ex(float* vertices, const float fvalue, const size_t floats,
                                    const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_vec2add_ex(float* vertices, const float val[2], const size_t floats,
                                    const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_vec3add_ex(float* vertices, const float val[3], const size_t floats,
                                    const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_vec4add_ex(float* vertices, const float val[4], const size_t floats,
                                    const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_flt_add(float* vertices, const float fvalue, const size_t floats)
{
    AE_STUB();
}

void vtx_vec2add(float* vertices, const float val[2], const size_t floats)
{
    AE_STUB();
}

void vtx_vec3add(float* vertices, const float val[3], const size_t floats)
{
    AE_STUB();
}

void vtx_vec4add(float* vertices, const float val[4], const size_t floats)
{
    AE_STUB();
}

void vtx_vec_mul_ex(float * vertices, const float* const val,
                    const size_t floats, const size_t offset,
                    const size_t stride, const size_t length)
{
    AE_STUB();
}

void vtx_flt_mul_ex(float* vertices, const float fvalue, const size_t floats,
                                    const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_vec2mul_ex(float* vertices, const float val[2], const size_t floats,
                                    const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_vec3mul_ex(float* vertices, const float val[3], const size_t floats,
                                    const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_vec4mul_ex(float* vertices, const float val[4], const size_t floats,
                                    const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_flt_mul(float* vertices, const float fvalue, const size_t floats)
{
    AE_STUB();
}

void vtx_vec2mul(float* vertices, const float val[2], const size_t floats)
{
    AE_STUB();
}

void vtx_vec3mul(float* vertices, const float val[3], const size_t floats)
{
    AE_STUB();
}

void vtx_vec4mul(float* vertices, const float val[4], const size_t floats)
{
    AE_STUB();
}

void vtx_vec_lerp_ex(float* verts, const float* const val, const float t,
                                const size_t floats, const size_t offset,
                                const size_t stride, const size_t length)
{
    AE_STUB();
}

void vtx_flt_lerp_ex(float* vertices, const float value, const float t,
        const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_vec2lerp_ex(float* vertices, const float v[2], const float t,
        const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_vec3lerp_ex(float* vertices, const float v[3], const float t,
        const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_vec4lerp_ex(float* vertices, const float v[4], const float t,
        const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_flt_lerp(float* verts, const float fval, const float t, const size_t floats)
{
    AE_STUB();
}

void vtx_vec2lerp(float* verts, const float v[2], const float t, const size_t floats)
{
    AE_STUB();
}

void vtx_vec3lerp(float* verts, const float v[3], const float t, const size_t floats)
{
    AE_STUB();
}

void vtx_vec4lerp(float* verts, const float v[4], const float t, const size_t floats)
{
    AE_STUB();
}

void vtx_copy_ex(float* dst, const float * const src, // strided element copy
    const size_t dst_floats, const size_t dst_offset, const size_t dst_stride,
    const size_t src_floats, const size_t src_offset, const size_t src_stride,
    const size_t length)
{
    AE_STUB();
}

void vtx_copy(float* dst, const float* const src, const size_t floats)
{
    AE_STUB();
}

int vtx_cmp_e_ex(const float* const a, const float* const b, const float ev,
        const size_t a_floats, const size_t a_offset, const size_t a_stride,
        const size_t b_floats, const size_t b_offset, const size_t b_stride,
        const size_t length)
{
    AE_STUB(); return 0;
}

int vtx_cmp_e(const float * const a, const float* const b,
                const float epsilon, const size_t floats)
{
    AE_STUB(); return 0;
}

void vtx_add_ex( float* d, const float * const a, const float * const b,
    const size_t d_floats, const size_t d_offset, const size_t d_stride,
    const size_t a_floats, const size_t a_offset, const size_t a_stride,
    const size_t b_floats, const size_t b_offset, const size_t b_stride,
    const size_t length)
{
    AE_STUB();
}

void vtx_add(float* out, const float* const a,
    const float* const b, const size_t floats)
{
    AE_STUB();
}

void vtx_mul_ex( float* d, const float * const a, const float * const b,
    const size_t d_floats, const size_t d_offset, const size_t d_stride,
    const size_t a_floats, const size_t a_offset, const size_t a_stride,
    const size_t b_floats, const size_t b_offset, const size_t b_stride,
    const size_t length)
{
    AE_STUB();
}

void vtx_mul(float* out, const float* const a,
    const float* const b, const size_t floats)
{
    AE_STUB();
}

void vtx_madd_ex(
    float* d, const float* const a, const float* const b, const float s,
    const size_t d_floats, const size_t d_offset, const size_t d_stride,
    const size_t a_floats, const size_t a_offset, const size_t a_stride,
    const size_t b_floats, const size_t b_offset, const size_t b_stride,
    const size_t length)
{
    AE_STUB();
}

void vtx_madd(float* out, const float* const a, const float* const b,
                                const float s, const size_t floats)
{
    AE_STUB();
}

void vtx_lerp_ex(
    float* d, const float* const a, const float* const b, const float t,
    const size_t d_floats, const size_t d_offset, const size_t d_stride,
    const size_t a_floats, const size_t a_offset, const size_t a_stride,
    const size_t b_floats, const size_t b_offset, const size_t b_stride,
    const size_t length)
{
    AE_STUB();
}

void vtx_lerp(float* out, const float* const a, const float* const b,
                                const float t, const size_t floats)
{
    AE_STUB();
}

void vtx_mat4x4row_mul_point3_ex(float* vertices, const float mat[16],
        const size_t floats, const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_mat4x4row_mul_point3(float* v, const float m[16], const size_t n)
{
    AE_STUB();
}

void vtx_mat4x4row_mul_vector3_ex( float * vertices, const float mat[16],
                                const int normalize, const size_t floats,
                                const size_t offset, const size_t stride)
{
    AE_STUB();
}

void vtx_mat4x4row_mul_vector3( float* vertices, const float mat[16],
                            const int normalize, const size_t floats)
{
    AE_STUB();
}

void vtx_weld_func(ae_array_t* vertices, ae_vertex_format_t vtx_format, // shrinks
    ae_array_t* indices, ae_index_type_t idx_type, vtx_weld_func_t fn, void* data)
{
    AE_STUB();
}

size_t vtx_weld_func_ex(float* vertices, ae_vertex_format_t vertex_format,
            void* indices, ae_index_type_t index_type, size_t index_count,
            vtx_weld_func_t func, void* data) // returns new vertex count
{
    AE_STUB(); return 0;
}

void vtx_weld(ae_array_t* vertices, ae_vertex_format_t vtx_format,
    ae_array_t* indices, ae_index_type_t idx_type, float epsilon)
{
    AE_STUB();
}

size_t vtx_weld_ex(float* vertices, ae_vertex_format_t vtx_format, void* indices,
                    ae_index_type_t idx_type, size_t index_count, float epsilon)
{
    AE_STUB(); return 0;
}

void vtx_weld_dist2(ae_array_t* vertices, ae_vertex_format_t vtx_format,
        ae_array_t* indices, ae_index_type_t idx_type, float threshold2)
{
    AE_STUB();
}

size_t vtx_weld_dist2_ex(float * vertices, ae_vertex_format_t vertex_format,
    void* idx, ae_index_type_t idx_type, size_t idx_count, float threshold2)
{
    AE_STUB(); return 0;
}

void vtx_unweld(ae_array_t* vertices, ae_vertex_format_t format,
                ae_array_t* indices, ae_index_type_t index_type)
{
    AE_STUB();
}

void vtx_unweld_ex(float* packed, float* unpacked, ae_vertex_format_t vtx_format,
                    void* indices, ae_index_type_t index_type, size_t idx_count)
{
    AE_STUB();
}

void ae_vertex_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_vertex_quit(void)
{
    // AE_STUB();
}
