/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
TODO: make sure we're dealing with sRGB and premultiplied alpha correctly here!
TODO: split codecs and other stuff into two separate files to stub unused code
TODO: define STBI_NO_STDIO and use memory versions of codecs for file versions
TODO: Adam's Raw Texture files; raw pixel data dump with info in 16-byte header
TODO: "barrel shifting" of images - pixels translated out of rects wrap around
TODO: rotate images around a given point, with optional bilinear interpolation
TODO: 16bit pixels - 5 bits per color channel (32 steps) and 1 transparency bit
TODO: alpha-blended angled line blitting using bresenham's midpoint algorithm
TODO: draw bezier curves with straight lines using in-out cubic easing methods
TODO: quantize images using user-provided palettes, generate palette for image
TODO: consider splitting IMAGE log category into IMAGE_CODEC and IMAGE_FILTER?
TODO: memory block is PNG, JPEG, GIF, TGA, TIF, PSD, PNM, RAW, block is image
TODO: asynchronous image load, with the same thread pool that filters will use
TODO: "colour" defines for limeys (this will ruin our beloved grep-ability!!!)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

const char* ae_image_error_message(ae_image_error_t code, const char* filename)
{
    // TODO: translation/localization for these & other internal error messages
    static char img_error[1024];

    // HACK that lets us use switch tracking with this func (count is misnamed)
    static const ae_image_error_t AE_IMAGE_COUNT = AE_IMAGE_ERROR_COUNT;

    ae_if (filename != NULL)
    {
        ae_switch (code, ae_image_error, AE_IMAGE, suffix)
        {
            case AE_IMAGE_NO_CODEC:
                AE_SNPRINTF(img_error, "no codec for image file \"%s\"", filename);
                break;

            case AE_IMAGE_FILE_NOT_FOUND:
                AE_SNPRINTF(img_error, "image file \"%s\" not found", filename);
                break;

            case AE_IMAGE_FILE_CORRUPT:
                AE_SNPRINTF(img_error, "image file \"%s\" is corrupt", filename);
                break;

            case AE_IMAGE_WRITE_FAILED:
                AE_SNPRINTF(img_error, "failed to save image \"%s\"", filename);
                break;

            case AE_IMAGE_SUCCESS: default:
                ae_assert(0, "%s %u", filename, (u32)code); return NULL;
        }
    }
    else
    {
        ae_switch (code, ae_image_error, AE_IMAGE, suffix)
        {
            case AE_IMAGE_NO_CODEC:
                strcpy(img_error, "no codec available for memory image"); break;

            case AE_IMAGE_FILE_CORRUPT:
                strcpy(img_error, "memory image is corrupt"); break;

            case AE_IMAGE_WRITE_FAILED:
                strcpy(img_error, "failed writing image file to memory"); break;

            case AE_IMAGE_SUCCESS: default:
                ae_assert(0, "%u", (u32)code); return NULL;
        }
    }

    return (const char*)img_error;
}

static ae_memory_heap_t ae_image_heap;

#define malloc(s)       ae_heap_malloc(&ae_image_heap, (s))
#define realloc(p, s)   ae_heap_realloc(&ae_image_heap, (p), (s))
#define free(p)         ae_heap_free(&ae_image_heap, (p))

// this cuts about 10k of code on MSVC9 x86 debug builds, at the cost of slightly
// slower jpeg loads (which doesn't matter for us, as we only use PNG in release).
#define STBI_NO_SIMD

// we only use the memory buffer versions of these, and do our own file handling.
// #define STBI_NO_STDIO
// #define STBI_WRITE_NO_STDIO

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include <stb_image_write.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_STATIC
#include <stb_image_resize.h>

#if !defined(STB_IMAGE_RESIZE_IMPLEMENTATION) || \
    !defined(STB_IMAGE_WRITE_IMPLEMENTATION) || \
    !defined(STB_IMAGE_IMPLEMENTATION)
    /*
     * NOTE: this is just for examining the properties of this codebase without
     * having to wade through the clutter of dozens of external lib functions.
     * eventually, someday, we should be using only in-house code for images...
     */
    #include <stb_image_stubs.c>
#endif

#undef malloc
#undef realloc
#undef free

/*
================================================================================
 * ~~ [ devil boilerplate ] ~~ *
--------------------------------------------------------------------------------
*/

static void *devil_handle, *ilu_handle;

#define _IL_FILE_OVERWRITE  0x0620
#define _IL_VERSION_NUM     0x0DE2
#define _IL_IMAGE_WIDTH     0x0DE4
#define _IL_IMAGE_HEIGHT    0x0DE5
#define _IL_IMAGE_FORMAT    0x0DEA
#define _IL_IMAGE_TYPE      0x0DEB
#define _IL_UNSIGNED_BYTE   0x1401
#define _IL_FLOAT           0x1406
#define _IL_RGB             0x1907
#define _IL_RGBA            0x1908
#define _IL_LUMINANCE       0x1909
#define _IL_VENDOR          0x1F00

#define _ILU_VERSION_NUM    _IL_VERSION_NUM
#define _ILU_VENDOR         _IL_VENDOR

#if (_MSC_VER >= 800)           || \
    defined(_STDCALL_SUPPORTED) || \
    defined(__BORLANDC__)       || \
    defined(__LCC__)            || \
    defined(__MINGW32__)
    #define _ILAPIENTRY __stdcall
#else
    #define _ILAPIENTRY
#endif

static c_inline ae_image_format_t devil_format_to_image(u32 format)
{
    switch (format)
    {
        case _IL_LUMINANCE: return AE_IMAGE_FORMAT_MONO;
        case _IL_RGB:       return AE_IMAGE_FORMAT_RGB;
        case _IL_RGBA:      return AE_IMAGE_FORMAT_RGBA;

        default: break;
    }

    ae_assert(0, "%u", (u32)format); return AE_IMAGE_FORMAT_COUNT;
}

static c_inline u32 image_format_to_devil(ae_image_format_t format)
{
    switch (format)
    {
        case AE_IMAGE_FORMAT_MONO:  return _IL_LUMINANCE;
        case AE_IMAGE_FORMAT_RGB:   return _IL_RGB;
        case AE_IMAGE_FORMAT_RGBA:  return _IL_RGBA;

        default: break;
    }

    ae_assert(0, "%u", (u32)format); return AE_IMAGE_FORMAT_COUNT;
}

static c_inline ae_image_type_t devil_type_to_image(u32 type)
{
    switch (type)
    {
        case _IL_FLOAT:         return AE_IMAGE_TYPE_FLT;
        case _IL_UNSIGNED_BYTE: return AE_IMAGE_TYPE_U8;

        default: break;
    }

    ae_assert(0, "%u", (u32)type); return AE_IMAGE_TYPE_COUNT;
}

static c_inline u32 image_type_to_devil(ae_image_type_t type)
{
    switch (type)
    {
        case AE_IMAGE_TYPE_U8:  return _IL_UNSIGNED_BYTE;
        case AE_IMAGE_TYPE_FLT: return _IL_FLOAT;

        default: break;
    }

    ae_assert(0, "%u", (u32)type); return AE_IMAGE_TYPE_COUNT;
}

static void* _ILAPIENTRY devil_malloc(const size_t size)
{
    return ae_heap_malloc(&ae_image_heap, size);
}

static void _ILAPIENTRY devil_free(void* ptr)
{
    ae_heap_free(&ae_image_heap, ptr);
}

/* ===== [ devil image library ] ============================================ */

typedef void* (_ILAPIENTRY *mAlloc)(const size_t);
typedef void  (_ILAPIENTRY *mFree )(void*);

typedef void (_ILAPIENTRY *IL_BIND_IMAGE_FUNC)(unsigned int);
static IL_BIND_IMAGE_FUNC IL_BindImage;

typedef void (_ILAPIENTRY *IL_CLEAR_COLOUR_FUNC)(float, float, float, float);
static IL_CLEAR_COLOUR_FUNC IL_ClearColour;

typedef unsigned char (_ILAPIENTRY *IL_CONVERT_IMAGE_FUNC)(unsigned int, unsigned int);
static IL_CONVERT_IMAGE_FUNC IL_ConvertImage;

typedef void (_ILAPIENTRY *IL_DELETE_IMAGE_FUNC)(unsigned int);
static IL_DELETE_IMAGE_FUNC IL_DeleteImage;

typedef unsigned int (_ILAPIENTRY *IL_DETERMINE_TYPE_L_FUNC)(const void*, unsigned int);
static IL_DETERMINE_TYPE_L_FUNC IL_DetermineTypeL;

typedef unsigned char (_ILAPIENTRY *IL_ENABLE_FUNC)(unsigned int);
static IL_ENABLE_FUNC IL_Enable;

typedef unsigned int (_ILAPIENTRY *IL_GEN_IMAGE_FUNC)(void);
static IL_GEN_IMAGE_FUNC IL_GenImage;

typedef u8* (_ILAPIENTRY *IL_GET_DATA_FUNC)(void);
static IL_GET_DATA_FUNC IL_GetData;

typedef unsigned int (_ILAPIENTRY *IL_GET_ERROR_FUNC)(void);
static IL_GET_ERROR_FUNC IL_GetError;

typedef int (_ILAPIENTRY *IL_GET_INTEGER_FUNC)(unsigned int);
static IL_GET_INTEGER_FUNC IL_GetInteger;

typedef const char* (_ILAPIENTRY *IL_GET_STRING_FUNC)(unsigned int);
static IL_GET_STRING_FUNC IL_GetString;

typedef void (_ILAPIENTRY *IL_INIT_FUNC)(void);
static IL_INIT_FUNC IL_Init;

typedef unsigned char (_ILAPIENTRY *IL_LOAD_L_FUNC)(unsigned int, const void*, unsigned int);
static IL_LOAD_L_FUNC IL_LoadL;

typedef unsigned char (_ILAPIENTRY *IL_LOAD_IMAGE_FUNC)(const char*);
static IL_LOAD_IMAGE_FUNC IL_LoadImage;

typedef unsigned char (_ILAPIENTRY *IL_SAVE_IMAGE_FUNC)(const char*);
static IL_SAVE_IMAGE_FUNC IL_SaveImage;

typedef void (_ILAPIENTRY *IL_SET_MEMORY_FUNC)(mAlloc, mFree);
static IL_SET_MEMORY_FUNC IL_SetMemory;

typedef void (_ILAPIENTRY *IL_SHUTDOWN_FUNC)(void);
static IL_SHUTDOWN_FUNC IL_ShutDown;

typedef unsigned char (_ILAPIENTRY *IL_TEX_IMAGE_FUNC)(unsigned int, unsigned int,
                unsigned int, unsigned char, unsigned int, unsigned int, void *);
static IL_TEX_IMAGE_FUNC IL_TexImage;

/* ===== [ image utility library ] ========================================== */

typedef unsigned char (_ILAPIENTRY *ILU_ALIENIFY_FUNC)(void);
static ILU_ALIENIFY_FUNC ILU_Alienify;

typedef unsigned char (_ILAPIENTRY *ILU_BLUR_AVG_FUNC)(unsigned int Iter);
static ILU_BLUR_AVG_FUNC ILU_BlurAvg;

typedef unsigned char (_ILAPIENTRY *ILU_BLUR_GAUSSIAN_FUNC)(unsigned int Iter);
static ILU_BLUR_GAUSSIAN_FUNC ILU_BlurGaussian;

typedef unsigned char (_ILAPIENTRY *ILU_CONTRAST_FUNC)(float Contrast);
static ILU_CONTRAST_FUNC ILU_Contrast;

typedef unsigned char (_ILAPIENTRY *ILU_EDGE_DETECT_E_FUNC)(void);
static ILU_EDGE_DETECT_E_FUNC ILU_EdgeDetectE;

typedef unsigned char (_ILAPIENTRY *ILU_EDGE_DETECT_P_FUNC)(void);
static ILU_EDGE_DETECT_P_FUNC ILU_EdgeDetectP;

typedef unsigned char (_ILAPIENTRY *ILU_EDGE_DETECT_S_FUNC)(void);
static ILU_EDGE_DETECT_S_FUNC ILU_EdgeDetectS;

typedef unsigned char (_ILAPIENTRY *ILU_EMBOSS_FUNC)(void);
static ILU_EMBOSS_FUNC ILU_Emboss;

typedef unsigned char (_ILAPIENTRY *ILU_EQUALIZE_FUNC)(void);
static ILU_EQUALIZE_FUNC ILU_Equalize;

typedef const char* (_ILAPIENTRY *ILU_ERROR_STRING_FUNC)(unsigned int);
static ILU_ERROR_STRING_FUNC ILU_ErrorString;

typedef unsigned char (_ILAPIENTRY *ILU_FLIP_IMAGE_FUNC)(void);
static ILU_FLIP_IMAGE_FUNC ILU_FlipImage;

typedef unsigned char (_ILAPIENTRY *ILU_GAMMA_CORRECT_FUNC)(float Gamma);
static ILU_GAMMA_CORRECT_FUNC ILU_GammaCorrect;

typedef int (_ILAPIENTRY *ILU_GET_INTEGER_FUNC)(unsigned int);
static ILU_GET_INTEGER_FUNC ILU_GetInteger;

typedef const char* (_ILAPIENTRY *ILU_GET_STRING_FUNC)(unsigned int);
static ILU_GET_STRING_FUNC ILU_GetString;

typedef void (_ILAPIENTRY *ILU_INIT_FUNC)(void);
static ILU_INIT_FUNC ILU_Init;

typedef unsigned char (_ILAPIENTRY *ILU_INVERT_ALPHA_FUNC)(void);
static ILU_INVERT_ALPHA_FUNC ILU_InvertAlpha;

typedef unsigned char (_ILAPIENTRY *ILU_NEGATIVE_FUNC)(void);
static ILU_NEGATIVE_FUNC ILU_Negative;

typedef unsigned char (_ILAPIENTRY *ILU_NOISIFY_FUNC)(float Tolerance);
static ILU_NOISIFY_FUNC ILU_Noisify;

typedef unsigned char (_ILAPIENTRY *ILU_PIXELIZE_FUNC)(unsigned int PixSize);
static ILU_PIXELIZE_FUNC ILU_Pixelize;

typedef unsigned char (_ILAPIENTRY *ILU_REPLACE_COLOUR_FUNC)(unsigned char Red,
                    unsigned char Green, unsigned char Blue, float Tolerance);
static ILU_REPLACE_COLOUR_FUNC ILU_ReplaceColour;

typedef unsigned char (_ILAPIENTRY *ILU_SATURATE_1F_FUNC)(float Saturation);
static ILU_SATURATE_1F_FUNC ILU_Saturate1f;

typedef unsigned char (_ILAPIENTRY *ILU_SATURATE_4F_FUNC)(float r,
                            float g, float b, float Saturation );
static ILU_SATURATE_4F_FUNC ILU_Saturate4f;

typedef unsigned char (_ILAPIENTRY *ILU_SCALE_ALPHA_FUNC)(float scale);
static ILU_SCALE_ALPHA_FUNC ILU_ScaleAlpha;

typedef unsigned char (_ILAPIENTRY *ILU_SCALE_COLOURS_FUNC)(float r, float g, float b);
static ILU_SCALE_COLOURS_FUNC ILU_ScaleColours;

typedef unsigned char (_ILAPIENTRY *ILU_SHARPEN_FUNC)(float Factor, unsigned int Iter);
static ILU_SHARPEN_FUNC ILU_Sharpen;

typedef unsigned char (_ILAPIENTRY *ILU_SWAP_COLOURS_FUNC)(void);
static ILU_SWAP_COLOURS_FUNC ILU_SwapColours;

typedef unsigned char (_ILAPIENTRY *ILU_WAVE_FUNC)(float Angle);
static ILU_WAVE_FUNC ILU_Wave;

/* ===== [ initialization ] ================================================= */

