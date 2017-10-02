/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_VERTEX_H__
#define __AE_VERTEX_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_vertex_init(int argc, char** argv);
void ae_vertex_quit(void); // private init

/*
================================================================================
 * ~~ [ vertex format descriptors ] ~~ *
--------------------------------------------------------------------------------
NOTE: this doesn't support non-floating-point elements (like 24 or 32 bit color)
TODO: add an extra field for number of elem channels (AE_T2F_N3F_V3F has 3 etc.)
--------------------------------------------------------------------------------

    Extra data associated with vertex formats (in order):

    - total size of the vertex  (in floats)
    - texture coordinate offset (first T index in vertex)
    - texture coordinate size   (T floats)
    - color offset              (first C index in vertex)
    - color size                (C floats)
    - normal offset             (first N index in vertex)
    - normal size               (N floats)
    - position offset           (first V index in vertex)
    - position size             (V floats)

--------------------------------------------------------------------------------
*/

#define AE_VERTEX_FORMAT_N \
    \
    N(AE_T2F, 2, 0, 2, 0, 0, 0, 0, 0, 0), \
    N(AE_T3F, 3, 0, 3, 0, 0, 0, 0, 0, 0), \
    N(AE_T4F, 4, 0, 4, 0, 0, 0, 0, 0, 0), \
    N(AE_C3F, 3, 0, 0, 0, 3, 0, 0, 0, 0), \
    N(AE_C4F, 4, 0, 0, 0, 4, 0, 0, 0, 0), \
    N(AE_N3F, 3, 0, 0, 0, 0, 0, 3, 0, 0), \
    N(AE_V2F, 2, 0, 0, 0, 0, 0, 0, 0, 2), \
    N(AE_V3F, 3, 0, 0, 0, 0, 0, 0, 0, 3), \
    N(AE_V4F, 4, 0, 0, 0, 0, 0, 0, 0, 4), \
    \
    N(AE_T2F_V3F, 5, 0, 2, 0, 0, 0, 0, 3, 3), \
    N(AE_C3F_V3F, 6, 0, 0, 0, 3, 0, 0, 3, 3), \
    N(AE_N3F_V3F, 6, 0, 0, 0, 0, 0, 3, 3, 3), \
    N(AE_T4F_V4F, 8, 0, 4, 0, 0, 0, 0, 4, 4), \
    \
    N(AE_T2F_C3F_V3F,  8, 0, 2, 2, 3, 0, 0, 5, 3), \
    N(AE_T2F_N3F_V3F,  8, 0, 2, 0, 0, 2, 3, 5, 3), \
    N(AE_C4F_N3F_V3F, 10, 0, 0, 0, 4, 4, 3, 7, 3), \
    \
    N(AE_T2F_C4F_N3F_V3F, 12, 0, 2, 2, 4, 6, 3,  9, 3), \
    N(AE_T4F_C4F_N3F_V3F, 15, 0, 4, 4, 4, 8, 3, 11, 4), \
    \
    N(AE_VERTEX_FORMAT_COUNT, 0, 0, 0, 0, 0, 0, 0, 0, 0)

typedef enum ae_vertex_format_t
{
    #define N(name, size, Toff, Tlen, Coff, Clen, Noff, Nlen, Voff, Vlen) name
    AE_VERTEX_FORMAT_N
    #undef N
} \
    ae_vertex_format_t;

static const char* ae_vertex_format_name[] =
{
    #define N(name, size, Toff, Tlen, Coff, Clen, Noff, Nlen, Voff, Vlen) #name
    AE_VERTEX_FORMAT_N
    #undef N
};

static const size_t ae_vertex_format_size[] =
{
    #define N(name, size, Toff, Tlen, Coff, Clen, Noff, Nlen, Voff, Vlen) size
    AE_VERTEX_FORMAT_N
    #undef N
};

static const size_t ae_vertex_format_texcoord_offset[] =
{
    #define N(name, size, Toff, Tlen, Coff, Clen, Noff, Nlen, Voff, Vlen) Toff
    AE_VERTEX_FORMAT_N
    #undef N
};

static const size_t ae_vertex_format_texcoord_size[] =
{
    #define N(name, size, Toff, Tlen, Coff, Clen, Noff, Nlen, Voff, Vlen) Tlen
    AE_VERTEX_FORMAT_N
    #undef N
};

static const size_t ae_vertex_format_color_offset[] =
{
    #define N(name, size, Toff, Tlen, Coff, Clen, Noff, Nlen, Voff, Vlen) Coff
    AE_VERTEX_FORMAT_N
    #undef N
};

