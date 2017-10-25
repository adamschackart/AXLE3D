/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void ae_index_init(int argc, char** argv) {}
void ae_index_quit(void) {}

/*
================================================================================
 * ~~ [ generic indexing ] ~~ *
--------------------------------------------------------------------------------
*/

size_t idx_value_get(void* indices, const ae_index_type_t type,
                        const size_t count, const size_t which)
{
    // NULL simulates an identity index buffer for non-indexed mesh algorithms
    ae_if (indices != NULL)
    {
        ae_assert (which < count, "idx %u >= num %u", (u32)which, (u32)count);

        ae_switch (type, ae_index_type, AE_INDEX_TYPE, suffix)
        {
            case AE_INDEX_TYPE_U8:
                return (size_t)((u8 *)indices)[which];

            case AE_INDEX_TYPE_U16:
                return (size_t)((u16*)indices)[which];

            case AE_INDEX_TYPE_U32:
                return (size_t)((u32*)indices)[which];

            default:
                ae_assert(0, "%u", (u32) type); break;
        }

        return 0;
    }
    else
    {
        return which;
    }
}

void
idx_value_set(void* buffer, const ae_index_type_t type, const size_t count,
                                    const size_t which, const size_t value)
{
    ae_if (buffer != NULL)
    {
        ae_assert (which < count, "idx %u >= num %u", (u32)which, (u32)count);

        if (type != AE_INDEX_TYPE_U32) // FIXME: always fails for u32 on 32-bit
        ae_assert (value < (size_t)ae_ipow(2, ae_index_type_bytes[type] << 3),

                                    "value %u is too large for index type %s",
                                        (u32)value, ae_index_type_name[type]);

        ae_switch (type, ae_index_type, AE_INDEX_TYPE, suffix)
        {
            case AE_INDEX_TYPE_U8:
                ((u8 *)buffer)[ which ] = (u8 )value; break;

            case AE_INDEX_TYPE_U16:
                ((u16*)buffer)[ which ] = (u16)value; break;

            case AE_INDEX_TYPE_U32:
                ((u32*)buffer)[ which ] = (u32)value; break;

            default:
                ae_assert(0, "%u", (u32)type); break;
        }
    }
}

void
idx_value_append(ae_array_t* inds, const ae_index_type_t type, const size_t value)
{
    if (type != AE_INDEX_TYPE_U32) // FIXME: always fails for u32 on 32-bit builds
    {
        ae_assert (value < (size_t)ae_ipow(2, ae_index_type_bytes[type] << 3),

                                    "value %u is too large for index type %s",
                                        (u32)value, ae_index_type_name[type]);
    }

    /* TODO: ae_array_append(inds, index_offset(&value, ae_index_type_bytes[type]),
     *                                                  ae_index_type_bytes[type])
     */
    ae_switch (type, ae_index_type, AE_INDEX_TYPE, suffix)
    {
        case AE_INDEX_TYPE_U8:
        {
            u8  v = (const u8 )value; ae_array_append(inds, &v, sizeof(u8 ));
        }
        break;

        case AE_INDEX_TYPE_U16:
        {
            u16 v = (const u16)value; ae_array_append(inds, &v, sizeof(u16));
        }
        break;

        case AE_INDEX_TYPE_U32:
        {
            u32 v = (const u32)value; ae_array_append(inds, &v, sizeof(u32));
        }
        break;

        default:
        {
            ae_assert(0, "%u", (u32)type);
        }
        break;
    }
}

size_t idx_value_max(void* indices, const ae_index_type_t type, const size_t n)
{
    // TODO move the index type dispatch outside of the loop
    AE_PROFILE_ENTER();

    size_t i = 0, j = 0;
    for (; i < n; i++)
    {
        const size_t v = idx_value_get(indices, type, n, i);
        ae_if (v > j) j = v;
    }

    AE_PROFILE_LEAVE(); return j;
}

/*
================================================================================
 * ~~ [ identity indices ] ~~ *
--------------------------------------------------------------------------------
*/

void* idx_temp_identity_get(const ae_index_type_t type, const size_t count)
{
    const size_t b = ae_index_type_bytes[type] * count;
    void* p = ae_stack_malloc(ae_global_stack(), b);

    // TODO: keep one resizable array allocated and expand it as needed. guard
    // array retrieval with a bool, and fall back to the stack if it's in use.
    // maybe we can do something fun like indexing into a bitmask by type size.
    idx_identity(p, type, count); return p;
}

void idx_temp_identity_free(void* p, const ae_index_type_t type, const size_t n)
{
    /* since this implementation is free to hand back constant memory (ie.
     * we might just want to return a const static 256 byte table for u8),
     * we effectively treat it as read-only. it's also generally expected
     * that temp indices won't be part of an object (subject to mutation).
     */
    ae_assert(idx_is_identity(p, type, n), "temp id %s indices written",
                                            ae_index_type_name[type] );

    ae_stack_free(ae_global_stack(), p, ae_index_type_bytes[type] * n);
}

