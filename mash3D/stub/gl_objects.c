/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __GL_CORE_H__
#include <gl_core.h>
#endif

/*
================================================================================
 * ~~ [ object types ] ~~ *
--------------------------------------------------------------------------------
*/

gl_object_type_t gl_object_type(void* object)
{
    AE_STUB(); return GL_OBJECT_TYPE_UNKNOWN;
}

size_t gl_object_count_all(void)
{
    AE_STUB(); return 0;
}

void gl_object_list_all(void** objects)
{
    AE_STUB();
}

void gl_object_print_all(void)
{
    AE_STUB();
}

void gl_object_close_all(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ texture renderer ] ~~ *
--------------------------------------------------------------------------------
*/

void
gl_texture_set_int(gl_texture_t* texture, gl_texture_property_t property, int value)
{
    AE_STUB();
}

int
gl_texture_get_int(gl_texture_t* texture, gl_texture_property_t property)
{
    AE_STUB(); return 0;
}

void
gl_texture_set_str(gl_texture_t* texture, gl_texture_property_t property, const char* value)
{
    AE_STUB();
}

const char*
gl_texture_get_str(gl_texture_t* texture, gl_texture_property_t property)
{
    AE_STUB(); return "";
}

void
gl_texture_set_img(gl_texture_t* texture, gl_texture_property_t property, ae_image_t* value)
{
    AE_STUB();
}

ae_image_t*
gl_texture_get_img(gl_texture_t* texture, gl_texture_property_t property)
{
    AE_STUB(); return NULL;
}

gl_texture_t* gl_texture_create(int width, int height, int comp)
{
    AE_STUB(); return NULL;
}

gl_texture_t* gl_texture_load_from_memory(void* buf, size_t size)
{
    AE_STUB(); return NULL;
}

gl_texture_t*
gl_texture_load_from_memory_ex(void* buf, size_t size, ae_image_error_t* error_status)
{
    AE_STUB(); return NULL;
}

gl_texture_t* gl_texture_load(const char* filename)
{
    AE_STUB(); return NULL;
}

gl_texture_t* gl_texture_load_ex(const char* filename, ae_image_error_t* error_status)
{
    AE_STUB(); return NULL;
}

void gl_texture_bind(gl_texture_t* texture)
{
    AE_STUB();
}

void gl_texture_draw_ex(gl_texture_t* texture, float x, float y, const float rgba[4])
{
    AE_STUB();
}

void gl_texture_draw(gl_texture_t* texture, float x, float y)
{
    AE_STUB();
}

void gl_texture_draw_skybox_ex( gl_texture_t* front,
                                gl_texture_t* back,
                                gl_texture_t* left,
                                gl_texture_t* right,
                                gl_texture_t* up,
                                gl_texture_t* down,
                                gl_coord3D_t* viewer,
                                const float color[4])
{
    AE_STUB();
}

void gl_texture_draw_skybox(gl_texture_t* front,
                            gl_texture_t* back,
                            gl_texture_t* left,
                            gl_texture_t* right,
                            gl_texture_t* up,
                            gl_texture_t* down,
                            gl_coord3D_t* viewer)
{
    AE_STUB();
}

void gl_texture_list_all(gl_texture_t** textures)
{
    AE_STUB();
}

void gl_texture_print_all(void)
{
    AE_STUB();
}

void gl_texture_close_all(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ material system ] ~~ *
--------------------------------------------------------------------------------
*/

void
gl_material_set_int(gl_material_t* material, gl_material_property_t property, int value)
{
    AE_STUB();
}

int
gl_material_get_int(gl_material_t* material, gl_material_property_t property)
{
    AE_STUB(); return 0;
}

void
gl_material_set_u64(gl_material_t* material, gl_material_property_t property, u64 value)
{
    AE_STUB();
}

u64
gl_material_get_u64(gl_material_t* material, gl_material_property_t property)
{
    AE_STUB(); return 0;
}

void
gl_material_set_flt(gl_material_t* material, gl_material_property_t property, float value)
{
    AE_STUB();
}

float
gl_material_get_flt(gl_material_t* material, gl_material_property_t property)
{
    AE_STUB(); return 0.0f;
}

void
gl_material_set_vec(gl_material_t* material, gl_material_property_t property, float* value)
{
    AE_STUB();
}

float*
gl_material_get_vec(gl_material_t* material, gl_material_property_t property)
{
    AE_STUB(); return NULL;
}

void
gl_material_set_str(gl_material_t* material, gl_material_property_t property, const char* value)
{
    AE_STUB();
}