static void devil_init(int argc, char** argv)
{
    #if defined(_WIN32)
    if (devil_handle == NULL) { devil_handle = ae_library_open("DevIL.dll"); }
    #else
    if (devil_handle == NULL) { devil_handle = ae_library_open("libIL.so" ); }
    #endif
    if (devil_handle == NULL) { return; }

    #if defined(_WIN32)
    if (ilu_handle == NULL) { ilu_handle = ae_library_open("ILU.dll"  ); }
    #else
    if (ilu_handle == NULL) { ilu_handle = ae_library_open("libILU.so"); }
    #endif
    if (ilu_handle == NULL)
    {
        ae_library_close(devil_handle);
        devil_handle = NULL; return;
    }

    IL_BindImage = (IL_BIND_IMAGE_FUNC)ae_library_proc(devil_handle, "ilBindImage");
    IL_ClearColour = (IL_CLEAR_COLOUR_FUNC)ae_library_proc(devil_handle, "ilClearColour");
    IL_ConvertImage = (IL_CONVERT_IMAGE_FUNC)ae_library_proc(devil_handle, "ilConvertImage");
    IL_DeleteImage = (IL_DELETE_IMAGE_FUNC)ae_library_proc(devil_handle, "ilDeleteImage");
    IL_DetermineTypeL = (IL_DETERMINE_TYPE_L_FUNC)ae_library_proc(devil_handle, "ilDetermineTypeL");
    IL_Enable = (IL_ENABLE_FUNC)ae_library_proc(devil_handle, "ilEnable");
    IL_GenImage = (IL_GEN_IMAGE_FUNC)ae_library_proc(devil_handle, "ilGenImage");
    IL_GetData = (IL_GET_DATA_FUNC)ae_library_proc(devil_handle, "ilGetData");
    IL_GetError = (IL_GET_ERROR_FUNC)ae_library_proc(devil_handle, "ilGetError");
    IL_GetInteger = (IL_GET_INTEGER_FUNC)ae_library_proc(devil_handle, "ilGetInteger");
    IL_GetString = (IL_GET_STRING_FUNC)ae_library_proc(devil_handle, "ilGetString");
    IL_Init = (IL_INIT_FUNC)ae_library_proc(devil_handle, "ilInit");
    IL_LoadL = (IL_LOAD_L_FUNC)ae_library_proc(devil_handle, "ilLoadL");
    IL_LoadImage = (IL_LOAD_IMAGE_FUNC)ae_library_proc(devil_handle, "ilLoadImage");
    IL_SaveImage = (IL_SAVE_IMAGE_FUNC)ae_library_proc(devil_handle, "ilSaveImage");
    IL_SetMemory = (IL_SET_MEMORY_FUNC)ae_library_proc(devil_handle, "ilSetMemory");
    IL_ShutDown = (IL_SHUTDOWN_FUNC)ae_library_proc(devil_handle, "ilShutDown");
    IL_TexImage = (IL_TEX_IMAGE_FUNC)ae_library_proc(devil_handle, "ilTexImage");

    ILU_Alienify = (ILU_ALIENIFY_FUNC)ae_library_proc(ilu_handle, "iluAlienify");
    ILU_BlurAvg = (ILU_BLUR_AVG_FUNC)ae_library_proc(ilu_handle, "iluBlurAvg");
    ILU_BlurGaussian = (ILU_BLUR_GAUSSIAN_FUNC)ae_library_proc(ilu_handle, "iluBlurGaussian");
    ILU_Contrast = (ILU_CONTRAST_FUNC)ae_library_proc(ilu_handle, "iluContrast");
    ILU_EdgeDetectE = (ILU_EDGE_DETECT_E_FUNC)ae_library_proc(ilu_handle, "iluEdgeDetectE");
    ILU_EdgeDetectP = (ILU_EDGE_DETECT_P_FUNC)ae_library_proc(ilu_handle, "iluEdgeDetectP");
    ILU_EdgeDetectS = (ILU_EDGE_DETECT_S_FUNC)ae_library_proc(ilu_handle, "iluEdgeDetectS");
    ILU_Emboss = (ILU_EMBOSS_FUNC)ae_library_proc(ilu_handle, "iluEmboss");
    ILU_Equalize = (ILU_EQUALIZE_FUNC)ae_library_proc(ilu_handle, "iluEqualize");
    ILU_ErrorString = (ILU_ERROR_STRING_FUNC)ae_library_proc(ilu_handle, "iluErrorString");
    ILU_FlipImage = (ILU_FLIP_IMAGE_FUNC)ae_library_proc(ilu_handle, "iluFlipImage");
    ILU_GammaCorrect = (ILU_GAMMA_CORRECT_FUNC)ae_library_proc(ilu_handle, "iluGammaCorrect");
    ILU_GetInteger = (ILU_GET_INTEGER_FUNC)ae_library_proc(ilu_handle, "iluGetInteger");
    ILU_GetString = (ILU_GET_STRING_FUNC)ae_library_proc(ilu_handle, "iluGetString");
    ILU_Init = (ILU_INIT_FUNC)ae_library_proc(ilu_handle, "iluInit");
    ILU_InvertAlpha = (ILU_INVERT_ALPHA_FUNC)ae_library_proc(ilu_handle, "iluInvertAlpha");
    ILU_Negative = (ILU_NEGATIVE_FUNC)ae_library_proc(ilu_handle, "iluNegative");
    ILU_Noisify = (ILU_NOISIFY_FUNC)ae_library_proc(ilu_handle, "iluNoisify");
    ILU_Pixelize = (ILU_PIXELIZE_FUNC)ae_library_proc(ilu_handle, "iluPixelize");
    ILU_ReplaceColour = (ILU_REPLACE_COLOUR_FUNC)ae_library_proc(ilu_handle, "iluReplaceColour");
    ILU_Saturate1f = (ILU_SATURATE_1F_FUNC)ae_library_proc(ilu_handle, "iluSaturate1f");
    ILU_Saturate4f = (ILU_SATURATE_4F_FUNC)ae_library_proc(ilu_handle, "iluSaturate4f");
    ILU_ScaleAlpha = (ILU_SCALE_ALPHA_FUNC)ae_library_proc(ilu_handle, "iluScaleAlpha");
    ILU_ScaleColours = (ILU_SCALE_COLOURS_FUNC)ae_library_proc(ilu_handle, "iluScaleColours");
    ILU_Sharpen = (ILU_SHARPEN_FUNC)ae_library_proc(ilu_handle, "iluSharpen");
    ILU_SwapColours = (ILU_SWAP_COLOURS_FUNC)ae_library_proc(ilu_handle, "iluSwapColours");
    ILU_Wave = (ILU_WAVE_FUNC)ae_library_proc(ilu_handle, "iluWave");

    IL_SetMemory((mAlloc)devil_malloc, (mFree)devil_free);
    IL_Init();
    ILU_Init();
    IL_Enable(_IL_FILE_OVERWRITE);

    ae_log(IMAGE, "using %s by %s", // DevIL version and vendor info
        IL_GetString(_IL_VERSION_NUM), IL_GetString(_IL_VENDOR));

    ae_log(IMAGE, "using %s by %s", // utils version and vendor info
        ILU_GetString(_ILU_VERSION_NUM), ILU_GetString(_ILU_VENDOR));
}

static void devil_quit(void)
{
    if (devil_handle)
    {
        IL_ShutDown(); // dealloc leaks

        ae_library_close(ilu_handle);
        ilu_handle = NULL;

        ae_library_close(devil_handle);
        devil_handle = NULL;
    }
}

/*
================================================================================
 * ~~ [ memory management ] ~~ *
--------------------------------------------------------------------------------
*/

void ae_image_alloc(ae_image_t* image)
{
    ae_image_t old = *image;
    ae_image_free(image);

    image->pixels = ae_heap_malloc(&ae_image_heap, ae_image_bytes(&old));
    image->width = old.width;
    image->height = old.height;
    image->format = old.format;
    image->type = old.type;
}

void ae_image_alloc_fit(ae_image_t* dst, const ae_image_t* const src)
{
    ae_assert(dst && src, "dst img: %p, src img: %p", dst, src);

    if (dst->width  != src->width   ||
        dst->height != src->height  ||
        dst->format != src->format  ||
        dst->type   != src->type    ||
        dst->pixels == NULL)
    {
        const size_t src_bytes = ae_image_bytes(src); // dst size
        ae_image_free(dst);

        dst->width = src->width;
        dst->height = src->height;
        dst->format = src->format;
        dst->type = src->type;

        ae_assert(ae_image_bytes(dst) == src_bytes, "%u != %u!",
            (unsigned)ae_image_bytes(dst), (unsigned)src_bytes);

        dst->pixels = ae_heap_malloc(&ae_image_heap, src_bytes);
    }
}

void ae_image_free(ae_image_t* image)
{
    ae_assert(image != NULL, "null struct pointer");

    /* NOTE: If DevIL (and therefore aecore) has been shut down before this image
     * was freed here, then DevIL has already purged this leaked pixel memory...
     * Python should not be freeing things after atexit() functions are executed!
     */
    if (image->_owner && devil_handle)
    {
        IL_DeleteImage((u32)(size_t) image->_owner);
    }
    else
    {
        ae_heap_free(&ae_image_heap, image->pixels);
    }

    memset(image, 0, sizeof(ae_image_t));
}

void ae_image_copy(ae_image_t* dst, const ae_image_t* const src)
{
    // keep track of the number of times we make this call superfluously
    void* outer = ae_profile_enter(__FILE__, "ae_image_copy_outer");

    if (src != dst)
    {
        void* inner = ae_profile_enter(__FILE__, "ae_image_copy_inner");

        ae_image_alloc_fit(dst, src); // ensure dst is alloced
        memcpy(dst->pixels, src->pixels, ae_image_bytes(src));

        ae_profile_leave(inner);
    }

    ae_profile_leave(outer);
}

/* ===== [ internal temp copies ] =========================================== */

static void ae_image_alloc_temp(ae_image_t* image, size_t width, size_t height,
                                ae_image_type_t type, ae_image_format_t format)
{
    image->width = width;
    image->height = height;
    image->format = format;
    image->type = type;

    image->pixels = ae_stack_malloc(ae_global_stack(), ae_image_bytes(image));
}

static void ae_image_free_temp(ae_image_t* image)
{
    ae_stack_free(ae_global_stack(), image->pixels, ae_image_bytes(image));
}

/*
================================================================================
 * ~~ [ memory codecs ] ~~ *
--------------------------------------------------------------------------------
TODO: save (append) an image to array; see ae_image_archive_save for an example
--------------------------------------------------------------------------------
*/

ae_image_error_t
ae_image_info_from_memory(ae_image_t* img, const void* const buf, const size_t len)
{
    AE_STUB(); return AE_IMAGE_NO_CODEC;
}

ae_image_error_t
ae_image_load_from_memory(ae_image_t* img, const void* const buf, const size_t len)
{
    AE_PROFILE_ENTER();

    ae_image_error_t error = AE_IMAGE_SUCCESS;
    int w = 0, h = 0, comp = 4;

    ae_image_free(img);

    if (stbi_is_hdr_from_memory((stbi_uc const*)buf, (int)len))
    {
        img->pixels = (void*)stbi_loadf_from_memory((stbi_uc const*)buf,
                                            (int)len, &w, &h, &comp, 0);
        img->type = AE_IMAGE_TYPE_FLT;
    }
    else
    {
        img->pixels = (void*)stbi_load_from_memory( (stbi_uc const*)buf,
                                            (int)len, &w, &h, &comp, 0);
    }

    img->width  = (size_t)w;
    img->height = (size_t)h;

    switch (comp)
    {
        case 1: img->format = AE_IMAGE_FORMAT_MONO; break;
        case 3: img->format = AE_IMAGE_FORMAT_RGB; break;
        case 4: img->format = AE_IMAGE_FORMAT_RGBA; break;

        default: assert(0); break;
    }

    if (img->pixels == NULL)
    {
        if (!strcmp(stbi_failure_reason(), "unknown image type"))
        {
            error = AE_IMAGE_NO_CODEC;
        }
        else
        {
            error = AE_IMAGE_FILE_CORRUPT;
        }

        ae_log(IMAGE, "failed to load image from memory in %s: %s",
                            __FUNCTION__, stbi_failure_reason());
    }

    AE_PROFILE_LEAVE(); return error;
}

/*
================================================================================
 * ~~ [ file codecs ] ~~ *
--------------------------------------------------------------------------------
*/

ae_image_error_t ae_image_info(ae_image_t* image, const char* filename)
{
    AE_PROFILE_ENTER();

    int x, y, comp; // get image dimensions and channel count
    ae_image_free(image);

    // FIXME: this could also mean file corruption or no codec
    if (!stbi_info(filename, &x, &y, &comp))
    {
        AE_PROFILE_LEAVE(); return AE_IMAGE_FILE_NOT_FOUND;
    }

    image->width  = (size_t)x;
    image->height = (size_t)y;

    switch (comp)
    {
        case 1: image->format = AE_IMAGE_FORMAT_MONO; break;
        case 3: image->format = AE_IMAGE_FORMAT_RGB; break;
        case 4: image->format = AE_IMAGE_FORMAT_RGBA; break;

        default: assert(0); break;
    }

    // TODO: see what utils DevIL has for parsing image memory
    if (stbi_is_hdr(filename)) image->type = AE_IMAGE_TYPE_FLT;

    AE_PROFILE_LEAVE(); return AE_IMAGE_SUCCESS;
}

/* ===== [ load (decode) ] ================================================== */

static ae_image_error_t devil_image_load(ae_image_t* image, const char* filename)
{
    if (devil_handle == NULL) { return AE_IMAGE_NO_CODEC; } else
    {
        AE_PROFILE_ENTER();

        u32 handle = IL_GenImage(); // generate an image id and make it current
        if (handle) { IL_BindImage(handle); } else { goto error; }

        if (!IL_LoadImage(filename)) goto error; // load image data to buffer

        image->pixels = IL_GetData();
        image->width  = IL_GetInteger(_IL_IMAGE_WIDTH);
        image->height = IL_GetInteger(_IL_IMAGE_HEIGHT);
        image->format = devil_format_to_image(IL_GetInteger(_IL_IMAGE_FORMAT));
        image->type   = devil_type_to_image(IL_GetInteger(_IL_IMAGE_TYPE));
        image->_owner = (void*)(size_t)handle;

        ae_log(IMAGE, "loaded \"%s\" in %s", filename, __FUNCTION__);
        AE_PROFILE_LEAVE(); return AE_IMAGE_SUCCESS;

        error:
        {
            ae_log(IMAGE, "failed to load \"%s\" in %s: %s", filename,
                        __FUNCTION__, ILU_ErrorString(IL_GetError()));

            AE_PROFILE_LEAVE(); return AE_IMAGE_NO_CODEC;
        }
    }
}

static ae_image_error_t stb_image_load(ae_image_t* image, const char* filename)
{
    AE_PROFILE_ENTER();

    ae_image_error_t error = AE_IMAGE_SUCCESS;
    int w = 0, h = 0, comp = 4;

    if (stbi_is_hdr(filename))
    {
        image->pixels = (void*)stbi_loadf(filename, &w, &h, &comp, 0);
        image->type = AE_IMAGE_TYPE_FLT;
    }
    else
    {
        image->pixels = (void*)stbi_load(filename, &w, &h, &comp, 0);
    }

    image->width  = (size_t)w;
    image->height = (size_t)h;

    switch (comp)
    {
        case 1: image->format = AE_IMAGE_FORMAT_MONO; break;
        case 3: image->format = AE_IMAGE_FORMAT_RGB; break;
        case 4: image->format = AE_IMAGE_FORMAT_RGBA; break;

        default: assert(0); break;
    }

    if (image->pixels == NULL)
    {
        ae_log(IMAGE, "failed to load \"%s\" in %s: %s", filename,
                            __FUNCTION__, stbi_failure_reason());

        if (!strcmp(stbi_failure_reason(), "unknown image type"))
        {
            error = AE_IMAGE_NO_CODEC;
        }
        else if (!strcmp(stbi_failure_reason(), "can't fopen"))
        {
            error = AE_IMAGE_FILE_NOT_FOUND;
        }
        else
        {
            error = AE_IMAGE_FILE_CORRUPT;
        }
    }
    else
    {
        ae_log(IMAGE, "loaded \"%s\" in %s", filename, __FUNCTION__);
    }

    AE_PROFILE_LEAVE(); return error;
}

ae_image_error_t ae_image_load(ae_image_t* image, const char* filename)
{
    ae_image_free(image);

    #if defined(AE_FILE_TRACE) // HACK emit file read messages
    if (ae_log_is_enabled(AE_LOG_CATEGORY_FILE))
    {
        ae_file_exists(filename);
    }
    #endif

    if (devil_image_load(image, filename) == AE_IMAGE_SUCCESS)
    {
        return AE_IMAGE_SUCCESS;
    }

    return stb_image_load(image, filename);
}

/* ===== [ save (encode) ] ================================================== */