/* ===== [ generation ] ===================================================== */

static void idx_identity_u8 (u8 * idx, const size_t n)
{
    AE_PROFILE_ENTER(); size_t i = 0;

    // ensure that index values don't wrap back around
    ae_assert(n <= AE_U8_MAX + 1, "%u", (unsigned)n);

    for (; i < n; i++) idx[i] = (u8)i;
    AE_PROFILE_LEAVE();
}

static void idx_identity_u16(u16* idx, const size_t n)
{
    AE_PROFILE_ENTER(); size_t i = 0;

    // ensure that index values don't wrap back around
    ae_assert(n <= AE_U16_MAX + 1, "%u", (unsigned)n);

    for (; i < n; i++) idx[i] = (u16)i;
    AE_PROFILE_LEAVE();
}

static void idx_identity_u32(u32* idx, const size_t n)
{
    AE_PROFILE_ENTER(); size_t i = 0;

    // technically off by one, but avoids overflowing
    ae_assert(n <= AE_U32_MAX, "%u", (unsigned int)n);

    for (; i < n; i++) idx[i] = (u32)i;
    AE_PROFILE_LEAVE();
}

void idx_identity(void* indices, const ae_index_type_t type, const size_t count)
{
    AE_PROFILE_ENTER();

    ae_switch (type, ae_index_type, AE_INDEX_TYPE, suffix)
    {
        case AE_INDEX_TYPE_U8:
        {
            idx_identity_u8 ((u8 *)indices, count);
        }
        break;

        case AE_INDEX_TYPE_U16:
        {
            idx_identity_u16((u16*)indices, count);
        }
        break;

        case AE_INDEX_TYPE_U32:
        {
            idx_identity_u32((u32*)indices, count);
        }
        break;

        default: ae_assert(0, "%u", (u32)type); break;
    }

    AE_PROFILE_LEAVE();
}

static int idx_is_identity_u8 (u8 * idx, const size_t n)
{
    AE_PROFILE_ENTER();
    size_t i = 0;
    int isidentity = 1;

    // ensure that index values don't wrap back around
    ae_assert(n <= AE_U8_MAX + 1, "%u", (unsigned)n);

    for (; i < n; i++)
    {
        if (idx[i] != (u8) i) { isidentity = 0; break; }
    }

    AE_PROFILE_LEAVE();
    return(isidentity);
}

static int idx_is_identity_u16(u16* idx, const size_t n)
{
    AE_PROFILE_ENTER();
    size_t i = 0;
    int isidentity = 1;

    // ensure that index values don't wrap back around
    ae_assert(n <= AE_U16_MAX + 1, "%u", (unsigned)n);

    for (; i < n; i++)
    {
        if (idx[i] != (u16)i) { isidentity = 0; break; }
    }

    AE_PROFILE_LEAVE();
    return(isidentity);
}

static int idx_is_identity_u32(u32* idx, const size_t n)
{
    AE_PROFILE_ENTER();
    size_t i = 0;
    int isidentity = 1;

    // technically off by one, but avoids overflowing
    ae_assert(n <= AE_U32_MAX, "%u", (unsigned int)n);

    for (; i < n; i++)
    {
        if (idx[i] != (u32)i) { isidentity = 0; break; }
    }

    AE_PROFILE_LEAVE();
    return(isidentity);
}

int idx_is_identity(void* idx_p, const ae_index_type_t type, const size_t count)
{
    AE_PROFILE_ENTER(); int v = 0;

    ae_switch (type, ae_index_type, AE_INDEX_TYPE, suffix)
    {
        case AE_INDEX_TYPE_U8:
        {
            v = idx_is_identity_u8((u8*)idx_p, count);
        }
        break;

        case AE_INDEX_TYPE_U16:
        {
            v = idx_is_identity_u16((u16*)idx_p, count);
        }
        break;

        case AE_INDEX_TYPE_U32:
        {
            v = idx_is_identity_u32((u32*)idx_p, count);
        }
        break;

        default: ae_assert(0, "%u", (u32)type); break;
    }

    AE_PROFILE_LEAVE(); return v;
}

/*
================================================================================
 * ~~ [ scalar offsets ] ~~ *
--------------------------------------------------------------------------------
*/

static void idx_add_offset_u8 (u8 * idx, const size_t num, const size_t val)
{
    AE_PROFILE_ENTER();

    u8 * end = idx + num; // TODO: wrap check
    for (; idx < end; idx++) { *idx += val; }

    AE_PROFILE_LEAVE();
}

static void idx_add_offset_u16(u16* idx, const size_t num, const size_t val)
{
    AE_PROFILE_ENTER();

    u16* end = idx + num; // TODO: wrap check
    for (; idx < end; idx++) { *idx += val; }

    AE_PROFILE_LEAVE();
}

