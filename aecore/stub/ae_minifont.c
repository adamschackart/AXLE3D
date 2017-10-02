/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void ae_minifont8x8_putc(char c, int x, int y, u32* font, float* color, ae_image_t* dst)
{
    AE_STUB();
}

void ae_minifont8x8_puts(const char* s, int x, int y, u32* font, float* color, ae_image_t* dst)
{
    AE_STUB();
}

void ae_minifont8x8_printf( int x, int y, u32* font, float* color,
                            ae_image_t* dst, const char* fmt, ...)
{
    AE_STUB();
}

void ae_minifont8x8_flip_glyph_x(char c, u32* font)
{
    AE_STUB();
}

void ae_minifont8x8_flip_glyph_y(char c, u32* font)
{
    AE_STUB();
}

void ae_minifont8x8_flip_x(u32* font)
{
    AE_STUB();
}

void ae_minifont8x8_flip_y(u32* font)
{
    AE_STUB();
}

void ae_minifont8x8_negative_glyph(char c, u32* font)
{
    AE_STUB();
}

void ae_minifont8x8_negative(u32* font)
{
    AE_STUB();
}

void ae_minifont8x8_clear_glyph(char c, u32* font)
{
    AE_STUB();
}

void ae_minifont8x8_clear(u32* font)
{
    AE_STUB();
}

void ae_minifont8x8_fill_glyph(char c, u32* font)
{
    AE_STUB();
}

void ae_minifont8x8_fill(u32* font)
{
    AE_STUB();
}

void ae_minifont8x8_pack_glyph_ex(u32* out, float* key_color, float epsilon,
                                    int img_x, int img_y, ae_image_t* image)
{
    AE_STUB();
}

void ae_minifont8x8_pack_glyph(char c, u32* font, float* key_color,
            float epsilon, int img_x, int img_y, ae_image_t* image)
{
    AE_STUB();
}

void ae_minifont_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_minifont_quit(void)
{
    // AE_STUB();
}