static ae_image_error_t devil_image_save(ae_image_t* image, const char* filename)
{
    if (!devil_handle) return AE_IMAGE_NO_CODEC; // attempt fast path
    if (image->_owner)
    {
        IL_BindImage((u32)(size_t)image->_owner);
        if (!IL_SaveImage(filename)) goto error;
    }
    else
    {
        u32 c = ae_image_format_components[image->format];
        u32 t = image_type_to_devil(image->type);
        u32 f = image_format_to_devil(image->format);
        u32 w = image->width, h = image->height;
        u32 i = IL_GenImage();

        if (i) { IL_BindImage(i); } else { goto error; }
        if (!IL_TexImage(w, h, 1, c, f, t, image->pixels)) goto error;
        if (!ILU_FlipImage()) goto error;
        if (!IL_SaveImage(filename)) goto error;

        IL_DeleteImage(i);
    }

    ae_log(IMAGE, "saved \"%s\" in %s", filename, __FUNCTION__);

    return AE_IMAGE_SUCCESS;
    error:
    {
        ae_log(IMAGE, "failed to save \"%s\" in %s: %s", filename,
                    __FUNCTION__, ILU_ErrorString(IL_GetError()));

        return AE_IMAGE_NO_CODEC;
    }
}

static ae_image_error_t // these might call into each other
stb_image_save_u8(ae_image_t* image, const char* filename);

#define STBI_SAVE_LOG_AND_RETURN                                                \
    if (err == AE_IMAGE_SUCCESS)                                                \
    {                                                                           \
        ae_log(IMAGE, "saved \"%s\" in %s", filename, __FUNCTION__);            \
    }                                                                           \
    else                                                                        \
    {                                                                           \
        ae_log(IMAGE, "failed to save \"%s\" in %s", filename, __FUNCTION__);   \
    }                                                                           \
                                                                                \
    return err;

static ae_image_error_t stb_image_save_flt(ae_image_t* image, const char* filename)
{
    ae_image_error_t err = AE_IMAGE_SUCCESS;

    int c = (int)ae_image_format_components[image->format];
    int w = (int)image->width, h = (int)image->height;

    if (strstr(filename, ".hdr"))
    {
        if (!stbi_write_hdr(filename, w, h, c, (float*)image->pixels))
            err = AE_IMAGE_WRITE_FAILED;
    }
    else
    {
        ae_image_t temp;

        ae_image_alloc_temp(&temp, image->width, image->height,
                            AE_IMAGE_TYPE_U8, image->format );

        ae_image_type_convert(&temp, image, AE_IMAGE_TYPE_U8);
        err = stb_image_save_u8(&temp, filename);

        ae_image_free_temp(&temp);
    }

    STBI_SAVE_LOG_AND_RETURN;
}

static ae_image_error_t stb_image_save_u8(ae_image_t* image, const char* filename)
{
    ae_image_error_t err = AE_IMAGE_SUCCESS;

    int c = (int)ae_image_format_components[image->format];
    int w = (int)image->width, h = (int)image->height;

    if (strstr(filename, ".hdr"))
    {
        ae_image_t temp;

        ae_image_alloc_temp(&temp, image->width, image->height,
                            AE_IMAGE_TYPE_FLT, image->format );

        ae_image_type_convert(&temp, image, AE_IMAGE_TYPE_FLT);
        err = stb_image_save_flt(&temp, filename);

        ae_image_free_temp(&temp);
    }
    else
    {
        /* TODO: stbi_write_jpg (func to get/set global quality?)
         */
        if      (strstr(filename, ".png"))
        {
            if (!stbi_write_png(filename, w, h, c, image->pixels, 0))
                err = AE_IMAGE_WRITE_FAILED;
        }
        else if (strstr(filename, ".bmp"))
        {
            if (!stbi_write_bmp(filename, w, h, c, image->pixels))
                err = AE_IMAGE_WRITE_FAILED;
        }
        else if (strstr(filename, ".tga"))
        {
            if (!stbi_write_tga(filename, w, h, c, image->pixels))
                err = AE_IMAGE_WRITE_FAILED;
        }
        else
        {
            err = AE_IMAGE_NO_CODEC;
        }
    }

    STBI_SAVE_LOG_AND_RETURN;
}

ae_image_error_t ae_image_save(ae_image_t* image, const char* filename)
{
    if (image == NULL || ae_image_bytes(image) == 0)
    {
        AE_WARN("saved invalid image at %p to \"%s\"", image, filename);
        return AE_IMAGE_SUCCESS;
    }

    if (devil_image_save(image, filename) == AE_IMAGE_SUCCESS)
    {
        return AE_IMAGE_SUCCESS;
    }

    if (image->type == AE_IMAGE_TYPE_FLT)
    {
        return stb_image_save_flt(image, filename);
    }
    else
    {
        return stb_image_save_u8(image, filename);
    }
}

/*
================================================================================
 * ~~ [ image archives ] ~~ *
--------------------------------------------------------------------------------
TODO: this assumes we're on a little-endian (x86) architecture, swap if we aren't
TODO: expose a function to load one image from a memory block (and use it here?)
TODO: read files incrementally to reduce temp memory usage (if it isn't slower)
TODO: expose a lower-level load function that loads an index from archive memory
TODO: use stbi_is_hdr_from_memory and stbi_loadf_from_memory in archive loading
--------------------------------------------------------------------------------
*/

ae_image_error_t ae_image_archive_load(ae_image_t** images, const char* filename)
{
    AE_PROFILE_ENTER();

    ae_image_error_t err = AE_IMAGE_SUCCESS;
    ae_image_t* image;

    size_t a_size, protocol_v, i, num_images = 0; // read archive into the stack
    void* archive = ae_file_read_stack(ae_global_stack(), &a_size, filename, 0);

    // if we encounter an error, we shouldn't leave this pointing to stack junk
    assert(images); *images = NULL;

    if (archive == NULL)
    {
        err = AE_IMAGE_FILE_NOT_FOUND; goto done;
    }

    protocol_v = (size_t) ((u16*)archive)[0]; (void)sizeof( protocol_v );
    num_images = (size_t) ((u16*)archive)[1]; // protocol val is unused

    // allocate extra space for the sentinel image (all members are zero)
    *images = (ae_image_t*)ae_calloc(num_images + 1, sizeof(ae_image_t));

    for (image = *images, i = 0; i < num_images; image++, i++)
    {
        int w, h, comp;

        u32 next_offset = i == num_images - 1 ? a_size : ((u32*)archive)[2 + i];
        u32 offset = ((u32*)archive)[1 + i]; // get img buffer size from offsets

    #if 0
        if (devil_handle) // for whatever reason, this was ~5x slower on a png benchmark
        {
            image->_owner = (void*)(size_t)IL_GenImage();

            if (image->_owner)
            {
                u8* lump = (u8*)archive + offset;
                u32 size = next_offset - offset;

                IL_BindImage((u32)(size_t)image->_owner);

                if (IL_LoadL(IL_DetermineTypeL(lump, size), lump, size))
                {
                    image->pixels = IL_GetData();

                    image->width  = IL_GetInteger(_IL_IMAGE_WIDTH );
                    image->height = IL_GetInteger(_IL_IMAGE_HEIGHT);

                    image->format = devil_format_to_image(IL_GetInteger(_IL_IMAGE_FORMAT));
                    image->type   = devil_type_to_image(IL_GetInteger(_IL_IMAGE_TYPE));

                    continue;
                }
                else
                {
                    IL_DeleteImage( (u32) (size_t)image->_owner );
                    image->_owner = NULL; /* TODO: log failure */
                }
            }
        }
    #endif

        image->pixels = (void*)stbi_load_from_memory( // TODO: try ilLoadL first
            (u8*)archive + offset, (int)(next_offset-offset), &w, &h, &comp, 0);

        if (image->pixels == NULL)
        {
            err = AE_IMAGE_FILE_CORRUPT; goto done;
        }

        image->width  = (size_t)w;
        image->height = (size_t)h;

        switch (comp)
        {
            case 1: image->format = AE_IMAGE_FORMAT_MONO; break;
            case 3: image->format = AE_IMAGE_FORMAT_RGB; break;
            case 4: image->format = AE_IMAGE_FORMAT_RGBA; break;

            default: assert(0); break;
        }
    }

    done:
    {
        // if loading fails at any stage, free the partial decode
        if (err != AE_IMAGE_SUCCESS && *images)
        {
            for (image = *images; ae_image_bytes(image); image++)
            {
                ae_image_free(image);
            }

            ae_free(*images); *images = NULL;
        }

        ae_log( IMAGE, "%s from \"%s\": %s", __FUNCTION__,
                    filename, ae_image_error_name[ err ]);

        ae_stack_free(ae_global_stack(), archive, a_size);

        // TODO: if loading fails, we should log stb/il messages
        AE_PROFILE_LEAVE(); return err;
    }
}

static void ae_stbi_write_func(void* context, void* data, int sz)
{
    ae_array_t* archive = (ae_array_t*)context;
    ae_array_append(archive, data, (size_t)sz);
}

ae_image_error_t ae_image_archive_save(ae_image_t* images, const char* filename)
{
    // ===== [ locals ] ========================================================

    ae_image_error_t err = AE_IMAGE_SUCCESS;
    ae_image_t* image;
    size_t num_images, i;

    ae_array_t archive; // we build a file buffer in memory before writing.
    memset(&archive, 0, sizeof(ae_array_t));

    // ===== [ allocate and write header ] =====================================

    for (image = images; ae_image_bytes(image); image++) {}
    num_images = (size_t)(image - images); // count images

    // u16 archive encoder protocol, u16 image count, and u32 image offsets
    // (these are absolute offsets from the beginning of the entire file).
    ae_array_resize(&archive, 2 + 2 + 4 * num_images);

    // we assume here that we're not archiving a ridiculously massive set.
    // TODO: maybe protocol 1 could LZ compress at the end, will it help?
    ((u16*)archive.data)[0] = 0;
    ((u16*)archive.data)[1] = (u16)num_images;

    // ===== [ encode images ] =================================================

    for (image = images, i = 0; i < num_images; image++, i++)
    {
        ae_assert(image->type == AE_IMAGE_TYPE_U8, "TODO: %ux%u image with type %s",
            (u32)image->width, (u32)image->height, ae_image_type_name[image->type]);

        // set the current image's offset in the header
        ((u32*)archive.data)[1 + i] = (u32)archive.size;

        if      (strstr(filename, ".png"))
        {
            /* TODO from the stb_image_write v1.02 docs:

                "The PNG output is not optimal; it is 20-50% larger than the file
                written by a decent optimizing implementation. This library is
                designed for source code compactness and simplicity, not optimal
                image file size or run-time performance."

                The workaround for this currently is to build straight from files.
                In the future, we should try using ilSaveL or some offline tool.
            */
            if(!stbi_write_png_to_func(
                ae_stbi_write_func,
                &archive,
                (int)image->width,
                (int)image->height,
                (int)ae_image_format_components[image->format],
                image->pixels, 0))
            {
                err = AE_IMAGE_WRITE_FAILED; goto done;
            }
        }
        else if (strstr(filename, ".bmp"))
        {
            if(!stbi_write_bmp_to_func(
                ae_stbi_write_func,
                &archive,
                (int)image->width,
                (int)image->height,
                (int)ae_image_format_components[image->format],
                image->pixels))
            {
                err = AE_IMAGE_WRITE_FAILED; goto done;
            }
        }
        else if (strstr(filename, ".tga"))
        {
            if(!stbi_write_tga_to_func(
                ae_stbi_write_func,
                &archive,
                (int)image->width,
                (int)image->height,
                (int)ae_image_format_components[image->format],
                image->pixels))
            {
                err = AE_IMAGE_WRITE_FAILED; goto done;
            }
        }
        else
        {
            err = AE_IMAGE_NO_CODEC; goto done;
        }
    }

    // ===== [ write to disk ] =================================================

    if (!ae_file_write(filename, archive.data, archive.size, 0))
    {
        err = AE_IMAGE_WRITE_FAILED; goto done;
    }

    done:
    {
        ae_log( IMAGE, "%s to \"%s\": %s", __FUNCTION__,
                    filename, ae_image_error_name[err]);

        ae_array_free(&archive); return err;
    }
}

ae_image_error_t ae_image_archive_save_ex(const char** filenames,
                                    const char* archive_filename)
{
    ae_image_error_t err = AE_IMAGE_SUCCESS;
    size_t num_images, i;

    ae_array_t archive;
    memset(&archive, 0, sizeof(ae_array_t));

    // first pass over the filenames to get their count (null terminated)
    for (num_images = 0; filenames[num_images]; num_images++) {}

    ae_array_resize(&archive, 2 + 2 + 4 * num_images);
    ((u16*)archive.data)[0] = 0;
    ((u16*)archive.data)[1] = (u16)num_images;

    for (i = 0; i < num_images; i++) // TODO: ensure all extensions match
    {
        ae_assert(filenames[i], "out of bounds %u", (u32)i);

        // set the current image's offset in the header
        ((u32*)archive.data)[1 + i] = (u32)archive.size;

        if (!ae_file_read_array(&archive, filenames[i], 0))
        {
            err = AE_IMAGE_FILE_NOT_FOUND; goto done;
        }
    }

    if (!ae_file_write(archive_filename, archive.data, archive.size, 0))
    {
        err = AE_IMAGE_WRITE_FAILED; goto done;
    }

    done:
    {
        ae_log( IMAGE, "%s to \"%s\": %s", __FUNCTION__,
            archive_filename, ae_image_error_name[err]);

        ae_array_free(&archive); return err;
    }
}

/*
================================================================================
 * ~~ [ data conversion ] ~~ *
--------------------------------------------------------------------------------
*/

static void ae_image_hdr_to_ldr(ae_image_t* dst, const ae_image_t* const src)
{
    AE_PROFILE_ENTER();

    int i, k, n, comp = (int)ae_image_format_components[dst->format];
    int total = (int)(dst->width * dst->height);

    u8* output = (u8*)dst->pixels;
    float* data = (float*)src->pixels;

    #if defined(__SSE2__) && !defined(AE_BRANCH_COVERAGE)
    if (ae_cpuinfo_sse2() && comp == 4 && num_is_aligned((size_t)data, 16))
    {
        float* end = data + total * comp;

        for (; data < end; output += 4, data += 4)
        {
            __m128 d = _mm_load_ps(data);

            __m128 c = _mm_clamp_ps(
                _mm_add_ps(
                    _mm_mul_ps(
                        _mm_pow_ps(
                            _mm_mul_ps(
                                d,
                                _mm_set1_ps(stbi__h2l_scale_i)
                            ),
                            _mm_set1_ps(stbi__h2l_gamma_i)
                        ),
                        _mm_set1_ps(255.0f)
                    ),
                    _mm_set1_ps(0.5f)
                ),
                _mm_set1_ps(0.0f),
                _mm_set1_ps(255.0f)
            );

            __m128 a = _mm_clamp_ps(
                _mm_add_ps(
                    _mm_mul_ps(
                        d,
                        _mm_set1_ps(255.0f)
                    ),
                    _mm_set1_ps(0.5f)
                ),
                _mm_set1_ps(0.0f),
                _mm_set1_ps(255.0f)
            );

            /* TODO: find a better and faster method for extracting bytes
             * TODO: use _mm_cvtps_epi32 to avoid adding 0.5 in rounding?
             * TODO: faster to clamp the rounded integer vectors instead?
             * TODO: SSE RGB path that works on 12 channels per iteration
             */
            __m128i ci = _mm_cvttps_epi32(c);
            __m128i ai = _mm_cvttps_epi32(a);

            AE_ALIGN16 int cx[4];
            AE_ALIGN16 int ax[4];

            _mm_store_si128((__m128i*)cx, ci);
            _mm_store_si128((__m128i*)ax, ai);

            output[0] = (u8)cx[0];
            output[1] = (u8)cx[1];
            output[2] = (u8)cx[2];
            output[3] = (u8)ax[3];
        }

        AE_PROFILE_LEAVE(); return;
    }
    #endif

    // compute number of non-alpha components (we don't remap alpha)
    ae_if (comp & 1) n = comp; else n = comp - 1;

    for (i = 0; i < total; ++i)
    {
        for (k = 0; k < n; ++k)
        {
            float z = (float)pow(data[i*comp+k] * stbi__h2l_scale_i,
                                    stbi__h2l_gamma_i) * 255 + 0.5f;

            ae_if (z > 255.0f) z = 255.0f;
            ae_if (z < 0.0f) z = 0.0f;

            output[i * comp + k] = (u8)stbi__float2int(z);
        }

        ae_if (k < comp)
        {
            float z = data[i * comp + k] * 255 + 0.5f;

            ae_if (z > 255.0f) z = 255.0f;
            ae_if (z < 0.0f) z = 0.0f;

            output[i * comp + k] = (u8)stbi__float2int(z);
        }
    }

    AE_PROFILE_LEAVE();
}