static void idx_add_offset_u32(u32* idx, const size_t num, const size_t val)
{
    AE_PROFILE_ENTER();

    u32* end = idx + num; // TODO: wrap check
    for (; idx < end; idx++) { *idx += val; }

    AE_PROFILE_LEAVE();
}

void idx_add_offset(void* indices, const ae_index_type_t type,
                    const size_t count, const size_t offset_v)
{
    AE_PROFILE_ENTER();

    ae_switch (type, ae_index_type, AE_INDEX_TYPE, suffix)
    {
        case AE_INDEX_TYPE_U8:
        {
            idx_add_offset_u8 ((u8 *)indices, count, offset_v);
        }
        break;

        case AE_INDEX_TYPE_U16:
        {
            idx_add_offset_u16((u16*)indices, count, offset_v);
        }
        break;

        case AE_INDEX_TYPE_U32:
        {
            idx_add_offset_u32((u32*)indices, count, offset_v);
        }
        break;

        default: ae_assert(0, "%u", (u32)type); break;
    }

    AE_PROFILE_LEAVE();
}

static void idx_sub_offset_u8 (u8 * idx, const size_t num, const size_t val)
{
    AE_PROFILE_ENTER();

    u8 * end = idx + num; // TODO: wrap check
    for (; idx < end; idx++) { *idx -= val; }

    AE_PROFILE_LEAVE();
}

static void idx_sub_offset_u16(u16* idx, const size_t num, const size_t val)
{
    AE_PROFILE_ENTER();

    u16* end = idx + num; // TODO: wrap check
    for (; idx < end; idx++) { *idx -= val; }

    AE_PROFILE_LEAVE();
}

static void idx_sub_offset_u32(u32* idx, const size_t num, const size_t val)
{
    AE_PROFILE_ENTER();

    u32* end = idx + num; // TODO: wrap check
    for (; idx < end; idx++) { *idx -= val; }

    AE_PROFILE_LEAVE();
}

void idx_sub_offset(void* indices, const ae_index_type_t type,
                    const size_t count, const size_t offset_v)
{
    AE_PROFILE_ENTER();

    ae_switch (type, ae_index_type, AE_INDEX_TYPE, suffix)
    {
        case AE_INDEX_TYPE_U8:
        {
            idx_sub_offset_u8 ((u8 *)indices, count, offset_v);
        }
        break;

        case AE_INDEX_TYPE_U16:
        {
            idx_sub_offset_u16((u16*)indices, count, offset_v);
        }
        break;

        case AE_INDEX_TYPE_U32:
        {
            idx_sub_offset_u32((u32*)indices, count, offset_v);
        }
        break;

        default: ae_assert(0, "%u", (u32)type); break;
    }

    AE_PROFILE_LEAVE();
}

/*
================================================================================
 * ~~ [ type conversion ] ~~ *
--------------------------------------------------------------------------------
*/

static int idx_does_u16_fit_u8 (u16* idx, const size_t count)
{
    AE_PROFILE_ENTER(); int v = 1;

    u16 * end = idx + count;
    for (; idx < end; idx++)
    {
        if (*idx > AE_U8_MAX) { v = 0; break; }
    }

    AE_PROFILE_LEAVE(); return(v);
}

static int idx_does_u32_fit_u8 (u32* idx, const size_t count)
{
    AE_PROFILE_ENTER(); int v = 1;

    u32 * end = idx + count;
    for (; idx < end; idx++)
    {
        if (*idx > AE_U8_MAX) { v = 0; break; }
    }

    AE_PROFILE_LEAVE(); return(v);
}

static int idx_does_u32_fit_u16(u32* idx, const size_t count)
{
    AE_PROFILE_ENTER(); int v = 1;

    u32 * end = idx + count;
    for (; idx < end; idx++)
    {
        if (*idx > AE_U16_MAX) { v = 0; break; }
    }

    AE_PROFILE_LEAVE(); return(v);
}

int idx_fits_type(const void * const idx, const size_t count,
        const ae_index_type_t src, const ae_index_type_t dst)
{
    AE_PROFILE_ENTER(); int v = 1;

    ae_switch (src, ae_index_type, AE_INDEX_TYPE, suffix)
    {
        case AE_INDEX_TYPE_U8: break; // u8 fits in anything!
        case AE_INDEX_TYPE_U16:
        {
            ae_switch (dst, ae_index_type, AE_INDEX_TYPE, suffix)
            {
                case AE_INDEX_TYPE_U8:
                {
                    v = idx_does_u16_fit_u8((u16*)idx, count);
                }
                break;

                case AE_INDEX_TYPE_U16: break;
                case AE_INDEX_TYPE_U32: break;

                default: ae_assert(0, "%u", (u32)dst); break;
            }
        }
        break;

        case AE_INDEX_TYPE_U32:
        {
            ae_switch (dst, ae_index_type, AE_INDEX_TYPE, suffix)
            {
                case AE_INDEX_TYPE_U8:
                {
                    v = idx_does_u32_fit_u8((u32*)idx, count);
                }
                break;

                case AE_INDEX_TYPE_U16:
                {
                    v = idx_does_u32_fit_u16((u32*)idx, count);
                }
                break;

                case AE_INDEX_TYPE_U32: break;

                default: ae_assert(0, "%u", (u32)dst); break;
            }
        }
        break;

        default: ae_assert(0, "%u", (u32)src); break;
    }

    AE_PROFILE_LEAVE(); return(v);
}

