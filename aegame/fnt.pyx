# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
from img cimport IntRect, Image, ae_image_t
from mem cimport u32
from vec cimport Vec4

import sys # version info

cdef extern from "ae_minifont.h":
    # ==========================================================================
    # ~ [ 8x8 fonts ]
    # ==========================================================================

    void ae_minifont8x8_putc(char c, int x, int y, u32* font, float* color, ae_image_t* dst)

    void ae_minifont8x8_puts(const char* s, int x, int y,
                u32* font, float* color, ae_image_t* dst)

    void ae_minifont8x8_printf( int x, int y, u32* font, float* color,
                                ae_image_t* dst, const char* fmt, ...)

    # ===== [ font manipulation ] ==============================================

    void ae_minifont8x8_flip_glyph_x(char c, u32* font)
    void ae_minifont8x8_flip_glyph_y(char c, u32* font)

    void ae_minifont8x8_flip_x(u32* font)
    void ae_minifont8x8_flip_y(u32* font)

    void ae_minifont8x8_negative_glyph(char c, u32* font)
    void ae_minifont8x8_negative(u32* font)

    void ae_minifont8x8_clear_glyph(char c, u32* font)
    void ae_minifont8x8_clear(u32* font)

    void ae_minifont8x8_fill_glyph(char c, u32* font)
    void ae_minifont8x8_fill(u32* font)

    # ===== [ font creation ] ==================================================

    void ae_minifont8x8_pack_glyph_ex(u32* out, float* key_color, float epsilon,
                                        int img_x, int img_y, ae_image_t* image)

    void ae_minifont8x8_pack_glyph(char c, u32* font, float* key_color,
                float epsilon, int img_x, int img_y, ae_image_t* image)

# ==============================================================================
# ~ [ 8x8 fonts ]
# ==============================================================================

cdef class MiniFont8x8:
    cdef u32 font[192]

    def __repr__(self):
        return '{}({} empty)'.format(self.__class__.__name__, self.empty_glyph_count())

    def __len__(self):
        return 96

    def __getitem__(self, size_t index):
        if index >= 96: raise IndexError

        return (self.font[index * 2 + 0],
                self.font[index * 2 + 1])

    def __setitem__(self, size_t index, tuple value):
        if index >= 96: raise IndexError

        self.font[index * 2 + 0] = value[0]
        self.font[index * 2 + 1] = value[1]

    def address(self):
        return <size_t>self.font

    def set_v(self, *values):
        """Set all font data. Use this method to declare inline fonts in python files."""
        cdef int index
        cdef u32 value

        assert len(values) == 192, "{} {}".format(self, len(values))

        for index, value in enumerate(values):
            self.font[index] = value

        return self

    def empty_glyph_count(self):
        """Count the number of unfilled glyphs (zero pixels) in this font."""
        cdef int c = 0
        cdef int i

        cdef u32 top
        cdef u32 btm

        for i in range(0, 192, 2):
            top = self.font[i + 0]
            btm = self.font[i + 1]

            if not (top or btm):
                c += 1

        return c

    def create_atlas(self, int num_x = 16, int num_y = 6):
        assert num_x * num_y == 96, "{} * {} = {}".format(num_x, num_y, num_x * num_y)

        atlas = Image()
        atlas.width  = num_x * 8
        atlas.height = num_y * 8
        atlas.type   = Image.TYPE_FLT
        atlas.format = Image.FORMAT_RGB
        atlas.alloc()

        atlas.set_color(Vec4(1, 1, 1, 1))
        atlas.blit_grid_outline(IntRect(0, 0, 8, 8), num_x, num_y, Vec4(1, 0, 0, 1))

        x = 0
        y = (num_y - 1) * 8

        for i in range(32, 128, 1):
            self.putc(i, x, y, Vec4(0, 0, 0, 1), atlas)
            x += 8

            if x == atlas.width:
                x = 0
                y -= 8

        return atlas

    def putc(self, int character, int x, int y, Vec4 color, Image dst):
        """
        Render a single 8x8 character to an image - char is the character ordinal.
        """
        ae_minifont8x8_putc(character, x, y, self.font, color.v, &dst.image)
        return self

    def puts(self, str string, int x, int y, Vec4 color, Image dst):
        """
        Render a string to an image at a given location - font text is not resized.
        """
        cdef bytes b_string

        if sys.version_info.major > 2:
            b_string = <bytes>string.encode('utf-8')
        else:
            b_string = <bytes>string

        ae_minifont8x8_puts(<char*>b_string, x, y, self.font, color.v, &dst.image)
        return self

    def flip_glyph_x(self, int c):
        ae_minifont8x8_flip_glyph_x(c, self.font); return self

    def flip_glyph_y(self, int c):
        ae_minifont8x8_flip_glyph_y(c, self.font); return self

    def flip_x(self):
        ae_minifont8x8_flip_x(self.font); return self

    def flip_y(self):
        ae_minifont8x8_flip_y(self.font); return self

    def negative_glyph(self, int c):
        ae_minifont8x8_negative_glyph(c, self.font); return self

    def negative(self):
        ae_minifont8x8_negative(self.font); return self

    def clear_glyph(self, int c):
        ae_minifont8x8_clear_glyph(c, self.font); return self

    def clear(self):
        ae_minifont8x8_clear(self.font); return self

    def fill_glyph(self, int c):
        ae_minifont8x8_fill_glyph(c, self.font); return self

    def fill(self):
        ae_minifont8x8_fill(self.font); return self

    def pack_glyph(self, int c, Vec4 key, float epsilon, int x, int y, Image img):
        ae_minifont8x8_pack_glyph(c, self.font, key.v, epsilon, x, y, &img.image)
        return self

    def create_from_atlas(self, Image atlas):
        assert atlas.height & 8 == 0, "{}".format(atlas)
        assert atlas.width  % 8 == 0, "{}".format(atlas)

        th = atlas.height / 8
        tw = atlas.width  / 8

        assert tw * th == 96, "{} * {} = {}".format(tw, th, tw * th)

        x = 0
        y = atlas.height - 8

        for i in range(32, 128, 1):
            self.pack_glyph(i, Vec4(0, 0, 0, 1), 0.0001, x, y, atlas)
            x += 8

            if x >= atlas.width:
                x = 0
                y -= 8

        return self

    def write_to_c_file(self, is_static, typename, fontname, filename):
        """
            Write the font to a c-formatted file for inclusion in code.
        """
        with open(filename, 'w') as out_file:
            #
            # Write the standard C declaration stuff.
            #
            out_file.write("{}const {} {}[192] =\n{}\n".format("static "\
                        if is_static else "", typename, fontname, '{'))

            for i in range(0, 96, 3):
                ah, al = self[i + 0]
                bh, bl = self[i + 1]
                ch, cl = self[i + 2]

                out_file.write("    ")

                out_file.write("0x%0*x, "  % (8, ah))
                out_file.write("0x%0*x, "  % (8, al))
                out_file.write("0x%0*x, "  % (8, bh))
                out_file.write("0x%0*x, "  % (8, bl))
                out_file.write("0x%0*x, "  % (8, ch))
                out_file.write("0x%0*x,\n" % (8, cl))

            out_file.write("};\n")

        return self