static float ae_image_ldr_to_hdr_table[256] =
{
    0.000000f, 0.000005f, 0.000023f, 0.000057f, 0.000107f, 0.000175f, 0.000262f, 0.000367f,
    0.000493f, 0.000638f, 0.000805f, 0.000992f, 0.001202f, 0.001433f, 0.001687f, 0.001963f,
    0.002263f, 0.002586f, 0.002932f, 0.003303f, 0.003697f, 0.004116f, 0.004560f, 0.005028f,
    0.005522f, 0.006041f, 0.006585f, 0.007155f, 0.007751f, 0.008373f, 0.009021f, 0.009696f,
    0.010398f, 0.011126f, 0.011881f, 0.012664f, 0.013473f, 0.014311f, 0.015175f, 0.016068f,
    0.016988f, 0.017936f, 0.018913f, 0.019918f, 0.020951f, 0.022013f, 0.023104f, 0.024223f,
    0.025371f, 0.026549f, 0.027755f, 0.028991f, 0.030257f, 0.031551f, 0.032876f, 0.034230f,
    0.035614f, 0.037029f, 0.038473f, 0.039947f, 0.041452f, 0.042987f, 0.044553f, 0.046149f,
    0.047776f, 0.049433f, 0.051122f, 0.052842f, 0.054592f, 0.056374f, 0.058187f, 0.060032f,
    0.061907f, 0.063815f, 0.065754f, 0.067725f, 0.069727f, 0.071761f, 0.073828f, 0.075926f,
    0.078057f, 0.080219f, 0.082414f, 0.084642f, 0.086901f, 0.089194f, 0.091518f, 0.093876f,
    0.096266f, 0.098689f, 0.101145f, 0.103634f, 0.106156f, 0.108711f, 0.111299f, 0.113921f,
    0.116576f, 0.119264f, 0.121986f, 0.124741f, 0.127530f, 0.130352f, 0.133209f, 0.136099f,
    0.139022f, 0.141980f, 0.144972f, 0.147998f, 0.151058f, 0.154152f, 0.157281f, 0.160444f,
    0.163641f, 0.166872f, 0.170138f, 0.173439f, 0.176774f, 0.180144f, 0.183549f, 0.186989f,
    0.190463f, 0.193972f, 0.197516f, 0.201096f, 0.204710f, 0.208360f, 0.212044f, 0.215764f,
    0.219520f, 0.223310f, 0.227137f, 0.230998f, 0.234895f, 0.238828f, 0.242796f, 0.246800f,
    0.250840f, 0.254916f, 0.259027f, 0.263175f, 0.267358f, 0.271577f, 0.275833f, 0.280124f,
    0.284452f, 0.288816f, 0.293216f, 0.297653f, 0.302125f, 0.306635f, 0.311180f, 0.315763f,
    0.320382f, 0.325037f, 0.329729f, 0.334458f, 0.339223f, 0.344026f, 0.348865f, 0.353741f,
    0.358654f, 0.363604f, 0.368591f, 0.373615f, 0.378676f, 0.383775f, 0.388910f, 0.394083f,
    0.399293f, 0.404541f, 0.409826f, 0.415148f, 0.420508f, 0.425905f, 0.431340f, 0.436813f,
    0.442323f, 0.447871f, 0.453456f, 0.459080f, 0.464741f, 0.470440f, 0.476177f, 0.481952f,
    0.487765f, 0.493616f, 0.499505f, 0.505432f, 0.511398f, 0.517401f, 0.523443f, 0.529523f,
    0.535642f, 0.541798f, 0.547993f, 0.554227f, 0.560499f, 0.566810f, 0.573159f, 0.579547f,
    0.585973f, 0.592438f, 0.598942f, 0.605484f, 0.612066f, 0.618686f, 0.625345f, 0.632043f,
    0.638779f, 0.645555f, 0.652370f, 0.659224f, 0.666117f, 0.673049f, 0.680020f, 0.687031f,
    0.694081f, 0.701169f, 0.708298f, 0.715465f, 0.722672f, 0.729919f, 0.737205f, 0.744530f,
    0.751895f, 0.759300f, 0.766744f, 0.774227f, 0.781751f, 0.789314f, 0.796917f, 0.804559f,
    0.812241f, 0.819964f, 0.827726f, 0.835528f, 0.843370f, 0.851252f, 0.859174f, 0.867136f,
    0.875138f, 0.883180f, 0.891262f, 0.899385f, 0.907547f, 0.915750f, 0.923993f, 0.932277f,
    0.940601f, 0.948965f, 0.957370f, 0.965815f, 0.974300f, 0.982826f, 0.991393f, 1.000000f,
};

static void ae_image_ldr_to_hdr(ae_image_t* dst, const ae_image_t* const src)
{
    AE_PROFILE_ENTER();

    int i, k, n, comp = (int)ae_image_format_components[dst->format];
    int total = (int)(dst->width * dst->height);

    float* output = (float*)dst->pixels;
    u8* data = (u8*)src->pixels;

    switch (comp)
    {
    #if !defined(AE_BRANCH_COVERAGE)
        case 3:
        {
            for (i = 0; i < total; ++i)
            {
                output[i*comp+0] = ae_image_ldr_to_hdr_table[data[i*comp+0]];
                output[i*comp+1] = ae_image_ldr_to_hdr_table[data[i*comp+1]];
                output[i*comp+2] = ae_image_ldr_to_hdr_table[data[i*comp+2]];
            }
        }
        break;

        case 4:
        {
            for (i = 0; i < total; ++i)
            {
                output[i*comp+0] = ae_image_ldr_to_hdr_table[data[i*comp+0]];
                output[i*comp+1] = ae_image_ldr_to_hdr_table[data[i*comp+1]];
                output[i*comp+2] = ae_image_ldr_to_hdr_table[data[i*comp+2]];

                output[i*comp+3] = data[i*comp+3] / 255.0f;
            }
        }
        break;
    #elif defined(__cplusplus) /* MSVC warning C4065: no case labels */
        case 3:
        case 4:
    #endif
        default:
        {
            // compute number of non-alpha components (we don't remap alpha)
            ae_if (comp & 1) n = comp; else n = comp - 1;

            for (i = 0; i < total; ++i)
            {
                for (k = 0; k < n; ++k)
                {
                #if 1
                    // TODO ensure that generated table matches gamma and scale!
                    output[i*comp+k] = ae_image_ldr_to_hdr_table[data[i*comp+k]];
                #else
                    output[i*comp+k] = (float) (pow(data[i*comp+k] / 255.0f,
                                        stbi__l2h_gamma) * stbi__l2h_scale);
                #endif
                }

                ae_if (k < comp) output[i*comp+k] = data[i*comp+k] / 255.0f;
            }
        }
        break;
    }

    AE_PROFILE_LEAVE();
}

void ae_image_type_convert(ae_image_t* dst, const ae_image_t* const src,
                                            const ae_image_type_t type)
{
    const size_t dst_bytes = ae_image_format_components[src->format] *
                src->width * src->height * ae_image_type_bytes[type];

    ae_assert(src->pixels != NULL, "cannot convert invalid (NULL) source pixels");

    // no actual conversion, so just copy source to destination
    if (src->type == type) { ae_image_copy(dst, src); return; }

    if ( !(dst->pixels != NULL && ae_image_bytes(dst) == dst_bytes) )
    {
        ae_image_free(dst); // reallocate the dst buffer to fit
        dst->pixels = ae_heap_malloc(&ae_image_heap, dst_bytes);
    }

    dst->format = src->format;
    dst->type = type;
    dst->width = src->width;
    dst->height = src->height;

    ae_switch (type, ae_image_type, AE_IMAGE_TYPE, suffix)
    {
        case AE_IMAGE_TYPE_FLT: ae_image_ldr_to_hdr(dst, src); break;
        case AE_IMAGE_TYPE_U8:  ae_image_hdr_to_ldr(dst, src); break;

        default: assert(0); break;
    }
}

void ae_image_format_convert(ae_image_t* dst, const ae_image_t* const src,
                                        const ae_image_format_t format)
{
    const size_t dst_bytes = ae_image_format_components[format] *
        src->width * src->height * ae_image_type_bytes[src->type];

    ae_assert(src->pixels != NULL, "cannot convert invalid (NULL) source pixels");

    // no actual conversion to do here, so just copy source to dest
    if (src->format == format) { ae_image_copy(dst, src); return; }

    if ( !(dst->pixels != NULL && ae_image_bytes(dst) == dst_bytes) )
    {
        ae_image_free(dst); // reallocate the dst buffer to fit
        dst->pixels = ae_heap_malloc(&ae_image_heap, dst_bytes);
    }

    dst->format = format;
    dst->type = src->type;
    dst->width = src->width;
    dst->height = src->height;

    ae_switch (src->type, ae_image_type, AE_IMAGE_TYPE, suffix)
    {
        case AE_IMAGE_TYPE_U8:
        {
            u8* src_p = (u8*)src->pixels;
            u8* dst_p = (u8*)dst->pixels;
            u8* end_p = dst_p + dst_bytes / sizeof(u8);

            ae_switch (src->format, ae_image_format, AE_IMAGE_FORMAT, suffix)
            {
                case AE_IMAGE_FORMAT_MONO:
                {
                    ae_switch (format, ae_image_format, AE_IMAGE_FORMAT, suffix)
                    {
                        case AE_IMAGE_FORMAT_RGBA:
                        {
                            for (; dst_p < end_p; src_p += 1, dst_p += 4)
                            {
                                dst_p[0] = src_p[0];
                                dst_p[1] = src_p[0];
                                dst_p[2] = src_p[0];
                                dst_p[3] = 255;
                            }
                        }
                        break;

                        case AE_IMAGE_FORMAT_RGB:
                        {
                            for (; dst_p < end_p; src_p += 1, dst_p += 3)
                            {
                                dst_p[0] = src_p[0];
                                dst_p[1] = src_p[0];
                                dst_p[2] = src_p[0];
                            }
                        }
                        break;

                        default: assert(0); break;
                    }
                }
                break;

                case AE_IMAGE_FORMAT_RGB:
                {
                    ae_switch (format, ae_image_format, AE_IMAGE_FORMAT, suffix)
                    {
                        case AE_IMAGE_FORMAT_RGBA:
                        {
                            for (; dst_p < end_p; src_p += 3, dst_p += 4)
                            {
                                dst_p[0] = src_p[0];
                                dst_p[1] = src_p[1];
                                dst_p[2] = src_p[2];
                                dst_p[3] = 255;
                            }
                        }
                        break;

                        default: assert(0); break;
                    }
                }
                break;

                case AE_IMAGE_FORMAT_RGBA:
                {
                    ae_switch (format, ae_image_format, AE_IMAGE_FORMAT, suffix)
                    {
                        case AE_IMAGE_FORMAT_RGB:
                        {
                            for (; dst_p < end_p; src_p += 4, dst_p += 3)
                            {
                                dst_p[0] = src_p[0];
                                dst_p[1] = src_p[1];
                                dst_p[2] = src_p[2];
                            }
                        }
                        break;

                        default: assert(0); break;
                    }
                }
                break;

                default: assert(0); break;
            }
        }
        break;

        case AE_IMAGE_TYPE_FLT:
        {
            float* src_p = (float*)src->pixels;
            float* dst_p = (float*)dst->pixels;
            float* end_p = (float*)dst_p + dst_bytes / sizeof(float);

            ae_switch (src->format, ae_image_format, AE_IMAGE_FORMAT, suffix)
            {
                case AE_IMAGE_FORMAT_MONO:
                {
                    ae_switch (format, ae_image_format, AE_IMAGE_FORMAT, suffix)
                    {
                        case AE_IMAGE_FORMAT_RGBA:
                        {
                            for (; dst_p < end_p; src_p += 1, dst_p += 4)
                            {
                                dst_p[0] = src_p[0];
                                dst_p[1] = src_p[0];
                                dst_p[2] = src_p[0];
                                dst_p[3] = 1.0f;
                            }
                        }
                        break;

                        case AE_IMAGE_FORMAT_RGB:
                        {
                            for (; dst_p < end_p; src_p += 1, dst_p += 3)
                            {
                                dst_p[0] = src_p[0];
                                dst_p[1] = src_p[0];
                                dst_p[2] = src_p[0];
                            }
                        }
                        break;

                        default: assert(0); break;
                    }
                }
                break;

                case AE_IMAGE_FORMAT_RGB:
                {
                    ae_switch (format, ae_image_format, AE_IMAGE_FORMAT, suffix)
                    {
                        case AE_IMAGE_FORMAT_RGBA:
                        {
                            for (; dst_p < end_p; src_p += 3, dst_p += 4)
                            {
                                dst_p[0] = src_p[0];
                                dst_p[1] = src_p[1];
                                dst_p[2] = src_p[2];
                                dst_p[3] = 1.0f;
                            }
                        }
                        break;

                        default: assert(0); break;
                    }
                }
                break;

                case AE_IMAGE_FORMAT_RGBA:
                {
                    ae_switch (format, ae_image_format, AE_IMAGE_FORMAT, suffix)
                    {
                        case AE_IMAGE_FORMAT_RGB:
                        {
                            for (; dst_p < end_p; src_p += 4, dst_p += 3)
                            {
                                dst_p[0] = src_p[0];
                                dst_p[1] = src_p[1];
                                dst_p[2] = src_p[2];
                            }
                        }
                        break;

                        default: assert(0); break;
                    }
                }
                break;

                default: assert(0); break;
            }
        }
        break;

        default: assert(0); break;
    }
}

/*
================================================================================
 * ~~ [ resizing ] ~~ *
--------------------------------------------------------------------------------
*/

void ae_image_resize_ex(ae_image_t* dst, const ae_image_t* const src,
                    const size_t new_width, const size_t new_height,
                    ae_image_resize_filter_t filter)
{
    ae_image_t temp; // allocate destination image if needed

    temp.width = new_width;
    temp.height = new_height;
    temp.format = src->format;
    temp.type = src->type;
    ae_image_alloc_fit(dst, &temp);

    if (!ae_image_bytes(src))
    {
        // resize to black anyway if the src image is invalid
        memset(dst->pixels, 0, ae_image_bytes(dst)); return;
    }

    ae_switch (src->type, ae_image_type, AE_IMAGE_TYPE, suffix)
    {
        case AE_IMAGE_TYPE_U8:
        {
            if(!stbir_resize_uint8_generic(
                (u8*)src->pixels,
                (int)src->width,
                (int)src->height,
                0,
                (u8*)dst->pixels,
                (int)dst->width,
                (int)dst->height,
                0,
                (int)ae_image_format_components[src->format],
                STBIR_ALPHA_CHANNEL_NONE,
                0,
                STBIR_EDGE_CLAMP,
                (stbir_filter)filter,
                STBIR_COLORSPACE_LINEAR,
                NULL))
            {
                goto error;
            }
        }
        break;

        case AE_IMAGE_TYPE_FLT:
        {
            if(!stbir_resize_float_generic(
                (float *)src->pixels,
                (  int  )src->width,
                (  int  )src->height,
                0,
                (float *)dst->pixels,
                (  int  )dst->width,
                (  int  )dst->height,
                0,
                (int)ae_image_format_components[src->format],
                STBIR_ALPHA_CHANNEL_NONE,
                0,
                STBIR_EDGE_CLAMP,
                (stbir_filter)filter,
                STBIR_COLORSPACE_LINEAR,
                NULL))
            {
                goto error;
            }
        }
        break;

        default: assert(0); break;
    }

    return; error:
    {
        // TODO: do something better here, even though this
        // usually means invalid parameters were passed...
        memset(dst->pixels, 0, ae_image_bytes(dst));

        AE_WARN("image resize call failed (filter %s)",
                ae_image_resize_filter_name[filter] );
    }
}

void ae_image_resize(ae_image_t* dst, const ae_image_t* const src,
                const size_t new_width, const size_t new_height )
{
    ae_image_resize_ex( dst, src, new_width, new_height,
                        AE_IMAGE_RESIZE_FILTER_DEFAULT);
}

/*
================================================================================
 * ~~ [ matrix transformation ] ~~ *
--------------------------------------------------------------------------------
*/

// TODO

/*
================================================================================
 * ~~ [ unary operations ] ~~ *
--------------------------------------------------------------------------------
TODO: get image region in DevIL memory, then apply blur, alienify, misc filters
--------------------------------------------------------------------------------
*/

