/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_IMAGE_H__
#define __AE_IMAGE_H__

#ifndef __AE_MACROS_H__
#include <ae_macros.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

void ae_image_init(int argc, char** argv);
void ae_image_quit(void); // private init

/*
================================================================================
 * ~~ [ image format enum ] ~~ *
--------------------------------------------------------------------------------
*/

#define AE_IMAGE_FORMAT_N \
    N(AE_IMAGE_FORMAT_RGBA, 4), \
    N(AE_IMAGE_FORMAT_RGB, 3), \
    N(AE_IMAGE_FORMAT_MONO, 1), \
    N(AE_IMAGE_FORMAT_COUNT, 0)

typedef enum ae_image_format_t
{
    #define N(x, c) x
    AE_IMAGE_FORMAT_N
    #undef N
} \
    ae_image_format_t;

static const char* ae_image_format_name[] =
{
    #define N(x, c) #x
    AE_IMAGE_FORMAT_N
    #undef N
};

static size_t ae_image_format_components[] =
{
    #define N(x, c) c
    AE_IMAGE_FORMAT_N
    #undef N
};

/*
================================================================================
 * ~~ [ pixel type enum ] ~~ *
--------------------------------------------------------------------------------
NOTE: float data is in the range [0.0, 1.0], byte data is in the range [0, 255].
NOTE: don't convert by scaling, use ae_image_type_convert for gamma correction.
TODO: now that it's supported by stb_image, AE_IMAGE_TYPE_U16 would be possible.
--------------------------------------------------------------------------------
*/

#define AE_IMAGE_TYPE_N \
    N(AE_IMAGE_TYPE_U8, sizeof(u8)), \
    N(AE_IMAGE_TYPE_FLT, sizeof(float)), \
    N(AE_IMAGE_TYPE_COUNT, 0)

typedef enum ae_image_type_t
{
    #define N(x, b) x
    AE_IMAGE_TYPE_N
    #undef N
} \
    ae_image_type_t;

static const char* ae_image_type_name[] =
{
    #define N(x, b) #x
    AE_IMAGE_TYPE_N
    #undef N
};

static size_t ae_image_type_bytes[] =
{
    #define N(x, b) b
    AE_IMAGE_TYPE_N
    #undef N
};

/*
================================================================================
 * ~~ [ error codes ] ~~ *
--------------------------------------------------------------------------------
*/

#define AE_IMAGE_ERROR_N \
    N(AE_IMAGE_SUCCESS), \
    N(AE_IMAGE_NO_CODEC), \
    N(AE_IMAGE_FILE_NOT_FOUND), \
    N(AE_IMAGE_FILE_CORRUPT), \
    N(AE_IMAGE_WRITE_FAILED), \
    N(AE_IMAGE_ERROR_COUNT)

typedef enum ae_image_error_t
{
    #define N(x) x
    AE_IMAGE_ERROR_N
    #undef N
} \
    ae_image_error_t;

static const char* ae_image_error_name[] =
{
    #define N(x) #x
    AE_IMAGE_ERROR_N
    #undef N
};

AE_DECL const char* AE_CALL // get an error message for a given image operation.
ae_image_error_message(ae_image_error_t code, const char* filename);

/*
================================================================================
 * ~~ [ image struct ] ~~ *
--------------------------------------------------------------------------------
*/

typedef struct ae_image_t
{
    void *              pixels;         // pointer to pixel data buffer
    size_t              width, height;  // image dimensions in pixels
    ae_image_format_t   format;         // RGB or RGBA format, etc. enum
    ae_image_type_t     type;           // data type (bytes or floats)
    void *              _owner;         // implementation-specific data
} \
    ae_image_t;

static c_inline size_t ae_image_bytes(const ae_image_t* const image)
{
    return (ae_image_format_components[image->format] * image->width
                * image->height * ae_image_type_bytes[image->type]);
}

static c_inline size_t ae_image_pitch(const ae_image_t* const image)
{
    // images aren't horizontally padded for pow2 textures or caching,
    // so the stride of an image and its width are always the same...
    #if 0
    return ae_image_bytes(image) / image->height;
    #else
    return (ae_image_format_components[image->format] * image->width
                                * ae_image_type_bytes[image->type]);
    #endif
}

