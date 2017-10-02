# ------------------------------------------------------------------------------
# Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
# Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
# ------------------------------------------------------------------------------
from aegame.vec cimport *
from aegame.mat cimport *
from aegame.lin cimport *
from aegame.mem cimport *
from aegame.vtx cimport *
from aegame.idx cimport *
from aegame.tri cimport *
from aegame.img cimport *

cdef extern from "gl_core.h":
    # ==========================================================================
    # ~ [ macros & init ]
    # ==========================================================================

    int gl_is_init() # safe hacking
    const char* gl_implementation()

    void gl_init()
    void gl_quit()

    float GL_DEFAULT_EPSILON

    # ==========================================================================
    # ~ [ OpenGL wrapper ]
    # ==========================================================================

    void GL_AlphaFunc(unsigned int func, float ref)
    void GL_Begin(unsigned int mode)
    void GL_BindTexture(unsigned int target, unsigned int texture)
    void GL_BlendFunc(unsigned int src, unsigned int dst)

    void GL_BlendFuncSeparate(unsigned int srcRGB, unsigned int dstRGB,
                                unsigned int srcA, unsigned int dstA)

    void GL_Clear(unsigned int mask)
    void GL_ClearColor(float r, float g, float b, float a)
    void GL_Color3f(float r, float g, float b)
    void GL_Color4f(float r, float g, float b, float a)
    void GL_Color3fv(const float* rgb)
    void GL_Color4fv(const float* rgba)
    void GL_ColorMask(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    void GL_ColorMaterial(unsigned int face, unsigned int mode)
    void GL_ColorPointer(int size, unsigned int type, int stride, const void* data)
    void GL_CullFace(unsigned int face)
    void GL_DeleteTextures(int n, const unsigned int* textures)
    void GL_DepthFunc(unsigned int func)
    void GL_DepthMask(unsigned char flag)
    void GL_DepthRange(double near_val, double far_val)
    void GL_Disable(unsigned int cap)
    void GL_DisableClientState(unsigned int cap)
    void GL_DrawArrays(unsigned int mode, int first, int count)
    void GL_DrawElements(unsigned int mode, int n, unsigned int type, const void* inds)
    void GL_Enable(unsigned int cap)
    void GL_EnableClientState(unsigned int cap)
    void GL_End()
    void GL_Finish()
    void GL_Flush()
    void GL_Fogfv(unsigned int pname, const float* params)
    void GL_Fogiv(unsigned int pname, const int* params)
    void GL_Fogf(unsigned int pname, float param)
    void GL_Fogi(unsigned int pname, int param)
    void GL_FrontFace(unsigned int mode)
    void GL_Frustum(double lf, double rt, double bt, double tp, double nr, double fr)
    void GL_GenTextures(int n, unsigned int* textures)
    unsigned int GL_GetError()
    void GL_GetFloatv(unsigned int pname, float* data)
    void GL_GetIntegerv(unsigned int pname, int* data)
    void GL_GetLightfv(unsigned int light, unsigned int pname, float* params)
    void GL_GetMaterialfv(unsigned int face, unsigned int pname, float* params)
    const char* GL_GetString(unsigned int name)
    void GL_GetTexParameteriv(unsigned int target, unsigned int pname, int* params)
    void GL_InterleavedArrays(unsigned int format, int stride, const void* pointer)
    unsigned char GL_IsEnabled(unsigned int cap)
    unsigned char GL_IsTexture(unsigned int texture)
    void GL_Lightfv(unsigned int light, unsigned int pname, const float* params)
    void GL_Lightf(unsigned int light, unsigned int pname, float param)
    void GL_LineWidth(float width)
    void GL_LoadIdentity()
    void GL_LogicOp(unsigned int opcode)
    void GL_Materialfv(unsigned int face, unsigned int pname, const float* params)
    void GL_Materialf(unsigned int face, unsigned int pname, float param)
    void GL_MatrixMode(unsigned int mode)
    void GL_MultMatrixf(const float* m)
    void GL_Normal3f(float x, float y, float z)
    void GL_Normal3fv(const float* normal)
    void GL_NormalPointer(unsigned int type, int stride, const void* data)
    void GL_Ortho(double lf, double rt, double btm, double tp, double nr, double fr)
    void GL_PointParameterfv(unsigned int pname, const float* data)
    void GL_PointParameterf(unsigned int pname, float param)
    void GL_PointSize(float size)
    void GL_PolygonMode(unsigned int face, unsigned int mode)
    void GL_PolygonOffset(float factor, float units)
    void GL_PopAttrib()
    void GL_PushAttrib(unsigned int mask)
    void GL_PopClientAttrib()
    void GL_PushClientAttrib(unsigned int mask)
    void GL_PopMatrix()
    void GL_PushMatrix()
    void GL_Rotatef(float theta, float x, float y, float z)
    void GL_Scalef(float x, float y, float z)
    void GL_Scissor(int x, int y, int width, int height)
    void GL_ShadeModel(unsigned int mode)
    void GL_TexCoord2f(float s, float t)
    void GL_TexCoord3f(float s, float t, float r)
    void GL_TexCoord4f(float s, float t, float r, float q)
    void GL_TexCoord2fv(const float* st)
    void GL_TexCoord3fv(const float* str)
    void GL_TexCoord4fv(const float* strq)
    void GL_TexCoordPointer(int size, unsigned int type, int stride, const void* data)
    void GL_TexEnvf(unsigned int target, unsigned int pname, float param)
    void GL_TexEnvi(unsigned int target, unsigned int pname, int param)

    void GL_TexImage2D(unsigned int target, int level, int internal_format, int w,
            int h, int border, unsigned int fmt, unsigned int type, const void* p)

    void GL_TexParameteri(unsigned int target, unsigned int pname, int param)

    void GL_TexSubImage2D(unsigned int target, int level, int xoffset, int yoffset,
            int w, int h, unsigned int format, unsigned int type, const void* pix)

    void GL_Translatef(float x, float y, float z)
    void GL_Vertex2f(float x, float y)
    void GL_Vertex3f(float x, float y, float z)
    void GL_Vertex2fv(const float* xy)
    void GL_Vertex3fv(const float* xyz)
    void GL_VertexPointer(int size, unsigned int type, int stride, const void* data)
    void GL_Viewport(int x, int y, int width, int height)

    # ==========================================================================
    # ~ [ utilities ]
    # ==========================================================================

    void gl_enter3D(int width, int height, double FOV, double znear, double zfar)
    void gl_leave3D()

    void gl_enter2D(int width, int height)
    void gl_leave2D()

    void gl_rect_ex(float*, float*, float*, float*, float*, int, float)

    void gl_outline(float* rect, float* rgba)
    void gl_rect(float* rect, float* rgba)

    void gl_aabbox_ex(float* minv, float* maxv, float* rgba, float line_width)
    void gl_aabbox(float* minv, float* maxv, float* rgba)

    void gl_ellipsoid(float* center, float* extent, float* rgba)
    void gl_sphere(float* center, float radius, float* rgba)

    # ==========================================================================
    # ~ [ Gl state logging ]
    # ==========================================================================

    void gl_log_error_state_ex(const char* filename, const char* funcname)
    void gl_log_error_state()
    void gl_debug_log_error_state()

    void gl_log_texture_state_ex(const char* filename, const char* funcname)
    void gl_log_texture_state()
    void gl_debug_log_texture_state()

    void gl_log_blend_state_ex(const char* filename, const char* funcname)
    void gl_log_blend_state()
    void gl_debug_log_blend_state()

    void gl_log_material_state_ex(const char* filename, const char* funcname)
    void gl_log_material_state()
    void gl_debug_log_material_state()

    void gl_log_light_state_ex(const char* filename, const char* funcname)
    void gl_log_light_state()
    void gl_debug_log_light_state()

    void gl_log_internal_state_ex(const char* filename, const char* funcname)
    void gl_log_internal_state()
    void gl_debug_log_internal_state()

    # ==========================================================================
    # ~ [ coordinate system ]
    # ==========================================================================

    ctypedef struct gl_coord3D_t:
        #
        # NOTE cython doesn't really understand unions, so we have
        # to fake it by making this look like a regular old struct.
        #
        float position[3]
        float pos[3]
        float translation[3]
        float xyz[3]
        float x, y, z

        float rotation[3]
        float rot[3]
        float pitch, heading, roll

        float scale[3]
        float scl[3]
        float scale_x, scale_y, scale_z

    void gl_coord3D_init(gl_coord3D_t* coord) # set up defaults
    void gl_coord3D_copy(gl_coord3D_t* dst, gl_coord3D_t* src)

    void gl_coord3D_normalize(gl_coord3D_t* coord,
            const float min_v, const float max_v)

    void gl_coord3D_normalize180(gl_coord3D_t* coord)
    void gl_coord3D_normalize360(gl_coord3D_t* coord)

    # ===== [ modelview transforms ] ===========================================

    void gl_coord3D_apply_camera_transform_ex(gl_coord3D_t* coord, int absolute)
    void gl_coord3D_push_camera_transform_ex(gl_coord3D_t* coord, int absolute)

    void gl_coord3D_apply_camera_transform(gl_coord3D_t* coord)
    void gl_coord3D_push_camera_transform(gl_coord3D_t* coord)

    void gl_coord3D_pop_camera_transform()

    void gl_coord3D_apply_object_transform_ex(gl_coord3D_t* coord, int absolute)
    void gl_coord3D_push_object_transform_ex(gl_coord3D_t* coord, int absolute)

    void gl_coord3D_apply_object_transform(gl_coord3D_t* coord)
    void gl_coord3D_push_object_transform(gl_coord3D_t* coord)

    void gl_coord3D_pop_object_transform()

    # ===== [ conversion ] =====================================================

    void gl_coord3D_heading_towards_point(gl_coord3D_t* coord, float* p)
    void gl_coord3D_pitch_towards_point(gl_coord3D_t* coord, float* p)
    void gl_coord3D_face_towards_point(gl_coord3D_t* coord, float* p)

    void gl_coord3D_get_matrix(gl_coord3D_t* coord, float* m)
    void gl_coord3D_set_matrix(gl_coord3D_t* coord, float* m)

    void gl_coord3D_get_quat(gl_coord3D_t* coord, float* q)
    void gl_coord3D_set_quat(gl_coord3D_t* coord, float* q)

    void gl_coord3D_get_normal(gl_coord3D_t* coord, float* v)
    void gl_coord3D_set_normal(gl_coord3D_t* coord, float* v)

    void gl_coord3D_get_ray(gl_coord3D_t* coord, float* p, float* v)
    void gl_coord3D_set_ray(gl_coord3D_t* coord, float* p, float* v)

    void gl_coord3D_get_up_vector(gl_coord3D_t* coord, float* v)
    void gl_coord3D_set_up_vector(gl_coord3D_t* coord, float* v)

    void gl_coord3D_get_down_vector(gl_coord3D_t* coord, float* v)
    void gl_coord3D_set_down_vector(gl_coord3D_t* coord, float* v)

    void gl_coord3D_get_left_vector(gl_coord3D_t* coord, float* v)
    void gl_coord3D_set_left_vector(gl_coord3D_t* coord, float* v)

    void gl_coord3D_get_right_vector(gl_coord3D_t* coord, float* v)
    void gl_coord3D_set_right_vector(gl_coord3D_t* coord, float* v)

    # ==========================================================================
    # ~ [ object types ]
    # ==========================================================================

    ctypedef void gl_texture_t
    ctypedef void gl_material_t
    ctypedef void gl_light_t
    ctypedef void gl_buffer_t
    ctypedef void gl_particle_emitter_t

    ctypedef enum gl_object_type_t:
        GL_OBJECT_TYPE_UNKNOWN
        GL_OBJECT_TYPE_TEXTURE
        GL_OBJECT_TYPE_MATERIAL
        GL_OBJECT_TYPE_LIGHT
        GL_OBJECT_TYPE_BUFFER
        GL_OBJECT_TYPE_PARTICLE_EMITTER
        GL_OBJECT_TYPE_COUNT

    gl_object_type_t gl_object_type(void *)

    const char* gl_object_type_name[]
    const char* gl_object_type_short_name[]

    size_t gl_object_count_all()

    void gl_object_list_all(void** objects)
    void gl_object_close_all()

    # ==========================================================================
    # ~ [ texture renderer ]
    # ==========================================================================

    ctypedef enum gl_texture_property_t:
        GL_TEXTURE_PROPERTY_TOTAL
        GL_TEXTURE_PROPERTY_ID
        GL_TEXTURE_PROPERTY_TARGET
        GL_TEXTURE_PROPERTY_WIDTH
        GL_TEXTURE_PROPERTY_HEIGHT
        GL_TEXTURE_PROPERTY_COMP
        GL_TEXTURE_PROPERTY_IMAGE
        GL_TEXTURE_PROPERTY_DEFAULT_COPY
        GL_TEXTURE_PROPERTY_COPY_ENABLED
        GL_TEXTURE_PROPERTY_STATUS
        GL_TEXTURE_PROPERTY_PATH
        GL_TEXTURE_PROPERTY_NAME
        GL_TEXTURE_PROPERTY_OPEN
        GL_TEXTURE_PROPERTY_COUNT

    const char* gl_texture_property_name[]
    const char* gl_texture_property_type[]

    void gl_texture_set_int(gl_texture_t*, gl_texture_property_t, int)
    int  gl_texture_get_int(gl_texture_t*, gl_texture_property_t)

    void gl_texture_set_str(gl_texture_t*, gl_texture_property_t, const char*)
    const char* gl_texture_get_str(gl_texture_t*, gl_texture_property_t)

    void gl_texture_set_img(gl_texture_t*, gl_texture_property_t, ae_image_t*)
    ae_image_t* gl_texture_get_img(gl_texture_t*, gl_texture_property_t)

    void gl_texture_close(gl_texture_t* texture)

    # create a new blank texture with the given dimensions and color depth
    gl_texture_t* gl_texture_create(int width, int height, int components)

    # load texture from compressed memory block (archive entry, for example)
    gl_texture_t* gl_texture_load_from_memory(void* buf, size_t size)

    gl_texture_t* \
    gl_texture_load_from_memory_ex(void*, size_t, ae_image_error_t*)

    gl_texture_t* gl_texture_load(const char* filename) # load texture from file
    gl_texture_t* gl_texture_load_ex(const char* filename, ae_image_error_t*)

    void gl_texture_bind(gl_texture_t* texture)

    # shameful false implication of software blitting, for pyglet compatibility
    void gl_texture_blit(gl_texture_t* texture, float x, float y)
    void gl_texture_blit_ex(gl_texture_t* tex, float x, float y, float rgba[4])

    void gl_texture_draw_skybox(gl_texture_t* front,
                                gl_texture_t* back,
                                gl_texture_t* left,
                                gl_texture_t* right,
                                gl_texture_t* up,
                                gl_texture_t* down,
                                gl_coord3D_t* viewer)

    size_t gl_texture_count_all()

    void gl_texture_list_all(gl_texture_t** textures)
    void gl_texture_close_all()

    # ==========================================================================
    # ~ [ material system ]
    # ==========================================================================

    ctypedef enum gl_material_property_t:
        GL_MATERIAL_PROPERTY_TOTAL
        GL_MATERIAL_PROPERTY_ID
        GL_MATERIAL_PROPERTY_FLAGS
        GL_MATERIAL_PROPERTY_TEXTURE
        GL_MATERIAL_PROPERTY_OWNS_TEXTURE
        GL_MATERIAL_PROPERTY_DIFFUSE
        GL_MATERIAL_PROPERTY_AMBIENT
        GL_MATERIAL_PROPERTY_SPECULAR
        GL_MATERIAL_PROPERTY_EMISSION
        GL_MATERIAL_PROPERTY_LIGHT_ENABLED
        GL_MATERIAL_PROPERTY_SHININESS
        GL_MATERIAL_PROPERTY_OPACITY
        GL_MATERIAL_PROPERTY_STATUS
        GL_MATERIAL_PROPERTY_PATH
        GL_MATERIAL_PROPERTY_NAME
        GL_MATERIAL_PROPERTY_OPEN
        GL_MATERIAL_PROPERTY_COUNT

    const char* gl_material_property_name[]
    const char* gl_material_property_type[]

    void gl_material_set_int(gl_material_t*, gl_material_property_t, int)
    int  gl_material_get_int(gl_material_t*, gl_material_property_t)

    void gl_material_set_u64(gl_material_t*, gl_material_property_t, u64)
    u64  gl_material_get_u64(gl_material_t*, gl_material_property_t)

    void  gl_material_set_flt(gl_material_t*, gl_material_property_t, float)
    float gl_material_get_flt(gl_material_t*, gl_material_property_t)

    void gl_material_set_vec(gl_material_t*, gl_material_property_t, float*)
    float* gl_material_get_vec(gl_material_t*, gl_material_property_t)

    void gl_material_set_str(gl_material_t*, gl_material_property_t, const char*)
    const char* gl_material_get_str(gl_material_t*, gl_material_property_t)

    void gl_material_set_tex(gl_material_t*, gl_material_property_t, gl_texture_t*)
    gl_texture_t* gl_material_get_tex(gl_material_t*, gl_material_property_t)

    void gl_material_close(gl_material_t* material)

    gl_material_t* gl_material_create()

    # set all material properties (ambient, diffuse, etc.) to the same value
    void gl_material_set_all(gl_material_t* material, float* value)

    # interpolate A and B properties into a destination material (for animation)
    void gl_material_lerp(gl_material_t*, gl_material_t*, gl_material_t*, float)

    void gl_material_apply_ex(gl_material_t* material, gl_buffer_t* buffer)
    void gl_material_apply(gl_material_t* material)

    size_t gl_material_count_all()

    void gl_material_list_all(gl_material_t** materials)
    void gl_material_close_all()

    # ==========================================================================
    # ~ [ lighting system ]
    # ==========================================================================

    ctypedef enum gl_light_property_t:
        GL_LIGHT_PROPERTY_TOTAL
        GL_LIGHT_PROPERTY_ID
        GL_LIGHT_PROPERTY_AMBIENT
        GL_LIGHT_PROPERTY_DIFFUSE
        GL_LIGHT_PROPERTY_SPECULAR
        GL_LIGHT_PROPERTY_POSITION
        GL_LIGHT_PROPERTY_SPOT_DIRECTION
        GL_LIGHT_PROPERTY_SPOT_EXPONENT
        GL_LIGHT_PROPERTY_SPOT_CUTOFF
        GL_LIGHT_PROPERTY_CONSTANT_ATTENUATION
        GL_LIGHT_PROPERTY_LINEAR_ATTENUATION
        GL_LIGHT_PROPERTY_QUADRATIC_ATTENUATION
        GL_LIGHT_PROPERTY_STATUS
        GL_LIGHT_PROPERTY_NAME
        GL_LIGHT_PROPERTY_OPEN
        GL_LIGHT_PROPERTY_COUNT

    const char* gl_light_property_name[]
    const char* gl_light_property_type[]

    void gl_light_set_int(gl_light_t*, gl_light_property_t, int)
    int  gl_light_get_int(gl_light_t*, gl_light_property_t)

    void gl_light_set_str(gl_light_t*, gl_light_property_t, const char*)
    const char* gl_light_get_str(gl_light_t*, gl_light_property_t)

    void gl_light_set_vec(gl_light_t*, gl_light_property_t, float*)
    float* gl_light_get_vec(gl_light_t*, gl_light_property_t)

    void  gl_light_set_flt(gl_light_t*, gl_light_property_t, float)
    float gl_light_get_flt(gl_light_t*, gl_light_property_t)

    void gl_light_close(gl_light_t* light)

    # add new default light to the global scene
    gl_light_t* gl_light_create()

    # set ambient and diffuse properties; set the light to a solid color profile
    void gl_light_set_all(gl_light_t* light, float* value)

    # interpolate the properties of lights A and B into the destination light
    void gl_light_lerp(gl_light_t* light, gl_light_t* a, gl_light_t* b, float t)

    # create a standard ambient lighting setup
    void gl_light_default_scene()

    # hardware light state for the current scene
    void gl_light_apply_all()

    size_t gl_light_count_all()

    void gl_light_list_all(gl_light_t** lights)
    void gl_light_close_all()

    # ==========================================================================
    # ~ [ vertex buffers ]
    # ==========================================================================

    ctypedef enum gl_buffer_property_t:
        GL_BUFFER_PROPERTY_TOTAL
        GL_BUFFER_PROPERTY_ID
        GL_BUFFER_PROPERTY_VERTEX_FORMAT
        GL_BUFFER_PROPERTY_INDEX_TYPE
        GL_BUFFER_PROPERTY_VERTEX_COUNT
        GL_BUFFER_PROPERTY_INDEX_COUNT
        GL_BUFFER_PROPERTY_TRIANGLE_COUNT
        GL_BUFFER_PROPERTY_STATIC_STORAGE
        GL_BUFFER_PROPERTY_COMPUTE_AABBOX
        GL_BUFFER_PROPERTY_LOCKED
        GL_BUFFER_PROPERTY_SMOOTH_SHADING
        GL_BUFFER_PROPERTY_LINE_MODE
        GL_BUFFER_PROPERTY_STATUS
        GL_BUFFER_PROPERTY_PATH
        GL_BUFFER_PROPERTY_NAME
        GL_BUFFER_PROPERTY_OPEN
        GL_BUFFER_PROPERTY_COUNT

    const char* gl_buffer_property_name[]
    const char* gl_buffer_property_type[]

    void gl_buffer_set_int(gl_buffer_t*, gl_buffer_property_t, int)
    int  gl_buffer_get_int(gl_buffer_t*, gl_buffer_property_t)

    void gl_buffer_set_str(gl_buffer_t*, gl_buffer_property_t, const char*)
    const char* gl_buffer_get_str(gl_buffer_t*, gl_buffer_property_t)

    void gl_buffer_close(gl_buffer_t* buffer)
    gl_buffer_t* gl_buffer_create_empty()

    gl_buffer_t* \
    gl_buffer_create_from( ae_array_t* vertices, ae_vertex_format_t vertex_format,
                        ae_array_t* indices, ae_index_type_t index_type, int copy)

    gl_buffer_t* gl_buffer_create_cylinder(int slices, int stacks)
    gl_buffer_t* gl_buffer_create_torus(int slices, int stacks, float radius)
    gl_buffer_t* gl_buffer_create_parametric_sphere(int slices, int stacks)
    gl_buffer_t* gl_buffer_create_subdivided_sphere(int nsubdivisions)
    gl_buffer_t* gl_buffer_create_klein_bottle(int slices, int stacks)
    gl_buffer_t* gl_buffer_create_trefoil_knot(int slices, int stacks, float radius)
    gl_buffer_t* gl_buffer_create_hemisphere(int slices, int stacks)
    gl_buffer_t* gl_buffer_create_plane(int slices, int stacks)

    # FIXME: forgot the cython function pointer syntax, we don't wrap this anyways
    #
    # gl_buffer_t* gl_buffer_create_parametric_shape(void (*func)(float const* uv,
    #             float* xyz, void* data), int slices, int stacks, void* userdata)

    gl_buffer_t* gl_buffer_create_icosahedron()
    gl_buffer_t* gl_buffer_create_dodecahedron()
    gl_buffer_t* gl_buffer_create_octahedron()
    gl_buffer_t* gl_buffer_create_tetrahedron()
    gl_buffer_t* gl_buffer_create_cube()

    gl_buffer_t* gl_buffer_create_disk( float radius, int slices,
                        const float* center, const float* normal)

    gl_buffer_t* gl_buffer_create_rock(int seed, int nsubdivisions)

    gl_buffer_t* gl_buffer_create_lsystem(const char* program,
                                    int slices, int max_depth)

    # acquire buffer memory
    void gl_buffer_lock(gl_buffer_t* b, ae_array_t*, ae_array_t*)

    # release buffer memory
    void gl_buffer_unlock(gl_buffer_t*, ae_array_t*, ae_vertex_format_t,
                                        ae_array_t*, ae_index_type_t)

    void gl_buffer_get_vertex(gl_buffer_t* buffer, size_t which,
                        float* t, float* c, float* n, float* p)

    void gl_buffer_set_vertex(gl_buffer_t* buffer, size_t which,
                        float* t, float* c, float* n, float* p)

    void gl_buffer_get_face(gl_buffer_t * buffer, size_t which,
                            size_t *v0, size_t *v1, size_t *v2)

    void gl_buffer_set_face(gl_buffer_t * buffer, size_t which,
                            size_t iv0, size_t iv1, size_t iv2)

    void gl_buffer_append_vertex(gl_buffer_t*, float*, float*, float*, float*)
    void gl_buffer_append_face(gl_buffer_t*, size_t, size_t, size_t)
    void gl_buffer_merge(gl_buffer_t* dst, gl_buffer_t* src, int copy)

    void gl_buffer_get_aabbox(gl_buffer_t* buffer, float* minv, float* maxv)
    void gl_buffer_translate(gl_buffer_t* buffer, const float v[3])
    void gl_buffer_scale(gl_buffer_t* buffer, const float v[3])
    void gl_buffer_lerp(gl_buffer_t* buffer, gl_buffer_t* a, gl_buffer_t* b, float t)
    void gl_buffer_transform(gl_buffer_t* buffer, const float m[16], int normalize)
    void gl_buffer_center_at_origin(gl_buffer_t* buffer)

    float gl_buffer_unique_vertex_ratio(gl_buffer_t* buffer, float epsilon)
    void gl_buffer_weld(gl_buffer_t* buffer, float epsilon)
    void gl_buffer_weld_dist2(gl_buffer_t* buffer, float threshold2)
    void gl_buffer_unweld(gl_buffer_t* buffer)

    void gl_buffer_shrink_indices(gl_buffer_t* buffer) # optimize
    void gl_buffer_vertex_cache_optimize(gl_buffer_t* buffer)

    double gl_buffer_vertex_cache_miss_ratio(gl_buffer_t* buffer,
                                        int cache_size, int FIFO)

    int gl_buffer_intersect_ray3(gl_buffer_t * buffer,

                                const float ray_pos[3],
                                const float ray_dir[3],

                                u32 hit_indices[3],
                                float * hit_point,
                                float * t,
                                float * u,
                                float * v,

                                const float epsilon)

    void gl_buffer_depthsort(gl_buffer_t* buffer, const float p[3])
    void gl_buffer_invert_faces(gl_buffer_t* buffer)

    void gl_buffer_draw_ex(gl_buffer_t* buffer, gl_material_t* material)
    void gl_buffer_draw(gl_buffer_t* buffer)

    void gl_buffer_draw_normals_ex(gl_buffer_t* buffer, float* start_color,
                                        float* end_color, float line_width)

    void gl_buffer_draw_normals(gl_buffer_t* buffer, float* rgba)

    size_t gl_buffer_count_all()

    void gl_buffer_list_all(gl_buffer_t** buffers)
    void gl_buffer_close_all()

    # ==========================================================================
    # ~ [ particle system ]
    # ==========================================================================

    ctypedef enum gl_particle_emitter_property_t:
        GL_PARTICLE_EMITTER_PROPERTY_TOTAL
        GL_PARTICLE_EMITTER_PROPERTY_ID
        GL_PARTICLE_EMITTER_PROPERTY_TEXTURE
        GL_PARTICLE_EMITTER_PROPERTY_OWNS_TEXTURE
        GL_PARTICLE_EMITTER_PROPERTY_COLOR
        GL_PARTICLE_EMITTER_PROPERTY_MIN_POINT_SIZE
        GL_PARTICLE_EMITTER_PROPERTY_MAX_POINT_SIZE
        GL_PARTICLE_EMITTER_PROPERTY_POSITION
        GL_PARTICLE_EMITTER_PROPERTY_POSITION_SIGMA
        GL_PARTICLE_EMITTER_PROPERTY_VELOCITY
        GL_PARTICLE_EMITTER_PROPERTY_VELOCITY_SIGMA
        GL_PARTICLE_EMITTER_PROPERTY_GRAVITY
        GL_PARTICLE_EMITTER_PROPERTY_VELGEN_MODE
        GL_PARTICLE_EMITTER_PROPERTY_VELGEN_SCALE
        GL_PARTICLE_EMITTER_PROPERTY_FROZEN
        GL_PARTICLE_EMITTER_PROPERTY_BURST_SIZE
        GL_PARTICLE_EMITTER_PROPERTY_NUM_PARTICLES
        GL_PARTICLE_EMITTER_PROPERTY_MAX_PARTICLES
        GL_PARTICLE_EMITTER_PROPERTY_PERIOD
        GL_PARTICLE_EMITTER_PROPERTY_STATUS
        GL_PARTICLE_EMITTER_PROPERTY_NAME
        GL_PARTICLE_EMITTER_PROPERTY_OPEN
        GL_PARTICLE_EMITTER_PROPERTY_COUNT

    const char* gl_particle_emitter_property_name[]
    const char* gl_particle_emitter_property_type[]

    void \
    gl_particle_emitter_set_int(gl_particle_emitter_t* emitter, # int
                                gl_particle_emitter_property_t prop,
                                int value)

    int \
    gl_particle_emitter_get_int(gl_particle_emitter_t* emitter, # int
                                gl_particle_emitter_property_t prop)

    void \
    gl_particle_emitter_set_str(gl_particle_emitter_t* emitter, # str
                                gl_particle_emitter_property_t prop,
                                const char* value)

    const char* \
    gl_particle_emitter_get_str(gl_particle_emitter_t* emitter, # str
                                gl_particle_emitter_property_t prop)

    void \
    gl_particle_emitter_set_vec(gl_particle_emitter_t* emitter, # vec
                                gl_particle_emitter_property_t prop,
                                float* value)

    float* \
    gl_particle_emitter_get_vec(gl_particle_emitter_t* emitter, # vec
                                gl_particle_emitter_property_t prop)

    void \
    gl_particle_emitter_set_flt(gl_particle_emitter_t* emitter, # flt
                                gl_particle_emitter_property_t prop,
                                float value)

    float \
    gl_particle_emitter_get_flt(gl_particle_emitter_t* emitter, # flt
                                gl_particle_emitter_property_t prop)

    void \
    gl_particle_emitter_set_dbl(gl_particle_emitter_t* emitter, # dbl
                                gl_particle_emitter_property_t prop,
                                double value)

    double \
    gl_particle_emitter_get_dbl(gl_particle_emitter_t* emitter, # dbl
                                gl_particle_emitter_property_t prop)

    void \
    gl_particle_emitter_set_tex(gl_particle_emitter_t* emitter, # tex
                                gl_particle_emitter_property_t prop,
                                gl_texture_t* value)

    gl_texture_t* \
    gl_particle_emitter_get_tex(gl_particle_emitter_t* emitter, # tex
                                gl_particle_emitter_property_t prop)

    void gl_particle_emitter_close(gl_particle_emitter_t* emitter)
    gl_particle_emitter_t* gl_particle_emitter_create()

    void gl_particle_emitter_lock( gl_particle_emitter_t * emitter,
                    ae_array_t* vel_array, ae_array_t* particles)

    void gl_particle_emitter_unlock(gl_particle_emitter_t* emitter,
                    ae_array_t* vel_array, ae_array_t* particles)

    void gl_particle_emitter_get(gl_particle_emitter_t* emitter, size_t which,
                                                    float * vel, float * pos)

    void gl_particle_emitter_set(gl_particle_emitter_t* emitter, size_t which,
                                                    float * vel, float * pos)

    void gl_particle_emitter_append(gl_particle_emitter_t* emitter,
                                float* vel, float* pos, int expand)

    void gl_particle_emitter_merge( gl_particle_emitter_t* emitter,
                            gl_particle_emitter_t* other, int copy)

    void gl_particle_emitter_get_aabbox(gl_particle_emitter_t* emitter,
                                        float* min_val, float* max_val)

    void gl_particle_emitter_translate(gl_particle_emitter_t* emitter, float* v)
    void gl_particle_emitter_scale(gl_particle_emitter_t* emitter, float* v)

    void gl_particle_emitter_lerp( gl_particle_emitter_t * dst_emitter,
        gl_particle_emitter_t * a, gl_particle_emitter_t * b, float t)

    void gl_particle_emitter_transform(gl_particle_emitter_t* emitter, float* m)
    void gl_particle_emitter_center_at_origin(gl_particle_emitter_t* emitter)

    ctypedef enum gl_particle_emitter_velgen_mode_t:
        GL_PARTICLE_EMITTER_VELGEN_MODE_NONE
        GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL
        GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_RANDOM
        GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_BI_RANDOM
        GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_RANDOM
        GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_BI_RANDOM
        GL_PARTICLE_EMITTER_VELGEN_MODE_RANDOM
        GL_PARTICLE_EMITTER_VELGEN_MODE_ZERO
        GL_PARTICLE_EMITTER_VELGEN_MODE_COUNT

    const char* gl_particle_emitter_velgen_mode_name[] # name strings
    const char* gl_particle_emitter_velgen_mode_short_name[]

    gl_particle_emitter_velgen_mode_t \
    gl_particle_emitter_velgen_mode_from_short_name(const char* name)

    void gl_particle_emitter_aabbox_limit(gl_particle_emitter_t* emitter,
                                                float* minv, float* maxv)

    void gl_particle_emitter_aabbox_clamp(gl_particle_emitter_t* emitter,
                                                float* minv, float* maxv)

    void gl_particle_emitter_aabbox_reset(gl_particle_emitter_t* emitter,
                                                float* minv, float* maxv)

    void gl_particle_emitter_sphere_limit(gl_particle_emitter_t* emitter,
                                            float* center, float radius)

    void gl_particle_emitter_sphere_clamp(gl_particle_emitter_t* emitter,
                                            float* center, float radius)

    void gl_particle_emitter_sphere_reset(gl_particle_emitter_t* emitter,
                                            float* center, float radius)

    void gl_particle_emitter_radius_limit(gl_particle_emitter_t* emitter, float radius)
    void gl_particle_emitter_radius_clamp(gl_particle_emitter_t* emitter, float radius)
    void gl_particle_emitter_radius_reset(gl_particle_emitter_t* emitter, float radius)

    void gl_particle_emitter_line_reset(gl_particle_emitter_t* emitter,
                                        float* min_val, float* max_val)

    void gl_particle_emitter_reset_all()
    void gl_particle_emitter_reset(gl_particle_emitter_t* emitter)

    void gl_particle_emitter_fill_all()
    void gl_particle_emitter_fill(gl_particle_emitter_t* emitter)

    void gl_particle_emitter_update_all(double dt)
    void gl_particle_emitter_update(gl_particle_emitter_t* emitter, double dt)

    void gl_particle_emitter_draw_all()
    void gl_particle_emitter_draw(gl_particle_emitter_t* emitter)

    void gl_particle_emitter_draw_velocity_ex( gl_particle_emitter_t * emitter,
                        float* start_color, float* end_color, float line_width)

    void gl_particle_emitter_draw_velocity(gl_particle_emitter_t*, float* rgba)

    size_t gl_particle_emitter_count_all()

    void gl_particle_emitter_list_all(gl_particle_emitter_t** elist)
    void gl_particle_emitter_close_all()

def early_init():
    """
    Take the initialization lag hit before issuing any 3D graphics API calls.
    """
    gl_init()

def implementation():
    """
    Identify the underlying platform layer, for 'safer' game-specific hacks.
    """
    return gl_implementation()

# ==============================================================================
# ~ [ OpenGL wrapper ]
# ==============================================================================

if 1:
    # --------------------------------------
    # boolean values
    #
    FALSE = 0
    TRUE  = 1

    # --------------------------------------
    # data types
    #
    BYTE           = 0x1400
    UNSIGNED_BYTE  = 0x1401
    SHORT          = 0x1402
    UNSIGNED_SHORT = 0x1403
    INT            = 0x1404
    UNSIGNED_INT   = 0x1405
    FLOAT          = 0x1406
    DOUBLE         = 0x140A

    # --------------------------------------
    # primitives
    #
    POINTS         = 0x0000
    LINES          = 0x0001
    LINE_LOOP      = 0x0002
    LINE_STRIP     = 0x0003
    TRIANGLES      = 0x0004
    TRIANGLE_STRIP = 0x0005
    TRIANGLE_FAN   = 0x0006
    QUADS          = 0x0007
    QUAD_STRIP     = 0x0008
    POLYGON        = 0x0009

    # --------------------------------------
    # vertex arrays
    #
    VERTEX_ARRAY                = 0x8074
    NORMAL_ARRAY                = 0x8075
    COLOR_ARRAY                 = 0x8076
    INDEX_ARRAY                 = 0x8077
    TEXTURE_COORD_ARRAY         = 0x8078
    EDGE_FLAG_ARRAY             = 0x8079
    VERTEX_ARRAY_SIZE           = 0x807A
    VERTEX_ARRAY_TYPE           = 0x807B
    VERTEX_ARRAY_STRIDE         = 0x807C
    NORMAL_ARRAY_TYPE           = 0x807E
    NORMAL_ARRAY_STRIDE         = 0x807F
    COLOR_ARRAY_SIZE            = 0x8081
    COLOR_ARRAY_TYPE            = 0x8082
    COLOR_ARRAY_STRIDE          = 0x8083
    INDEX_ARRAY_TYPE            = 0x8085
    INDEX_ARRAY_STRIDE          = 0x8086
    TEXTURE_COORD_ARRAY_SIZE    = 0x8088
    TEXTURE_COORD_ARRAY_TYPE    = 0x8089
    TEXTURE_COORD_ARRAY_STRIDE  = 0x808A
    EDGE_FLAG_ARRAY_STRIDE      = 0x808C
    VERTEX_ARRAY_POINTER        = 0x808E
    NORMAL_ARRAY_POINTER        = 0x808F
    COLOR_ARRAY_POINTER         = 0x8090
    INDEX_ARRAY_POINTER         = 0x8091
    TEXTURE_COORD_ARRAY_POINTER = 0x8092
    EDGE_FLAG_ARRAY_POINTER     = 0x8093
    V2F                         = 0x2A20
    V3F                         = 0x2A21
    C4UB_V2F                    = 0x2A22
    C4UB_V3F                    = 0x2A23
    C3F_V3F                     = 0x2A24
    N3F_V3F                     = 0x2A25
    C4F_N3F_V3F                 = 0x2A26
    T2F_V3F                     = 0x2A27
    T4F_V4F                     = 0x2A28
    T2F_C4UB_V3F                = 0x2A29
    T2F_C3F_V3F                 = 0x2A2A
    T2F_N3F_V3F                 = 0x2A2B
    T2F_C4F_N3F_V3F             = 0x2A2C
    T4F_C4F_N3F_V4F             = 0x2A2D

    # --------------------------------------
    # matrix modes
    #
    MATRIX_MODE = 0x0BA0
    MODELVIEW   = 0x1700
    PROJECTION  = 0x1701
    TEXTURE     = 0x1702

    # --------------------------------------
    # polygons
    #
    POINT                 = 0x1B00
    LINE                  = 0x1B01
    FILL                  = 0x1B02
    CW                    = 0x0900
    CCW                   = 0x0901
    FRONT                 = 0x0404
    BACK                  = 0x0405
    POLYGON_MODE          = 0x0B40
    POLYGON_SMOOTH        = 0x0B41
    POLYGON_STIPPLE       = 0x0B42
    EDGE_FLAG             = 0x0B43
    CULL_FACE             = 0x0B44
    CULL_FACE_MODE        = 0x0B45
    FRONT_FACE            = 0x0B46
    POLYGON_OFFSET_FACTOR = 0x8038
    POLYGON_OFFSET_UNITS  = 0x2A00
    POLYGON_OFFSET_POINT  = 0x2A01
    POLYGON_OFFSET_LINE   = 0x2A02
    POLYGON_OFFSET_FILL   = 0x8037

    # --------------------------------------
    # depth buffer
    #
    NEVER             = 0x0200
    LESS              = 0x0201
    EQUAL             = 0x0202
    LEQUAL            = 0x0203
    GREATER           = 0x0204
    NOTEQUAL          = 0x0205
    GEQUAL            = 0x0206
    ALWAYS            = 0x0207
    DEPTH_TEST        = 0x0B71
    DEPTH_BITS        = 0x0D56
    DEPTH_CLEAR_VALUE = 0x0B73
    DEPTH_FUNC        = 0x0B74
    DEPTH_RANGE       = 0x0B70
    DEPTH_WRITEMASK   = 0x0B72
    DEPTH_COMPONENT   = 0x1902

    # --------------------------------------
    # lighting
    #
    LIGHTING                 = 0x0B50
    LIGHT0                   = 0x4000
    LIGHT1                   = 0x4001
    LIGHT2                   = 0x4002
    LIGHT3                   = 0x4003
    LIGHT4                   = 0x4004
    LIGHT5                   = 0x4005
    LIGHT6                   = 0x4006
    LIGHT7                   = 0x4007
    SPOT_EXPONENT            = 0x1205
    SPOT_CUTOFF              = 0x1206
    CONSTANT_ATTENUATION     = 0x1207
    LINEAR_ATTENUATION       = 0x1208
    QUADRATIC_ATTENUATION    = 0x1209
    AMBIENT                  = 0x1200
    DIFFUSE                  = 0x1201
    SPECULAR                 = 0x1202
    SHININESS                = 0x1601
    EMISSION                 = 0x1600
    POSITION                 = 0x1203
    SPOT_DIRECTION           = 0x1204
    AMBIENT_AND_DIFFUSE      = 0x1602
    COLOR_INDEXES            = 0x1603
    LIGHT_MODEL_TWO_SIDE     = 0x0B52
    LIGHT_MODEL_LOCAL_VIEWER = 0x0B51
    LIGHT_MODEL_AMBIENT      = 0x0B53
    FRONT_AND_BACK           = 0x0408
    SHADE_MODEL              = 0x0B54
    FLAT                     = 0x1D00
    SMOOTH                   = 0x1D01
    COLOR_MATERIAL           = 0x0B57
    COLOR_MATERIAL_FACE      = 0x0B55
    COLOR_MATERIAL_PARAMETER = 0x0B56
    NORMALIZE                = 0x0BA1

    # --------------------------------------
    # blending
    #
    BLEND               = 0x0BE2
    BLEND_SRC           = 0x0BE1
    BLEND_DST           = 0x0BE0
    ZERO                = 0x0000
    ONE                 = 0x0001
    SRC_COLOR           = 0x0300
    ONE_MINUS_SRC_COLOR = 0x0301
    SRC_ALPHA           = 0x0302
    ONE_MINUS_SRC_ALPHA = 0x0303
    DST_ALPHA           = 0x0304
    ONE_MINUS_DST_ALPHA = 0x0305
    DST_COLOR           = 0x0306
    ONE_MINUS_DST_COLOR = 0x0307
    SRC_ALPHA_SATURATE  = 0x0308

    # --------------------------------------
    # fog
    #
    FOG         = 0x0B60
    FOG_MODE    = 0x0B65
    FOG_DENSITY = 0x0B62
    FOG_COLOR   = 0x0B66
    FOG_INDEX   = 0x0B61
    FOG_START   = 0x0B63
    FOG_END     = 0x0B64
    LINEAR      = 0x2601
    EXP         = 0x0800
    EXP2        = 0x0801

    # --------------------------------------
    # logic ops
    #
    LOGIC_OP       = 0x0BF1
    INDEX_LOGIC_OP = 0x0BF1
    COLOR_LOGIC_OP = 0x0BF2
    LOGIC_OP_MODE  = 0x0BF0
    CLEAR          = 0x1500
    SET            = 0x150F
    COPY           = 0x1503
    COPY_INVERTED  = 0x150C
    NOOP           = 0x1505
    INVERT         = 0x150A
    AND            = 0x1501
    NAND           = 0x150E
    OR             = 0x1507
    NOR            = 0x1508
    XOR            = 0x1506
    EQUIV          = 0x1509
    AND_REVERSE    = 0x1502
    AND_INVERTED   = 0x1504
    OR_REVERSE     = 0x150B
    OR_INVERTED    = 0x150D

    # --------------------------------------
    # stencil
    #
    STENCIL_BITS            = 0x0D57
    STENCIL_TEST            = 0x0B90
    STENCIL_CLEAR_VALUE     = 0x0B91
    STENCIL_FUNC            = 0x0B92
    STENCIL_VALUE_MASK      = 0x0B93
    STENCIL_FAIL            = 0x0B94
    STENCIL_PASS_DEPTH_FAIL = 0x0B95
    STENCIL_PASS_DEPTH_PASS = 0x0B96
    STENCIL_REF             = 0x0B97
    STENCIL_WRITEMASK       = 0x0B98
    STENCIL_INDEX           = 0x1901
    KEEP                    = 0x1E00
    REPLACE                 = 0x1E01
    INCR                    = 0x1E02
    DECR                    = 0x1E03

    # --------------------------------------
    # texture mapping
    #
    TEXTURE_ENV            = 0x2300
    TEXTURE_ENV_MODE       = 0x2200
    TEXTURE_1D             = 0x0DE0
    TEXTURE_2D             = 0x0DE1
    TEXTURE_WRAP_S         = 0x2802
    TEXTURE_WRAP_T         = 0x2803
    TEXTURE_MAG_FILTER     = 0x2800
    TEXTURE_MIN_FILTER     = 0x2801
    TEXTURE_ENV_COLOR      = 0x2201
    TEXTURE_GEN_S          = 0x0C60
    TEXTURE_GEN_T          = 0x0C61
    TEXTURE_GEN_R          = 0x0C62
    TEXTURE_GEN_Q          = 0x0C63
    TEXTURE_GEN_MODE       = 0x2500
    TEXTURE_BORDER_COLOR   = 0x1004
    TEXTURE_WIDTH          = 0x1000
    TEXTURE_HEIGHT         = 0x1001
    TEXTURE_BORDER         = 0x1005
    TEXTURE_COMPONENTS     = 0x1003
    TEXTURE_RED_SIZE       = 0x805C
    TEXTURE_GREEN_SIZE     = 0x805D
    TEXTURE_BLUE_SIZE      = 0x805E
    TEXTURE_ALPHA_SIZE     = 0x805F
    TEXTURE_LUMINANCE_SIZE = 0x8060
    TEXTURE_INTENSITY_SIZE = 0x8061
    NEAREST_MIPMAP_NEAREST = 0x2700
    NEAREST_MIPMAP_LINEAR  = 0x2702
    LINEAR_MIPMAP_NEAREST  = 0x2701
    LINEAR_MIPMAP_LINEAR   = 0x2703
    OBJECT_LINEAR          = 0x2401
    OBJECT_PLANE           = 0x2501
    EYE_LINEAR             = 0x2400
    EYE_PLANE              = 0x2502
    SPHERE_MAP             = 0x2402
    DECAL                  = 0x2101
    MODULATE               = 0x2100
    NEAREST                = 0x2600
    REPEAT                 = 0x2901
    CLAMP                  = 0x2900
    S                      = 0x2000
    T                      = 0x2001
    R                      = 0x2002
    Q                      = 0x2003

    # --------------------------------------
    # utility
    #
    VENDOR     = 0x1F00
    RENDERER   = 0x1F01
    VERSION    = 0x1F02
    EXTENSIONS = 0x1F03

    # --------------------------------------
    # errors
    #
    NO_ERROR          = 0x0000
    INVALID_ENUM      = 0x0500
    INVALID_VALUE     = 0x0501
    INVALID_OPERATION = 0x0502
    STACK_OVERFLOW    = 0x0503
    STACK_UNDERFLOW   = 0x0504
    OUT_OF_MEMORY     = 0x0505

    # --------------------------------------
    # attribute bits
    #
    CURRENT_BIT         = 0x00000001
    POINT_BIT           = 0x00000002
    LINE_BIT            = 0x00000004
    POLYGON_BIT         = 0x00000008
    POLYGON_STIPPLE_BIT = 0x00000010
    PIXEL_MODE_BIT      = 0x00000020
    LIGHTING_BIT        = 0x00000040
    FOG_BIT             = 0x00000080
    DEPTH_BUFFER_BIT    = 0x00000100
    ACCUM_BUFFER_BIT    = 0x00000200
    STENCIL_BUFFER_BIT  = 0x00000400
    VIEWPORT_BIT        = 0x00000800
    TRANSFORM_BIT       = 0x00001000
    ENABLE_BIT          = 0x00002000
    COLOR_BUFFER_BIT    = 0x00004000
    HINT_BIT            = 0x00008000
    EVAL_BIT            = 0x00010000
    LIST_BIT            = 0x00020000
    TEXTURE_BIT         = 0x00040000
    SCISSOR_BIT         = 0x00080000
    ALL_ATTRIB_BITS     = 0x000FFFFF

    # --------------------------------------
    # OpenGL 1.1
    #
    PROXY_TEXTURE_1D        = 0x8063
    PROXY_TEXTURE_2D        = 0x8064
    TEXTURE_PRIORITY        = 0x8066
    TEXTURE_RESIDENT        = 0x8067
    TEXTURE_BINDING_1D      = 0x8068
    TEXTURE_BINDING_2D      = 0x8069
    TEXTURE_INTERNAL_FORMAT = 0x1003
    ALPHA4                  = 0x803B
    ALPHA8                  = 0x803C
    ALPHA12                 = 0x803D
    ALPHA16                 = 0x803E
    LUMINANCE4              = 0x803F
    LUMINANCE8              = 0x8040
    LUMINANCE12             = 0x8041
    LUMINANCE16             = 0x8042
    LUMINANCE4_ALPHA4       = 0x8043
    LUMINANCE6_ALPHA2       = 0x8044
    LUMINANCE8_ALPHA8       = 0x8045
    LUMINANCE12_ALPHA4      = 0x8046
    LUMINANCE12_ALPHA12     = 0x8047
    LUMINANCE16_ALPHA16     = 0x8048
    INTENSITY               = 0x8049
    INTENSITY4              = 0x804A
    INTENSITY8              = 0x804B
    INTENSITY12             = 0x804C
    INTENSITY16             = 0x804D
    R3_G3_B2                = 0x2A10
    RGB4                    = 0x804F
    RGB5                    = 0x8050
    RGB8                    = 0x8051
    RGB10                   = 0x8052
    RGB12                   = 0x8053
    RGB16                   = 0x8054
    RGBA2                   = 0x8055
    RGBA4                   = 0x8056
    RGB5_A1                 = 0x8057
    RGBA8                   = 0x8058
    RGB10_A2                = 0x8059
    RGBA12                  = 0x805A
    RGBA16                  = 0x805B
    CLIENT_PIXEL_STORE_BIT  = 0x00000001
    CLIENT_VERTEX_ARRAY_BIT = 0x00000002
    ALL_CLIENT_ATTRIB_BITS  = 0xFFFFFFFF
    CLIENT_ALL_ATTRIB_BITS  = 0xFFFFFFFF

    # --------------------------------------
    # multisample
    #
    MULTISAMPLE              = 0x809D
    SAMPLE_ALPHA_TO_COVERAGE = 0x809E
    SAMPLE_ALPHA_TO_ONE      = 0x809F
    SAMPLE_COVERAGE          = 0x80A0
    SAMPLE_BUFFERS           = 0x80A8
    SAMPLES                  = 0x80A9
    SAMPLE_COVERAGE_VALUE    = 0x80AA
    SAMPLE_COVERAGE_INVERT   = 0x80AB
    MULTISAMPLE_BIT          = 0x20000000

def AlphaFunc(unsigned int func, float ref):
    GL_AlphaFunc(func, ref)

def Begin(unsigned int mode):
    GL_Begin(mode)

def BindTexture(unsigned int target, unsigned int texture):
    GL_BindTexture(target, texture)

def BlendFunc(unsigned int src, unsigned int dst):
    GL_BlendFunc(src, dst)

def BlendFuncSeparate(unsigned sRGB, unsigned dRGB, unsigned sA, unsigned dA):
    GL_BlendFuncSeparate(sRGB, dRGB, sA, dA)

def Clear(unsigned int mask):
    GL_Clear(mask)

def ClearColor(float r, float g, float b, float a):
    GL_ClearColor(r, g, b, a)

def Color3f(float r, float g, float b):
    GL_Color3f(r, g, b)

def Color4f(float r, float g, float b, float a):
    GL_Color4f(r, g, b, a)

def Color3fv(Vec3 rgb):
    GL_Color3fv(rgb.v)

def Color4fv(Vec4 rgba):
    GL_Color4fv(rgba.v)

def ColorMask(unsigned char r, unsigned char g, unsigned char b, unsigned char a):
    GL_ColorMask(r, g, b, a)

def ColorMaterial(unsigned int face, unsigned int mode):
    GL_ColorMaterial(face, mode)

def ColorPointer(int size, unsigned int type, int stride, size_t data):
    GL_ColorPointer(size, type, stride, <const void*>data)

def CullFace(unsigned int face):
    GL_CullFace(face)

def DeleteTextures(*a, **k):
    raise NotImplementedError("TODO")

def DepthFunc(unsigned int func):
    GL_DepthFunc(func)

def DepthMask(unsigned char flag):
    GL_DepthMask(flag)

def DepthRange(double near_val, double far_val):
    GL_DepthRange(near_val, far_val)

def Disable(unsigned int cap):
    GL_Disable(cap)

def DisableClientState(unsigned int cap):
    GL_DisableClientState(cap)

def DrawArrays(unsigned int mode, int first, int count):
    GL_DrawArrays(mode, first, count)

def DrawElements(*a, **k):
    raise NotImplementedError("TODO")

def Enable(unsigned int cap):
    GL_Enable(cap)

def EnableClientState(unsigned int cap):
    GL_EnableClientState(cap)

def End():
    GL_End()

def Finish():
    GL_Finish()

def Flush():
    GL_Flush()

def Fogfv(unsigned int pname, size_t params):
    GL_Fogfv(pname, <const float*>params)

def Fogiv(unsigned int pname, size_t params):
    GL_Fogiv(pname, <const int*>params)

def Fogf(unsigned int pname, float param):
    GL_Fogf(pname, param)

def Fogi(unsigned int pname, int param):
    GL_Fogi(pname, param)

def FrontFace(unsigned int mode):
    GL_FrontFace(mode)

def Frustum(double lf, double rt, double bt, double tp, double nr, double fr):
    GL_Frustum(lf, rt, bt, tp, nr, fr)

def GenTextures(int n, size_t textures):
    GL_GenTextures(n, <unsigned int*>textures)

def GetError():
    return GL_GetError()

def GetFloatv(*a, **k):
    raise NotImplementedError("TODO")

def GetIntegerv(*a, **k):
    raise NotImplementedError("TODO")

def GetLightfv(*a, **k):
    raise NotImplementedError("TODO")

def GetMaterialfv(*a, **k):
    raise NotImplementedError("TODO")

def GetString(unsigned int name):
    return GL_GetString(name)

def GetTexParameteriv(*a, **k):
    raise NotImplementedError("TODO")

def InterleavedArrays(unsigned int format, int stride, size_t data):
    GL_InterleavedArrays(format, stride, <const void*>data)

def IsEnabled(unsigned int cap):
    return GL_IsEnabled(cap)

def IsTexture(unsigned int texture):
    return GL_IsTexture(texture)

def Lightfv(unsigned int light, unsigned int pname, size_t params):
    GL_Lightfv(light, pname, <const float*>params)

def Lightf(unsigned int light, unsigned int pname, float param):
    GL_Lightf(light, pname, param)

def LineWidth(float width):
    GL_LineWidth(width)

def LoadIdentity():
    GL_LoadIdentity()

def LogicOp(unsigned int opcode):
    GL_LogicOp(opcode)

def Materialfv(unsigned int face, unsigned int pname, size_t params):
    GL_Materialfv(face, pname, <const float*>params)

def Materialf(unsigned int face, unsigned int pname, float param):
    GL_Materialf(face, pname, param)

def MatrixMode(unsigned int mode):
    GL_MatrixMode(mode)

def MultMatrixf(Mat4x4 matrix):
    GL_MultMatrixf(matrix.m)

def Normal3f(float x, float y, float z):
    GL_Normal3f(x, y, z)

def Normal3fv(Vec3 normal):
    GL_Normal3fv(normal.v)

def NormalPointer(unsigned int type, int stride, size_t data):
    GL_NormalPointer(type, stride, <const void*>data)

def Ortho(double lf, double rt, double btm, double top, double near, double far):
    GL_Ortho(lf, rt, btm, top, near, far)

def PointParameterfv(unsigned int pname, size_t data):
    GL_PointParameterfv(pname, <const float*>data)

def PointParameterf(unsigned int pname, float param):
    GL_PointParameterf(pname, param)

def PointSize(float size):
    GL_PointSize(size)

def PolygonMode(unsigned int face, unsigned int mode):
    GL_PolygonMode(face, mode)

def PolygonOffset(float factor, float units):
    GL_PolygonOffset(factor, units)

def PopAttrib():
    GL_PopAttrib()

def PushAttrib(unsigned int mask):
    GL_PushAttrib(mask)

def PopClientAttrib():
    GL_PopClientAttrib()

def PushClientAttrib(unsigned int mask):
    GL_PushClientAttrib(mask)

def PopMatrix():
    GL_PopMatrix()

def PushMatrix():
    GL_PushMatrix()

def Rotatef(float theta, float x, float y, float z):
    GL_Rotatef(theta, x, y, z)

def Scalef(float x, float y, float z):
    GL_Scalef(x, y, z)

def Scissor(int x, int y, int width, int height):
    GL_Scissor(x, y, width, height)

def ShadeModel(unsigned int mode):
    GL_ShadeModel(mode)

def TexCoord2f(float s, float t):
    GL_TexCoord2f(s, t)

def TexCoord3f(float s, float t, float r):
    GL_TexCoord3f(s, t, r)

def TexCoord4f(float s, float t, float r, float q):
    GL_TexCoord4f(s, t, r, q)

def TexCoord2fv(Vec2 st):
    GL_TexCoord2fv(st.v)

def TexCoord3fv(Vec3 str):
    GL_TexCoord3fv(str.v)

def TexCoord4fv(Vec4 strq):
    GL_TexCoord4fv(strq.v)

def TexCoordPointer(int size, unsigned int type, int stride, size_t data):
    GL_TexCoordPointer(size, type, stride, <const void*>data)

def TexEnvf(unsigned int target, unsigned int pname, float param):
    GL_TexEnvf(target, pname, param)

def TexEnvi(unsigned int target, unsigned int pname, int param):
    GL_TexEnvi(target, pname, param)

def TexImage2D(*a, **k):
    raise NotImplementedError("TODO")

def TexParameteri(unsigned int target, unsigned int pname, int param):
    GL_TexParameteri(target, pname, param)

def TexSubImage2D(*a, **k):
    raise NotImplementedError("TODO")

def Translatef(float x, float y, float z):
    GL_Translatef(x, y, z)

def Vertex2f(float x, float y):
    GL_Vertex2f(x, y)

def Vertex3f(float x, float y, float z):
    GL_Vertex3f(x, y, z)

def Vertex2fv(Vec2 xy):
    GL_Vertex2fv(xy.v)

def Vertex3fv(Vec3 xyz):
    GL_Vertex3fv(xyz.v)

def VertexPointer(int size, unsigned int type, int stride, size_t data):
    GL_VertexPointer(size, type, stride, <const void*>data)

def Viewport(int x, int y, int width, int height):
    GL_Viewport(x, y, width, height)

# ==============================================================================
# ~ [ GL state logging ]
# ==============================================================================

cdef class Logger:
    """
    Logs various bits of internal OpenGL state (debugging only).
    """
    def __call__(self, str msg):
        __import__('aegame').utl.log("OPENGL", msg); return self

    @staticmethod
    def enable(bint enabled):
        __import__('aegame').utl.log.enable("OPENGL", enabled)

    @staticmethod
    def is_enabled():
        return __import__('aegame').utl.log.is_enabled("OPENGL")

    @staticmethod
    def error_state_ex(bytes filename, bytes funcname):
        gl_log_error_state_ex(<char*>filename, <char*>funcname)

    @staticmethod
    def error_state():
        gl_log_error_state()

    @staticmethod
    def debug_error_state():
        gl_debug_log_error_state()

    @staticmethod
    def texture_state_ex(bytes filename, bytes funcname):
        gl_log_texture_state_ex(<char*>filename, <char*>funcname)

    @staticmethod
    def texture_state():
        gl_log_texture_state()

    @staticmethod
    def debug_texture_state():
        gl_debug_log_texture_state()

    @staticmethod
    def blend_state_ex(bytes filename, bytes funcname):
        gl_log_blend_state_ex(<char*>filename, <char*>funcname)

    @staticmethod
    def blend_state():
        gl_log_blend_state()

    @staticmethod
    def debug_blend_state():
        gl_debug_log_blend_state()

    @staticmethod
    def material_state_ex(bytes filename, bytes funcname):
        gl_log_material_state_ex(<char*>filename, <char*>funcname)

    @staticmethod
    def material_state():
        gl_log_material_state()

    @staticmethod
    def debug_material_state():
        gl_debug_log_material_state()

    @staticmethod
    def light_state_ex(bytes filename, bytes funcname):
        gl_log_light_state_ex(<char*>filename, <char*>funcname)

    @staticmethod
    def light_state():
        gl_log_light_state()

    @staticmethod
    def debug_light_state():
        gl_debug_log_light_state()

    @staticmethod
    def internal_state_ex(bytes filename, bytes funcname):
        gl_log_internal_state_ex(<char*>filename, <char*>funcname)

    @staticmethod
    def internal_state():
        gl_log_internal_state()

    @staticmethod
    def debug_internal_state():
        gl_debug_log_internal_state()

log = Logger()

# ==============================================================================
# ~ [ utilities ]
# ==============================================================================

class util(object):
    """
    Namespace for freestanding utility functions and other miscellaneous things.
    """
    @staticmethod
    def enter3D(int width, int height, double FOV, double znear, double zfar):
        gl_enter3D(width, height, FOV, znear, zfar)

    @staticmethod
    def leave3D(): gl_leave3D()

    class Scene3D(object):
        """
        Context manager for (enter/leave)3D.
        """
        def __init__(self, *a, **k):
            self.a = a
            self.k = k

        def __enter__(self):
            util.enter3D(*self.a, **self.k)

        def __exit__(self, *a):
            util.leave3D()

    @staticmethod
    def enter2D(int width, int height):
        gl_enter2D(width, height)

    @staticmethod
    def leave2D(): gl_leave2D()

    class Scene2D(object):
        """
        Context manager for (enter/leave)2D.
        """
        def __init__(self, *a, **k):
            self.a = a
            self.k = k

        def __enter__(self):
            util.enter2D(*self.a, **self.k)

        def __exit__(self, *a):
            util.leave2D()

    class Attrib(object):
        """
        Context manager for the server (GPU) attribute stack.
        """
        def __init__(self, unsigned int mask):
            self.mask = mask

        def __enter__(self):
            GL_PushAttrib(self.mask)

        def __exit__(self, *a):
            GL_PopAttrib()

    class ClientAttrib(object):
        """
        Context manager for the client (CPU) attribute stack.
        """
        def __init__(self, unsigned int mask):
            self.mask = mask

        def __enter__(self):
            GL_PushClientAttrib(self.mask)

        def __exit__(self, *a):
            GL_PopClientAttrib()

    class Immediate(object):
        """
        Context manager for glBegin and glEnd.
        """
        def __init__(self, unsigned int mode):
            self.mode = mode

        def __enter__(self):
            GL_Begin(self.mode)

        def __exit__(self, *a):
            GL_End()

    class Matrix(object):
        """
        Context manager for the current matrix stack.
        """
        def __enter__(self):
            GL_PushMatrix()

        def __exit__(self, *a):
            GL_PopMatrix()

    @staticmethod
    def rect_ex(FloatRect rect, Vec4 bl, Vec4 br, Vec4 tr, Vec4 tl,
                bint line, float line_width):
        """
        Make the standard "Hello OpenGL" gradient quadrilateral, or something.
        """
        gl_rect_ex(rect.rect, bl.v, br.v, tr.v, tl.v, line, line_width)

    @staticmethod
    def outline(FloatRect rect, Vec4 rgba):
        """
        Draw an alpha-blended 2-dimensional rectangle from lines.
        """
        gl_outline(rect.rect, rgba.v)

    @staticmethod
    def rect(FloatRect rect, Vec4 rgba):
        """
        Draw an alpha-blended 2-dimensional quad.
        """
        gl_rect(rect.rect, rgba.v)

    @staticmethod
    def aabbox(Vec3 minv, Vec3 maxv, Vec4 rgba, float line_width=1.0):
        """
        Render a 3-dimensional bounding box outline. Mostly for debug.
        """
        gl_aabbox_ex(minv.v, maxv.v, rgba.v, line_width)

    @staticmethod
    def ellipsoid(Ellipsoid ellipsoid, Vec4 rgba):
        """
        Draw a wireframe capsule shape for character collision volumes.
        """
        gl_ellipsoid(ellipsoid.c.v, ellipsoid.v.v, rgba.v)

    @staticmethod
    def sphere(Sphere sphere, Vec4 rgba):
        """
        Render a 3-dimensional wireframe sphere shape for debugging.
        """
        gl_sphere(sphere.c.v, sphere.r, rgba.v)

# ==============================================================================
# ~ [ coordinate system ]
# ==============================================================================
# TODO: ObjectTransform and CameraTransform context managers here would be cool!
# ------------------------------------------------------------------------------

cdef class Coord3D:
    cdef gl_coord3D_t coord

    def __init__(self):
        gl_coord3D_init(&self.coord)

    def __repr__(self):
        return "{}(pos: {} rot: {} scl: {})".format( self.__class__.__name__,
                                    self.position, self.rotation, self.scale)

    def __copy__(self):
        cdef Coord3D c = self.__class__()
        gl_coord3D_copy(&c.coord, &self.coord); return c

    copy = __copy__

    def __reduce__(self):
        cdef bytes b = b'\0' * sizeof(gl_coord3D_t)
        cdef char* p = <char *>b
        cdef gl_coord3D_t* c = <gl_coord3D_t*>p

        gl_coord3D_copy(c, &self.coord)
        return (Coord3D, (), b)

    def __setstate__(self, bytes b):
        cdef char* p = <char *>b
        cdef gl_coord3D_t* c = <gl_coord3D_t*>p

        gl_coord3D_copy(&self.coord, c)

    def __nonzero__(self):
        return not memiszero(&self.coord, sizeof(gl_coord3D_t))

    def normalize180(self):
        gl_coord3D_normalize180(&self.coord); return self

    def normalize360(self):
        gl_coord3D_normalize360(&self.coord); return self

    def apply_camera_transform_ex(self, bint absolute):
        gl_coord3D_apply_camera_transform_ex(&self.coord, absolute); return self

    def push_camera_transform_ex(self, bint absolute):
        gl_coord3D_push_camera_transform_ex(&self.coord, absolute); return self

    def apply_camera_transform(self):
        gl_coord3D_apply_camera_transform(&self.coord); return self

    def push_camera_transform(self):
        gl_coord3D_push_camera_transform(&self.coord); return self

    @staticmethod
    def pop_camera_transform():
        gl_coord3D_pop_camera_transform()

    apply_camera_ex = apply_camera_transform_ex
    push_camera_ex = push_camera_transform_ex
    apply_camera = apply_camera_transform
    push_camera = push_camera_transform
    pop_camera = pop_camera_transform

    def apply_object_transform_ex(self, bint absolute):
        gl_coord3D_apply_object_transform_ex(&self.coord, absolute); return self

    def push_object_transform_ex(self, bint absolute):
        gl_coord3D_push_object_transform_ex(&self.coord, absolute); return self

    def apply_object_transform(self):
        gl_coord3D_apply_object_transform(&self.coord); return self

    def push_object_transform(self):
        gl_coord3D_push_object_transform(&self.coord); return self

    @staticmethod
    def pop_object_transform():
        gl_coord3D_pop_object_transform()

    apply_object_ex = apply_object_transform_ex
    push_object_ex = push_object_transform_ex
    apply_object = apply_object_transform
    push_object = push_object_transform
    pop_object = pop_object_transform

    def heading_towards_point(self, Vec3 p):
        gl_coord3D_heading_towards_point(&self.coord, p.v); return self

    def pitch_towards_point(self, Vec3 p):
        gl_coord3D_pitch_towards_point(&self.coord, p.v); return self

    def face_towards_point(self, Vec3 p):
        gl_coord3D_face_towards_point(&self.coord, p.v); return self

    property position:
        def __get__(self):
            cdef Vec3 v = Vec3()
            vec3copy(v.v, self.coord.position); return v

        def __set__(self, Vec3 value):
            vec3copy(self.coord.position, value.v)

    property pos:
        def __get__(self):
            cdef Vec3 v = Vec3()
            vec3copy(v.v, self.coord.pos); return v

        def __set__(self, Vec3 value):
            vec3copy(self.coord.pos, value.v)

    property translation:
        def __get__(self):
            cdef Vec3 v = Vec3()
            vec3copy(v.v, self.coord.translation); return v

        def __set__(self, Vec3 value):
            vec3copy(self.coord.translation, value.v)

    property xyz:
        def __get__(self):
            cdef Vec3 v = Vec3()
            vec3copy(v.v, self.coord.xyz); return v

        def __set__(self, Vec3 value):
            vec3copy(self.coord.xyz, value.v)

    property x:
        def __get__(self):
            return self.coord.x

        def __set__(self, float value):
            self.coord.x = value

    property y:
        def __get__(self):
            return self.coord.y

        def __set__(self, float value):
            self.coord.y = value

    property z:
        def __get__(self):
            return self.coord.z

        def __set__(self, float value):
            self.coord.z = value

    property rotation:
        def __get__(self):
            cdef Vec3 v = Vec3()
            vec3copy(v.v, self.coord.rotation); return v

        def __set__(self, Vec3 value):
            vec3copy(self.coord.rotation, value.v)

    property rot:
        def __get__(self):
            cdef Vec3 v = Vec3()
            vec3copy(v.v, self.coord.rot); return v

        def __set__(self, Vec3 value):
            vec3copy(self.coord.rot, value.v)

    property pitch:
        def __get__(self):
            return self.coord.pitch

        def __set__(self, float value):
            self.coord.pitch = value

    property heading:
        def __get__(self):
            return self.coord.heading

        def __set__(self, float value):
            self.coord.heading = value

    property roll:
        def __get__(self):
            return self.coord.roll

        def __set__(self, float value):
            self.coord.roll = value

    property scale:
        def __get__(self):
            cdef Vec3 v = Vec3()
            vec3copy(v.v, self.coord.scale); return v

        def __set__(self, Vec3 value):
            vec3copy(self.coord.scale, value.v)

    property scl:
        def __get__(self):
            cdef Vec3 v = Vec3()
            vec3copy(v.v, self.coord.scl); return v

        def __set__(self, Vec3 value):
            vec3copy(self.coord.scl, value.v)

    property scale_x:
        def __get__(self):
            return self.coord.scale_x

        def __set__(self, float value):
            self.coord.scale_x = value

    property scale_y:
        def __get__(self):
            return self.coord.scale_y

        def __set__(self, float value):
            self.coord.scale_y = value

    property scale_z:
        def __get__(self):
            return self.coord.scale_z

        def __set__(self, float value):
            self.coord.scale_z = value

    property matrix:
        def __get__(self):
            cdef Mat4x4 m = Mat4x4()
            gl_coord3D_get_matrix(&self.coord, m.m); return m

        def __set__(self, Mat4x4 m):
            gl_coord3D_set_matrix(&self.coord, m.m)

    property quat:
        def __get__(self):
            cdef Quat q = Quat()
            gl_coord3D_get_quat(&self.coord, q.q); return q

        def __set__(self, Quat q):
            gl_coord3D_set_quat(&self.coord, q.q)

    property normal:
        def __get__(self):
            cdef Vec3 v = Vec3()
            gl_coord3D_get_normal(&self.coord, v.v); return v

        def __set__(self, Vec3 v):
            gl_coord3D_set_normal(&self.coord, v.v)

    property ray:
        def __get__(self):
            cdef Ray3 r = Ray3(Vec3(), Vec3(0.0, 0.0, -1.0))
            gl_coord3D_get_ray(&self.coord, r.p.v, r.v.v); return r

        def __set__(self, Ray3 r):
            gl_coord3D_set_ray(&self.coord, r.p.v, r.v.v)

    property up_vector:
        def __get__(self):
            cdef Vec3 v = Vec3()
            gl_coord3D_get_up_vector(&self.coord, v.v); return v

        def __set__(self, Vec3 v):
            gl_coord3D_set_up_vector(&self.coord, v.v)

    property down_vector:
        def __get__(self):
            cdef Vec3 v = Vec3()
            gl_coord3D_get_down_vector(&self.coord, v.v); return v

        def __set__(self, Vec3 v):
            gl_coord3D_set_down_vector(&self.coord, v.v)

    property left_vector:
        def __get__(self):
            cdef Vec3 v = Vec3()
            gl_coord3D_get_left_vector(&self.coord, v.v); return v

        def __set__(self, Vec3 v):
            gl_coord3D_set_left_vector(&self.coord, v.v)

    property right_vector:
        def __get__(self):
            cdef Vec3 v = Vec3()
            gl_coord3D_get_right_vector(&self.coord, v.v); return v

        def __set__(self, Vec3 v):
            gl_coord3D_set_right_vector(&self.coord, v.v)

# ==============================================================================
# ~ [ generic objects ]
# ==============================================================================

cdef class Object:
    """
    Optionally garbage-collected interface for any GL object. Not a base class.
    """
    cdef public bint collect
    cdef void * ptr

    def __init__(self, size_t reference=0, bint collect=False):
        self.collect = collect
        self.ptr = <void*>reference

    def __dealloc__(self):
        if self.collect and self.open: self.close()

    def __repr__(self):
        if self.open:
            return ('Collected' if self.collect else 'Wrapped') + repr(self.cast())
        else:
            return '{}(unknown)'.format(self.__class__.__name__)

    def __hash__(self):
        return hash(self.address())

    def __richcmp__(self, object other, int op):
        if   op == 0: return self.address() <  other.address()
        elif op == 1: return self.address() <= other.address()
        elif op == 2: return self.address() == other.address()
        elif op == 3: return self.address() != other.address()
        elif op == 4: return self.address() >  other.address()
        elif op == 5: return self.address() >= other.address()

        else: assert 0

    def __nonzero__(self):
        return self.open

    def __reduce__(self):
        raise TypeError('cannot pickle {}'.format(self))

    def __copy__(self):
        raise TypeError('cannot copy {}'.format(self))

    def __getattr__(self, key):
        if self.open:
            return getattr(self.cast(), key)
        else:
            raise AttributeError("'{}' object has no attribute '{}'" \
                                        .format(str(type(self)), key))

    def __setattr__(self, key, val):
        if self.open:
            setattr(self.cast(), key, val)
        else:
            raise AttributeError("'{}' object has no attribute '{}'" \
                                        .format(str(type(self)), key))

    @staticmethod
    def count_all(): return gl_object_count_all()

    @classmethod
    def list_all(cls, bint cast=True):
        """
        Gather references to all open, active, valid objects in a single list.
        """
        cdef void* gl_objects[8192]
        cdef int i
        cdef list objects = []

        if gl_object_count_all() > 8192:
            raise MemoryError("too many open gl objects for temp")

        gl_object_list_all(gl_objects)

        for i in range(<int>gl_object_count_all()):
            objects.append(cls(reference = <size_t>gl_objects[i]))

            if cast: objects[i] = objects[i].cast()

        return objects

    @staticmethod
    def close_all(): gl_object_close_all()

    property open:
        def __get__(self):
            return gl_object_type(self.ptr) != GL_OBJECT_TYPE_UNKNOWN

    def address(self):
        return <size_t>self.ptr

    def cast(self):
        """
        Get self converted to the type of object it wraps. This is slow!!!
        """
        cdef gl_object_type_t object_type = gl_object_type(self.ptr) # enum
        cdef bytes name = gl_object_type_short_name[< size_t >object_type]

        if object_type == GL_OBJECT_TYPE_UNKNOWN:
            return self

        return globals()[name.replace('_', ' ').title().replace(' ', '')] \
                                            (reference = <size_t>self.ptr)

def ObjectFrom(object obj, bint collect=False):
    """
    Cheesy, easy grammar for the lazy typist. This would have been a class
    method, but 'from' is a keyword in Python for imports and generators.
    """
    return Object(obj.address(), collect)

def AutoObject(object obj):
    """
    Convenience ctor for locals, ex. `t = AutoObject(Texture().load(...))`.
    """
    return Object(obj.address(), True)

# ==============================================================================
# ~ [ texture renderer ]
# ==============================================================================

cdef class Texture:
    cdef gl_texture_t* texture

    def __init__(self, size_t reference=0):
        self.texture = <gl_texture_t*>reference

    def __repr__(self):
        return "{}({})".format(self.__class__.__name__, self.status)

    def __hash__(self):
        return hash(self.address())

    def __richcmp__(self, Texture other, int op):
        if   op == 0: return self.address() <  other.address()
        elif op == 1: return self.address() <= other.address()
        elif op == 2: return self.address() == other.address()
        elif op == 3: return self.address() != other.address()
        elif op == 4: return self.address() >  other.address()
        elif op == 5: return self.address() >= other.address()

        else: assert 0

    def __nonzero__(self):
        return self.open

    def __reduce__(self):
        raise TypeError('cannot pickle {}'.format(self))

    def __copy__(self):
        raise TypeError('cannot copy {}'.format(self))

    @staticmethod
    def count_all(): return gl_texture_count_all()

    @classmethod
    def list_all(cls):
        """
        Gather references to all open, active, valid textures in a single list.
        """
        cdef gl_texture_t* textures[8192]
        cdef int i
        cdef list objects = []

        if gl_texture_count_all() > 8192:
            raise MemoryError("too many open textures for temp")

        gl_texture_list_all(textures)

        for i in range(<int>gl_texture_count_all()):
            objects.append(cls(reference = <size_t>textures[i]))

        return objects

    @staticmethod
    def close_all(): gl_texture_close_all()

    property id:
        def __get__(self):
            return gl_texture_get_int(self.texture, GL_TEXTURE_PROPERTY_ID)

    property target:
        def __get__(self):
            return gl_texture_get_int(self.texture, GL_TEXTURE_PROPERTY_TARGET)

    property width:
        def __get__(self):
            return gl_texture_get_int(self.texture, GL_TEXTURE_PROPERTY_WIDTH)

    property height:
        def __get__(self):
            return gl_texture_get_int(self.texture, GL_TEXTURE_PROPERTY_HEIGHT)

    property comp:
        def __get__(self):
            return gl_texture_get_int(self.texture, GL_TEXTURE_PROPERTY_COMP)

    property image:
        def __get__(self):
            cdef ae_image_t * image_pointer = gl_texture_get_img(self.texture,
                                                    GL_TEXTURE_PROPERTY_IMAGE)
            cdef Image image_object = Image()

            # if we're not returning None (due to copy-on-upload being disabled), we must
            # make a copy of the image data for safe use with python's memory management.
            # this is the slow path you take by setting GL_TEXTURE_PROPERTY_COPY_ENABLED!

            if image_pointer != NULL:
                ae_image_copy(&image_object.image, image_pointer); return image_object

        def __set__(self, Image value):
            gl_texture_set_img(self.texture, GL_TEXTURE_PROPERTY_IMAGE, &value.image)

    property default_copy:
        def __get__(self):
            # XXX this is kind of a goofy place to put this - maybe make a staticmethod
            return gl_texture_get_int(self.texture, GL_TEXTURE_PROPERTY_DEFAULT_COPY)

        def __set__(self, bint value):
            gl_texture_set_int(self.texture, GL_TEXTURE_PROPERTY_DEFAULT_COPY, value)

    property copy_enabled:
        def __get__(self):
            return gl_texture_get_int(self.texture, GL_TEXTURE_PROPERTY_COPY_ENABLED)

        def __set__(self, bint value):
            gl_texture_set_int(self.texture, GL_TEXTURE_PROPERTY_COPY_ENABLED, value)

    property status:
        def __get__(self):
            return gl_texture_get_str(self.texture, GL_TEXTURE_PROPERTY_STATUS) # repr

        def __set__(self, bytes value):
            gl_texture_set_str(self.texture, GL_TEXTURE_PROPERTY_STATUS, <char*>value)

    property path:
        def __get__(self):
            return gl_texture_get_str(self.texture, GL_TEXTURE_PROPERTY_PATH)

        def __set__(self, bytes value):
            gl_texture_set_str(self.texture, GL_TEXTURE_PROPERTY_PATH, <char*>value)

    property name:
        def __get__(self):
            return gl_texture_get_str(self.texture, GL_TEXTURE_PROPERTY_NAME)

        def __set__(self, bytes value):
            gl_texture_set_str(self.texture, GL_TEXTURE_PROPERTY_NAME, <char*>value)

    property open:
        def __get__(self):
            return gl_texture_get_int(self.texture, GL_TEXTURE_PROPERTY_OPEN)

        def __set__(self, bint value):
            gl_texture_set_int(self.texture, GL_TEXTURE_PROPERTY_OPEN, value)

    def address(self):
        return <size_t>self.texture

    def cast(self):
        return self

    def close(self):
        gl_texture_close(self.texture); return self

    def create(self, int width, int height, int comp, **kwargs):
        """
        Manually create an OpenGL texture (set pixel data with the image property).
        """
        if not self.open:
            self.texture = gl_texture_create(width, height, comp)

            # convenient way to set some texture attributes inline
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def load_from_memory(self, size_t ptr, size_t length, **kwargs):
        """
        Load a compressed image file into a texture. Raises IOError if load fails.
        """
        cdef ae_image_error_t error = AE_IMAGE_NO_CODEC

        if not self.open:
            self.texture = gl_texture_load_from_memory_ex(<void*>ptr,
                                                    length, &error)

            if error != AE_IMAGE_SUCCESS:
                raise IOError(ae_image_error_message(error, NULL))

            # convenient way to set some texture attributes inline
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def load_from_array(self, Array data, **kwargs):
        """
        Load a texture from an array, likely a subview pointing to an archive slot.
        """
        return self.load_from_memory(data.address(), len(data), **kwargs)

    def load_from_bytes(self, bytes data, **kwargs):
        return self.load_from_memory(<size_t>(<char*>data), len(data), **kwargs)

    def load(self, bytes filename, **kwargs):
        """
        Load an image file into a texture. Raises IOError if image loading fails.
        """
        cdef ae_image_error_t error = AE_IMAGE_NO_CODEC # default for stubs

        if not self.open:
            self.texture = gl_texture_load_ex(<char*>filename, &error)

            if error != AE_IMAGE_SUCCESS:
                raise IOError(ae_image_error_message(error, <char*>filename))

            # convenient way to set some texture attributes inline
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def bind(self):
        """
        Make this the active texture in the current GL context, or disable textures.
        """
        gl_texture_bind(self.texture); return self

    def blit(self, float x, float y):
        """
        Draw a texture at (x, y). The origin is the texture's lower left corner.
        """
        gl_texture_blit(self.texture, x, y); return self

    def blit_ex(self, float x, float y, Vec4 rgba):
        """
        Draw a translucent, color-modulated texture at (x, y). Lower-left origin.
        """
        gl_texture_blit_ex(self.texture, x, y, rgba.v); return self

    @staticmethod
    def draw_skybox(list textures, Coord3D coord):
        """
        Draw an infinitely-distant, fully bright textured cube around the viewer.
        """
        cdef Texture ft = textures[0]
        cdef Texture bk = textures[1]
        cdef Texture lf = textures[2]
        cdef Texture rt = textures[3]
        cdef Texture up = textures[4]
        cdef Texture dn = textures[5]

        gl_texture_draw_skybox( ft.texture, bk.texture,
                                lf.texture, rt.texture,
                                up.texture, dn.texture,
                                &coord.coord )

# ==============================================================================
# ~ [ material system ]
# ==============================================================================

cdef class Material:
    cdef gl_material_t* material

    def __init__(self, *args, **kwargs):
        if 'reference' in kwargs:
            self.material = <gl_material_t*>(<size_t>kwargs['reference'])
        else:
            self.material = gl_material_create()

            if args: self.name = args[0] # Material("name")
            for k, v in kwargs.items(): setattr(self, k, v)

    def __repr__(self):
        return "{}({})".format(self.__class__.__name__, self.status)

    def __hash__(self):
        return hash(self.address())

    def __richcmp__(self, Material other, int op):
        if   op == 0: return self.address() <  other.address()
        elif op == 1: return self.address() <= other.address()
        elif op == 2: return self.address() == other.address()
        elif op == 3: return self.address() != other.address()
        elif op == 4: return self.address() >  other.address()
        elif op == 5: return self.address() >= other.address()

        else: assert 0

    def __nonzero__(self):
        return self.open

    def __reduce__(self):
        raise TypeError('cannot pickle {}'.format(self))

    def __copy__(self):
        raise TypeError('cannot copy {}'.format(self))

    @staticmethod
    def count_all(): return gl_material_count_all()

    @classmethod
    def list_all(cls):
        """
        Gather references to all open, active, valid materials in a single list.
        """
        cdef gl_material_t* materials[8192]
        cdef int i
        cdef list objects = []

        if gl_material_count_all() > 8192:
            raise MemoryError("too many open materials for temp")

        gl_material_list_all(materials)

        for i in range(<int>gl_material_count_all()):
            objects.append(cls(reference = <size_t>materials[i]))

        return objects

    @staticmethod
    def close_all(): gl_material_close_all()

    property id:
        def __get__(self):
            return gl_material_get_int(self.material, GL_MATERIAL_PROPERTY_ID)

    property flags:
        def __get__(self):
            return gl_material_get_u64(self.material, GL_MATERIAL_PROPERTY_FLAGS)

        def __set__(self, u64 value):
            gl_material_set_u64(self.material, GL_MATERIAL_PROPERTY_FLAGS, value)

    property texture:
        def __get__(self):
            return Texture(reference=<size_t>gl_material_get_tex(self.material,
                                                GL_MATERIAL_PROPERTY_TEXTURE))

        def __set__(self, Texture value):
            gl_material_set_tex(self.material, GL_MATERIAL_PROPERTY_TEXTURE, value.texture)

    property owns_texture:
        def __get__(self):
            return gl_material_get_int(self.material, GL_MATERIAL_PROPERTY_OWNS_TEXTURE)

        def __set__(self, bint value):
            gl_material_set_int(self.material, GL_MATERIAL_PROPERTY_OWNS_TEXTURE, value)

    property diffuse:
        def __get__(self):
            cdef Vec3 v = Vec3()

            vec3copy(v.v, gl_material_get_vec(self.material, GL_MATERIAL_PROPERTY_DIFFUSE))
            return v

        def __set__(self, Vec3 value):
            gl_material_set_vec(self.material, GL_MATERIAL_PROPERTY_DIFFUSE, value.v)

    property ambient:
        def __get__(self):
            cdef Vec3 v = Vec3()

            vec3copy(v.v, gl_material_get_vec(self.material, GL_MATERIAL_PROPERTY_AMBIENT))
            return v

        def __set__(self, Vec3 value):
            gl_material_set_vec(self.material, GL_MATERIAL_PROPERTY_AMBIENT, value.v)

    property specular:
        def __get__(self):
            cdef Vec3 v = Vec3()

            vec3copy(v.v, gl_material_get_vec(self.material, GL_MATERIAL_PROPERTY_SPECULAR))
            return v

        def __set__(self, Vec3 value):
            gl_material_set_vec(self.material, GL_MATERIAL_PROPERTY_SPECULAR, value.v)

    property emission:
        def __get__(self):
            cdef Vec3 v = Vec3()

            vec3copy(v.v, gl_material_get_vec(self.material, GL_MATERIAL_PROPERTY_EMISSION))
            return v

        def __set__(self, Vec3 value):
            gl_material_set_vec(self.material, GL_MATERIAL_PROPERTY_EMISSION, value.v)

    property light_enabled:
        def __get__(self):
            return gl_material_get_int(self.material, GL_MATERIAL_PROPERTY_LIGHT_ENABLED)

        def __set__(self, bint value):
            gl_material_set_int(self.material, GL_MATERIAL_PROPERTY_LIGHT_ENABLED, value)

    property shininess:
        def __get__(self):
            return gl_material_get_flt(self.material, GL_MATERIAL_PROPERTY_SHININESS)

        def __set__(self, float value):
            gl_material_set_flt(self.material, GL_MATERIAL_PROPERTY_SHININESS, value)

    property opacity:
        def __get__(self):
            return gl_material_get_flt(self.material, GL_MATERIAL_PROPERTY_OPACITY)

        def __set__(self, float value):
            gl_material_set_flt(self.material, GL_MATERIAL_PROPERTY_OPACITY, value)

    property status:
        def __get__(self):
            return gl_material_get_str(self.material, GL_MATERIAL_PROPERTY_STATUS)

        def __set__(self, bytes value):
            gl_material_set_str(self.material, GL_MATERIAL_PROPERTY_STATUS, <char*>value)

    property path:
        def __get__(self):
            return gl_material_get_str(self.material, GL_MATERIAL_PROPERTY_PATH)

        def __set__(self, bytes value):
            gl_material_set_str(self.material, GL_MATERIAL_PROPERTY_PATH, <char*>value)

    property name:
        def __get__(self):
            return gl_material_get_str(self.material, GL_MATERIAL_PROPERTY_NAME)

        def __set__(self, bytes value):
            gl_material_set_str(self.material, GL_MATERIAL_PROPERTY_NAME, <char*>value)

    property open:
        def __get__(self):
            return gl_material_get_int(self.material, GL_MATERIAL_PROPERTY_OPEN)

        def __set__(self, bint value):
            gl_material_set_int(self.material, GL_MATERIAL_PROPERTY_OPEN, value)

    def address(self):
        return <size_t>self.material

    def cast(self):
        return self

    def close(self):
        gl_material_close(self.material); return self

    def set_all(self, Vec3 value):
        gl_material_set_all(self.material, value.v); return self

    def lerp(self, Material a, Material b, float t):
        gl_material_lerp(self.material, a.material, b.material, t); return self

    def apply_ex(self, Buffer buffer):
        gl_material_apply_ex(self.material, buffer.buffer); return self

    def apply(self):
        gl_material_apply(self.material); return self

# ==============================================================================
# ~ [ lighting system ]
# ==============================================================================

cdef class Light:
    cdef gl_light_t* light

    def __init__(self, **kwargs):
        if 'reference' in kwargs:
            self.light = <gl_light_t*>(<size_t>kwargs['reference'])
        else:
            self.light = gl_light_create() # set default properties
            for key, val in kwargs.items(): setattr(self, key, val)

    def __repr__(self):
        return "{}({})".format(self.__class__.__name__, self.status)

    def __hash__(self):
        return hash(self.address())

    def __richcmp__(self, Light other, int op):
        if   op == 0: return self.address() <  other.address()
        elif op == 1: return self.address() <= other.address()
        elif op == 2: return self.address() == other.address()
        elif op == 3: return self.address() != other.address()
        elif op == 4: return self.address() >  other.address()
        elif op == 5: return self.address() >= other.address()

        else: assert 0

    def __nonzero__(self):
        return self.open

    def __reduce__(self):
        raise TypeError('cannot pickle {}'.format(self))

    def __copy__(self):
        raise TypeError('cannot copy {}'.format(self))

    @staticmethod
    def count_all(): return gl_light_count_all()

    @classmethod
    def list_all(cls):
        """
        Gather references to all open, active, valid lights in a single list.
        """
        cdef gl_light_t* lights[8192]
        cdef int i
        cdef list objects = []

        if gl_light_count_all() > 8192:
            raise MemoryError("too many open lights for temp")

        gl_light_list_all(lights)

        for i in range(<int>gl_light_count_all()):
            objects.append(cls(reference = <size_t>lights[i]))

        return objects

    @staticmethod
    def close_all(): gl_light_close_all()

    property id:
        def __get__(self):
            return gl_light_get_int(self.light, GL_LIGHT_PROPERTY_ID)

    property ambient:
        def __get__(self):
            cdef Vec4 v = Vec4()

            vec4copy(v.v, gl_light_get_vec(self.light, GL_LIGHT_PROPERTY_AMBIENT))
            return v

        def __set__(self, Vec4 value):
            gl_light_set_vec(self.light, GL_LIGHT_PROPERTY_AMBIENT, value.v)

    property diffuse:
        def __get__(self):
            cdef Vec4 v = Vec4()

            vec4copy(v.v, gl_light_get_vec(self.light, GL_LIGHT_PROPERTY_DIFFUSE))
            return v

        def __set__(self, Vec4 value):
            gl_light_set_vec(self.light, GL_LIGHT_PROPERTY_DIFFUSE, value.v)

    property specular:
        def __get__(self):
            cdef Vec4 v = Vec4()

            vec4copy(v.v, gl_light_get_vec(self.light, GL_LIGHT_PROPERTY_SPECULAR))
            return v

        def __set__(self, Vec4 value):
            gl_light_set_vec(self.light, GL_LIGHT_PROPERTY_SPECULAR, value.v)

    property position:
        def __get__(self):
            cdef Vec4 v = Vec4()

            vec4copy(v.v, gl_light_get_vec(self.light, GL_LIGHT_PROPERTY_POSITION))
            return v

        def __set__(self, Vec4 value):
            gl_light_set_vec(self.light, GL_LIGHT_PROPERTY_POSITION, value.v)

    property spot_direction:
        def __get__(self):
            cdef Vec3 v = Vec3()

            vec3copy(v.v, gl_light_get_vec(self.light, GL_LIGHT_PROPERTY_SPOT_DIRECTION))
            return v

        def __set__(self, Vec3 value):
            gl_light_set_vec(self.light, GL_LIGHT_PROPERTY_SPOT_DIRECTION, value.v)

    property spot_exponent:
        def __get__(self):
            return gl_light_get_flt(self.light, GL_LIGHT_PROPERTY_SPOT_EXPONENT)

        def __set__(self, float value):
            gl_light_set_flt(self.light, GL_LIGHT_PROPERTY_SPOT_EXPONENT, value)

    property spot_cutoff:
        def __get__(self):
            return gl_light_get_flt(self.light, GL_LIGHT_PROPERTY_SPOT_CUTOFF)

        def __set__(self, float value):
            gl_light_set_flt(self.light, GL_LIGHT_PROPERTY_SPOT_CUTOFF, value)

    property constant_attenuation:
        def __get__(self):
            return gl_light_get_flt(self.light, GL_LIGHT_PROPERTY_CONSTANT_ATTENUATION)

        def __set__(self, float value):
            gl_light_set_flt(self.light, GL_LIGHT_PROPERTY_CONSTANT_ATTENUATION, value)

    property linear_attenuation:
        def __get__(self):
            return gl_light_get_flt(self.light, GL_LIGHT_PROPERTY_LINEAR_ATTENUATION)

        def __set__(self, float value):
            gl_light_set_flt(self.light, GL_LIGHT_PROPERTY_LINEAR_ATTENUATION, value)

    property quadratic_attenuation:
        def __get__(self):
            return gl_light_get_flt(self.light, GL_LIGHT_PROPERTY_QUADRATIC_ATTENUATION)

        def __set__(self, float value):
            gl_light_set_flt(self.light, GL_LIGHT_PROPERTY_QUADRATIC_ATTENUATION, value)

    property status:
        def __get__(self):
            return gl_light_get_str(self.light, GL_LIGHT_PROPERTY_STATUS)

        def __set__(self, bytes value):
            gl_light_set_str(self.light, GL_LIGHT_PROPERTY_STATUS, <char*>value)

    property name:
        def __get__(self):
            return gl_light_get_str(self.light, GL_LIGHT_PROPERTY_NAME)

        def __set__(self, bytes value):
            gl_light_set_str(self.light, GL_LIGHT_PROPERTY_NAME, <char*>value)

    property open:
        def __get__(self):
            return gl_light_get_int(self.light, GL_LIGHT_PROPERTY_OPEN)

        def __set__(self, bint value):
            gl_light_set_int(self.light, GL_LIGHT_PROPERTY_OPEN, value)

    def address(self):
        return <size_t>self.light

    def cast(self):
        return self

    def close(self):
        gl_light_close(self.light); return self

    def set_all(self, Vec4 value):
        gl_light_set_all(self.light, value.v); return self

    def lerp(self, Light a, Light b, float t):
        gl_light_lerp(self.light, a.light, b.light, t); return self

    @staticmethod
    def default_scene():
        """
        Create a standard ambient lighting setup (medium-soft white light).
        """
        gl_light_default_scene()

    @staticmethod
    def apply_all():
        """
        Set up light state for the current scene (call once per frame?)
        """
        gl_light_apply_all()

# ==============================================================================
# ~ [ vertex buffers ]
# ==============================================================================

cdef class Buffer:
    cdef gl_buffer_t* buffer

    def __init__(self, size_t reference=0):
        self.buffer = <gl_buffer_t*>reference

    def __repr__(self):
        return "{}({})".format(self.__class__.__name__, self.status)

    def __hash__(self):
        return hash(self.address())

    def __richcmp__(self, Buffer other, int op):
        if   op == 0: return self.address() <  other.address()
        elif op == 1: return self.address() <= other.address()
        elif op == 2: return self.address() == other.address()
        elif op == 3: return self.address() != other.address()
        elif op == 4: return self.address() >  other.address()
        elif op == 5: return self.address() >= other.address()

        else: assert 0

    def __nonzero__(self):
        return self.open

    def __reduce__(self):
        raise TypeError('cannot pickle {}'.format(self))

    def __copy__(self):
        raise TypeError('cannot copy {}'.format(self))

    @staticmethod
    def count_all(): return gl_buffer_count_all()

    @classmethod
    def list_all(cls):
        """
        Gather references to all open, active, valid buffers in a single list.
        """
        cdef gl_buffer_t* buffers[8192]
        cdef int i
        cdef list objects = []

        if gl_buffer_count_all() > 8192:
            raise MemoryError("too many open buffers for temp")

        gl_buffer_list_all(buffers)

        for i in range(<int>gl_buffer_count_all()):
            objects.append(cls(reference = <size_t>buffers[i]))

        return objects

    @staticmethod
    def close_all(): gl_buffer_close_all()

    property id:
        def __get__(self):
            return gl_buffer_get_int(self.buffer, GL_BUFFER_PROPERTY_ID)

    property aabbox:
        def __get__(self):
            cdef Vec3 minv = Vec3()
            cdef Vec3 maxv = Vec3()

            gl_buffer_get_aabbox(self.buffer, minv.v, maxv.v)
            return (minv, maxv)

    property vertex_format:
        def __get__(self):
            return gl_buffer_get_int(self.buffer, GL_BUFFER_PROPERTY_VERTEX_FORMAT)

        def __set__(self, int value):
            gl_buffer_set_int(self.buffer, GL_BUFFER_PROPERTY_VERTEX_FORMAT, value)

    property vertex_format_name:
        def __get__(self):
            return gl_buffer_get_str(self.buffer, GL_BUFFER_PROPERTY_VERTEX_FORMAT)

        def __set__(self, bytes value):
            gl_buffer_set_str(self.buffer, GL_BUFFER_PROPERTY_VERTEX_FORMAT, <char*>value)

    property index_type:
        def __get__(self):
            return gl_buffer_get_int(self.buffer, GL_BUFFER_PROPERTY_INDEX_TYPE)

        def __set__(self, int value):
            gl_buffer_set_int(self.buffer, GL_BUFFER_PROPERTY_INDEX_TYPE, value)

    property index_type_name:
        def __get__(self):
            return gl_buffer_get_str(self.buffer, GL_BUFFER_PROPERTY_INDEX_TYPE)

        def __set__(self, bytes value):
            gl_buffer_set_str(self.buffer, GL_BUFFER_PROPERTY_INDEX_TYPE, <char*>value)

    property vertex_count:
        def __get__(self):
            return gl_buffer_get_int(self.buffer, GL_BUFFER_PROPERTY_VERTEX_COUNT)

    property index_count:
        def __get__(self):
            return gl_buffer_get_int(self.buffer, GL_BUFFER_PROPERTY_INDEX_COUNT)

    property triangle_count:
        def __get__(self):
            return gl_buffer_get_int(self.buffer, GL_BUFFER_PROPERTY_TRIANGLE_COUNT)

    property static_storage:
        def __get__(self):
            return gl_buffer_get_int(self.buffer, GL_BUFFER_PROPERTY_STATIC_STORAGE)

        def __set__(self, bint value):
            gl_buffer_set_int(self.buffer, GL_BUFFER_PROPERTY_STATIC_STORAGE, value)

    property compute_aabbox:
        def __get__(self):
            return gl_buffer_get_int(self.buffer, GL_BUFFER_PROPERTY_COMPUTE_AABBOX)

        def __set__(self, bint value):
            gl_buffer_set_int(self.buffer, GL_BUFFER_PROPERTY_COMPUTE_AABBOX, value)

    property locked:
        def __get__(self):
            return gl_buffer_get_int(self.buffer, GL_BUFFER_PROPERTY_LOCKED)

    property smooth_shading:
        def __get__(self):
            return gl_buffer_get_int(self.buffer, GL_BUFFER_PROPERTY_SMOOTH_SHADING)

        def __set__(self, bint value):
            gl_buffer_set_int(self.buffer, GL_BUFFER_PROPERTY_SMOOTH_SHADING, value)

    property line_mode:
        def __get__(self):
            return gl_buffer_get_int(self.buffer, GL_BUFFER_PROPERTY_LINE_MODE)

        def __set__(self, bint value):
            gl_buffer_set_int(self.buffer, GL_BUFFER_PROPERTY_LINE_MODE, value)

    property status:
        def __get__(self):
            return gl_buffer_get_str(self.buffer, GL_BUFFER_PROPERTY_STATUS)

        def __set__(self, bytes value):
            gl_buffer_set_str(self.buffer, GL_BUFFER_PROPERTY_STATUS, <char*>value)

    property path:
        def __get__(self):
            return gl_buffer_get_str(self.buffer, GL_BUFFER_PROPERTY_PATH)

        def __set__(self, bytes value):
            gl_buffer_set_str(self.buffer, GL_BUFFER_PROPERTY_PATH, <char*>value)

    property name:
        def __get__(self):
            return gl_buffer_get_str(self.buffer, GL_BUFFER_PROPERTY_NAME)

        def __set__(self, bytes value):
            gl_buffer_set_str(self.buffer, GL_BUFFER_PROPERTY_NAME, <char*>value)

    property open:
        def __get__(self):
            return gl_buffer_get_int(self.buffer, GL_BUFFER_PROPERTY_OPEN)

        def __set__(self, bint value):
            gl_buffer_set_int(self.buffer, GL_BUFFER_PROPERTY_OPEN, value)

    def address(self):
        return <size_t>self.buffer

    def cast(self):
        return self

    def close(self):
        gl_buffer_close(self.buffer); return self

    def create_empty(self, **kwarg):
        if not self.open:
            self.buffer = gl_buffer_create_empty() # blank buffer
            for key, val in kwarg.items(): setattr(self, key, val)

        return self

    def create_from(self, TriangleVertexArray vertices, IndexArray indices,
                                                        bint copy, **kwarg):
        if not self.open:
            self.buffer = gl_buffer_create_from(&vertices.array,
                                                vertices.vertex_format,
                                                &indices.array,
                                                indices.index_type,
                                                copy)

            for key, val in kwarg.items(): setattr(self, key, val)

        return self

    def create_cylinder(self, int slices, int stacks, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_cylinder(slices, stacks)

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_torus(self, int slices, int stacks, float radius, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_torus(slices, stacks, radius)

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_parametric_sphere(self, int slices, int stacks, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_parametric_sphere(slices, stacks)

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_subdivided_sphere(self, int nsubdivisions, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_subdivided_sphere(nsubdivisions)

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_klein_bottle(self, int slices, int stacks, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_klein_bottle(slices, stacks)

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_trefoil_knot(self, int slices, int stacks, float radius, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_trefoil_knot(slices, stacks, radius)

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_hemisphere(self, int slices, int stacks, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_hemisphere(slices, stacks)

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_plane(self, int slices, int stacks, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_plane(slices, stacks)

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_icosahedron(self, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_icosahedron()

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_dodecahedron(self, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_dodecahedron()

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_octahedron(self, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_octahedron()

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_tetrahedron(self, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_tetrahedron()

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_cube(self, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_cube()

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_disk(self, float radius, int slices, Vec3 center, Vec3 normal, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_disk(radius, slices, center.v, normal.v)

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_rock(self, int seed, int nsubdivisions, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_rock(seed, nsubdivisions)

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def create_lsystem(self, bytes program, int slices, int max_depth, **kwargs):
        if not self.open:
            self.buffer = gl_buffer_create_lsystem(program, slices, max_depth)

            # set attributes
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def lock(self):
        cdef TriangleVertexArray vertices = TriangleVertexArray(self.vertex_format)
        cdef IndexArray indices = IndexArray(self.index_type)

        gl_buffer_lock(self.buffer, &vertices.array, &indices.array)
        return (vertices, indices)

    def unlock(self, TriangleVertexArray vertices, IndexArray indices):
        gl_buffer_unlock(self.buffer,
                        &vertices.array, vertices.vertex_format,
                        &indices.array, indices.index_type)
        return self

    def get_vertex(self, size_t which):
        cdef ae_vertex_format_t fmt = < ae_vertex_format_t >gl_buffer_get_int(
                                self.buffer, GL_BUFFER_PROPERTY_VERTEX_FORMAT)

        cdef object t = None
        cdef object c = None
        cdef object n = None
        cdef object p = None

        cdef size_t t_num = ae_vertex_format_texcoord_size[<size_t>fmt]
        cdef size_t c_num = ae_vertex_format_color_size[<size_t>fmt]
        cdef size_t n_num = ae_vertex_format_normal_size[<size_t>fmt]
        cdef size_t p_num = ae_vertex_format_position_size[<size_t>fmt]

        cdef float t_mem[4]
        cdef float c_mem[4]
        cdef float n_mem[4]
        cdef float p_mem[4]

        vec4zero(t_mem)
        vec4zero(c_mem)
        vec4zero(n_mem)
        vec4zero(p_mem)

        gl_buffer_get_vertex(self.buffer, which, t_mem, c_mem, n_mem, p_mem)

        if self.open:
            if t_num:
                if t_num == 2:
                    t = Vec2(); vec2copy(<float*>(<size_t>t.address()), t_mem)

                if t_num == 3:
                    t = Vec3(); vec3copy(<float*>(<size_t>t.address()), t_mem)

                if t_num == 4:
                    t = Vec4(); vec4copy(<float*>(<size_t>t.address()), t_mem)

            if c_num:
                if c_num == 2:
                    c = Vec2(); vec2copy(<float*>(<size_t>c.address()), c_mem)

                if c_num == 3:
                    c = Vec3(); vec3copy(<float*>(<size_t>c.address()), c_mem)

                if c_num == 4:
                    c = Vec4(); vec4copy(<float*>(<size_t>c.address()), c_mem)

            if n_num:
                if n_num == 2:
                    n = Vec2(); vec2copy(<float*>(<size_t>n.address()), n_mem)

                if n_num == 3:
                    n = Vec3(); vec3copy(<float*>(<size_t>n.address()), n_mem)

                if n_num == 4:
                    n = Vec4(); vec4copy(<float*>(<size_t>n.address()), n_mem)

            if p_num:
                if p_num == 2:
                    p = Vec2(); vec2copy(<float*>(<size_t>p.address()), p_mem)

                if p_num == 3:
                    p = Vec3(); vec3copy(<float*>(<size_t>p.address()), p_mem)

                if p_num == 4:
                    p = Vec4(); vec4copy(<float*>(<size_t>p.address()), p_mem)

        return (t, c, n, p)

    def set_vertex(self, size_t which, object tex, object col, object nrm, object pos):
        gl_buffer_set_vertex(self.buffer, which,
                NULL if tex is None else <float *>(<size_t>tex.address()),
                NULL if col is None else <float *>(<size_t>col.address()),
                NULL if nrm is None else <float *>(<size_t>nrm.address()),
                NULL if pos is None else <float *>(<size_t>pos.address()))

        return self

    def append_vertex(self, object tex, object col, object nrm, object pos):
        gl_buffer_append_vertex(self.buffer,
                NULL if tex is None else <float *>(<size_t>tex.address()),
                NULL if col is None else <float *>(<size_t>col.address()),
                NULL if nrm is None else <float *>(<size_t>nrm.address()),
                NULL if pos is None else <float *>(<size_t>pos.address()))

        return self

    def get_face(self, size_t which):
        cdef size_t v0 = 0
        cdef size_t v1 = 0
        cdef size_t v2 = 0

        gl_buffer_get_face(self.buffer, which, &v0, &v1, &v2)
        return (v0, v1, v2)

    def set_face(self, size_t which, size_t v0, size_t v1, size_t v2):
        gl_buffer_set_face(self.buffer, which, v0, v1, v2); return self

    def append_face(self, size_t v0, size_t v1, size_t v2):
        gl_buffer_append_face(self.buffer, v0, v1, v2); return self

    def merge(self, Buffer other, bint copy):
        gl_buffer_merge(self.buffer, other.buffer, copy); return self

    def translate(self, Vec3 v):
        gl_buffer_translate(self.buffer, v.v); return self

    def scale(self, Vec3 v):
        gl_buffer_scale(self.buffer, v.v); return self

    def lerp(self, Buffer a, Buffer b, float t):
        gl_buffer_lerp(self.buffer, a.buffer, b.buffer, t); return self

    def transform(self, Mat4x4 m, bint normalize=True):
        gl_buffer_transform(self.buffer, m.m, normalize); return self

    def center_at_origin(self):
        gl_buffer_center_at_origin(self.buffer); return self

    def unique_vertex_ratio(self, float epsilon=GL_DEFAULT_EPSILON):
        return gl_buffer_unique_vertex_ratio(self.buffer, epsilon)

    def weld(self, float epsilon=GL_DEFAULT_EPSILON):
        gl_buffer_weld(self.buffer, epsilon); return self

    def weld_dist2(self, float threshold2):
        gl_buffer_weld_dist2(self.buffer, threshold2); return self

    def unweld(self):
        gl_buffer_unweld(self.buffer); return self

    def shrink_indices(self):
        gl_buffer_shrink_indices(self.buffer); return self

    def vertex_cache_optimize(self):
        gl_buffer_vertex_cache_optimize(self.buffer); return self

    def vertex_cache_miss_ratio(self, int cache_size=16, bint FIFO=True):
        return gl_buffer_vertex_cache_miss_ratio(self.buffer, cache_size, FIFO)

    def intersect_ray3(self, Ray3 ray, float epsilon=GL_DEFAULT_EPSILON):
        cdef u32 hit_indices[3]
        cdef Vec3 hit_point = Vec3()
        cdef float t, u, v

        cdef int hit = gl_buffer_intersect_ray3(self.buffer, ray.p.v, ray.v.v,
                                hit_indices, hit_point.v, &t, &u, &v, epsilon)

        if hit: return ((hit_indices[0], hit_indices[1], hit_indices[2]),
                                                    hit_point, t, u, v)

    def depthsort(self, Vec3 p):
        gl_buffer_depthsort(self.buffer, p.v); return self

    def invert_faces(self):
        gl_buffer_invert_faces(self.buffer); return self

    def draw_ex(self, Material material):
        gl_buffer_draw_ex(self.buffer, material.material); return self

    def draw(self):
        gl_buffer_draw(self.buffer); return self

    def draw_normals_ex(self, Vec4 start_color, Vec4 end_color, float width=1.0):
        gl_buffer_draw_normals_ex(self.buffer, start_color.v, end_color.v, width)
        return self

    def draw_normals(self, Vec4 rgba):
        gl_buffer_draw_normals(self.buffer, rgba.v); return self

# ==============================================================================
# ~ [ particle system ]
# ==============================================================================

def Emitter(*args, **kwargs):
    """
    Convenience thing for porting an old game - define this for other classes?
    """
    return AutoObject(ParticleEmitter(*args, **kwargs))

cdef class ParticleEmitter:
    cdef gl_particle_emitter_t* emitter

    def __init__(self, *args, **kwargs):
        if 'reference' in kwargs:
            self.emitter = <gl_particle_emitter_t*>(<size_t>kwargs['reference'])
        else:
            self.emitter = gl_particle_emitter_create() # set props

            if args: self.name = args[0] # ParticleEmitter("name")
            for key, val in kwargs.items(): setattr(self, key, val)

    def __repr__(self):
        return "{}({})".format(self.__class__.__name__, self.status)

    def __hash__(self):
        return hash(self.address())

    def __richcmp__(self, ParticleEmitter other, int op):
        if   op == 0: return self.address() <  other.address()
        elif op == 1: return self.address() <= other.address()
        elif op == 2: return self.address() == other.address()
        elif op == 3: return self.address() != other.address()
        elif op == 4: return self.address() >  other.address()
        elif op == 5: return self.address() >= other.address()

        else: assert 0

    def __nonzero__(self):
        return self.open

    def __reduce__(self):
        raise TypeError('cannot pickle {}'.format(self))

    def __copy__(self):
        raise TypeError('cannot copy {}'.format(self))

    @staticmethod
    def count_all(): return gl_particle_emitter_count_all()

    @classmethod
    def list_all(cls):
        """
        Gather references to all open, active, valid particle emitters in a single list.
        """
        cdef gl_particle_emitter_t* emitters[8192]
        cdef int i
        cdef list objects = []

        if gl_particle_emitter_count_all() > 8192:
            raise MemoryError("too many open emitters for temp")

        gl_particle_emitter_list_all(emitters)

        for i in range(<int>gl_particle_emitter_count_all()):
            objects.append(cls(reference = <size_t>emitters[i]))

        return objects

    @staticmethod
    def close_all(): gl_particle_emitter_close_all()

    property id:
        def __get__(self):
            return gl_particle_emitter_get_int(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_ID)

    property aabbox:
        def __get__(self):
            cdef Vec3 minv = Vec3()
            cdef Vec3 maxv = Vec3()

            gl_particle_emitter_get_aabbox(self.emitter, minv.v, maxv.v)
            return (minv, maxv)

    property texture:
        def __get__(self):
            return Texture(reference=<size_t>gl_particle_emitter_get_tex(self.emitter,
                                                GL_PARTICLE_EMITTER_PROPERTY_TEXTURE))

        def __set__(self, Texture value):
            gl_particle_emitter_set_tex(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_TEXTURE,
                                                                        value.texture)

    property owns_texture:
        def __get__(self):
            return gl_particle_emitter_get_int(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_OWNS_TEXTURE)

        def __set__(self, bint value):
            gl_particle_emitter_set_int(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_OWNS_TEXTURE, value)

    property color:
        def __get__(self):
            cdef Vec4 v = Vec4()
            vec4copy(v.v, gl_particle_emitter_get_vec(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_COLOR))
            return v

        def __set__(self, Vec4 value):
            gl_particle_emitter_set_vec(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_COLOR, value.v)

    property min_point_size:
        def __get__(self):
            return gl_particle_emitter_get_flt(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_MIN_POINT_SIZE)

        def __set__(self, float value):
            gl_particle_emitter_set_flt(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_MIN_POINT_SIZE, value)

    property max_point_size:
        def __get__(self):
            return gl_particle_emitter_get_flt(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_MAX_POINT_SIZE)

        def __set__(self, float value):
            gl_particle_emitter_set_flt(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_MAX_POINT_SIZE, value)

    property position:
        def __get__(self):
            cdef Vec3 v = Vec3()
            vec3copy(v.v, gl_particle_emitter_get_vec(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_POSITION))
            return v

        def __set__(self, Vec3 value):
            gl_particle_emitter_set_vec(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_POSITION, value.v)

    property position_sigma:
        def __get__(self):
            cdef Vec3 v = Vec3()
            vec3copy(v.v, gl_particle_emitter_get_vec(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_POSITION_SIGMA))
            return v

        def __set__(self, Vec3 value):
            gl_particle_emitter_set_vec(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_POSITION_SIGMA, value.v)

    property velocity:
        def __get__(self):
            cdef Vec3 v = Vec3()
            vec3copy(v.v, gl_particle_emitter_get_vec(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_VELOCITY))
            return v

        def __set__(self, Vec3 value):
            gl_particle_emitter_set_vec(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_VELOCITY, value.v)

    property velocity_sigma:
        def __get__(self):
            cdef Vec3 v = Vec3()
            vec3copy(v.v, gl_particle_emitter_get_vec(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_VELOCITY_SIGMA))
            return v

        def __set__(self, Vec3 value):
            gl_particle_emitter_set_vec(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_VELOCITY_SIGMA, value.v)

    property gravity:
        def __get__(self):
            cdef Vec3 v = Vec3()
            vec3copy(v.v, gl_particle_emitter_get_vec(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_GRAVITY))
            return v

        def __set__(self, Vec3 value):
            gl_particle_emitter_set_vec(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_GRAVITY, value.v)

    property velgen_mode:
        def __get__(self):
            return gl_particle_emitter_get_str(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_VELGEN_MODE)

        def __set__(self, bytes value):
            gl_particle_emitter_set_str(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_VELGEN_MODE, <char*>value)

    property velgen_scale:
        def __get__(self):
            return gl_particle_emitter_get_flt(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_VELGEN_SCALE)

        def __set__(self, float value):
            gl_particle_emitter_set_flt(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_VELGEN_SCALE, value)

    property frozen:
        def __get__(self):
            return gl_particle_emitter_get_int(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_FROZEN)

        def __set__(self, bint value):
            gl_particle_emitter_set_int(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_FROZEN, value)

    property burst_size:
        def __get__(self):
            return gl_particle_emitter_get_int(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_BURST_SIZE)

        def __set__(self, int value):
            gl_particle_emitter_set_int(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_BURST_SIZE, value)

    property num_particles:
        def __get__(self):
            return gl_particle_emitter_get_int(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_NUM_PARTICLES)

        def __set__(self, int value):
            gl_particle_emitter_set_int(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_NUM_PARTICLES, value)

    property max_particles:
        def __get__(self):
            return gl_particle_emitter_get_int(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_MAX_PARTICLES)

        def __set__(self, int value):
            gl_particle_emitter_set_int(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_MAX_PARTICLES, value)

    property period:
        def __get__(self):
            return gl_particle_emitter_get_dbl(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_PERIOD)

        def __set__(self, double value):
            gl_particle_emitter_set_dbl(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_PERIOD, value)

    property status:
        def __get__(self):
            return gl_particle_emitter_get_str(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_STATUS) # repr

        def __set__(self, bytes value):
            gl_particle_emitter_set_str(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_STATUS, <char*>value)

    property name:
        def __get__(self):
            return gl_particle_emitter_get_str(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_NAME) # repr

        def __set__(self, bytes value):
            gl_particle_emitter_set_str(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_NAME, <char*>value)

    property open:
        def __get__(self):
            return gl_particle_emitter_get_int(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_OPEN)

        def __set__(self, bint value):
            gl_particle_emitter_set_int(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_OPEN, value)

    def address(self):
        return <size_t>self.emitter

    def cast(self):
        return self

    def close(self):
        gl_particle_emitter_close(self.emitter); return self

    def lock(self):
        """
        Access the underlying velocity and particle arrays for reading and writing.
        """
        cdef VertexArray vel_array = VertexArray(AE_N3F)
        cdef VertexArray particles = VertexArray(AE_V3F)

        gl_particle_emitter_lock(self.emitter, &vel_array.array, &particles.array)
        return (vel_array, particles)

    def unlock(self, VertexArray vel_array, VertexArray particles):
        gl_particle_emitter_unlock(self.emitter, &vel_array.array, &particles.array)
        return self

    def get_particle(self, size_t which):
        cdef Vec3 vel = Vec3()
        cdef Vec3 pos = Vec3()

        gl_particle_emitter_get(self.emitter, which, vel.v, pos.v)
        return (vel, pos)

    def set_particle(self, size_t which, Vec3 vel, Vec3 pos):
        gl_particle_emitter_set(self.emitter, which, vel.v, pos.v); return self

    def append_particle(self, Vec3 vel, Vec3 pos, bint expand):
        gl_particle_emitter_append(self.emitter, vel.v, pos.v, expand); return self

    def merge(self, ParticleEmitter other, bint copy):
        gl_particle_emitter_merge(self.emitter, other.emitter, copy); return self

    def translate(self, Vec3 v):
        gl_particle_emitter_translate(self.emitter, v.v); return self

    def scale(self, Vec3 v):
        gl_particle_emitter_scale(self.emitter, v.v); return self

    def lerp(self, ParticleEmitter a, ParticleEmitter b, float t):
        gl_particle_emitter_lerp(self.emitter, a.emitter, b.emitter, t); return self

    def transform(self, Mat4x4 m):
        gl_particle_emitter_transform(self.emitter, m.m); return self

    def center_at_origin(self):
        gl_particle_emitter_center_at_origin(self.emitter); return self

    def aabbox_limit(self, Vec3 minv, Vec3 maxv):
        """
        If any particle is outside the aabbox, translate back to a point inside the box.
        """
        gl_particle_emitter_aabbox_limit(self.emitter, minv.v, maxv.v); return self

    def aabbox_clamp(self, Vec3 minv, Vec3 maxv):
        """
        Clamp all particle positions to an aabbox (particles stick to box faces).
        """
        gl_particle_emitter_aabbox_clamp(self.emitter, minv.v, maxv.v); return self

    def aabbox_reset(self, Vec3 minv, Vec3 maxv):
        """
        Set the position of each particle to a random position on an aabbox's face.
        """
        gl_particle_emitter_aabbox_reset(self.emitter, minv.v, maxv.v); return self

    def sphere_limit(self, Sphere sphere):
        """
        If any particle is outside a sphere's bounds, translate to a point inside sphere.
        """
        gl_particle_emitter_sphere_limit(self.emitter, sphere.c.v, sphere.r); return self

    def sphere_clamp(self, Sphere sphere):
        """
        Clamp particle positions to the face of a sphere (creates a `snowglobe` effect).
        """
        gl_particle_emitter_sphere_clamp(self.emitter, sphere.c.v, sphere.r); return self

    def sphere_reset(self, Sphere sphere):
        """
        Set the position of each particle to a random position on a sphere's face.
        """
        gl_particle_emitter_sphere_reset(self.emitter, sphere.c.v, sphere.r); return self

    def radius_limit(self, float radius):
        """
        Same as sphere_limit, but the center of the sphere is the emitter's position.
        """
        gl_particle_emitter_radius_limit(self.emitter, radius); return self

    def radius_clamp(self, float radius):
        """
        Same as sphere_clamp, but the center of the sphere is the emitter's position.
        """
        gl_particle_emitter_radius_clamp(self.emitter, radius); return self

    def radius_reset(self, float radius):
        """
        Same as sphere_reset, but the center of the sphere is the emitter's position.
        """
        gl_particle_emitter_radius_reset(self.emitter, radius); return self

    def line_reset(self, Vec3 minv, Vec3 maxv):
        """
        Set the position of each particle to a random position on a line segment.
        """
        gl_particle_emitter_line_reset(self.emitter, minv.v, maxv.v); return self

    @staticmethod
    def reset_all():
        gl_particle_emitter_reset_all()

    def reset(self):
        gl_particle_emitter_reset(self.emitter); return self

    @staticmethod
    def fill_all():
        gl_particle_emitter_fill_all()

    def fill(self):
        gl_particle_emitter_fill(self.emitter); return self

    @staticmethod
    def update_all(double dt):
        gl_particle_emitter_update_all(dt)

    def update(self, double dt):
        gl_particle_emitter_update(self.emitter, dt); return self

    @staticmethod
    def draw_all():
        gl_particle_emitter_draw_all()

    def draw(self):
        gl_particle_emitter_draw(self.emitter); return self

    def draw_velocity_ex(self, Vec4 start_color, Vec4 end_color, float width=1.0):
        """
        Draw the velocity of each particle as a line. Mostly for debugging.
        """
        gl_particle_emitter_draw_velocity_ex(self.emitter, start_color.v,
                                        end_color.v, width); return self

    def draw_velocity(self, Vec4 rgba):
        gl_particle_emitter_draw_velocity(self.emitter, rgba.v); return self
