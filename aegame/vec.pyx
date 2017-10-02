# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------

cdef extern from "ae_math.h":
    # ==========================================================================
    # ~ [ float math ]
    # ==========================================================================

    float ae_rad2degf(float angle)
    float ae_deg2radf(float angle)
    float ae_degreesf(float angle)
    float ae_radiansf(float angle)

    float ae_atan2f(float y, float x)

    float ae_cosf(float angle)
    float ae_sinf(float angle)
    float ae_tanf(float angle)

    float ae_acosf(float v)
    float ae_asinf(float v)
    float ae_atanf(float v)

    float ae_sinf_01(const float theta)
    float ae_cosf_01(const float theta)

    float ae_sinf_period(const float theta, const float period)
    float ae_cosf_period(const float theta, const float period)

    float ae_sinf_period_01(const float theta, const float period)
    float ae_cosf_period_01(const float theta, const float period)

    float ae_fovx_radiansf(const float fov_y, const float aspect)
    float ae_fovx_degreesf(const float fov_y, const float aspect)

    float ae_lerpf( float a, float b, float t )
    float ae_unlerpf(float a, float b, float t)

    float ae_sqrtf(float v)
    float ae_powf(float base, float exponent)
    float ae_fabsf(float v)
    float ae_logf(float arg)

    float ae_fminf(float x, float y)
    float ae_fmaxf(float x, float y)

    float ae_fmin3f(float a, float b, float c)
    float ae_fmax3f(float a, float b, float c)

    float ae_fmin4f(float a, float b, float c, float d)
    float ae_fmax4f(float a, float b, float c, float d)

    float ae_fclampf(float x, float mn, float mx)

    float ae_floorf(float v)
    float ae_ceilf(float v)
    float ae_roundf(float v)

    ctypedef enum ae_ftoi_mode_t:
        AE_FTOI_MODE_FLOOR
        AE_FTOI_MODE_CEIL
        AE_FTOI_MODE_ROUND
        AE_FTOI_MODE_COUNT

    int ae_ftoi_ex(float v, ae_ftoi_mode_t mode)
    int ae_ftoi(float v)

    float ae_normalize_deg_360f(float degrees)
    float ae_normalize_deg_180f(float degrees)

    float ae_normalize_rad_tauf(float radians)
    float ae_normalize_rad_pif (float radians)

    # ==========================================================================
    # ~ [ n-dimensional vector math ]
    # ==========================================================================

    float* ae_vector_alloc(const size_t n) # allocates
    void ae_vector_free(float* p, const size_t n)

    void vec_add_vec( float* out, const float* const a,
                const float* const b, const size_t n )

    void vec_add_flt( float* out, const float* const v,
                const float f_factor, const size_t n )

    void vec_sub_vec( float* out, const float* const a,
                const float* const b, const size_t n )

    void vec_sub_flt( float* out, const float* const v,
                const float f_factor, const size_t n )

    void vec_mul_vec( float* out, const float* const a,
                const float* const b, const size_t n )

    void vec_mul_flt( float* out, const float* const v,
                const float f_factor, const size_t n )

    void vec_div_vec( float* out, const float* const a,
                const float* const b, const size_t n )

    void vec_div_flt( float* out, const float* const v,
                const float f_factor, const size_t n )

    void vec_copy(float* dst, const float* const src, const size_t n)
    void vec_zero(float* v, const size_t n)

    int vec_cmp(const float* const a, const float* const b, size_t n) # vec comparison
    int vec_cmp_e(const float* const a, const float* const b, float epsilon, size_t n)

    int vec_is_zero(const float* const v, const size_t n) # vector elems are zero
    int vec_is_zero_e(const float* const v, const float epsilon, const size_t n)

    float vec_sum(const float* const v, const size_t n)
    float vec_dot(const float* const a, const float* const b, const size_t n)

    float vec_mag2(const float* const v, const size_t n)
    float vec_mag(const float* const v, const size_t n)

    float vec_dist2(const float* const a, const float* const b, const size_t n)
    float vec_dist(const float* const a, const float* const b, const size_t n)

    float vec_angle(const float* const a, const float* const b, const size_t n)
    void vec_neg(float* out, const float* const v, size_t n)

    void vec_reciprocal(float* out, const float* const v, size_t n)
    float vec_normalize(float* out, const float* const v, const size_t n)

    void vec_min_vec(float* out, const float* const a,
                const float* const b, const size_t n)

    void vec_max_vec(float* out, const float* const a,
                const float* const b, const size_t n)

    void vec_min_flt(float* out, const float* const vec,
                        const float fv, const size_t n)

    void vec_max_flt(float* out, const float* const vec,
                        const float fv, const size_t n)

    void vec_clamp_vec(float* out, const float* const v, # clamp to range
            const float* const mn, const float* const mx, const size_t n)

    void vec_clamp_flt( float * out, const float * const v,
            const float mn, const float mx, const size_t n)

    int vec_is_unit(const float* const v, const size_t n) # is this vec normal?
    int vec_is_unit_e(const float* const v, const float epsilon, const size_t n)

    int vec_fix_denormals_ex(float* v, const float denormal, const size_t n)
    int vec_fix_denormals(float* v, const size_t n) # floor very tiny values

    void vec_reflect(float* out, const float* const v, # reflection
                        const float* const normal, const size_t n)

    void vec_madd(float* out, const float* const a, # d = a + s * b
                const float* const b, const float s, const size_t n)

    void vec_lerp(float* out, const float* const a, # interpolation
                const float* const b, const float t, const size_t n)

    void vec_floor(float* out, const float* const v, const size_t n)
    void vec_ceil(float* out, const float* const v, const size_t n)
    void vec_round(float* out, const float* const v, const size_t n)

    # ==========================================================================
    # ~ [ fixed-size vector math ]
    # ==========================================================================

    void vec2add_flt(float out[2], const float v[2], const float f)
    void vec2add_vec(float out[2], const float a[2], const float b[2])
    void vec3add_flt(float out[3], const float v[3], const float f)
    void vec3add_vec(float out[3], const float a[3], const float b[3])
    void vec4add_flt(float out[4], const float v[4], const float f)
    void vec4add_vec(float out[4], const float a[4], const float b[4])

    void vec2sub_flt(float out[2], const float v[2], const float f)
    void vec2sub_vec(float out[2], const float a[2], const float b[2])
    void vec3sub_flt(float out[3], const float v[3], const float f)
    void vec3sub_vec(float out[3], const float a[3], const float b[3])
    void vec4sub_flt(float out[4], const float v[4], const float f)
    void vec4sub_vec(float out[4], const float a[4], const float b[4])

    void vec2mul_flt(float out[2], const float v[2], const float f)
    void vec2mul_vec(float out[2], const float a[2], const float b[2])
    void vec3mul_flt(float out[3], const float v[3], const float f)
    void vec3mul_vec(float out[3], const float a[3], const float b[3])
    void vec4mul_flt(float out[4], const float v[4], const float f)
    void vec4mul_vec(float out[4], const float a[4], const float b[4])

    void vec2div_flt(float out[2], const float v[2], const float f)
    void vec2div_vec(float out[2], const float a[2], const float b[2])
    void vec3div_flt(float out[3], const float v[3], const float f)
    void vec3div_vec(float out[3], const float a[3], const float b[3])
    void vec4div_flt(float out[4], const float v[4], const float f)
    void vec4div_vec(float out[4], const float a[4], const float b[4])

    void vec2translate(float out[2], const float a[2], const float b[2])
    void vec3translate(float out[3], const float a[3], const float b[3])
    void vec4translate(float out[4], const float a[4], const float b[4])

    void vec2scale(float out[2], const float v[2], const float f)
    void vec3scale(float out[3], const float v[3], const float f)
    void vec4scale(float out[4], const float v[4], const float f)

    void vec2copy(float dst[2], const float src[2])
    void vec3copy(float dst[3], const float src[3])
    void vec4copy(float dst[4], const float src[4])

    void vec2zero(float dst[2])
    void vec3zero(float dst[3])
    void vec4zero(float dst[4])

    int vec2cmp(const float a[2], const float b[2])
    int vec3cmp(const float a[3], const float b[3])
    int vec4cmp(const float a[4], const float b[4])

    int vec2cmp_e(const float a[2], const float b[2], const float epsilon)
    int vec3cmp_e(const float a[3], const float b[3], const float epsilon)
    int vec4cmp_e(const float a[4], const float b[4], const float epsilon)

    int vec2is_zero(const float v[2])
    int vec3is_zero(const float v[3])
    int vec4is_zero(const float v[4])

    int vec2is_zero_e(const float v[2], const float epsilon)
    int vec3is_zero_e(const float v[3], const float epsilon)
    int vec4is_zero_e(const float v[4], const float epsilon)

    int vec2is_unit(const float v[2])
    int vec3is_unit(const float v[3])
    int vec4is_unit(const float v[4])

    int vec2is_unit_e(const float v[2], const float epsilon)
    int vec3is_unit_e(const float v[3], const float epsilon)
    int vec4is_unit_e(const float v[4], const float epsilon)

    int vec2fix_denormals_ex(float v[2], const float denormal)
    int vec3fix_denormals_ex(float v[3], const float denormal)
    int vec4fix_denormals_ex(float v[4], const float denormal)

    int vec2fix_denormals(float v[2])
    int vec3fix_denormals(float v[3])
    int vec4fix_denormals(float v[4])

    void vec2reflect(float out[2], const float v[2], const float normal[2])
    void vec3reflect(float out[3], const float v[3], const float normal[3])
    void vec4reflect(float out[4], const float v[4], const float normal[4])

    float vec2sum(const float v[2])
    float vec3sum(const float v[3])
    float vec4sum(const float v[4])

    float vec2dot(const float a[2], const float b[2])
    float vec3dot(const float a[3], const float b[3])
    float vec4dot(const float a[4], const float b[4])

    float vec2mag2(const float v[2])
    float vec3mag2(const float v[3])
    float vec4mag2(const float v[4])

    float vec2mag(const float v[2])
    float vec3mag(const float v[3])
    float vec4mag(const float v[4])

    float vec2dist2(const float a[2], const float b[2])
    float vec3dist2(const float a[3], const float b[3])
    float vec4dist2(const float a[4], const float b[4])

    float vec2dist(const float a[2], const float b[2])
    float vec3dist(const float a[3], const float b[3])
    float vec4dist(const float a[4], const float b[4])

    float vec2angle(const float a[2], const float b[2])
    float vec3angle(const float a[3], const float b[3])
    float vec4angle(const float a[4], const float b[4])

    void vec2neg(float dst[2], const float src[2])
    void vec3neg(float dst[3], const float src[3])
    void vec4neg(float dst[4], const float src[4])

    void vec2reciprocal(float out[2], const float v[2])
    void vec3reciprocal(float out[3], const float v[3])
    void vec4reciprocal(float out[4], const float v[4])

    float vec2normalize(float out[2], const float v[2])
    float vec3normalize(float out[3], const float v[3])
    float vec4normalize(float out[4], const float v[4])

    void vec2min_vec(float out[2], const float a[2], const float b[2])
    void vec3min_vec(float out[3], const float a[3], const float b[3])
    void vec4min_vec(float out[4], const float a[4], const float b[4])

    void vec2max_vec(float out[2], const float a[2], const float b[2])
    void vec3max_vec(float out[3], const float a[3], const float b[3])
    void vec4max_vec(float out[4], const float a[4], const float b[4])

    void vec2min_flt(float out[2], const float v[2], const float f)
    void vec3min_flt(float out[3], const float v[3], const float f)
    void vec4min_flt(float out[4], const float v[4], const float f)

    void vec2max_flt(float out[2], const float v[2], const float f)
    void vec3max_flt(float out[3], const float v[3], const float f)
    void vec4max_flt(float out[4], const float v[4], const float f)

    void vec2clamp_vec(float out[2], const float v[2], const float mn[2], const float mx[2])
    void vec3clamp_vec(float out[3], const float v[3], const float mn[3], const float mx[3])
    void vec4clamp_vec(float out[4], const float v[4], const float mn[4], const float mx[4])

    void vec2clamp_flt(float out[2], const float v[2], const float mn, const float mx)
    void vec3clamp_flt(float out[3], const float v[3], const float mn, const float mx)
    void vec4clamp_flt(float out[4], const float v[4], const float mn, const float mx)

    void vec2madd(float out[2], const float a[2], const float b[2], const float s)
    void vec3madd(float out[3], const float a[3], const float b[3], const float s)
    void vec4madd(float out[4], const float a[4], const float b[4], const float s)

    void vec2lerp(float out[2], const float a[2], const float b[2], const float t)
    void vec3lerp(float out[3], const float a[3], const float b[3], const float t)
    void vec4lerp(float out[4], const float a[4], const float b[4], const float t)

    void vec2floor(float out[2], const float v[2])
    void vec3floor(float out[3], const float v[3])
    void vec4floor(float out[4], const float v[4])

    void vec2ceil(float out[2], const float v[2])
    void vec3ceil(float out[3], const float v[3])
    void vec4ceil(float out[4], const float v[4])

    void vec2round(float out[2], const float v[2])
    void vec3round(float out[3], const float v[3])
    void vec4round(float out[4], const float v[4])

    void rotate_point2_ex(float out[2], const float point[2], const float center[2],
                                    const float sin_theta, const float cos_theta)

    void rotate_point2(float out[2], const float point[2],
                const float center[2], const float theta)

    void vec2rotate_ex(float out[2], const float v[2],
        const float sin_theta, const float cos_theta)

    void vec2rotate(float out[2], const float v[2], const float theta)

    void vec3cross(float out[3], const float a[3], const float b[3])
    int vec3fix_degenerate_normal(float v[3])

    void vec3rotate_ex( float out[3], const float s[3], const float axis[3],
                        const float sin_theta, const float cos_theta )

    void vec3rotate(float out[3], const float s[3],
            const float axis[3], const float theta)

    void vec3up_from_forward(float outv[3], const float abs_up[3],
                                    const float object_forward[3])

    void euler_radians_to_gl_normal(float gl_vec[3],
            const float heading, const float pitch)

    void euler_degrees_to_gl_normal(float gl_vec[3],
            const float heading, const float pitch)

    float gl_normal_to_heading_radians(const float v[3])
    float gl_normal_to_heading_degrees(const float v[3])

    float gl_normal_to_pitch_radians(const float v[3])
    float gl_normal_to_pitch_degrees(const float v[3])

