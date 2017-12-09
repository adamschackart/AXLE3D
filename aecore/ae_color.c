/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

static ae_strmap_t ae_color_index; /* TODO: use a statically-allocated table? */

void ae_color_rgb_f(float* rgb, const char* string)
{
    /* in the event of an invalid string key, this function will return black */
    vec3copy(rgb, ae_color_f[(size_t)ae_strmap_get(&ae_color_index, string)]);
}

void ae_color_rgba_f(float* rgba, const char* string)
{
    ae_color_rgb_f(rgba, string); rgba[3] = 1.0f;
}

void ae_color_rgb_b(u8* rgb, const char* string)
{
    memcpy(rgb, ae_color_b[(size_t)ae_strmap_get(&ae_color_index, string)], 3);
}

void ae_color_rgba_b(u8* rgba, const char* string)
{
    ae_color_rgb_b(rgba, string); rgba[3] = 255;
}

void ae_color_init(int argc, char** argv)
{
    size_t i = 0, n = AE_COLOR_COUNT; /* add color indices to hash table */
    ae_strmap_init(&ae_color_index, num_next_pow2(AE_COLOR_COUNT));

    for (; i < n; i++)
    {
        if (!ae_strmap_add(&ae_color_index, ae_color_string[i], (void*)i))
        {
            ae_error("failed to add color index to table (stubbed hash?)");
        }
    }
}

void ae_color_quit(void)
{
    ae_strmap_free(&ae_color_index);
}
