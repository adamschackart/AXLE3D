# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
from vec cimport *
from lin cimport *
from utl cimport *
from mem cimport *

cdef extern from "ae_polygon.h":
    # ===== [ collision polygons ] =============================================

    ctypedef struct ae_plane_winding_t:
        float* edges # array of edge planes
        size_t count # count of edge planes

        float face[4] # polygon face plane
        float vert[3] # point on polygon
        float bmin[3] # aabbox bounds min
        float bmax[3] # aabbox bounds max

    void pw_alloc_edges(ae_plane_winding_t* winding, size_t count)
    void pw_free_edges(ae_plane_winding_t* winding)

    void pw_copy_winding(ae_plane_winding_t* dst, ae_plane_winding_t* src)

    # compares the edge planes and members of A against B and returns true if they are equal
    int pw_winding_cmp(const ae_plane_winding_t* const a, const ae_plane_winding_t* const b)

    int pw_winding_cmp_e(const ae_plane_winding_t* const a, # fuzzy comparison
                        const ae_plane_winding_t* const b, const float epsilon)

    int pw_create_from_tri(ae_plane_winding_t * dst, const float v0[3],
        const float v1[3], const float v2[3], const int fix_degenerate)

    # ===== [ collision detection ] ============================================

    float pw_winding_vs_ellipsoid(const ae_plane_winding_t* const winding,
                        const float e_center[3], const float e_extent[3])

    int pw_winding_vs_line_type(float hit[3], const ae_plane_winding_t* const w,
        const float line_p[3], const float line_v[3], const ae_line_type_t type)

    int pw_winding_vs_line(float hit[3], const ae_plane_winding_t* const winding,
                            const float line_point[3], const float line_vec[3])

    int pw_winding_vs_line_segment(float hit[3], const ae_plane_winding_t* const winding,
                                    const float line_point[3], const float line_vec[3])

    int pw_winding_vs_ray(float hit[3], const ae_plane_winding_t* const winding,
                            const float line_point[3], const float line_vec[3])

    # ===== [ serialization ] ==================================================

    size_t pw_buffer_size(const ae_plane_winding_t* const winding)

    void pw_buffer_save(u8* out, const ae_plane_winding_t* const winding)
    void pw_buffer_load(ae_plane_winding_t* winding, u8* buf, size_t len)

# ==============================================================================
# ~ [ collision polygons ]
# ==============================================================================
# TODO translation by vector, row and column-major transformation by 4x4 matrix
# ------------------------------------------------------------------------------

