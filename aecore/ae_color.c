/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

/* TODO: use a statically-allocated hashtable here? would prevent adding new colors */
static ae_strmap_t ae_color_index;

void ae_color_rgb_flt(float* rgb, const char* string)
{
    /* in the event of an invalid color string key, this function will return black */
    vec3copy(rgb, ae_color_flt[(size_t)ae_strmap_get(&ae_color_index, (char*)string)]);
}

void ae_color_rgba_flt(float* rgba, const char* string)
{
    ae_color_rgb_flt(rgba, string); rgba[3] = 1.0f;
}

void ae_color_rgb_u8(u8* rgb, const char* string)
{
    /* in the event of an invalid color string key, this function will return black */
    memcpy(rgb, ae_color_u8[(size_t)ae_strmap_get(&ae_color_index, (char*)string)], 3);
}

void ae_color_rgba_u8(u8* rgba, const char* string)
{
    ae_color_rgb_u8(rgba, string); rgba[3] = 255;
}

void ae_color_init(int argc, char** argv)
{
    size_t i = 0, n = AE_COLOR_COUNT; // add color indices to the name hash table
    ae_strmap_init(&ae_color_index, num_next_pow2(AE_COLOR_COUNT));

    for (; i < n; i++)
    {
        if (!ae_strmap_add(&ae_color_index, (char*)ae_color_string[i], (void*)i))
        {
            ae_error("failed to add color index to hash table (table stubbed?)");
        }
    }
}

void ae_color_quit(void)
{
    ae_strmap_free(&ae_color_index);
}