static const size_t ae_vertex_format_color_size[] =
{
    #define N(name, size, Toff, Tlen, Coff, Clen, Noff, Nlen, Voff, Vlen) Clen
    AE_VERTEX_FORMAT_N
    #undef N
};

static const size_t ae_vertex_format_normal_offset[] =
{
    #define N(name, size, Toff, Tlen, Coff, Clen, Noff, Nlen, Voff, Vlen) Noff
    AE_VERTEX_FORMAT_N
    #undef N
};

static const size_t ae_vertex_format_normal_size[] =
{
    #define N(name, size, Toff, Tlen, Coff, Clen, Noff, Nlen, Voff, Vlen) Nlen
    AE_VERTEX_FORMAT_N
    #undef N
};

static const size_t ae_vertex_format_position_offset[] =
{
    #define N(name, size, Toff, Tlen, Coff, Clen, Noff, Nlen, Voff, Vlen) Voff
    AE_VERTEX_FORMAT_N
    #undef N
};

static const size_t ae_vertex_format_position_size[] =
{
    #define N(name, size, Toff, Tlen, Coff, Clen, Noff, Nlen, Voff, Vlen) Vlen
    AE_VERTEX_FORMAT_N
    #undef N
};

static c_inline int ae_vertex_format_has_texcoord(ae_vertex_format_t format)
{
    return ae_vertex_format_texcoord_size[format] != 0;
}

static c_inline int ae_vertex_format_has_color(ae_vertex_format_t format)
{
    return ae_vertex_format_color_size[format] != 0;
}

static c_inline int ae_vertex_format_has_normal(ae_vertex_format_t format)
{
    return ae_vertex_format_normal_size[format] != 0;
}

static c_inline int ae_vertex_format_has_position(ae_vertex_format_t format)
{
    return ae_vertex_format_position_size[format] != 0;
}

AE_DECL size_t AE_CALL // vertex descriptor info from a string like "v3f" or "N"
ae_vertex_format_element_offset(const ae_vertex_format_t fmt, const char* elem);

AE_DECL size_t AE_CALL
ae_vertex_format_element_size(const ae_vertex_format_t fmt, const char* elem);

static c_inline int
ae_vertex_format_has_element(const ae_vertex_format_t format, const char* elem)
{
    return ae_vertex_format_element_size(format, elem) != 0;
}

/* find the interleaved vertex format for a given set of element sizes.
 * this is kind of the inverse of everything above (info -> format).
 * ae_vertex_format_from_sizes(4, 4, 3, 3) == AE_T4F_C4F_N3F_V3F, etc.
 */
AE_DECL ae_vertex_format_t AE_CALL
ae_vertex_format_from_sizes(const size_t tex_coord, const size_t color,
                            const size_t normal, const size_t position);

/*
================================================================================
 * ~~ [ bounding volume construction ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL // get the axis-aligned bounding box of a vertex buffer
vtx_aabbox_ex(float * minv, float* maxv, const float * const vertices,
                            const size_t floats, const size_t offset,
                            const size_t stride, const size_t length);

/* ===== [ strided ] ======================================================== */

AE_DECL void AE_CALL
vtx_aabbox_flt_ex(float * minv, float * maxv, const float * const vertices,
            const size_t floats, const size_t offset, const size_t stride);

AE_DECL void AE_CALL
vtx_aabbox2_ex( float minv[2], float maxv[2], const float * const vertices,
            const size_t floats, const size_t offset, const size_t stride);

AE_DECL void AE_CALL
vtx_aabbox3_ex( float minv[3], float maxv[3], const float * const vertices,
            const size_t floats, const size_t offset, const size_t stride);

AE_DECL void AE_CALL
vtx_aabbox4_ex( float minv[4], float maxv[4], const float * const vertices,
            const size_t floats, const size_t offset, const size_t stride);

/* ===== [ packed ] ========================================================= */

AE_DECL void AE_CALL vtx_aabbox_flt( float* minv, float* maxv,
                const float* const vert, const size_t floats);

AE_DECL void AE_CALL vtx_aabbox2(float minv[2], float maxv[2],
            const float* const vertices, const size_t floats);

AE_DECL void AE_CALL vtx_aabbox3(float minv[3], float maxv[3],
            const float* const vertices, const size_t floats);

AE_DECL void AE_CALL vtx_aabbox4(float minv[4], float maxv[4],
            const float* const vertices, const size_t floats);

