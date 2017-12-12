/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
TODO: this whole file assumes the target platform is little endian (swap blits)
TODO: render string as image (get string bounds, then blit text into the image)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void ae_minifont_init(int argc, char** argv) {}
void ae_minifont_quit(void) {}

// While it's helpful to be able to trace execution (coverage testing using the profile),
// the glyph drawing runtime is often dominated by the mechanics of the profiler itself.
#if 0
    #define MF_PROFILE_ENTER AE_PROFILE_ENTER
    #define MF_PROFILE_LEAVE AE_PROFILE_LEAVE
#else
    static c_inline void* mf_profile_enter(void) { return NULL; }
    static c_inline void  mf_profile_leave(void* p) { /* nop */ }

    #define MF_PROFILE_ENTER() void* _mf_profile_context = mf_profile_enter()
    #define MF_PROFILE_LEAVE() mf_profile_leave(_mf_profile_context)
#endif

/*
================================================================================
 * ~~ [ 8x8 fonts ] ~~ *
--------------------------------------------------------------------------------
*/

static void
ae_minifont8x8_putc_rgba_flt_clipped(int x, int y, u8* glyph, float* color, float* dst, int dst_w, int dst_h)
{
    MF_PROFILE_ENTER();
    int cur_x, cur_y;

    for (cur_y = y; cur_y < y + 8; cur_y++)
    {
        const size_t glyph_row = glyph[cur_y - y];
        if (cur_y < 0 || cur_y >= dst_h) continue;

        for (cur_x = x; cur_x < x + 8; cur_x++)
        {
            if (glyph_row & (1 << (cur_x - x)) && cur_x >= 0 && cur_x < dst_w)
            {
                // FIXME this super slow hack highlights the need for a single pixel blit function!!!
                int rect[] = { cur_x, dst_h - cur_y - 1, 1, 1 };

                ae_image_t temp =
                {
                    dst, (size_t)dst_w, (size_t)dst_h, AE_IMAGE_FORMAT_RGBA, AE_IMAGE_TYPE_FLT, NULL
                };

                ae_image_blit_rect(&temp, rect, color, 1, 1, 1, 1);
            }
        }
    }

    MF_PROFILE_LEAVE();
}

static void
ae_minifont8x8_putc_rgba_flt(int x, int y, u8* glyph, float* color, float* dst, int dst_w, int dst_h)
{
#if 1
    ae_minifont8x8_putc_rgba_flt_clipped(x, y, glyph, color, dst, dst_w, dst_h);
#else
    #error TODO
#endif
}

static void
ae_minifont8x8_putc_rgb_flt_clipped(int x, int y, u8* glyph, float* color, float* dst, int dst_w, int dst_h)
{
    MF_PROFILE_ENTER();
    int cur_x, cur_y;

    for (cur_y = y; cur_y < y + 8; cur_y++)
    {
        const size_t glyph_row = glyph[cur_y - y];
        if (cur_y < 0 || cur_y >= dst_h) continue;

        for (cur_x = x; cur_x < x + 8; cur_x++)
        {
            if (glyph_row & (1 << (cur_x - x)) && cur_x >= 0 && cur_x < dst_w)
            {
                const size_t offset = cur_x * 3 + cur_y * 3 * dst_w;

                dst[offset + 0] = color[0];
                dst[offset + 1] = color[1];
                dst[offset + 2] = color[2];
            }
        }
    }

    MF_PROFILE_LEAVE();
}

static void
ae_minifont8x8_putc_rgb_flt(int x, int y, u8* glyph, float* color, float* dst, int dst_w, int dst_h)
{
#if 0
    ae_minifont8x8_putc_rgb_flt_clipped(x, y, glyph, color, dst, dst_w, dst_h);
#else
    MF_PROFILE_ENTER();

    float* out = dst + x * 3 + y * 3 * dst_w;
    float* end = out + 8 * 3 * dst_w;

    for (; out < end; out += dst_w * 3)
    {
        const size_t glyph_row = *glyph++;

        if (glyph_row & (1 << 0)) memcpy(out + 3 * 0, color, sizeof(float[3]));
        if (glyph_row & (1 << 1)) memcpy(out + 3 * 1, color, sizeof(float[3]));
        if (glyph_row & (1 << 2)) memcpy(out + 3 * 2, color, sizeof(float[3]));
        if (glyph_row & (1 << 3)) memcpy(out + 3 * 3, color, sizeof(float[3]));
        if (glyph_row & (1 << 4)) memcpy(out + 3 * 4, color, sizeof(float[3]));
        if (glyph_row & (1 << 5)) memcpy(out + 3 * 5, color, sizeof(float[3]));
        if (glyph_row & (1 << 6)) memcpy(out + 3 * 6, color, sizeof(float[3]));
        if (glyph_row & (1 << 7)) memcpy(out + 3 * 7, color, sizeof(float[3]));
    }

    MF_PROFILE_LEAVE();
#endif
}