void ae_image_unary_op(ae_image_t* image, int* rect, ae_image_unary_op_t* op, void* data)
{
    /* NOTE: we don't profile this function (or binary op) because it might call itself,
     * which could lead to inflated times (outer call also contains time of inner call).
     */
    int box[4], w = (int)image->width, h = (int)image->height, y;
    size_t comp = ae_image_format_components[image->format];

    // when this is implemented, "tile" size is to be determined by the op based on cost
    // per pixel and total image size. this is really a matter of tuning and profiling...
    ae_assert(op->num_tasks == 0, "the multithreaded job queue is not yet implemented");

    // TODO: should this just be caught in the image size check, instead of blowing up?
    ae_assert(image->pixels != NULL, "called filter operation on invalid (NULL) image");

    ae_if (rect != NULL)
    {
        box[0] = rect[0]; // y flip bottom-left to top-left
        box[1] = ((int) image->height - rect[3]) - rect[1];

        box[2] = box[0] + rect[2];
        box[3] = box[1] + rect[3];

        // clamp the bounding box to the image's dimensions
        if (box[0] < 0) box[0] = 0;
        if (box[0] > w) box[0] = w;

        if (box[1] < 0) box[1] = 0;
        if (box[1] > h) box[1] = h;

        if (box[2] < 0) box[2] = 0;
        if (box[2] > w) box[2] = w;

        if (box[3] < 0) box[3] = 0;
        if (box[3] > h) box[3] = h;

        // rect dimensions may have changed after clamping
        w = box[2] - box[0];
        h = box[3] - box[1];
    }
    else
    {
        box[0] = box[1] = 0; // apply the op to the entire image
        box[2] = (int)image->width, box[3] = (int)image->height;
    }

    #if 0
        // show warning if we're passing weird image dimensions
        ae_if (w <= 0 || h <= 0) goto fail_msg;
    #else
        // generic warning message can be confusing and annoying
        ae_if (w <= 0 || h <= 0) return;
    #endif

    ae_switch (image->type, ae_image_type, AE_IMAGE_TYPE, suffix)
    {
        case AE_IMAGE_TYPE_U8:
        {
            void (*func)(u8* start, u8* end, size_t comp, void* data) = op->u8_func[comp];
            u8* pixels = (u8*)image->pixels;

            ae_if (func != NULL)
            {
                if (rect) // optimization: only call function once for entire image ops
                {
                #if 0
                    for (y = box[3] - 1; y >= box[1]; y--)
                #else
                    for (y = box[1]; y < box[3]; y++)
                #endif
                    {
                        u8* start = pixels + box[0] * comp + y * image->width * comp;
                        u8* end = start + w * comp; // TODO: optimize this outer loop

                        func(start, end, comp, data);
                    }
                }
                else
                {
                    func(pixels, pixels + w * h * comp, comp, data);
                }
            }
            else
            {
                if (op->flt_func[comp]) // try filtering converted temp copy
                {
                    ae_image_t temp;

                    ae_image_alloc_temp(&temp, image->width, image->height,
                                        AE_IMAGE_TYPE_FLT, image->format);

                    ae_image_type_convert(&temp, image, AE_IMAGE_TYPE_FLT);
                    ae_image_unary_op(&temp, rect, op, data);

                    ae_image_type_convert(image, &temp, AE_IMAGE_TYPE_U8);
                    ae_image_free_temp(&temp);
                }
                else
                {
                    goto fail_msg;
                }
            }
        }
        break;

        case AE_IMAGE_TYPE_FLT:
        {
            void (*func)(float* start, float* end, size_t comp, void* data) = op->flt_func[comp];
            float* pixels = (float*)image->pixels;

            ae_if (func != NULL)
            {
                if (rect) // optimization: only call function once for entire image ops
                {
                #if 0
                    for (y = box[3] - 1; y >= box[1]; y--)
                #else
                    for (y = box[1]; y < box[3]; y++)
                #endif
                    {
                        float* start = pixels + box[0] * comp + y * image->width * comp;
                        float* end = start + w * comp; // TODO: optimize this outer loop

                        func(start, end, comp, data);
                    }
                }
                else
                {
                    func(pixels, pixels + w * h * comp, comp, data);
                }
            }
            else
            {
                if (op->u8_func[comp]) // try filtering converted temp copy
                {
                    ae_image_t temp;

                    ae_image_alloc_temp(&temp, image->width, image->height,
                                        AE_IMAGE_TYPE_U8, image->format );

                    ae_image_type_convert(&temp, image, AE_IMAGE_TYPE_U8);
                    ae_image_unary_op(&temp, rect, op, data);

                    ae_image_type_convert(image, &temp, AE_IMAGE_TYPE_FLT);
                    ae_image_free_temp(&temp);
                }
                else
                {
                    goto fail_msg;
                }
            }
        }
        break;

        default: assert(0); break;
    }

    /* TODO the type conversion fallback path for both u8 and float images is the
            same with different src and dst type arguments. we could assign those
            arguments to variables and goto a label here to save some code space.
    */

    return; fail_msg:
    {
        AE_WARN("op \"%s\" not supported for %ux%u image of type %s and format %s",
            op->name, (u32)image->width, (u32)image->height,
            ae_image_type_name[image->type], ae_image_format_name[image->format]);
    }
}

// TODO optimize this by pulling branches out of the loop and replacing
// ~~~~ NULL color lookup tables with identity tables (0, 1, 2, 3 ...).

static void ae_unary_clut_rgb(u8* pix, u8* end, size_t comp, void* data)
{
    u8* r = ((u8**)data)[0];
    u8* g = ((u8**)data)[1];
    u8* b = ((u8**)data)[2];

    for (; pix < end; pix += comp)
    {
        if (r) pix[0] = r[pix[0]];
        if (g) pix[1] = g[pix[1]];
        if (b) pix[2] = b[pix[2]];
    }
}

static void ae_unary_clut_rgba(u8* pix, u8* end, size_t comp, void* data)
{
    u8* r = ((u8**)data)[0];
    u8* g = ((u8**)data)[1];
    u8* b = ((u8**)data)[2];
    u8* a = ((u8**)data)[3];

    for (; pix < end; pix += comp)
    {
        if (r) pix[0] = r[pix[0]];
        if (g) pix[1] = g[pix[1]];
        if (b) pix[2] = b[pix[2]];
        if (a) pix[3] = a[pix[3]];
    }
}

void ae_image_unary_clut(ae_image_t* image, int* rect, u8* r, u8* g, u8* b, u8* a)
{
    u8* clut[4] = { r, g, b, a };
    ae_image_unary_op_t op;

    memset(&op, 0, sizeof(ae_image_unary_op_t));
    op.name = "unary_clut";

    op.u8_func[3] = ae_unary_clut_rgb;
    op.u8_func[4] = ae_unary_clut_rgba;

    ae_image_unary_op(image, rect, &op, clut);
}

/* ===== [ filters ] ======================================================== */

void ae_image_negative(ae_image_t* image, int* rect, int r, int g, int b)
{
    AE_PROFILE_ENTER();

    // TODO this isn't optimized at all (avoid table?),
    // implement for floating point and mono images...
    size_t i = 0;
    u8 clut[256];

    for (; i < 256; ++i)
    {
        clut[i] = ~(u8)i;
    }

    ae_image_unary_clut(image, rect, r ? clut : NULL,
            g ? clut : NULL, b ? clut : NULL, NULL );

    AE_PROFILE_LEAVE();
}

void ae_image_solarize(ae_image_t* image, int* rect, u8 threshold, int r, int g, int b)
{
    AE_PROFILE_ENTER();

    // TODO this isn't optimized at all (avoid table?)
    size_t i = 0;
    u8 clut[256];

    for (; i < 256; ++i)
    {
        clut[i] = i >= threshold ? ~(u8)i : (u8)i;
    }

    ae_image_unary_clut(image, rect, r ? clut : NULL,
            g ? clut : NULL, b ? clut : NULL, NULL );

    AE_PROFILE_LEAVE();
}

static void ae_image_greyscale_u8(u8* pix, u8* end, size_t comp, void* data)
{
    int r = ((int*)data)[0];
    int g = ((int*)data)[1];
    int b = ((int*)data)[2];

    for (; pix < end; pix += comp)
    {
        // use biased color channel mean, as it's more visually pleasing
        u8 average = ( pix[0] * 61 + pix[1] * 174 + pix[2] * 21 ) / 256;

        if (r) pix[0] = average;
        if (g) pix[1] = average;
        if (b) pix[2] = average;
    }
}

static void ae_image_greyscale_flt(float* pix, float* end, size_t comp, void* data)
{
    int r = ((int*)data)[0];
    int g = ((int*)data)[1];
    int b = ((int*)data)[2];

    for (; pix < end; pix += comp)
    {
        // TODO: optimize! make a special version of this without inner branches
        float average = pix[0] * 0.2126f + pix[1] * 0.7152f + pix[2] * 0.0722f;

        if (r) pix[0] = average;
        if (g) pix[1] = average;
        if (b) pix[2] = average;
    }
}

void ae_image_greyscale(ae_image_t* image, int* rect, int r, int g, int b)
{
    AE_PROFILE_ENTER(); // TODO: optimize inner functions!!!

    int rgb[3] = { r, g, b };
    ae_image_unary_op_t op;

    memset(&op, 0, sizeof(ae_image_unary_op_t));
    op.name = "greyscale";

    op.flt_func[3] = op.flt_func[4] = ae_image_greyscale_flt;
    op.u8_func [3] = op.u8_func [4] = ae_image_greyscale_u8;

    ae_image_unary_op(image, rect, &op, rgb);
    AE_PROFILE_LEAVE();
}

static void ae_image_threshold_u8(u8* pix, u8* end, size_t comp, void* data)
{
    int r = ((int*)data)[0];
    int g = ((int*)data)[1];
    int b = ((int*)data)[2];

    u8 threshold = (u8)((int*)data)[3];

    for (; pix < end; pix += comp)
    {
        // use biased color channel mean, as it's more visually pleasing
        u8 average = ( pix[0] * 61 + pix[1] * 174 + pix[2] * 21 ) / 256;

        if (average > threshold)
        {
            average = 255;
        }
        else
        {
            average = 0;
        }

        if (r) pix[0] = average;
        if (g) pix[1] = average;
        if (b) pix[2] = average;
    }
}

static void ae_image_threshold_flt(float* pix, float* end, size_t comp, void* data)
{
    int r = ((int*)data)[0];
    int g = ((int*)data)[1];
    int b = ((int*)data)[2];

    float threshold = ( (float) ((int*)data)[3] ) / 255.0f;

    for (; pix < end; pix += comp)
    {
        // TODO: optimize! make a special version of this without inner branches
        float average = pix[0] * 0.2126f + pix[1] * 0.7152f + pix[2] * 0.0722f;

        if (average > threshold)
        {
            average = 1.0f;
        }
        else
        {
            average = 0.0f;
        }

        if (r) pix[0] = average;
        if (g) pix[1] = average;
        if (b) pix[2] = average;
    }
}

void ae_image_threshold(ae_image_t* image, int* rect, u8 threshold, int r, int g, int b)
{
    AE_PROFILE_ENTER();

    // NOTE: this looks best with a light gaussian blur after
    int rgb[4] = { r, g, b, (int)threshold };
    ae_image_unary_op_t op;

    memset(&op, 0, sizeof(ae_image_unary_op_t));
    op.name = "threshold";

    op.flt_func[3] = op.flt_func[4] = ae_image_threshold_flt;
    op.u8_func [3] = op.u8_func [4] = ae_image_threshold_u8;

    ae_image_unary_op(image, rect, &op, rgb);
    AE_PROFILE_LEAVE();
}

static void ae_image_bleach_u8(u8* pix, u8* end, size_t comp, void* data)
{
    int r = ((int*)data)[0];
    int g = ((int*)data)[1];
    int b = ((int*)data)[2];

    u8 threshold = (u8)((int*)data)[3];

    for (; pix < end; pix += comp)
    {
        // use biased color channel mean, as it's more visually pleasing
        u8 average = ( pix[0] * 61 + pix[1] * 174 + pix[2] * 21 ) / 256;

        if (average > threshold)
        {
            if (r) pix[0] = average;
            if (g) pix[1] = average;
            if (b) pix[2] = average;
        }
    }
}

static void ae_image_bleach_flt(float* pix, float* end, size_t comp, void* data)
{
    int r = ((int*)data)[0];
    int g = ((int*)data)[1];
    int b = ((int*)data)[2];

    float threshold = ( (float) ((int*)data)[3] ) / 255.0f;

    for (; pix < end; pix += comp)
    {
        // TODO: optimize! make a special version of this without inner branches
        float average = pix[0] * 0.2126f + pix[1] * 0.7152f + pix[2] * 0.0722f;

        if (average > threshold)
        {
            if (r) pix[0] = average;
            if (g) pix[1] = average;
            if (b) pix[2] = average;
        }
    }
}

void ae_image_bleach(ae_image_t* image, int* rect, u8 threshold, int r, int g, int b)
{
    AE_PROFILE_ENTER(); // TODO: optimize inner functions

    int rgb[4] = { r, g, b, (int)threshold };
    ae_image_unary_op_t op;

    memset(&op, 0, sizeof(ae_image_unary_op_t));
    op.name = "bleach";

    op.flt_func[3] = op.flt_func[4] = ae_image_bleach_flt;
    op.u8_func [3] = op.u8_func [4] = ae_image_bleach_u8;

    ae_image_unary_op(image, rect, &op, rgb);
    AE_PROFILE_LEAVE();
}

void ae_image_neonize(ae_image_t * image, int * rect, u8 threshold,
                    int lr, int lg, int lb, int hr, int hg, int hb)
{
    AE_PROFILE_ENTER();

    size_t i; // TODO: optimize!!! (float path, RGB branch removal)
    u8 r[256], g[256], b[256];

    for (i = 0; i < 256; ++i)
    {
        if (i >= threshold)
        {
            r[i] = hr ? ~(u8)i : (u8)i;
            g[i] = hg ? ~(u8)i : (u8)i;
            b[i] = hb ? ~(u8)i : (u8)i;
        }
        else
        {
            r[i] = lr ? ~(u8)i : (u8)i;
            g[i] = lg ? ~(u8)i : (u8)i;
            b[i] = lb ? ~(u8)i : (u8)i;
        }
    }

    ae_image_unary_clut(image, rect, r, g, b, NULL);
    AE_PROFILE_LEAVE();
}

#define AE_IMAGE_PASTELIZE_LOCALS()     \
                                        \
    const int lr = ((int*)data)[0];     \
    const int lg = ((int*)data)[1];     \
    const int lb = ((int*)data)[2];     \
                                        \
    const int hr = ((int*)data)[3];     \
    const int hg = ((int*)data)[4];     \
    const int hb = ((int*)data)[5];     \
                                        \
    u8 threshold = (u8)((int*)data)[6]  \

static void ae_image_pastelize_u8(u8* pix, u8* end, size_t comp, void* data)
{
    AE_IMAGE_PASTELIZE_LOCALS();

    for (; pix < end; pix += comp)
    {
        // use biased color channel mean, as it's more visually pleasing
        u8 average = ( pix[0] * 61 + pix[1] * 174 + pix[2] * 21 ) / 256;

        if (average > threshold)
        {
            if (hr) pix[0] = average;
            if (hg) pix[1] = average;
            if (hb) pix[2] = average;
        }
        else
        {
            if (lr) pix[0] = average;
            if (lg) pix[1] = average;
            if (lb) pix[2] = average;
        }
    }
}

static void ae_image_pastelize_flt(float* pix, float* end, size_t comp, void* data)
{
    AE_IMAGE_PASTELIZE_LOCALS();
    const float f_threshold = (float)threshold / 255.0f;

    for (; pix < end; pix += comp)
    {
        // TODO: optimize! make a special version of this without inner branches
        float average = pix[0] * 0.2126f + pix[1] * 0.7152f + pix[2] * 0.0722f;

        if (average > f_threshold)
        {
            if (hr) pix[0] = average;
            if (hg) pix[1] = average;
            if (hb) pix[2] = average;
        }
        else
        {
            if (lr) pix[0] = average;
            if (lg) pix[1] = average;
            if (lb) pix[2] = average;
        }
    }
}

void ae_image_pastelize(ae_image_t* image, int* rect, u8 threshold,
                    int lr, int lg, int lb, int hr, int hg, int hb)
{
    AE_PROFILE_ENTER(); // TODO: optimize inner functions!!!
    int rgb[7] = { lr, lg, lb, hr, hg, hb, (int)threshold };

    ae_image_unary_op_t op = AE_ZERO_STRUCT;
    op.name = "pastelize";

    op.flt_func[3] = op.flt_func[4] = ae_image_pastelize_flt;
    op.u8_func [3] = op.u8_func [4] = ae_image_pastelize_u8;

    ae_image_unary_op(image, rect, &op, rgb);
    AE_PROFILE_LEAVE();
}