cdef class PlaneWinding:
    """Convex polygon defined by parallel (face) and edge planes. Also contains an
    arbitrary point on the plane and axis-aligned box for fast collision tests."""
    cdef ae_plane_winding_t winding

    # XXX cpython zeroes structure data automatically, do other interpreters?
    # def __init__(self): memset(&self.winding, 0, sizeof(ae_plane_winding_t))

    def __dealloc__(self):
        pw_free_edges(&self.winding)

    def __repr__(self):
        return "PlaneWinding({} polygon edges)".format(self.winding.count)

    # XXX: when this is enabled, it disallows use of PlaneWindings as set members
    # unless we define __hash__ as well. even if we did that, it would dramatically
    # slow down the performance of the time-critical "get_windings_in_aabbox_v".

    # def __richcmp__(self, PlaneWinding other, int op):
    #     cdef int cmpv = self.cmp(other)
    #     if op == 2: return     cmpv
    #     if op == 3: return not cmpv
    #     raise TypeError('unorderable type: PlaneWinding')

    def __nonzero__(self):
        return self.winding.count

    def __copy__(self):
        cdef PlaneWinding other = PlaneWinding() # XXX use __class__?
        pw_copy_winding(&other.winding, &self.winding); return other

    copy = __copy__

    def __len__(self):
        return self.winding.count

    def __getitem__(self, size_t index):
        cdef Plane p

        if index >= self.winding.count: raise IndexError # for tuples
        p = Plane(); plane_copy(p.p, &self.winding.edges[index << 2])

        return p

    def __reduce__(self):
        cdef bytes b = b'\0' * pw_buffer_size(&self.winding)
        cdef char* p = <char*>b

        pw_buffer_save(<u8*>p, &self.winding)
        return (PlaneWinding, (), b) # struct & edge planes

    def __setstate__(self, bytes state):
        cdef char* p = <char*>state # edge count from length
        pw_buffer_load(&self.winding, <u8*>p, len(state))

    def cmp(self, PlaneWinding other):
        return pw_winding_cmp(&self.winding, &other.winding)

    def cmp_e(self, PlaneWinding other, float epsilon):
        return pw_winding_cmp_e(&self.winding, &other.winding, epsilon)

    # ===== [ construction ] ===================================================

    def from_polygon_points(self, list points, bint fix_degenerate=0):
        cdef Vec3 v0 = points[0]
        cdef Vec3 v1 = points[1]
        cdef Vec3 v2 = points[2]

        cdef Vec3 p1, p2, n
        cdef list l # temps

        cdef Plane p
        cdef int i

        if len(points) == 3:
            if pw_create_from_tri(&self.winding, v0.v, v1.v, v2.v, fix_degenerate):
                return self
            else:
                raise RuntimeError("points on plane are collinear") # ~15x speedup

        if not plane_from_points(self.winding.face, v0.v, v1.v, v2.v, fix_degenerate):
            raise RuntimeError("points on plane are collinear") # assumes coplanarity

        n = Vec3(); vec3copy(n.v, self.winding.face) # take out any non-planar points

        l = [Plane().from_point_vec(p1, p2.sub_vec(p1).cross(n), fix_degenerate) for \
        p1, p2 in zip(points[:], points[1:] + [points[0]]) if p2.sub_vec(p1).cross(n)]

        # TODO: see if this ever fires and maybe use a faster method?
        # assert len(l) == len(points), "{} != {}".format(l, points)

        pw_alloc_edges(&self.winding, len(l))

        for i in range(len(l)):
            p = l[i] # copy edge planes into winding ptr
            plane_copy(&self.winding.edges[i * 4], p.p)

        vec3copy(self.winding.vert, v0.v)
        vec3copy(self.winding.bmin, v0.v)
        vec3copy(self.winding.bmax, v0.v)

        for p1 in points:
            self.winding.bmin[0] = min(self.winding.bmin[0], p1.v[0])
            self.winding.bmin[1] = min(self.winding.bmin[1], p1.v[1])
            self.winding.bmin[2] = min(self.winding.bmin[2], p1.v[2])

            self.winding.bmax[0] = max(self.winding.bmax[0], p1.v[0])
            self.winding.bmax[1] = max(self.winding.bmax[1], p1.v[1])
            self.winding.bmax[2] = max(self.winding.bmax[2], p1.v[2])

        return self

    def from_quad_points(self, Vec3 v0, Vec3 v1, Vec3 v2, Vec3 v3, bint fix_degenerate=0):
        return self.from_polygon_points([v0, v1, v2, v3], fix_degenerate) # TODO optimize

    def from_triangle_points(self, Vec3 v0, Vec3 v1, Vec3 v2, bint fix_degenerate=0):
        if not pw_create_from_tri(&self.winding, v0.v, v1.v, v2.v, fix_degenerate):
            raise RuntimeError("points on plane are collinear") # faster tri path

        return self

    # ===== [ attribute getters ] ==============================================

    def get_face(self):
        cdef Plane p = Plane() # face plane
        plane_copy(p.p, self.winding.face)

        return p

    def get_face_normal(self):
        cdef Vec3 v = Vec3()

        # fast self.get_face().get_normal()
        vec3copy( v.v, self.winding.face )

        return v

    def get_point(self):
        cdef Vec3 v = Vec3() # spawn points
        vec3copy( v.v, self.winding.vert )

        return v

    def get_aabbox(self):
        cdef Vec3 bmin = Vec3()
        cdef Vec3 bmax = Vec3()

        vec3copy(bmin.v, self.winding.bmin)
        vec3copy(bmax.v, self.winding.bmax)

        return (bmin, bmax)

    # ===== [ collision detection ] ============================================

    def intersect_ellipsoid(self, Ellipsoid other):
        return pw_winding_vs_ellipsoid(&self.winding, other.c.v, other.v.v)

    def intersect_line3(self, Line3 line):
        cdef Vec3 v = Vec3() # hit point not used if nothing hit

        cdef int rv = pw_winding_vs_line_type( v.v, &self.winding,
                                line.p.v, line.v.v, line.type() )

        return v if rv else None

# ==============================================================================
# ~ [ collision mesh ]
# ==============================================================================

