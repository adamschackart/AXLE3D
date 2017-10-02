/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------

A minifont is a simple set of text characters that has the following properties:

    mini-ascii (upper 96 ASCII characters, omits non-printable control codes)
    monochrome (one color, one bit of storage requirement per pixel)
    monospace (glyphs are equally spaced 8x8 pixel characters)
    little-endian (bit values ascend from right to left for intel/arm chips)

The font data is thus represented as an array of 192 u32s (two words per glyph).
For more heavyweight features (utf-8, variable-width) use a truetype library.
This library has been previously referred to as both "Nintexto" and "Fontendo".

For an example of this kind of system in practice, see the tonc text rendering
tutorial for the Gameboy Advance at http://www.coranac.com/tonc/text/text.htm

----------------------------------------------------------------------------- */
#ifndef __AE_MINIFONT_H__
#define __AE_MINIFONT_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif
#ifndef __AE_IMAGE_H__
#include <ae_image.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_minifont_init(int argc, char** argv);
void ae_minifont_quit(void); // private init

/*
================================================================================
 * ~~ [ 8x8 fonts ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL // draw character C at the given position without a filled background.
ae_minifont8x8_putc(char c, int x, int y, u32* font, float* color, ae_image_t* dst);

AE_DECL void AE_CALL // draw null-terminated string S. handles newline and tab characters.
ae_minifont8x8_puts(const char* s, int x, int y, u32* font, float* color, ae_image_t* dst);

AE_DECL void AE_CALL // draw null-terminated string S after formatting variable arguments.
ae_minifont8x8_printf(int x, int y, u32 * font, float * color, ae_image_t * dst,
                                    const char * fmt, ...) AE_PRINTF_ARGS(5, 6);

/* ===== [ font manipulation ] ============================================== */

AE_DECL void AE_CALL ae_minifont8x8_flip_glyph_x(char c, u32* font);
AE_DECL void AE_CALL ae_minifont8x8_flip_glyph_y(char c, u32* font);

AE_DECL void AE_CALL ae_minifont8x8_flip_x(u32* font);
AE_DECL void AE_CALL ae_minifont8x8_flip_y(u32* font);

AE_DECL void AE_CALL ae_minifont8x8_negative_glyph(char c, u32* font);
AE_DECL void AE_CALL ae_minifont8x8_negative(u32* font);

AE_DECL void AE_CALL ae_minifont8x8_clear_glyph(char c, u32* font);
AE_DECL void AE_CALL ae_minifont8x8_clear(u32* font);

AE_DECL void AE_CALL ae_minifont8x8_fill_glyph(char c, u32* font);
AE_DECL void AE_CALL ae_minifont8x8_fill(u32* font);

/* ===== [ font creation ] ================================================== */

AE_DECL void AE_CALL
ae_minifont8x8_pack_glyph_ex(u32* out, float* key_color, float epsilon,
                            int img_x, int img_y, ae_image_t * image);

AE_DECL void AE_CALL
ae_minifont8x8_pack_glyph(char c, u32* font, float* key, float epsilon,
                            int img_x, int img_y, ae_image_t * image);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_MINIFONT_H__ */