static void ae_image_isolate_channel_u8(u8* pix, u8* end, size_t comp, void* data)
{
    int r = ((int*)data)[0];
    int g = ((int*)data)[1];
    int b = ((int*)data)[2];
    int c = ((int*)data)[3];

    for (; pix < end; pix += comp)
    {
        if (r) pix[0] = pix[c];
        if (g) pix[1] = pix[c];
        if (b) pix[2] = pix[c];
    }
}

static void ae_image_isolate_channel_flt(float* pix, float* end, size_t comp, void* data)
{
    int r = ((int*)data)[0];
    int g = ((int*)data)[1];
    int b = ((int*)data)[2];
    int c = ((int*)data)[3];

    for (; pix < end; pix += comp)
    {
        if (r) pix[0] = pix[c];
        if (g) pix[1] = pix[c];
        if (b) pix[2] = pix[c];
    }
}

void ae_image_isolate_channel(ae_image_t* image, int* rect, int channel, int r, int g, int b)
{
    AE_PROFILE_ENTER(); // TODO: optimize (branch removal, memcpy)

    int rgb[4] = { r, g, b, channel };
    ae_image_unary_op_t op;

    memset(&op, 0, sizeof(ae_image_unary_op_t));
    op.name = "isolate_channel";

    op.flt_func[3] = op.flt_func[4] = ae_image_isolate_channel_flt;
    op.u8_func [3] = op.u8_func [4] = ae_image_isolate_channel_u8;

    ae_assert(channel >= 0 && channel <= 2, "%i", channel);
    ae_image_unary_op(image, rect, &op, rgb);

    AE_PROFILE_LEAVE();
}

typedef struct ae_image_set_color_t
{
    float* color; // args
    int r, g, b, a;
} \
    ae_image_set_color_t;

static void ae_image_set_color_flt(float* pix, float* end, size_t comp, void* data)
{
    ae_image_set_color_t* args = (ae_image_set_color_t*)data;
    float* color = args->color;

    const int r = args->r;
    const int g = args->g;
    const int b = args->b;
    const int a = args->a;

    for (; pix < end; pix += comp)
    {
        if (r) pix[0] = color[0];
        if (g) pix[1] = color[1];
        if (b) pix[2] = color[2];

        if (a && comp == 4) pix[3] = color[3];
    }
}

static void ae_image_set_color_u8(u8* pix, u8* end, size_t comp, void* data)
{
    AE_PROFILE_ENTER();

    /* TODO: take branches out of scanline callback, put color conversion in a function
     */
    ae_image_set_color_t* args = (ae_image_set_color_t*)data;
    float* color = args->color;

    const u8 uR = (u8)stbi__float2int(ae_fclampf((float)pow(color[0] * stbi__h2l_scale_i,
                                        stbi__h2l_gamma_i) * 255 + 0.5f, 0.0f, 255.0f));

    const u8 uG = (u8)stbi__float2int(ae_fclampf((float)pow(color[1] * stbi__h2l_scale_i,
                                        stbi__h2l_gamma_i) * 255 + 0.5f, 0.0f, 255.0f));

    const u8 uB = (u8)stbi__float2int(ae_fclampf((float)pow(color[2] * stbi__h2l_scale_i,
                                        stbi__h2l_gamma_i) * 255 + 0.5f, 0.0f, 255.0f));

    const u8 uA = (u8)stbi__float2int(ae_fclampf(color[3] * 255 + 0.5f, 0.0f, 255.0f));

    const int r = args->r;
    const int g = args->g;
    const int b = args->b;
    const int a = args->a;

    if (r && g && b && a && comp == 4)
    {
        for (; pix < end; pix += comp)
        {
            pix[0] = uR;
            pix[1] = uG;
            pix[2] = uB;
            pix[3] = uA;
        }
    }
    else
    {
        for (; pix < end; pix += comp)
        {
            if (r) pix[0] = uR;
            if (g) pix[1] = uG;
            if (b) pix[2] = uB;

            if (a && comp == 4) pix[3] = uA;
        }
    }

    AE_PROFILE_LEAVE();
}

void ae_image_set_color(ae_image_t* image, int* rect, const float color[4],
                        int r, int g, int b, int a)
{
    AE_PROFILE_ENTER();

    ae_image_set_color_t args = { (float*)color, r, g, b, a };
    ae_image_unary_op_t op;

    memset(&op, 0, sizeof(ae_image_unary_op_t));
    op.name = "set_color";

    op.flt_func[3] = op.flt_func[4] = ae_image_set_color_flt;
    op.u8_func [3] = op.u8_func [4] = ae_image_set_color_u8;

    ae_image_unary_op(image, rect, &op, &args);
    AE_PROFILE_LEAVE();
}

static void ae_image_scale_color_flt(float* pix, float* end, size_t comp, void* data)
{
    for (; pix < end; pix += comp)
    {
        pix[0] *= ((float*)data)[0];
        pix[1] *= ((float*)data)[1];
        pix[2] *= ((float*)data)[2];

        if (comp == 4) // TODO: SSE
        pix[3] *= ((float*)data)[3];
    }
}

void ae_image_scale_color(ae_image_t* image, int* rect, const float scale[4])
{
    ae_image_unary_op_t op;
    AE_PROFILE_ENTER();

    memset(&op, 0, sizeof(ae_image_unary_op_t));
    op.name = "scale_color";

    // TODO: fast u8 version (crc32 to make sure it's correct)
    op.flt_func[3] = op.flt_func[4] = ae_image_scale_color_flt;

    ae_image_unary_op(image, rect, &op, (void*)scale);
    AE_PROFILE_LEAVE();
}

static void ae_image_flip_x_u8(u8* pix, u8* end, size_t comp, void* data)
{
    int r = ((int*)data)[0];
    int g = ((int*)data)[1];
    int b = ((int*)data)[2];
    int a = ((int*)data)[3];

    end -= comp;

    while (pix < end)
    {
        if (r) { u8 t = pix[0]; pix[0] = end[0]; end[0] = t; }
        if (g) { u8 t = pix[1]; pix[1] = end[1]; end[1] = t; }
        if (b) { u8 t = pix[2]; pix[2] = end[2]; end[2] = t; }

        if (a && comp == 4)
        {
            u8 t = pix[3]; pix[3] = end[3]; end[3] = t;
        }

        pix += comp;
        end -= comp;
    }
}

void ae_image_flip_x(ae_image_t* image, int* rect, int r, int g, int b, int a)
{
    AE_PROFILE_ENTER(); // TODO: optimize inner funcs!!!

    int fixed_rect[4], rgba[4] = { r, g, b, a };
    ae_image_unary_op_t op;

    // hack: make sure function is called each scanline
    if (rect == NULL)
    {
        fixed_rect[0] = fixed_rect[1] = 0;
        fixed_rect[2] = (int)image->width;
        fixed_rect[3] = (int)image->height;

        rect = fixed_rect;
    }

    memset(&op, 0, sizeof(ae_image_unary_op_t));
    op.name = "flip_x";

    // TODO: macro u8 body (it can apply to floats too)
    op.u8_func[3] = op.u8_func[4] = ae_image_flip_x_u8;

    ae_image_unary_op(image, rect, &op, rgba);
    AE_PROFILE_LEAVE();
}

void ae_image_flip_y(ae_image_t* image, int* rect, int r, int g, int b, int a)
{
    AE_PROFILE_ENTER();

    int c = (int)ae_image_format_components[image->format];
    int box[4], y1, y2, w, h, h2;

    if (rect)
    {
        box[0] = rect[0]; // y flip bottom-left to top-left
        box[1] = ((int) image->height - rect[3]) - rect[1];

        box[2] = box[0] + rect[2];
        box[3] = box[1] + rect[3];

        box[0] = ae_iclamp(box[0], 0, image->width);
        box[1] = ae_iclamp(box[1], 0, image->height);
        box[2] = ae_iclamp(box[2], 0, image->width);
        box[3] = ae_iclamp(box[3], 0, image->height);
    }
    else
    {
        box[0] = box[1] = 0; // apply the op to the entire image
        box[2] = (int)image->width, box[3] = (int)image->height;
    }

    w = box[2] - box[0]; // get the new rect dimensions after clamping
    h = box[3] - box[1], h2 = h / 2;

    ae_switch (image->type, ae_image_type, AE_IMAGE_TYPE, suffix)
    {
        case AE_IMAGE_TYPE_U8:
        {
            // for each row of pixels, swap them with the opposite row
            u8* p = (u8*)image->pixels;

            if (r && g && b && (c == 4 ? a : 1))
            {
                for(y1 = box[1], y2 = box[3] - 1; y1 < y2; y1++, y2--)
                {
                    memswap(p + y1 * image->width * c + box[0] * c, // fast
                            p + y2 * image->width * c + box[0] * c, w * c);
                }
            }
            else
            {
                for(y1 = box[1], y2 = box[3] - 1;
                    y1 < box[1] + h2; y1++, y2--)
                {
                    u8* p1 = p + y1 * image->width * c + box[0] * c;
                    u8* p2 = p + y2 * image->width * c + box[0] * c;

                    u8* end = p1 + w * c;

                    for (; p1 < end; p1 += c, p2 += c)
                    {
                        if (r) { u8 t = p1[0]; p1[0] = p2[0]; p2[0] = t; }
                        if (g) { u8 t = p1[1]; p1[1] = p2[1]; p2[1] = t; }
                        if (b) { u8 t = p1[2]; p1[2] = p2[2]; p2[2] = t; }

                        if (a && c == 4)
                        {
                            u8 t = p1[3]; p1[3] = p2[3]; p2[3] = t;
                        }
                    }
                }
            }
        }
        break;

        case AE_IMAGE_TYPE_FLT:
        {
            ae_image_t temp;

            ae_image_alloc_temp(&temp, image->width, image->height,
                                AE_IMAGE_TYPE_U8, image->format );

            ae_image_type_convert(&temp, image, AE_IMAGE_TYPE_U8);
            ae_image_flip_y(&temp, rect, r, g, b, a);

            ae_image_type_convert(image, &temp, AE_IMAGE_TYPE_FLT);
            ae_image_free_temp(&temp);
        }
        break;

        default: assert(0); break;
    }

    AE_PROFILE_LEAVE();
}

/* FIXME: This gives glitchy results that don't match the images present at the
 * convolution filter tutorial site (http://lodev.org/cgtutor/filtering.html).
 * Does the filter operation have to be out-of-place in order to work properly?
 * Do we need to get an image region, apply the op to it, and copy it back in?
 *
 * TODO: the alpha component is ignored entirely - is this the right approach?
 * TODO: consider different border modes (see GIMP's extend, wrap, and crop).
 * TODO: other GIMP convolution filter options (normalize and alpha weighting)?
 */
#if 0

typedef struct ae_image_convolution_filter_t
{
    void * image_pixels;
    size_t image_w;
    size_t image_h;

    const float* const matrix;
    size_t matrix_w;
    size_t matrix_h;

    const float* const factor;
    const float* const bias;

    int abs_below_zero;
    int r, g, b;
} \
    ae_image_convolution_filter_t;

static void ae_image_convolution_filter_u8(u8* pix, u8* end, size_t comp, void* data)
{
    ae_image_convolution_filter_t* args = (ae_image_convolution_filter_t*)data;

    u8* image_pixels = (u8*)args->image_pixels;
    size_t image_w = args->image_w;
    size_t image_h = args->image_h;

    const float* const matrix = args->matrix;
    size_t mx, matrix_w = args->matrix_w;
    size_t my, matrix_h = args->matrix_h;

    const float* const factor = args->factor;
    const float* const bias = args->bias;

    int abs_below_zero = args->abs_below_zero;
    int re = args->r;
    int ge = args->g;
    int be = args->b;

    // TODO: we could probably calculate this global image position outside of this function
    int iy = (pix - image_pixels) / (image_w * comp);
    int ix = (pix - (image_pixels + image_w * iy * comp)) / comp;

    for (; pix < end; pix += comp, ix++)
    {
        float r = 0.0f, g = 0.0f, b = 0.0f;

        for (mx = 0; mx < matrix_w; mx++)
        for (my = 0; my < matrix_h; my++)
        {
            // TODO: pull common subexpressions out to locals if the compiler doesn't already
            int x = (ix - matrix_w / 2 + mx + image_w) % image_w;
            int y = (iy - matrix_h / 2 + my + image_h) % image_h;

            r += image_pixels[y * image_w * comp + x * comp + 0] * matrix[my * matrix_w + mx];
            g += image_pixels[y * image_w * comp + x * comp + 1] * matrix[my * matrix_w + mx];
            b += image_pixels[y * image_w * comp + x * comp + 2] * matrix[my * matrix_w + mx];
        }

        // TODO: it would probably be faster to cast scale factor and bias outside the loop
        if (abs_below_zero)
        {
            if (re) pix[0] = ae_imin(ae_iabs((int)(factor[0] * r + bias[0])), 255);
            if (ge) pix[1] = ae_imin(ae_iabs((int)(factor[1] * g + bias[1])), 255);
            if (be) pix[2] = ae_imin(ae_iabs((int)(factor[2] * b + bias[2])), 255);
        }
        else
        {
            if (re) pix[0] = ae_imin(ae_imax((int)(factor[0] * r + bias[0]), 0), 255);
            if (ge) pix[1] = ae_imin(ae_imax((int)(factor[1] * g + bias[1]), 0), 255);
            if (be) pix[2] = ae_imin(ae_imax((int)(factor[2] * b + bias[2]), 0), 255);
        }
    }
}

void ae_image_convolution_filter(ae_image_t* image, int* rect,
                                const float* const matrix,
                                const size_t matrix_w,
                                const size_t matrix_h,
                                const float factor[3],
                                const float bias[3],
                                const int abs_below_zero,
                                int r, int g, int b)
{
    int full_image_rect[4] = { 0, 0, image->width, image->height };

    ae_image_convolution_filter_t args =
    {
        image->pixels,
        image->width,
        image->height,
        matrix,
        matrix_w,
        matrix_h,
        factor,
        bias,
        abs_below_zero,
        r, g, b,
    };

    ae_image_unary_op_t op;
    AE_PROFILE_ENTER();

    // HACK: need this rect to find the correct image x coordinate
    if (rect == NULL) rect = full_image_rect;

    memset(&op, 0, sizeof(ae_image_unary_op_t));
    op.name = "convolution_filter";

    op.u8_func[3] = op.u8_func[4] = ae_image_convolution_filter_u8;
    ae_image_unary_op(image, rect, &op, &args);

    AE_PROFILE_LEAVE();
}

#endif

/*
================================================================================
 * ~~ [ binary operations ] ~~ *
--------------------------------------------------------------------------------
TODO: ae_image_binary_clut with huge 64k tables: dst[i] = table[src[i]][dst[i]]
--------------------------------------------------------------------------------
*/

