// =============================================================================
// stb_image.h
// =============================================================================

static float stbi__h2l_gamma_i = 1.0f/2.2f, stbi__h2l_scale_i = 1.0f;
static float stbi__l2h_gamma = 2.2f, stbi__l2h_scale = 1.0f;

#define stbi__float2int(x) ((int)(x))

STBIDEF stbi_uc *stbi_load_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)
{
    AE_STUB(); return NULL;
}

STBIDEF stbi_uc *stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp)
{
    AE_STUB(); return NULL;
}

STBIDEF float *stbi_loadf_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp, int req_comp)
{
    AE_STUB(); return NULL;
}

STBIDEF float *stbi_loadf(char const *filename, int *x, int *y, int *comp, int req_comp)
{
    AE_STUB(); return NULL;
}

STBIDEF int stbi_info_from_memory(stbi_uc const *buffer, int len, int *x, int *y, int *comp)
{
    AE_STUB(); return 0;
}

STBIDEF int stbi_info(char const *filename, int *x, int *y, int *comp)
{
    AE_STUB(); return 0;
}

STBIDEF int stbi_is_hdr_from_memory(stbi_uc const *buffer, int len)
{
    AE_STUB(); return 0;
}

STBIDEF int stbi_is_hdr(char const *filename)
{
    AE_STUB(); return 0;
}

STBIDEF const char *stbi_failure_reason(void)
{
    AE_STUB(); return "";
}

// =============================================================================
// stb_image_resize.h
// =============================================================================

STBIRDEF int stbir_resize_uint8_generic(const unsigned char *input_pixels , int input_w , int input_h , int input_stride_in_bytes ,
                                              unsigned char *output_pixels, int output_w, int output_h, int output_stride_in_bytes,
                                        int num_channels, int alpha_channel, int flags,
                                        stbir_edge edge_wrap_mode, stbir_filter filter, stbir_colorspace space, void *alloc_ctx)
{
    AE_STUB(); return 0;
}

STBIRDEF int stbir_resize_float_generic(const float *input_pixels , int input_w , int input_h , int input_stride_in_bytes ,
                                              float *output_pixels, int output_w, int output_h, int output_stride_in_bytes,
                                        int num_channels, int alpha_channel, int flags,
                                        stbir_edge edge_wrap_mode, stbir_filter filter, stbir_colorspace space, void *alloc_ctx)
{
    AE_STUB(); return 0;
}

// =============================================================================
// stb_image_write.h
// =============================================================================

STBIWDEF int stbi_write_bmp_to_func(stbi_write_func *func, void *context, int x, int y, int comp, const void *data)
{
    AE_STUB(); return 0;
}

STBIWDEF int stbi_write_bmp(char const *filename, int x, int y, int comp, const void *data)
{
    AE_STUB(); return 0;
}

STBIWDEF int stbi_write_hdr_to_func(stbi_write_func *func, void *context, int x, int y, int comp, const float *data)
{
    AE_STUB(); return 0;
}

STBIWDEF int stbi_write_hdr(char const *filename, int x, int y, int comp, const float *data)
{
    AE_STUB(); return 0;
}

STBIWDEF int stbi_write_png_to_func(stbi_write_func *func, void *context, int x, int y, int comp, const void *data, int stride_bytes)
{
    AE_STUB(); return 0;
}

STBIWDEF int stbi_write_png(char const *filename, int x, int y, int comp, const void *data, int stride_bytes)
{
    AE_STUB(); return 0;
}

STBIWDEF int stbi_write_tga_to_func(stbi_write_func *func, void *context, int x, int y, int comp, const void *data)
{
    AE_STUB(); return 0;
}

STBIWDEF int stbi_write_tga(char const *filename, int x, int y, int comp, const void *data)
{
    AE_STUB(); return 0;
}
