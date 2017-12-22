/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
--- This is a 2D multimedia library inspired by experiments on Gameboy Advance,
--- Casey Muratori's "Handmade Hero", and "Game Coding Complete - 4th Edition".
--- It could easily be used in conjunction with a 3D renderer and audio system.
----------------------------------------------------------------------------- */
#ifndef __XL_CORE_H__
#define __XL_CORE_H__

#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

/*
================================================================================
 * ~~ [ macros & init ] ~~ *
--------------------------------------------------------------------------------
*/

#if defined(XL_COMPILING_EXE)
    #define XL_DECL
    #define XL_CALL
#endif

#if !defined(XL_DECL)
    #if defined(XL_COMPILING_DLL)
        #define XL_DECL AE_EXPORT
    #else
        #define XL_DECL AE_IMPORT
    #endif
#endif

#if !defined(XL_CALL)
    #define XL_CALL AE_CALL
#endif

XL_DECL int XL_CALL xl_is_init(void);

/* NOTE: Initialization is automatically performed in window creation, but you
 * might want to do it earlier for whatever reason. xl_quit is automatically
 * registered with ae_atexit, so there's no reason to invoke shutdown manually.
 */
XL_DECL void XL_CALL xl_init(void);
XL_DECL void XL_CALL xl_quit(void);

XL_DECL int XL_CALL xl_audio_is_init(void);

/* NOTE: Initialization is automatically performed whenever sound is emitted,
 * but you might want to do it earlier for whatever reason. xl_audio_quit is
 * automatically registered with ae_atexit, so there's no reason to shut down
 * manually. Audio initialization and shutdown is performed separately from
 * everything else, in case games/apps want to provide their own audio layer.
 */
XL_DECL void XL_CALL xl_audio_init(void);
XL_DECL void XL_CALL xl_audio_quit(void);

// Get a single-token string identifying the platform and renderer backend.
XL_DECL const char* XL_CALL xl_implementation(void);

// Get a single-token string identifying the audio backend.
XL_DECL const char* XL_CALL xl_audio_implementation(void);

/*
================================================================================
 * ~~ [ object types ] ~~ *
--------------------------------------------------------------------------------
*/

/* All objects are represented by opaque handles to implementation-defined data.
 */
#define XL_OBJECT_TYPE_N        \
                                \
    N(WINDOW, window)           \
    N(CONTROLLER, controller)   \
    N(TEXTURE, texture)         \
    N(FONT, font)               \
    N(SOUND, sound)             \
    N(ANIMATION, animation)     \
    N(KEYBOARD, keyboard)       \
    N(MOUSE, mouse)             \
    N(CLOCK, clock)             \

#if 0
#define N(cap, low) typedef struct xl_internal_ ## low ## _t xl_ ## low ## _t;
#else
#define N(cap, low) typedef void xl_ ## low ## _t;
#endif
XL_OBJECT_TYPE_N
#undef N

typedef enum xl_object_type_t
{
    XL_OBJECT_TYPE_UNKNOWN,

    #define N(cap, low) XL_OBJECT_TYPE_ ## cap,
    XL_OBJECT_TYPE_N
    #undef N

    XL_OBJECT_TYPE_COUNT,
} \
    xl_object_type_t;

static const char* xl_object_type_name[] =
{
    "XL_OBJECT_TYPE_UNKNOWN",

    #define N(cap, low) AE_STRINGIFY(XL_OBJECT_TYPE_ ## cap),
    XL_OBJECT_TYPE_N
    #undef N

    "XL_OBJECT_TYPE_COUNT",
};

static const char* xl_object_type_short_name[] =
{
    "unknown",

    #define N(cap, low) #low,
    XL_OBJECT_TYPE_N
    #undef N

    "count",
};

/* Identify an object. Invalid, closed, or NULL objects are of "unknown" type.
 */
XL_DECL xl_object_type_t XL_CALL xl_object_type(void* object);

/* These functions can be used to manage all objects tracked by this system.
 */
XL_DECL size_t XL_CALL xl_object_count_all(void);
XL_DECL void   XL_CALL xl_object_list_all (void** objects);
XL_DECL void   XL_CALL xl_object_close_all(void);

/*
================================================================================
 * ~~ [ window management ] ~~ *
--------------------------------------------------------------------------------
*/

/* Create new window with default properties. Optionally invisible to allow
 * setting properties on creation without flashing the default window first.
 */
XL_DECL xl_window_t* XL_CALL xl_window_create(int initially_visible);

/* Get the main application window (usually the first one opened), or NULL
 * if no window has been opened yet - this value acts as a closed window.
 */
XL_DECL xl_window_t* XL_CALL xl_primary_window(void);

/* Window attribute access is constrained to a set of common typed functions.
 * Windows are closed by setting the XL_WINDOW_PROPERTY_OPEN attribute to 0.
 */
#define XL_WINDOW_PROPERTY_N                                        \
                                                                    \
    /* the total number of open windows */                          \
    N(XL_WINDOW_PROPERTY_TOTAL, int, int, total)                    \
                                                                    \
    /* for when a pointer isn't a good enough unique identifier */  \
    N(XL_WINDOW_PROPERTY_ID, int, int, id)                          \
                                                                    \
    /* total number of textures active in GL context's table */     \
    N(XL_WINDOW_PROPERTY_TEXTURE_COUNT, int, int, texture_count)    \
                                                                    \
    /* default copy mode for textures created in this window */     \
    N(XL_WINDOW_PROPERTY_COPY_TEXTURES, int, int, copy_textures)    \
                                                                    \
    /* total number of fonts active in GL context table */          \
    N(XL_WINDOW_PROPERTY_FONT_COUNT, int, int, font_count)          \
                                                                    \
    /* the dimensions of the window */                              \
    N(XL_WINDOW_PROPERTY_X, int, int, x)                            \
    N(XL_WINDOW_PROPERTY_Y, int, int, y)                            \
    N(XL_WINDOW_PROPERTY_WIDTH, int, int, width)                    \
    N(XL_WINDOW_PROPERTY_HEIGHT, int, int, height)                  \
                                                                    \
    /* dimensions of containing monitor */                          \
    N(XL_WINDOW_PROPERTY_DISPLAY_X, int, int, display_x)            \
    N(XL_WINDOW_PROPERTY_DISPLAY_Y, int, int, display_y)            \
    N(XL_WINDOW_PROPERTY_DISPLAY_WIDTH, int, int, display_width)    \
    N(XL_WINDOW_PROPERTY_DISPLAY_HEIGHT, int, int, display_height)  \
                                                                    \
    /* renderer dimensions */                                       \
    N(XL_WINDOW_PROPERTY_RENDER_WIDTH, int, int, render_width)      \
    N(XL_WINDOW_PROPERTY_RENDER_HEIGHT, int, int, render_height)    \
                                                                    \
    /* boolean properties */                                        \
    N(XL_WINDOW_PROPERTY_FULLSCREEN, int, int, fullscreen)          \
    N(XL_WINDOW_PROPERTY_BORDERED, int, int, bordered)              \
    N(XL_WINDOW_PROPERTY_VISIBLE, int, int, visible)                \
    N(XL_WINDOW_PROPERTY_RESIZABLE, int, int, resizable)            \
    N(XL_WINDOW_PROPERTY_ACTIVE, int, int, active)                  \
    N(XL_WINDOW_PROPERTY_GRABBED, int, int, grabbed)                \
    N(XL_WINDOW_PROPERTY_OPENGL, int, int, opengl)                  \
    N(XL_WINDOW_PROPERTY_VSYNC, int, int, vsync)                    \
    N(XL_WINDOW_PROPERTY_PRIMARY, int, int, primary)                \
    N(XL_WINDOW_PROPERTY_OPEN, int, int, open)                      \
                                                                    \
    /* miscellaneous properties */                                  \
    N(XL_WINDOW_PROPERTY_STATUS, const char*, str, status)          \
    N(XL_WINDOW_PROPERTY_TITLE, const char*, str, title)            \
    N(XL_WINDOW_PROPERTY_NAME, const char*, str, name)              \
    N(XL_WINDOW_PROPERTY_ICON, ae_image_t*, img, icon)              \
    N(XL_WINDOW_PROPERTY_OPACITY, float, flt, opacity)              \
                                                                    \
    N(XL_WINDOW_PROPERTY_COUNT, int, int, _)                        \

typedef enum xl_window_property_t
{
    #define N(x, t, s, n) x,
    XL_WINDOW_PROPERTY_N
    #undef N
} \
    xl_window_property_t;

static const char* xl_window_property_name[] =
{
    #define N(x, t, s, n) #x,
    XL_WINDOW_PROPERTY_N
    #undef N
};

static const char* xl_window_property_type[] =
{
    #define N(x, t, s, n) #s,
    XL_WINDOW_PROPERTY_N
    #undef N
};

XL_DECL void XL_CALL
xl_window_set_int(xl_window_t* window, xl_window_property_t property, int value);

XL_DECL int XL_CALL
xl_window_get_int(xl_window_t* window, xl_window_property_t property);

XL_DECL void XL_CALL
xl_window_set_flt(xl_window_t* window, xl_window_property_t property, float value);

XL_DECL float XL_CALL
xl_window_get_flt(xl_window_t* window, xl_window_property_t property);

XL_DECL void XL_CALL
xl_window_set_str(xl_window_t* window, xl_window_property_t property, const char* value);

XL_DECL const char* XL_CALL
xl_window_get_str(xl_window_t* window, xl_window_property_t property);

XL_DECL void XL_CALL
xl_window_set_ptr(xl_window_t* window, xl_window_property_t property, void* value);

XL_DECL void* XL_CALL
xl_window_get_ptr(xl_window_t* window, xl_window_property_t property);

XL_DECL void XL_CALL
xl_window_set_img(xl_window_t* window, xl_window_property_t property, ae_image_t* value);

XL_DECL ae_image_t* XL_CALL
xl_window_get_img(xl_window_t* window, xl_window_property_t property);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C code.
 * i.e. xl_window_set_int(w, XL_WINDOW_PROPERTY_WIDTH, v) = xl_window_set_width(w, v).
 */
#define N(x, type, short_type, name)                                                \
                                                                                    \
    static c_inline void xl_window_set_ ## name (xl_window_t* window, type value)   \
    {                                                                               \
        xl_window_set_ ## short_type (window, x, value);                            \
    }                                                                               \
                                                                                    \
    static c_inline type xl_window_get_ ## name (xl_window_t* window)               \
    {                                                                               \
        return xl_window_get_ ## short_type (window, x);                            \
    }                                                                               \

XL_WINDOW_PROPERTY_N
#undef N

static c_inline void xl_window_close(xl_window_t* window)
{
    xl_window_set_open(window, 0);
}

// Clear the whole window to a solid color, ignoring all viewports and clip rectangles.
XL_DECL void XL_CALL xl_window_clear(xl_window_t* window, float r, float g, float b);

// Displays the contents of the backbuffer (the surface we draw into) after rendering.
XL_DECL void XL_CALL xl_window_flip(xl_window_t* window);

// Copies the contents of the window to an image (window sized, not renderer sized).
XL_DECL void XL_CALL xl_window_screenshot(xl_window_t* window, ae_image_t* image);

static c_inline size_t xl_window_count_all(void)
{
    return (size_t)xl_window_get_total(NULL);
}

// Fill array with windows (use the TOTAL window property to get list size).
XL_DECL void XL_CALL xl_window_list_all(xl_window_t** windows);

// Close all open windows. This is automatically called during shutdown.
XL_DECL void XL_CALL xl_window_close_all(void);

/* These three functions manage the window's texture table. Each texture belongs to
 * a GL context (window), which is the only context that can render that texture.
 * Methods to deal with ALL textures are also available (see the texture section).
 */
static c_inline size_t xl_window_count_textures(xl_window_t* window)
{
    return (size_t)xl_window_get_texture_count(window);
}

XL_DECL void XL_CALL
xl_window_list_textures(xl_window_t* window, xl_texture_t** textures);

XL_DECL void XL_CALL xl_window_close_textures(xl_window_t* window);

/* These three functions manage the window's truetype font table. Fonts are held
 * by windows in case atlases are baked into textures for more efficient drawing.
 */
static c_inline size_t xl_window_count_fonts(xl_window_t* window)
{
    return (size_t)xl_window_get_font_count(window);
}

XL_DECL void XL_CALL xl_window_list_fonts(xl_window_t* window, xl_font_t** fonts);
XL_DECL void XL_CALL xl_window_close_fonts(xl_window_t* window);

