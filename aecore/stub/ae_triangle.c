/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

void tri3_mesh_depthsort_ex(const float point[3],

                            float* vertices,
                            size_t floats,
                            size_t offset,
                            size_t stride,

                            void * indices,
                            ae_index_type_t type,
                            size_t i_count)
{
    AE_STUB();
}

void tri3_mesh_depthsort(   const float point[3],

                            float* vertices,
                            size_t floats,

                            void * indices,
                            ae_index_type_t type,
                            size_t i_count)
{
    AE_STUB();
}

int tri3_vs_ray(float ray_pos[3],
                float ray_dir[3],
                float vert0[3],
                float vert1[3],
                float vert2[3],
                float *t,
                float *u,
                float *v,
                float epsilon)
{
    AE_STUB(); return 0;
}

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
{
    AE_STUB(); return 0;
}

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
{
    AE_STUB(); return 0;
}

void tri3_mesh_invert_ex(   float* vertices,

                            size_t floats,
                            size_t stride,

                            void * indices,
                            ae_index_type_t type,
                            size_t i_count)
{
    AE_STUB();
}

void tri3_mesh_invert(  float* vertices,
                        size_t floats,

                        void * indices,
                        ae_index_type_t type,
                        size_t i_count)
{
    AE_STUB();
}

void ae_triangle_init(int argc, char** argv)
{
    // AE_STUB();
}

void ae_triangle_quit(void)
{
    // AE_STUB();
}
