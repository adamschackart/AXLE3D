# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
from lin cimport Plane, plane_copy
from vec cimport Vec2, Vec3
from mat cimport Mat4x4

# ==============================================================================
# ~ [ axis-aligned bounding boxes ]
# ==============================================================================

cdef extern from "ae_math.h":
    void aabbox2_get_center( float center_out_point[2],
            const float min_v[2], const float max_v[2])

    void aabbox3_get_center( float center_out_point[3],
            const float min_v[3], const float max_v[3])

    void aabbox2_set_center(float minv[2], float maxv[2],
                            const float center_point2[2])

    void aabbox3_set_center(float minv[3], float maxv[3],
                            const float center_point3[3])

    void aabbox2_center_in( float minv[2], float maxv[2],
        const float dst_minv[2], const float dst_maxv[2])

    void aabbox3_center_in( float minv[3], float maxv[3],
        const float dst_minv[3], const float dst_maxv[3])

    # TODO: aabbox3 surface area, maybe box2 volume that returns 0? :P
    float aabbox2_area(const float minv[2], const float maxv[2])
    float aabbox3_volume(const float minv[3], const float maxv[3])

    int aabbox2_vs_aabbox2( const float amin[2], const float amax[2],
                            const float bmin[2], const float bmax[2])

    int aabbox3_vs_aabbox3( const float amin[3], const float amax[3],
                            const float bmin[3], const float bmax[3])

    int aabbox2_vs_point(const float minv[2], const float maxv[2],
                                        const float test_point[2])

    int aabbox3_vs_point(const float minv[3], const float maxv[3],
                                        const float test_point[3])

    void aabbox2_points( float out[ 8], const float minv[2],
                                        const float maxv[2])

    void aabbox3_points( float out[24], const float minv[3],
                                        const float maxv[3])

    void aabbox2_rotated_bounds_ex( float min_out[2],
                                    float max_out[2],

                                    const float min_val[2],
                                    const float max_val[2],

                                    const float center[2],
                                    const float sin_theta,
                                    const float cos_theta)

    void aabbox2_rotated_bounds(float min_out[2],
                                float max_out[2],

                                const float min_val[2],
                                const float max_val[2],

                                const float center[2],
                                const float theta)

# ===== [ python api ] =========================================================

def get_center2(Vec2 minv, Vec2 maxv):
    cdef Vec2 v = Vec2()
    aabbox2_get_center(v.v, minv.v, maxv.v); return v

def get_center3(Vec3 minv, Vec3 maxv):
    cdef Vec3 v = Vec3()
    aabbox3_get_center(v.v, minv.v, maxv.v); return v

def set_center2(Vec2 minv, Vec2 maxv, Vec2 center):
    aabbox2_set_center(minv.v, maxv.v, center.v)

def set_center3(Vec3 minv, Vec3 maxv, Vec3 center):
    aabbox3_set_center(minv.v, maxv.v, center.v)

def center_in2(Vec2 minv, Vec2 maxv, Vec2 dst_minv, Vec2 dst_maxv):
    aabbox2_center_in(minv.v, maxv.v, dst_minv.v, dst_maxv.v)

def center_in3(Vec3 minv, Vec3 maxv, Vec3 dst_minv, Vec3 dst_maxv):
    aabbox3_center_in(minv.v, maxv.v, dst_minv.v, dst_maxv.v)

def area2(Vec2 minv, Vec2 maxv):
    return aabbox2_area(minv.v, maxv.v)

def volume3(Vec3 minv, Vec3 maxv):
    return aabbox3_volume(minv.v, maxv.v)

def intersect_box2(Vec2 amin, Vec2 amax, Vec2 bmin, Vec2 bmax):
    return aabbox2_vs_aabbox2(amin.v, amax.v, bmin.v, bmax.v)

def intersect_box3(Vec3 amin, Vec3 amax, Vec3 bmin, Vec3 bmax):
    return aabbox3_vs_aabbox3(amin.v, amax.v, bmin.v, bmax.v)

def intersect_point2(Vec2 minv, Vec2 maxv, Vec2 point):
    return aabbox2_vs_point(minv.v, maxv.v, point.v)

def intersect_point3(Vec3 minv, Vec3 maxv, Vec3 point):
    return aabbox3_vs_point(minv.v, maxv.v, point.v)