const char*
gl_material_get_str(gl_material_t* material, gl_material_property_t property)
{
    AE_STUB(); return "";
}

void
gl_material_set_tex(gl_material_t* material, gl_material_property_t property, gl_texture_t* value)
{
    AE_STUB();
}

gl_texture_t*
gl_material_get_tex(gl_material_t* material, gl_material_property_t property)
{
    AE_STUB(); return NULL;
}

gl_material_t* gl_material_create(void)
{
    AE_STUB(); return NULL;
}

void gl_material_set_all(gl_material_t* material, const float* value)
{
    AE_STUB();
}

void gl_material_lerp(gl_material_t* dst, gl_material_t* a, gl_material_t* b, float t)
{
    AE_STUB();
}

void gl_material_apply_ex(gl_material_t* material, gl_buffer_t* buffer)
{
    AE_STUB();
}

void gl_material_apply(gl_material_t* material)
{
    AE_STUB();
}

void gl_material_list_all(gl_material_t** materials)
{
    AE_STUB();
}

void gl_material_print_all(void)
{
    AE_STUB();
}

void gl_material_close_all(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ lighting system ] ~~ *
--------------------------------------------------------------------------------
*/

void
gl_light_set_int(gl_light_t* light, gl_light_property_t property, int value)
{
    AE_STUB();
}

int
gl_light_get_int(gl_light_t* light, gl_light_property_t property)
{
    AE_STUB(); return 0;
}

void
gl_light_set_str(gl_light_t* light, gl_light_property_t property, const char* value)
{
    AE_STUB();
}

const char*
gl_light_get_str(gl_light_t* light, gl_light_property_t property)
{
    AE_STUB(); return "";
}

void
gl_light_set_vec(gl_light_t* light, gl_light_property_t property, float* value)
{
    AE_STUB();
}

float*
gl_light_get_vec(gl_light_t* light, gl_light_property_t property)
{
    AE_STUB(); return NULL;
}

void
gl_light_set_flt(gl_light_t* light, gl_light_property_t property, float value)
{
    AE_STUB();
}

float
gl_light_get_flt(gl_light_t* light, gl_light_property_t property)
{
    AE_STUB(); return 0.0f;
}

gl_light_t* gl_light_create(void)
{
    AE_STUB(); return NULL;
}

void gl_light_set_all(gl_light_t* light, const float* value)
{
    AE_STUB();
}

void gl_light_lerp(gl_light_t* dst, gl_light_t* a, gl_light_t* b, float t)
{
    AE_STUB();
}

void gl_light_default_scene(void)
{
    AE_STUB();
}

void gl_light_apply_all(void)
{
    AE_STUB();
}

void gl_light_list_all(gl_light_t** lights)
{
    AE_STUB();
}

void gl_light_print_all(void)
{
    AE_STUB();
}

void gl_light_close_all(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ vertex buffers ] ~~ *
--------------------------------------------------------------------------------
*/

void
gl_buffer_set_int(gl_buffer_t* buffer, gl_buffer_property_t property, int value)
{
    AE_STUB();
}

int
gl_buffer_get_int(gl_buffer_t* buffer, gl_buffer_property_t property)
{
    AE_STUB(); return 0;
}

void
gl_buffer_set_str(gl_buffer_t* buffer, gl_buffer_property_t property, const char* value)
{
    AE_STUB();
}

const char*
gl_buffer_get_str(gl_buffer_t* buffer, gl_buffer_property_t property)
{
    AE_STUB(); return "";
}

gl_buffer_t* gl_buffer_create_empty(void)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_from(ae_array_t* vertices, ae_vertex_format_t vertex_format,
                                ae_array_t* indices, ae_index_type_t index_type, int copy)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_cylinder(int slices, int stacks)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_torus(int slices, int stacks, float radius)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_parametric_sphere(int slices, int stacks)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_subdivided_sphere(int nsubdivisions)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_klein_bottle(int slices, int stacks)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_trefoil_knot(int slices, int stacks, float radius)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_hemisphere(int slices, int stacks)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_plane(int slices, int stacks)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_parametric_shape(void (*func)(float const* uv,
            float* xyz, void* data), int slices, int stacks, void* userdata)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_icosahedron(void)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_dodecahedron(void)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_octahedron(void)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_tetrahedron(void)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_cube(void)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_disk( float radius, int slices,
                    float const* center, float const* normal)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_rock(int seed, int nsubdivisions)
{
    AE_STUB(); return NULL;
}

