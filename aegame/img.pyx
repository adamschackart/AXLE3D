# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
# TODO: Image.pixel_index - get R component offset of (x,y) in floats or bytes
# ------------------------------------------------------------------------------
from libc.string cimport memset

from vec cimport *
from mem cimport *

import sys # version info

cdef extern from "ae_math.h":
    # ===== [ rects ] ==========================================================

    void flt_rect_copy(float* dst, const float* const src)
    void int_rect_copy(int* dst, const int* const src)

    int flt_rect_cmp(const float* const a, const float* const b)
    int int_rect_cmp(const int* const a, const int* const b)

    int flt_rect_cmp_e(const float* const a, const float* const b,
                        const float epsilon) # fuzzy rect compare

    void flt_rect_zero(float rect[4])
    void int_rect_zero(int rect[4])

    int flt_rect_is_zero(const float rect[4])
    int int_rect_is_zero(const int rect[4])

    int flt_rect_is_zero_e(const float rect[4], const float epsilon)

    # ===== [ center ] =========================================================

    void flt_rect_get_center(float center[2], const float rect[4])
    void int_rect_get_center(int center[2], const int rect[4])

    void flt_rect_set_center(float rect[4], const float center[2])
    void int_rect_set_center(int rect[4], const int center[2])

    void flt_rect_center_in(float rect[4], const float target[4])
    void int_rect_center_in(int rect[4], const int target[4])

    # ===== [ area ] ===========================================================

    float flt_rect_area(const float rect[4])
    int int_rect_area(const int rect[4])

    # ===== [ aabbox conversion ] ==============================================

    void flt_rect_to_aabbox(float mn[2], float mx[2], const float rect[4])
    void int_rect_to_aabbox(int mn[2], int mx[2], const int rect[4])

    void flt_rect_from_aabbox(float rect[4], const float mn[2], const float mx[2])
    void int_rect_from_aabbox(int rect[4], const int mn[2], const int mx[2])

    # ===== [ collision detection ] ============================================

    int flt_rect_vs_rect(const float a[4], const float b[4])
    int int_rect_vs_rect(const int a[4], const int b[4])

    int flt_rect_vs_point(const float rect[4], const float point[2])
    int int_rect_vs_point(const int rect[4], const int point[2])

    # ===== [ normalized coordinates ] =========================================

    void flt_rect_point_to_normalized(float out[2],
        const float rect[4], const float rpoint[2])

    void flt_rect_normalized_to_point(float out[2],
        const float rect[4], const float normal[2])

    # ===== [ type conversion ] ================================================

    void flt_rect_to_int_ex(int out[4], const float rect[4],
                            const ae_ftoi_mode_t mode)

    void flt_rect_to_int(int out[4], const float rect[4])
    void int_rect_to_flt(float out[4], const int rect[4])

    # ===== [ rotated bounds ] =================================================

    void flt_rect_rotated_bounds_ex(float out[4], const float rect[4],
                                    const float center[2],
                                    const float sin_theta,
                                    const float cos_theta)

    void flt_rect_rotated_bounds( float out_v[4], const float rect[4],
                            const float center[2], const float theta)