/*
================================================================================
 * ~~ [ vector copy ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL // copy an N-wide value into an array with stride
vtx_vec_copy_ex(float * vertices, const float* const val,
                const size_t floats, const size_t offset,
                const size_t stride, const size_t length);

/* ===== [ strided ] ======================================================== */

AE_DECL void AE_CALL
vtx_flt_copy_ex(float* vert, const float fvalue, const size_t floats ,
                            const size_t offset, const size_t stride);

AE_DECL void AE_CALL
vtx_vec2copy_ex(float* vert, const float val[2], const size_t floats ,
                            const size_t offset, const size_t stride);

AE_DECL void AE_CALL
vtx_vec3copy_ex(float* vert, const float val[3], const size_t floats ,
                            const size_t offset, const size_t stride);

AE_DECL void AE_CALL
vtx_vec4copy_ex(float* vert, const float val[4], const size_t floats ,
                            const size_t offset, const size_t stride);

/* ===== [ packed ] ========================================================= */

AE_DECL void AE_CALL vtx_flt_copy(float* vertices,
        const float fvalue, const size_t floats );

AE_DECL void AE_CALL vtx_vec2copy(float* vertices,
        const float val[2], const size_t floats );

AE_DECL void AE_CALL vtx_vec3copy(float* vertices,
        const float val[3], const size_t floats );

AE_DECL void AE_CALL vtx_vec4copy(float* vertices,
        const float val[4], const size_t floats );

/*
================================================================================
 * ~~ [ vector epsilon compare ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL int AE_CALL vtx_vec_cmp_e_ex( // compare channel values against constant
    const float* const vertices, const float* const value, const float epsilon,
    const size_t floats, const size_t offset,
    const size_t stride, const size_t length);

/* ===== [ strided ] ======================================================== */

AE_DECL int AE_CALL vtx_flt_cmp_e_ex(const float* const verts, const float fl_value,
    const float tol, const size_t floats, const size_t offset, const size_t stride);

AE_DECL int AE_CALL vtx_vec2cmp_e_ex(const float* const verts, const float value[2],
    const float tol, const size_t floats, const size_t offset, const size_t stride);

AE_DECL int AE_CALL vtx_vec3cmp_e_ex(const float* const verts, const float value[3],
    const float tol, const size_t floats, const size_t offset, const size_t stride);

AE_DECL int AE_CALL vtx_vec4cmp_e_ex(const float* const verts, const float value[4],
    const float tol, const size_t floats, const size_t offset, const size_t stride);

/* ===== [ packed ] ========================================================= */

AE_DECL int AE_CALL vtx_flt_cmp_e( const float * const vertices,
    const float fval, const float epsilon, const size_t floats);

AE_DECL int AE_CALL vtx_vec2cmp_e( const float * const vertices,
    const float v[2], const float epsilon, const size_t floats);

AE_DECL int AE_CALL vtx_vec3cmp_e( const float * const vertices,
    const float v[3], const float epsilon, const size_t floats);

AE_DECL int AE_CALL vtx_vec4cmp_e( const float * const vertices,
    const float v[4], const float epsilon, const size_t floats);

/*
================================================================================
 * ~~ [ vector add ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL
vtx_vec_add_ex( float * vertices, const float* const val, const size_t floats ,
                const size_t offset, const size_t stride, const size_t length);

/* ===== [ strided ] ======================================================== */

AE_DECL void AE_CALL vtx_flt_add_ex(float* vertices, const float fvalue,
        const size_t floats, const size_t offset, const size_t stride);

AE_DECL void AE_CALL vtx_vec2add_ex(float* vertices, const float val[2],
        const size_t floats, const size_t offset, const size_t stride);

AE_DECL void AE_CALL vtx_vec3add_ex(float* vertices, const float val[3],
        const size_t floats, const size_t offset, const size_t stride);

AE_DECL void AE_CALL vtx_vec4add_ex(float* vertices, const float val[4],
        const size_t floats, const size_t offset, const size_t stride);

/* ===== [ packed ] ========================================================= */

AE_DECL void AE_CALL vtx_flt_add(float* vertices,
        const float fvalue, const size_t floats);

AE_DECL void AE_CALL vtx_vec2add(float* vertices,
        const float val[2], const size_t floats);

AE_DECL void AE_CALL vtx_vec3add(float* vertices,
        const float val[3], const size_t floats);

AE_DECL void AE_CALL vtx_vec4add(float* vertices,
        const float val[4], const size_t floats);