# ==============================================================================
# ~ [ n-dimensional vector class ]
# ==============================================================================

cdef class Vector:
    cdef float* v
    cdef size_t n

    def __init__(self, size_t vector_length, *args):
        cdef float item
        cdef int i

        self.v = ae_vector_alloc(vector_length)
        self.n = vector_length

        for i, item in enumerate(args): self[i] = item

    def __dealloc__(self):
        ae_vector_free(self.v, self.n)

    def __repr__(self):
        return "{}{}{}".format(self.__class__.__name__, self.n, tuple(self))

    def __richcmp__(self, Vector other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: Vector')

    def __copy__(self):
        cdef Vector v = Vector(self.n)
        vec_copy( v.v, self.v, self.n)

        return v

    copy = __copy__

    def __neg__(self):
        cdef Vector v = Vector(self.n)
        vec_neg( v.v, self.v, self.n )

        return v

    def __nonzero__(self):
        return not vec_is_zero(self.v, self.n)

    def __len__(self):
        return self.n

    def __getitem__(self, int key):
        if key < 0 or key >= <int>self.n: raise IndexError
        return self.v[key]

    def __setitem__(self, int key, float value):
        if key < 0 or key >= <int>self.n: raise IndexError
        self.v[key] = value

    def __reduce__(self):
        cdef bytes b = b'\0\0\0\0' * self.n
        cdef char* p = <char*>b

        vec_copy(<float*>p, self.v, self.n)
        return (Vector, (self.n, ), b)

    def __setstate__(self, bytes state):
        cdef char* p = <char*>state
        vec_copy(self.v, <float*>p, len(state) // sizeof(float))

    def address(self):
        return <size_t>self.v

    def sum(self):
        return vec_sum(self.v, self.n)

    def dot(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)
        return vec_dot(self.v, other.v, self.n) # vec inner product

    def __abs__(self):
        return vec_mag(self.v, self.n)

    mag = magnitude = __abs__

    def magnitude_squared(self):
        return vec_mag2(self.v, self.n)

    mag2 = magnitude_squared

    def dist(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)
        return vec_dist(self.v, other.v, self.n)

    def dist2(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)
        return vec_dist2(self.v, other.v, self.n)

    distance = dist
    distance_squared = dist2

    def angle(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)
        return vec_angle(self.v, other.v, self.n)

    def neg(self):
        vec_neg(self.v, self.v, self.n); return self

    def reciprocate(self):
        vec_reciprocal(self.v, self.v, self.n); return self

    def reciprocal(self):
        cdef Vector v = Vector(self.n)
        vec_reciprocal(v.v, self.v, self.n); return v

    def normalize(self):
        vec_normalize(self.v, self.v, self.n); return self

    def normalized(self):
        cdef Vector v = Vector(self.n)
        vec_normalize(v.v, self.v, self.n); return v

    def clamped_vec(self, Vector minv, Vector maxv):
        assert len(self) == len(minv), "{} {}".format(self, minv)
        assert len(self) == len(maxv), "{} {}".format(self, maxv)

        cdef Vector v = Vector(self.n)
        vec_clamp_vec(v.v, self.v, minv.v, maxv.v, self.n); return v

    def clamp_vec(self, Vector minv, Vector maxv):
        assert len(self) == len(minv), "{} {}".format(self, minv)
        assert len(self) == len(maxv), "{} {}".format(self, maxv)

        vec_clamp_vec(self.v, self.v, minv.v, maxv.v, self.n); return self

    def clamped_flt(self, float minv, float maxv):
        cdef Vector v = Vector(self.n)
        vec_clamp_flt(v.v, self.v, minv, maxv, self.n); return v

    def clamp_flt(self, float minv, float maxv):
        vec_clamp_flt(self.v, self.v, minv, maxv, self.n); return self

    def madd(self, Vector other, float s):
        assert len(self) == len(other), "{} {}".format(self, other)

        cdef Vector v = Vector(self.n)
        vec_madd(v.v, self.v, other.v, s, self.n); return v

    def imadd(self, Vector other, float s):
        assert len(self) == len(other), "{} {}".format(self, other)
        vec_madd(self.v, self.v, other.v, s, self.n); return self

    def lerp(self, Vector other, float t):
        assert len(self) == len(other), "{} {}".format(self, other)
        vec_lerp(self.v, self.v, other.v, t, self.n); return self

    def lerped(self, Vector other, float t):
        assert len(self) == len(other), "{} {}".format(self, other)

        cdef Vector v = Vector(self.n)
        vec_lerp(v.v, self.v, other.v, t, self.n); return v

    def reflect(self, Vector normal):
        assert len(self) == len(normal), "{} {}".format(self, normal)
        vec_reflect(self.v, self.v, normal.v, self.n); return self

    def reflected(self, Vector normal):
        assert len(self) == len(normal), "{} {}".format(self, normal)

        cdef Vector v = Vector(self.n)
        vec_reflect(v.v, self.v, normal.v, self.n); return v

    def floor(self):
        vec_floor(self.v, self.v, self.n); return self

    def floored(self):
        cdef Vector v = Vector(self.n)
        vec_floor(v.v, self.v, self.n); return v

    def ceil(self):
        vec_ceil(self.v, self.v, self.n); return self

    def ceiled(self):
        cdef Vector v = Vector(self.n)
        vec_ceil(v.v, self.v, self.n); return v

    def round(self):
        vec_round(self.v, self.v, self.n); return self

    def rounded(self):
        cdef Vector v = Vector(self.n)
        vec_round(v.v, self.v, self.n); return v

    def cmp(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)
        return vec_cmp(self.v, other.v, self.n)

    def cmp_e(self, Vector other, float epsilon):
        assert len(self) == len(other), "{} {}".format(self, other)
        return vec_cmp_e(self.v, other.v, epsilon, self.n)

    def zero(self):
        vec_zero(self.v, self.n); return self

    def is_zero(self):
        return vec_is_zero(self.v, self.n)

    def is_zero_e(self, float epsilon):
        return vec_is_zero_e(self.v, epsilon, self.n)

    def is_unit(self):
        return vec_is_unit(self.v, self.n)

    def is_unit_e(self, float epsilon):
        return vec_is_unit_e(self.v, epsilon, self.n)

    def fix_denormals_ex(self, float denormal):
        return vec_fix_denormals_ex(self.v, denormal, self.n)

    def fix_denormals(self):
        return vec_fix_denormals(self.v, self.n)

    def min_flt(self, float other):
        cdef Vector v = Vector(self.n)
        vec_min_flt(v.v, self.v, other, self.n); return v

    def max_flt(self, float other):
        cdef Vector v = Vector(self.n)
        vec_max_flt(v.v, self.v, other, self.n); return v

    def min_vec(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)

        cdef Vector v = Vector(self.n)
        vec_min_vec(v.v, self.v, other.v, self.n); return v

    def max_vec(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)

        cdef Vector v = Vector(self.n)
        vec_max_vec(v.v, self.v, other.v, self.n); return v

    def imin_flt(self, float other):
        vec_min_flt(self.v, self.v, other, self.n); return self

    def imax_flt(self, float other):
        vec_max_flt(self.v, self.v, other, self.n); return self

    def imin_vec(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)
        vec_min_vec(self.v, self.v, other.v, self.n); return self

    def imax_vec(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)
        vec_max_vec(self.v, self.v, other.v, self.n); return self

    def add_flt(self, float other):
        cdef Vector v = Vector(self.n)
        vec_add_flt(v.v, self.v, other, self.n); return v

    def add_vec(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)

        cdef Vector v = Vector(self.n)
        vec_add_vec(v.v, self.v, other.v, self.n); return v

    def iadd_flt(self, float other):
        vec_add_flt(self.v, self.v, other, self.n); return self

    def iadd_vec(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)
        vec_add_vec(self.v, self.v, other.v, self.n); return self

    translate = iadd_vec
    translated = add_vec

    def sub_flt(self, float other):
        cdef Vector v = Vector(self.n)
        vec_sub_flt(v.v, self.v, other, self.n); return v

    def sub_vec(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)

        cdef Vector v = Vector(self.n)
        vec_sub_vec(v.v, self.v, other.v, self.n); return v

    def isub_flt(self, float other):
        vec_sub_flt(self.v, self.v, other, self.n); return self

    def isub_vec(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)
        vec_sub_vec(self.v, self.v, other.v, self.n); return self

    def mul_flt(self, float other):
        cdef Vector v = Vector(self.n)
        vec_mul_flt(v.v, self.v, other, self.n); return v

    def mul_vec(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)

        cdef Vector v = Vector(self.n)
        vec_mul_vec(v.v, self.v, other.v, self.n); return v

    def imul_flt(self, float other):
        vec_mul_flt(self.v, self.v, other, self.n); return self

    def imul_vec(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)
        vec_mul_vec(self.v, self.v, other.v, self.n); return self

    scale = imul_flt
    scaled = mul_flt

    def div_flt(self, float other):
        cdef Vector v = Vector(self.n)
        vec_div_flt(v.v, self.v, other, self.n); return v

    def div_vec(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)

        cdef Vector v = Vector(self.n)
        vec_div_vec(v.v, self.v, other.v, self.n); return v

    def idiv_flt(self, float other):
        vec_div_flt(self.v, self.v, other, self.n); return self

    def idiv_vec(self, Vector other):
        assert len(self) == len(other), "{} {}".format(self, other)
        vec_div_vec(self.v, self.v, other.v, self.n); return self

# ==============================================================================
# ~ [ fixed-size vector classes ]
# ==============================================================================

cdef class Vec2:
    cdef float v[2]

    def __init__(self, float x=0, float y=0):
        self.v[0] = x; self.v[1] = y

    def __repr__(self):
        return "{}{}".format(self.__class__.__name__, tuple(self))

    def __richcmp__(self, Vec2 other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: Vec2')

    def __copy__(self):
        cdef Vec2 v = Vec2()
        vec2copy(v.v, self.v); return v

    copy = __copy__

    def __neg__(self):
        cdef Vec2 v = Vec2()
        vec2neg(v.v, self.v); return v

    def __nonzero__(self):
        return not vec2is_zero(self.v)

    property x:
        def __get__(self): return self.v[0]
        def __set__(self, float value): self.v[0] = value

    property y:
        def __get__(self): return self.v[1]
        def __set__(self, float value): self.v[1] = value

    property u:
        def __get__(self): return self.v[0]
        def __set__(self, float value): self.v[0] = value

    property v:
        def __get__(self): return self.v[1]
        def __set__(self, float value): self.v[1] = value

    def __len__(self): return 2

    def __getitem__(self, int key):
        if key < 0 or key > 1: raise IndexError
        return self.v[key]

    def __setitem__(self, int key, float value):
        if key < 0 or key > 1: raise IndexError
        self.v[key] = value

    def __reduce__(self):
        cdef bytes b = b'\0' * sizeof(float[2])
        cdef char* p = <char*>b

        vec2copy(<float*>p, self.v)
        return (Vec2, (), b)

    def __setstate__(self, bytes state):
        cdef char* p = <char*>state
        vec2copy(self.v, <float*>p)

    def address(self):
        return <size_t>self.v

    def sum(self):
        return vec2sum(self.v)

    def dot(self, Vec2 other):
        return vec2dot(self.v, other.v)

    def __abs__(self):
        return vec2mag(self.v)

    mag = magnitude = __abs__

    def magnitude_squared(self):
        return vec2mag2(self.v)

    mag2 = magnitude_squared

    def dist(self, Vec2 other):
        return vec2dist(self.v, other.v)

    def dist2(self, Vec2 other):
        return vec2dist2(self.v, other.v)

    distance = dist
    distance_squared = dist2

    def angle(self, Vec2 other):
        return vec2angle(self.v, other.v)

    def neg(self):
        vec2neg(self.v, self.v); return self

    def reciprocate(self):
        vec2reciprocal(self.v, self.v); return self

    def reciprocal(self):
        cdef Vec2 v = Vec2()
        vec2reciprocal(v.v, self.v); return v

    def normalize(self):
        vec2normalize(self.v, self.v); return self

    def normalized(self):
        cdef Vec2 v = Vec2() # make unit vec
        vec2normalize(v.v, self.v); return v

    def clamped_vec(self, Vec2 minv, Vec2 maxv):
        cdef Vec2 v = Vec2()
        vec2clamp_vec(v.v, self.v, minv.v, maxv.v); return v

    def clamp_vec(self, Vec2 minv, Vec2 maxv):
        vec2clamp_vec(self.v, self.v, minv.v, maxv.v); return self

    def clamped_flt(self, float minv, float maxv):
        cdef Vec2 v = Vec2()
        vec2clamp_flt(v.v, self.v, minv, maxv); return v

    def clamp_flt(self, float minv, float maxv):
        vec2clamp_flt(self.v, self.v, minv, maxv); return self

    def madd(self, Vec2 other, float s):
        cdef Vec2 v = Vec2()
        vec2madd(v.v, self.v, other.v, s); return v

    def imadd(self, Vec2 other, float s):
        vec2madd(self.v, self.v, other.v, s); return self

    def lerp(self, Vec2 other, float t):
        vec2lerp(self.v, self.v, other.v, t); return self

    def lerped(self, Vec2 other, float t):
        cdef Vec2 v = Vec2()
        vec2lerp(v.v, self.v, other.v, t); return v

    def reflect(self, Vec2 normal):
        vec2reflect(self.v, self.v, normal.v); return self

    def reflected(self, Vec2 normal):
        cdef Vec2 v = Vec2()
        vec2reflect(v.v, self.v, normal.v); return v

    def floor(self):
        vec2floor(self.v, self.v); return self

    def floored(self):
        cdef Vec2 v = Vec2() # round down
        vec2floor(v.v, self.v); return v

    def ceil(self):
        vec2ceil(self.v, self.v); return self

    def ceiled(self):
        cdef Vec2 v = Vec2() # round up
        vec2ceil(v.v, self.v); return v

    def round(self):
        vec2round(self.v, self.v); return self

    def rounded(self):
        cdef Vec2 v = Vec2() # round vals
        vec2round(v.v, self.v); return v

    def cmp(self, Vec2 other):
        return vec2cmp(self.v, other.v)

    def cmp_e(self, Vec2 other, float epsilon):
        return vec2cmp_e(self.v, other.v, epsilon)

    def zero(self):
        vec2zero(self.v); return self

    def is_zero(self):
        return vec2is_zero(self.v)

    def is_zero_e(self, float epsilon):
        return vec2is_zero_e(self.v, epsilon)

    def is_unit(self):
        return vec2is_unit(self.v)

    def is_unit_e(self, float epsilon):
        return vec2is_unit_e(self.v, epsilon)

    def fix_denormals_ex(self, float denormal):
        return vec2fix_denormals_ex(self.v, denormal)

    def fix_denormals(self):
        return vec2fix_denormals(self.v)

    def min_flt(self, float other):
        cdef Vec2 v = Vec2()
        vec2min_flt(v.v, self.v, other); return v

    def max_flt(self, float other):
        cdef Vec2 v = Vec2()
        vec2max_flt(v.v, self.v, other); return v

    def min_vec(self, Vec2 other):
        cdef Vec2 v = Vec2()
        vec2min_vec(v.v, self.v, other.v); return v

    def max_vec(self, Vec2 other):
        cdef Vec2 v = Vec2()
        vec2max_vec(v.v, self.v, other.v); return v

    def imin_flt(self, float other):
        vec2min_flt(self.v, self.v, other); return self

    def imax_flt(self, float other):
        vec2max_flt(self.v, self.v, other); return self

    def imin_vec(self, Vec2 other):
        vec2min_vec(self.v, self.v, other.v); return self

    def imax_vec(self, Vec2 other):
        vec2max_vec(self.v, self.v, other.v); return self

    def add_flt(self, float other):
        cdef Vec2 v = Vec2()
        vec2add_flt(v.v, self.v, other); return v

    def add_vec(self, Vec2 other):
        cdef Vec2 v = Vec2()
        vec2add_vec(v.v, self.v, other.v); return v

    def iadd_flt(self, float other):
        vec2add_flt(self.v, self.v, other); return self

    def iadd_vec(self, Vec2 other):
        vec2add_vec(self.v, self.v, other.v); return self

    translate = iadd_vec
    translated = add_vec

    def sub_flt(self, float other):
        cdef Vec2 v = Vec2()
        vec2sub_flt(v.v, self.v, other); return v

    def sub_vec(self, Vec2 other):
        cdef Vec2 v = Vec2()
        vec2sub_vec(v.v, self.v, other.v); return v

    def isub_flt(self, float other):
        vec2sub_flt(self.v, self.v, other); return self

    def isub_vec(self, Vec2 other):
        vec2sub_vec(self.v, self.v, other.v); return self

    def mul_flt(self, float other):
        cdef Vec2 v = Vec2()
        vec2mul_flt(v.v, self.v, other); return v

    def mul_vec(self, Vec2 other):
        cdef Vec2 v = Vec2()
        vec2mul_vec(v.v, self.v, other.v); return v

    def imul_flt(self, float other):
        vec2mul_flt(self.v, self.v, other); return self

    def imul_vec(self, Vec2 other):
        vec2mul_vec(self.v, self.v, other.v); return self

    scale = imul_flt
    scaled = mul_flt

    def div_flt(self, float other):
        cdef Vec2 v = Vec2()
        vec2div_flt(v.v, self.v, other); return v

    def div_vec(self, Vec2 other):
        cdef Vec2 v = Vec2()
        vec2div_vec(v.v, self.v, other.v); return v

    def idiv_flt(self, float other):
        vec2div_flt(self.v, self.v, other); return self

    def idiv_vec(self, Vec2 other):
        vec2div_vec(self.v, self.v, other.v); return self

    def rotate_ex(self, float sin_theta, float cos_theta):
        vec2rotate_ex(self.v, self.v, sin_theta, cos_theta); return self

    def rotated_ex(self, float sin_theta, float cos_theta):
        cdef Vec2 v = Vec2()
        vec2rotate_ex(v.v, self.v, sin_theta, cos_theta); return v

    def rotate(self, float theta):
        vec2rotate(self.v, self.v, theta); return self

    def rotated(self, float theta):
        cdef Vec2 v = Vec2()
        vec2rotate(v.v, self.v, theta); return v

    def point_rotate_ex(self, Vec2 center, float sin_theta, float cos_theta):
        rotate_point2_ex(self.v, self.v, center.v, sin_theta, cos_theta); return self

    def point_rotated_ex(self, Vec2 center, float sin_theta, float cos_theta):
        cdef Vec2 v = Vec2()
        rotate_point2_ex(v.v, self.v, center.v, sin_theta, cos_theta); return v

    def point_rotate(self, Vec2 center, float theta):
        rotate_point2(self.v, self.v, center.v, theta); return self

    def point_rotated(self, Vec2 center, float theta):
        cdef Vec2 v = Vec2()
        rotate_point2(v.v, self.v, center.v, theta); return v

cdef class Vec3:
    cdef float v[3]

    def __init__(self, float x=0, float y=0, float z=0):
        self.v[0] = x; self.v[1] = y; self.v[2] = z

    def __repr__(self):
        return "{}{}".format(self.__class__.__name__, tuple(self))

    def __richcmp__(self, Vec3 other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: Vec3')

    def __copy__(self):
        cdef Vec3 v = Vec3()
        vec3copy(v.v, self.v); return v

    copy = __copy__

    def __neg__(self):
        cdef Vec3 v = Vec3()
        vec3neg(v.v, self.v); return v

    def __nonzero__(self):
        return not vec3is_zero(self.v)

    property x:
        def __get__(self): return self.v[0]
        def __set__(self, float value): self.v[0] = value

    property y:
        def __get__(self): return self.v[1]
        def __set__(self, float value): self.v[1] = value

    property z:
        def __get__(self): return self.v[2]
        def __set__(self, float value): self.v[2] = value

    property u:
        def __get__(self): return self.v[0]
        def __set__(self, float value): self.v[0] = value

    property v:
        def __get__(self): return self.v[1]
        def __set__(self, float value): self.v[1] = value

    property w:
        def __get__(self): return self.v[2]
        def __set__(self, float value): self.v[2] = value

    property r:
        def __get__(self): return self.v[0]
        def __set__(self, float value): self.v[0] = value

    property g:
        def __get__(self): return self.v[1]
        def __set__(self, float value): self.v[1] = value

    property b:
        def __get__(self): return self.v[2]
        def __set__(self, float value): self.v[2] = value

    def __len__(self): return 3

    def __getitem__(self, int key):
        if key < 0 or key > 2: raise IndexError
        return self.v[key]

    def __setitem__(self, int key, float value):
        if key < 0 or key > 2: raise IndexError
        self.v[key] = value

    def __reduce__(self):
        cdef bytes b = b'\0' * sizeof(float[3])
        cdef char* p = <char*>b

        vec3copy(<float*>p, self.v)
        return (Vec3, (), b)

    def __setstate__(self, bytes state):
        cdef char* p = <char*>state
        vec3copy(self.v, <float*>p)

    def address(self):
        return <size_t>self.v

    def sum(self):
        return vec3sum(self.v)

    def dot(self, Vec3 other):
        return vec3dot(self.v, other.v)

    def __abs__(self):
        return vec3mag(self.v)

    mag = magnitude = __abs__

    def magnitude_squared(self):
        return vec3mag2(self.v)

    mag2 = magnitude_squared

    def dist(self, Vec3 other):
        return vec3dist(self.v, other.v)

    def dist2(self, Vec3 other):
        return vec3dist2(self.v, other.v)

    distance = dist
    distance_squared = dist2

    def angle(self, Vec3 other):
        return vec3angle(self.v, other.v)

    def neg(self):
        vec3neg(self.v, self.v); return self

    def reciprocate(self):
        vec3reciprocal(self.v, self.v); return self

    def reciprocal(self):
        cdef Vec3 v = Vec3()
        vec3reciprocal(v.v, self.v); return v

    def normalize(self):
        vec3normalize(self.v, self.v); return self

    def normalized(self):
        cdef Vec3 v = Vec3() # make unit vec
        vec3normalize(v.v, self.v); return v

    def clamped_vec(self, Vec3 minv, Vec3 maxv):
        cdef Vec3 v = Vec3()
        vec3clamp_vec(v.v, self.v, minv.v, maxv.v); return v

    def clamp_vec(self, Vec3 minv, Vec3 maxv):
        vec3clamp_vec(self.v, self.v, minv.v, maxv.v); return self

    def clamped_flt(self, float minv, float maxv):
        cdef Vec3 v = Vec3()
        vec3clamp_flt(v.v, self.v, minv, maxv); return v

    def clamp_flt(self, float minv, float maxv):
        vec3clamp_flt(self.v, self.v, minv, maxv); return self

    def madd(self, Vec3 other, float s):
        cdef Vec3 v = Vec3()
        vec3madd(v.v, self.v, other.v, s); return v

    def imadd(self, Vec3 other, float s):
        vec3madd(self.v, self.v, other.v, s); return self

    def lerp(self, Vec3 other, float t):
        vec3lerp(self.v, self.v, other.v, t); return self

    def lerped(self, Vec3 other, float t):
        cdef Vec3 v = Vec3()
        vec3lerp(v.v, self.v, other.v, t); return v

    def reflect(self, Vec3 normal):
        vec3reflect(self.v, self.v, normal.v); return self

    def reflected(self, Vec3 normal):
        cdef Vec3 v = Vec3()
        vec3reflect(v.v, self.v, normal.v); return v

    def floor(self):
        vec3floor(self.v, self.v); return self

    def floored(self):
        cdef Vec3 v = Vec3() # round down
        vec3floor(v.v, self.v); return v

    def ceil(self):
        vec3ceil(self.v, self.v); return self

    def ceiled(self):
        cdef Vec3 v = Vec3() # round up
        vec3ceil(v.v, self.v); return v

    def round(self):
        vec3round(self.v, self.v); return self

    def rounded(self):
        cdef Vec3 v = Vec3() # round vals
        vec3round(v.v, self.v); return v

    def cmp(self, Vec3 other):
        return vec3cmp(self.v, other.v)

    def cmp_e(self, Vec3 other, float epsilon):
        return vec3cmp_e(self.v, other.v, epsilon)

    def zero(self):
        vec3zero(self.v); return self

    def is_zero(self):
        return vec3is_zero(self.v)

    def is_zero_e(self, float epsilon):
        return vec3is_zero_e(self.v, epsilon)

    def is_unit(self):
        return vec3is_unit(self.v)

    def is_unit_e(self, float epsilon):
        return vec3is_unit_e(self.v, epsilon)

    def fix_denormals_ex(self, float denormal):
        return vec3fix_denormals_ex(self.v, denormal)

    def fix_denormals(self):
        return vec3fix_denormals(self.v)

    def min_flt(self, float other):
        cdef Vec3 v = Vec3()
        vec3min_flt(v.v, self.v, other); return v

    def max_flt(self, float other):
        cdef Vec3 v = Vec3()
        vec3max_flt(v.v, self.v, other); return v

    def min_vec(self, Vec3 other):
        cdef Vec3 v = Vec3()
        vec3min_vec(v.v, self.v, other.v); return v

    def max_vec(self, Vec3 other):
        cdef Vec3 v = Vec3()
        vec3max_vec(v.v, self.v, other.v); return v

    def imin_flt(self, float other):
        vec3min_flt(self.v, self.v, other); return self

    def imax_flt(self, float other):
        vec3max_flt(self.v, self.v, other); return self

    def imin_vec(self, Vec3 other):
        vec3min_vec(self.v, self.v, other.v); return self

    def imax_vec(self, Vec3 other):
        vec3max_vec(self.v, self.v, other.v); return self

    def add_flt(self, float other):
        cdef Vec3 v = Vec3()
        vec3add_flt(v.v, self.v, other); return v

    def add_vec(self, Vec3 other):
        cdef Vec3 v = Vec3()
        vec3add_vec(v.v, self.v, other.v); return v

    def iadd_flt(self, float other):
        vec3add_flt(self.v, self.v, other); return self

    def iadd_vec(self, Vec3 other):
        vec3add_vec(self.v, self.v, other.v); return self

    translate = iadd_vec
    translated = add_vec

    def sub_flt(self, float other):
        cdef Vec3 v = Vec3()
        vec3sub_flt(v.v, self.v, other); return v

    def sub_vec(self, Vec3 other):
        cdef Vec3 v = Vec3()
        vec3sub_vec(v.v, self.v, other.v); return v

    def isub_flt(self, float other):
        vec3sub_flt(self.v, self.v, other); return self

    def isub_vec(self, Vec3 other):
        vec3sub_vec(self.v, self.v, other.v); return self

    def mul_flt(self, float other):
        cdef Vec3 v = Vec3()
        vec3mul_flt(v.v, self.v, other); return v

    def mul_vec(self, Vec3 other):
        cdef Vec3 v = Vec3()
        vec3mul_vec(v.v, self.v, other.v); return v

    def imul_flt(self, float other):
        vec3mul_flt(self.v, self.v, other); return self

    def imul_vec(self, Vec3 other):
        vec3mul_vec(self.v, self.v, other.v); return self

    scale = imul_flt
    scaled = mul_flt

    def div_flt(self, float other):
        cdef Vec3 v = Vec3()
        vec3div_flt(v.v, self.v, other); return v

    def div_vec(self, Vec3 other):
        cdef Vec3 v = Vec3()
        vec3div_vec(v.v, self.v, other.v); return v

    def idiv_flt(self, float other):
        vec3div_flt(self.v, self.v, other); return self

    def idiv_vec(self, Vec3 other):
        vec3div_vec(self.v, self.v, other.v); return self

    def cross(self, Vec3 other):
        cdef Vec3 v = Vec3()
        vec3cross(v.v, self.v, other.v); return v

    def up_from_forward(self, Vec3 abs_up, Vec3 object_forward):
        vec3up_from_forward(self.v, abs_up.v, object_forward.v); return self

    def rad_euler_to_gl_normal(self, float heading, float pitch):
        euler_radians_to_gl_normal(self.v, heading, pitch); return self

    def deg_euler_to_gl_normal(self, float heading, float pitch):
        euler_degrees_to_gl_normal(self.v, heading, pitch); return self

    def gl_normal_to_rad_heading(self):
        return gl_normal_to_heading_radians(self.v)

    def gl_normal_to_deg_heading(self):
        return gl_normal_to_heading_degrees(self.v)

    def gl_normal_to_rad_pitch(self):
        return gl_normal_to_pitch_radians(self.v)

    def gl_normal_to_deg_pitch(self):
        return gl_normal_to_pitch_degrees(self.v)

    def rotate_ex(self, Vec3 axis, float sin_theta, float cos_theta):
        vec3rotate_ex(self.v, self.v, axis.v, sin_theta, cos_theta)
        return self

    def rotated_ex(self, Vec3 axis, float sin_theta, float cos_theta):
        cdef Vec3 v = Vec3()
        vec3rotate_ex(v.v, self.v, axis.v, sin_theta, cos_theta); return v

    def rotate(self, Vec3 axis, float theta):
        vec3rotate(self.v, self.v, axis.v, theta); return self

    def rotated(self, Vec3 axis, float theta):
        cdef Vec3 v = Vec3()
        vec3rotate(v.v, self.v, axis.v, theta); return v

    def fix_degenerate_normal(self):
        return vec3fix_degenerate_normal(self.v)

cdef class Vec4:
    cdef float v[4]

    def __init__(self, float x=0, float y=0, float z=0, float w=0):
        self.v[0] = x; self.v[1] = y; self.v[2] = z; self.v[3] = w

    def __repr__(self):
        return "{}{}".format(self.__class__.__name__, tuple(self))

    def __richcmp__(self, Vec4 other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: Vec4')

    def __copy__(self):
        cdef Vec4 v = Vec4()
        vec4copy(v.v, self.v); return v

    copy = __copy__

    def __neg__(self):
        cdef Vec4 v = Vec4()
        vec4neg(v.v, self.v); return v

    def __nonzero__(self):
        return not vec4is_zero(self.v)

    property x:
        def __get__(self): return self.v[0]
        def __set__(self, float value): self.v[0] = value

    property y:
        def __get__(self): return self.v[1]
        def __set__(self, float value): self.v[1] = value

    property z:
        def __get__(self): return self.v[2]
        def __set__(self, float value): self.v[2] = value

    property w:
        def __get__(self): return self.v[3]
        def __set__(self, float value): self.v[3] = value

    property r:
        def __get__(self): return self.v[0]
        def __set__(self, float value): self.v[0] = value

    property g:
        def __get__(self): return self.v[1]
        def __set__(self, float value): self.v[1] = value

    property b:
        def __get__(self): return self.v[2]
        def __set__(self, float value): self.v[2] = value

    property a:
        def __get__(self): return self.v[3]
        def __set__(self, float value): self.v[3] = value

    def __len__(self): return 4

    def __getitem__(self, int key):
        if key < 0 or key > 3: raise IndexError
        return self.v[key]

    def __setitem__(self, int key, float value):
        if key < 0 or key > 3: raise IndexError
        self.v[key] = value

    def __reduce__(self):
        cdef bytes b = b'\0' * sizeof(float[4])
        cdef char* p = <char*>b

        vec4copy(<float*>p, self.v)
        return (Vec4, (), b)

    def __setstate__(self, bytes state):
        cdef char* p = <char*>state
        vec4copy(self.v, <float*>p)

    def address(self):
        return <size_t>self.v

    def sum(self):
        return vec4sum(self.v)

    def dot(self, Vec4 other):
        return vec4dot(self.v, other.v)

    def __abs__(self):
        return vec4mag(self.v)

    mag = magnitude = __abs__

    def magnitude_squared(self):
        return vec4mag2(self.v)

    mag2 = magnitude_squared

    def dist(self, Vec4 other):
        return vec4dist(self.v, other.v)

    def dist2(self, Vec4 other):
        return vec4dist2(self.v, other.v)

    distance = dist
    distance_squared = dist2

    def angle(self, Vec4 other):
        return vec4angle(self.v, other.v)

    def neg(self):
        vec4neg(self.v, self.v); return self

    def reciprocate(self):
        vec4reciprocal(self.v, self.v); return self

    def reciprocal(self):
        cdef Vec4 v = Vec4()
        vec4reciprocal(v.v, self.v); return v

    def normalize(self):
        vec4normalize(self.v, self.v); return self

    def normalized(self):
        cdef Vec4 v = Vec4() # make unit vec
        vec4normalize(v.v, self.v); return v

    def clamped_vec(self, Vec4 minv, Vec4 maxv):
        cdef Vec4 v = Vec4()
        vec4clamp_vec(v.v, self.v, minv.v, maxv.v); return v

    def clamp_vec(self, Vec4 minv, Vec4 maxv):
        vec4clamp_vec(self.v, self.v, minv.v, maxv.v); return self

    def clamped_flt(self, float minv, float maxv):
        cdef Vec4 v = Vec4()
        vec4clamp_flt(v.v, self.v, minv, maxv); return v

    def clamp_flt(self, float minv, float maxv):
        vec4clamp_flt(self.v, self.v, minv, maxv); return self

    def madd(self, Vec4 other, float s):
        cdef Vec4 v = Vec4()
        vec4madd(v.v, self.v, other.v, s); return v

    def imadd(self, Vec4 other, float s):
        vec4madd(self.v, self.v, other.v, s); return self

    def lerp(self, Vec4 other, float t):
        vec4lerp(self.v, self.v, other.v, t); return self

    def lerped(self, Vec4 other, float t):
        cdef Vec4 v = Vec4()
        vec4lerp(v.v, self.v, other.v, t); return v

    def reflect(self, Vec4 normal):
        vec4reflect(self.v, self.v, normal.v); return self

    def reflected(self, Vec4 normal):
        cdef Vec4 v = Vec4()
        vec4reflect(v.v, self.v, normal.v); return v

    def floor(self):
        vec4floor(self.v, self.v); return self

    def floored(self):
        cdef Vec4 v = Vec4() # round down
        vec4floor(v.v, self.v); return v

    def ceil(self):
        vec4ceil(self.v, self.v); return self

    def ceiled(self):
        cdef Vec4 v = Vec4() # round up
        vec4ceil(v.v, self.v); return v

    def round(self):
        vec4round(self.v, self.v); return self

    def rounded(self):
        cdef Vec4 v = Vec4() # round vals
        vec4round(v.v, self.v); return v

    def cmp(self, Vec4 other):
        return vec4cmp(self.v, other.v)

    def cmp_e(self, Vec4 other, float epsilon):
        return vec4cmp_e(self.v, other.v, epsilon)

    def zero(self):
        vec4zero(self.v); return self

    def is_zero(self):
        return vec4is_zero(self.v)

    def is_zero_e(self, float epsilon):
        return vec4is_zero_e(self.v, epsilon)

    def is_unit(self):
        return vec4is_unit(self.v)

    def is_unit_e(self, float epsilon):
        return vec4is_unit_e(self.v, epsilon)

    def fix_denormals_ex(self, float denormal):
        return vec4fix_denormals_ex(self.v, denormal)

    def fix_denormals(self):
        return vec4fix_denormals(self.v)

    def min_flt(self, float other):
        cdef Vec4 v = Vec4()
        vec4min_flt(v.v, self.v, other); return v

    def max_flt(self, float other):
        cdef Vec4 v = Vec4()
        vec4max_flt(v.v, self.v, other); return v

    def min_vec(self, Vec4 other):
        cdef Vec4 v = Vec4()
        vec4min_vec(v.v, self.v, other.v); return v

    def max_vec(self, Vec4 other):
        cdef Vec4 v = Vec4()
        vec4max_vec(v.v, self.v, other.v); return v

    def imin_flt(self, float other):
        vec4min_flt(self.v, self.v, other); return self

    def imax_flt(self, float other):
        vec4max_flt(self.v, self.v, other); return self

    def imin_vec(self, Vec4 other):
        vec4min_vec(self.v, self.v, other.v); return self

    def imax_vec(self, Vec4 other):
        vec4max_vec(self.v, self.v, other.v); return self

    def add_flt(self, float other):
        cdef Vec4 v = Vec4()
        vec4add_flt(v.v, self.v, other); return v

    def add_vec(self, Vec4 other):
        cdef Vec4 v = Vec4()
        vec4add_vec(v.v, self.v, other.v); return v

    def iadd_flt(self, float other):
        vec4add_flt(self.v, self.v, other); return self

    def iadd_vec(self, Vec4 other):
        vec4add_vec(self.v, self.v, other.v); return self

    translate = iadd_vec
    translated = add_vec

    def sub_flt(self, float other):
        cdef Vec4 v = Vec4()
        vec4sub_flt(v.v, self.v, other); return v

    def sub_vec(self, Vec4 other):
        cdef Vec4 v = Vec4()
        vec4sub_vec(v.v, self.v, other.v); return v

    def isub_flt(self, float other):
        vec4sub_flt(self.v, self.v, other); return self

    def isub_vec(self, Vec4 other):
        vec4sub_vec(self.v, self.v, other.v); return self

    def mul_flt(self, float other):
        cdef Vec4 v = Vec4()
        vec4mul_flt(v.v, self.v, other); return v

    def mul_vec(self, Vec4 other):
        cdef Vec4 v = Vec4()
        vec4mul_vec(v.v, self.v, other.v); return v

    def imul_flt(self, float other):
        vec4mul_flt(self.v, self.v, other); return self

    def imul_vec(self, Vec4 other):
        vec4mul_vec(self.v, self.v, other.v); return self

    scale = imul_flt
    scaled = mul_flt

    def div_flt(self, float other):
        cdef Vec4 v = Vec4()
        vec4div_flt(v.v, self.v, other); return v

    def div_vec(self, Vec4 other):
        cdef Vec4 v = Vec4()
        vec4div_vec(v.v, self.v, other.v); return v

    def idiv_flt(self, float other):
        vec4div_flt(self.v, self.v, other); return self

    def idiv_vec(self, Vec4 other):
        vec4div_vec(self.v, self.v, other.v); return self