/*
================================================================================
 * ~~ [ shape renderer ] ~~ *
--------------------------------------------------------------------------------
*/

/* Draw rect with a given color and rotation about a center point, which can be NULL.
 * If outline is true, then a 1-pixel border is drawn instead of a filled rectangle.
 * TODO: this call should eventually take a blend mode (same as texture blend modes).
 */
XL_DECL void XL_CALL xl_draw_rect_ex(xl_window_t* window, float* rect, float* color,
                                    double angle, float* center, const int outline);

/* Draw a filled rect without rotation.
 */
XL_DECL void XL_CALL xl_draw_rect(xl_window_t* window, float* rect, float* color);

/* Draw an array of points. This call should also eventually take a blend mode.
 */
XL_DECL void XL_CALL
xl_draw_points(xl_window_t* window, float* points, size_t count, float* color);

/* Draw a single 1-pixel point.
 */
XL_DECL void XL_CALL
xl_draw_point(xl_window_t* window, float* point, float* color);

/* Draw a single 1-pixel wide line from point A to point B.
 */
XL_DECL void XL_CALL
xl_draw_line(xl_window_t* window, float* a, float* b, float* color);

/* Draw a simple curve with a given mode (CUBIC_INOUT recommended). TODO flip mode?
 * num_divisions is the number of segments in the line (actual division count + 1)
 */
XL_DECL void XL_CALL
xl_draw_curve(xl_window_t* window, float* a, float* b, float* color,
                    ae_ease_mode_t mode, const size_t num_divisions);

/* num_divisions is the number of segments in the circle (lines or triangles).
 */
XL_DECL void XL_CALL
xl_draw_circle(xl_window_t* window, float* center, float radius, float* color,
                                    int outline, const size_t num_divisions);

/* Draw a triangle, either filled or 1-pixel outlined.
 */
XL_DECL void XL_CALL
xl_draw_triangle(xl_window_t* window, float* a, float* b, float* c,
                                        float* color, int outline);

/*
================================================================================
 * ~~ [ texture renderer ] ~~ *
--------------------------------------------------------------------------------
*/

// Create fixed-size image in server (GPU) memory, bound to a rendering context.
XL_DECL xl_texture_t* XL_CALL xl_texture_create(xl_window_t* win, int w, int h);

#define XL_TEXTURE_PROPERTY_N                                   \
                                                                \
    /* the total number of textures across all windows */       \
    N(XL_TEXTURE_PROPERTY_TOTAL, int, int, total)               \
                                                                \
    /* when a pointer isn't a good enough unique identifier */  \
    N(XL_TEXTURE_PROPERTY_ID, int, int, id)                     \
                                                                \
    /* the number of times this texture has been rendered */    \
    N(XL_TEXTURE_PROPERTY_DRAW_CALLS, int, int, draw_calls)     \
                                                                \
    /* parent GL context */                                     \
    N(XL_TEXTURE_PROPERTY_WINDOW, void*, ptr, window)           \
                                                                \
    /* dimensions */                                            \
    N(XL_TEXTURE_PROPERTY_WIDTH, int, int, width)               \
    N(XL_TEXTURE_PROPERTY_HEIGHT, int, int, height)             \
                                                                \
    /* pixel data */                                            \
    N(XL_TEXTURE_PROPERTY_IMAGE, ae_image_t*, img, image)       \
                                                                \
    /* allows xl_texture_get_image by copying uploaded data */  \
    N(XL_TEXTURE_PROPERTY_COPY_ENABLED, int, int, copy_enabled) \
                                                                \
    /* whatever interesting information we want to display */   \
    N(XL_TEXTURE_PROPERTY_STATUS, const char*, str, status)     \
                                                                \
    /* optional loaded file path + name + ext (maybe empty) */  \
    N(XL_TEXTURE_PROPERTY_PATH, const char*, str, path)         \
                                                                \
    /* optional user string identifier (maybe empty) */         \
    N(XL_TEXTURE_PROPERTY_NAME, const char*, str, name)         \
                                                                \
    /* color modulation */                                      \
    N(XL_TEXTURE_PROPERTY_RED, float, flt, red)                 \
    N(XL_TEXTURE_PROPERTY_GREEN, float, flt, green)             \
    N(XL_TEXTURE_PROPERTY_BLUE, float, flt, blue)               \
    N(XL_TEXTURE_PROPERTY_ALPHA, float, flt, alpha)             \
    N(XL_TEXTURE_PROPERTY_RGB, void*, ptr, rgb)                 \
    N(XL_TEXTURE_PROPERTY_RGBA, void*, ptr, rgba)               \
                                                                \
    /* flip mask (horizontal, vertical, or both) */             \
    N(XL_TEXTURE_PROPERTY_FLIP, int, int, flip)                 \
                                                                \
    /* texture is valid */                                      \
    N(XL_TEXTURE_PROPERTY_OPEN, int, int, open)                 \
                                                                \
    /* the number of texture properties */                      \
    N(XL_TEXTURE_PROPERTY_COUNT, int, int, _)                   \

typedef enum xl_texture_property_t
{
    #define N(x, t, s, n) x,
    XL_TEXTURE_PROPERTY_N
    #undef N
} \
    xl_texture_property_t;

static const char* xl_texture_property_name[] =
{
    #define N(x, t, s, n) #x,
    XL_TEXTURE_PROPERTY_N
    #undef N
};

static const char* xl_texture_property_type[] =
{
    #define N(x, t, s, n) #s,
    XL_TEXTURE_PROPERTY_N
    #undef N
};

XL_DECL void XL_CALL
xl_texture_set_int(xl_texture_t* texture, xl_texture_property_t property, int value);

XL_DECL int XL_CALL
xl_texture_get_int(xl_texture_t* texture, xl_texture_property_t property);

XL_DECL void XL_CALL
xl_texture_set_flt(xl_texture_t* texture, xl_texture_property_t property, float value);

XL_DECL float XL_CALL
xl_texture_get_flt(xl_texture_t* texture, xl_texture_property_t property);

XL_DECL void XL_CALL
xl_texture_set_str(xl_texture_t* texture, xl_texture_property_t property, const char* value);

XL_DECL const char* XL_CALL
xl_texture_get_str(xl_texture_t* texture, xl_texture_property_t property);

XL_DECL void XL_CALL
xl_texture_set_ptr(xl_texture_t* texture, xl_texture_property_t property, void* value);

XL_DECL void* XL_CALL
xl_texture_get_ptr(xl_texture_t* texture, xl_texture_property_t property);

XL_DECL void XL_CALL
xl_texture_set_img(xl_texture_t* texture, xl_texture_property_t property, ae_image_t* value);

XL_DECL ae_image_t* XL_CALL
xl_texture_get_img(xl_texture_t* texture, xl_texture_property_t property);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C code.
 * i.e. xl_texture_set_int(t, XL_TEXTURE_PROPERTY_OPEN, v) = xl_texture_set_open(t, v).
 */
#define N(x, type, short_type, name)                                                    \
                                                                                        \
    static c_inline void xl_texture_set_ ## name (xl_texture_t* texture, type value)    \
    {                                                                                   \
        xl_texture_set_ ## short_type (texture, x, value);                              \
    }                                                                                   \
                                                                                        \
    static c_inline type xl_texture_get_ ## name (xl_texture_t* texture)                \
    {                                                                                   \
        return xl_texture_get_ ## short_type (texture, x);                              \
    }                                                                                   \

XL_TEXTURE_PROPERTY_N
#undef N

static c_inline void xl_texture_close(xl_texture_t* texture)
{
    xl_texture_set_open(texture, 0);
}

/* NOTE: since this really only has two values, it is also usable as a bitmask.
 * XL_TEXTURE_FLIP_BOTH = XL_TEXTURE_FLIP_HORIZONTAL|XL_TEXTURE_FLIP_VERTICAL.
 */
#define XL_TEXTURE_FLIP_N                       \
                                                \
    N(XL_TEXTURE_FLIP_NONE, none)               \
    N(XL_TEXTURE_FLIP_HORIZONTAL, horizontal)   \
    N(XL_TEXTURE_FLIP_VERTICAL, vertical)       \
    N(XL_TEXTURE_FLIP_BOTH, both)               \
                                                \
    N(XL_TEXTURE_FLIP_COUNT, _)                 \

typedef enum xl_texture_flip_t
{
    #define N(x, s) x,
    XL_TEXTURE_FLIP_N
    #undef N
} \
    xl_texture_flip_t;

static const char* xl_texture_flip_name[] =
{
    #define N(x, s) #x,
    XL_TEXTURE_FLIP_N
    #undef N
};

static const char* xl_texture_flip_short_name[] =
{
    #define N(x, s) #s,
    XL_TEXTURE_FLIP_N
    #undef N
};

XL_DECL xl_texture_flip_t XL_CALL // flip mode from string
        xl_texture_flip_from_short_name(const char* name);

/* Draw a clipped, scaled, rotated texture. src_rect is the region of the texture
 * to render (or NULL for the entire texture). dst_rect is the area of the window
 * to draw into (NULL stretches the texture across the entire rendering target).
 *
 * Center is the point around which the destination rectangle will be rotated by
 * the given angle (in radians). NULL is the default (center of the dest rect).
 */
XL_DECL void XL_CALL xl_texture_draw_ex(xl_texture_t* texture, float* src_rect,
                                float* dst_rect, double angle, float* center);

// Draw texture at (x, y), where the origin is the bottom left (GL-style coords).
XL_DECL void XL_CALL xl_texture_draw(xl_texture_t* texture, const float xy[2]);

/* Load a compressed image file from memory; calls ae_error on failure to load.
 */
XL_DECL xl_texture_t* XL_CALL xl_texture_load_from_memory(xl_window_t* window,
                                                void* buffer, size_t length);

/* Load a compressed image file from a memory buffer, allowing for user errors.
 */
XL_DECL xl_texture_t* XL_CALL xl_texture_load_from_memory_ex(xl_window_t* window,
                    void* buffer, size_t length, ae_image_error_t* error_status);

/* Load an image file into a texture. This is a quick and dirty function that
 * terminates the application with an error message if the file fails to load.
 * The texture path will be set to filename, and the texture name will be set
 * to the filename with all path and extension characters stripped out of it.
 */
XL_DECL xl_texture_t* XL_CALL xl_texture_load(xl_window_t* window,
                                            const char* filename);

/* Load an image file into a texture, allowing for error handling by the user.
 */
XL_DECL xl_texture_t* XL_CALL xl_texture_load_ex(xl_window_t* window,
                const char* filename, ae_image_error_t* error_status);

static c_inline size_t xl_texture_count_all(void)
{
    return (size_t)xl_texture_get_total(NULL);
}

XL_DECL void XL_CALL xl_texture_list_all(xl_texture_t** textures);
XL_DECL void XL_CALL xl_texture_close_all(void);

/*
================================================================================
 * ~~ [ font renderer ] ~~ *
--------------------------------------------------------------------------------
*/

#define XL_FONT_PROPERTY_N                                  \
                                                            \
    /* the total number of fonts across all windows */      \
    N(XL_FONT_PROPERTY_TOTAL, int, int, total)              \
                                                            \
    /* unique id. no guarantees are made as to whether ints \
     * are sequential, random, or shared between XL types.  \
     */                                                     \
    N(XL_FONT_PROPERTY_ID, int, int, id)                    \
                                                            \
    /* parent GL context */                                 \
    N(XL_FONT_PROPERTY_WINDOW, void*, ptr, window)          \
                                                            \
    /* arbitrary measure of the font's size */              \
    N(XL_FONT_PROPERTY_POINT_SIZE, int, int, point_size)    \
                                                            \
    /* recommended spacing between lines for this font */   \
    N(XL_FONT_PROPERTY_LINE_SKIP, int, int, line_skip)      \
                                                            \
    /* whatever interesting information we want to show */  \
    N(XL_FONT_PROPERTY_STATUS, const char*, str, status)    \
                                                            \
    /* optional loaded file path + name + ext */            \
    N(XL_FONT_PROPERTY_PATH, const char*, str, path)        \
                                                            \
    /* optional user string identifier (maybe empty) */     \
    N(XL_FONT_PROPERTY_NAME, const char*, str, name)        \
                                                            \
    /* color modulation */                                  \
    N(XL_FONT_PROPERTY_RED, float, flt, red)                \
    N(XL_FONT_PROPERTY_GREEN, float, flt, green)            \
    N(XL_FONT_PROPERTY_BLUE, float, flt, blue)              \
    N(XL_FONT_PROPERTY_ALPHA, float, flt, alpha)            \
    N(XL_FONT_PROPERTY_RGB, void*, ptr, rgb)                \
    N(XL_FONT_PROPERTY_RGBA, void*, ptr, rgba)              \
                                                            \
    /* font is valid */                                     \
    N(XL_FONT_PROPERTY_OPEN, int, int, open)                \
                                                            \
    /* the number of font properties */                     \
    N(XL_FONT_PROPERTY_COUNT, int, int, _)                  \