ae_index_type_t idx_smallest_fitting_type(const void* const idx,
                const ae_index_type_t type, const size_t count)
{
    AE_PROFILE_ENTER(); ae_index_type_t ret = AE_INDEX_TYPE_U32;

    // TODO: it could be faster to put both of these into one loop.
    // keep a fits u8 flag and break if the value is > AE_U16_MAX.
    // we could even back into a u16-only loop if u8 packing fails.
    ae_if (idx_fits_type(idx, count, type, AE_INDEX_TYPE_U8))
    {
        ret = AE_INDEX_TYPE_U8;
    }
    else ae_if (idx_fits_type(idx, count, type, AE_INDEX_TYPE_U16))
    {
        ret = AE_INDEX_TYPE_U16;
    }

    AE_PROFILE_LEAVE(); return ret;
}

void* idx_temp_type_converted_get(void * idx, const size_t count,
        const ae_index_type_t src_t, const ae_index_type_t dst_t)
{
    // TODO: debug CRC before mem to ensure no mutation of temps?
    void* p = ae_stack_malloc(ae_global_stack(),
            count * ae_index_type_bytes[dst_t]);

    // this is unexpected slow behavior that i want to know about
    #if defined(AE_DEBUG)
    if ( src_t == dst_t )
    {
        AE_WARN("frivolous copy of %u-element %s array",
            (unsigned)count, ae_index_type_name[src_t]);
    }
    #endif

    idx_type_converted_ex(idx, src_t, p, dst_t, count); return p;
}

void idx_temp_type_converted_free(void * dst, // stack free temp
                    const size_t n, const ae_index_type_t dst_t)
{
    ae_stack_free(ae_global_stack(), dst, n * ae_index_type_bytes[dst_t]);
}

void idx_type_convert(ae_array_t* indices, // expand or contract in-place
                    const ae_index_type_t src, const ae_index_type_t dst)
{
    AE_PROFILE_ENTER();

    const size_t src_type_bytes = ae_index_type_bytes[src];
    const size_t dst_type_bytes = ae_index_type_bytes[dst];

    const size_t count = indices->size / src_type_bytes;

    ae_if      (dst > src) // grow and unpack
    {
        ae_array_resize(indices, count * dst_type_bytes);
        idx_type_convert_ex(indices->data, src, dst, count);
    }
    else ae_if (dst < src) // pack and shrink
    {
        idx_type_convert_ex(indices->data, src, dst, count);
        ae_array_resize(indices, count * dst_type_bytes);
    }
    /*else
    {
        idx_type_convert_ex(indices->data, src, dst, count);
    }*/

    AE_PROFILE_LEAVE();
}

static void idx_pack_u32_into_u8(u32* indices, const size_t count)
{
    AE_PROFILE_ENTER();

    u8 * dst = (u8 *)indices;
    size_t i = 0;

    for (; i < count; i++)
    {
        dst[i] = indices[i];
    }

    AE_PROFILE_LEAVE();
}

static void idx_pack_u32_into_u16(u32* indices, const size_t count)
{
    AE_PROFILE_ENTER();

    u16* dst = (u16*)indices;
    size_t i = 0;

    for (; i < count; i++)
    {
        dst[i] = indices[i];
    }

    AE_PROFILE_LEAVE();
}

static void idx_pack_u16_into_u8(u16* indices, const size_t count)
{
    AE_PROFILE_ENTER();

    u8 * dst = (u8 *)indices;
    size_t i = 0;

    for (; i < count; i++)
    {
        dst[i] = indices[i];
    }

    AE_PROFILE_LEAVE();
}

static void idx_unpack_u32_from_u8(u8* indices, const size_t count)
{
    AE_PROFILE_ENTER();

    u32* dst = (u32*)indices;
    size_t i = count;

    for (; i; i--)
    {
        dst[i - 1] = indices[i - 1];
    }

    AE_PROFILE_LEAVE();
}

