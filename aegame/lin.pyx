# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
from vec cimport *

cdef extern from "ae_math.h":
    # ===== [ quadric shapes ] =================================================

    int ellipse_vs_ellipse( const float a_c[2], const float a_v[2],
                            const float b_c[2], const float b_v[2])

    int ellipsoid_vs_ellipsoid( const float a_c[3], const float a_v[3],
                                const float b_c[3], const float b_v[3])

    int ellipsoid_vs_ray(float* hd, const float p[3], const float e[3],
                                const float r_p[3], const float r_d[3])

    # ===== [ line math ] ======================================================

    int u_in_line(const float u)
    int u_in_line_segment(const float u)
    int u_in_ray(const float u)

    ctypedef enum ae_line_type_t:
        AE_LINE_TYPE_LINE
        AE_LINE_TYPE_LINE_SEGMENT
        AE_LINE_TYPE_RAY

    const char* ae_line_type_name[] # get str name of type enum
    int u_in_line_type(const float u, const ae_line_type_t type)

    int plane_vs_line_type( float hit[3], float line_p[3],
        float line_v[3], ae_line_type_t t, float plane[4])

    int plane_vs_line(float hit[3], float lp[3], float lv[3], float plane[4])
    int plane_vs_line_segment(float hit[3], float lp[3], float lv[3], float plane[4])
    int plane_vs_ray(float hit[3], float lp[3], float lv[3], float plane[4])

    # ===== [ planar math ] ====================================================

    void plane_copy(float dst[4], const float src[4])
    int plane_cmp(const float a[4], const float b[4])
    int plane_cmp_e(const float a[4], const float b[4], const float epsilon)

    void plane_zero(float dst[4])
    int plane_is_zero(const float p[4])
    int plane_is_zero_e(const float p[4], const float epsilon)

    void plane_neg(float dst[4], const float src[4])

    void plane_translate(float p[4], const float vec[3])
    float plane_distance(const float p[4], const float vec[3])
    void point_on_plane(float out[3], const float p[4])

    ctypedef enum ae_plane_side_t:
        AE_PLANE_SIDE_FRONT
        AE_PLANE_SIDE_BACK
        AE_PLANE_SIDE_ON

    const char* ae_plane_side_name[]

    ae_plane_side_t plane_side( const float plane[4],
            const float vec[3], const float epsilon)

    ctypedef enum ae_plane_type_t:
        AE_PLANE_TYPE_X
        AE_PLANE_TYPE_Y
        AE_PLANE_TYPE_Z
        AE_PLANE_TYPE_NEG_X
        AE_PLANE_TYPE_NEG_Y
        AE_PLANE_TYPE_NEG_Z
        AE_PLANE_TYPE_TRUE_AXIAL
        AE_PLANE_TYPE_ZERO_X
        AE_PLANE_TYPE_ZERO_Y
        AE_PLANE_TYPE_ZERO_Z
        AE_PLANE_TYPE_NON_AXIAL

    const char* ae_plane_type_name[]
    ae_plane_type_t plane_type(const float plane[4])

    float plane_normalize(float out[4], const float plane[4], const int fix_degenerate)

    int plane_from_point_vec(float plane[4],
        const float p[3], const float v[3], const int fix_degenerate)

    int plane_from_vecs(float plane[4], const float dir1[3],
        const float dir2[3], const float p[3], const int fix_degenerate)

    int plane_from_points(float plane[4], const float p1[3],
        const float p2[3], const float p3[3], const int fix_degenerate)

    # ===== [ aabbox math ] ====================================================

    void ellipse_get_outer_aabbox2( float minv[2], float maxv[2],
                const float e_center[2], const float e_extent[2])

    void ellipsoid_get_outer_aabbox3(float minv[3], float maxv[3],
                const float e_center[3], const float e_extent[3])

    void aabbox2_get_inner_ellipse( float e_center[2], float e_extent[2],
                                const float minv[2], const float maxv[2])

    void aabbox3_get_inner_ellipsoid(float e_center[3], float e_extent[3],
                                const float minv[3], const float maxv[3])

# ==============================================================================
# ~ [ 2d lines ]
# ==============================================================================