typedef enum xl_font_property_t
{
    #define N(x, t, s, n) x,
    XL_FONT_PROPERTY_N
    #undef N
} \
    xl_font_property_t;

static const char* xl_font_property_name[] =
{
    #define N(x, t, s, n) #x,
    XL_FONT_PROPERTY_N
    #undef N
};

static const char* xl_font_property_type[] =
{
    #define N(x, t, s, n) #s,
    XL_FONT_PROPERTY_N
    #undef N
};

XL_DECL void XL_CALL
xl_font_set_int(xl_font_t* font, xl_font_property_t property, int value);

XL_DECL int XL_CALL
xl_font_get_int(xl_font_t* font, xl_font_property_t property);

XL_DECL void XL_CALL
xl_font_set_flt(xl_font_t* font, xl_font_property_t property, float value);

XL_DECL float XL_CALL
xl_font_get_flt(xl_font_t* font, xl_font_property_t property);

XL_DECL void XL_CALL
xl_font_set_str(xl_font_t* font, xl_font_property_t property, const char* value);

XL_DECL const char* XL_CALL
xl_font_get_str(xl_font_t* font, xl_font_property_t property);

XL_DECL void XL_CALL
xl_font_set_ptr(xl_font_t* font, xl_font_property_t property, void* value);

XL_DECL void* XL_CALL
xl_font_get_ptr(xl_font_t* font, xl_font_property_t property);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C code.
 */
#define N(x, type, short_type, name)                                        \
                                                                            \
    static c_inline void xl_font_set_ ## name (xl_font_t* font, type value) \
    {                                                                       \
        xl_font_set_ ## short_type (font, x, value);                        \
    }                                                                       \
                                                                            \
    static c_inline type xl_font_get_ ## name (xl_font_t* font)             \
    {                                                                       \
        return xl_font_get_ ## short_type (font, x);                        \
    }                                                                       \

XL_FONT_PROPERTY_N
#undef N

static c_inline void xl_font_close(xl_font_t* font)
{
    xl_font_set_open(font, 0);
}

XL_DECL void XL_CALL xl_font_text_size(xl_font_t * font, int * w, int * h,
                            const char* format, ...) AE_PRINTF_ARGS(3, 4);

XL_DECL void XL_CALL xl_font_render_image(xl_font_t * font, ae_image_t* image,
                                const char* format, ...) AE_PRINTF_ARGS(2, 3);

XL_DECL xl_texture_t* XL_CALL xl_font_render_texture(xl_font_t * font,
                        const char* format, ...) AE_PRINTF_ARGS(1, 2);

XL_DECL void XL_CALL xl_font_blit(xl_font_t* font, ae_image_t* image, int x, int y,
        int r, int g, int b, int a, const char* format, ...) AE_PRINTF_ARGS(8, 9);

XL_DECL void XL_CALL xl_font_draw(xl_font_t* font, float xy[2],
                const char* format, ...) AE_PRINTF_ARGS(2, 3);

XL_DECL xl_font_t* XL_CALL xl_font_load_from_memory( xl_window_t * window,
                                void* ptr, size_t length, int point_size);

XL_DECL xl_font_t* XL_CALL xl_font_load( xl_window_t * window,
                        const char* filename, int point_size);

XL_DECL xl_font_t* XL_CALL xl_font_load_system_monospace(xl_window_t* window,
                                                            int point_size);

static c_inline size_t xl_font_count_all(void)
{
    return (size_t)xl_font_get_total(NULL);
}

XL_DECL void XL_CALL xl_font_list_all(xl_font_t** fonts);
XL_DECL void XL_CALL xl_font_close_all(void);

/*
================================================================================
 * ~~ [ streaming music ] ~~ *
--------------------------------------------------------------------------------
*/

#define XL_MUSIC_PROPERTY_N                                 \
                                                            \
    /* music is valid (paused is still playing) */          \
    N(XL_MUSIC_PROPERTY_PLAYING, int, int, playing)         \
                                                            \
    /* playback is temporarily suspended */                 \
    N(XL_MUSIC_PROPERTY_PAUSED, int, int, paused)           \
                                                            \
    /* fade status */                                       \
    N(XL_MUSIC_PROPERTY_FADING_IN, int, int, fading_in)     \
    N(XL_MUSIC_PROPERTY_FADING_OUT, int, int, fading_out)   \
                                                            \
    /* playing, stopped, paused, fading in or out */        \
    N(XL_MUSIC_PROPERTY_STATUS, const char*, str, status)   \
                                                            \
    /* audio length in seconds */                           \
    N(XL_MUSIC_PROPERTY_DURATION, double, dbl, duration)    \
                                                            \
    /* seconds elapsed since the beginning of the song */   \
    N(XL_MUSIC_PROPERTY_POSITION, double, dbl, position)    \
                                                            \
    /* music volume in [0.0, 1.0] */                        \
    N(XL_MUSIC_PROPERTY_VOLUME, double, dbl, volume)        \
                                                            \
    /* file path and custom user string identifier */       \
    N(XL_MUSIC_PROPERTY_PATH, const char*, str, path)       \
    N(XL_MUSIC_PROPERTY_NAME, const char*, str, name)       \
                                                            \
    /* the number of music properties */                    \
    N(XL_MUSIC_PROPERTY_COUNT, int, int, _)                 \

typedef enum xl_music_property_t
{
    #define N(x, t, s, n) x,
    XL_MUSIC_PROPERTY_N
    #undef N
} \
    xl_music_property_t;

static const char* xl_music_property_name[] =
{
    #define N(x, t, s, n) #x,
    XL_MUSIC_PROPERTY_N
    #undef N
};

static const char* xl_music_property_type[] =
{
    #define N(x, t, s, n) #s,
    XL_MUSIC_PROPERTY_N
    #undef N
};

XL_DECL void XL_CALL
xl_music_set_int(xl_music_property_t property, int value);

XL_DECL int XL_CALL
xl_music_get_int(xl_music_property_t property);

XL_DECL void XL_CALL
xl_music_set_dbl(xl_music_property_t property, double value);

XL_DECL double XL_CALL
xl_music_get_dbl(xl_music_property_t property);

XL_DECL void XL_CALL
xl_music_set_str(xl_music_property_t property, const char* value);

XL_DECL const char* XL_CALL
xl_music_get_str(xl_music_property_t property);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C code.
 */
#define N(x, type, short_type, name)                        \
                                                            \
    static c_inline void xl_music_set_ ## name (type value) \
    {                                                       \
        xl_music_set_ ## short_type (x, value);             \
    }                                                       \
                                                            \
    static c_inline type xl_music_get_ ## name (void)       \
    {                                                       \
        return xl_music_get_ ## short_type (x);             \
    }                                                       \

XL_MUSIC_PROPERTY_N
#undef N

XL_DECL void XL_CALL xl_music_fade_in(const char * filename,
                int loop, double fade_in, double start_pos);

XL_DECL void XL_CALL xl_music_fade_out(double fade_out);

XL_DECL void XL_CALL xl_music_play(const char* filename);
XL_DECL void XL_CALL xl_music_stop(void);

/*
================================================================================
 * ~~ [ sound effects ] ~~ *
--------------------------------------------------------------------------------
*/

#define XL_SOUND_PROPERTY_N                                 \
                                                            \
    /* the total number of open sounds */                   \
    N(XL_SOUND_PROPERTY_TOTAL, int, int, total)             \
                                                            \
    /* unique integer identifier */                         \
    N(XL_SOUND_PROPERTY_ID, int, int, id)                   \
                                                            \
    /* audio length in seconds */                           \
    N(XL_SOUND_PROPERTY_DURATION, double, dbl, duration)    \
                                                            \
    /* sound volume in [0.0, 1.0] */                        \
    N(XL_SOUND_PROPERTY_VOLUME, double, dbl, volume)        \
                                                            \
    /* whatever info we want to show about this sound */    \
    N(XL_SOUND_PROPERTY_STATUS, const char*, str, status)   \
                                                            \
    /* file path and custom user string identifier */       \
    N(XL_SOUND_PROPERTY_PATH, const char*, str, path)       \
    N(XL_SOUND_PROPERTY_NAME, const char*, str, name)       \
                                                            \
    /* sound is valid */                                    \
    N(XL_SOUND_PROPERTY_OPEN, int, int, open)               \
                                                            \
    /* the number of sound properties */                    \
    N(XL_SOUND_PROPERTY_COUNT, int, int, _)                 \

typedef enum xl_sound_property_t
{
    #define N(x, t, s, n) x,
    XL_SOUND_PROPERTY_N
    #undef N
} \
    xl_sound_property_t;

static const char* xl_sound_property_name[] =
{
    #define N(x, t, s, n) #x,
    XL_SOUND_PROPERTY_N
    #undef N
};

static const char* xl_sound_property_type[] =
{
    #define N(x, t, s, n) #s,
    XL_SOUND_PROPERTY_N
    #undef N
};

XL_DECL void XL_CALL
xl_sound_set_int(xl_sound_t* sound, xl_sound_property_t property, int value);

XL_DECL int XL_CALL
xl_sound_get_int(xl_sound_t* sound, xl_sound_property_t property);

XL_DECL void XL_CALL
xl_sound_set_dbl(xl_sound_t* sound, xl_sound_property_t property, double value);

XL_DECL double XL_CALL
xl_sound_get_dbl(xl_sound_t* sound, xl_sound_property_t property);

XL_DECL void XL_CALL
xl_sound_set_str(xl_sound_t* sound, xl_sound_property_t property, const char* value);

XL_DECL const char* XL_CALL
xl_sound_get_str(xl_sound_t* sound, xl_sound_property_t property);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C code.
 */
#define N(x, type, short_type, name)                                            \
                                                                                \
    static c_inline void xl_sound_set_ ## name (xl_sound_t* sound, type value)  \
    {                                                                           \
        xl_sound_set_ ## short_type (sound, x, value);                          \
    }                                                                           \
                                                                                \
    static c_inline type xl_sound_get_ ## name (xl_sound_t* sound)              \
    {                                                                           \
        return xl_sound_get_ ## short_type (sound, x);                          \
    }                                                                           \

XL_SOUND_PROPERTY_N
#undef N

static c_inline void xl_sound_close(xl_sound_t* sound)
{
    xl_sound_set_open(sound, 0);
}

XL_DECL void XL_CALL // play N times, with an optional fade in effect and cutoff.
xl_sound_fade_in(xl_sound_t* sound, int count, double fade_in, double length);

XL_DECL void XL_CALL xl_sound_play(xl_sound_t* sound);

/* Stop all playing instances of a sound, or all sounds if NULL is passed.
 */
XL_DECL void XL_CALL xl_sound_fade_out(xl_sound_t* sound, double fade_out);
XL_DECL void XL_CALL xl_sound_stop(xl_sound_t* sound);

XL_DECL xl_sound_t* XL_CALL xl_sound_load_from_memory(void* ptr, size_t length);
XL_DECL xl_sound_t* XL_CALL xl_sound_load(const char* filename);

static c_inline size_t xl_sound_count_all(void)
{
    return (size_t)xl_sound_get_total(NULL);
}

XL_DECL void XL_CALL xl_sound_list_all(xl_sound_t** sounds);
XL_DECL void XL_CALL xl_sound_close_all(void);

/*
================================================================================
 * ~~ [ keyboard input ] ~~ *
--------------------------------------------------------------------------------
*/

/* Get the main application/system keyboard (usually the first one plugged in).
 * If nothing is present, this returns NULL (which acts as a closed keyboard).
 */
XL_DECL xl_keyboard_t* XL_CALL xl_primary_keyboard(void);