void ae_image_binary_op(ae_image_t* dst, const ae_image_t* const src, const int x,
                    const int y, const ae_image_binary_op_t* const op, void* data)
{
    int comp = ae_image_format_components[dst->format], sy, dy;

    int dst_box[4];
    int src_box[4];

    // for potential bugs in certain ops that don't expect overlapping memory
    // ae_assert(dst != src, "binary image op \"%s\" src == dst", op->name);

    ae_assert(src->format == dst->format, "op src format %s != dst format %s",
        ae_image_format_name[src->format], ae_image_format_name[dst->format]);

    ae_assert(src->type == dst->type, "op src type %s != dst type %s",
        ae_image_type_name[src->type], ae_image_type_name[dst->type]);

    ae_assert(op->num_tasks == 0, "multithreaded job queue not implemented");

    // TODO: should this just be caught in the bounds check & return early?
    ae_assert(dst->pixels != NULL && src->pixels != NULL, "dst %p src %p",
                                                dst->pixels, src->pixels);

    dst_box[0] = x; // flip y for bottom-left rect
    dst_box[1] = (dst->height - src->height) - y;

    dst_box[2] = x + (int)src->width;
    dst_box[3] = y + (int)src->height;

    src_box[0] = 0;
    src_box[1] = 0;

    src_box[2] = (int)src->width;
    src_box[3] = (int)src->height;

    // return early if the source image is outside the destination image
    if (x > (int)dst->width || y > (int)dst->height ||
        dst_box[2] < 0 || dst_box[3] < 0) goto fail_msg;

    // fix for case where source is larger than destination x or y axis
    if (x < 0 && dst_box[2] > (int)dst->width)
    {
        src_box[0] = -x;
        src_box[2] = dst->width + src_box[0];

        dst_box[0] = 0;
        dst_box[2] = dst->width;
    }

    if (y < 0 && dst_box[3] > (int)dst->height)
    {
        src_box[1] = src->height - dst->height + y;
        src_box[3] = dst->height + src_box [1] - y;

        dst_box[1] = 0;
        dst_box[3] = dst->height;
    }

    // ugly clamping of source bounding box to destination bounding box
    if (x < 0)
    {
        src_box[0] -= dst_box[0];
        dst_box[0] = 0;
    }

    if (dst_box[2] > (int)dst->width)
    {
        src_box[2] -= x - dst->width;
        dst_box[2] = (int)dst->width;
    }

    if (y < 0)
    {
        src_box[3] += y;
        dst_box[3] = (int)dst->height - (int)src->height;
    }

    if (dst_box[3] > (int)dst->height)
    {
        src_box[1] -= dst_box[1];
        dst_box[1] = 0;
    }

    ae_switch (dst->type, ae_image_type, AE_IMAGE_TYPE, suffix)
    {
        case AE_IMAGE_TYPE_U8:
        {
            void (*img_func)(u8* dst, u8* src, size_t pixels,
                size_t comp, void* data) = op->u8_func[comp];

            u8* dst_pixels = (u8*)dst->pixels;
            u8* src_pixels = (u8*)src->pixels;

            ae_if (img_func != NULL)
            {
                for (sy = src_box[1], dy = dst_box[1]; sy < src_box[3]; sy++, dy++)
                {
                    u8* dst_p = dst_pixels + dst_box[0] * comp + dy * dst->width * comp;
                    u8* src_p = src_pixels + src_box[0] * comp + sy * src->width * comp;

                    img_func(dst_p, src_p, dst_box[2] - dst_box[0], comp, data);
                }
            }
            else
            {
                if (op->flt_func[comp])
                {
                    ae_image_t a, b;

                    ae_image_alloc_temp(&a, dst->width, dst->height,
                                    AE_IMAGE_TYPE_FLT, dst->format);

                    ae_image_alloc_temp(&b, src->width, src->height,
                                    AE_IMAGE_TYPE_FLT, src->format);

                    ae_image_type_convert(&a, dst, AE_IMAGE_TYPE_FLT);
                    ae_image_type_convert(&b, src, AE_IMAGE_TYPE_FLT);

                    ae_image_binary_op(&a, &b, x, y, op, data);
                    ae_image_type_convert(dst, &a, AE_IMAGE_TYPE_U8);

                    ae_image_free_temp(&b);
                    ae_image_free_temp(&a);
                }
                else
                {
                    goto fail_msg;
                }
            }
        }
        break;

        case AE_IMAGE_TYPE_FLT:
        {
            void (*img_func)(float* dst, float* src, size_t pixels,
                    size_t comp, void * data) = op->flt_func[comp];

            float* dst_pixels = (float*)dst->pixels;
            float* src_pixels = (float*)src->pixels;

            ae_if (img_func != NULL)
            {
                for (sy = src_box[1], dy = dst_box[1]; sy < src_box[3]; sy++, dy++)
                {
                    float* dst_p = dst_pixels + dst_box[0] * comp + dy * dst->width * comp;
                    float* src_p = src_pixels + src_box[0] * comp + sy * src->width * comp;

                    img_func(dst_p, src_p, dst_box[2] - dst_box[0], comp, data);
                }
            }
            else
            {
                if (op->u8_func[comp])
                {
                    ae_image_t a, b;

                    ae_image_alloc_temp(&a, dst->width, dst->height,
                                    AE_IMAGE_TYPE_U8, dst->format);

                    ae_image_alloc_temp(&b, src->width, src->height,
                                    AE_IMAGE_TYPE_U8, src->format);

                    ae_image_type_convert(&a, dst, AE_IMAGE_TYPE_U8);
                    ae_image_type_convert(&b, src, AE_IMAGE_TYPE_U8);

                    ae_image_binary_op(&a, &b, x, y, op, data);
                    ae_image_type_convert(dst, &a, AE_IMAGE_TYPE_FLT);

                    ae_image_free_temp(&b);
                    ae_image_free_temp(&a);
                }
                else
                {
                    goto fail_msg;
                }
            }
        }
        break;

        default: assert(0); break;
    }

    /* TODO the type conversion fallback path for both u8 and float images is the
            same with different src and dst type arguments. we could assign those
            arguments to variables and goto a label here to save some code space.
    */

    return; fail_msg:
    {
        // TODO
    }
}

static void ae_image_binary_copy_u8(u8* dst, u8* src, size_t pixels, size_t comp, void* data)
{
    int r = ((int*)data)[0];
    int g = ((int*)data)[1];
    int b = ((int*)data)[2];
    int a = ((int*)data)[3];

    u8* end = dst + pixels * comp;
    for (; dst < end; dst += comp, src += comp)
    {
        if (r) dst[0] = src[0];
        if (g) dst[1] = src[1];
        if (b) dst[2] = src[2];

        if (a && comp == 4) dst[3] = src[3];
    }
}

static void ae_image_binary_copy_flt(float* dst, float* src, size_t pixels, size_t comp, void* data)
{
    int r = ((int*)data)[0];
    int g = ((int*)data)[1];
    int b = ((int*)data)[2];
    int a = ((int*)data)[3];

    float* end = dst + pixels * comp;
    for (; dst < end; dst += comp, src += comp)
    {
        if (r) dst[0] = src[0];
        if (g) dst[1] = src[1];
        if (b) dst[2] = src[2];

        if (a && comp == 4) dst[3] = src[3];
    }
}

static void ae_image_binary_copy_u8_fast(u8* dst, u8* src, size_t pixels, size_t comp, void* data)
{
    memcpy(dst, src, pixels * comp * sizeof(u8));
}

static void ae_image_binary_copy_flt_fast(float* dst, float* src, size_t pixels, size_t comp, void* data)
{
    memcpy(dst, src, pixels * comp * sizeof(float));
}

void ae_image_binary_copy(ae_image_t * dst, const ae_image_t * const src,
                    const int x, const int y, int r, int g, int b, int a)
{
    AE_PROFILE_ENTER();

    int rgba[4] = { r, g, b, a };
    ae_image_binary_op_t op;

    memset(&op, 0, sizeof(ae_image_binary_op_t));
    op.name = "binary_copy";

    if (r && g && b && a)
    {
        op.flt_func[3] = op.flt_func[4] = ae_image_binary_copy_flt_fast;
        op.u8_func[3] = op.u8_func[4] = ae_image_binary_copy_u8_fast;
    }
    else
    {
        op.flt_func[3] = op.flt_func[4] = ae_image_binary_copy_flt;
        op.u8_func[3] = op.u8_func[4] = ae_image_binary_copy_u8;
    }

    ae_image_binary_op(dst, src, x, y, &op, rgba);
    AE_PROFILE_LEAVE();
}

typedef struct ae_image_lerp_t
{
    int r, g, b, a;
    float t;
} \
    ae_image_lerp_t;

static void
ae_image_lerp_flt(float* dst, float* src, size_t pixels, size_t comp, void* data)
{
    const int r = ((ae_image_lerp_t*)data)->r;
    const int g = ((ae_image_lerp_t*)data)->g;
    const int b = ((ae_image_lerp_t*)data)->b;
    const int a = ((ae_image_lerp_t*)data)->a;

    const float t = ((ae_image_lerp_t*)data)->t;
    const float s = 1.0f - t;

    float* end = dst + pixels * comp;
    for (; dst < end; dst += comp, src += comp)
    {
        if (r) dst[0] = s * src[0] + t * dst[0];
        if (g) dst[1] = s * src[1] + t * dst[1];
        if (b) dst[2] = s * src[2] + t * dst[2];

        if (a && comp == 4)
        {
            dst[3] = s * src[3] + t * dst[3];
        }
    }
}

void ae_image_lerp(ae_image_t* dst, const ae_image_t* const src, const int x,
                            const int y, float t, int r, int g, int b, int a)
{
    void* outer = ae_profile_enter(__FILE__, "ae_image_lerp_outer");

    if (t == 1.0f)
    {
        ae_image_binary_copy(dst, src, x, y, r, g, b, a);
    }
    else if (t != 0.0f)
    {
        void* inner = ae_profile_enter(__FILE__, "ae_image_lerp_inner");

        // flip this to select source factor in destination
        ae_image_binary_op_t op;
        ae_image_lerp_t lerp = { r, g, b, a, 1.0f - t };

        memset(&op, 0, sizeof(ae_image_binary_op_t));
        op.name = "lerp";

        op.flt_func[3] = op.flt_func[4] = ae_image_lerp_flt;
        //op.u8_func[3] = op.u8_func[4] = ae_image_lerp_u8;

        ae_image_binary_op(dst, src, x, y, &op, &lerp);
        ae_profile_leave(inner); // show us how many times we just copy
    }

    ae_profile_leave(outer);
}

/*
================================================================================
 * ~~ [ blitting ] ~~ *
--------------------------------------------------------------------------------
TODO: tiled rendering across x and y axes, rotated blitting, subpixel precision
--------------------------------------------------------------------------------
*/

typedef struct ae_image_blit_t
{
    ae_image_blend_equation_t rgb_equation;
    ae_image_blend_equation_t alpha_equation;

    ae_image_blend_function_t src_rgb;
    ae_image_blend_function_t src_alpha;
    ae_image_blend_function_t dst_rgb;
    ae_image_blend_function_t dst_alpha;

    const float* const constant;
    const float* const src_scale;
    const float* const dst_scale;

    int r, g, b, a; // pass arguments through
} \
    ae_image_blit_t;

static float
ae_image_blit_function( const float * const src, const float * const dst,
                        const float * const constant, const size_t index,
                        const ae_image_blend_function_t function )
{
    ae_switch (function, ae_image_blend_function, AE_IMAGE_BLEND_FUNCTION, suffix)
    {
        case AE_IMAGE_BLEND_FUNCTION_ZERO:
            return 0.0f;

        case AE_IMAGE_BLEND_FUNCTION_ONE:
            return 1.0f;

        case AE_IMAGE_BLEND_FUNCTION_SRC_COLOR:
            return src[index];

        case AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_COLOR:
            return 1.0f - src[index];

        case AE_IMAGE_BLEND_FUNCTION_DST_COLOR:
            return dst[index];

        case AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_DST_COLOR:
            return 1.0f - dst[index];

        case AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA:
            return src[3];

        case AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_ALPHA:
            return 1.0f - src[3];

        case AE_IMAGE_BLEND_FUNCTION_DST_ALPHA:
            return dst[3];

        case AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_DST_ALPHA:
            return 1.0f - dst[3];

        case AE_IMAGE_BLEND_FUNCTION_CONSTANT_COLOR:
            return constant[index];

        case AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_CONSTANT_COLOR:
            return 1.0f - constant[index];

        case AE_IMAGE_BLEND_FUNCTION_CONSTANT_ALPHA:
            return constant[3];

        case AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_CONSTANT_ALPHA:
            return 1.0f - constant[3];

        case AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA_SATURATE:
            return ae_fminf(src[3], 1.0f - dst[3]);

        default:
            ae_assert(0, "%u", (u32)function); return 0.0f;
    }
}

static float
ae_image_blit_equation(float lhs, float rhs, ae_image_blend_equation_t equation)
{
    ae_switch (equation, ae_image_blend_equation, AE_IMAGE_BLEND_EQUATION, suffix)
    {
        case AE_IMAGE_BLEND_EQUATION_ADD:               return lhs + rhs;
        case AE_IMAGE_BLEND_EQUATION_SUBTRACT:          return lhs - rhs;
        case AE_IMAGE_BLEND_EQUATION_REVERSE_SUBTRACT:  return rhs - lhs;

        case AE_IMAGE_BLEND_EQUATION_MIN:               return ae_fminf(lhs, rhs);
        case AE_IMAGE_BLEND_EQUATION_MAX:               return ae_fmaxf(lhs, rhs);

        default: ae_assert(0, "%u", (u32)equation);     return 0.0f;
    }
}

static void
ae_image_blit_flt(float* dst, float* src, size_t pixels, size_t comp, void* data)
{
    AE_PROFILE_ENTER();

    const ae_image_blit_t* const args = (const ae_image_blit_t* const)data;

    const ae_image_blend_equation_t rgb_equation = args->rgb_equation;
    const ae_image_blend_equation_t alpha_equation = args->alpha_equation;

    const ae_image_blend_function_t src_rgb = args->src_rgb;
    const ae_image_blend_function_t src_alpha = args->src_alpha;

    const ae_image_blend_function_t dst_rgb = args->dst_rgb;
    const ae_image_blend_function_t dst_alpha = args->dst_alpha;

    const float* const constant = args->constant;

    const float* const src_scale = args->src_scale;
    const float* const dst_scale = args->dst_scale;

    const int r = args->r;
    const int g = args->g;
    const int b = args->b;
    const int a = args->a;

    float* end = dst + pixels * comp; // NOTE this is the slow generic alpha path
    for (; dst < end; dst += comp, src += comp)
    {
        if (r)
        {
            const float lR = src[0] * src_scale[0] *
                        ae_image_blit_function(src, dst, constant, 0, src_rgb);
            const float rR = dst[0] * dst_scale[0] *
                        ae_image_blit_function(src, dst, constant, 0, dst_rgb);

            dst[0] = ae_image_blit_equation(lR, rR, rgb_equation);
        }

        if (g)
        {
            const float lG = src[1] * src_scale[1] *
                        ae_image_blit_function(src, dst, constant, 1, src_rgb);
            const float rG = dst[1] * dst_scale[1] *
                        ae_image_blit_function(src, dst, constant, 1, dst_rgb);

            dst[1] = ae_image_blit_equation(lG, rG, rgb_equation);
        }

        if (b)
        {
            const float lB = src[2] * src_scale[2] *
                        ae_image_blit_function(src, dst, constant, 2, src_rgb);
            const float rB = dst[2] * dst_scale[2] *
                        ae_image_blit_function(src, dst, constant, 2, dst_rgb);

            dst[2] = ae_image_blit_equation(lB, rB, rgb_equation);
        }

        if (a)
        {
            const float lA = src[3] * src_scale[3] *
                        ae_image_blit_function(src, dst, constant, 3, src_alpha);
            const float rA = dst[3] * dst_scale[3] *
                        ae_image_blit_function(src, dst, constant, 3, dst_alpha);

            dst[3] = ae_image_blit_equation(lA, rA, alpha_equation);
        }
    }

    AE_PROFILE_LEAVE();
}

#if defined(__SSE__) && 0
static void ae_image_blit_transparent_sse( float* dst, float* src ,
                            size_t pixels, size_t comp, void* data)
{
    static const __m128 ae_blit_ones = { 1.0f, 1.0f, 1.0f, 1.0f };

    AE_PROFILE_ENTER(); // simd path
    float* end = dst + pixels * comp;

    ae_assert(num_is_aligned((size_t)dst, 16), "%p", dst);
    ae_assert(num_is_aligned((size_t)src, 16), "%p", src);

    for (; dst < end; dst += comp, src += comp)
    {
        const __m128 s_alpha = _mm_set1_ps(src[3]);
        const __m128 d_alpha = _mm_sub_ps(ae_blit_ones, s_alpha);

        const __m128 src_128 = _mm_load_ps(src);
        const __m128 dst_128 = _mm_load_ps(dst);

        const __m128 src_lhs = _mm_mul_ps(src_128, s_alpha);
        const __m128 dst_rhs = _mm_mul_ps(dst_128, d_alpha);
        const __m128 add_res = _mm_add_ps(src_lhs, dst_rhs);

        _mm_store_ps(dst, add_res);
    }

    AE_PROFILE_LEAVE();
}
#endif

static void ae_image_blit_transparent_flt( float* dst, float* src ,
                            size_t pixels, size_t comp, void* data)
{
    AE_PROFILE_ENTER();

    float* end = dst + pixels * comp; // faster path
    for (; dst < end; dst += comp, src += comp)
    {
        const float s_alpha = src[3]; /* functions */
        const float d_alpha = 1.0f - s_alpha;

        dst[0] = src[0] * s_alpha + dst[0] * d_alpha;
        dst[1] = src[1] * s_alpha + dst[1] * d_alpha;
        dst[2] = src[2] * s_alpha + dst[2] * d_alpha;
        dst[3] = src[3] * s_alpha + dst[3] * d_alpha;
    }

    AE_PROFILE_LEAVE();
}

