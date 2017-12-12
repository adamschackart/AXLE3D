/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_COLOR_H__
#define __AE_COLOR_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif
#ifndef __AE_MEMORY_H__
#include <ae_memory.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_color_init(int argc, char** argv);
void ae_color_quit(void); // private init

/*
================================================================================
 * ~~ [ named colors ] ~~ *
--------------------------------------------------------------------------------
TODO: func to get the hexadecimal string of a color, add alt names to the table?
--------------------------------------------------------------------------------
*/

#define AE_COLOR_N                                                                                      \
                                                                                                        \
    N(BLACK,            0.000, 0.000, 0.000,   0,   0,   0, "black"             ),                      \
    N(VERY_DARK_GREY,   0.125, 0.125, 0.125,  32,  32,  32, "very dark grey"    ),                      \
    N(DARK_GREY,        0.250, 0.250, 0.250,  64,  64,  64, "dark grey"         ),                      \
    N(MID_DARK_GREY,    0.375, 0.375, 0.375,  96,  96,  96, "mid dark grey"     ),                      \
    N(GREY,             0.500, 0.500, 0.500, 128, 128, 128, "grey"              ),                      \
    N(MID_LIGHT_GREY,   0.625, 0.625, 0.625, 160, 160, 160, "mid light grey"    ),                      \
    N(LIGHT_GREY,       0.750, 0.750, 0.750, 192, 192, 192, "light grey"        ),  /* silver */        \
    N(VERY_LIGHT_GREY,  0.875, 0.875, 0.875, 224, 224, 224, "very light grey"   ),                      \
    N(WHITE,            1.000, 1.000, 1.000, 255, 255, 255, "white"             ),                      \
                                                                                                        \
    N(RED,              1.000, 0.000, 0.000, 255,   0,   0, "red"               ),                      \
    N(SALMON,           1.000, 0.500, 0.500, 255, 128, 128, "salmon"            ),                      \
    N(LIGHT_SALMON,     1.000, 0.750, 0.750, 255, 192, 192, "light salmon"      ),                      \
    N(ORANGE,           1.000, 0.500, 0.000, 255, 128,   0, "orange"            ),  /* tangerine */     \
    N(YELLOW_ORANGE,    1.000, 0.750, 0.000, 255, 192,   0, "yellow orange"     ),  /* amber */         \
    N(HOT_PINK,         1.000, 0.000, 0.500, 255,   0, 128, "hot pink"          ),                      \
    N(TOY_PINK,         1.000, 0.000, 0.750, 255,   0, 192, "toy pink"          ),                      \
                                                                                                        \
    N(GREEN,            0.000, 1.000, 0.000,   0, 255,   0, "green"             ),                      \
    N(MINT_GREEN,       0.500, 1.000, 0.500, 128, 255, 128, "mint green"        ),                      \
    N(LIGHT_MINT_GREEN, 0.750, 1.000, 0.750, 192, 255, 192, "light mint green"  ),  /* celadon */       \
    N(LIME_GREEN,       0.500, 1.000, 0.000, 128, 255,   0, "lime green"        ),  /* chartreuse */    \
    N(DEW_YELLOW,       0.750, 1.000, 0.000, 192, 255,   0, "dew yellow"        ),  /* mellow yellow */ \
    N(CYAN_GREEN,       0.000, 1.000, 0.500,   0, 255, 128, "cyan green"        ),  /* spring green */  \
    N(SEA_GREEN,        0.000, 1.000, 0.750,   0, 255, 192, "sea green"         ),  /* aquamarine */    \
                                                                                                        \
    N(BLUE,             0.000, 0.000, 1.000,   0,   0, 255, "blue"              ),                      \
    N(VIOLET,           0.500, 0.500, 1.000, 128, 128, 255, "violet"            ),  /* lavender */      \
    N(LIGHT_VIOLET,     0.750, 0.750, 1.000, 192, 192, 255, "light violet"      ),  /* periwinkle */    \
    N(PURPLE,           0.500, 0.000, 1.000, 128,   0, 255, "purple"            ),  /* grape */         \
    N(TOY_PURPLE,       0.750, 0.000, 1.000, 192,   0, 255, "toy purple"        ),  /* orchid */        \
    N(CYAN_BLUE,        0.000, 0.500, 1.000,   0, 128, 255, "cyan blue"         ),                      \
    N(SKY_BLUE,         0.000, 0.750, 1.000,   0, 192, 255, "sky blue"          ),  /* azure */         \
                                                                                                        \
    N(YELLOW,           1.000, 1.000, 0.000, 255, 255,   0, "yellow"            ),  /* gold */          \
    N(LEMON_YELLOW,     1.000, 1.000, 0.500, 255, 255, 128, "lemon yellow"      ),  /* meringue */      \
    N(CANDY_YELLOW,     1.000, 1.000, 0.750, 255, 255, 192, "candy yellow"      ),  /* chiffon */       \
    N(MAGENTA,          1.000, 0.000, 1.000, 255,   0, 255, "magenta"           ),  /* fuchsia */       \
    N(CANDY_PINK,       1.000, 0.500, 1.000, 255, 128, 255, "candy pink"        ),  /* cyclamen */      \
    N(POWDER_PINK,      1.000, 0.750, 1.000, 255, 192, 255, "powder pink"       ),  /* carnation */     \
    N(CYAN,             0.000, 1.000, 1.000,   0, 255, 255, "cyan"              ),  /* aqua */          \
    N(ICE_BLUE,         0.500, 1.000, 1.000, 128, 255, 255, "ice blue"          ),                      \
    N(BABY_BLUE,        0.750, 1.000, 1.000, 192, 255, 255, "baby blue"         ),                      \

// TODO: choco brown
// TODO: desert tan
// TODO: red orange
// TODO: forest green
// TODO: maroon
// TODO: navy blue

typedef enum ae_color_t
{
    #define N(name, rf, gf, bf, rb, gb, bb, string) AE_COLOR_ ## name
    AE_COLOR_N
    #undef N
    AE_COLOR_COUNT
} \
    ae_color_t;

static const char* ae_color_name[] =
{
    #define N(name, rf, gf, bf, rb, gb, bb, string) AE_STRINGIFY(AE_COLOR_ ## name)
    AE_COLOR_N
    #undef N
};

static const char* ae_color_string[] =
{
    #define N(name, rf, gf, bf, rb, gb, bb, string) string
    AE_COLOR_N
    #undef N
};

static const float ae_color_flt[][4] =
{
    #define N(name, rf, gf, bf, rb, gb, bb, string) { rf ## f, gf ## f, bf ## f, 1.0f }
    AE_COLOR_N
    #undef N
};

static const u8 ae_color_u8[][4] =
{
    #define N(name, rf, gf, bf, rb, gb, bb, string) { rb, gb, bb, 255 }
    AE_COLOR_N
    #undef N
};

AE_DECL void AE_CALL ae_color_rgb_flt (float* rgb , const char* string);
AE_DECL void AE_CALL ae_color_rgba_flt(float* rgba, const char* string);

AE_DECL void AE_CALL ae_color_rgb_u8 (u8* rgb , const char* string);
AE_DECL void AE_CALL ae_color_rgba_u8(u8* rgba, const char* string);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_COLOR_H__ */