#define XL_KEYBOARD_PROPERTY_N                                                  \
                                                                                \
    /* the total number of active keyboards */                                  \
    N(XL_KEYBOARD_PROPERTY_TOTAL, int, int, total)                              \
                                                                                \
    /* the keyboard's unique integer identifier */                              \
    N(XL_KEYBOARD_PROPERTY_ID, int, int, id)                                    \
                                                                                \
    /* the current state of the keyboard mods and keys */                       \
    N(XL_KEYBOARD_PROPERTY_DOWN_MODS, int, int, down_mods)                      \
    N(XL_KEYBOARD_PROPERTY_UP_MODS, int, int, up_mods)                          \
    N(XL_KEYBOARD_PROPERTY_DOWN_KEYS, void*, ptr, down_keys)                    \
    N(XL_KEYBOARD_PROPERTY_UP_KEYS, void*, ptr, up_keys)                        \
                                                                                \
    /* the last key pressed or released on a given keyboard */                  \
    N(XL_KEYBOARD_PROPERTY_LAST_PRESSED_KEY, int, int, last_pressed_key)        \
    N(XL_KEYBOARD_PROPERTY_LAST_RELEASED_KEY, int, int, last_released_key)      \
                                                                                \
    /* the last time a supported key was pressed or released on the keyboard */ \
    N(XL_KEYBOARD_PROPERTY_LAST_PRESSED_TIME, double, dbl, last_pressed_time)   \
    N(XL_KEYBOARD_PROPERTY_LAST_RELEASED_TIME, double, dbl, last_released_time) \
                                                                                \
    /* the current state of the keyboard as a string */                         \
    N(XL_KEYBOARD_PROPERTY_STATUS, const char*, str, status)                    \
                                                                                \
    /* the keyboard's string identifier */                                      \
    N(XL_KEYBOARD_PROPERTY_NAME, const char*, str, name)                        \
                                                                                \
    /* this keyboard is the first one plugged in */                             \
    N(XL_KEYBOARD_PROPERTY_PRIMARY, int, int, primary)                          \
                                                                                \
    /* whether or not the keyboard is plugged in */                             \
    N(XL_KEYBOARD_PROPERTY_OPEN, int, int, open)                                \
                                                                                \
    /* the total number of keyboard properties */                               \
    N(XL_KEYBOARD_PROPERTY_COUNT, int, int, _)                                  \

typedef enum xl_keyboard_property_t
{
    #define N(x, t, s, n) x,
    XL_KEYBOARD_PROPERTY_N
    #undef N
} \
    xl_keyboard_property_t;

static const char* xl_keyboard_property_name[] =
{
    #define N(x, t, s, n) #x,
    XL_KEYBOARD_PROPERTY_N
    #undef N
};

static const char* xl_keyboard_property_type[] =
{
    #define N(x, t, s, n) #s,
    XL_KEYBOARD_PROPERTY_N
    #undef N
};

XL_DECL void XL_CALL
xl_keyboard_set_int(xl_keyboard_t* kbd, xl_keyboard_property_t prop, int value);

XL_DECL int XL_CALL
xl_keyboard_get_int(xl_keyboard_t* kbd, xl_keyboard_property_t prop);

XL_DECL void XL_CALL
xl_keyboard_set_dbl(xl_keyboard_t* kbd, xl_keyboard_property_t prop, double value);

XL_DECL double XL_CALL
xl_keyboard_get_dbl(xl_keyboard_t* kbd, xl_keyboard_property_t prop);

XL_DECL void XL_CALL
xl_keyboard_set_str(xl_keyboard_t* kbd, xl_keyboard_property_t prop, const char* value);

XL_DECL const char* XL_CALL
xl_keyboard_get_str(xl_keyboard_t* kbd, xl_keyboard_property_t prop);

XL_DECL void XL_CALL
xl_keyboard_set_ptr(xl_keyboard_t* kbd, xl_keyboard_property_t prop, void* value);

XL_DECL void* XL_CALL
xl_keyboard_get_ptr(xl_keyboard_t* kbd, xl_keyboard_property_t prop);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C code.
 * xl_keyboard_get_str(kbd, XL_KEYBOARD_PROPERTY_STATUS) = xl_keyboard_get_status(kbd).
 */
#define N(x, type, short_type, name)                                            \
                                                                                \
    static c_inline void xl_keyboard_set_ ## name (xl_keyboard_t* c, type val)  \
    {                                                                           \
        xl_keyboard_set_ ## short_type (c, x, val);                             \
    }                                                                           \
                                                                                \
    static c_inline type xl_keyboard_get_ ## name (xl_keyboard_t* keyboard)     \
    {                                                                           \
        return xl_keyboard_get_ ## short_type (keyboard, x);                    \
    }                                                                           \

XL_KEYBOARD_PROPERTY_N
#undef N

static c_inline size_t xl_keyboard_count_all(void)
{
    return (size_t)xl_keyboard_get_total(NULL);
}

XL_DECL void XL_CALL xl_keyboard_list_all(xl_keyboard_t** keyboards);

/* ===== [ modifiers and keys ] ============================================= */

#define XL_KEYBOARD_MOD_N                   \
                                            \
    N(LEFT_SHIFT,       left_shift,     0 ) \
    N(RIGHT_SHIFT,      right_shift,    1 ) \
    N(LEFT_CONTROL,     left_control,   2 ) \
    N(RIGHT_CONTROL,    right_control,  3 ) \
    N(LEFT_ALT,         left_alt,       4 ) \
    N(RIGHT_ALT,        right_alt,      5 ) \
    N(LEFT_GUI,         left_gui,       6 ) \
    N(RIGHT_GUI,        right_gui,      7 ) \
    N(NUMLOCK,          numlock,        8 ) \
    N(CAPSLOCK,         capslock,       9 ) \
    N(COUNT,            _,              10) \

typedef enum xl_keyboard_mod_index_t
{
    #define N(cap, low, val) XL_KEYBOARD_MOD_INDEX_ ## cap,
    XL_KEYBOARD_MOD_N
    #undef N
} \
    xl_keyboard_mod_index_t;

static const char* xl_keyboard_mod_index_name[] =
{
    #define N(cap, low, val) AE_STRINGIFY(XL_KEYBOARD_MOD_INDEX_ ## cap),
    XL_KEYBOARD_MOD_N
    #undef N
};

static const char* xl_keyboard_mod_short_name[] =
{
    #define N(cap, low, val) #low,
    XL_KEYBOARD_MOD_N
    #undef N
};

typedef enum xl_keyboard_mod_bit_t
{
    #define N(cap, low, val) XL_KEYBOARD_MOD_BIT_ ## cap = AE_IDX2BIT(val),
    XL_KEYBOARD_MOD_N
    #undef N
} \
    xl_keyboard_mod_bit_t;

#define XL_KEYBOARD_MOD_BIT_NONE 0 /* no modifier key is tested for */

#define XL_KEYBOARD_MOD_BIT_SHIFT /* test for either shift key */ \
    (XL_KEYBOARD_MOD_BIT_LEFT_SHIFT | XL_KEYBOARD_MOD_BIT_RIGHT_SHIFT)

#define XL_KEYBOARD_MOD_BIT_CONTROL /* test for either control key */ \
    (XL_KEYBOARD_MOD_BIT_LEFT_CONTROL | XL_KEYBOARD_MOD_BIT_RIGHT_CONTROL)

#define XL_KEYBOARD_MOD_BIT_ALT /* test for either alt key */ \
    (XL_KEYBOARD_MOD_BIT_LEFT_ALT | XL_KEYBOARD_MOD_BIT_RIGHT_ALT)

#define XL_KEYBOARD_MOD_BIT_GUI /* test for either gui key */ \
    (XL_KEYBOARD_MOD_BIT_LEFT_GUI | XL_KEYBOARD_MOD_BIT_RIGHT_GUI)

XL_DECL xl_keyboard_mod_index_t XL_CALL // get modifier from name
        xl_keyboard_mod_index_from_short_name(const char * name);

static c_inline xl_keyboard_mod_bit_t // get modifier bitmask from name
                xl_keyboard_mod_bit_from_short_name(const char * name)
{
    /* TODO: move this call to the implementation file and export it,
     * on account of these standard lib calls that might be replaced.
     */
    if (!strcmp(name, "shift"))
    {
        return (xl_keyboard_mod_bit_t) XL_KEYBOARD_MOD_BIT_SHIFT;
    }

    if (!strcmp(name, "control"))
    {
        return (xl_keyboard_mod_bit_t) XL_KEYBOARD_MOD_BIT_CONTROL;
    }

    if (!strcmp(name, "alt"))
    {
        return (xl_keyboard_mod_bit_t) XL_KEYBOARD_MOD_BIT_ALT;
    }

    if (!strcmp(name, "gui"))
    {
        return (xl_keyboard_mod_bit_t) XL_KEYBOARD_MOD_BIT_GUI;
    }

    return ((xl_keyboard_mod_bit_t)
            AE_IDX2BIT(xl_keyboard_mod_index_from_short_name(name)));
}

static c_inline int
xl_keyboard_mod_is_down(xl_keyboard_t* k, xl_keyboard_mod_index_t i)
{
    return (xl_keyboard_get_down_mods(k) & AE_IDX2BIT(i)) != 0;
}

static c_inline int
xl_keyboard_mod_is_up(xl_keyboard_t* k, xl_keyboard_mod_index_t i)
{
    return !xl_keyboard_mod_is_down(k, i);
}

#define XL_KEYBOARD_KEY_N               \
                                        \
    /* unsupported key (browser etc) */ \
    N(UNKNOWN,          unknown)        \
                                        \
    N(A,                a)              \
    N(B,                b)              \
    N(C,                c)              \
    N(D,                d)              \
    N(E,                e)              \
    N(F,                f)              \
    N(G,                g)              \
    N(H,                h)              \
    N(I,                i)              \
    N(J,                j)              \
    N(K,                k)              \
    N(L,                l)              \
    N(M,                m)              \
    N(N,                n)              \
    N(O,                o)              \
    N(P,                p)              \
    N(Q,                q)              \
    N(R,                r)              \
    N(S,                s)              \
    N(T,                t)              \
    N(U,                u)              \
    N(V,                v)              \
    N(W,                w)              \
    N(X,                x)              \
    N(Y,                y)              \
    N(Z,                z)              \
                                        \
    N(1,                1)              \
    N(2,                2)              \
    N(3,                3)              \
    N(4,                4)              \
    N(5,                5)              \
    N(6,                6)              \
    N(7,                7)              \
    N(8,                8)              \
    N(9,                9)              \
    N(0,                0)              \
                                        \
    N(RETURN,           return)         \
    N(ESCAPE,           escape)         \
    N(BACKSPACE,        backspace)      \
    N(TAB,              tab)            \
    N(SPACE,            space)          \
    N(MINUS,            minus)          \
    N(EQUALS,           equals)         \
    N(LEFT_BRACKET,     left_bracket)   \
    N(RIGHT_BRACKET,    right_bracket)  \
    N(BACKSLASH,        backslash)      \
    N(SEMICOLON,        semicolon)      \
    N(APOSTROPHE,       apostrophe)     \
    N(GRAVE,            grave)          \
    N(COMMA,            comma)          \
    N(PERIOD,           period)         \
    N(SLASH,            slash)          \
                                        \
    N(F1,               f1)             \
    N(F2,               f2)             \
    N(F3,               f3)             \
    N(F4,               f4)             \
    N(F5,               f5)             \
    N(F6,               f6)             \
    N(F7,               f7)             \
    N(F8,               f8)             \
    N(F9,               f9)             \
    N(F10,              f10)            \
    N(F11,              f11)            \
    N(F12,              f12)            \
                                        \
    N(PRINT_SCREEN,     print_screen)   \
    N(SCROLL_LOCK,      scroll_lock)    \
    N(PAUSE,            pause)          \
                                        \
    /* `help` on some mac keyboards */  \
    N(INSERT,           insert)         \
    N(DELETE,           delete)         \
    N(HOME,             home)           \
    N(PAGE_UP,          page_up)        \
    N(PAGE_DOWN,        page_down)      \
    N(END,              end)            \
                                        \
    N(RIGHT,            right)          \
    N(LEFT,             left)           \
    N(DOWN,             down)           \
    N(UP,               up)             \
                                        \
    N(KP_DIVIDE,        kp_divide)      \
    N(KP_MULTIPLY,      kp_multiply)    \
    N(KP_MINUS,         kp_minus)       \
    N(KP_PLUS,          kp_plus)        \
    N(KP_ENTER,         kp_enter)       \
    N(KP_PERIOD,        kp_period)      \
                                        \
    N(KP_1,             kp_1)           \
    N(KP_2,             kp_2)           \
    N(KP_3,             kp_3)           \
    N(KP_4,             kp_4)           \
    N(KP_5,             kp_5)           \
    N(KP_6,             kp_6)           \
    N(KP_7,             kp_7)           \
    N(KP_8,             kp_8)           \
    N(KP_9,             kp_9)           \
    N(KP_0,             kp_0)           \
                                        \
    N(LEFT_SHIFT,       left_shift)     \
    N(RIGHT_SHIFT,      right_shift)    \
    N(LEFT_CONTROL,     left_control)   \
    N(RIGHT_CONTROL,    right_control)  \
    N(LEFT_ALT,         left_alt)       \
    N(RIGHT_ALT,        right_alt)      \
    N(LEFT_GUI,         left_gui)       \
    N(RIGHT_GUI,        right_gui)      \
                                        \
    /* `clear` on some mac keyboards */ \
    N(NUMLOCK,          numlock)        \
    N(CAPSLOCK,         capslock)       \
                                        \
    N(COUNT,            _)              \