cdef extern from "ae_image.h":
    # ===== [ image format enum ] ==============================================

    ctypedef enum ae_image_format_t:
        AE_IMAGE_FORMAT_RGBA
        AE_IMAGE_FORMAT_RGB
        AE_IMAGE_FORMAT_MONO
        AE_IMAGE_FORMAT_COUNT

    size_t ae_image_format_components[]
    char * ae_image_format_name[]

    # ===== [ pixel type enum ] ================================================

    ctypedef enum ae_image_type_t:
        AE_IMAGE_TYPE_U8
        AE_IMAGE_TYPE_FLT
        AE_IMAGE_TYPE_COUNT

    size_t ae_image_type_bytes[]
    char * ae_image_type_name[]

    # ===== [ error codes ] ====================================================

    ctypedef enum ae_image_error_t:
        AE_IMAGE_SUCCESS
        AE_IMAGE_NO_CODEC
        AE_IMAGE_FILE_NOT_FOUND
        AE_IMAGE_FILE_CORRUPT
        AE_IMAGE_WRITE_FAILED
        AE_IMAGE_ERROR_COUNT

    char* ae_image_error_name[] # string names for errors, and full str messages
    const char* ae_image_error_message(ae_image_error_t c, const char* filename)

    # ===== [ image struct ] ===================================================

    ctypedef struct ae_image_t:
        void* pixels
        size_t width, height
        ae_image_format_t format
        ae_image_type_t type
        void* _owner

    size_t ae_image_bytes(const ae_image_t* const image)
    size_t ae_image_pitch(const ae_image_t* const image)

    void * ae_image_pixel(const ae_image_t* const image, size_t x, size_t y)

    # ===== [ memory management ] ==============================================

    void ae_image_alloc(ae_image_t* image)
    void ae_image_alloc_fit(ae_image_t* dst, const ae_image_t* const src)
    void ae_image_free(ae_image_t* image)
    void ae_image_copy(ae_image_t* dst, const ae_image_t* const src)

    # ===== [ memory codecs ] ==================================================

    ae_image_error_t ae_image_info_from_memory(ae_image_t * image,
                    const void* const buffer, const size_t length)

    ae_image_error_t ae_image_load_from_memory(ae_image_t * image,
                    const void* const buffer, const size_t length)

    # ===== [ file codecs ] ====================================================

    ae_image_error_t ae_image_info(ae_image_t* image, const char* filename)
    ae_image_error_t ae_image_load(ae_image_t* image, const char* filename)
    ae_image_error_t ae_image_save(ae_image_t* image, const char* filename)

    # ===== [ image archives ] =================================================

    ae_image_error_t ae_image_archive_load(ae_image_t** images, const char* filename)
    ae_image_error_t ae_image_archive_save(ae_image_t* images, const char* filename)

    ae_image_error_t ae_image_archive_save_ex(const char** filenames,
                                        const char* archive_filename)

    # ===== [ data conversion ] ================================================

    void ae_image_type_convert(ae_image_t* dst, const ae_image_t* const src,
                                                const ae_image_type_t type)

    void ae_image_format_convert(ae_image_t* dst, const ae_image_t* const src,
                                            const ae_image_format_t format)

    # ===== [ resizing ] =======================================================

    ctypedef enum ae_image_resize_filter_t:
        AE_IMAGE_RESIZE_FILTER_DEFAULT
        AE_IMAGE_RESIZE_FILTER_BOX
        AE_IMAGE_RESIZE_FILTER_TRIANGLE
        AE_IMAGE_RESIZE_FILTER_CUBIC_B_SPLINE
        AE_IMAGE_RESIZE_FILTER_CATMULL_ROM
        AE_IMAGE_RESIZE_FILTER_MITCHELL
        AE_IMAGE_RESIZE_FILTER_COUNT

    char* ae_image_resize_filter_name[]

    void ae_image_resize_ex(ae_image_t* dst, const ae_image_t* const src,
                        const size_t new_width, const size_t new_height,
                        ae_image_resize_filter_t filter)

    void ae_image_resize(ae_image_t* dst, const ae_image_t* const src,
                    const size_t new_width, const size_t new_height)

    # ===== [ unary operations ] ===============================================

    ctypedef struct ae_image_unary_op_t:
        const char* name
        size_t num_tasks

        void (*flt_func[5])(float* start, float* end, size_t comp, void* data)
        void (*u8_func[5])(u8* start, u8* end, size_t comp, void* data)

    void ae_image_unary_op(ae_image_t* image, int* rect,
                    ae_image_unary_op_t* op, void* data)

    void ae_image_unary_clut(ae_image_t* image,
        int* rect, u8* r, u8* g, u8* b, u8* a)

    void ae_image_negative(ae_image_t* image, int* rect, int r, int g, int b)
    void ae_image_solarize(ae_image_t* image, int* rect, u8 threshold, int r, int g, int b)

    void ae_image_greyscale(ae_image_t* image, int* rect, int r, int g, int b)
    void ae_image_threshold(ae_image_t* image, int* rect, u8 threshold, int r, int g, int b)
    void ae_image_bleach(ae_image_t* image, int* rect, u8 threshold, int r, int g, int b)

    void ae_image_neonize(ae_image_t * image, int * rect, u8 threshold,
                        int lr, int lg, int lb, int hr, int hg, int hb)

    void ae_image_pastelize(ae_image_t* image, int* rect, u8 threshold,
                        int lr, int lg, int lb, int hr, int hg, int hb)

    void ae_image_isolate_channel(ae_image_t* image,
        int* rect, int channel, int r, int g, int b)

    void ae_image_set_color(ae_image_t * image, int * rect, const float color[4],
                            int r, int g, int b, int a)

    void ae_image_scale_color(ae_image_t* image, int* rect, const float scale[4])

    void ae_image_flip_x(ae_image_t* image, int* rect, int r, int g, int b, int a)
    void ae_image_flip_y(ae_image_t* image, int* rect, int r, int g, int b, int a)

    # ===== [ binary operations ] ==============================================

    ctypedef struct ae_image_binary_op_t:
        const char* name
        size_t num_tasks

        void (*flt_func[5])(float* dst, float* src, size_t pixels, size_t comp, void* data)
        void (*u8_func[5])(u8* dst, u8* src, size_t pixels, size_t comp, void* data)

    void ae_image_binary_op(ae_image_t* dst, const ae_image_t* const src, const int x,
                        const int y, const ae_image_binary_op_t* const op, void* data)

    void ae_image_binary_copy(ae_image_t * dst, const ae_image_t * const src,
                        const int x, const int y, int r, int g, int b, int a)

    void ae_image_lerp(ae_image_t* dst, const ae_image_t* const src, const int x,
                                const int y, float t, int r, int g, int b, int a)

    # ===== [ blitting ] =======================================================

    ctypedef enum ae_image_blend_function_t:
        AE_IMAGE_BLEND_FUNCTION_ZERO
        AE_IMAGE_BLEND_FUNCTION_ONE
        AE_IMAGE_BLEND_FUNCTION_SRC_COLOR
        AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_COLOR
        AE_IMAGE_BLEND_FUNCTION_DST_COLOR
        AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_DST_COLOR
        AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA
        AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_ALPHA
        AE_IMAGE_BLEND_FUNCTION_DST_ALPHA
        AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_DST_ALPHA
        AE_IMAGE_BLEND_FUNCTION_CONSTANT_COLOR
        AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_CONSTANT_COLOR
        AE_IMAGE_BLEND_FUNCTION_CONSTANT_ALPHA
        AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_CONSTANT_ALPHA
        AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA_SATURATE

    const char* ae_image_blend_function_name[]

    ctypedef enum ae_image_blend_equation_t:
        AE_IMAGE_BLEND_EQUATION_ADD
        AE_IMAGE_BLEND_EQUATION_SUBTRACT
        AE_IMAGE_BLEND_EQUATION_REVERSE_SUBTRACT
        AE_IMAGE_BLEND_EQUATION_MIN
        AE_IMAGE_BLEND_EQUATION_MAX

    const char* ae_image_blend_equation_name[]

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

    void ae_image_blit(ae_image_t * dst, const ae_image_t * const src,
                const int x, const int y, int r, int g, int b, int a)

    # ===== [ unary rects ] ====================================================

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

    void ae_image_blit_rect(ae_image_t * image, int * rect,
                float * color, int r, int g, int b, int a)

    # ===== [ unary lines ] ====================================================

    void ae_image_blit_line_horizontal_ex( ae_image_t * image,
                        int x, int y, int width, float* color)

    void ae_image_blit_line_vertical_ex( ae_image_t* image,
                    int x, int y, int height, float* color)

    void ae_image_blit_line_horizontal(ae_image_t* image,
                    int x1, int y, int x2, float* color)

    void ae_image_blit_line_vertical( ae_image_t* image,
                    int x, int y1, int y2, float* color)

    void ae_image_blit_rect_outline(ae_image_t* image, int* rect, float* color)

    void ae_image_blit_grid_outline(ae_image_t * image, int * origin,
                const size_t num_x, const size_t num_y, float* color)

    # ===== [ regions ] ========================================================

    void ae_image_get_region(ae_image_t* dst, ae_image_t* src, int* rect)