static void
ae_minifont8x8_putc_mono_flt_clipped(int x, int y, u8* glyph, float* color, float* dst, int dst_w, int dst_h)
{
    AE_STUB();
}

static void
ae_minifont8x8_putc_mono_flt(int x, int y, u8* glyph, float* color, float* dst, int dst_w, int dst_h)
{
    AE_STUB();
}

static void
ae_minifont8x8_putc_rgba_u8_clipped(int x, int y, u8* glyph, float* color, u8* dst, int dst_w, int dst_h)
{
    AE_STUB();
}

static void
ae_minifont8x8_putc_rgba_u8(int x, int y, u8* glyph, float* color, u8* dst, int dst_w, int dst_h)
{
    AE_STUB();
}

static void
ae_minifont8x8_putc_rgb_u8_clipped(int x, int y, u8* glyph, float* color, u8* dst, int dst_w, int dst_h)
{
    AE_STUB();
}

static void
ae_minifont8x8_putc_rgb_u8(int x, int y, u8* glyph, float* color, u8* dst, int dst_w, int dst_h)
{
    AE_STUB();
}

static void
ae_minifont8x8_putc_mono_u8_clipped(int x, int y, u8* glyph, float* color, u8* dst, int dst_w, int dst_h)
{
    AE_STUB();
}

static void
ae_minifont8x8_putc_mono_u8(int x, int y, u8* glyph, float* color, u8* dst, int dst_w, int dst_h)
{
    AE_STUB();
}

void ae_minifont8x8_putc(char c, int x, int y, u32* font, float* color, ae_image_t* dst)
{
    MF_PROFILE_ENTER();

    u8* glyph = (u8*)&font[(c - 32) * 2];

    int img_h = (int)dst->height,
        img_w = (int)dst->width;

    y = (img_h - 8) - y; // flip y

    if (x >= 0 && x + 8 <= img_w &&
        y >= 0 && y + 8 <= img_h)
    {
        switch (dst->type)
        {
            case AE_IMAGE_TYPE_FLT:
            {
                switch (dst->format)
                {
                    case AE_IMAGE_FORMAT_RGBA:
                    {
                        ae_minifont8x8_putc_rgba_flt(x, y, glyph, color, (float*)dst->pixels, img_w, img_h);
                    }
                    break;

                    case AE_IMAGE_FORMAT_RGB:
                    {
                        ae_minifont8x8_putc_rgb_flt(x, y, glyph, color, (float*)dst->pixels, img_w, img_h);
                    }
                    break;

                    case AE_IMAGE_FORMAT_MONO:
                    {
                        ae_minifont8x8_putc_mono_flt(x, y, glyph, color, (float*)dst->pixels, img_w, img_h);
                    }
                    break;

                    default:
                    {
                        ae_assert(0, "unsupported dst format %u", (u32)dst->format);
                    }
                    break;
                }
            }
            break;

            case AE_IMAGE_TYPE_U8:
            {
                switch (dst->format)
                {
                    case AE_IMAGE_FORMAT_RGBA:
                    {
                        ae_minifont8x8_putc_rgba_u8(x, y, glyph, color, (u8*)dst->pixels, img_w, img_h);
                    }
                    break;

                    case AE_IMAGE_FORMAT_RGB:
                    {
                        ae_minifont8x8_putc_rgb_u8(x, y, glyph, color, (u8*)dst->pixels, img_w, img_h);
                    }
                    break;

                    case AE_IMAGE_FORMAT_MONO:
                    {
                        ae_minifont8x8_putc_mono_u8(x, y, glyph, color, (u8*)dst->pixels, img_w, img_h);
                    }
                    break;

                    default:
                    {
                        ae_assert(0, "unsupported dst format %u", (u32)dst->format);
                    }
                    break;
                }
            }
            break;

            default:
            {
                ae_assert(0, "unsupported dst type %u", (u32)dst->type);
            }
            break;
        }
    }
    else
    {
        if (x >= img_w || x + 8 <= 0 ||
            y >= img_h || y + 8 <= 0)
        {
            MF_PROFILE_LEAVE(); return;
        }

        switch (dst->type)
        {
            case AE_IMAGE_TYPE_FLT:
            {
                switch (dst->format)
                {
                    case AE_IMAGE_FORMAT_RGBA:
                    {
                        ae_minifont8x8_putc_rgba_flt_clipped(x, y, glyph, color, (float*)dst->pixels, img_w, img_h);
                    }
                    break;

                    case AE_IMAGE_FORMAT_RGB:
                    {
                        ae_minifont8x8_putc_rgb_flt_clipped(x, y, glyph, color, (float*)dst->pixels, img_w, img_h);
                    }
                    break;

                    case AE_IMAGE_FORMAT_MONO:
                    {
                        ae_minifont8x8_putc_mono_flt_clipped(x, y, glyph, color, (float*)dst->pixels, img_w, img_h);
                    }
                    break;

                    default:
                    {
                        ae_assert(0, "unsupported dst format %u", (u32)dst->format);
                    }
                    break;
                }
            }
            break;

            case AE_IMAGE_TYPE_U8:
            {
                switch (dst->format)
                {
                    case AE_IMAGE_FORMAT_RGBA:
                    {
                        ae_minifont8x8_putc_rgba_u8_clipped(x, y, glyph, color, (u8*)dst->pixels, img_w, img_h);
                    }
                    break;

                    case AE_IMAGE_FORMAT_RGB:
                    {
                        ae_minifont8x8_putc_rgb_u8_clipped(x, y, glyph, color, (u8*)dst->pixels, img_w, img_h);
                    }
                    break;

                    case AE_IMAGE_FORMAT_MONO:
                    {
                        ae_minifont8x8_putc_mono_u8_clipped(x, y, glyph, color, (u8*)dst->pixels, img_w, img_h);
                    }
                    break;

                    default:
                    {
                        ae_assert(0, "unsupported dst format %u", (u32)dst->format);
                    }
                    break;
                }
            }
            break;

            default:
            {
                ae_assert(0, "unsupported dst type %u", (u32)dst->type);
            }
            break;
        }
    }

    MF_PROFILE_LEAVE();
}

