/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

const char* ae_image_error_message(ae_image_error_t code, const char* filename)
{
    AE_STUB(); return "ae_image not implemented";
}

void ae_image_alloc(ae_image_t* image)
{
    AE_STUB();
}

void ae_image_alloc_fit(ae_image_t* dst, const ae_image_t* const src)
{
    AE_STUB();
}

void ae_image_free(ae_image_t* image)
{
    AE_STUB();
}

void ae_image_copy(ae_image_t* dst, const ae_image_t* const src)
{
    AE_STUB();
}

ae_image_error_t
ae_image_info_from_memory(ae_image_t* img, const void* const buf, const size_t len)
{
    AE_STUB(); return AE_IMAGE_NO_CODEC;
}

ae_image_error_t
ae_image_load_from_memory(ae_image_t* img, const void* const buf, const size_t len)
{
    AE_STUB(); return AE_IMAGE_NO_CODEC;
}

ae_image_error_t ae_image_info(ae_image_t* image, const char* filename)
{
    AE_STUB(); return AE_IMAGE_NO_CODEC;
}

ae_image_error_t ae_image_load(ae_image_t* image, const char* filename)
{
    AE_STUB(); return AE_IMAGE_NO_CODEC;
}

ae_image_error_t ae_image_save(ae_image_t* image, const char* filename)
{
    AE_STUB(); return AE_IMAGE_NO_CODEC;
}

ae_image_error_t ae_image_archive_load(ae_image_t** images, const char* filename)
{
    AE_STUB(); return AE_IMAGE_NO_CODEC;
}

ae_image_error_t ae_image_archive_save(ae_image_t* images, const char* filename)
{
    AE_STUB(); return AE_IMAGE_NO_CODEC;
}

ae_image_error_t ae_image_archive_save_ex(const char** filenames,
                                    const char* archive_filename)
{
    AE_STUB(); return AE_IMAGE_NO_CODEC;
}

void ae_image_type_convert(ae_image_t* dst, const ae_image_t* const src,
                                            const ae_image_type_t type)
{
    AE_STUB();
}

void ae_image_format_convert(ae_image_t* dst, const ae_image_t* const src,
                                        const ae_image_format_t format)
{
    AE_STUB();
}

void ae_image_resize_ex(ae_image_t* dst, const ae_image_t* const src,
                    const size_t new_width, const size_t new_height,
                    ae_image_resize_filter_t filter)
{
    AE_STUB();
}

void ae_image_resize(ae_image_t* dst, const ae_image_t* const src,
                const size_t new_width, const size_t new_height )
{
    AE_STUB();
}

void ae_image_unary_op(ae_image_t* image, int* rect, ae_image_unary_op_t* op, void* data)
{
    AE_STUB();
}

void ae_image_unary_clut(ae_image_t* image, int* rect, u8* r, u8* g, u8* b, u8* a)
{
    AE_STUB();
}

void ae_image_unary_and(ae_image_t* image, int* rect, int r, int g, int b)
{
    AE_STUB();
}

void ae_image_unary_xor(ae_image_t* image, int* rect, int r, int g, int b)
{
    AE_STUB();
}

void ae_image_unary_or(ae_image_t* image, int* rect, int r, int g, int b)
{
    AE_STUB();
}

void ae_image_unary_add(ae_image_t* image, int* rect, int r, int g, int b)
{
    AE_STUB();
}

void ae_image_unary_mul(ae_image_t* image, int* rect, int r, int g, int b)
{
    AE_STUB();
}

void ae_image_negative(ae_image_t* image, int* rect, int r, int g, int b)
{
    AE_STUB();
}

void ae_image_solarize(ae_image_t* image, int* rect, u8 threshold, int r, int g, int b)
{
    AE_STUB();
}

void ae_image_greyscale(ae_image_t* image, int* rect, int r, int g, int b)
{
    AE_STUB();
}

void ae_image_threshold(ae_image_t* image, int* rect, u8 threshold, int r, int g, int b)
{
    AE_STUB();
}