static void* ae_image_pixel(const ae_image_t* const image, size_t x, size_t y)
{
    // XXX this function is very slow - it's meant for reference only.
    // Note that a GL-style Y flip isn't performed (this is top-down).
    const size_t t = ae_image_type_bytes[image->type];
    const size_t c = ae_image_format_components[image->format];

    u8* p = (u8*)image->pixels + y * image->width * c * t + x * c * t;
    assert(ptr_in_array(p, image->pixels, ae_image_bytes(image)));

    return (void*)p;
}

/*
================================================================================
 * ~~ [ memory management ] ~~ *
--------------------------------------------------------------------------------
*/

/* manually allocate a pixel buffer for an image. the size of the buffer is
 * dependent upon the width, height, format and type members of the image.
 */
AE_DECL void AE_CALL ae_image_alloc(ae_image_t* image);

/* if dst and src don't have the same members and size, realloc and change dst to fit.
 * this may blow away the old data in dst - this is not an image resizing operation.
 */
AE_DECL void AE_CALL ae_image_alloc_fit(ae_image_t* dst, const ae_image_t* const src);

/* free pixel data allocated on the heap and clear struct members to zero.
 */
AE_DECL void AE_CALL ae_image_free(ae_image_t* image);

/* copy data from src image to dst. dst image is possibly reallocated to fit src.
 */
AE_DECL void AE_CALL ae_image_copy(ae_image_t* dst, const ae_image_t* const src);

/*
================================================================================
 * ~~ [ memory codecs ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL ae_image_error_t AE_CALL ae_image_info_from_memory(ae_image_t* image,
                                    const void* const buf, const size_t len);

AE_DECL ae_image_error_t AE_CALL ae_image_load_from_memory(ae_image_t* image,
                                    const void* const buf, const size_t len);

/*
================================================================================
 * ~~ [ file codecs ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL ae_image_error_t AE_CALL // get dimensions, format, and type of an image.
ae_image_info(ae_image_t* image, const char* filename);

AE_DECL ae_image_error_t AE_CALL // load and decompress image data from a file.
ae_image_load(ae_image_t* image, const char* filename);

AE_DECL ae_image_error_t AE_CALL // write image to disk. overwrites the old file.
ae_image_save(ae_image_t* image, const char* filename);

/*
================================================================================
 * ~~ [ image archives ] ~~ *
--------------------------------------------------------------------------------
This is an intermediate solution between the potential nastiness of dealing with
monolithic resource files (like Quake PAK files), and the potential nastiness and
slowness of dealing with loose files that might be scattered all over the disk.
--------------------------------------------------------------------------------
*/

/*  Load an archive created with ae_image_archive_save(_ex) in this manner:

        ae_image_t* images;
        ae_image_archive_load(&images, "test_image.jpg_archive");

    The buffer pointed to by images is allocated with ae_malloc, and should
    be freed with ae_free. The array is terminated by a zero-sized image.
    Counting might have been less awkward with ae_array_t, but here we are.
*/
AE_DECL ae_image_error_t AE_CALL
ae_image_archive_load(ae_image_t** images, const char* filename);

/*  Save a zero-size-image-terminated array of images to an archive. The first
    3 or more letters in the extension selects the codec to use for each image.
    For instance, "test_image.jpg_ex" or "test_image.png_image_lib" both work.
*/
AE_DECL ae_image_error_t AE_CALL
ae_image_archive_save(ae_image_t* images, const char* filename);

/*  Same as ae_image_archive_save, but uses images on the filesystem directly,
    instead of using our encoders. All images should be of the same file type.

    The list of filename strings must be terminated by a NULL entry.

    Note that for this function, as well as the rest of this library, a list
    of filenames to indices is not preserved (user must sync resource names).
    Keeping a master resource manifest file is one possible solution to this.
*/
AE_DECL ae_image_error_t AE_CALL
ae_image_archive_save_ex(const char** filenames, const char* archive_filename);

/*
================================================================================
 * ~~ [ data conversion ] ~~ *
--------------------------------------------------------------------------------
TODO: since these ops are out of place, they should be suffixed with _converted
--------------------------------------------------------------------------------
*/