cdef class Line2:
    cdef public Vec2 p # origin
    cdef public Vec2 v # extent

    def __init__(self, Vec2 origin, Vec2 extent):
        self.p = Vec2()
        self.v = Vec2()

        vec2copy(self.p.v, origin.v)
        vec2copy(self.v.v, extent.v)

    @classmethod
    def from_points(cls, Vec2 p0, Vec2 p1):
        cdef Vec2 extent = Vec2() # normal

        vec2sub_vec(extent.v, p1.v, p0.v)
        return cls(p0, extent) # non-unit

    def __repr__(self):
        return 'Line2(<{}> + u<{}>)'.format(self.p, self.v)

    def __richcmp__(self, Line2 other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: Line2')

    def __copy__(self):
        return self.__class__(self.p, self.v)

    copy = __copy__

    def __reduce__(self):
        return (self.__class__, (self.p, self.v))

    cpdef type(self):
        return AE_LINE_TYPE_LINE

    def type_name(self):
        return ae_line_type_name[<size_t>AE_LINE_TYPE_LINE]

    def u_in(self, float u):
        return u_in_line(u)

    def cmp(self, Line2 other):
        return (vec2cmp(self.p.v, other.p.v) and
                vec2cmp(self.v.v, other.v.v))

    def cmp_e(self, Line2 other, float epsilon):
        return (vec2cmp_e(self.p.v, other.p.v, epsilon) and
                vec2cmp_e(self.v.v, other.v.v, epsilon))

cdef class LineSegment2(Line2):
    def __repr__(self):
        return 'LineSegment2(<{}> to <{}>)'.format(self.p, self.p.add_vec(self.v))

    cpdef type(self):
        return AE_LINE_TYPE_LINE_SEGMENT

    def type_name(self):
        return ae_line_type_name[<size_t>AE_LINE_TYPE_LINE_SEGMENT]

    def u_in(self, float u):
        return u_in_line_segment(u)

cdef class Ray2(Line2):
    def __repr__(self):
        return 'Ray2(<{}> + u<{}>)'.format(self.p, self.v)

    cpdef type(self):
        return AE_LINE_TYPE_RAY

    def type_name(self):
        return ae_line_type_name[<size_t>AE_LINE_TYPE_RAY]

    def u_in(self, float u):
        return u_in_ray(u)

# ==============================================================================
# ~ [ 3d lines ]
# ==============================================================================

cdef class Line3:
    cdef public Vec3 p # origin
    cdef public Vec3 v # extent

    def __init__(self, Vec3 origin, Vec3 extent):
        self.p = Vec3()
        self.v = Vec3()

        vec3copy(self.p.v, origin.v)
        vec3copy(self.v.v, extent.v)

    @classmethod
    def from_points(cls, Vec3 p0, Vec3 p1):
        cdef Vec3 extent = Vec3() # normal

        vec3sub_vec(extent.v, p1.v, p0.v)
        return cls(p0, extent) # non-unit

    def __repr__(self):
        return 'Line3(<{}> + u<{}>)'.format(self.p, self.v)

    def __richcmp__(self, Line3 other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: Line3')

    def __copy__(self):
        return self.__class__(self.p, self.v)

    copy = __copy__

    def __reduce__(self):
        return (self.__class__, (self.p, self.v))

    cpdef type(self):
        return AE_LINE_TYPE_LINE

    def type_name(self):
        return ae_line_type_name[<size_t>AE_LINE_TYPE_LINE]

    def u_in(self, float u):
        return u_in_line(u)

    def cmp(self, Line3 other):
        return (vec3cmp(self.p.v, other.p.v) and
                vec3cmp(self.v.v, other.v.v))

    def cmp_e(self, Line3 other, float epsilon):
        return (vec3cmp_e(self.p.v, other.p.v, epsilon) and
                vec3cmp_e(self.v.v, other.v.v, epsilon))

    def intersect_plane(self, Plane other):
        cdef Vec3 hit_point = Vec3() # returns None if nothing hit

        # if plane_vs_line_type( hit_point.v, self.p.v, self.v.v,
        #                 self.type(), other.p): return hit_point

        if plane_vs_line(hit_point.v, self.p.v, self.v.v, other.p):
            return hit_point

    def intersect(self, object other):
        return other.intersect_line3(self)

cdef class LineSegment3(Line3):
    def __repr__(self):
        return 'LineSegment3(<{}> to <{}>)'.format(self.p, self.p.add_vec(self.v))

    cpdef type(self):
        return AE_LINE_TYPE_LINE_SEGMENT

    def type_name(self):
        return ae_line_type_name[<size_t>AE_LINE_TYPE_LINE_SEGMENT]

    def u_in(self, float u):
        return u_in_line_segment(u)

    def intersect_plane(self, Plane other):
        cdef Vec3 hit = Vec3() # None if the line segment and plane don't collide
        if plane_vs_line_segment(hit.v, self.p.v, self.v.v, other.p): return hit

cdef class Ray3(Line3):
    def __repr__(self):
        return 'Ray3(<{}> + u<{}>)'.format(self.p, self.v)

    cpdef type(self):
        return AE_LINE_TYPE_RAY

    def type_name(self):
        return ae_line_type_name[<size_t>AE_LINE_TYPE_RAY]

    def u_in(self, float u):
        return u_in_ray(u)

    def intersect_plane(self, Plane other):
        cdef Vec3 hit = Vec3() # None if the ray and plane don't collide
        if plane_vs_ray(hit.v, self.p.v, self.v.v, other.p): return hit

# ==============================================================================
# ~ [ circles & spheres ]
# ==============================================================================

cdef class Circle:
    cdef public Vec2  c # center
    cdef public float r # radius

    def __init__(self, Vec2 center, float radius):
        self.c = Vec2(); vec2copy(self.c.v, center.v)
        self.r = radius

    def __repr__(self):
        return '{}(<{}>, radius={})'.format(self.__class__.__name__, self.c, self.r)

    def __richcmp__(self, Circle other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: {}'.format(self.__class__.__name__))

    def __copy__(self):
        return self.__class__(self.c, self.r)

    copy = __copy__

    def __reduce__(self):
        return (Circle, (self.c, self.r))

    def cmp(self, Circle other):
        return vec2cmp(self.c.v, other.c.v) and self.r == other.r

    def cmp_e(self, Circle other, float epsilon):
        return (vec2cmp_e(self.c.v, other.c.v, epsilon) and not
                abs(self.r - other.r) > epsilon) # emits fabsf

    def as_ellipse(self):
        return Ellipse(self.c, Vec2(self.r, self.r))

cdef class Sphere:
    cdef public Vec3  c # center
    cdef public float r # radius

    def __init__(self, Vec3 center, float radius):
        self.c = Vec3(); vec3copy(self.c.v, center.v)
        self.r = radius

    def __repr__(self):
        return '{}(<{}>, radius={})'.format(self.__class__.__name__, self.c, self.r)

    def __richcmp__(self, Sphere other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: {}'.format(self.__class__.__name__))

    def __copy__(self):
        return self.__class__(self.c, self.r)

    copy = __copy__

    def __reduce__(self):
        return (Sphere, (self.c, self.r))

    def cmp(self, Sphere other):
        return vec3cmp(self.c.v, other.c.v) and self.r == other.r

    def cmp_e(self, Sphere other, float epsilon):
        return (vec3cmp_e(self.c.v, other.c.v, epsilon) and not
                abs(self.r - other.r) > epsilon) # emits fabsf

    def as_ellipsoid(self):
        return Ellipsoid(self.c, Vec3(self.r, self.r, self.r))

# ==============================================================================
# ~ [ elliptical shapes ]
# ==============================================================================

cdef class Ellipse:
    cdef public Vec2 c # center
    cdef public Vec2 v # radius

    def __init__(self, Vec2 center, Vec2 extent):
        self.c = Vec2() # origin
        self.v = Vec2() # extent

        vec2copy(self.c.v, center.v)
        vec2copy(self.v.v, extent.v)

    def __repr__(self):
        return "{}(c:{}, v:{})".format(self.__class__.__name__, self.c, self.v)

    def __richcmp__(self, Ellipse other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: {}'.format(self.__class__.__name__))

    def __copy__(self):
        return self.__class__(self.c, self.v)

    copy = __copy__

    def __reduce__(self):
        return (Ellipse, (self.c, self.v))

    def cmp(self, Ellipse other):
        return (vec2cmp(self.c.v, other.c.v) and
                vec2cmp(self.v.v, other.v.v))

    def cmp_e(self, Ellipse other, float epsilon):
        return (vec2cmp_e(self.c.v, other.c.v, epsilon) and
                vec2cmp_e(self.v.v, other.v.v, epsilon))

    def get_outer_aabbox(self):
        cdef Vec2 minv = Vec2()
        cdef Vec2 maxv = Vec2()

        ellipse_get_outer_aabbox2(minv.v, maxv.v, self.c.v, self.v.v)
        return (minv, maxv)

    @classmethod
    def aabbox_interior(cls, Vec2 minv, Vec2 maxv):
        cdef Vec2 center = Vec2()
        cdef Vec2 extent = Vec2()

        aabbox2_get_inner_ellipse(center.v, extent.v, minv.v, maxv.v)
        return cls(center, extent)

    def intersect_ellipse(self, Ellipse other):
        return ellipse_vs_ellipse(self.c.v, self.v.v, other.c.v, other.v.v)

    def intersect(self, object other):
        return other.intersect_ellipse(self)

cdef class Ellipsoid:
    cdef public Vec3 c # center
    cdef public Vec3 v # radius

    def __init__(self, Vec3 center, Vec3 extent):
        self.c = Vec3() # origin
        self.v = Vec3() # extent

        vec3copy(self.c.v, center.v)
        vec3copy(self.v.v, extent.v)

    def __repr__(self):
        return "{}(c:{}, v:{})".format(self.__class__.__name__, self.c, self.v)

    def __richcmp__(self, Ellipsoid other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: {}'.format(self.__class__.__name__))

    def __copy__(self):
        return self.__class__(self.c, self.v)

    copy = __copy__

    def __reduce__(self):
        return (Ellipsoid, (self.c, self.v))

    def cmp(self, Ellipsoid other):
        return (vec3cmp(self.c.v, other.c.v) and
                vec3cmp(self.v.v, other.v.v))

    def cmp_e(self, Ellipsoid other, float epsilon):
        return (vec3cmp_e(self.c.v, other.c.v, epsilon) and
                vec3cmp_e(self.v.v, other.v.v, epsilon))

    def get_outer_aabbox(self):
        cdef Vec3 minv = Vec3()
        cdef Vec3 maxv = Vec3()

        ellipsoid_get_outer_aabbox3(minv.v, maxv.v, self.c.v, self.v.v)
        return (minv, maxv)

    @classmethod
    def aabbox_interior(cls, Vec3 minv, Vec3 maxv):
        cdef Vec3 center = Vec3()
        cdef Vec3 extent = Vec3()

        aabbox3_get_inner_ellipsoid(center.v, extent.v, minv.v, maxv.v)
        return cls(center, extent)

    def intersect_ray3(self, Ray3 other):
        return ellipsoid_vs_ray(NULL, self.c.v, self.v.v, other.p.v, other.v.v)

    def intersect_ellipsoid(self, Ellipsoid other):
        return ellipsoid_vs_ellipsoid(self.c.v, self.v.v, other.c.v, other.v.v)

    def intersect(self, object other):
        return other.intersect_ellipsoid(self)

# ==============================================================================
# ~ [ infinite 3D planes ]
# ==============================================================================
# TODO: quaternion transform, matrix transform with and without shear and scale
# ------------------------------------------------------------------------------

cdef class Plane:
    cdef float p[4] # TODO make this a four-element union?

    # point vs. plane side testing enumerants
    SIDE_FRONT = AE_PLANE_SIDE_FRONT
    SIDE_BACK = AE_PLANE_SIDE_BACK
    SIDE_ON = AE_PLANE_SIDE_ON

    # any plane type < 6 is a true axial plane
    TYPE_TRUE_AXIAL = AE_PLANE_TYPE_TRUE_AXIAL

    # the plane is axial in a positive direction
    TYPE_X = AE_PLANE_TYPE_X
    TYPE_Y = AE_PLANE_TYPE_Y
    TYPE_Z = AE_PLANE_TYPE_Z

    # the plane is axial in a negative direction
    TYPE_NEG_X = AE_PLANE_TYPE_NEG_X
    TYPE_NEG_Y = AE_PLANE_TYPE_NEG_Y
    TYPE_NEG_Z = AE_PLANE_TYPE_NEG_Z

    # non-axial plane with a zero component
    TYPE_ZERO_X = AE_PLANE_TYPE_ZERO_X
    TYPE_ZERO_Y = AE_PLANE_TYPE_ZERO_Y
    TYPE_ZERO_Z = AE_PLANE_TYPE_ZERO_Z

    # common case for arbitrary polygon faces
    TYPE_NON_AXIAL = AE_PLANE_TYPE_NON_AXIAL

    # TODO: cpython zeroes data automatically, does pypy?
    # def __init__(self): plane_zero(self.p)

    def __repr__(self):
        return 'Plane(<{}, {}, {}>.p = {})'.format(*self)

    def __richcmp__(self, Plane other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: Plane')

    def __copy__(self):
        cdef Plane p = Plane() # x,y,z,k
        plane_copy(p.p,self.p); return p

    copy = __copy__

    def __neg__(self):
        cdef Plane p = Plane() # negated
        plane_neg(p.p, self.p); return p

    def __nonzero__(self):
        return not plane_is_zero(self.p)

    def __len__(self): return 4

    def __getitem__(self, int key):
        if key < 0 or key > 3: raise IndexError
        return self.p[key]

    def __setitem__(self, int key, float value):
        if key < 0 or key > 3: raise IndexError
        self.p[key] = value

    def __reduce__(self):
        cdef bytes b = b'\0' * sizeof(float[4])
        cdef float * p = <float *>( <char *>b )

        plane_copy(p, self.p)
        return (Plane, (), b)

    def __setstate__(self, bytes state):
        cdef float * p = <float*>(<char*>state)
        plane_copy(self.p, p)

    def address(self):
        return <size_t>self.p

    def cmp(self, Plane other):
        return plane_cmp(self.p, other.p)

    def cmp_e(self, Plane other, float epsilon):
        return plane_cmp_e(self.p, other.p, epsilon)

    def zero(self):
        plane_zero(self.p); return self

    def is_zero(self):
        return plane_is_zero(self.p)

    def is_zero_e(self, float epsilon):
        return plane_is_zero_e(self.p, epsilon)

    def neg(self):
        plane_neg(self.p, self.p); return self

    def normalize(self, bint fix_degenerate=False):
        plane_normalize(self.p, self.p, fix_degenerate); return self

    def normalized(self, bint fix_degenerate=False):
        cdef Plane p = Plane()
        plane_normalize(p.p, self.p, fix_degenerate); return p

    def from_points(self, Vec3 p1, Vec3 p2, Vec3 p3, bint fix_degenerate=False):
        if not plane_from_points(self.p, p1.v, p2.v, p3.v, fix_degenerate):
            raise RuntimeError("points on plane are collinear")

        return self

    def from_vecs(self, Vec3 dir1, Vec3 dir2, Vec3 p, bint fix_degenerate=False):
        if not plane_from_vecs(self.p, dir1.v, dir2.v, p.v, fix_degenerate):
            raise RuntimeError("points on plane are collinear")

        return self

    def from_point_vec(self, Vec3 p, Vec3 v, bint fix_degenerate=False):
        if not plane_from_point_vec(self.p, p.v, v.v, fix_degenerate):
            raise RuntimeError("points on plane are collinear")

        return self

    property k:
        def __get__(self): return self.p[3] # dist scalar
        def __set__(self, float value): self.p[3] = value

    def get_normal(self):
        cdef Vec3 v = Vec3() # copy plane normal
        vec3copy(v.v,self.p); return v

    def set_normal(self, Vec3 normal):
        vec3copy(self.p, normal.v); return self

    def get_point(self):
        cdef Vec3 v = Vec3() # point on plane
        point_on_plane(v.v, self.p); return v

    def translate(self, Vec3 translation):
        plane_translate(self.p, translation.v); return self

    def distance(self, Vec3 point):
        return plane_distance(self.p, point.v)

    def side(self, Vec3 point, float epsilon):
        return plane_side(self.p, point.v, epsilon)

    def side_name(self, Vec3 point, float epsilon):
        return ae_plane_side_name[<size_t>plane_side(self.p, point.v, epsilon)]

    def type(self):
        return plane_type(self.p)

    def type_name(self):
        return ae_plane_type_name[<size_t>plane_type(self.p)]

    def intersect_line3(self, Line3 other):
        cdef Vec3 hit_point = Vec3() # returns None if nothing hit

        if plane_vs_line_type(hit_point.v, other.p.v, other.v.v,
                        other.type(), self.p): return hit_point

    def intersect(self, object other):
        return other.intersect_plane(self)