# ==============================================================================
# ~ [ rectangles ]
# ==============================================================================

cdef class FloatRect:
    cdef float rect[4] # this is for ops that require subpixel precision.

    def __init__(self, float x=0, float y=0, float w=0, float h=0):
        self.rect[0] = x
        self.rect[1] = y
        self.rect[2] = w
        self.rect[3] = h

    def __repr__(self):
        return "{}(x:{} y:{} w:{} h:{})".format(self.__class__.__name__,
                self.rect[0], self.rect[1], self.rect[2], self.rect[3])

    def __richcmp__(self, FloatRect other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: FloatRect')

    def __copy__(self):
        cdef FloatRect other = FloatRect()

        flt_rect_copy(other.rect, self.rect)
        return other

    copy = __copy__

    def __nonzero__(self):
        return not flt_rect_is_zero(self.rect)

    property x:
        def __get__(self): return self.rect[0]
        def __set__(self, float v): self.rect[0] = v

    property y:
        def __get__(self): return self.rect[1]
        def __set__(self, float v): self.rect[1] = v

    property w:
        def __get__(self): return self.rect[2]
        def __set__(self, float v): self.rect[2] = v

    property h:
        def __get__(self): return self.rect[3]
        def __set__(self, float v): self.rect[3] = v

    property pos:
        def __get__(self):
            cdef Vec2 pos = Vec2()

            pos.v[0] = self.rect[0]
            pos.v[1] = self.rect[1]

            return pos

        def __set__(self, Vec2 pos):
            self.rect[0] = pos.v[0]
            self.rect[1] = pos.v[1]

    property dim:
        def __get__(self):
            cdef Vec2 dim = Vec2()

            dim.v[0] = self.rect[2]
            dim.v[1] = self.rect[3]

            return dim

        def __set__(self, Vec2 dim):
            self.rect[2] = dim.v[0]
            self.rect[3] = dim.v[1]

    property center:
        def __get__(self):
            cdef Vec2 center = Vec2()

            flt_rect_get_center(center.v, self.rect)
            return center

        def __set__(self, Vec2 center):
            flt_rect_set_center(self.rect, center.v)

    property area:
        def __get__(self): return flt_rect_area(self.rect)

    def __len__(self): return 4

    def __getitem__(self, int key):
        if key < 0 or key > 3: raise IndexError
        return self.rect[key]

    def __setitem__(self, int key, float value):
        if key < 0 or key > 3: raise IndexError
        self.rect[key] = value

    def __reduce__(self):
        cdef bytes b = b'\0' * sizeof(float[4])
        cdef char* p = <char *>b

        flt_rect_copy(<float*>p, self.rect)
        return (FloatRect, (), b)

    def __setstate__(self, bytes state):
        cdef char* p = <char*>state
        flt_rect_copy(self.rect, <float*>p)

    def address(self):
        return <size_t>self.rect

    def cmp(self, FloatRect other):
        return flt_rect_cmp(self.rect, other.rect)

    def cmp_e(self, FloatRect other, float epsilon):
        return flt_rect_cmp_e(self.rect, other.rect, epsilon)

    def zero(self):
        flt_rect_zero(self.rect); return self

    def is_zero(self):
        return flt_rect_is_zero(self.rect)

    def is_zero_e(self, float epsilon):
        return flt_rect_is_zero_e(self.rect, epsilon)

    def center_in(self, FloatRect other):
        flt_rect_center_in(self.rect, other.rect); return self

    def intersect_rect(self, FloatRect other):
        return flt_rect_vs_rect(self.rect, other.rect)

    def intersect_point(self, Vec2 point):
        return flt_rect_vs_point(self.rect, point.v)

    def to_aabbox(self):
        cdef Vec2 minv = Vec2()
        cdef Vec2 maxv = Vec2()

        flt_rect_to_aabbox(minv.v, maxv.v, self.rect)
        return (minv, maxv)

    def from_aabbox(self, Vec2 minv, Vec2 maxv):
        flt_rect_from_aabbox(self.rect, minv.v, maxv.v)
        return self

    FLOOR = AE_FTOI_MODE_FLOOR
    CEIL  = AE_FTOI_MODE_CEIL
    ROUND = AE_FTOI_MODE_ROUND

    def to_int_ex(self, ae_ftoi_mode_t mode):
        cdef IntRect other = IntRect()

        flt_rect_to_int_ex(other.rect, self.rect, mode)
        return other

    def to_int(self):
        cdef IntRect other = IntRect()

        flt_rect_to_int(other.rect, self.rect)
        return other

    def from_int(self, IntRect other):
        int_rect_to_flt(self.rect, other.rect); return self

    def point_to_normalized(self, Vec2 rpoint):
        cdef Vec2 out = Vec2()

        flt_rect_point_to_normalized(out.v, self.rect, rpoint.v)
        return out

    def normalized_to_point(self, Vec2 normal):
        cdef Vec2 out = Vec2()

        flt_rect_normalized_to_point(out.v, self.rect, normal.v)
        return out

    def rotated_bounds_ex(self, Vec2 center, float sin_theta, float cos_theta):
        cdef FloatRect bounds = FloatRect()

        flt_rect_rotated_bounds_ex( bounds.rect, self.rect,
                                    NULL if center is None else center.v,
                                    sin_theta, cos_theta )
        return bounds

    def rotated_bounds(self, Vec2 center, float theta):
        cdef FloatRect bounds = FloatRect()

        flt_rect_rotated_bounds(bounds.rect, self.rect, # compute from angle
                                NULL if center is None else center.v, theta)
        return bounds

cdef class IntRect:
    cdef int rect[4] # s32 regions for blitting, unary operations, etc.

    def __init__(self, int x=0, int y=0, int w=0, int h=0):
        self.rect[0] = x
        self.rect[1] = y
        self.rect[2] = w
        self.rect[3] = h

    def __repr__(self):
        return "{}(x:{} y:{} w:{} h:{})".format(self.__class__.__name__,
                self.rect[0], self.rect[1], self.rect[2], self.rect[3])

    def __richcmp__(self, IntRect other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: IntRect')

    def __copy__(self):
        cdef IntRect other = IntRect()

        int_rect_copy(other.rect, self.rect)
        return other

    copy = __copy__

    def __nonzero__(self):
        return not int_rect_is_zero(self.rect)

    property x:
        def __get__(self): return self.rect[0]
        def __set__(self, int v): self.rect[0] = v

    property y:
        def __get__(self): return self.rect[1]
        def __set__(self, int v): self.rect[1] = v

    property w:
        def __get__(self): return self.rect[2]
        def __set__(self, int v): self.rect[2] = v

    property h:
        def __get__(self): return self.rect[3]
        def __set__(self, int v): self.rect[3] = v

    property pos:
        def __get__(self):
            return (self.rect[0], self.rect[1])

        def __set__(self, tuple pos):
            self.rect[0] = pos[0]
            self.rect[1] = pos[1]

    property dim:
        def __get__(self):
            return (self.rect[2], self.rect[3])

        def __set__(self, tuple dim):
            self.rect[2] = dim[0]
            self.rect[3] = dim[1]

    property center:
        def __get__(self):
            cdef int center[2] # tuple from array
            int_rect_get_center(center, self.rect)

            return (center[0], center[1])

        def __set__(self, tuple center):
            cdef int ctr[2] # array from tuple

            ctr[0] = center[0]
            ctr[1] = center[1]

            int_rect_set_center(self.rect, ctr)

    property area:
        def __get__(self): return int_rect_area(self.rect)

    def __len__(self): return 4

    def __getitem__(self, int key):
        if key < 0 or key > 3: raise IndexError
        return self.rect[key]

    def __setitem__(self, int key, int value):
        if key < 0 or key > 3: raise IndexError
        self.rect[key] = value

    def __reduce__(self):
        cdef bytes b = b'\0' * sizeof(int[4])
        cdef char* p = <char *>b

        int_rect_copy(<int*>p, self.rect)
        return (IntRect, (), b)

    def __setstate__(self, bytes state):
        cdef char* p = <char*>state
        int_rect_copy(self.rect, <int*>p)

    def address(self):
        return <size_t>self.rect

    def cmp(self, IntRect other):
        return int_rect_cmp(self.rect, other.rect)

    def zero(self):
        int_rect_zero(self.rect); return self

    def is_zero(self):
        return int_rect_is_zero(self.rect)

    def center_in(self, IntRect other):
        int_rect_center_in(self.rect, other.rect); return self

    def intersect_rect(self, IntRect other):
        return int_rect_vs_rect(self.rect, other.rect)

    def intersect_point(self, Vec2 point):
        return self.to_flt().intersect_point(point)

    def to_aabbox(self):
        cdef int minv[2]
        cdef int maxv[2]

        int_rect_to_aabbox(minv, maxv, self.rect)
        return ((minv[0], minv[1]), (maxv[0], maxv[1]))

    def from_aabbox(self, tuple minv, tuple maxv):
        cdef int mn[2]
        cdef int mx[2]

        mn[0] = minv[0]
        mn[1] = minv[1]

        mx[0] = maxv[0]
        mx[1] = maxv[1]

        int_rect_from_aabbox(self.rect, mn, mx)
        return self

    FLOOR = AE_FTOI_MODE_FLOOR
    CEIL  = AE_FTOI_MODE_CEIL
    ROUND = AE_FTOI_MODE_ROUND

    def to_flt(self):
        cdef FloatRect other = FloatRect()

        int_rect_to_flt(other.rect, self.rect)
        return other

    def from_flt_ex(self, FloatRect other, ae_ftoi_mode_t mode):
        flt_rect_to_int_ex(self.rect, other.rect, mode); return self

    def from_flt(self, FloatRect other):
        flt_rect_to_int(self.rect, other.rect); return self

    def point_to_normalized(self, Vec2 rpoint):
        return self.to_flt().point_to_normalized(rpoint)

    def normalized_to_point(self, Vec2 normal):
        return self.to_flt().normalized_to_point(normal)

# ==============================================================================
# ~ [ image class ]
# ==============================================================================
# TODO: if any of this stuff becomes multithreaded, wrap the header with "nogil"
# ------------------------------------------------------------------------------

cdef class Image:
    """Two-dimensional grid of pixels in client (non-GPU) memory."""
    cdef ae_image_t image

    FORMAT_RGBA = AE_IMAGE_FORMAT_RGBA
    FORMAT_RGB = AE_IMAGE_FORMAT_RGB
    FORMAT_MONO = AE_IMAGE_FORMAT_MONO

    TYPE_U8 = AE_IMAGE_TYPE_U8
    TYPE_FLT = AE_IMAGE_TYPE_FLT

    # TODO: cpython does this automatically to ext. types, does pypy?
    # def __cinit__(self): memset(&self.image, 0, sizeof(ae_image_t))

    def __dealloc__(self):
        ae_image_free(&self.image)

    def __repr__(self):
        return ("{}({}x{})".format(self.__class__.__name__,
                self.image.width, self.image.height))

    def __nonzero__(self):
        return self.image.width and self.image.height

    def __copy__(self):
        cdef Image other = self.__class__() # reallocate to fit
        ae_image_copy(&other.image, &self.image); return other

    copy = __copy__

    def __reduce__(self):
        raise NotImplementedError("TODO")

    # ===== [ attribute accessors ] ============================================

    property width:
        def __get__(self): return self.image.width
        def __set__(self, size_t v): self.image.width = v

    property height:
        def __get__(self): return self.image.height
        def __set__(self, size_t v): self.image.height = v

    property format:
        def __get__(self): return self.image.format
        def __set__(self, ae_image_format_t v): self.image.format = v

    property type:
        def __get__(self): return self.image.type
        def __set__(self, ae_image_type_t v): self.image.type = v

    # ===== [ memory management ] ==============================================

    def alloc(self):
        ae_image_alloc(&self.image); return self

    def alloc_fit(self, Image other):
        ae_image_alloc_fit(&self.image, &other.image); return self

    def address(self):
        return <size_t>self.image.pixels

    def format_name(self):
        """
        Get the string name of this image's pixel format (`MONO`, `RGBA`, etc).
        """
        if sys.version_info.major > 2:
            return ae_image_format_name[<size_t>self.image.format][3:].decode()
        else:
            return ae_image_format_name[<size_t>self.image.format][3:]

    def format_components(self):
        return ae_image_format_components[<size_t>self.image.format]

    def type_name(self):
        """
        Get the string name of this image's pixel type (floats, bytes, etc).
        """
        if sys.version_info.major > 2:
            return ae_image_type_name[<size_t>self.image.type][3:].decode()
        else:
            return ae_image_type_name[<size_t>self.image.type][3:]

    def type_bytes(self):
        return ae_image_type_bytes[<size_t>self.image.type]

    def size_in_bytes(self):
        return ae_image_bytes(&self.image)

    def pitch(self):
        return ae_image_pitch(&self.image)

    def get_array_subview(self, object array_t=Array):
        cdef Array subview = array_t()

        subview.array.data = self.image.pixels # remember to call del_subview!
        subview.array.size = subview.array._cap = ae_image_bytes(&self.image)

        return subview

    # ===== [ hashing ] ========================================================

    def hash(self):
        return ae_hash_mem(self.image.pixels, <int>ae_image_bytes(&self.image))

    def adler32(self, u32 seed=1):
        return ae_adler32_block(seed, self.image.pixels, ae_image_bytes(&self.image))

    def crc32(self, u32 seed=0):
        return ae_crc32_block(seed, self.image.pixels, ae_image_bytes(&self.image))

    # ===== [ memory codecs ] ==================================================

    def info_from_memory(self, size_t ptr, size_t length, bint fatal=True):
        cdef ae_image_error_t err = ae_image_info_from_memory(&self.image,
                                        < const void * const >ptr, length)

        if fatal and err != AE_IMAGE_SUCCESS:
            raise IOError(ae_image_error_message(err, NULL))

        return self

    def load_from_memory(self, size_t ptr, size_t length, bint fatal=True):
        cdef ae_image_error_t err = ae_image_load_from_memory(&self.image,
                                        < const void * const >ptr, length)

        if fatal and err != AE_IMAGE_SUCCESS:
            raise IOError(ae_image_error_message(err, NULL))

        return self

    def info_from_array(self, Array image, bint fatal=True):
        return self.info_from_memory(image.address(), len(image), fatal)

    def load_from_array(self, Array image, bint fatal=True):
        return self.load_from_memory(image.address(), len(image), fatal)

    def info_from_bytes(self, bytes image, bint fatal=True):
        return self.info_from_memory(<size_t>(<char*>image), len(image), fatal)

    def load_from_bytes(self, bytes image, bint fatal=True):
        return self.load_from_memory(<size_t>(<char*>image), len(image), fatal)

    # ===== [ file codecs ] ====================================================

    def info(self, str filename, bint fatal=True):
        """
        Fill image with width, height, format, type info without loading pixels.
        """
        cdef ae_image_error_t err

        cdef bytes b_filename
        cdef bytes err_string

        if sys.version_info.major > 2:
            b_filename = <bytes>filename.encode('utf-8') # convert from unicode
        else:
            b_filename = <bytes>filename

        err = ae_image_info(&self.image, <char*>b_filename) # issue actual call

        if fatal and err != AE_IMAGE_SUCCESS:
            err_string = ae_image_error_message(err, <char*>b_filename)

            if sys.version_info.major > 2:
                raise IOError(err_string.decode()) # convert ascii to utf-8 str
            else:
                raise IOError(err_string)

        return self

    def load(self, str filename, bint fatal=True):
        """
        Load a single image file (PNG, JPEG, BMP, DDS, etc) from the filesystem.
        """
        cdef ae_image_error_t err

        cdef bytes b_filename
        cdef bytes err_string

        if sys.version_info.major > 2:
            b_filename = <bytes>filename.encode('utf-8') # convert from unicode
        else:
            b_filename = <bytes>filename

        err = ae_image_load(&self.image, <char*>b_filename) # issue actual call

        if fatal and err != AE_IMAGE_SUCCESS:
            err_string = ae_image_error_message(err, <char*>b_filename)

            if sys.version_info.major > 2:
                raise IOError(err_string.decode()) # convert ascii to utf-8 str
            else:
                raise IOError(err_string)

        return self

    def save(self, str filename, bint fatal=True):
        """
        Save an image. If the image file already exists, it will be overwritten.
        """
        cdef ae_image_error_t err

        cdef bytes b_filename
        cdef bytes err_string

        if sys.version_info.major > 2:
            b_filename = <bytes>filename.encode('utf-8') # convert from unicode
        else:
            b_filename = <bytes>filename

        err = ae_image_save(&self.image, <char*>b_filename) # issue actual call

        if fatal and err != AE_IMAGE_SUCCESS:
            err_string = ae_image_error_message(err, <char*>b_filename)

            if sys.version_info.major > 2:
                raise IOError(err_string.decode()) # convert ascii to utf-8 str
            else:
                raise IOError(err_string)

        return self

    # ===== [ image archives ] =================================================
    # --------------------------------------------------------------------------
    # XXX TODO FIXME: archive encoders aren't py3k-compatible yet (tricky work!)
    # --------------------------------------------------------------------------

    @classmethod
    def load_archive(cls, str filename, bint fatal=True):
        """
        Load a sequence of images from any implementation-supported archive file.
        """
        cdef ae_image_t* c_images
        cdef ae_image_t* c_image

        cdef list images = []
        cdef Image image

        cdef ae_image_error_t err

        cdef bytes b_filename
        cdef bytes err_string

        if sys.version_info.major > 2:
            b_filename = <bytes>filename.encode('utf-8') # convert utf-8 to ascii
        else:
            b_filename = <bytes>filename # filename is already oldschool byte str

        err = ae_image_archive_load(&c_images, <char*>b_filename)

        if err != AE_IMAGE_SUCCESS:
            if fatal:
                err_string = ae_image_error_message(err, <char*>b_filename)

                if sys.version_info.major > 2:
                    raise IOError(err_string.decode())
                else:
                    raise IOError(err_string)
            else:
                return images

        # scan for zero-size image sentinel, appending images along the way.
        c_image = c_images

        while (ae_image_bytes(c_image)):
            image = cls()

            image.image = c_image[0]
            images.append(image)

            c_image += 1

        # now that we've built our list, free the temporary c image array.
        ae_free(c_images); return images

    @staticmethod
    def save_archive_ex(list filenames, bytes filename, bint fatal=True):
        """
        Create an image archive from a list of image filenames and save it. All
        image files must be of the same file type due to implementation limits.
        """
        cdef ae_image_error_t err
        cdef bytes name

        # allocate an extra pointer for the null terminator
        cdef size_t i, string_buffer_bytes = <size_t> \
        (
            (<int>len(filenames) + 1) * <int>sizeof(char*)
        )

        cdef char** strings = <char**> ae_stack_malloc(
                ae_global_stack(), string_buffer_bytes)

        strings[len(filenames)] = NULL

        # TODO: we need to utf-8 convert and allocate these in python 3k
        for i, name in enumerate(filenames): strings[i] = <char*>name

        err = ae_image_archive_save_ex( # cast for clang
                <const char**>strings, <char*>filename)

        ae_stack_free(ae_global_stack(), strings, string_buffer_bytes)

        if err != AE_IMAGE_SUCCESS and fatal:
            raise IOError(ae_image_error_message(err, <char*>filename))

    @staticmethod
    def save_archive(list images, bytes filename, bint fatal=True):
        """
        Create an image archive from a list of image objects and save it. All
        images will be archived with the same underlying file codec (PNG etc).
        """
        cdef ae_image_error_t err
        cdef Image image

        # allocate an extra zero-sized image for the image array sentinel
        cdef size_t i, image_buffer_bytes = <size_t> \
        (
            (<int>len(images) + 1) * <int>sizeof(ae_image_t)
        )

        cdef ae_image_t* c_images = <ae_image_t*> ae_stack_malloc(
                            ae_global_stack(), image_buffer_bytes)

        memset(&c_images[len(images)], 0, sizeof(ae_image_t)) # sentinel
        for i, image in enumerate(images): c_images[i] = image.image

        err = ae_image_archive_save(c_images, <char*>filename) # write
        ae_stack_free(ae_global_stack(), c_images, image_buffer_bytes)

        if err != AE_IMAGE_SUCCESS and fatal:
            raise IOError(ae_image_error_message(err, <char*>filename))

    # ===== [ data conversion ] ================================================

    def type_convert(self, ae_image_type_t desired, object image_t=Image):
        cdef Image image = image_t()

        ae_image_type_convert(&image.image, &self.image, desired)
        return image # convert the image between chars and floats

    def format_convert(self, ae_image_format_t desired, object image_t=Image):
        cdef Image image = image_t()

        ae_image_format_convert(&image.image, &self.image, desired)
        return image # convert image between RGB, RGBA, mono, etc.

    # ===== [ resizing ] =======================================================

    RESIZE_FILTER_DEFAULT = AE_IMAGE_RESIZE_FILTER_DEFAULT
    RESIZE_FILTER_BOX = AE_IMAGE_RESIZE_FILTER_BOX
    RESIZE_FILTER_TRIANGLE = AE_IMAGE_RESIZE_FILTER_TRIANGLE
    RESIZE_FILTER_CUBIC_B_SPLINE = AE_IMAGE_RESIZE_FILTER_CUBIC_B_SPLINE
    RESIZE_FILTER_CATMULL_ROM = AE_IMAGE_RESIZE_FILTER_CATMULL_ROM
    RESIZE_FILTER_MITCHELL = AE_IMAGE_RESIZE_FILTER_MITCHELL

    def resize_ex(self, size_t w, size_t h, ae_image_resize_filter_t f, object image_t=Image):
        cdef Image image = image_t()

        ae_image_resize_ex(&image.image, &self.image, w, h, f)
        return image

    def resize(self, size_t w, size_t h, object image_t=Image):
        cdef Image image = image_t()

        ae_image_resize(&image.image, &self.image, w, h)
        return image

    # ===== [ unary operations ] ===============================================

    def rect(self):
        # origin is expected to be bottom-left
        cdef IntRect rect = IntRect()

        rect.rect[2] = <int>self.image.width
        rect.rect[3] = <int>self.image.height

        return rect

    def unary_clut(self, Array r=None, Array g=None, Array b=None,
                                Array a=None, IntRect rect=None ):
        cdef u8 * r_lut = NULL
        cdef u8 * g_lut = NULL
        cdef u8 * b_lut = NULL
        cdef u8 * a_lut = NULL
        cdef int* prect = NULL

        if rect is not None: prect = rect.rect # if null, clut entire image

        if r is not None:
            assert r.array.size >= 256, "{}".format(r)
            r_lut = <u8 *>r.array.data

        if g is not None:
            assert g.array.size >= 256, "{}".format(g)
            g_lut = <u8 *>g.array.data

        if b is not None:
            assert b.array.size >= 256, "{}".format(b)
            b_lut = <u8 *>b.array.data

        if a is not None:
            assert a.array.size >= 256, "{}".format(a)
            a_lut = <u8 *>a.array.data

        ae_image_unary_clut(&self.image, prect, r_lut, g_lut, b_lut, a_lut)
        return self

    def negative(self, bint r=True, bint g=True, bint b=True, IntRect rect=None):
        """
        Photo negative effect. Disable on one or more color channels for cool neon effects.
        """
        ae_image_negative(&self.image, NULL if rect is None else rect.rect, r, g, b)
        return self

    def solarize(self, u8 threshold, bint r=True, bint g=True, bint b=True, IntRect rect=None):
        """
        All pixels above a given monochrome brightness threshold are inverted (photo negative).
        """
        ae_image_solarize(&self.image, NULL if rect is None else rect.rect, threshold, r, g, b)
        return self

    def greyscale(self, bint r=True, bint g=True, bint b=True, IntRect rect=None):
        """
        Make the image monochrome based on a biased color average (for better visual quality).
        """
        ae_image_greyscale(&self.image, NULL if rect is None else rect.rect, r, g, b)
        return self

    def threshold(self, u8 threshold, bint r=True, bint g=True, bint b=True, IntRect rect=None):
        """
        All pixels above a given monochrome brightness threshold are white, otherwise black.
        """
        ae_image_threshold(&self.image, NULL if rect is None else rect.rect, threshold, r, g, b)
        return self

    def bleach(self, u8 threshold, bint r=True, bint g=True, bint b=True, IntRect rect=None):
        """
        All pixels above a given monochrome brightness threshold are greyscaled (with bias).
        """
        ae_image_bleach(&self.image, NULL if rect is None else rect.rect, threshold, r, g, b)
        return self

    def neonize(self, u8 threshold, bint lr=True, bint lg=True, bint lb=True,
                bint hr=True, bint hg=True, bint hb=True, IntRect rect=None):
        """
        Like solarize, but a different negative op is performed below the threshold.
        """
        ae_image_neonize(&self.image, NULL if rect is None else rect.rect,
                                        threshold, lr, lg, lb, hr, hg, hb)
        return self

    def pastelize(self, u8 threshold, bint lr=True, bint lg=True, bint lb=True,
                bint hr=True, bint hg=True, bint hb=True, IntRect rect=None):
        """
        Like bleach, but a different greyscale op is performed below the threshold.
        """
        ae_image_pastelize(&self.image, NULL if rect is None else rect.rect,
                                        threshold, lr, lg, lb, hr, hg, hb)
        return self

    def isolate_channel(self, int channel, bint r=True, bint g=True, bint b=True, IntRect rect=None):
        ae_image_isolate_channel(&self.image, NULL if rect is None else rect.rect, channel, r, g, b)
        return self

    def set_color(self, Vec4 color, bint r=True, bint g=True,
                bint b=True, bint a=True, IntRect rect=None):
        ae_image_set_color(&self.image, NULL if rect is None else rect.rect, color.v, r, g, b, a)
        return self

    def scale_color(self, Vec4 color, IntRect rect=None):
        ae_image_scale_color(&self.image, NULL if rect is None else rect.rect, color.v)
        return self

    def flip_x(self, bint r=True, bint g=True, bint b=True, bint a=True, IntRect rect=None):
        ae_image_flip_x(&self.image, NULL if rect is None else rect.rect, r, g, b, a)
        return self

    def flip_y(self, bint r=True, bint g=True, bint b=True, bint a=True, IntRect rect=None):
        ae_image_flip_y(&self.image, NULL if rect is None else rect.rect, r, g, b, a)
        return self

    # ===== [ binary operations ] ==============================================

    def copy_into(self, Image dst, int x, int y, bint r=True, bint g=True, bint b=True, bint a=True):
        ae_image_binary_copy(&dst.image, &self.image, x, y, r, g, b, a); return self

    def lerp_into(self, Image dst, int x, int y, float t, bint r = True,
                            bint g = True, bint b = True, bint a = True):
        ae_image_lerp(&dst.image, &self.image, x, y, t, r, g, b, a); return self

    # ===== [ blitting ] =======================================================

    BLEND_FUNCTION_ZERO = AE_IMAGE_BLEND_FUNCTION_ZERO
    BLEND_FUNCTION_ONE = AE_IMAGE_BLEND_FUNCTION_ONE
    BLEND_FUNCTION_SRC_COLOR = AE_IMAGE_BLEND_FUNCTION_SRC_COLOR
    BLEND_FUNCTION_ONE_MINUS_SRC_COLOR = AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_COLOR
    BLEND_FUNCTION_DST_COLOR = AE_IMAGE_BLEND_FUNCTION_DST_COLOR
    BLEND_FUNCTION_ONE_MINUS_DST_COLOR = AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_DST_COLOR
    BLEND_FUNCTION_SRC_ALPHA = AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA
    BLEND_FUNCTION_ONE_MINUS_SRC_ALPHA = AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_SRC_ALPHA
    BLEND_FUNCTION_DST_ALPHA = AE_IMAGE_BLEND_FUNCTION_DST_ALPHA
    BLEND_FUNCTION_ONE_MINUS_DST_ALPHA = AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_DST_ALPHA
    BLEND_FUNCTION_CONSTANT_COLOR = AE_IMAGE_BLEND_FUNCTION_CONSTANT_COLOR
    BLEND_FUNCTION_ONE_MINUS_CONSTANT_COLOR = AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_CONSTANT_COLOR
    BLEND_FUNCTION_CONSTANT_ALPHA = AE_IMAGE_BLEND_FUNCTION_CONSTANT_ALPHA
    BLEND_FUNCTION_ONE_MINUS_CONSTANT_ALPHA = AE_IMAGE_BLEND_FUNCTION_ONE_MINUS_CONSTANT_ALPHA
    BLEND_FUNCTION_SRC_ALPHA_SATURATE = AE_IMAGE_BLEND_FUNCTION_SRC_ALPHA_SATURATE

    BLEND_EQUATION_ADD = AE_IMAGE_BLEND_EQUATION_ADD
    BLEND_EQUATION_SUBTRACT = AE_IMAGE_BLEND_EQUATION_SUBTRACT
    BLEND_EQUATION_REVERSE_SUBTRACT = AE_IMAGE_BLEND_EQUATION_REVERSE_SUBTRACT
    BLEND_EQUATION_MIN = AE_IMAGE_BLEND_EQUATION_MIN
    BLEND_EQUATION_MAX = AE_IMAGE_BLEND_EQUATION_MAX

    def blit_into_ex(self, Image dst, int x, int y, # generic
                    ae_image_blend_equation_t rgb_equation,
                    ae_image_blend_equation_t alpha_equation,
                    ae_image_blend_function_t src_rgb,
                    ae_image_blend_function_t src_alpha,
                    ae_image_blend_function_t dst_rgb,
                    ae_image_blend_function_t dst_alpha,
                    Vec4 constant=None,
                    Vec4 src_scale=None, Vec4 dst_scale=None,
                    bint r=1, bint g=1, bint b=1, bint a=1 ):
        """
            Draw this image into another image with a blending mode.
        """
        cdef float* s = NULL if src_scale is None else src_scale.v
        cdef float* d = NULL if dst_scale is None else dst_scale.v
        cdef float* c = NULL if constant  is None else constant.v

        ae_image_blit_ex(&dst.image, &self.image, x, y, rgb_equation, alpha_equation,
                        src_rgb, src_alpha, dst_rgb, dst_alpha, c, s, d, r, g, b, a)
        return self

    def blit_into( self, Image dst, int x, int y, bint r = True ,
                    bint g = True, bint b = True, bint a = True):
        """
            Draw this image into another image with normal blending.
        """
        ae_image_blit(&dst.image, &self.image, x, y, r, g, b, a)
        return self

    # ===== [ unary rects ] ====================================================

    def blit_rect_ex( self, ae_image_blend_equation_t rgb_equation,
                            ae_image_blend_equation_t alpha_equation,
                            ae_image_blend_function_t src_rgb,
                            ae_image_blend_function_t src_alpha,
                            ae_image_blend_function_t dst_rgb,
                            ae_image_blend_function_t dst_alpha,
                            Vec4 constant=None,
                            Vec4 src_scale=None, Vec4 dst_scale=None,
                            bint r=1, bint g=1, bint b=1, bint a=1,
                            IntRect rect=None ):
        """
            Draw an alpha-blended rect over self with a given alpha blending mode.
        """
        cdef float* s = NULL if src_scale is None else src_scale.v
        cdef float* d = NULL if dst_scale is None else dst_scale.v
        cdef float* c = NULL if constant  is None else constant.v

        ae_image_blit_rect_ex(&self.image, NULL if rect is None else rect.rect, rgb_equation,
                alpha_equation, src_rgb, src_alpha, dst_rgb, dst_alpha, c, s, d, r, g, b, a)

    def blit_rect(self, Vec4 color = None, bint r = True, bint g = True,
                    bint b = True, bint a = True, IntRect rect = None ):
        """
            Draw an alpha-blended rect over self with normal transparent blending.
        """
        ae_image_blit_rect(&self.image, NULL if rect is None else rect.rect,
                            NULL if color is None else color.v, r, g, b, a)
        return self

    # ===== [ unary lines ] ====================================================

    def blit_line_horizontal_ex(self, int x, int y, int width, Vec4 color = None):
        ae_image_blit_line_horizontal_ex(&self.image, x, y, width, NULL if color is None else color.v)
        return self

    def blit_line_vertical_ex(self, int x, int y, int height, Vec4 color = None):
        ae_image_blit_line_vertical_ex(&self.image, x, y, height, NULL if color is None else color.v)
        return self

    def blit_line_horizontal(self, int x1, int y, int x2, Vec4 color = None):
        ae_image_blit_line_horizontal(&self.image, x1, y, x2, NULL if color is None else color.v)
        return self

    def blit_line_vertical(self, int x, int y1, int y2, Vec4 color = None):
        ae_image_blit_line_vertical(&self.image, x, y1, y2, NULL if color is None else color.v)
        return self

    def blit_rect_outline(self, IntRect rect=None, Vec4 color=None):
        ae_image_blit_rect_outline(&self.image, NULL if rect is None else rect.rect,
                                                NULL if color is None else color.v)
        return self

    def blit_grid_outline(self, IntRect origin, size_t w, size_t h, Vec4 color=None):
        ae_image_blit_grid_outline( &self.image, origin.rect, w, h,
                                NULL if color is None else color.v)
        return self

    # ===== [ regions ] ========================================================

    def get_region(self, IntRect rect=None):
        cdef Image region = self.__class__()

        ae_image_get_region(&region.image, &self.image, NULL if rect is None else rect.rect)
        return region