gl_buffer_t* gl_buffer_create_lsystem(char const* program, int slices, int maxdepth)
{
    AE_STUB(); return NULL;
}

void gl_buffer_lock(gl_buffer_t* buffer, ae_array_t* vertices, ae_array_t* indices)
{
    AE_STUB();
}

void gl_buffer_unlock(gl_buffer_t* buffer, ae_array_t* vertices, ae_vertex_format_t \
                    vertex_format, ae_array_t* indices, ae_index_type_t index_type)
{
    AE_STUB();
}

void gl_buffer_get_vertex(gl_buffer_t* buffer, size_t which,
                    float* t, float* c, float* n, float* p)
{
    AE_STUB();
}

void gl_buffer_set_vertex(gl_buffer_t* buffer, size_t which,
                    float* t, float* c, float* n, float* p)
{
    AE_STUB();
}

void gl_buffer_append_vertex(gl_buffer_t* buffer, float* t, float* c, float* n, float* p)
{
    AE_STUB();
}

void gl_buffer_get_face(gl_buffer_t * buffer, size_t which,
                        size_t *v0, size_t *v1, size_t *v2)
{
    AE_STUB();
}

void gl_buffer_set_face(gl_buffer_t * buffer, size_t which,
                        size_t v0, size_t v1, size_t v2)
{
    AE_STUB();
}

void gl_buffer_append_face(gl_buffer_t* buffer, size_t v0, size_t v1, size_t v2)
{
    AE_STUB();
}

void gl_buffer_merge(gl_buffer_t* dst, gl_buffer_t* src, int copy)
{
    AE_STUB();
}

void gl_buffer_get_aabbox(gl_buffer_t* buffer, float* minv, float* maxv)
{
    AE_STUB();
}

void gl_buffer_translate(gl_buffer_t* buffer, const float v[3])
{
    AE_STUB();
}

void gl_buffer_scale(gl_buffer_t* buffer, const float v[3])
{
    AE_STUB();
}

void gl_buffer_lerp(gl_buffer_t* buffer, gl_buffer_t* a, gl_buffer_t* b, float t)
{
    AE_STUB();
}

void gl_buffer_transform(gl_buffer_t* buffer, const float m[16], int normalize)
{
    AE_STUB();
}

void gl_buffer_center_at_origin(gl_buffer_t* buffer)
{
    AE_STUB();
}

float gl_buffer_unique_vertex_ratio(gl_buffer_t* buffer, float epsilon)
{
    AE_STUB(); return 0.0f;
}

void gl_buffer_weld(gl_buffer_t* buffer, float epsilon)
{
    AE_STUB();
}

void gl_buffer_weld_dist2(gl_buffer_t* buffer, float threshold2)
{
    AE_STUB();
}

void gl_buffer_unweld(gl_buffer_t* buffer)
{
    AE_STUB();
}

void gl_buffer_shrink_indices(gl_buffer_t* buffer)
{
    AE_STUB();
}

void gl_buffer_vertex_cache_optimize(gl_buffer_t* buffer)
{
    AE_STUB();
}

double gl_buffer_vertex_cache_miss_ratio(gl_buffer_t* buffer, int cache_size, int FIFO)
{
    AE_STUB(); return 0.0;
}

int gl_buffer_intersect_ray3(gl_buffer_t * buffer,

                            const float ray_pos[3],
                            const float ray_dir[3],

                            u32 hit_indices[3],
                            float * hit_point,
                            float * t,
                            float * u,
                            float * v,

                            const float epsilon)
{
    AE_STUB(); return 0;
}

void gl_buffer_depthsort(gl_buffer_t* buffer, const float p[3])
{
    AE_STUB();
}

void gl_buffer_invert_faces(gl_buffer_t* buffer)
{
    AE_STUB();
}

void gl_buffer_draw_ex(gl_buffer_t* buffer, gl_material_t* materials)
{
    AE_STUB();
}

void gl_buffer_draw(gl_buffer_t* buffer)
{
    AE_STUB();
}

void gl_buffer_draw_aabbox_ex(gl_buffer_t* buffer, const float* rgba,
                                                    float line_width)
{
    AE_STUB();
}

void gl_buffer_draw_aabbox(gl_buffer_t* buffer, const float* rgba)
{
    AE_STUB();
}

void gl_buffer_draw_normals_ex(gl_buffer_t* buffer, const float* start_color,
                                    const float* end_color, float line_width)
{
    AE_STUB();
}

void gl_buffer_draw_normals(gl_buffer_t* buffer, const float* rgba)
{
    AE_STUB();
}

