/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_INDEX_H__
#define __AE_INDEX_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_index_init(int argc, char** argv);
void ae_index_quit(void); // private init

/*
================================================================================
 * ~~ [ index type enum ] ~~ *
--------------------------------------------------------------------------------
NOTE: these don't match GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, or GL_UNSIGNED_INT
--------------------------------------------------------------------------------
*/

#define AE_INDEX_TYPE_N                 \
    N(AE_INDEX_TYPE_U8 , sizeof(u8 )),  \
    N(AE_INDEX_TYPE_U16, sizeof(u16)),  \
    N(AE_INDEX_TYPE_U32, sizeof(u32)),  \
    N(AE_INDEX_TYPE_COUNT, 0)

typedef enum ae_index_type_t
{
    #define N(x, b) x
    AE_INDEX_TYPE_N
    #undef N
} \
    ae_index_type_t;

static const char* ae_index_type_name[] =
{
    #define N(x, b) #x
    AE_INDEX_TYPE_N
    #undef N
};

static size_t ae_index_type_bytes[] =
{
    #define N(x, b) b
    AE_INDEX_TYPE_N
    #undef N
};

static c_inline ae_index_type_t ae_index_bytes_to_type(const size_t bytes)
{
    switch (bytes) // convert sizeof(type) to type enum
    {
        case 1: return AE_INDEX_TYPE_U8 ;
        case 2: return AE_INDEX_TYPE_U16;
        case 4: return AE_INDEX_TYPE_U32;

        default: assert(0); return AE_INDEX_TYPE_COUNT;
    }
}

/*
================================================================================
 * ~~ [ generic indexing ] ~~ *
--------------------------------------------------------------------------------
*/

/* returns indices[which]
 */
AE_DECL size_t AE_CALL idx_value_get(void * indices, const ae_index_type_t type,
                                        const size_t count, const size_t which);

/* indices[which] = value
 */
AE_DECL void AE_CALL
idx_value_set(void* indices, const ae_index_type_t type, const size_t count,
                                    const size_t which, const size_t value);

/* indices.append(value)
 */
AE_DECL void AE_CALL
idx_value_append(ae_array_t*, const ae_index_type_t type, const size_t value);

/* compute vertex count - 1 of an indexed mesh by finding the largest value.
 */
AE_DECL size_t AE_CALL
idx_value_max(void* buffer, const ae_index_type_t type, const size_t count);

/*
================================================================================
 * ~~ [ identity indices ] ~~ *
--------------------------------------------------------------------------------
*/

// This provides an easy fallback for mesh algorithms that want index buffers,
// but the user has passed a "raw" vertex buffer that hasn't been welded yet.

AE_DECL void* AE_CALL
idx_temp_identity_get(const ae_index_type_t index_type, const size_t i_count);

AE_DECL void AE_CALL
idx_temp_identity_free(void* ptr, const ae_index_type_t type, const size_t n);

/* ===== [ generation ] ===================================================== */

AE_DECL void AE_CALL // fill index buffer with indices ordered [ 0,1,2,... ]
idx_identity(void * indices, const ae_index_type_t type, const size_t count);

AE_DECL int AE_CALL // check if the index buffer is an identity index buffer
idx_is_identity(void* idx_p, const ae_index_type_t type, const size_t count);

/*
================================================================================
 * ~~ [ scalar offsets ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL idx_add_offset(void* indices, const ae_index_type_t type,
                                    const size_t count , const size_t offset);

AE_DECL void AE_CALL idx_sub_offset(void* indices, const ae_index_type_t type,
                                    const size_t count , const size_t offset);

/*
================================================================================
 * ~~ [ type conversion ] ~~ *
--------------------------------------------------------------------------------
*/

/* checks if all index values in the buffer are small enough to fit a given type.
 * idx_smallest_fitting_type is basically the automated version of this function.
 * NOTE: this can be done much more cheaply by simply checking the vertex count.
 */
AE_DECL int AE_CALL idx_fits_type(const void* const idx_p, const size_t count,
                    const ae_index_type_t src_t, const ae_index_type_t dst_t);

AE_DECL ae_index_type_t AE_CALL idx_smallest_fitting_type(const void* const idx,
                                const ae_index_type_t type, const size_t count);

AE_DECL void* AE_CALL idx_temp_type_converted_get(void * idx, const size_t count,
                        const ae_index_type_t src_t, const ae_index_type_t dst_t);

AE_DECL void AE_CALL idx_temp_type_converted_free(void* dst, // free temp
                        const size_t count, const ae_index_type_t dst_t);

/* this grows or shrinks an index buffer by type. index buffer values must fit.
 */
AE_DECL void AE_CALL idx_type_convert(ae_array_t* indices,
    const ae_index_type_t src, const ae_index_type_t dst);

AE_DECL void AE_CALL idx_type_convert_ex(void* indices, // presized idx array
    const ae_index_type_t src, const ae_index_type_t dst, const size_t count);

AE_DECL void AE_CALL // this grows or shrinks an index buffer into another array.
idx_type_converted( ae_array_t* src, const ae_index_type_t src_t,
                    ae_array_t* dst, const ae_index_type_t dst_t);

AE_DECL void AE_CALL idx_type_converted_ex(void* src, const ae_index_type_t src_t,
                    void* dst, const ae_index_type_t dst_t, const size_t count);

/*
================================================================================
 * ~~ [ vertex cache optimization ] ~~ *
--------------------------------------------------------------------------------
*/

/* re-order indices for more efficient use of graphics hardware vertex caches.
 * this works for indexed triangle meshes only (no quads or triangle strips).
 */
AE_DECL void AE_CALL idx_vertex_cache_optimize_ex(void * indices,
                                        const ae_index_type_t type,
                                        const size_t index_count,
                                        const size_t vertex_count);

AE_DECL void AE_CALL
idx_vertex_cache_optimize(void* indices, const ae_index_type_t type,
                                                const size_t count);

/* measure cache efficiency (and compressibility, kinda) of this index buffer.
 * recommended vertex cache size is 16 entries, with FIFO queue mode enabled.
 */
AE_DECL double AE_CALL
idx_vertex_cache_miss_ratio(const void* const idx, const ae_index_type_t type,
                                const size_t count, int cache_size, int FIFO);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_INDEX_H__ */