typedef enum xl_keyboard_key_index_t
{
    #define N(cap, low) XL_KEYBOARD_KEY_INDEX_ ## cap,
    XL_KEYBOARD_KEY_N
    #undef N
} \
    xl_keyboard_key_index_t;

static const char* xl_keyboard_key_index_name[] =
{
    #define N(cap, low) AE_STRINGIFY(XL_KEYBOARD_KEY_INDEX_ ## cap),
    XL_KEYBOARD_KEY_N
    #undef N
};

static const char* xl_keyboard_key_short_name[] =
{
    #define N(cap, low) #low,
    XL_KEYBOARD_KEY_N
    #undef N
};

typedef u8 // we can't fit all keys into a 32-bit int, so we use a key bit vector
xl_keyboard_key_bit_t[(XL_KEYBOARD_KEY_INDEX_COUNT + (CHAR_BIT - 1)) / CHAR_BIT];

XL_DECL xl_keyboard_key_index_t XL_CALL // get keycode from name
        xl_keyboard_key_index_from_short_name(const char* name);

static c_inline int
xl_keyboard_key_is_down(xl_keyboard_t* k, xl_keyboard_key_index_t i)
{
    return ae_bitvector_get((u8*)xl_keyboard_get_down_keys(k), i);
}

static c_inline int
xl_keyboard_key_is_up(xl_keyboard_t* k, xl_keyboard_key_index_t i)
{
    return !xl_keyboard_key_is_down(k, i);
}