void gl_buffer_list_all(gl_buffer_t** buffers)
{
    AE_STUB();
}

void gl_buffer_print_all(void)
{
    AE_STUB();
}

void gl_buffer_close_all(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ particle system ] ~~ *
--------------------------------------------------------------------------------
*/

void
gl_particle_emitter_set_int(gl_particle_emitter_t* emitter, // integer
                            gl_particle_emitter_property_t property,
                            int value)
{
    AE_STUB();
}

int
gl_particle_emitter_get_int(gl_particle_emitter_t* emitter, // integer
                            gl_particle_emitter_property_t property)
{
    AE_STUB(); return 0;
}

void
gl_particle_emitter_set_str(gl_particle_emitter_t* emitter, // string
                            gl_particle_emitter_property_t property,
                            const char* value)
{
    AE_STUB();
}

const char*
gl_particle_emitter_get_str(gl_particle_emitter_t* emitter, // string
                            gl_particle_emitter_property_t property)
{
    AE_STUB(); return "";
}

void
gl_particle_emitter_set_vec(gl_particle_emitter_t* emitter, // vector
                            gl_particle_emitter_property_t property,
                            float* value)
{
    AE_STUB();
}

float*
gl_particle_emitter_get_vec(gl_particle_emitter_t* emitter, // vector
                            gl_particle_emitter_property_t property)
{
    AE_STUB(); return NULL;
}

void
gl_particle_emitter_set_flt(gl_particle_emitter_t* emitter, // float
                            gl_particle_emitter_property_t property,
                            float value)
{
    AE_STUB();
}

float
gl_particle_emitter_get_flt(gl_particle_emitter_t* emitter, // float
                            gl_particle_emitter_property_t property)
{
    AE_STUB(); return 0.0f;
}

void
gl_particle_emitter_set_dbl(gl_particle_emitter_t* emitter, // double
                            gl_particle_emitter_property_t property,
                            double value)
{
    AE_STUB();
}

double
gl_particle_emitter_get_dbl(gl_particle_emitter_t* emitter, // double
                            gl_particle_emitter_property_t property)
{
    AE_STUB(); return 0.0;
}

void
gl_particle_emitter_set_tex(gl_particle_emitter_t* emitter, // texture
                            gl_particle_emitter_property_t property,
                            gl_texture_t* value)
{
    AE_STUB();
}

gl_texture_t*
gl_particle_emitter_get_tex(gl_particle_emitter_t* emitter, // texture
                            gl_particle_emitter_property_t property)
{
    AE_STUB(); return NULL;
}

gl_particle_emitter_t* gl_particle_emitter_create(void)
{
    AE_STUB(); return NULL;
}

void gl_particle_emitter_lock(gl_particle_emitter_t* emitter,
                ae_array_t* vel_array, ae_array_t* particles)
{
    AE_STUB();
}

void gl_particle_emitter_unlock(gl_particle_emitter_t* emitter,
                ae_array_t* vel_array, ae_array_t* particles)
{
    AE_STUB();
}

void gl_particle_emitter_get(gl_particle_emitter_t* emitter,
                    size_t which, float * vel, float * pos)
{
    AE_STUB();
}

void gl_particle_emitter_set(gl_particle_emitter_t* emitter,
                    size_t which, float * vel, float * pos)
{
    AE_STUB();
}

void gl_particle_emitter_append( gl_particle_emitter_t* emitter,
                            float* vel, float* pos, int expand)
{
    AE_STUB();
}

void gl_particle_emitter_merge( gl_particle_emitter_t * emitter,
                        gl_particle_emitter_t* other, int copy)
{
    AE_STUB();
}

void gl_particle_emitter_get_aabbox(gl_particle_emitter_t* emitter,
                                    float* min_val, float* max_val)
{
    AE_STUB();
}

void gl_particle_emitter_translate(gl_particle_emitter_t* emitter, float* xyz)
{
    AE_STUB();
}

void gl_particle_emitter_scale(gl_particle_emitter_t* emitter, float* xyz)
{
    AE_STUB();
}

void gl_particle_emitter_lerp(gl_particle_emitter_t* dst_emitter,
    gl_particle_emitter_t* a, gl_particle_emitter_t* b, float t)
{
    AE_STUB();
}

void gl_particle_emitter_transform(gl_particle_emitter_t* emitter, float* matrix)
{
    AE_STUB();
}

void gl_particle_emitter_center_at_origin(gl_particle_emitter_t* emitter)
{
    AE_STUB();
}