/*
================================================================================
 * ~~ [ vector mul ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL
vtx_vec_mul_ex( float * vertices, const float* const val, const size_t floats ,
                const size_t offset, const size_t stride, const size_t length);

/* ===== [ strided ] ======================================================== */

AE_DECL void AE_CALL vtx_flt_mul_ex(float* vertices, const float fvalue,
        const size_t floats, const size_t offset, const size_t stride);

AE_DECL void AE_CALL vtx_vec2mul_ex(float* vertices, const float val[2],
        const size_t floats, const size_t offset, const size_t stride);

AE_DECL void AE_CALL vtx_vec3mul_ex(float* vertices, const float val[3],
        const size_t floats, const size_t offset, const size_t stride);

AE_DECL void AE_CALL vtx_vec4mul_ex(float* vertices, const float val[4],
        const size_t floats, const size_t offset, const size_t stride);

/* ===== [ packed ] ========================================================= */

AE_DECL void AE_CALL vtx_flt_mul(float* vertices,
        const float fvalue, const size_t floats);

AE_DECL void AE_CALL vtx_vec2mul(float* vertices,
        const float val[2], const size_t floats);

AE_DECL void AE_CALL vtx_vec3mul(float* vertices,
        const float val[3], const size_t floats);

AE_DECL void AE_CALL vtx_vec4mul(float* vertices,
        const float val[4], const size_t floats);

/*
================================================================================
 * ~~ [ vector lerp ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL // lerp towards a point (1.0 - t to lerp away from point)
vtx_vec_lerp_ex(float* verts, const float* const val, const float t,
                const size_t floats, const size_t offset,
                const size_t stride, const size_t length);

/* ===== [ strided ] ======================================================== */

AE_DECL void AE_CALL vtx_flt_lerp_ex(float* verts, const float fval, const float t,
                    const size_t floats, const size_t offset, const size_t stride);

AE_DECL void AE_CALL vtx_vec2lerp_ex(float* verts, const float v[2], const float t,
                    const size_t floats, const size_t offset, const size_t stride);

AE_DECL void AE_CALL vtx_vec3lerp_ex(float* verts, const float v[3], const float t,
                    const size_t floats, const size_t offset, const size_t stride);

AE_DECL void AE_CALL vtx_vec4lerp_ex(float* verts, const float v[4], const float t,
                    const size_t floats, const size_t offset, const size_t stride);

/* ===== [ packed ] ========================================================= */

AE_DECL void AE_CALL
vtx_flt_lerp(float* verts, const float fval, const float t, const size_t floats);

AE_DECL void AE_CALL
vtx_vec2lerp(float* verts, const float v[2], const float t, const size_t floats);

AE_DECL void AE_CALL
vtx_vec3lerp(float* verts, const float v[3], const float t, const size_t floats);

AE_DECL void AE_CALL
vtx_vec4lerp(float* verts, const float v[4], const float t, const size_t floats);

