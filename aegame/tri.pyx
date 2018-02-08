# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
from idx cimport *
from mem cimport *
from vtx cimport *
from lin cimport *

cdef extern from "ae_triangle.h":
    # ===== [ misc. utils ] ====================================================

    float tri_area( const float* v0, const float* v1,
                    const float* v2, const size_t n)

    float tri2area(const float v0[2], const float v1[2], const float v2[2])
    float tri3area(const float v0[3], const float v1[3], const float v2[3])

    void tri_centroid(float * center, const float * v0, const float * v1,
                                        const float * v2, const size_t n)

    void tri2centroid(float center[2], const float v0[2],
                    const float v1[2], const float v2[2])

    void tri3centroid(float center[3], const float v0[3],
                    const float v1[3], const float v2[3])

    void tri3face_normal(float out[3], const float v0[3],
                    const float v1[3], const float v2[3])

    # ===== [ depth sorting ] ==================================================

    void tri3_mesh_depthsort_ex(const float point[3],
                                float* vertices,
                                size_t floats,
                                size_t offset,
                                size_t stride,
                                void * indices,
                                ae_index_type_t type,
                                size_t i_count)

    void tri3_mesh_depthsort(   const float point[3],
                                float* vertices,
                                size_t floats,
                                void * indices,
                                ae_index_type_t type,
                                size_t i_count)

    # ===== [ raycasting (collision detection) ] ===============================

    void tri3_ray_hit_point(float point[3],
                            const float ray_pos[3],
                            const float ray_dir[3],
                            const float vert0[3],
                            const float vert1[3],
                            const float vert2[3],
                            const float t,
                            const float u,
                            const float v)

    int tri3_vs_ray(float ray_pos[3],
                    float ray_dir[3],
                    float vert0[3],
                    float vert1[3],
                    float vert2[3],
                    float *t,
                    float *u,
                    float *v,
                    float epsilon)

    int tri3_mesh_vs_ray_ex(float ray_pos[3],
                            float ray_dir[3],
                            float* vertices,
                            size_t floats,
                            size_t offset,
                            size_t stride,
                            void * indices,
                            ae_index_type_t type,
                            size_t i_count,
                            u32 hit_indices[3],
                            float *t,
                            float *u,
                            float *v,
                            float epsilon)

    int tri3_mesh_vs_ray(   float ray_pos[3],
                            float ray_dir[3],
                            float* vertices,
                            size_t floats,
                            void * indices,
                            ae_index_type_t type,
                            size_t i_count,
                            u32 hit_indices[3],
                            float *t,
                            float *u,
                            float *v,
                            float epsilon)

    # ===== [ inversion (face swapping) ] ======================================

    void tri3_mesh_invert_ex(float *vertices,

                            size_t floats,
                            size_t stride,

                            void * indices,
                            ae_index_type_t type,
                            size_t i_count)

    void tri3_mesh_invert(  float* vertices,
                            size_t floats,

                            void * indices,
                            ae_index_type_t type,
                            size_t i_count)

# ==============================================================================
# ~ [ wrapper class ]
# ==============================================================================
# TODO: this API isn't properly wrapped as free functions (and for single tris)
# ------------------------------------------------------------------------------

cdef class TriangleVertexArray(VertexArray):
    def get_subview(self, size_t offset, size_t length, object array_t=TriangleVertexArray):
        return VertexArray.get_subview(self, offset, length, array_t)

    def chunk(self, size_t chunk_size=0, object chunk_t=TriangleVertexArray):
        return VertexArray.chunk(self, chunk_size, chunk_t)

    def get_region(self, size_t offset, size_t length, object array_t=TriangleVertexArray):
        return VertexArray.get_region(self, offset, length, array_t)

    def intersect_ray3( self, Ray3 ray,
                        IndexArray indices=IndexArray(AE_INDEX_TYPE_U32),
                        float epsilon=0.0000001):
        """Trace a ray through self and return nearest point hit, or None."""
        cdef u32 hit_idx[3]
        cdef int hit
        cdef Vec3 hit_point
        cdef float t, u, v

        cdef size_t offset = self.position_offset()

        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.position_size() == 3, "bad vert format: {}".format(self)

        if size == 3:
            hit = tri3_mesh_vs_ray(
                            ray.p.v, ray.v.v, <float*>self.array.data,
                            self.array.size // sizeof(float),
                            indices.array.data, indices.index_type,
                            indices.count(), hit_idx, &t, &u, &v, epsilon)
        else:
            hit = tri3_mesh_vs_ray_ex(
                            ray.p.v, ray.v.v, <float*>self.array.data,
                            self.array.size // sizeof(float), offset, size,
                            indices.array.data, indices.index_type,
                            indices.count(), hit_idx, &t, &u, &v, epsilon)

        if hit:
            # compute the point hit by the ray. hopefully a lot of this unused
            # information and its calculation will be removed by the compiler.
            hit_point = Vec3()

            tri3_ray_hit_point(hit_point.v, ray.p.v, ray.v.v,
                    &(<float*>self.array.data)[hit_idx[0] * size + offset],
                    &(<float*>self.array.data)[hit_idx[1] * size + offset],
                    &(<float*>self.array.data)[hit_idx[2] * size + offset],
                    t, u, v)

            return ((hit_idx[0], hit_idx[1], hit_idx[2]), hit_point, t, u, v)

    def depthsort(self, Vec3 point, IndexArray indices=IndexArray(AE_INDEX_TYPE_U32)):
        """
            Sort triangles from back to front based on distance from a viewpoint.
        """
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        assert self.position_size() == 3, "bad vert format: {}".format(self)

        if size == 3:
            tri3_mesh_depthsort(
                point.v, <float*>self.array.data, self.array.size // sizeof(float),
                indices.array.data, indices.index_type, indices.count())
        else:
            tri3_mesh_depthsort_ex(
                point.v, <float*>self.array.data, self.array.size // sizeof(float),
                self.position_offset(), size,
                indices.array.data, indices.index_type, indices.count())

        return self

    def invert_faces(self, IndexArray indices=IndexArray(AE_INDEX_TYPE_U32)):
        """
            Swap faces, so each triangle is made to face the opposite direction.
        """
        cdef size_t size = ae_vertex_format_size[<size_t>self.vertex_format]
        cdef size_t fnum = self.array.size // sizeof(float)

        if size == 3:
            tri3_mesh_invert(<float*>self.array.data, fnum, indices.array.data,
                                        indices.index_type, indices.count())
        else:
            tri3_mesh_invert_ex(<float*>self.array.data, fnum, size, indices. \
                            array.data, indices.index_type, indices.count())

        return self