/* convert the type of an image's color channels between 8-bit LDR and floating
 * point HDR. this is an out-of-place operation (source image is not modified).
 */
AE_DECL void AE_CALL ae_image_type_convert( ae_image_t* dst ,
    const ae_image_t* const src, const ae_image_type_t type);

/* convert the format of an image between RGB, RGBA, mono, etc. if possible.
 * this is also an out-of-place operation (source is copied, not modified).
 */
AE_DECL void AE_CALL ae_image_format_convert(ae_image_t* dst,
    const ae_image_t* const src, const ae_image_format_t fm);

/*
================================================================================
 * ~~ [ resizing ] ~~ *
--------------------------------------------------------------------------------
*/

#define AE_IMAGE_RESIZE_FILTER_N \
    N(AE_IMAGE_RESIZE_FILTER_DEFAULT), \
    N(AE_IMAGE_RESIZE_FILTER_BOX), \
    N(AE_IMAGE_RESIZE_FILTER_TRIANGLE), \
    N(AE_IMAGE_RESIZE_FILTER_CUBIC_B_SPLINE), \
    N(AE_IMAGE_RESIZE_FILTER_CATMULL_ROM), \
    N(AE_IMAGE_RESIZE_FILTER_MITCHELL), \
    N(AE_IMAGE_RESIZE_FILTER_COUNT)

typedef enum ae_image_resize_filter_t
{
    #define N(x) x
    AE_IMAGE_RESIZE_FILTER_N
    #undef N
} \
    ae_image_resize_filter_t;

static const char* ae_image_resize_filter_name[] =
{
    #define N(x) #x
    AE_IMAGE_RESIZE_FILTER_N
    #undef N
};

AE_DECL void AE_CALL
ae_image_resize_ex(ae_image_t* dst, const ae_image_t* const src,
                const size_t new_width, const size_t new_height,
                ae_image_resize_filter_t filter);

AE_DECL void AE_CALL
ae_image_resize(ae_image_t* dst, const ae_image_t* const src,
            const size_t new_width, const size_t new_height);

/*
================================================================================
 * ~~ [ unary operations ] ~~ *
--------------------------------------------------------------------------------
*/

typedef struct ae_image_unary_op_t
{
    const char* name;

    // thread pool job count. 0 issues the op immediately on the main thread,
    // for filter tasks that are not thread safe (like a matrix convolution).
    size_t num_tasks;

    void (*flt_func[5])(float* start, float* end, size_t comp, void* data);
    void (*u8_func[5])(u8* start, u8* end, size_t comp, void* data);
} \
    ae_image_unary_op_t;

/*  For a given rect region of an image (or the entire image if rect=NULL),
    apply a callback function to each scanline. Note that the func pointer
    table is indexed by the number of components in the image color format.
 */
AE_DECL void AE_CALL ae_image_unary_op(ae_image_t* image, int* rect,
                                ae_image_unary_op_t* op, void* data);

/*  Apply lookup tables to each pixel in the image rect. If a color channel's
    table is NULL, nothing is done on that channel (this makes cool effects).
 */
AE_DECL void AE_CALL ae_image_unary_clut(ae_image_t * image,
                int * rect, u8 * r, u8 * g, u8 * b, u8 * a);

/* ===== [ filters ] ======================================================== */

AE_DECL void AE_CALL
ae_image_negative(ae_image_t* image, int* rect, int r, int g, int b);

AE_DECL void AE_CALL
ae_image_solarize(ae_image_t* image, int* rect, u8 threshold, int r, int g, int b);

AE_DECL void AE_CALL
ae_image_greyscale(ae_image_t* image, int* rect, int r, int g, int b);

AE_DECL void AE_CALL
ae_image_threshold(ae_image_t* image, int* rect, u8 threshold, int r, int g, int b);

AE_DECL void AE_CALL
ae_image_bleach(ae_image_t* image, int* rect, u8 threshold, int r, int g, int b);

AE_DECL void AE_CALL
ae_image_neonize(ae_image_t* image, int* rect, u8 threshold, int lr, int lg, int lb,
                                                            int hr, int hg, int hb);