gl_particle_emitter_velgen_mode_t
gl_particle_emitter_velgen_mode_from_short_name(const char* name)
{
    AE_STUB(); return GL_PARTICLE_EMITTER_VELGEN_MODE_NONE;
}

void gl_particle_emitter_aabbox_limit(gl_particle_emitter_t* emitter,
                                            float* minv, float* maxv)
{
    AE_STUB();
}

void gl_particle_emitter_aabbox_clamp(gl_particle_emitter_t* emitter,
                                            float* minv, float* maxv)
{
    AE_STUB();
}

void gl_particle_emitter_aabbox_reset(gl_particle_emitter_t* emitter,
                                            float* minv, float* maxv)
{
    AE_STUB();
}

void gl_particle_emitter_sphere_limit(gl_particle_emitter_t* emitter,
                                        float* center, float radius)
{
    AE_STUB();
}

void gl_particle_emitter_sphere_clamp(gl_particle_emitter_t* emitter,
                                        float* center, float radius)
{
    AE_STUB();
}

void gl_particle_emitter_sphere_reset(gl_particle_emitter_t* emitter,
                                        float* center, float radius)
{
    AE_STUB();
}

void gl_particle_emitter_radius_limit(gl_particle_emitter_t* emitter, float radius)
{
    AE_STUB();
}

void gl_particle_emitter_radius_clamp(gl_particle_emitter_t* emitter, float radius)
{
    AE_STUB();
}

void gl_particle_emitter_radius_reset(gl_particle_emitter_t* emitter, float radius)
{
    AE_STUB();
}

void gl_particle_emitter_line_reset(gl_particle_emitter_t* emitter,
                                    float* min_val, float* max_val)
{
    AE_STUB();
}

void gl_particle_emitter_mesh_cover_face(gl_particle_emitter_t* emitter,
                    gl_buffer_t* buffer, size_t particles_per_triangle)
{
    AE_STUB();
}

void gl_particle_emitter_mesh_same_edges(gl_particle_emitter_t* emitter,
                    gl_buffer_t* buffer, size_t particles_per_triangle)
{
    AE_STUB();
}

void gl_particle_emitter_mesh_rand_edges(gl_particle_emitter_t* emitter,
                    gl_buffer_t* buffer, size_t particles_per_triangle)
{
    AE_STUB();
}

void gl_particle_emitter_mesh_given_edge(gl_particle_emitter_t * emitter,
        gl_buffer_t* buffer, size_t particles_per_triangle, size_t index)
{
    AE_STUB();
}

void gl_particle_emitter_mesh_rad_vertex(gl_particle_emitter_t * emitter,
        gl_buffer_t* buffer, size_t particles_per_triangle, float radius)
{
    AE_STUB();
}

void gl_particle_emitter_reset_all(void)
{
    AE_STUB();
}

void gl_particle_emitter_reset(gl_particle_emitter_t* emitter)
{
    AE_STUB();
}

void gl_particle_emitter_fill_all(void)
{
    AE_STUB();
}

void gl_particle_emitter_fill(gl_particle_emitter_t* emitter)
{
    AE_STUB();
}

void gl_particle_emitter_update_all(double dt)
{
    AE_STUB();
}

void gl_particle_emitter_update(gl_particle_emitter_t* emitter, double dt)
{
    AE_STUB();
}

void gl_particle_emitter_draw_all(void)
{
    AE_STUB();
}

void gl_particle_emitter_draw(gl_particle_emitter_t* emitter)
{
    AE_STUB();
}

void gl_particle_emitter_draw_aabbox_ex(gl_particle_emitter_t * emitter,
                                    const float* rgba, float line_width)
{
    AE_STUB();
}

void gl_particle_emitter_draw_aabbox(gl_particle_emitter_t* emitter,
                                                const float * rgba)
{
    AE_STUB();
}

void gl_particle_emitter_draw_velocity_ex( gl_particle_emitter_t * emitter,
        const float* start_color, const float* end_color, float line_width)
{
    AE_STUB();
}

void gl_particle_emitter_draw_velocity(gl_particle_emitter_t* emitter,
                                                    const float* rgba)
{
    AE_STUB();
}

void gl_particle_emitter_list_all(gl_particle_emitter_t** emitters)
{
    AE_STUB();
}

void gl_particle_emitter_print_all(void)
{
    AE_STUB();
}

void gl_particle_emitter_close_all(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ init & quit ] ~~ *
--------------------------------------------------------------------------------
*/

void gl_objects_init(void)
{
    // AE_STUB();
}

void gl_objects_quit(void)
{
    // AE_STUB();
}