static void ae_image_blit_transparent_u8( u8* dst, u8* src,
                    size_t pixels, size_t comp, void* data)
{
    // TODO: this is a temporary stopgap solution to the awful performance hit incurred by
    // image type conversion. this should eventually be replaced by a binary lookup table.
    AE_PROFILE_ENTER();

    u8* end = dst + pixels * comp;
    for (; dst < end; dst += comp, src += comp)
    {
        switch (src[3])
        {
            case 0: break;

            case 255:
                memcpy(dst, src, 4); break;

            default:
            {
                const float sR = ae_image_ldr_to_hdr_table[src[0]];
                const float sG = ae_image_ldr_to_hdr_table[src[1]];
                const float sB = ae_image_ldr_to_hdr_table[src[2]];
                const float sA = src[3] / 255.0f;

                const float dR = ae_image_ldr_to_hdr_table[dst[0]];
                const float dG = ae_image_ldr_to_hdr_table[dst[1]];
                const float dB = ae_image_ldr_to_hdr_table[dst[2]];
                const float dA = dst[3] / 255.0f;

                const float bR = sR * sA + dR * (1.0f - sA);
                const float bG = sG * sA + dG * (1.0f - sA);
                const float bB = sB * sA + dB * (1.0f - sA);
                const float bA = sA * sA + dA * (1.0f - sA);

                dst[0] = (u8)stbi__float2int(ae_fclampf((float)pow(bR * stbi__h2l_scale_i,
                                        stbi__h2l_gamma_i) * 255 + 0.5f, 0.0f, 255.0f));

                dst[1] = (u8)stbi__float2int(ae_fclampf((float)pow(bG * stbi__h2l_scale_i,
                                        stbi__h2l_gamma_i) * 255 + 0.5f, 0.0f, 255.0f));

                dst[2] = (u8)stbi__float2int(ae_fclampf((float)pow(bB * stbi__h2l_scale_i,
                                        stbi__h2l_gamma_i) * 255 + 0.5f, 0.0f, 255.0f));

                dst[3] = (u8)stbi__float2int(ae_fclampf(bA * 255 + 0.5f, 0.0f, 255.0f));
            }
            break;
        }
    }

    AE_PROFILE_LEAVE();
}

/* ===== [ public API ] ===================================================== */

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
    void* outer = ae_profile_enter(__FILE__, "ae_image_blit_ex_outer");

    // TODO: should be trivial to blit RGB images into RGBA & vice versa
    if (src->format == AE_IMAGE_FORMAT_RGB)
    {
        ae_image_binary_copy(dst, src, x, y, r, g, b, a);
    }
    else
    {
        void* inner = ae_profile_enter(__FILE__, "ae_image_blit_ex_inner");
        static const float f_ones[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

        // TODO: scale src_scale by r, g, b, a bools to avoid branching
        ae_image_binary_op_t op;
        ae_image_blit_t blit = \
        {
            rgb_equation, alpha_equation, src_rgb, src_alpha,
            dst_rgb, dst_alpha, constant ? constant : f_ones,
            src_scale ? src_scale : f_ones,
            dst_scale ? dst_scale : f_ones, r, g, b, a
        };

        memset(&op, 0, sizeof(ae_image_binary_op_t));
        op.name = "blit_ex";

        op.flt_func[4] = ae_image_blit_flt;

        if (r && g && b && a && // avoid tons of branches per pixel?
            rgb_equation == AE_IMAGE_BLEND_EQUATION_ADD &&
            alpha_equation == AE_IMAGE_BLEND_EQUATION_ADD &&
            src_scale == NULL && dst_scale == NULL && constant == NULL &&
            src_rgb == AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA &&
            src_alpha == AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA &&
            dst_rgb == AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_ALPHA &&
            dst_alpha == AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_ALPHA)
        {
            #if defined(__SSE__) && 0
            if(ae_cpuinfo_sse())
            {
                // 2x slower in debug, memory bound in release
                op.flt_func[4] = ae_image_blit_transparent_sse;
            }
            else
            #endif
            {
                op.flt_func[4] = ae_image_blit_transparent_flt;
            }

            op.u8_func[4] = ae_image_blit_transparent_u8;
        }

        ae_image_binary_op(dst, src, x, y, &op, &blit);
        ae_profile_leave(inner); // track rgb renders vs. alpha renders
    }

    ae_profile_leave(outer);
}

void ae_image_blit(ae_image_t * dst, const ae_image_t * const src,
            const int x, const int y, int r, int g, int b, int a)
{
    ae_image_blit_ex(dst, src, x, y, // standard alpha blending
                    AE_IMAGE_BLEND_EQUATION_ADD,
                    AE_IMAGE_BLEND_EQUATION_ADD,
                    AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA,
                    AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA,
                    AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_ALPHA,
                    AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_ALPHA,
                    NULL, NULL, NULL, r, g, b, a);
}

/* ===== [ unary rects ] ==================================================== */

static void ae_image_blit_rect_flt(float* pix, float* end, size_t comp, void* data)
{
    AE_PROFILE_ENTER(); // this is the generic slow path for rect blitting!
    const ae_image_blit_t* const args = (const ae_image_blit_t* const)data;

    const ae_image_blend_equation_t rgb_equation = args->rgb_equation;
    const ae_image_blend_equation_t alpha_equation = args->alpha_equation;

    const ae_image_blend_function_t src_rgb = args->src_rgb;
    const ae_image_blend_function_t src_alpha = args->src_alpha;

    const ae_image_blend_function_t dst_rgb = args->dst_rgb;
    const ae_image_blend_function_t dst_alpha = args->dst_alpha;

    const float* const constant = args->constant;

    const float* const src_scale = args->src_scale;
    const float* const dst_scale = args->dst_scale;

    const int r = args->r;
    const int g = args->g;
    const int b = args->b;
    const int a = args->a;

    for (; pix < end; pix += comp)
    {
        if (r)
        {
            const float lR = src_scale[0] * 1.0f *
                ae_image_blit_function(src_scale, pix, constant, 0, src_rgb);

            const float rR = dst_scale[0] * pix[0] *
                ae_image_blit_function(src_scale, pix, constant, 0, dst_rgb);

            pix[0] = ae_image_blit_equation(lR, rR, rgb_equation);
        }

        if (g)
        {
            const float lG = src_scale[1] * 1.0f *
                ae_image_blit_function(src_scale, pix, constant, 1, src_rgb);

            const float rG = dst_scale[1] * pix[1] *
                ae_image_blit_function(src_scale, pix, constant, 1, dst_rgb);

            pix[1] = ae_image_blit_equation(lG, rG, rgb_equation);
        }

        if (b)
        {
            const float lB = src_scale[2] * 1.0f *
                ae_image_blit_function(src_scale, pix, constant, 2, src_rgb);

            const float rB = dst_scale[2] * pix[2] *
                ae_image_blit_function(src_scale, pix, constant, 2, dst_rgb);

            pix[2] = ae_image_blit_equation(lB, rB, rgb_equation);
        }

        if (a)
        {
            const float lA = src_scale[3] * 1.0f *
                ae_image_blit_function(src_scale, pix, constant, 3, src_alpha);

            const float rA = dst_scale[3] * pix[3] *
                ae_image_blit_function(src_scale, pix, constant, 3, dst_alpha);

            pix[3] = ae_image_blit_equation(lA, rA, alpha_equation);
        }
    }

    AE_PROFILE_LEAVE();
}

static void
ae_image_blit_rect_transparent_flt(float* pix, float* end, size_t comp, void* data)
{
    AE_PROFILE_ENTER();
    const float* const src_scale = ((const ae_image_blit_t* const)data)->src_scale;

    for (; pix < end; pix += comp)
    {
        pix[0] = src_scale[0] * pix[3] + pix[0] * (1.0f - src_scale[3]);
        pix[1] = src_scale[1] * pix[3] + pix[1] * (1.0f - src_scale[3]);
        pix[2] = src_scale[2] * pix[3] + pix[2] * (1.0f - src_scale[3]);
        pix[3] = src_scale[3] * pix[3] + pix[3] * (1.0f - src_scale[3]);
    }

    AE_PROFILE_LEAVE();
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
    void* outer = ae_profile_enter(__FILE__, "ae_image_blit_rect_ex_outer");

    if (image->format == AE_IMAGE_FORMAT_RGB ||
        src_scale ? src_scale[3] == 1.0f : 0)
    {
        ae_image_set_color(image, rect, src_scale, r, g, b, a);
    }
    else
    {
        void* inner = ae_profile_enter(__FILE__, "ae_image_blit_rect_ex_inner");
        static const float f_ones[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

        // TODO: scale src_scale by r, g, b, a bools to avoid branching
        ae_image_unary_op_t op;
        ae_image_blit_t blit = \
        {
            rgb_equation, alpha_equation, src_rgb, src_alpha,
            dst_rgb, dst_alpha, constant ? constant : f_ones,
            src_scale ? src_scale : f_ones,
            dst_scale ? dst_scale : f_ones, r, g, b, a
        };

        memset(&op, 0, sizeof(ae_image_unary_op_t));
        op.name = "blit_rect_ex";

        // TODO: it should be really easy to make a lookup table for u8
        op.flt_func[4] = ae_image_blit_rect_flt;

        if (r && g && b && a && // avoid tons of branches at each pixel
            rgb_equation   == AE_IMAGE_BLEND_EQUATION_ADD &&
            alpha_equation == AE_IMAGE_BLEND_EQUATION_ADD &&
            src_scale != NULL && dst_scale == NULL && constant == NULL &&
            src_rgb   == AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA &&
            src_alpha == AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA &&
            dst_rgb   == AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_ALPHA &&
            dst_alpha == AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_ALPHA)
        {
            op.flt_func[4] = ae_image_blit_rect_transparent_flt;
        }

        ae_image_unary_op(image, rect, &op, &blit);
        ae_profile_leave(inner);
    }

    ae_profile_leave(outer);
}

void ae_image_blit_rect(ae_image_t * image, int * rect, float * color,
                        int r, int g, int b, int a)
{
    ae_image_blit_rect_ex(image, rect, // standard transparent blend
                        AE_IMAGE_BLEND_EQUATION_ADD,
                        AE_IMAGE_BLEND_EQUATION_ADD,
                        AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA,
                        AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA,
                        AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_ALPHA,
                        AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_ALPHA,
                        NULL, color, NULL, r, g, b, a);
}

/* ===== [ unary lines ] ==================================================== */

void ae_image_blit_line_horizontal_ex(ae_image_t* image,
                int x, int y, int width, float * color)
{
    AE_PROFILE_ENTER(); // HACK: just blit a skinny rect
    int rect[] = { x, y, width, 1 };

    ae_image_blit_rect(image, rect, color, 1, 1, 1, 1);
    AE_PROFILE_LEAVE();
}

void ae_image_blit_line_vertical_ex( ae_image_t* image,
                int x, int y, int height, float* color)
{
    /* This seems like the worst possible performance case. We're calling the
     * rect blit scanline callback for individual pixels along the vertical!
     * The profiler, however, demonstrates to us the forgiving nature of x86.
     * This is only about 20% slower than the horizontal line drawing code!
     */
    AE_PROFILE_ENTER(); // HACK: just blit a skinny rect
    int rect[] = { x, y, 1, height };

    ae_image_blit_rect(image, rect, color, 1, 1, 1, 1);
    AE_PROFILE_LEAVE();
}

void ae_image_blit_line_horizontal(ae_image_t* image,
                int x1, int y, int x2, float* color)
{
    ae_image_blit_line_horizontal_ex(image, x1, y, x2 - x1 + 1, color);
}

void ae_image_blit_line_vertical( ae_image_t* image,
                int x, int y1, int y2, float* color)
{
    ae_image_blit_line_vertical_ex(image, x, y1, y2 - y1 + 1, color);
}

void ae_image_blit_rect_outline(ae_image_t* image, int* rect, float* color)
{
    AE_PROFILE_ENTER(); // if the destination rect is NULL, draw a border around the entire image

    int fixed_rect[] = { 0, 0, (int)image->width, (int)image->height };
    if (rect == NULL) rect = fixed_rect;

    // top
    ae_image_blit_line_horizontal_ex(image, rect[0], rect[1] + rect[3] - 1, rect[2], color);

    // bottom
    ae_image_blit_line_horizontal_ex(image, rect[0], rect[1], rect[2], color);

    // left
    ae_image_blit_line_vertical_ex(image, rect[0], rect[1] + 1, rect[3] - 2, color);

    // right
    ae_image_blit_line_vertical_ex(image, rect[0] + rect[2] - 1, rect[1] + 1, rect[3] - 2, color);

    AE_PROFILE_LEAVE();
}

void ae_image_blit_grid_outline(ae_image_t * image, int * origin,
            const size_t num_x, const size_t num_y, float* color)
{
    AE_PROFILE_ENTER(); // TODO: if origin is NULL, just outline

    int nx = (int)num_x, ny = (int)num_y, x, y, current[4];
    int iw = (int)image->width, ih = (int)image->height;

    ae_assert(origin, "got NULL pointer for first rect in grid");
    memcpy(current + 2, origin + 2, sizeof(int[2]));

    for(y = 0, current[1] = origin[1];
        y < ny && current[1] < ih;
        y++, current[1] += current[3])

    for(x = 0, current[0] = origin[0];
        x < nx && current[0] < iw;
        x++, current[0] += current[2])
    {
    #if 0
        color[0] = ae_random_flt();
        color[1] = ae_random_flt();
        color[2] = ae_random_flt();
    #endif
        ae_image_blit_rect_outline(image, current, color);
    }

    AE_PROFILE_LEAVE();
}

/*
================================================================================
 * ~~ [ regions ] ~~ *
--------------------------------------------------------------------------------
*/

static void ae_image_get_region_u8(u8* pix, u8* end, size_t comp, void* data)
{
    size_t row_bytes = (size_t)(end - pix);
    ae_image_t* dst = (ae_image_t*)data;

    memmove(dst->pixels, pix, row_bytes);

    // hack to avoid keeping an extra pointer around, which relies on the unary
    // op going top-down. we have to write this in a slightly weird way for gcc.
    dst->pixels = (u8*)dst->pixels + row_bytes;
}

static void ae_image_get_region_flt(float* pix, float* end, size_t comp, void* data)
{
    ae_image_get_region_u8((u8*)pix, (u8*)end, comp, data);
}

void ae_image_get_region(ae_image_t* dst, ae_image_t* src, int* rect)
{
    AE_PROFILE_ENTER();

    if (rect)
    {
        // TODO: share function-ized clamping code with unary op
        int box[4], clamped_rect[4];

        int w = (int)src->width;
        int h = (int)src->height;

        ae_image_t temp;

        ae_image_unary_op_t op; // scanline callback
        memset(&op, 0, sizeof(ae_image_unary_op_t));

        // convert input rect to axis-aligned bounding box
        box[0] = rect[0];
        box[1] = rect[1];

        box[2] = rect[0] + rect[2];
        box[3] = rect[1] + rect[3];

        // clamp the bounding box to the image's dimensions
        if (box[0] < 0) box[0] = 0;
        if (box[0] > w) box[0] = w;

        if (box[1] < 0) box[1] = 0;
        if (box[1] > h) box[1] = h;

        if (box[2] < 0) box[2] = 0;
        if (box[2] > w) box[2] = w;

        if (box[3] < 0) box[3] = 0;
        if (box[3] > h) box[3] = h;

        // rect dimensions may have changed after clamping
        clamped_rect[0] = box[0];
        clamped_rect[1] = box[1];

        clamped_rect[2] = box[2] - box[0];
        clamped_rect[3] = box[3] - box[1];

        temp.width = clamped_rect[2];
        temp.height = clamped_rect[3];
        temp.format = src->format;
        temp.type = src->type;
        ae_image_alloc_fit(dst, &temp);

        op.name = "get_region";

        op.flt_func[3] = op.flt_func[4] = ae_image_get_region_flt;
        op.u8_func[3] = op.u8_func[4] = ae_image_get_region_u8;

        ae_image_unary_op(src, clamped_rect, &op, dst);

        // move the pixel pointer back to its original position
        dst->pixels = (u8*)dst->pixels - ae_image_bytes(dst);
    }
    else
    {
        ae_image_copy(dst, src);
    }

    AE_PROFILE_LEAVE();
}

/*
================================================================================
 * ~~ [ texture atlas packing ] ~~ *
--------------------------------------------------------------------------------
*/

// TODO

/*
================================================================================
 * ~~ [ flood filling ] ~~ *
--------------------------------------------------------------------------------
*/

// TODO

/*
================================================================================
 * ~~ [ initialization ] ~~ *
--------------------------------------------------------------------------------
*/

void ae_image_init(int argc, char** argv)
{
    #ifndef AE_IMAGE_HEAP_RESERVE // reserve 8 mb for image allocs
    #define AE_IMAGE_HEAP_RESERVE (1024 * 1024 * 8)
    #endif
    ae_heap_create(&ae_image_heap, "image", AE_IMAGE_HEAP_RESERVE);

    devil_init(argc, argv);
}

void ae_image_quit(void)
{
    devil_quit(); // close codec lib
    ae_heap_destroy(&ae_image_heap);
}