/*
================================================================================
 * ~~ [ array copy ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL
vtx_copy_ex(float* dst, const float * const src, // strided vertex element copy
            const size_t d_floats, const size_t d_offset, const size_t d_stride,
            const size_t s_floats, const size_t s_offset, const size_t s_stride,
            const size_t length);

AE_DECL void AE_CALL
vtx_copy(float* dst, const float* const src, const size_t floats);

/*
================================================================================
 * ~~ [ array epsilon compare ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL int AE_CALL // fuzzy element comparison (ev is tolerance epsilon)
vtx_cmp_e_ex(const float* const a, const float* const b, const float ev,
    const size_t a_floats, const size_t a_offset, const size_t a_stride,
    const size_t b_floats, const size_t b_offset, const size_t b_stride,
    const size_t length);

AE_DECL int AE_CALL vtx_cmp_e(const float * const a, const float* const b,
                                const float epsilon, const size_t floats);

/*
================================================================================
 * ~~ [ array add ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL
vtx_add_ex( float* d, const float* const a, const float* const b, // d = a + b
            const size_t d_floats, const size_t d_offset, const size_t d_stride,
            const size_t a_floats, const size_t a_offset, const size_t a_stride,
            const size_t b_floats, const size_t b_offset, const size_t b_stride,
            const size_t length);

AE_DECL void AE_CALL vtx_add(float * out, const float* const a,
                    const float* const b, const size_t floats);

/*
================================================================================
 * ~~ [ array mul ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL
vtx_mul_ex( float* d, const float* const a, const float* const b, // d = a * b
            const size_t d_floats, const size_t d_offset, const size_t d_stride,
            const size_t a_floats, const size_t a_offset, const size_t a_stride,
            const size_t b_floats, const size_t b_offset, const size_t b_stride,
            const size_t length);

AE_DECL void AE_CALL vtx_mul(float * out, const float* const a,
                    const float* const b, const size_t floats);

/*
================================================================================
 * ~~ [ array mul-add ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL
vtx_madd_ex(float* d, const float* const a, const float* const b, const float s,
            const size_t d_floats, const size_t d_offset, const size_t d_stride,
            const size_t a_floats, const size_t a_offset, const size_t a_stride,
            const size_t b_floats, const size_t b_offset, const size_t b_stride,
            const size_t length);

AE_DECL void AE_CALL
vtx_madd(float* out, const float* const a, const float* const b,
                            const float s, const size_t floats);

/*
================================================================================
 * ~~ [ array lerp ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL
vtx_lerp_ex(float* d, const float* const a, const float* const b, const float t,
            const size_t d_floats, const size_t d_offset, const size_t d_stride,
            const size_t a_floats, const size_t a_offset, const size_t a_stride,
            const size_t b_floats, const size_t b_offset, const size_t b_stride,
            const size_t length);

AE_DECL void AE_CALL
vtx_lerp(float* out, const float* const a, const float* const b,
                            const float t, const size_t floats);

/*
================================================================================
 * ~~ [ mat4x4 mul ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL // transform a point by a scale / rotation / translation matrix
vtx_mat4x4row_mul_point3_ex(float * vertices, const float mat[16],
    const size_t floats, const size_t offset, const size_t stride);

AE_DECL void AE_CALL
vtx_mat4x4row_mul_point3(float* vertices, const float mat[16], const size_t floats);

AE_DECL void AE_CALL
vtx_mat4x4row_mul_vector3_ex(float* vertices, const float mat[16], const int normalize,
                        const size_t floats, const size_t offset, const size_t stride);

AE_DECL void AE_CALL
vtx_mat4x4row_mul_vector3( float * vertices, const float mat[16],
                        const int normalize, const size_t floats);

/*
================================================================================
 * ~~ [ vertex welding ] ~~ *
--------------------------------------------------------------------------------
*/

// compare two vertices, passing all arguments through data ptr
typedef int (*vtx_weld_func_t)(float* a, float* b, void* data);

/* ===== [ abstract comparison ] ============================================ */

AE_DECL void AE_CALL // make vertices unique (remove adjacent) and create indices.
vtx_weld_func(ae_array_t* verts, ae_vertex_format_t v_format, ae_array_t* idx,
                    ae_index_type_t index_type, vtx_weld_func_t func, void* data);

AE_DECL size_t AE_CALL // works with sized buffers. returns the new vertex count.
vtx_weld_func_ex(float* vertices, ae_vertex_format_t vertex_format, void* indices,
    ae_index_type_t idx_type, size_t idx_count, vtx_weld_func_t func, void* data);

/* ===== [ epsilon comparison ] ============================================= */

AE_DECL void AE_CALL vtx_weld(ae_array_t* vertices, ae_vertex_format_t vtx_format,
                    ae_array_t* indices, ae_index_type_t idx_type, float epsilon);

AE_DECL size_t AE_CALL vtx_weld_ex(float* vertices, ae_vertex_format_t vtx_format,
    void* indices, ae_index_type_t index_type, size_t index_count, float epsilon);

/* ===== [ distance comparison ] ============================================ */

AE_DECL void AE_CALL // compare by distance between vertices, ignoring other data
vtx_weld_dist2(ae_array_t* vertices, ae_vertex_format_t vertex_format,
    ae_array_t* indices, ae_index_type_t idx_type, float threshold2);

AE_DECL size_t AE_CALL // squared distance
vtx_weld_dist2_ex(float* vertices, ae_vertex_format_t vertex_format, void* idx,
                ae_index_type_t idx_type, size_t idx_count, float threshold2);

/*
================================================================================
 * ~~ [ vertex unwelding ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL vtx_unweld(ae_array_t* vtx, ae_vertex_format_t vfmt,
                                ae_array_t* idx, ae_index_type_t i_type);

AE_DECL void AE_CALL // remap indexed vertices back to "raw" (non-indexed)
vtx_unweld_ex(float* packed, float* unpacked, ae_vertex_format_t vformat,
            void* indices, ae_index_type_t index_type, size_t idx_count);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_VERTEX_H__ */