#define N(cap, low)                                                         \
                                                                            \
    static c_inline int xl_keyboard_ ## low ## _is_down (xl_keyboard_t* k)  \
    {                                                                       \
        return xl_keyboard_key_is_down(k, XL_KEYBOARD_KEY_INDEX_ ## cap);   \
    }                                                                       \
                                                                            \
    static c_inline int xl_keyboard_ ## low ## _is_up (xl_keyboard_t* k)    \
    {                                                                       \
        return xl_keyboard_key_is_up(k, XL_KEYBOARD_KEY_INDEX_ ## cap);     \
    }                                                                       \

XL_KEYBOARD_KEY_N
#undef N

XL_DECL double XL_CALL // get last time a given key was pressed
xl_keyboard_get_last_key_pressed_time (xl_keyboard_t * keyboard,
                                    xl_keyboard_key_index_t key);

XL_DECL double XL_CALL // get last time a given key was released
xl_keyboard_get_last_key_released_time(xl_keyboard_t * keyboard,
                                    xl_keyboard_key_index_t key);

#define N(cap, low)                                                         \
                                                                            \
    static c_inline double                                                  \
    xl_keyboard_get_last_ ## low ## _pressed_time(xl_keyboard_t* keyboard)  \
    {                                                                       \
        const  xl_keyboard_key_index_t v = XL_KEYBOARD_KEY_INDEX_ ## cap;   \
        return xl_keyboard_get_last_key_pressed_time(keyboard, v);          \
    }                                                                       \
                                                                            \
    static c_inline double                                                  \
    xl_keyboard_get_last_ ## low ## _released_time(xl_keyboard_t* keyboard) \
    {                                                                       \
        const  xl_keyboard_key_index_t v = XL_KEYBOARD_KEY_INDEX_ ## cap;   \
        return xl_keyboard_get_last_key_released_time(keyboard, v);         \
    }                                                                       \

XL_KEYBOARD_KEY_N
#undef N

/* The input history system is used for things like cheat codes, fighting
 * game key combos, etc. This function clears the input history, which
 * avoids repeatedly calling behavior triggered by successful key combos.
 */
XL_DECL void XL_CALL xl_keyboard_clear_history(xl_keyboard_t* keyboard);

/* Returns true if the last N key inputs match with given state vectors.
 */
XL_DECL int XL_CALL xl_keyboard_check_history( xl_keyboard_t* keyboard,
                const xl_keyboard_key_bit_t* const masks, size_t count);

/*
================================================================================
 * ~~ [ mouse input ] ~~ *
--------------------------------------------------------------------------------
*/

/* Get the main application/system mouse (usually the first one plugged in).
 * If nothing is present, this returns NULL (which acts as a closed mouse).
 */
XL_DECL xl_mouse_t* XL_CALL xl_primary_mouse(void);

#define XL_MOUSE_PROPERTY_N                                                     \
                                                                                \
    /* the total number of mice plugged in */                                   \
    N(XL_MOUSE_PROPERTY_TOTAL, int, int, total)                                 \
                                                                                \
    /* the mouse's unique integer identifier */                                 \
    N(XL_MOUSE_PROPERTY_ID, int, int, id)                                       \
                                                                                \
    /* the current mouse button mask (see xl_mouse_button_bit_t for bits) */    \
    N(XL_MOUSE_PROPERTY_DOWN_BUTTONS, int, int, down_buttons)                   \
    N(XL_MOUSE_PROPERTY_UP_BUTTONS, int, int, up_buttons)                       \
                                                                                \
    /* +1 if right mouse button is down, -1 if the left is down, otherwise 0 */ \
    N(XL_MOUSE_PROPERTY_TRIBOOL, int, int, tribool)                             \
                                                                                \
    /* the last button pressed or released on a given mouse */                  \
    N(XL_MOUSE_PROPERTY_LAST_PRESSED_BUTTON, int, int, last_pressed_button)     \
    N(XL_MOUSE_PROPERTY_LAST_RELEASED_BUTTON, int, int, last_released_button)   \
                                                                                \
    /* the last time any mouse button was pressed or released */                \
    N(XL_MOUSE_PROPERTY_LAST_PRESSED_TIME, double, dbl, last_pressed_time)      \
    N(XL_MOUSE_PROPERTY_LAST_RELEASED_TIME, double, dbl, last_released_time)    \
                                                                                \
    /* the current location of the mouse cursor */                              \
    N(XL_MOUSE_PROPERTY_WINDOW, void*, ptr, window)                             \
    N(XL_MOUSE_PROPERTY_X, double, dbl, x)                                      \
    N(XL_MOUSE_PROPERTY_Y, double, dbl, y)                                      \
    N(XL_MOUSE_PROPERTY_DX, double, dbl, dx)                                    \
    N(XL_MOUSE_PROPERTY_DY, double, dbl, dy)                                    \
                                                                                \
    /* When the mouse is in relative mode, the cursor is hidden, and the driver \
     * will report continuous motion within the current window. Only relative   \
     * motion events will be delivered, and the mouse position will not change. \
     */                                                                         \
    N(XL_MOUSE_PROPERTY_RELATIVE, int, int, relative)                           \
                                                                                \
    /* whether or not the cursor is hidden from view */                         \
    N(XL_MOUSE_PROPERTY_VISIBLE, int, int, visible)                             \
                                                                                \
    /* the current state of the mouse as a string */                            \
    N(XL_MOUSE_PROPERTY_STATUS, const char*, str, status)                       \
                                                                                \
    /* the mouse's string identifier */                                         \
    N(XL_MOUSE_PROPERTY_NAME, const char*, str, name)                           \
                                                                                \
    /* is this mouse the first one plugged in? */                               \
    N(XL_MOUSE_PROPERTY_PRIMARY, int, int, primary)                             \
                                                                                \
    /* is this mouse plugged in? */                                             \
    N(XL_MOUSE_PROPERTY_OPEN, int, int, open)                                   \
                                                                                \
    /* the total number of mouse properties */                                  \
    N(XL_MOUSE_PROPERTY_COUNT, int, int, _)                                     \

typedef enum xl_mouse_property_t
{
    #define N(x, t, s, n) x,
    XL_MOUSE_PROPERTY_N
    #undef N
} \
    xl_mouse_property_t;

static const char* xl_mouse_property_name[] =
{
    #define N(x, t, s, n) #x,
    XL_MOUSE_PROPERTY_N
    #undef N
};

static const char* xl_mouse_property_type[] =
{
    #define N(x, t, s, n) #s,
    XL_MOUSE_PROPERTY_N
    #undef N
};

XL_DECL void XL_CALL
xl_mouse_set_int(xl_mouse_t* mouse, xl_mouse_property_t prop, int value);

XL_DECL int XL_CALL
xl_mouse_get_int(xl_mouse_t* mouse, xl_mouse_property_t prop);

XL_DECL void XL_CALL
xl_mouse_set_dbl(xl_mouse_t* mouse, xl_mouse_property_t prop, double value);

XL_DECL double XL_CALL
xl_mouse_get_dbl(xl_mouse_t* mouse, xl_mouse_property_t prop);

XL_DECL void XL_CALL
xl_mouse_set_str(xl_mouse_t* mouse, xl_mouse_property_t prop, const char* value);

XL_DECL const char* XL_CALL
xl_mouse_get_str(xl_mouse_t* mouse, xl_mouse_property_t prop);

XL_DECL void XL_CALL
xl_mouse_set_ptr(xl_mouse_t* mouse, xl_mouse_property_t prop, void* value);

XL_DECL void* XL_CALL
xl_mouse_get_ptr(xl_mouse_t* mouse, xl_mouse_property_t prop);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C.
 * xl_mouse_get_str(mouse, XL_MOUSE_PROPERTY_STATUS) = xl_mouse_get_status(mouse).
 */
#define N(x, type, short_type, name)                                        \
                                                                            \
    static c_inline void xl_mouse_set_ ## name (xl_mouse_t* mouse, type v)  \
    {                                                                       \
        xl_mouse_set_ ## short_type (mouse, x, v);                          \
    }                                                                       \
                                                                            \
    static c_inline type xl_mouse_get_ ## name (xl_mouse_t* mouse)          \
    {                                                                       \
        return xl_mouse_get_ ## short_type (mouse, x);                      \
    }                                                                       \

XL_MOUSE_PROPERTY_N
#undef N

static c_inline size_t xl_mouse_count_all(void)
{
    return (size_t)xl_mouse_get_total(NULL);
}

XL_DECL void XL_CALL xl_mouse_list_all(xl_mouse_t** mice);

/* ===== [ mouse buttons ] ================================================== */

#define XL_MOUSE_BUTTON_N   \
                            \
    N(LEFT,     left,   0)  \
    N(MIDDLE,   middle, 1)  \
    N(RIGHT,    right,  2)  \
    N(COUNT,    _,      3)  \

typedef enum xl_mouse_button_index_t
{
    #define N(cap, low, val) XL_MOUSE_BUTTON_INDEX_ ## cap,
    XL_MOUSE_BUTTON_N
    #undef N
} \
    xl_mouse_button_index_t;

static const char* xl_mouse_button_index_name[] =
{
    #define N(cap, low, val) AE_STRINGIFY(XL_MOUSE_BUTTON_INDEX_ ## cap),
    XL_MOUSE_BUTTON_N
    #undef N
};

static const char* xl_mouse_button_short_name[] =
{
    #define N(cap, low, val) #low,
    XL_MOUSE_BUTTON_N
    #undef N
};

typedef enum xl_mouse_button_bit_t
{
    #define N(cap, low, val) XL_MOUSE_BUTTON_BIT_ ## cap = AE_IDX2BIT(val),
    XL_MOUSE_BUTTON_N
    #undef N
} \
    xl_mouse_button_bit_t;

XL_DECL xl_mouse_button_index_t XL_CALL // get button index from name
        xl_mouse_button_index_from_short_name(const char* name);

static c_inline xl_mouse_button_bit_t // get button bitmask from name
                xl_mouse_button_bit_from_short_name(const char* name)
{
    return ((xl_mouse_button_bit_t)
            AE_IDX2BIT(xl_mouse_button_index_from_short_name(name)));
}

static c_inline int
xl_mouse_button_is_down(xl_mouse_t* c, xl_mouse_button_index_t b)
{
    return (xl_mouse_get_down_buttons(c) & AE_IDX2BIT(b)) != 0;
}

static c_inline int
xl_mouse_button_is_up(xl_mouse_t* c, xl_mouse_button_index_t b)
{
    return !xl_mouse_button_is_down(c, b);
}

#define N(cap, low, val)                                                    \
                                                                            \
    static c_inline int xl_mouse_ ## low ## _is_down (xl_mouse_t* c)        \
    {                                                                       \
        return xl_mouse_button_is_down(c, XL_MOUSE_BUTTON_INDEX_ ## cap);   \
    }                                                                       \
                                                                            \
    static c_inline int xl_mouse_ ## low ## _is_up (xl_mouse_t* c)          \
    {                                                                       \
        return xl_mouse_button_is_up(c, XL_MOUSE_BUTTON_INDEX_ ## cap);     \
    }                                                                       \

XL_MOUSE_BUTTON_N
#undef N

XL_DECL double XL_CALL xl_mouse_get_last_button_pressed_time(xl_mouse_t * mouse,
                                                xl_mouse_button_index_t button);

XL_DECL double XL_CALL xl_mouse_get_last_button_released_time(xl_mouse_t* mouse,
                                                xl_mouse_button_index_t button);

#define N(cap, low, val)                                                    \
                                                                            \
    static c_inline double                                                  \
    xl_mouse_get_last_ ## low ## _pressed_time(xl_mouse_t* mouse)           \
    {                                                                       \
        const xl_mouse_button_index_t v = XL_MOUSE_BUTTON_INDEX_ ## cap;    \
        return xl_mouse_get_last_button_pressed_time(mouse, v);             \
    }                                                                       \
                                                                            \
    static c_inline double                                                  \
    xl_mouse_get_last_ ## low ## _released_time(xl_mouse_t* mouse)          \
    {                                                                       \
        const xl_mouse_button_index_t v = XL_MOUSE_BUTTON_INDEX_ ## cap;    \
        return xl_mouse_get_last_button_released_time(mouse, v);            \
    }                                                                       \

XL_MOUSE_BUTTON_N
#undef N

XL_DECL void XL_CALL xl_mouse_clear_history(xl_mouse_t* mouse);

XL_DECL int XL_CALL xl_mouse_check_history(xl_mouse_t* mouse,
                        const int* const masks, size_t count);

/*
================================================================================
 * ~~ [ controller input ] ~~ *
--------------------------------------------------------------------------------
*/

/* Get the main application/system controller (usually the first one plugged in).
 * If nothing is present, this returns NULL (which acts as a closed controller).
 */
XL_DECL xl_controller_t* XL_CALL xl_primary_controller(void);

#define XL_CONTROLLER_PROPERTY_N                                                            \
                                                                                            \
    /* the total number of controllers plugged in */                                        \
    N(XL_CONTROLLER_PROPERTY_TOTAL, int, int, total)                                        \
                                                                                            \
    /* unique integer identifier */                                                         \
    N(XL_CONTROLLER_PROPERTY_ID, int, int, id)                                              \
                                                                                            \
    /* get button mask (see ae_controller_button_bit_t) */                                  \
    N(XL_CONTROLLER_PROPERTY_DOWN_BUTTONS, int, int, down_buttons)                          \
    N(XL_CONTROLLER_PROPERTY_UP_BUTTONS, int, int, up_buttons)                              \
                                                                                            \
    /* tribools (-1, 0, or +1) are based on the state of two opposite buttons */            \
    N(XL_CONTROLLER_PROPERTY_SHOULDER_TRIBOOL, int, int, shoulder_tribool)                  \
    N(XL_CONTROLLER_PROPERTY_DPAD_HORIZONTAL_TRIBOOL, int, int, dpad_horizontal_tribool)    \
    N(XL_CONTROLLER_PROPERTY_DPAD_VERTICAL_TRIBOOL, int, int, dpad_vertical_tribool)        \
    N(XL_CONTROLLER_PROPERTY_STICK_TRIBOOL, int, int, stick_tribool)                        \
                                                                                            \
    /* most recent input */                                                                 \
    N(XL_CONTROLLER_PROPERTY_LAST_PRESSED_BUTTON, int, int, last_pressed_button)            \
    N(XL_CONTROLLER_PROPERTY_LAST_RELEASED_BUTTON, int, int, last_released_button)          \
    N(XL_CONTROLLER_PROPERTY_LAST_PRESSED_TIME, double, dbl, last_pressed_time)             \
    N(XL_CONTROLLER_PROPERTY_LAST_RELEASED_TIME, double, dbl, last_released_time)           \
                                                                                            \
    /* triggers (pressure-sensitive shoulder buttons) */                                    \
    N(XL_CONTROLLER_PROPERTY_RIGHT_TRIGGER, double, dbl, right_trigger)                     \
    N(XL_CONTROLLER_PROPERTY_LEFT_TRIGGER, double, dbl, left_trigger)                       \
                                                                                            \
    /* analog sticks */                                                                     \
    N(XL_CONTROLLER_PROPERTY_RIGHT_DEADZONE_MODE, int, int, right_deadzone_mode)            \
    N(XL_CONTROLLER_PROPERTY_RIGHT_DEADZONE_VALUE, double, dbl, right_deadzone_value)       \
    N(XL_CONTROLLER_PROPERTY_LEFT_DEADZONE_MODE, int, int, left_deadzone_mode)              \
    N(XL_CONTROLLER_PROPERTY_LEFT_DEADZONE_VALUE, double, dbl, left_deadzone_value)         \
                                                                                            \
    N(XL_CONTROLLER_PROPERTY_RIGHT_STICK_ANGLE, double, dbl, right_stick_angle)             \
    N(XL_CONTROLLER_PROPERTY_RIGHT_STICK_MAGNITUDE, double, dbl, right_stick_magnitude)     \
    N(XL_CONTROLLER_PROPERTY_LEFT_STICK_ANGLE, double, dbl, left_stick_angle)               \
    N(XL_CONTROLLER_PROPERTY_LEFT_STICK_MAGNITUDE, double, dbl, left_stick_magnitude)       \
                                                                                            \
    N(XL_CONTROLLER_PROPERTY_RIGHT_STICK_X, double, dbl, right_stick_x)                     \
    N(XL_CONTROLLER_PROPERTY_RIGHT_STICK_Y, double, dbl, right_stick_y)                     \
    N(XL_CONTROLLER_PROPERTY_LEFT_STICK_X, double, dbl, left_stick_x)                       \
    N(XL_CONTROLLER_PROPERTY_LEFT_STICK_Y, double, dbl, left_stick_y)                       \
                                                                                            \
    /* miscellaneous properties */                                                          \
    N(XL_CONTROLLER_PROPERTY_PRIMARY, int, int, primary)                                    \
    N(XL_CONTROLLER_PROPERTY_OPEN, int, int, open)                                          \
    N(XL_CONTROLLER_PROPERTY_STATUS, const char*, str, status)                              \
    N(XL_CONTROLLER_PROPERTY_NAME, const char*, str, name)                                  \
    N(XL_CONTROLLER_PROPERTY_COUNT, int, int, _)                                            \

typedef enum xl_controller_property_t
{
    #define N(x, t, s, n) x,
    XL_CONTROLLER_PROPERTY_N
    #undef N
} \
    xl_controller_property_t;

static const char* xl_controller_property_name[] =
{
    #define N(x, t, s, n) #x,
    XL_CONTROLLER_PROPERTY_N
    #undef N
};

static const char* xl_controller_property_type[] =
{
    #define N(x, t, s, n) #s,
    XL_CONTROLLER_PROPERTY_N
    #undef N
};

XL_DECL void XL_CALL
xl_controller_set_int(xl_controller_t* ctl, xl_controller_property_t prop, int value);

XL_DECL int XL_CALL
xl_controller_get_int(xl_controller_t* ctl, xl_controller_property_t prop);

XL_DECL void XL_CALL
xl_controller_set_flt(xl_controller_t* ctl, xl_controller_property_t prop, float value);

XL_DECL float XL_CALL
xl_controller_get_flt(xl_controller_t* ctl, xl_controller_property_t prop);

XL_DECL void XL_CALL
xl_controller_set_dbl(xl_controller_t* ctl, xl_controller_property_t prop, double value);

XL_DECL double XL_CALL
xl_controller_get_dbl(xl_controller_t* ctl, xl_controller_property_t prop);

XL_DECL void XL_CALL
xl_controller_set_str(xl_controller_t* ctl, xl_controller_property_t prop, const char* value);

XL_DECL const char* XL_CALL
xl_controller_get_str(xl_controller_t* ctl, xl_controller_property_t prop);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C code.
 * xl_controller_get_str(ct, XL_CONTROLLER_PROPERTY_NAME) = xl_controller_get_name(ct).
 */
#define N(x, type, short_type, name)                                                \
                                                                                    \
    static c_inline void xl_controller_set_ ## name (xl_controller_t* c, type val)  \
    {                                                                               \
        xl_controller_set_ ## short_type (c, x, val);                               \
    }                                                                               \
                                                                                    \
    static c_inline type xl_controller_get_ ## name (xl_controller_t* controller)   \
    {                                                                               \
        return xl_controller_get_ ## short_type (controller, x);                    \
    }                                                                               \

XL_CONTROLLER_PROPERTY_N
#undef N

static c_inline size_t xl_controller_count_all(void)
{
    return (size_t)xl_controller_get_total(NULL);
}

XL_DECL void XL_CALL xl_controller_list_all(xl_controller_t** controllers);

/* ===== [ digital buttons ] ================================================ */

#define XL_CONTROLLER_BUTTON_N              \
    N(A,                a,               0) \
    N(B,                b,               1) \
    N(X,                x,               2) \
    N(Y,                y,               3) \
    N(SELECT,           select,          4) \
    N(START,            start,           5) \
    N(LEFT_STICK,       left_stick,      6) \
    N(RIGHT_STICK,      right_stick,     7) \
    N(LEFT_SHOULDER,    left_shoulder,   8) \
    N(RIGHT_SHOULDER,   right_shoulder,  9) \
    N(DPAD_UP,          dpad_up,        10) \
    N(DPAD_DOWN,        dpad_down,      11) \
    N(DPAD_LEFT,        dpad_left,      12) \
    N(DPAD_RIGHT,       dpad_right,     13) \
    N(COUNT,            _,              14) \

typedef enum xl_controller_button_index_t
{
    #define N(cap, low, val) XL_CONTROLLER_BUTTON_INDEX_ ## cap,
    XL_CONTROLLER_BUTTON_N
    #undef N
} \
    xl_controller_button_index_t;

static const char* xl_controller_button_index_name[] =
{
    #define N(cap, low, val) AE_STRINGIFY(XL_CONTROLLER_BUTTON_INDEX_ ## cap),
    XL_CONTROLLER_BUTTON_N
    #undef N
};

static const char* xl_controller_button_short_name[] =
{
    #define N(cap, low, val) #low,
    XL_CONTROLLER_BUTTON_N
    #undef N
};

typedef enum xl_controller_button_bit_t
{
    #define N(cap, low, val) XL_CONTROLLER_BUTTON_BIT_ ## cap = AE_IDX2BIT(val),
    XL_CONTROLLER_BUTTON_N
    #undef N
} \
    xl_controller_button_bit_t;

XL_DECL xl_controller_button_index_t XL_CALL // get button from name
        xl_controller_button_index_from_short_name(const char* name);

static c_inline xl_controller_button_bit_t // get button bitmask from name
                xl_controller_button_bit_from_short_name(const char* name)
{
    return ((xl_controller_button_bit_t)
            AE_IDX2BIT(xl_controller_button_index_from_short_name(name)));
}

static c_inline int
xl_controller_button_is_down(xl_controller_t* c, xl_controller_button_index_t b)
{
    return (xl_controller_get_down_buttons(c) & AE_IDX2BIT(b)) != 0;
}

static c_inline int
xl_controller_button_is_up(xl_controller_t* c, xl_controller_button_index_t b)
{
    return !xl_controller_button_is_down(c, b);
}

#define N(cap, low, val)                                                            \
                                                                                    \
    static c_inline int xl_controller_ ## low ## _is_down (xl_controller_t* c)      \
    {                                                                               \
        return xl_controller_button_is_down(c, XL_CONTROLLER_BUTTON_INDEX_ ## cap); \
    }                                                                               \
                                                                                    \
    static c_inline int xl_controller_ ## low ## _is_up (xl_controller_t* c)        \
    {                                                                               \
        return xl_controller_button_is_up(c, XL_CONTROLLER_BUTTON_INDEX_ ## cap);   \
    }                                                                               \

XL_CONTROLLER_BUTTON_N
#undef N

XL_DECL double XL_CALL
xl_controller_get_last_button_pressed_time( xl_controller_t * controller,
                                    xl_controller_button_index_t button);

XL_DECL double XL_CALL
xl_controller_get_last_button_released_time(xl_controller_t * controller,
                                    xl_controller_button_index_t button);

#define N(cap, low, val)                                                            \
                                                                                    \
    static c_inline double                                                          \
    xl_controller_get_last_ ## low ## _pressed_time(xl_controller_t* controller)    \
    {                                                                               \
        const xl_controller_button_index_t v = XL_CONTROLLER_BUTTON_INDEX_ ## cap;  \
        return xl_controller_get_last_button_pressed_time(controller, v);           \
    }                                                                               \
                                                                                    \
    static c_inline double                                                          \
    xl_controller_get_last_ ## low ## _released_time(xl_controller_t* controller)   \
    {                                                                               \
        const xl_controller_button_index_t v = XL_CONTROLLER_BUTTON_INDEX_ ## cap;  \
        return xl_controller_get_last_button_released_time(controller, v);          \
    }                                                                               \

XL_CONTROLLER_BUTTON_N
#undef N

/* Controller input history is used for things like cheat codes, button combos, etc.
 * It tracks the state of the controller using button bitmasks, not single indices.
 *
 * If the button combo isn't setting a flag that will be used later, the user of the
 * API should clear the history upon successful checks to avoid re-triggering events.
 */
XL_DECL void XL_CALL xl_controller_clear_history(xl_controller_t* controller);

/* Returns true if the last N button button inputs match with the given masks.
 */
XL_DECL int XL_CALL xl_controller_check_history( xl_controller_t* controller,
                                        const int* const masks, size_t count);

/* ===== [ analog axes & triggers ] ========================================= */

// get the shoulder trigger state based on a character identifier ('<', 'R', etc.)
XL_DECL double XL_CALL xl_controller_get_trigger(xl_controller_t* c, char which);

#define XL_CONTROLLER_DEADZONE_MODE_N                           \
                                                                \
    /* raw controller input allowing "stick drift" to happen */ \
    N(XL_CONTROLLER_DEADZONE_MODE_NONE, none)                   \
                                                                \
    /* snaps to four cardinal directions, like "analog mode" */ \
    N(XL_CONTROLLER_DEADZONE_MODE_AXIAL, axial)                 \
                                                                \
    /* throws away all input inside a discrete circle shape */  \
    N(XL_CONTROLLER_DEADZONE_MODE_RADIAL, radial)               \
                                                                \
    /* smoothly remaps input away from the dead zone */         \
    N(XL_CONTROLLER_DEADZONE_MODE_SCALED_RADIAL, scaled_radial) \
                                                                \
    /* bowtie shape for sensitive twitchy FPS controls */       \
    N(XL_CONTROLLER_DEADZONE_MODE_X_BOWTIE, x_bowtie)           \
    N(XL_CONTROLLER_DEADZONE_MODE_Y_BOWTIE, y_bowtie)           \
                                                                \
    N(XL_CONTROLLER_DEADZONE_MODE_COUNT, _)                     \

typedef enum xl_controller_deadzone_mode_t
{
    #define N(cap, low) cap,
    XL_CONTROLLER_DEADZONE_MODE_N
    #undef N
} \
    xl_controller_deadzone_mode_t;

static const char* xl_controller_deadzone_mode_name[] =
{
    #define N(cap, low) #cap,
    XL_CONTROLLER_DEADZONE_MODE_N
    #undef N
};

static const char* xl_controller_deadzone_short_name[] =
{
    #define N(cap, low) #low,
    XL_CONTROLLER_DEADZONE_MODE_N
    #undef N
};

XL_DECL xl_controller_deadzone_mode_t XL_CALL // deadzone mode from name
        xl_controller_deadzone_mode_from_short_name(const char* s_name);

XL_DECL void XL_CALL
xl_controller_get_deadzone(xl_controller_t * controller, char which,
                xl_controller_deadzone_mode_t* mode, double* value);

XL_DECL void XL_CALL
xl_controller_set_deadzone(xl_controller_t* controller, char which,
                xl_controller_deadzone_mode_t mode, double value);

XL_DECL double XL_CALL // polar (len, dir)
xl_controller_get_stick_angle(xl_controller_t* controller, char which);

XL_DECL double XL_CALL // polar (len, dir)
xl_controller_get_stick_magnitude(xl_controller_t* controller, char which);

XL_DECL void XL_CALL // cartesian (x, y)
xl_controller_get_stick(xl_controller_t* c, char which, double* x, double* y);

/*
================================================================================
 * ~~ [ atlas animation ] ~~ *
--------------------------------------------------------------------------------
*/

XL_DECL xl_animation_t* XL_CALL xl_animation_create(void); // init and clone
XL_DECL xl_animation_t* XL_CALL xl_animation_copy(xl_animation_t* animation);

#define XL_ANIMATION_PROPERTY_N                                     \
                                                                    \
    /* the current number of open animations */                     \
    N(XL_ANIMATION_PROPERTY_TOTAL, int, int, total)                 \
                                                                    \
    /* a unique integer identifier */                               \
    N(XL_ANIMATION_PROPERTY_ID, int, int, id)                       \
                                                                    \
    /* a single-texture sequence of frames in a 2D grid pattern */  \
    N(XL_ANIMATION_PROPERTY_ATLAS, xl_texture_t*, tex, atlas)       \
                                                                    \
    /* do we close the texture with the animation? */               \
    N(XL_ANIMATION_PROPERTY_OWNS_ATLAS, int, int, owns_atlas)       \
                                                                    \
    /* the dimensions of one frame within the atlas */              \
    N(XL_ANIMATION_PROPERTY_FRAME_WIDTH, int, int, frame_width)     \
    N(XL_ANIMATION_PROPERTY_FRAME_HEIGHT, int, int, frame_height)   \
                                                                    \
    /* optional starting point (usually frame zero) */              \
    N(XL_ANIMATION_PROPERTY_FIRST_FRAME, int, int, first_frame)     \
                                                                    \
    /* auto-calculated, can also be optionally set to limit anim */ \
    N(XL_ANIMATION_PROPERTY_FRAME_COUNT, int, int, frame_count)     \
                                                                    \
    /* period (time per frame) and the total animation time */      \
    N(XL_ANIMATION_PROPERTY_FRAME_TIME, double, dbl, frame_time)    \
    N(XL_ANIMATION_PROPERTY_TOTAL_TIME, double, dbl, total_time)    \
                                                                    \
    /* the current slot/index in the atlas we're rendering */       \
    N(XL_ANIMATION_PROPERTY_CURRENT_FRAME, int, int, current_frame) \
                                                                    \
    /* the current elapsed playback time (advanced by update) */    \
    N(XL_ANIMATION_PROPERTY_POSITION, double, dbl, position)        \
                                                                    \
    /* does the animation repeat when finished? */                  \
    N(XL_ANIMATION_PROPERTY_LOOPS, int, int, loops)                 \
                                                                    \
    /* is the animation finished playing (if not looping)? */       \
    N(XL_ANIMATION_PROPERTY_FINISHED, int, int, finished)           \
                                                                    \
    /* whatever info we want to print about this animation */       \
    N(XL_ANIMATION_PROPERTY_STATUS, const char*, str, status)       \
                                                                    \
    /* optional atlas path and custom user identifier */            \
    N(XL_ANIMATION_PROPERTY_PATH, const char*, str, path)           \
    N(XL_ANIMATION_PROPERTY_NAME, const char*, str, name)           \
                                                                    \
    /* is the animation live and valid? */                          \
    N(XL_ANIMATION_PROPERTY_OPEN, int, int, open)                   \
                                                                    \
    /* the total number of animation properties */                  \
    N(XL_ANIMATION_PROPERTY_COUNT, int, int, _)                     \

typedef enum xl_animation_property_t
{
    #define N(x, t, s, n) x,
    XL_ANIMATION_PROPERTY_N
    #undef N
} \
    xl_animation_property_t;

static const char* xl_animation_property_name[] =
{
    #define N(x, t, s, n) #x,
    XL_ANIMATION_PROPERTY_N
    #undef N
};

static const char* xl_animation_property_type[] =
{
    #define N(x, t, s, n) #s,
    XL_ANIMATION_PROPERTY_N
    #undef N
};

XL_DECL void XL_CALL
xl_animation_set_int(xl_animation_t* anim, xl_animation_property_t prop, int value);

XL_DECL int XL_CALL
xl_animation_get_int(xl_animation_t* anim, xl_animation_property_t prop);

XL_DECL void XL_CALL
xl_animation_set_dbl(xl_animation_t* anim, xl_animation_property_t prop, double value);

XL_DECL double XL_CALL
xl_animation_get_dbl(xl_animation_t* anim, xl_animation_property_t prop);

XL_DECL void XL_CALL
xl_animation_set_str(xl_animation_t* anim, xl_animation_property_t prop, const char* value);

XL_DECL const char* XL_CALL
xl_animation_get_str(xl_animation_t* anim, xl_animation_property_t prop);

XL_DECL void XL_CALL
xl_animation_set_tex(xl_animation_t* anim, xl_animation_property_t prop, xl_texture_t* value);

XL_DECL xl_texture_t* XL_CALL
xl_animation_get_tex(xl_animation_t* anim, xl_animation_property_t prop);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C code.
 */
#define N(x, type, short_type, name)                                                \
                                                                                    \
    static c_inline void xl_animation_set_ ## name (xl_animation_t* anim, type val) \
    {                                                                               \
        xl_animation_set_ ## short_type (anim, x, val);                             \
    }                                                                               \
                                                                                    \
    static c_inline type xl_animation_get_ ## name (xl_animation_t* anim)           \
    {                                                                               \
        return xl_animation_get_ ## short_type (anim, x);                           \
    }                                                                               \

XL_ANIMATION_PROPERTY_N
#undef N

static c_inline void xl_animation_close(xl_animation_t* animation)
{
    xl_animation_set_open(animation, 0);
}

/* Essentially a shorthand alias for xl_animation_set_position(animation, 0.0).
 */
XL_DECL void XL_CALL xl_animation_reset(xl_animation_t* animation);
XL_DECL void XL_CALL xl_animation_reset_all(void);

/* Advance an animation by a given number of seconds (usually the frame delta).
 * Essentially a shorthand alias for set_position(anm, get_position(anm) + dt).
 */
XL_DECL void XL_CALL xl_animation_update(xl_animation_t* animation, double dt);
XL_DECL void XL_CALL xl_animation_update_all(double dt);

/* Map the 1D animation index (current frame) to the 2D region of the atlas to draw.
 */
XL_DECL void XL_CALL xl_animation_src_rect(xl_animation_t* animation, float* rect);

/* Create a destination rect (region of the render target to stretch frame onto).
 */
XL_DECL void XL_CALL xl_animation_dst_rect(xl_animation_t* animation, float* rect,
                                        const float pos[2], const float scale[2]);

/* Draw the current frame in the texture atlas, with a given scale and rotation.
 */
XL_DECL void XL_CALL
xl_animation_draw_ex(xl_animation_t* animation, float* dst_rect,
                                    double angle, float* center);

/* Draw the current frame in the texture atlas at a given position.
 */
XL_DECL void XL_CALL
xl_animation_draw(xl_animation_t* animation, const float xy[2]);

/* Load an animation from a file, which (given the implementation), could be:
 *
 *  - A single-image texture atlas.
 *  - An image archive containing images of equal size.
 *  - A directory or zip file containing of images of equal size.
 *  - An animated GIF file.
 *  - A very, very small video clip.
 *  - Something else (rocket probe to the moon?)
 *
 * The period (frame time) of the animation may or may not be changed from the
 * default value, depending on the type of animation loaded.
 * Frame width and height may be zero for codecs that contain frame size data.
 */
XL_DECL xl_animation_t* XL_CALL xl_animation_load(xl_window_t* window,
            const char* filename, int frame_width, int frame_height);

XL_DECL xl_animation_t* XL_CALL
xl_animation_load_ex(xl_window_t* window, const char* filename, int frame_width,
                            int frame_height, ae_image_error_t* error_status);

static c_inline size_t xl_animation_count_all(void)
{
    return (size_t)xl_animation_get_total(NULL);
}

XL_DECL void XL_CALL xl_animation_list_all(xl_animation_t** animations);
XL_DECL void XL_CALL xl_animation_close_all(void);

/*
================================================================================
 * ~~ [ timer objects ] ~~ *
--------------------------------------------------------------------------------
*/

XL_DECL xl_clock_t* XL_CALL xl_clock_create(void);

#define XL_CLOCK_PROPERTY_N                                 \
                                                            \
    N(XL_CLOCK_PROPERTY_TOTAL, int, int, total)             \
    N(XL_CLOCK_PROPERTY_ID, int, int, id)                   \
    N(XL_CLOCK_PROPERTY_NUM_TIMERS, int, int, num_timers)   \
    N(XL_CLOCK_PROPERTY_DT, double, dbl, dt)                \
    N(XL_CLOCK_PROPERTY_AUTO_UPDATE, int, int, auto_update) \
    N(XL_CLOCK_PROPERTY_STATUS, const char*, str, status)   \
    N(XL_CLOCK_PROPERTY_NAME, const char*, str, name)       \
    N(XL_CLOCK_PROPERTY_OPEN, int, int, open)               \
    N(XL_CLOCK_PROPERTY_COUNT, int, int, _)                 \

typedef enum xl_clock_property_t
{
    #define N(x, t, s, n) x,
    XL_CLOCK_PROPERTY_N
    #undef N
} \
    xl_clock_property_t;

static const char* xl_clock_property_name[] =
{
    #define N(x, t, s, n) #x,
    XL_CLOCK_PROPERTY_N
    #undef N
};

static const char* xl_clock_property_type[] =
{
    #define N(x, t, s, n) #s,
    XL_CLOCK_PROPERTY_N
    #undef N
};

XL_DECL void XL_CALL
xl_clock_set_int(xl_clock_t* clock, xl_clock_property_t prop, int value);

XL_DECL int XL_CALL
xl_clock_get_int(xl_clock_t* clock, xl_clock_property_t prop);

XL_DECL void XL_CALL
xl_clock_set_dbl(xl_clock_t* clock, xl_clock_property_t prop, double value);

XL_DECL double XL_CALL
xl_clock_get_dbl(xl_clock_t* clock, xl_clock_property_t prop);

XL_DECL void XL_CALL
xl_clock_set_str(xl_clock_t* clock, xl_clock_property_t prop, const char* value);

XL_DECL const char* XL_CALL
xl_clock_get_str(xl_clock_t* clock, xl_clock_property_t prop);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C code.
 */
#define N(x, type, short_type, name)                                            \
                                                                                \
    static c_inline void xl_clock_set_ ## name (xl_clock_t* clock, type value)  \
    {                                                                           \
        xl_clock_set_ ## short_type (clock, x, value);                          \
    }                                                                           \
                                                                                \
    static c_inline type xl_clock_get_ ## name (xl_clock_t* clock)              \
    {                                                                           \
        return xl_clock_get_ ## short_type (clock, x);                          \
    }                                                                           \

XL_CLOCK_PROPERTY_N
#undef N

static c_inline void xl_clock_close(xl_clock_t* clock)
{
    xl_clock_set_open(clock, 0);
}

XL_DECL void XL_CALL xl_clock_update(xl_clock_t* clock, double dt);
XL_DECL void XL_CALL xl_clock_update_all(double dt); // tick clock

XL_DECL void XL_CALL
xl_clock_add_timer(xl_clock_t* clock, const char* name, double seconds, int repeat);

XL_DECL void XL_CALL
xl_clock_remove_timer(xl_clock_t* clock, const char* name);

XL_DECL void XL_CALL
xl_clock_remove_all_timers(xl_clock_t* clock);

XL_DECL int XL_CALL xl_clock_get_timer(xl_clock_t* clock, const char* name,
                double* current, double* seconds, int* paused, int* repeat);

XL_DECL void XL_CALL
xl_clock_set_timer_current(xl_clock_t* clock, const char* name, double value);

XL_DECL void XL_CALL
xl_clock_set_timer_seconds(xl_clock_t* clock, const char* name, double value);

XL_DECL void XL_CALL
xl_clock_set_timer_paused(xl_clock_t* clock, const char* name, int value);

XL_DECL void XL_CALL
xl_clock_set_timer_repeat(xl_clock_t* clock, const char* name, int value);

XL_DECL char** XL_CALL
xl_clock_copy_timer_names(xl_clock_t* clock);

XL_DECL void XL_CALL
xl_clock_free_timer_names(xl_clock_t* clock, char** names);

static c_inline size_t xl_clock_count_all(void)
{
    return (size_t)xl_clock_get_total(NULL);
}

XL_DECL void XL_CALL xl_clock_list_all(xl_clock_t** clocks);
XL_DECL void XL_CALL xl_clock_close_all(void);

/*
================================================================================
 * ~~ [ timed events ] ~~ *
--------------------------------------------------------------------------------
*/

XL_DECL void XL_CALL
xl_timer_register(const char* name, double seconds, int repeat);

XL_DECL void XL_CALL
xl_timer_unregister(const char* name);

XL_DECL int XL_CALL
xl_timer_get(const char* name, double* current, double* seconds, int* repeat);

XL_DECL void XL_CALL
xl_timer_set_repeat(const char* name, int repeat);

/*
================================================================================
 * ~~ [ event handling ] ~~ *
--------------------------------------------------------------------------------
*/

#define XL_EVENT_N                                                                  \
                                                                                    \
    /* nothing happened, this event can (and should) be safely ignored */           \
    N(XL_EVENT_NOTHING, nothing, char _pad;)                                        \
                                                                                    \
    /* the last open window was closed or a signal/interrupt was fired */           \
    N(XL_EVENT_QUIT, quit, char _pad;)                                              \
                                                                                    \
    /* the user clicked on the titlebar [X] to close the window or app */           \
    N(XL_EVENT_WINDOW_CLOSE, window_close, xl_window_t* window;)                    \
                                                                                    \
    /* the user clicked on the window and dragged it somewhere else */              \
    N(XL_EVENT_WINDOW_MOVE, window_move, xl_window_t* window; int x; int y;)        \
                                                                                    \
    /* the user dragged the edge of the window to change its dimensions */          \
    N(XL_EVENT_WINDOW_RESIZE, window_resize, xl_window_t * window;                  \
                                            int width; int height;)                 \
                                                                                    \
    /* the "visibility" property of the window was changed programmatically */      \
    N(XL_EVENT_WINDOW_VISIBILITY_CHANGE, window_visibility_change,                  \
                                xl_window_t* window; int visible;)                  \
                                                                                    \
    /* for tool-type applications that wait for events, this indicates that the     \
     * window needs to be repainted (WM_PAINT for all you win32 programmers).       \
     * games that render every frame needn't pay any attention to this event!!      \
     */                                                                             \
    N(XL_EVENT_WINDOW_REDRAW, window_redraw, xl_window_t* window;)                  \
                                                                                    \
    /* the window entered or left the foreground (TODO: "changed focus" instead) */ \
    N(XL_EVENT_WINDOW_GAIN_FOCUS, window_gain_focus, xl_window_t* window;)          \
    N(XL_EVENT_WINDOW_LOSE_FOCUS, window_lose_focus, xl_window_t* window;)          \
                                                                                    \
    /* the mouse cursor entered or left the window (TODO: same change as above!) */ \
    N(XL_EVENT_WINDOW_MOUSE_ENTER, window_mouse_enter, xl_window_t* window;         \
                                                        xl_mouse_t* mouse;)         \
                                                                                    \
    N(XL_EVENT_WINDOW_MOUSE_LEAVE, window_mouse_leave, xl_window_t* window;         \
                                                        xl_mouse_t* mouse;)         \
                                                                                    \
    /* music has finished playing and is now stopped */                             \
    N(XL_EVENT_MUSIC_FINISHED, music_finished, char _pad;)                          \
                                                                                    \
    /* a playing instance of this sound effect has finished */                      \
    N(XL_EVENT_SOUND_FINISHED, sound_finished, xl_sound_t* sound;)                  \
                                                                                    \
    /* an animation has finished playing back */                                    \
    N(XL_EVENT_ANIMATION_FINISHED, animation_finished, xl_animation_t* animation;)  \
                                                                                    \
    /* the user plugged in or unplugged a recognized and valid keyboard */          \
    N(XL_EVENT_KEYBOARD_INSERT, keyboard_insert, xl_keyboard_t* keyboard;)          \
    N(XL_EVENT_KEYBOARD_REMOVE, keyboard_remove, xl_keyboard_t* keyboard;)          \
                                                                                    \
    /* the user hit a key on an active plugged-in keyboard device or released it */ \
    N(XL_EVENT_KEYBOARD_KEY, keyboard_key, xl_keyboard_t * keyboard;                \
            xl_keyboard_mod_bit_t mods; xl_keyboard_key_index_t key; int pressed;)  \
                                                                                    \
    /* the user plugged in or unplugged a valid mouse device */                     \
    N(XL_EVENT_MOUSE_INSERT, mouse_insert, xl_mouse_t* mouse;)                      \
    N(XL_EVENT_MOUSE_REMOVE, mouse_remove, xl_mouse_t* mouse;)                      \
                                                                                    \
    /* the user pressed or released a button on the mouse */                        \
    N(XL_EVENT_MOUSE_BUTTON, mouse_button, xl_mouse_t* mouse;                       \
                xl_mouse_button_index_t button; int pressed;)                       \
                                                                                    \
    /* the user moved the scroll wheel on the mouse, in one of four directions */   \
    N(XL_EVENT_MOUSE_WHEEL, mouse_wheel, xl_mouse_t* mouse; int x; int y;)          \
                                                                                    \
    /* the user moved or dragged the mouse cursor */                                \
    N(XL_EVENT_MOUSE_MOTION, mouse_motion, xl_mouse_t* mouse; xl_window_t* window;  \
        xl_mouse_button_bit_t buttons; double x; double y; double dx; double dy;)   \
                                                                                    \
    /* the user plugged in or unplugged a recognized and valid game controller */   \
    N(XL_EVENT_CONTROLLER_INSERT, controller_insert, xl_controller_t* controller;)  \
    N(XL_EVENT_CONTROLLER_REMOVE, controller_remove, xl_controller_t* controller;)  \
                                                                                    \
    /* the user pressed or released a digital button */                             \
    N(XL_EVENT_CONTROLLER_BUTTON, controller_button, xl_controller_t* controller;   \
                                xl_controller_button_index_t button; int pressed;)  \
                                                                                    \
    /* the user depressed or let up on an analog shoulder button */                 \
    N(XL_EVENT_CONTROLLER_TRIGGER, controller_trigger, xl_controller_t* controller; \
                                                        char which; double value;)  \
                                                                                    \
    /* the user moved an analog thumbstick (NOTE: deadzone is applied to input) */  \
    N(XL_EVENT_CONTROLLER_STICK, controller_stick, xl_controller_t* controller;     \
                char which; double magnitude; double angle; double x; double y;)    \
                                                                                    \
    /* a named timer has fired - if global timer, clock is NULL */                  \
    N(XL_EVENT_TIMER, timer, xl_clock_t* clock; char name[128];                     \
                                    double seconds; int repeat;)                    \
                                                                                    \
    /* got a long frame delta due to a loading screen or poor performance */        \
    N(XL_EVENT_LONG_FRAME, long_frame, double dt;)                                  \
                                                                                    \
    N(XL_EVENT_COUNT, count, char _pad;)                                            \

typedef enum xl_event_type_t
{
    #define N(x, name, members) x,
    XL_EVENT_N
    #undef N
} \
    xl_event_type_t;

static const char* xl_event_type_name[] =
{
    #define N(x, name, members) #x,
    XL_EVENT_N
    #undef N
};

typedef struct xl_event_t
{
    xl_event_type_t type;

    // HACK making sure everything gets assigned to its proper name!
    // this comes at the cost of breaking ABI between debug/release.
#if !defined(AE_DEBUG)
    union
    {
#endif
        #define N(x, name, members) struct { members } as_ ## name;
        XL_EVENT_N
        #undef N

#if !defined(AE_DEBUG)
    };
#endif
} \
    xl_event_t;

// Get and set a callback to be run on all events before poll returns.
typedef void (*xl_event_handler_t)(xl_event_t* event, void* context);

XL_DECL void XL_CALL xl_event_get_handler(xl_event_handler_t*, void**);
XL_DECL void XL_CALL xl_event_set_handler(xl_event_handler_t, void*);

// Get the number of events in the queue waiting to be processed.
XL_DECL size_t XL_CALL xl_event_count_pending(void);

// Grab from the queue, optionally blocking until an event is fired.
XL_DECL int XL_CALL xl_event_poll(xl_event_t* event, int wait);

// NOTE avoiding linker issues when changing build configurations,
// in case somebody accidentally types this into a header file...
#undef XL_DECL

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __XL_CORE_H__ */