void ae_minifont8x8_puts(const char* s, int x, int y, u32* font, float* color, ae_image_t* dst)
{
    AE_PROFILE_ENTER();

    int cur_x = x;
    int cur_y = y;

    for (;; s++)
    {
        switch (*s)
        {
            case '\0': AE_PROFILE_LEAVE(); return;
            case '\n': cur_x = x; cur_y -= 8; break;
            case '\t': cur_x += 8 * 4; break;

            default:
            {
                ae_minifont8x8_putc(*s, cur_x, cur_y, font, color, dst);
                cur_x += 8;
            }
            break;
        }
    }
}

void ae_minifont8x8_printf( int x, int y, u32* font, float* color,
                            ae_image_t* dst, const char* fmt, ...)
{
    AE_PROFILE_ENTER(); // track how much time we're spending formatting the string

    const size_t size = 1024 * 16;
    char* buffer = (char*)ae_stack_malloc(ae_global_stack(), size);

    va_list args;
    va_start(args, fmt);

    if (ae_vsnprintf(buffer, size, fmt, args) < 0)
    {
        AE_WARN("minifont8x8 printf string truncated (%u char max)", (u32)size - 1);
    }

    va_end(args);

    ae_minifont8x8_puts(buffer, x, y, font, color, dst);
    ae_stack_free(ae_global_stack(), buffer, size);

    AE_PROFILE_LEAVE();
}

/* ===== [ font manipulation ] ============================================== */