static void idx_unpack_u32_from_u16(u16* indices, const size_t count)
{
    AE_PROFILE_ENTER();

    u32* dst = (u32*)indices;
    size_t i = count;

    for (; i; i--)
    {
        dst[i - 1] = indices[i - 1];
    }

    AE_PROFILE_LEAVE();
}

static void idx_unpack_u16_from_u8(u8* indices, const size_t count)
{
    AE_PROFILE_ENTER();

    u16* dst = (u16*)indices;
    size_t i = count;

    for (; i; i--)
    {
        dst[i - 1] = indices[i - 1];
    }

    AE_PROFILE_LEAVE();
}

void idx_type_convert_ex(void* indices, // doesn't reallocate index array
    const ae_index_type_t src, const ae_index_type_t dst, const size_t n)
{
    AE_PROFILE_ENTER(); // check if new indices will fit without wrapping
    ae_assert(idx_fits_type(indices, n, src, dst),

        "%u-element index buffer of type %s at %p won't fit in %s", (u32)n,
        ae_index_type_name[src], indices, ae_index_type_name[dst]);

    // avoiding a possible bug if we loop backwards in the implementation
    if (n == 0) { AE_PROFILE_LEAVE(); return; }

    ae_switch (src, ae_index_type, AE_INDEX_TYPE, suffix)
    {
        case AE_INDEX_TYPE_U8:
        {
            ae_switch (dst, ae_index_type, AE_INDEX_TYPE, suffix)
            {
                case AE_INDEX_TYPE_U8: break;
                case AE_INDEX_TYPE_U16:
                {
                    idx_unpack_u16_from_u8((u8*)indices, n);
                }
                break;

                case AE_INDEX_TYPE_U32:
                {
                    idx_unpack_u32_from_u8((u8*)indices, n);
                }
                break;

                default: ae_assert(0, "%u", (u32)dst); break;
            }
        }
        break;

        case AE_INDEX_TYPE_U16:
        {
            ae_switch (dst, ae_index_type, AE_INDEX_TYPE, suffix)
            {
                case AE_INDEX_TYPE_U8:
                {
                    idx_pack_u16_into_u8((u16*)indices, n);
                }
                break;

                case AE_INDEX_TYPE_U16: break;
                case AE_INDEX_TYPE_U32:
                {
                    idx_unpack_u32_from_u16((u16*)indices, n);
                }
                break;

                default: ae_assert(0, "%u", (u32)dst); break;
            }
        }
        break;

        case AE_INDEX_TYPE_U32:
        {
            ae_switch (dst, ae_index_type, AE_INDEX_TYPE, suffix)
            {
                case AE_INDEX_TYPE_U8:
                {
                    idx_pack_u32_into_u8((u32*)indices, n);
                }
                break;

                case AE_INDEX_TYPE_U16:
                {
                    idx_pack_u32_into_u16((u32*)indices, n);
                }
                break;

                case AE_INDEX_TYPE_U32: break;
                default: ae_assert(0, "%u", (u32)dst); break;
            }
        }
        break;

        default: ae_assert(0, "%u", (u32)src); break;
    }

    AE_PROFILE_LEAVE();
}

void idx_type_converted(ae_array_t* src, const ae_index_type_t src_t,
                        ae_array_t* dst, const ae_index_type_t dst_t)
{
    AE_PROFILE_ENTER();

    const size_t src_type_bytes = ae_index_type_bytes[src_t];
    const size_t dst_type_bytes = ae_index_type_bytes[dst_t];

    const size_t count = src->size / src_type_bytes;

    ae_array_resize(dst, count * dst_type_bytes); // (un)pack buffer
    idx_type_converted_ex(src->data, src_t, dst->data, dst_t, count);

    AE_PROFILE_LEAVE();
}

#define AE_INDEX_PACK_OP(src_t, dst_t)  \
    AE_PROFILE_ENTER();                 \
                                        \
    src_t* end = src + count;           \
    while (src < end) *dst++ = *src++;  \
                                        \
    AE_PROFILE_LEAVE();

static void idx_packed_u32_into_u8(u32* src, u8* dst, const size_t count)
{
    AE_INDEX_PACK_OP(u32, u8);
}

static void idx_packed_u32_into_u16(u32* src, u16* dst, const size_t count)
{
    AE_INDEX_PACK_OP(u32, u16);
}

static void idx_packed_u16_into_u8(u16* src, u8* dst, const size_t count)
{
    AE_INDEX_PACK_OP(u16, u8);
}

static void idx_unpacked_u32_from_u8(u8* src, u32* dst, const size_t count)
{
    AE_INDEX_PACK_OP(u8, u32);
}

static void idx_unpacked_u32_from_u16(u16* src, u32* dst, const size_t count)
{
    AE_INDEX_PACK_OP(u16, u32);
}

static void idx_unpacked_u16_from_u8(u8* src, u16* dst, const size_t count)
{
    AE_INDEX_PACK_OP(u8, u16);
}