AE_DECL void AE_CALL
ae_image_pastelize(ae_image_t* image, int* rect, u8 threshold, int lr, int lg, int lb,
                                                            int hr, int hg, int hb);

AE_DECL void AE_CALL
ae_image_isolate_channel(ae_image_t* image, int* rect, int channel, int r, int g, int b);

AE_DECL void AE_CALL
ae_image_set_color( ae_image_t* image, int* rect, const float color[4],
                    int r, int g, int b, int a );

AE_DECL void AE_CALL
ae_image_scale_color(ae_image_t* image, int* rect, const float scale[4]);

AE_DECL void AE_CALL
ae_image_flip_x(ae_image_t* image, int* rect, int r, int g, int b, int a);

AE_DECL void AE_CALL
ae_image_flip_y(ae_image_t* image, int* rect, int r, int g, int b, int a);

/*
================================================================================
 * ~~ [ binary operations ] ~~ *
--------------------------------------------------------------------------------
*/

typedef struct ae_image_binary_op_t
{
    const char* name;

    // thread pool job count. 0 issues the op immediately on the main thread,
    // for tasks that read adjacent pixels and are therefore not thread safe.
    size_t num_tasks;

    void (*flt_func[5])(float* dst, float* src, size_t pixels, size_t comp, void* data);
    void (*u8_func[5])(u8* dst, u8* src, size_t pixels, size_t comp, void* data);
} \
    ae_image_binary_op_t;

AE_DECL void AE_CALL
ae_image_binary_op(ae_image_t * dst, const ae_image_t * const src, const int x,
                const int y, const ae_image_binary_op_t* const op, void* data);

AE_DECL void AE_CALL
ae_image_binary_copy(ae_image_t* dst, const ae_image_t* const src,
            const int x, const int y, int r, int g, int b, int a);

AE_DECL void AE_CALL
ae_image_lerp(ae_image_t * dst, const ae_image_t * const src, const int x,
                        const int y, float t, int r, int g, int b, int a);

/*
================================================================================
 * ~~ [ blitting ] ~~ *
--------------------------------------------------------------------------------
*/

#define AE_IMAGE_BLEND_FUNCTION_N \
    N(AE_IMAGE_BLEND_FUNCTION_ZERO), \
    N(AE_IMAGE_BLEND_FUNCTION_ONE), \
    N(AE_IMAGE_BLEND_FUNCTION_SRC_COLOR), \
    N(AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_COLOR), \
    N(AE_IMAGE_BLEND_FUNCTION_DST_COLOR), \
    N(AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_DST_COLOR), \
    N(AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA), \
    N(AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_ALPHA), \
    N(AE_IMAGE_BLEND_FUNCTION_DST_ALPHA), \
    N(AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_DST_ALPHA), \
    N(AE_IMAGE_BLEND_FUNCTION_CONSTANT_COLOR), \
    N(AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_CONSTANT_COLOR), \
    N(AE_IMAGE_BLEND_FUNCTION_CONSTANT_ALPHA), \
    N(AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_CONSTANT_ALPHA), \
    N(AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA_SATURATE), \
    N(AE_IMAGE_BLEND_FUNCTION_COUNT)

typedef enum ae_image_blend_function_t
{
    #define N(x) x
    AE_IMAGE_BLEND_FUNCTION_N
    #undef N
} \
    ae_image_blend_function_t;

static const char* ae_image_blend_function_name[] =
{
    #define N(x) #x
    AE_IMAGE_BLEND_FUNCTION_N
    #undef N
};

#define AE_IMAGE_BLEND_EQUATION_N \
    N(AE_IMAGE_BLEND_EQUATION_ADD), \
    N(AE_IMAGE_BLEND_EQUATION_SUBTRACT), \
    N(AE_IMAGE_BLEND_EQUATION_REVERSE_SUBTRACT), \
    N(AE_IMAGE_BLEND_EQUATION_MIN), \
    N(AE_IMAGE_BLEND_EQUATION_MAX), \
    N(AE_IMAGE_BLEND_EQUATION_COUNT)