void ae_minifont8x8_flip_glyph_x(char c, u32* font)
{
    u8* chr = (u8*)&font[(c - 32) * 2];
    u8* end = chr + 8;

    for (; chr < end; chr++)
    {
    #if 1
        // Swap left four bits with right four bits. Then swap adjacent pairs,
        // then adjacent single bits. This results in a reversed bit order.
        *chr = (*chr & 0xF0) >> 4 | (*chr & 0x0F) << 4;
        *chr = (*chr & 0xCC) >> 2 | (*chr & 0x33) << 2;
        *chr = (*chr & 0xAA) >> 1 | (*chr & 0x55) << 1;
    #elif 0
        u8 v = *chr;
        u8 r = *chr;
        int s = 7;

        for (v >>= 1; v; v >>= 1)
        {
            r <<= 1;
            r |= v & 1;
            s--;
        }

        *chr = r << s;
    #elif 0
        // 7 ops (no 64-bit operations)
        *chr = ((*chr * 0x0802LU & 0x22110LU) | (*chr * 0x8020LU & 0x88440LU)) * 0x10101LU >> 16;
    #elif 0
        // 4 ops (64-bit mul, no mod)
        *chr = ((*chr * 0x80200802ULL) & 0x0884422110ULL) * 0x0101010101ULL >> 32;
    #elif 0
        // 3 ops (64-bit mul & modulus)
        *chr = (*chr * 0x0202020202ULL & 0x010884422010ULL) % 1023;
    #else
        #error no byte reverse implementation is available for this platform!
    #endif
    }
}

void ae_minifont8x8_flip_glyph_y(char c, u32* font)
{
    u8 temp, *chr = (u8*)&font[(c - 32) * 2];

    temp = chr[0]; chr[0] = chr[7]; chr[7] = temp;
    temp = chr[1]; chr[1] = chr[6]; chr[6] = temp;
    temp = chr[2]; chr[2] = chr[5]; chr[5] = temp;
    temp = chr[3]; chr[3] = chr[4]; chr[4] = temp;
}

void ae_minifont8x8_flip_x(u32* font)
{
    int c = 32;
    for (; c < 128; c++) ae_minifont8x8_flip_glyph_x(c, font);
}

void ae_minifont8x8_flip_y(u32* font)
{
    int c = 32;
    for (; c < 128; c++) ae_minifont8x8_flip_glyph_y(c, font);
}

void ae_minifont8x8_negative_glyph(char c, u32* font)
{
    u32* chr = &font[(c - 32) * 2];

    chr[0] = ~chr[0];
    chr[1] = ~chr[1];
}

void ae_minifont8x8_negative(u32* font)
{
    int c = 32;
    for (; c < 128; c++) ae_minifont8x8_negative_glyph(c, font);
}

void ae_minifont8x8_clear_glyph(char c, u32* font)
{
    u32* chr = &font[(c - 32) * 2];

    chr[0] = 0;
    chr[1] = 0;
}

void ae_minifont8x8_clear(u32* font)
{
    memset(font, 0, sizeof(u32[192]));
}

void ae_minifont8x8_fill_glyph(char c, u32* font)
{
    u32* chr = &font[(c - 32) * 2];

    chr[0] = 0xFFFFFFFF;
    chr[1] = 0xFFFFFFFF;
}

void ae_minifont8x8_fill(u32* font)
{
    memset(font, 0xFFFFFFFF, sizeof(u32[192]));
}

/* ===== [ font creation ] ================================================== */

void ae_minifont8x8_pack_glyph_ex(u32* out, float* key_color, float epsilon,
                                    int img_x, int img_y, ae_image_t* image)
{
    AE_PROFILE_ENTER();

    int y, img_h = (int)image->height,
        x, img_w = (int)image->width;

    // FIXME: this is an early implementation!
    assert(image->format == AE_IMAGE_FORMAT_RGB);
    assert(image->type == AE_IMAGE_TYPE_FLT);

    assert(img_x <= img_w - 8);
    assert(img_y <= img_h - 8);

    assert(img_x >= 0);
    assert(img_y >= 0);

    // invert from y up coord to y down
    img_y = (img_h - 8) - img_y;

    out[0] = 0;
    out[1] = 0;

    for (y = img_y; y < img_y + 8; y++)
    for (x = img_x; x < img_x + 8; x++)
    {
        float* p = (float*)ae_image_pixel(image, (size_t)x, (size_t)y);

        int gx = x - img_x;
        int gy = y - img_y;

        if (vec3cmp_e(p, key_color, epsilon))
        {
            ((u8*)out)[gy] |= (1 << gx);
        }
    }

    AE_PROFILE_LEAVE();
}

void ae_minifont8x8_pack_glyph(char c, u32* font, float* key_color,
            float epsilon, int img_x, int img_y, ae_image_t* image)
{
    ae_minifont8x8_pack_glyph_ex(&font[(c - 32) * 2],
            key_color, epsilon, img_x, img_y, image);
}