void idx_type_converted_ex( void* src, const ae_index_type_t src_t,
        void* dst, const ae_index_type_t dst_t, const size_t count)
{
    AE_PROFILE_ENTER();

    ae_assert(src != dst, "src and dst of type %s (to %s) alias at %p",
            ae_index_type_name[src_t], ae_index_type_name[dst_t], src);

    ae_switch (src_t, ae_index_type, AE_INDEX_TYPE, suffix)
    {
        case AE_INDEX_TYPE_U8:
        {
            ae_switch (dst_t, ae_index_type, AE_INDEX_TYPE, suffix)
            {
                case AE_INDEX_TYPE_U8:
                {
                    memcpy(dst, src, count * sizeof(u8));
                }
                break;

                case AE_INDEX_TYPE_U16:
                {
                    idx_unpacked_u16_from_u8((u8*)src, (u16*)dst, count);
                }
                break;

                case AE_INDEX_TYPE_U32:
                {
                    idx_unpacked_u32_from_u8((u8*)src, (u32*)dst, count);
                }
                break;

                default: ae_assert(0, "%u", (u32)dst_t); break;
            }
        }
        break;

        case AE_INDEX_TYPE_U16:
        {
            ae_switch (dst_t, ae_index_type, AE_INDEX_TYPE, suffix)
            {
                case AE_INDEX_TYPE_U8:
                {
                    idx_packed_u16_into_u8((u16*)src, (u8*)dst, count);
                }
                break;

                case AE_INDEX_TYPE_U16:
                {
                    memcpy(dst, src, count * sizeof(u16));
                }
                break;

                case AE_INDEX_TYPE_U32:
                {
                    idx_unpacked_u32_from_u16((u16*)src, (u32*)dst, count);
                }
                break;

                default: ae_assert(0, "%u", (u32)dst_t); break;
            }
        }
        break;

        case AE_INDEX_TYPE_U32:
        {
            ae_switch (dst_t, ae_index_type, AE_INDEX_TYPE, suffix)
            {
                case AE_INDEX_TYPE_U8:
                {
                    idx_packed_u32_into_u8((u32*)src, (u8*)dst, count);
                }
                break;

                case AE_INDEX_TYPE_U16:
                {
                    idx_packed_u32_into_u16((u32*)src, (u16*)dst, count);
                }
                break;

                case AE_INDEX_TYPE_U32:
                {
                    memcpy(dst, src, count * sizeof(u32));
                }
                break;

                default: ae_assert(0, "%u", (u32)dst_t); break;
            }
        }
        break;

        default: ae_assert(0, "%u", (u32)src_t); break;
    }

    AE_PROFILE_LEAVE();
}

/*
================================================================================
 * ~~ [ vertex cache optimization ] ~~ *
--------------------------------------------------------------------------------
TODO: the CryEngine implementation of this method can handle degenerate triangle
definitions like (8, 1, 8) and (6, 2, 6). is this a useful feature that we need?
--------------------------------------------------------------------------------
*/