typedef enum ae_image_blend_equation_t
{
    #define N(x) x
    AE_IMAGE_BLEND_EQUATION_N
    #undef N
} \
    ae_image_blend_equation_t;

static const char* ae_image_blend_equation_name[] =
{
    #define N(x) #x
    AE_IMAGE_BLEND_EQUATION_N
    #undef N
};

/*  Draw src into dst with blend modes that match OpenGL's (see the glBlendFunc(Separate)
    and glBlendEquation(Separate) manpages for more details, as well as an interactive
    visualization of the blending modes at http://www.andersriggelsen.dk/glblendfunc.php).

    Note that this operates differently than OpenGL when the blending equations are min or
    max. In GL, no blending is performed in this case, whereas it is here for ease of use.
    To emulate GL's behavior, use AE_IMAGE_BLEND_FUNCTION_ONE for all blending functions.

    Normal transparent images add SRC_ALPHA to ONE_MINUS_SRC_ALPHA (used by ae_image_blit).
    Premultiplied alpha images should use AE_IMAGE_BLEND_FUNCTION_ONE for src blend funcs.
*/
AE_DECL void AE_CALL ae_image_blit_ex(ae_image_t* dst, const ae_image_t* const src,
                                    const int x, const int y,
                                    const ae_image_blend_equation_t rgb_equation,
                                    const ae_image_blend_equation_t alpha_equation,
                                    const ae_image_blend_function_t src_rgb,
                                    const ae_image_blend_function_t src_alpha,
                                    const ae_image_blend_function_t dst_rgb,
                                    const ae_image_blend_function_t dst_alpha,
                                    const float * const constant,
                                    const float * const src_scale,
                                    const float * const dst_scale,
                                    int r, int g, int b, int a );

AE_DECL void AE_CALL ae_image_blit(ae_image_t* dst, const ae_image_t* const src,
                        const int x, const int y, int r, int g, int b, int a );

/* ===== [ unary rects ] ==================================================== */

AE_DECL void AE_CALL ae_image_blit_rect_ex( ae_image_t* image, int* rect,
                                    const ae_image_blend_equation_t rgb_equation,
                                    const ae_image_blend_equation_t alpha_equation,
                                    const ae_image_blend_function_t src_rgb,
                                    const ae_image_blend_function_t src_alpha,
                                    const ae_image_blend_function_t dst_rgb,
                                    const ae_image_blend_function_t dst_alpha,
                                    const float * const constant,
                                    const float * const src_scale,
                                    const float * const dst_scale,
                                    int r, int g, int b, int a );

AE_DECL void AE_CALL ae_image_blit_rect(ae_image_t * image, int * rect,
                            float * color, int r, int g, int b, int a);

/* ===== [ unary lines ] ==================================================== */

AE_DECL void AE_CALL
ae_image_blit_line_horizontal_ex(ae_image_t* image, int x, int y, int width, float* color);

AE_DECL void AE_CALL
ae_image_blit_line_vertical_ex(ae_image_t* image, int x, int y, int height, float* color);

AE_DECL void AE_CALL
ae_image_blit_line_horizontal(ae_image_t* image, int x1, int y, int x2, float* color);

AE_DECL void AE_CALL
ae_image_blit_line_vertical(ae_image_t* image, int x, int y1, int y2, float* color);

AE_DECL void AE_CALL
ae_image_blit_rect_outline(ae_image_t* image, int* rect, float* color);

/* Draw a rectangular grid outline into a region of an image. Origin gives the
 * position and scale of the first grid cell (bottom left corner), while num_x
 * and num_y give the number of grid cells to repeat across the X and y axes.
 * This is useful for building "structured art" for tools - sprite atlases etc.
 */
AE_DECL void AE_CALL ae_image_blit_grid_outline(ae_image_t* image, int* origin,
                        const size_t num_x, const size_t num_y, float* color);

/*
================================================================================
 * ~~ [ regions ] ~~ *
--------------------------------------------------------------------------------
*/

AE_DECL void AE_CALL // allocate dst to fit src, and copy it over
ae_image_get_region(ae_image_t* dst, ae_image_t* src, int* rect);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __AE_IMAGE_H__ */