cdef class CollisionMesh:
    """Sparse uniform grid of cells containing plane windings for collision."""

    cdef readonly float interval
    cdef readonly dict grid

    def __init__(self, list windings = [], float interval = 5.0):
        """Create an empty hash, optionally adding polygons to cells.
        This is the only place where interval can be safely assigned."""
        assert interval > 0.0, "invalid interval: {}".format(interval)

        self.grid, self.interval = {}, interval
        if windings: self.add_windings(windings)

    def __repr__(self):
        return "{}({} cells)".format(self.__class__.__name__, len(self.grid))

    def __hash__(self):
        raise TypeError('{} is not hashable'.format(self.__class__.__name__))

    def __copy__(self):
        raise TypeError('{} is not copyable'.format(self.__class__.__name__))

    def __reduce__(self):
        return (self.__class__, ([], self.interval), self.grid)

    def __setstate__(self, dict state):
        self.grid = state # TODO: save in space-optimal format

    cdef void _changed(self, bytes op_name):
        if ae_log_is_enabled_str("POLYGON"):
            ae_log_str("POLYGON", "%s_windings: grid with interval %f now has %u cells",
                        <char*>op_name, self.interval, <unsigned int>len(self.grid))

            ae_log_str("POLYGON", "%s_windings: grid now averages %f polygons per cell",
                    <char*>op_name, <float>sum([len(l) for l in self.grid.values()]) / \
                                                        float(len(self.grid.values())) )

    def add_windings(self, list windings):
        """Add a list of windings to the spatial hash. Doesn't check if the
        windings are already in the hash, which would waste time and space."""

        cdef void * ctx = ae_profile_enter( "aegame/cvx.pyx",
                                "CollisionMesh.add_windings")

        cdef PlaneWinding winding # current poly
        cdef int x1, y1, z1, x2, y2, z2, x, y, z

        # cell interval reciprocal for fast hashing
        cdef float r_interval = 1.0 / self.interval

        for winding in windings:
            x1 = <int>ae_floorf(winding.winding.bmin[0] * r_interval)
            y1 = <int>ae_floorf(winding.winding.bmin[1] * r_interval)
            z1 = <int>ae_floorf(winding.winding.bmin[2] * r_interval)
            x2 = <int>ae_floorf(winding.winding.bmax[0] * r_interval) + 1
            y2 = <int>ae_floorf(winding.winding.bmax[1] * r_interval) + 1
            z2 = <int>ae_floorf(winding.winding.bmax[2] * r_interval) + 1

            for x in range(x1, x2):
                for y in range(y1, y2):
                    for z in range(z1, z2):
                        try:
                            self.grid[x, y, z].append(winding)
                        except KeyError:
                            self.grid[x, y, z] = [winding]

        self._changed(b"add")
        ae_profile_leave(ctx)

    def del_windings(self, list windings):
        """Remove a list of windings from the spatial hash. If a cell is empty after
        having all of its windings removed, it will not be deleted from the grid."""

        cdef void * ctx = ae_profile_enter( "aegame/cvx.pyx",
                                "CollisionMesh.del_windings")

        cdef PlaneWinding winding # current poly
        cdef int x1, y1, z1, x2, y2, z2, x, y, z

        # cell interval reciprocal for fast hashing
        cdef float r_interval = 1.0 / self.interval

        # FIXME: this is O(N^2). we could make performance linear by getting the
        # total bounding box of all input windings, iterating through the cells
        # contained in that box, and testing each value against a set made of the
        # input windings. this should be done in a list comprehension for speed.
        # this could also just use sets instead of lists to store cell windings,
        # although that may cause memory usage to balloon (sets are much larger).

        for winding in windings:
            x1 = <int>ae_floorf(winding.winding.bmin[0] * r_interval)
            y1 = <int>ae_floorf(winding.winding.bmin[1] * r_interval)
            z1 = <int>ae_floorf(winding.winding.bmin[2] * r_interval)
            x2 = <int>ae_floorf(winding.winding.bmax[0] * r_interval) + 1
            y2 = <int>ae_floorf(winding.winding.bmax[1] * r_interval) + 1
            z2 = <int>ae_floorf(winding.winding.bmax[2] * r_interval) + 1

            for x in range(x1, x2):
                for y in range(y1, y2):
                    for z in range(z1, z2):
                        try:
                            self.grid[x, y, z].remove(winding)
                        except:
                            pass # index error (or key error?)

        self._changed(b"del")
        ae_profile_leave(ctx)

    def clear_empty_cells(self):
        """Remove all empty cells from the spatial hash. Call this only after
        del_windings (offline). Returns the total number of cells removed."""

        cdef void * ctx = ae_profile_enter( "aegame/cvx.pyx" ,
                            "CollisionMesh.clear_empty_cells")

        cdef size_t ln = len(self.grid) # old dict size
        cdef list keys = list(self.grid.keys())
        cdef tuple key # safely delete whilst iterating

        for key in keys: # this is quadratic in pypy!
            if not self.grid[key]: del self.grid[key]

        ae_profile_leave(ctx); return ln - len(self.grid)

    cdef set get_windings_in_aabbox_v(self, float[3] p1, float[3] p2):
        cdef void * ctx = ae_profile_enter( "aegame/cvx.pyx",
                    "CollisionMesh.get_windings_in_aabbox_v")

        # cell interval reciprocal for fast hashing
        cdef float r_interval = 1.0 / self.interval

        cdef int x1 = <int>ae_floorf(p1[0] * r_interval)
        cdef int y1 = <int>ae_floorf(p1[1] * r_interval)
        cdef int z1 = <int>ae_floorf(p1[2] * r_interval)
        cdef int x2 = <int>ae_floorf(p2[0] * r_interval) + 1
        cdef int y2 = <int>ae_floorf(p2[1] * r_interval) + 1
        cdef int z2 = <int>ae_floorf(p2[2] * r_interval) + 1

        cdef set windings = set()
        cdef int x, y, z

        for x in range(x1, x2):
            for y in range(y1, y2):
                for z in range(z1, z2):
                    if (x, y, z) in self.grid:
                        windings.update(self.grid[x, y, z])

        # return polygons that intersect with box
        ae_profile_leave(ctx); return windings

    def get_windings_in_aabbox(self, Vec3 p1, Vec3 p2):
        return self.get_windings_in_aabbox_v(p1.v, p2.v)

    def intersect_ellipsoid(self, Ellipsoid ellipsoid):
        """Get all intersections between ellipsoid and self. Returns a list of
        (winding, penetration of ellipsoid through winding along face normal)."""

        cdef void* ctx = ae_profile_enter( "aegame/cvx.pyx",
                        "CollisionMesh.intersect_ellipsoid")

        cdef PlaneWinding winding
        cdef list results = []
        cdef float p1[3]
        cdef float p2[3]
        cdef float d

        # get the axis-aligned bounding box of ellipsoid
        vec3sub_vec(p1, ellipsoid.c.v, ellipsoid.v.v)
        vec3add_vec(p2, ellipsoid.c.v, ellipsoid.v.v)

        for winding in self.get_windings_in_aabbox_v(p1, p2):
            d = pw_winding_vs_ellipsoid(&winding.winding,
                            ellipsoid.c.v, ellipsoid.v.v)

            if d: results.append((winding, d))

        ae_profile_leave(ctx); return results

    def intersect_floor(self, LineSegment3 segment, int up_index=1,
                        float floor_max_radians=0.7):
        """Returns the highest point of contact and winding that a line segment
        intersects, or (None, None) if nothing hit. up_index defaults to +Y up,
        and floor_max_radians is max walkable surface angle (40ish degrees)."""

        cdef void * ctx = ae_profile_enter("aegame/cvx.pyx",
                            "CollisionMesh.intersect_floor")

        cdef PlaneWinding winding
        cdef Vec3 hit_pt = Vec3()

        cdef PlaneWinding best_winding = None
        cdef Vec3 best = None # trace results

        cdef int linehit
        cdef float p1[3]
        cdef float p2[3]
        cdef float q2[3]

        # calculate line segment extents for aabbox
        vec3add_vec(q2, segment.p.v, segment.v.v)

        p1[0] = min(segment.p.v[0], q2[0])
        p1[1] = min(segment.p.v[1], q2[1])
        p1[2] = min(segment.p.v[2], q2[2])
        p2[0] = max(segment.p.v[0], q2[0])
        p2[1] = max(segment.p.v[1], q2[1])
        p2[2] = max(segment.p.v[2], q2[2])

        for winding in self.get_windings_in_aabbox_v(p1, p2):
            #if winding.get_face_normal().dot(up_vec) < floor_max_radians: continue
            if winding.winding.face[up_index] < floor_max_radians: continue # wall?

            linehit = pw_winding_vs_line_segment( hit_pt.v,
                &winding.winding, segment.p.v, segment.v.v)

            if linehit and (best is None or hit_pt.v[up_index] > best.v[up_index]):
                best = Vec3(); vec3copy(best.v, hit_pt.v) # FIXME: mandatory pos up
                best_winding = winding # need to copy hit point to avoid overwrites

        ae_profile_leave(ctx); return best, best_winding