def rotated_bounds2_ex( Vec2 minv, Vec2 maxv, Vec2 center,
                        float sin_theta, float cos_theta):
    """
    Get the axis-aligned bounding box of a rotated (oriented) axis-aligned box.
    If None is passed to center, then the rotation will be about aabbox center.
    """
    cdef Vec2 min_out = Vec2()
    cdef Vec2 max_out = Vec2()

    aabbox2_rotated_bounds_ex(  min_out.v, max_out.v, minv.v, maxv.v,
                                NULL if center is None else center.v,
                                sin_theta, cos_theta)
    return (min_out, max_out)

def rotated_bounds2(Vec2 minv, Vec2 maxv, Vec2 center, float theta):
    cdef Vec2 min_out = Vec2()
    cdef Vec2 max_out = Vec2()

    aabbox2_rotated_bounds( min_out.v, max_out.v, minv.v, maxv.v, # angle
                            NULL if center is None else center.v, theta )

    return (min_out, max_out)

# ==============================================================================
# ~ [ frustum culling ]
# ==============================================================================
# TODO: translate and transform the frustum planes. a naive plane transformation
# won't work, as the frustum plane normals are not unit length. sort it out! ;(
# ------------------------------------------------------------------------------

cdef extern from "ae_math.h":
    void plane_frustum_copy(float dst[24], const float src[24])
    int plane_frustum_cmp(const float a[24], const float b[24])
    int plane_frustum_cmp_e(const float a[24], const float b[24], const float e)

    void plane_frustum_zero(float p[24])
    int plane_frustum_is_zero(const float p[24])
    int plane_frustum_is_zero_e(const float p[24], const float e)

    void plane_frustum_from_model_view_projection_matrix(float* p, float* m)
    void plane_frustum_normalize(float planes[24]) # need for sphere testing

    int plane_frustum_contains(const float planes[24],
            const float center[3], const float radius)

    float plane_frustum_distance(const float planes[24],
            const float center[3], const float radius)

    int plane_frustum_contains_aabbox(const float planes[24],
                        const float p1[3], const float p2[3])

# ===== [ plane frustum class ] ================================================

cdef class PlaneFrustum:
    """A truncated pyramid shape outlining the field of view. Performs standard
    plane side tests using the dot product against six inward-facing planes."""
    cdef float planes[24] # in order: right, left, top, bottom, far, near plane

    def __richcmp__(self, PlaneFrustum other, int op):
        cdef int cmpv = self.cmp(other)
        if op == 2: return     cmpv
        if op == 3: return not cmpv
        raise TypeError('unorderable type: PlaneFrustum')

    def __copy__(self):
        cdef PlaneFrustum other = PlaneFrustum()
        plane_frustum_copy(other.planes, self.planes); return other

    copy = __copy__

    def __nonzero__(self):
        return not plane_frustum_is_zero(self.planes)

    def __len__(self): return 6

    def __getitem__(self, int index):
        cdef Plane plane = Plane()

        if index > 5: raise IndexError # for tuple()
        plane_copy(plane.p, &self.planes[index * 4])

        return plane

    def __setitem__(self, int index, Plane plane):
        if index > 5: raise IndexError # for tuple()
        plane_copy(&self.planes[index * 4], plane.p)

    def __reduce__(self):
        cdef bytes b = b'\0' * sizeof(float[24])
        cdef char* p = <char *>b

        plane_frustum_copy(<float*>p, self.planes)
        return (PlaneFrustum, (), b)

    def __setstate__(self, bytes state):
        plane_frustum_copy(self.planes, <float*>(<char*>state))

    def address(self):
        return <size_t>self.planes

    def cmp(self, PlaneFrustum other):
        return plane_frustum_cmp(self.planes, other.planes)

    def cmp_e(self, PlaneFrustum other, float epsilon):
        return plane_frustum_cmp_e(self.planes, other.planes, epsilon)

    def zero(self):
        plane_frustum_zero(self.planes); return self

    def is_zero(self):
        return plane_frustum_is_zero(self.planes)

    def is_zero_e(self, float epsilon):
        return plane_frustum_is_zero_e(self.planes, epsilon)

    def from_model_view_projection_matrix(self, Mat4x4 clip):
        cdef float* p = self.planes
        cdef float* m = clip.m

        plane_frustum_from_model_view_projection_matrix(p, m)
        return self

    def normalize(self):
        plane_frustum_normalize(self.planes); return self

    def contains(self, Vec3 center, float radius=0):
        return plane_frustum_contains(self.planes, center.v, radius)

    def distance(self, Vec3 center, float radius=0):
        return plane_frustum_distance(self.planes, center.v, radius)

    def contains_aabbox(self, Vec3 p1, Vec3 p2):
        return plane_frustum_contains_aabbox(self.planes, p1.v, p2.v)
