/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

size_t idx_value_get(void* indices, const ae_index_type_t type,
                        const size_t count, const size_t which)
{
    AE_STUB(); return 0;
}

void idx_value_set(void* indices, const ae_index_type_t type, const size_t num,
                                        const size_t which, const size_t value)
{
    AE_STUB();
}

void
idx_value_append(ae_array_t* inds, const ae_index_type_t type, const size_t value)
{
    AE_STUB();
}

size_t idx_value_max(void* indices, const ae_index_type_t type, const size_t n)
{
    AE_STUB(); return 0;
}

void* idx_temp_identity_get(const ae_index_type_t type, const size_t count)
{
    AE_STUB(); return NULL;
}

void idx_temp_identity_free(void* p, const ae_index_type_t type, const size_t n)
{
    AE_STUB();
}

void idx_identity(void* indices, const ae_index_type_t type, const size_t count)
{
    AE_STUB();
}

int idx_is_identity(void* idx_p, const ae_index_type_t type, const size_t count)
{
    AE_STUB(); return 0;
}

void idx_add_offset(void* indices, const ae_index_type_t type,
                    const size_t count, const size_t offset_v)
{
    AE_STUB();
}

void idx_sub_offset(void* indices, const ae_index_type_t type,
                    const size_t count, const size_t offset_v)
{
    AE_STUB();
}

int idx_fits_type(const void * const idx, const size_t count,
        const ae_index_type_t src, const ae_index_type_t dst)
{
    AE_STUB(); return 0;
}

ae_index_type_t idx_smallest_fitting_type(const void* const idx,
                const ae_index_type_t type, const size_t count)
{
    AE_STUB(); return AE_INDEX_TYPE_U32;
}

void* idx_temp_type_converted_get(void * idx, const size_t count,
        const ae_index_type_t src_t, const ae_index_type_t dst_t)
{
    AE_STUB(); return NULL;
}

void idx_temp_type_converted_free(void * dst, // stack free temp
                const size_t count, const ae_index_type_t dst_t)
{
    AE_STUB();
}

void idx_type_convert(ae_array_t* indices, // expand or contract in-place
                    const ae_index_type_t src, const ae_index_type_t dst)
{
    AE_STUB();
}

void idx_type_convert_ex(void* indices, // doesn't reallocate index array
    const ae_index_type_t src, const ae_index_type_t dst, const size_t n)
{
    AE_STUB();
}

void idx_type_converted(ae_array_t* src, const ae_index_type_t src_t,
                        ae_array_t* dst, const ae_index_type_t dst_t)
{
    AE_STUB();
}

void idx_type_converted_ex( void* src, const ae_index_type_t src_t,
        void* dst, const ae_index_type_t dst_t, const size_t count)
{
    AE_STUB();
}

void idx_vertex_cache_optimize_ex(void * indices, const ae_index_type_t type,
                        const size_t index_count, const size_t vertex_count)
{
    AE_STUB();
}

void idx_vertex_cache_optimize(void* idx_buffer, const ae_index_type_t type,
                                                        const size_t count)
{
    AE_STUB();
}

double
idx_vertex_cache_miss_ratio(const void* const idx, const ae_index_type_t type,
                                const size_t count, int cache_size, int FIFO)
{
    AE_STUB(); return 0.0;
}

void ae_index_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_index_quit(void)
{
    // AE_STUB();
}
