# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
# TODO: add a function that adds a value to lists, dicts, and the c color table
# ------------------------------------------------------------------------------
from vec cimport Vec3, Vec4
from mem cimport u8

import sys # version info

cdef extern from "ae_color.h":
    ctypedef enum ae_color_t:
        AE_COLOR_BLACK
        AE_COLOR_VERY_DARK_GREY
        AE_COLOR_DARK_GREY
        AE_COLOR_MID_DARK_GREY
        AE_COLOR_GREY
        AE_COLOR_MID_LIGHT_GREY
        AE_COLOR_LIGHT_GREY
        AE_COLOR_VERY_LIGHT_GREY
        AE_COLOR_WHITE
        AE_COLOR_RED
        AE_COLOR_SALMON
        AE_COLOR_LIGHT_SALMON
        AE_COLOR_ORANGE
        AE_COLOR_YELLOW_ORANGE
        AE_COLOR_HOT_PINK
        AE_COLOR_TOY_PINK
        AE_COLOR_GREEN
        AE_COLOR_MINT_GREEN
        AE_COLOR_LIGHT_MINT_GREEN
        AE_COLOR_LIME_GREEN
        AE_COLOR_DEW_YELLOW
        AE_COLOR_CYAN_GREEN
        AE_COLOR_SEA_GREEN
        AE_COLOR_BLUE
        AE_COLOR_VIOLET
        AE_COLOR_LIGHT_VIOLET
        AE_COLOR_PURPLE
        AE_COLOR_TOY_PURPLE
        AE_COLOR_CYAN_BLUE
        AE_COLOR_SKY_BLUE
        AE_COLOR_YELLOW
        AE_COLOR_LEMON_YELLOW
        AE_COLOR_CANDY_YELLOW
        AE_COLOR_MAGENTA
        AE_COLOR_CANDY_PINK
        AE_COLOR_POWDER_PINK
        AE_COLOR_CYAN
        AE_COLOR_ICE_BLUE
        AE_COLOR_BABY_BLUE
        AE_COLOR_COUNT

    const char* ae_color_name[] # full enum name table
    const char* ae_color_string[] # shorter name table

    const float ae_color_flt[][4]
    const u8 ae_color_u8[][4]

    void ae_color_rgb_flt (float* rgb , const char* string)
    void ae_color_rgba_flt(float* rgba, const char* string)

    void ae_color_rgb_u8 (u8* rgb , const char* string)
    void ae_color_rgba_u8(u8* rgba, const char* string)

# TODO: list functions should avoid calling ae_color_rgb(a)_flt, not only for
# efficiency reasons, but also to avoid log spamming if ae_color is stubbed.

def get_rgb_list():
    cdef list values = [] # TODO: optimize this function!
    cdef int i
    cdef bytes b

    for i in range(<int>AE_COLOR_COUNT):
        b = <bytes>ae_color_string[i]

        if sys.version_info.major > 2:
            s = b.decode()

            values.append((s, rgb(s)))
        else:
            values.append((b, rgb(b)))

    return values

def get_rgba_list():
    cdef list values = [] # TODO: optimize this function!
    cdef int i
    cdef bytes b

    for i in range(<int>AE_COLOR_COUNT):
        b = <bytes>ae_color_string[i]

        if sys.version_info.major > 2:
            s = b.decode()

            values.append((s, rgba(s)))
        else:
            values.append((b, rgba(b)))

    return values

def get_rgb_dict():
    return dict(get_rgb_list())

def get_rgba_dict():
    return dict(get_rgba_list())

def rgb(str name):
    """
    Get a floating-point RGB color from the default ae_color table by name.
    """
    cdef Vec3 v = Vec3()
    cdef bytes s

    if sys.version_info.major > 2:
        s = <bytes>name.encode('utf-8')
    else:
        s = <bytes>name

    ae_color_rgb_flt(v.v, <char*>s)
    return v

def rgba(str name):
    """
    Get a floating-point RGBA color from the default ae_color table by name.
    """
    cdef Vec4 v = Vec4()
    cdef bytes s

    if sys.version_info.major > 2:
        s = <bytes>name.encode('utf-8')
    else:
        s = <bytes>name

    ae_color_rgba_flt(v.v, <char*>s)
    return v

rgb_list = get_rgb_list()
rgba_list = get_rgba_list()
rgb_dict = dict(rgb_list)
rgba_dict = dict(rgba_list)