void ae_image_bleach(ae_image_t* image, int* rect, u8 threshold, int r, int g, int b)
{
    AE_STUB();
}

void ae_image_neonize(ae_image_t * image, int * rect, u8 threshold,
                    int lr, int lg, int lb, int hr, int hg, int hb)
{
    AE_STUB();
}

void ae_image_pastelize(ae_image_t* image, int* rect, u8 threshold,
                    int lr, int lg, int lb, int hr, int hg, int hb)
{
    AE_STUB();
}

void ae_image_isolate_channel(ae_image_t* image, int* rect, int channel, int r, int g, int b)
{
    AE_STUB();
}

void ae_image_randomize(ae_image_t* image, int* rect, int r, int g, int b, int a)
{
    AE_STUB();
}

void ae_image_set_color(ae_image_t* image, int* rect, const float color[4],
                        int r, int g, int b, int a)
{
    AE_STUB();
}

void ae_image_scale_color(ae_image_t* image, int* rect, const float scale[4])
{
    AE_STUB();
}

void ae_image_flip_x(ae_image_t* image, int* rect, int r, int g, int b, int a)
{
    AE_STUB();
}

void ae_image_flip_y(ae_image_t* image, int* rect, int r, int g, int b, int a)
{
    AE_STUB();
}

void ae_image_binary_op(ae_image_t* dst, const ae_image_t* const src, const int x,
                    const int y, const ae_image_binary_op_t* const op, void* data)
{
    AE_STUB();
}

void ae_image_binary_copy(ae_image_t * dst, const ae_image_t * const src,
                    const int x, const int y, int r, int g, int b, int a)
{
    AE_STUB();
}

void ae_image_lerp(ae_image_t* dst, const ae_image_t* const src, const int x,
                            const int y, float t, int r, int g, int b, int a)
{
    AE_STUB();
}

void ae_image_blit_ex(  ae_image_t* dst, const ae_image_t* const src,
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
                        int r, int g, int b, int a  )
{
    AE_STUB();
}

void ae_image_blit(ae_image_t * dst, const ae_image_t * const src,
            const int x, const int y, int r, int g, int b, int a)
{
    AE_STUB();
}

void ae_image_blit_rect_ex( ae_image_t* image, int* rect,
                            const ae_image_blend_equation_t rgb_equation,
                            const ae_image_blend_equation_t alpha_equation,
                            const ae_image_blend_function_t src_rgb,
                            const ae_image_blend_function_t src_alpha,
                            const ae_image_blend_function_t dst_rgb,
                            const ae_image_blend_function_t dst_alpha,
                            const float * const constant,
                            const float * const src_scale,
                            const float * const dst_scale,
                            int r, int g, int b, int a  )
{
    AE_STUB();
}

void ae_image_blit_rect(ae_image_t * image, int * rect, float * color,
                        int r, int g, int b, int a)
{
    AE_STUB();
}

void ae_image_blit_line_horizontal_ex(ae_image_t* image,
                int x, int y, int width, float * color)
{
    AE_STUB();
}

void ae_image_blit_line_vertical_ex( ae_image_t* image,
                int x, int y, int height, float* color)
{
    AE_STUB();
}

void ae_image_blit_line_horizontal(ae_image_t* image,
                int x1, int y, int x2, float* color)
{
    AE_STUB();
}

void ae_image_blit_line_vertical( ae_image_t* image,
                int x, int y1, int y2, float* color)
{
    AE_STUB();
}

void ae_image_blit_rect_outline(ae_image_t* image, int* rect, float* color)
{
    AE_STUB();
}

void ae_image_blit_grid_outline(ae_image_t * image, int * origin,
            const size_t num_x, const size_t num_y, float* color)
{
    AE_STUB();
}

void ae_image_get_region(ae_image_t* dst, ae_image_t* src, int* rect)
{
    AE_STUB();
}

void ae_image_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_image_quit(void)
{
    // AE_STUB();
}