void idx_vertex_cache_optimize_ex(void * indices, const ae_index_type_t type,
                        const size_t index_count, const size_t vertex_count)
{
    if (type == AE_INDEX_TYPE_U32)
    {
        /* TODO: make this implementation unsigned, so U32_MAX will be the magic sentinel.
         * TODO: use idx_value_get/set instead of converting to and from a 32-bit buffer?
         */
        typedef struct VCacheVert
        {
            int CachePos;   // its position in the cache (-1 if not in)
            int Score;      // its score (higher=better)
            int TrisLeft;   // number of not-yet-used tris
            int *TriList;   // list of triangle indices
            int OpenPos;    // position in "open vertex" list
        } \
            VCacheVert;

        typedef struct VCacheTri
        {
            int Score;      // current score (-1 if already done)
            int Inds[3];    // vertex indices
        } \
            VCacheTri;

        AE_PROFILE_ENTER(); // NOTE: original sint32 implementation by Fabian "ryg" Giesen
        int* IndexBuffer = (int*)indices;

        #define MAX_VALENCE 15
        #define CACHE_SIZE 32

        int val2Score[MAX_VALENCE+1];
        int cache[CACHE_SIZE+3];
        int pos2Score[CACHE_SIZE];

        int i, j, nTris, *IndPtr, *VertTriInd, *VertTriPtr, *OpenVerts, OpenCount, SeedPos;

        VCacheVert* verts;
        VCacheTri* tris;

        verts = (VCacheVert*)ae_stack_malloc(ae_global_stack(),
                            vertex_count * sizeof(VCacheVert));

        for (i = 0; i < (int)vertex_count; i++)
        {
            verts[i].CachePos = -1;
            verts[i].Score = 0;
            verts[i].TrisLeft = 0;
            verts[i].TriList = 0;
            verts[i].OpenPos = -1;
        }

        nTris = index_count / 3;
        tris = (VCacheTri*)ae_stack_malloc(ae_global_stack(), nTris * sizeof(VCacheTri));
        IndPtr = IndexBuffer;

        for (i = 0; i < nTris; i++)
        {
            tris[i].Score = 0;

            for (j = 0; j < 3; j++)
            {
                const int ind = *IndPtr++;

                tris[i].Inds[j] = ind;
                verts[ind].TrisLeft++;
            }
        }

        VertTriInd = (int*)ae_stack_malloc(ae_global_stack(), nTris * 3 * sizeof(int));
        VertTriPtr = VertTriInd;

        for (i = 0; i < (int)vertex_count; i++)
        {
            verts[i].TriList = VertTriPtr;
            VertTriPtr += verts[i].TrisLeft;
            verts[i].TrisLeft = 0;
        }

        for (i = 0; i < nTris; i++)
        {
            for (j = 0; j < 3; j++)
            {
                const int ind = tris[i].Inds[j];

                verts[ind].TriList[verts[ind].TrisLeft] = i;
                verts[ind].TrisLeft++;
            }
        }

        OpenVerts = (int*)ae_stack_malloc(ae_global_stack(), vertex_count * sizeof(int));
        OpenCount = 0;

        for (i = 0; i < CACHE_SIZE + 3; i++)
        {
            cache[i] = -1;
        }

        for (i = 0; i < CACHE_SIZE; i++)
        {
            const float score = (i < 3) ? 0.75f : ae_powf(1.0f - (i - 3) /
                                            (float)(CACHE_SIZE - 3), 1.5f);

            pos2Score[i] = (int) (score * 65536.0f + 0.5f);
        }

        val2Score[0] = 0;

        for (i = 1; i < 16; i++)
        {
            const float score = 2.0f / ae_sqrtf((float) i);
            val2Score[i] = (int) (score * 65536.0f + 0.5f);
        }

        IndPtr = IndexBuffer;
        SeedPos = 0;

        while (1) // outer loop - find a suitable candidate triangle to start with.
        {
            int BestTriInd, k;

            int SeedScore = -1;
            int SeedTri = -1;

            // if there are open verts, search them for the seed tri with max score.
            for (i = 0; i < OpenCount; i++)
            {
                VCacheVert *vert = &verts[OpenVerts[i]];

                for (j = 0; j < vert->TrisLeft; j++)
                {
                    const int TriInd = vert->TriList[j];
                    VCacheTri *tri = &tris[TriInd];

                    ae_if (tri->Score > SeedScore)
                    {
                        SeedScore = tri->Score;
                        SeedTri = TriInd;
                    }
                }
            }

            // if we haven't found a seed tri, there are no open verts! pick any.
            ae_if (SeedTri == -1)
            {
                while (SeedPos < nTris && tris[SeedPos].Score < 0)
                {
                    SeedPos++;
                }

                ae_if (SeedPos == nTris)
                {
                    break;
                }

                SeedTri = SeedPos;
            }

            BestTriInd = SeedTri;

            while (BestTriInd != -1)
            {
                /* main loop! mark this tri as used, remove it from the "remaining
                 * tris" list of the verts it uses, and add it to the index buffer.
                 */
                int BestTriScore = -1;

                VCacheTri *BestTri = &tris[BestTriInd];
                BestTri->Score = -1;

                for (j = 0; j < 3; j++)
                {
                    VCacheVert *vert;

                    const int VertInd = BestTri->Inds[j];
                    *IndPtr++ = VertInd;

                    k = 0; // find this triangles' entry
                    vert = &verts[VertInd];

                    while (vert->TriList[k] != BestTriInd)
                    {
                        assert(k < vert->TrisLeft); k++;
                    }

                    // swap it to the end and decrement the number of tris left.
                    ae_if (--vert->TrisLeft)
                    {
                        memswap(&vert->TriList[k], &vert->TriList[vert->TrisLeft], sizeof(int));
                    }
                    else ae_if (vert->OpenPos >= 0)
                    {
                        memswap(&OpenVerts[vert->OpenPos], &OpenVerts[--OpenCount], sizeof(int));
                    }
                }

                // update the status of the cache; start by marking sentinel values.
                cache[CACHE_SIZE] = cache[CACHE_SIZE+1] = cache[CACHE_SIZE+2] = -1;

                for (j = 0; j < 3; j++)
                {
                    int pos;

                    const int ind = BestTri->Inds[j];
                    cache[CACHE_SIZE + 2] = ind;

                    for (pos = 0; cache[pos] != ind; pos++);

                    for (k = pos; k > 0; k--)
                    {
                        cache[k] = cache[k - 1];
                    }

                    cache[0] = ind;

                    ae_if (pos != CACHE_SIZE + 2)
                    {
                        cache[CACHE_SIZE + 2] = -1;
                    }
                }

                for (i = 0; i < CACHE_SIZE + 3; i++)
                {
                    VCacheVert *vert;

                    const int VertInd = cache[i];
                    ae_if (VertInd == -1)
                    {
                        continue;
                    }

                    vert = &verts[VertInd];
                    vert->Score = val2Score[ae_imin(vert->TrisLeft, MAX_VALENCE)];

                    ae_if (i < CACHE_SIZE)
                    {
                        vert->CachePos = i;
                        vert->Score += pos2Score[i];
                    }
                    else
                    {
                        vert->CachePos = -1;
                    }

                    ae_if (vert->OpenPos < 0 && vert->TrisLeft)
                    {
                        vert->OpenPos = OpenCount;
                        OpenVerts[OpenCount++] = VertInd;
                    }
                }

                // update triangle scores and find the new best triangle.
                BestTriInd = -1;

                for (i = 0; i < CACHE_SIZE; i++)
                {
                    const VCacheVert * vert;

                    ae_if (cache[i] == -1)
                    {
                        continue;
                    }

                    vert = &verts[cache[i]];

                    for (j = 0; j < vert->TrisLeft; j++)
                    {
                        int score = 0;

                        const int TriInd = vert->TriList[j];
                        VCacheTri *tri = &tris[TriInd];

                        ae_assert(tri->Score != -1, "bad tri score: %i",
                                                            tri->Score);

                        for (k = 0; k < 3; k++)
                        {
                            score += verts[tri->Inds[k]].Score;
                        }

                        tri->Score = score;

                        ae_if (score > BestTriScore)
                        {
                            BestTriScore = score;
                            BestTriInd = TriInd;
                        }
                    }
                }
            }
        }

        ae_stack_free(ae_global_stack(), OpenVerts, vertex_count * sizeof(int));
        ae_stack_free(ae_global_stack(), VertTriInd, nTris * 3 * sizeof(int));
        ae_stack_free(ae_global_stack(), tris, nTris * sizeof(VCacheTri));
        ae_stack_free(ae_global_stack(), verts, vertex_count * sizeof(VCacheVert));

        #undef MAX_VALENCE
        #undef CACHE_SIZE

        AE_PROFILE_LEAVE();
    }
    else
    {
        void* temp = idx_temp_type_converted_get((void*)indices, index_count,
                                                    type, AE_INDEX_TYPE_U32);

        idx_vertex_cache_optimize_ex(temp, AE_INDEX_TYPE_U32, index_count, vertex_count);
        idx_type_converted_ex(temp, AE_INDEX_TYPE_U32, indices, type, index_count);

        idx_temp_type_converted_free(temp, index_count, AE_INDEX_TYPE_U32);
    }
}

void idx_vertex_cache_optimize(void* idx_buffer, const ae_index_type_t type,
                                                        const size_t count)
{
    AE_PROFILE_ENTER();

    if (count) idx_vertex_cache_optimize_ex(idx_buffer, type, count,
                        idx_value_max(idx_buffer, type, count) + 1);

    AE_PROFILE_LEAVE();
}

double
idx_vertex_cache_miss_ratio(const void* const idx, const ae_index_type_t type,
                                const size_t count, int cache_size, int FIFO)
{
    double ACMR;

    if (type == AE_INDEX_TYPE_U32)
    {
        AE_PROFILE_ENTER();

        int misses = 0, cache_pos;
        int *cache = (int*)alloca((cache_size + 1) * sizeof(int));
        int wr_pos = 0, i, j;

        ae_if (!count)
        {
            AE_PROFILE_LEAVE(); return 0.0;
        }

        for (i = 0; i < cache_size; i++)
        {
            cache[i] = -1;
        }

        for (i = 0; i < (int)count; i++)
        {
            int* buffer = (int*)idx;
            const int ind = buffer[i];
            cache[cache_size] = ind;

            for (cache_pos = 0; cache[cache_pos] != buffer[i]; cache_pos++);
            misses += cache_pos == cache_size;

            ae_if (FIFO)
            {
                cache[wr_pos] = ind;

                ae_if (++wr_pos == cache_size)
                {
                    wr_pos = 0;
                }
            }
            else
            {
                for (j = cache_pos; j > 0; j--)
                {
                    cache[j] = cache[j - 1];
                }

                cache[0] = ind;
            }
        }

        ACMR = misses * 3.0 / count;
        AE_PROFILE_LEAVE();
    }
    else
    {
        void * t = idx_temp_type_converted_get((void *)idx, count,
                                        type, AE_INDEX_TYPE_U32);

        ACMR = idx_vertex_cache_miss_ratio( t, AE_INDEX_TYPE_U32,
                                        count, cache_size, FIFO);

        idx_temp_type_converted_free(t, count, AE_INDEX_TYPE_U32);
    }

    return ACMR;
}
