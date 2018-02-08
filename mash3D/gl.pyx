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

import sys # version info

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

    void GL_Accum(unsigned int op, float value)
    void GL_ActiveShaderProgram(unsigned int pipeline, unsigned int program)
    void GL_ActiveTexture(unsigned int texture)
    void GL_AlphaFunc(unsigned int func, float ref)
    unsigned char GL_AreTexturesResident(int, const unsigned int*, unsigned char*)
    void GL_ArrayElement(int i)
    void GL_AttachShader(unsigned int program, unsigned int shader)
    void GL_Begin(unsigned int mode)
    void GL_BeginConditionalRender(unsigned int id, unsigned int mode)
    void GL_BeginQuery(unsigned int target, unsigned int id)
    void GL_BeginQueryIndexed(unsigned int target, unsigned int index, unsigned int id)
    void GL_BeginTransformFeedback(unsigned int primitiveMode)
    void GL_BindAttribLocation(unsigned int program, unsigned int index, const char *name)
    void GL_BindBuffer(unsigned int target, unsigned int buf)
    void GL_BindTexture(unsigned int target, unsigned int texture)
    void GL_BindVertexArray(unsigned int array)
    void GL_Bitmap(int, int, float, float, float, float, const unsigned char*)
    void GL_BlendColor(float red, float green, float blue, float alpha)
    void GL_BlendEquation(unsigned int mode)
    void GL_BlendEquationSeparate(unsigned int modeRGB, unsigned int modeA)
    void GL_BlendFunc(unsigned int src, unsigned int dst)
    void GL_BlendFuncSeparate(unsigned int, unsigned int, unsigned int, unsigned int)
    void GL_BufferData(unsigned int, ptrdiff_t, const void*, unsigned int)
    void GL_CallList(unsigned int list)
    void GL_CallLists(int n, unsigned int type, const void *lists)
    void GL_Clear(unsigned int mask)
    void GL_ClearAccum(float r, float g, float b, float a)
    void GL_ClearColor(float r, float g, float b, float a)
    void GL_ClearDepth(double depth)
    void GL_ClearIndex(float c)
    void GL_ClearStencil(int s)
    void GL_ClipPlane(unsigned int plane, const double *equation)
    void GL_Color3b(char red, char green, char blue)
    void GL_Color3bv(const char *v)
    void GL_Color3d(double red, double green, double blue)
    void GL_Color3dv(const double *v)
    void GL_Color3f(float r, float g, float b)
    void GL_Color3fv(const float* rgb)
    void GL_Color3i(int red, int green, int blue)
    void GL_Color3iv(const int *v)
    void GL_Color3s(short red, short green, short blue)
    void GL_Color3sv(const short *v)
    void GL_Color3ub(unsigned char red, unsigned char green, unsigned char blue)
    void GL_Color3ubv(const unsigned char *v)
    void GL_Color3ui(unsigned int red, unsigned int green, unsigned int blue)
    void GL_Color3uiv(const unsigned int *v)
    void GL_Color3us(unsigned short red, unsigned short green, unsigned short blue)
    void GL_Color3usv(const unsigned short *v)
    void GL_Color4b(char red, char green, char blue, char alpha)
    void GL_Color4bv(const char *v)
    void GL_Color4d(double red, double green, double blue, double alpha)
    void GL_Color4dv(const double *v)
    void GL_Color4f(float r, float g, float b, float a)
    void GL_Color4fv(const float* rgba)
    void GL_Color4i(int red, int green, int blue, int alpha)
    void GL_Color4iv(const int *v)
    void GL_Color4s(short red, short green, short blue, short alpha)
    void GL_Color4sv(const short *v)
    void GL_Color4ub(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
    void GL_Color4ubv(const unsigned char *v)
    void GL_Color4ui(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha)
    void GL_Color4uiv(const unsigned int *v)
    void GL_Color4us(unsigned short red, unsigned short green, unsigned short blue, unsigned short alpha)
    void GL_Color4usv(const unsigned short *v)
    void GL_ColorMask(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    void GL_ColorMaterial(unsigned int face, unsigned int mode)
    void GL_ColorPointer(int size, unsigned int type, int stride, const void* data)
    void GL_CompileShader(unsigned int shader)
    void GL_CopyPixels(int x, int y, int width, int height, unsigned int type)

    void GL_CopyTexImage1D(unsigned int target, int level, unsigned int internal_format,
                                                    int x, int y, int width, int border)

    void GL_CopyTexImage2D(unsigned int target, int level, unsigned int internal_format,
                                        int x, int y, int width, int height, int border)

    void GL_CopyTexSubImage1D(unsigned int target, int level, int xoffset,
                                                int x, int y, int width)

    void GL_CopyTexSubImage2D(unsigned int target, int level, int xoffset,
                        int yoffset, int x, int y, int width, int height)

    unsigned int GL_CreateProgram()
    unsigned int GL_CreateShader(unsigned int type)
    void GL_CullFace(unsigned int face)
    void GL_DeleteBuffers(int n, const unsigned int* buffers)
    void GL_DeleteLists(unsigned int list, int range)
    void GL_DeleteProgram(unsigned int program)
    void GL_DeleteTextures(int n, const unsigned int* textures)
    void GL_DepthFunc(unsigned int func)
    void GL_DeleteVertexArrays(int n, const unsigned int *arrays)
    void GL_DepthMask(unsigned char flag)
    void GL_DepthRange(double near_val, double far_val)
    void GL_Disable(unsigned int cap)
    void GL_DisableClientState(unsigned int cap)
    void GL_DisableVertexAttribArray(unsigned int index)
    void GL_DrawArrays(unsigned int mode, int first, int count)
    void GL_DrawBuffer(unsigned int mode)
    void GL_DrawElements(unsigned int mode, int n, unsigned int type, const void* inds)
    void GL_DrawPixels(int w, int h, unsigned int fmt, unsigned int type_id, const void *data)
    void GL_EdgeFlag(unsigned char flag)
    void GL_EdgeFlagPointer(int stride, const void *ptr)
    void GL_EdgeFlagv(const unsigned char *flag)
    void GL_Enable(unsigned int cap)
    void GL_EnableClientState(unsigned int cap)
    void GL_EnableVertexAttribArray(unsigned int index)
    void GL_End()
    void GL_EndConditionalRender()
    void GL_EndList()
    void GL_EndQuery(unsigned int target)
    void GL_EndQueryIndexed(unsigned int target, unsigned int index)
    void GL_EndTransformFeedback()
    void GL_EvalCoord1d(double u)
    void GL_EvalCoord1dv(const double *u)
    void GL_EvalCoord1f(float u)
    void GL_EvalCoord1fv(const float *u)
    void GL_EvalCoord2d(double u, double v)
    void GL_EvalCoord2dv(const double *u)
    void GL_EvalCoord2f(float u, float v)
    void GL_EvalCoord2fv(const float *u)
    void GL_EvalMesh1(unsigned int mode, int i1, int i2)
    void GL_EvalMesh2(unsigned int mode, int i1, int i2, int j1, int j2)
    void GL_EvalPoint1(int i)
    void GL_EvalPoint2(int i, int j)
    void GL_FeedbackBuffer(int size, unsigned int type, float *buffer)
    void GL_Finish()
    void GL_Flush()
    void GL_Fogfv(unsigned int pname, const float* params)
    void GL_Fogiv(unsigned int pname, const int* params)
    void GL_Fogf(unsigned int pname, float param)
    void GL_Fogi(unsigned int pname, int param)
    void GL_FrontFace(unsigned int mode)
    void GL_Frustum(double lf, double rt, double bt, double tp, double nr, double fr)
    void GL_GenBuffers(int n, unsigned int *buffers)
    unsigned int GL_GenLists(int range)
    void GL_GenTextures(int n, unsigned int* textures)
    void GL_GenVertexArrays(int n, unsigned int *arrays)
    int GL_GetAttribLocation(unsigned int program, const char *name)
    void GL_GetBooleanv(unsigned int pname, unsigned char *params)
    void GL_GetClipPlane(unsigned int plane, double *equation)
    void GL_GetDoublev(unsigned int pname, double *params)
    unsigned int GL_GetError()
    void GL_GetFloatv(unsigned int pname, float* data)
    void GL_GetIntegerv(unsigned int pname, int* data)
    void GL_GetLightfv(unsigned int light, unsigned int pname, float* params)
    void GL_GetLightiv(unsigned int light, unsigned int pname, int* params)
    void GL_GetMapdv(unsigned int target, unsigned int query, double *v)
    void GL_GetMapfv(unsigned int target, unsigned int query, float *v)
    void GL_GetMapiv(unsigned int target, unsigned int query, int *v)
    void GL_GetMaterialfv(unsigned int face, unsigned int pname, float* params)
    void GL_GetMaterialiv(unsigned int face, unsigned int pname, int* params)
    void GL_GetPixelMapfv(unsigned int map, float *values)
    void GL_GetPixelMapuiv(unsigned int map, unsigned int *values)
    void GL_GetPixelMapusv(unsigned int map, unsigned short *values)
    void GL_GetPointerv(unsigned int pname, void **params)
    void GL_GetPolygonStipple(unsigned char *mask)
    void GL_GetProgramiv(unsigned int program, unsigned int pname, int *params)
    void GL_GetProgramInfoLog(unsigned int program, int buf_size, int *length, char *info_log)
    void GL_GetShaderiv(unsigned int shader, unsigned int pname, int *params)
    void GL_GetShaderInfoLog(unsigned int shader, int buf_size, int *length, char *info_log)
    const char* GL_GetString(unsigned int name)
    void GL_GetTexEnvfv(unsigned int target, unsigned int pname, float* params)
    void GL_GetTexEnviv(unsigned int target, unsigned int pname, int* params)
    void GL_GetTexGendv(unsigned int coord, unsigned int pname, double* params)
    void GL_GetTexGenfv(unsigned int coord, unsigned int pname, float* params)
    void GL_GetTexGeniv(unsigned int coord, unsigned int pname, int* params)
    void GL_GetTexParameterfv(unsigned int target, unsigned int pname, float* params)
    void GL_GetTexParameteriv(unsigned int target, unsigned int pname, int* params)
    void GL_Hint(unsigned int target, unsigned int mode)
    void GL_InterleavedArrays(unsigned int format, int stride, const void* pointer)
    unsigned char GL_IsBuffer(unsigned int buffer_id)
    unsigned char GL_IsEnabled(unsigned int cap)
    unsigned char GL_IsList(unsigned int list_id)
    unsigned char GL_IsProgram(unsigned int program_id)
    unsigned char GL_IsShader(unsigned int shader_id)
    unsigned char GL_IsTexture(unsigned int texture_id)
    unsigned char GL_IsVertexArray(unsigned int array_id)
    void GL_Lightfv(unsigned int light, unsigned int pname, const float* params)
    void GL_Lightf(unsigned int light, unsigned int pname, float param)
    void GL_LineWidth(float width)
    void GL_LinkProgram(unsigned int program)
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
    void GL_PrioritizeTextures(int n, const unsigned int* tex, const float* priorities)
    void GL_ProvokingVertex(unsigned int mode)
    int GL_RenderMode(unsigned int mode)
    void GL_Rotatef(float theta, float x, float y, float z)
    void GL_Scalef(float x, float y, float z)
    void GL_Scissor(int x, int y, int width, int height)
    void GL_ShadeModel(unsigned int mode)

    #
    # FIXME: cython doesn't like this string argument - google around for a solution!
    #
    #void GL_ShaderSource(unsigned int shader, int count, const char** const string,
    #                                                            const int * length)

    void GL_TexCoord2f(float s, float t)
    void GL_TexCoord2fv(const float* st)
    void GL_TexCoord3f(float s, float t, float r)
    void GL_TexCoord3fv(const float* str)
    void GL_TexCoord4f(float s, float t, float r, float q)
    void GL_TexCoord4fv(const float* strq)
    void GL_TexCoordPointer(int size, unsigned int type, int stride, const void* data)
    void GL_TexEnvfv(unsigned int target, unsigned int pname, const float *params)
    void GL_TexEnviv(unsigned int target, unsigned int pname, const int *params)
    void GL_TexEnvf(unsigned int target, unsigned int pname, float param)
    void GL_TexEnvi(unsigned int target, unsigned int pname, int param)
    void GL_TexGeniv(unsigned int coord, unsigned int pname, const int *params)
    void GL_TexGenfv(unsigned int coord, unsigned int pname, const float *params)
    void GL_TexGendv(unsigned int coord, unsigned int pname, const double *params)
    void GL_TexGeni(unsigned int coord, unsigned int pname, int param)
    void GL_TexGenf(unsigned int coord, unsigned int pname, float param)
    void GL_TexGend(unsigned int coord, unsigned int pname, double param)

    void GL_TexImage2D(unsigned int target, int level, int internal_format, int w,
            int h, int border, unsigned int fmt, unsigned int type, const void* p)

    void GL_TexParameteri(unsigned int target, unsigned int pname, int param)

    void GL_TexSubImage2D(unsigned int target, int level, int xoffset, int yoffset,
            int w, int h, unsigned int format, unsigned int type, const void* pix)

    void GL_Translatef(float x, float y, float z)
    void GL_UseProgram(unsigned int program)
    void GL_Vertex2f(float x, float y)
    void GL_Vertex2fv(const float* xy)
    void GL_Vertex3f(float x, float y, float z)
    void GL_Vertex3fv(const float* xyz)
    void GL_Vertex4f(float x, float y, float z, float w)
    void GL_Vertex4fv(const float* xyzw)

    void GL_VertexAttribPointer(unsigned int index, int size, unsigned int type,
                        unsigned char normalized, int stride, const void * data)

    void GL_VertexPointer(int size, unsigned int type, int stride, const void* data)
    void GL_Viewport(int x, int y, int width, int height)

    # ==========================================================================
    # ~ [ utilities ]
    # ==========================================================================

    void gl_enter3D(int width, int height, double FOV, double znear, double zfar)
    void gl_leave3D()

    void gl_enter2D(int width, int height)
    void gl_leave2D()

    # ===== [ 2D shapes ] ======================================================

    void gl_rect_ex(float* rect, const float* bl, const float* br, const float* tr,
                                const float* tl, int is_outline, float line_width)

    void gl_outline(float* rect, const float* rgba)
    void gl_rect(float* rect, const float* rgba)

    # ===== [ 3D shapes ] ======================================================

    void gl_aabbox_ex(float* minv, float* maxv, const float* rgba, float width)
    void gl_aabbox(float* minv, float* maxv, const float* rgba)

    void gl_ellipsoid(float* center, float* extent, const float* rgba)
    void gl_sphere(float* center, float radius, const float* rgba)

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

    void gl_object_print_all()
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

    void gl_texture_draw(gl_texture_t* texture, float x, float y) # blit
    void gl_texture_draw_ex(gl_texture_t*, float, float, const float[4])

    void gl_texture_draw_skybox_ex( gl_texture_t* front,
                                    gl_texture_t* back,
                                    gl_texture_t* left,
                                    gl_texture_t* right,
                                    gl_texture_t* up,
                                    gl_texture_t* down,
                                    gl_coord3D_t* viewer,
                                    const float color[4])

    void gl_texture_draw_skybox(gl_texture_t* front,
                                gl_texture_t* back,
                                gl_texture_t* left,
                                gl_texture_t* right,
                                gl_texture_t* up,
                                gl_texture_t* down,
                                gl_coord3D_t* viewer)

    size_t gl_texture_count_all()
    void gl_texture_list_all(gl_texture_t** textures)

    void gl_texture_print_all()
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
    void gl_material_set_all(gl_material_t* material, const float* value)

    # interpolate A and B properties into a destination material (for animation)
    void gl_material_lerp(gl_material_t*, gl_material_t*, gl_material_t*, float)

    void gl_material_apply_ex(gl_material_t* material, gl_buffer_t* buffer)
    void gl_material_apply(gl_material_t* material)

    size_t gl_material_count_all()
    void gl_material_list_all(gl_material_t** materials)

    void gl_material_print_all()
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
        GL_LIGHT_PROPERTY_PATH
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
    void gl_light_set_all(gl_light_t* light, const float* value)

    # interpolate the properties of lights A and B into the destination light
    void gl_light_lerp(gl_light_t* light, gl_light_t* a, gl_light_t* b, float t)

    # create a standard ambient lighting setup
    void gl_light_default_scene()

    # hardware light state for the current scene
    void gl_light_apply_all()

    size_t gl_light_count_all()
    void gl_light_list_all(gl_light_t** lights)

    void gl_light_print_all()
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

    void gl_buffer_draw_aabbox_ex(gl_buffer_t* buffer, const float* rgba,
                                                        float line_width)

    void gl_buffer_draw_aabbox(gl_buffer_t* buffer, const float* rgba)

    void gl_buffer_draw_normals_ex(gl_buffer_t* buffer, const float* start_color,
                                        const float* end_color, float line_width)

    void gl_buffer_draw_normals(gl_buffer_t* buffer, const float* color)

    size_t gl_buffer_count_all()
    void gl_buffer_list_all(gl_buffer_t** buffers)

    void gl_buffer_print_all()
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
        GL_PARTICLE_EMITTER_PROPERTY_PATH
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

    void gl_particle_emitter_mesh_cover_face(gl_particle_emitter_t* emitter,
                        gl_buffer_t* buffer, size_t particles_per_triangle)

    void gl_particle_emitter_mesh_same_edges(gl_particle_emitter_t* emitter,
                        gl_buffer_t* buffer, size_t particles_per_triangle)

    void gl_particle_emitter_mesh_rand_edges(gl_particle_emitter_t* emitter,
                        gl_buffer_t* buffer, size_t particles_per_triangle)

    void gl_particle_emitter_mesh_given_edge(gl_particle_emitter_t * emitter,
            gl_buffer_t* buffer, size_t particles_per_triangle, size_t index)

    void gl_particle_emitter_mesh_rad_vertex(gl_particle_emitter_t * emitter,
            gl_buffer_t* buffer, size_t particles_per_triangle, float radius)

    void gl_particle_emitter_reset_all()
    void gl_particle_emitter_reset(gl_particle_emitter_t* emitter)

    void gl_particle_emitter_fill_all()
    void gl_particle_emitter_fill(gl_particle_emitter_t* emitter)

    void gl_particle_emitter_update_all(double dt)
    void gl_particle_emitter_update(gl_particle_emitter_t* emitter, double dt)

    void gl_particle_emitter_draw_all()
    void gl_particle_emitter_draw(gl_particle_emitter_t* emitter)

    void gl_particle_emitter_draw_aabbox_ex(gl_particle_emitter_t * emitter,
                                        const float* rgba, float line_width)

    void gl_particle_emitter_draw_aabbox(gl_particle_emitter_t* emitter,
                                                    const float* color)

    void gl_particle_emitter_draw_velocity_ex( gl_particle_emitter_t * emitter,
            const float* start_color, const float* end_color, float line_width)

    void gl_particle_emitter_draw_velocity(gl_particle_emitter_t* emitter,
                                                        const float* rgba)

    size_t gl_particle_emitter_count_all()
    void gl_particle_emitter_list_all(gl_particle_emitter_t** elist)

    void gl_particle_emitter_print_all()
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
    if sys.version_info.major > 2:
        return gl_implementation().decode() # auto-convert to unicode string
    else:
        return gl_implementation() # auto-convert to oldschool ascii string

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
    # points
    #
    POINT_SMOOTH           = 0x0B10
    POINT_SIZE             = 0x0B11
    POINT_SIZE_GRANULARITY = 0x0B13
    POINT_SIZE_RANGE       = 0x0B12

    # --------------------------------------
    # lines
    #
    LINE_SMOOTH            = 0x0B20
    LINE_STIPPLE           = 0x0B24
    LINE_STIPPLE_PATTERN   = 0x0B25
    LINE_STIPPLE_REPEAT    = 0x0B26
    LINE_WIDTH             = 0x0B21
    LINE_WIDTH_GRANULARITY = 0x0B23
    LINE_WIDTH_RANGE       = 0x0B22

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
    # display lists
    #
    COMPILE             = 0x1300
    COMPILE_AND_EXECUTE = 0x1301
    LIST_BASE           = 0x0B32
    LIST_INDEX          = 0x0B33
    LIST_MODE           = 0x0B30

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
    # user clipping planes
    #
    CLIP_PLANE0 = 0x3000
    CLIP_PLANE1 = 0x3001
    CLIP_PLANE2 = 0x3002
    CLIP_PLANE3 = 0x3003
    CLIP_PLANE4 = 0x3004
    CLIP_PLANE5 = 0x3005

    # --------------------------------------
    # accumulation buffer
    #
    ACCUM_RED_BITS    = 0x0D58
    ACCUM_GREEN_BITS  = 0x0D59
    ACCUM_BLUE_BITS   = 0x0D5A
    ACCUM_ALPHA_BITS  = 0x0D5B
    ACCUM_CLEAR_VALUE = 0x0B80
    ACCUM             = 0x0100
    ADD               = 0x0104
    LOAD              = 0x0101
    MULT              = 0x0103
    RETURN            = 0x0102

    # --------------------------------------
    # alpha testing
    #
    ALPHA_TEST      = 0x0BC0
    ALPHA_TEST_REF  = 0x0BC2
    ALPHA_TEST_FUNC = 0x0BC1

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
    # render mode
    #
    FEEDBACK = 0x1C01
    RENDER   = 0x1C00
    SELECT   = 0x1C02

    # --------------------------------------
    # feedback (underscores for numbers)
    #
    _2D                     = 0x0600
    _3D                     = 0x0601
    _3D_COLOR               = 0x0602
    _3D_COLOR_TEXTURE       = 0x0603
    _4D_COLOR_TEXTURE       = 0x0604
    POINT_TOKEN             = 0x0701
    LINE_TOKEN              = 0x0702
    LINE_RESET_TOKEN        = 0x0707
    POLYGON_TOKEN           = 0x0703
    BITMAP_TOKEN            = 0x0704
    DRAW_PIXEL_TOKEN        = 0x0705
    COPY_PIXEL_TOKEN        = 0x0706
    PASS_THROUGH_TOKEN      = 0x0700
    FEEDBACK_BUFFER_POINTER = 0x0DF0
    FEEDBACK_BUFFER_SIZE    = 0x0DF1
    FEEDBACK_BUFFER_TYPE    = 0x0DF2

    # --------------------------------------
    # selection
    #
    SELECTION_BUFFER_POINTER = 0x0DF3
    SELECTION_BUFFER_SIZE    = 0x0DF4

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
    # buffers, pixel drawing / reading
    #
    NONE            = 0x0000
    LEFT            = 0x0406
    RIGHT           = 0x0407
    FRONT_LEFT      = 0x0400
    FRONT_RIGHT     = 0x0401
    BACK_LEFT       = 0x0402
    BACK_RIGHT      = 0x0403
    AUX0            = 0x0409
    AUX1            = 0x040A
    AUX2            = 0x040B
    AUX3            = 0x040C
    COLOR_INDEX     = 0x1900
    RED             = 0x1903
    GREEN           = 0x1904
    BLUE            = 0x1905
    ALPHA           = 0x1906
    LUMINANCE       = 0x1909
    LUMINANCE_ALPHA = 0x190A
    ALPHA_BITS      = 0x0D55
    RED_BITS        = 0x0D52
    GREEN_BITS      = 0x0D53
    BLUE_BITS       = 0x0D54
    INDEX_BITS      = 0x0D51
    SUBPIXEL_BITS   = 0x0D50
    AUX_BUFFERS     = 0x0C00
    READ_BUFFER     = 0x0C02
    DRAW_BUFFER     = 0x0C01
    DOUBLEBUFFER    = 0x0C32
    STEREO          = 0x0C33
    BITMAP          = 0x1A00
    COLOR           = 0x1800
    DEPTH           = 0x1801
    STENCIL         = 0x1802
    DITHER          = 0x0BD0
    RGB             = 0x1907
    RGBA            = 0x1908

    # --------------------------------------
    # implementation limits
    #
    MAX_LIST_NESTING              = 0x0B31
    MAX_EVAL_ORDER                = 0x0D30
    MAX_LIGHTS                    = 0x0D31
    MAX_CLIP_PLANES               = 0x0D32
    MAX_TEXTURE_SIZE              = 0x0D33
    MAX_PIXEL_MAP_TABLE           = 0x0D34
    MAX_ATTRIB_STACK_DEPTH        = 0x0D35
    MAX_MODELVIEW_STACK_DEPTH     = 0x0D36
    MAX_NAME_STACK_DEPTH          = 0x0D37
    MAX_PROJECTION_STACK_DEPTH    = 0x0D38
    MAX_TEXTURE_STACK_DEPTH       = 0x0D39
    MAX_VIEWPORT_DIMS             = 0x0D3A
    MAX_CLIENT_ATTRIB_STACK_DEPTH = 0x0D3B

    # --------------------------------------
    # gets
    #
    ATTRIB_STACK_DEPTH            = 0x0BB0
    CLIENT_ATTRIB_STACK_DEPTH     = 0x0BB1
    COLOR_CLEAR_VALUE             = 0x0C22
    COLOR_WRITEMASK               = 0x0C23
    CURRENT_INDEX                 = 0x0B01
    CURRENT_COLOR                 = 0x0B00
    CURRENT_NORMAL                = 0x0B02
    CURRENT_RASTER_COLOR          = 0x0B04
    CURRENT_RASTER_DISTANCE       = 0x0B09
    CURRENT_RASTER_INDEX          = 0x0B05
    CURRENT_RASTER_POSITION       = 0x0B07
    CURRENT_RASTER_TEXTURE_COORDS = 0x0B06
    CURRENT_RASTER_POSITION_VALID = 0x0B08
    CURRENT_TEXTURE_COORDS        = 0x0B03
    INDEX_CLEAR_VALUE             = 0x0C20
    INDEX_MODE                    = 0x0C30
    INDEX_WRITEMASK               = 0x0C21
    MODELVIEW_MATRIX              = 0x0BA6
    MODELVIEW_STACK_DEPTH         = 0x0BA3
    NAME_STACK_DEPTH              = 0x0D70
    PROJECTION_MATRIX             = 0x0BA7
    PROJECTION_STACK_DEPTH        = 0x0BA4
    RENDER_MODE                   = 0x0C40
    RGBA_MODE                     = 0x0C31
    TEXTURE_MATRIX                = 0x0BA8
    TEXTURE_STACK_DEPTH           = 0x0BA5
    VIEWPORT                      = 0x0BA2

    # --------------------------------------
    # evaluators
    #
    AUTO_NORMAL          = 0x0D80
    MAP1_COLOR_4         = 0x0D90
    MAP1_INDEX           = 0x0D91
    MAP1_NORMAL          = 0x0D92
    MAP1_TEXTURE_COORD_1 = 0x0D93
    MAP1_TEXTURE_COORD_2 = 0x0D94
    MAP1_TEXTURE_COORD_3 = 0x0D95
    MAP1_TEXTURE_COORD_4 = 0x0D96
    MAP1_VERTEX_3        = 0x0D97
    MAP1_VERTEX_4        = 0x0D98
    MAP2_COLOR_4         = 0x0DB0
    MAP2_INDEX           = 0x0DB1
    MAP2_NORMAL          = 0x0DB2
    MAP2_TEXTURE_COORD_1 = 0x0DB3
    MAP2_TEXTURE_COORD_2 = 0x0DB4
    MAP2_TEXTURE_COORD_3 = 0x0DB5
    MAP2_TEXTURE_COORD_4 = 0x0DB6
    MAP2_VERTEX_3        = 0x0DB7
    MAP2_VERTEX_4        = 0x0DB8
    MAP1_GRID_DOMAIN     = 0x0DD0
    MAP1_GRID_SEGMENTS   = 0x0DD1
    MAP2_GRID_DOMAIN     = 0x0DD2
    MAP2_GRID_SEGMENTS   = 0x0DD3
    COEFF                = 0x0A00
    ORDER                = 0x0A01
    DOMAIN               = 0x0A02

    # --------------------------------------
    # hints
    #
    PERSPECTIVE_CORRECTION_HINT = 0x0C50
    POINT_SMOOTH_HINT           = 0x0C51
    LINE_SMOOTH_HINT            = 0x0C52
    POLYGON_SMOOTH_HINT         = 0x0C53
    FOG_HINT                    = 0x0C54
    DONT_CARE                   = 0x1100
    FASTEST                     = 0x1101
    NICEST                      = 0x1102

    # --------------------------------------
    # scissor box
    #
    SCISSOR_BOX  = 0x0C10
    SCISSOR_TEST = 0x0C11

    # --------------------------------------
    # pixel mode / transfer
    #
    MAP_COLOR             = 0x0D10
    MAP_STENCIL           = 0x0D11
    INDEX_SHIFT           = 0x0D12
    INDEX_OFFSET          = 0x0D13
    RED_SCALE             = 0x0D14
    RED_BIAS              = 0x0D15
    GREEN_SCALE           = 0x0D18
    GREEN_BIAS            = 0x0D19
    BLUE_SCALE            = 0x0D1A
    BLUE_BIAS             = 0x0D1B
    ALPHA_SCALE           = 0x0D1C
    ALPHA_BIAS            = 0x0D1D
    DEPTH_SCALE           = 0x0D1E
    DEPTH_BIAS            = 0x0D1F
    PIXEL_MAP_S_TO_S_SIZE = 0x0CB1
    PIXEL_MAP_I_TO_I_SIZE = 0x0CB0
    PIXEL_MAP_I_TO_R_SIZE = 0x0CB2
    PIXEL_MAP_I_TO_G_SIZE = 0x0CB3
    PIXEL_MAP_I_TO_B_SIZE = 0x0CB4
    PIXEL_MAP_I_TO_A_SIZE = 0x0CB5
    PIXEL_MAP_R_TO_R_SIZE = 0x0CB6
    PIXEL_MAP_G_TO_G_SIZE = 0x0CB7
    PIXEL_MAP_B_TO_B_SIZE = 0x0CB8
    PIXEL_MAP_A_TO_A_SIZE = 0x0CB9
    PIXEL_MAP_S_TO_S      = 0x0C71
    PIXEL_MAP_I_TO_I      = 0x0C70
    PIXEL_MAP_I_TO_R      = 0x0C72
    PIXEL_MAP_I_TO_G      = 0x0C73
    PIXEL_MAP_I_TO_B      = 0x0C74
    PIXEL_MAP_I_TO_A      = 0x0C75
    PIXEL_MAP_R_TO_R      = 0x0C76
    PIXEL_MAP_G_TO_G      = 0x0C77
    PIXEL_MAP_B_TO_B      = 0x0C78
    PIXEL_MAP_A_TO_A      = 0x0C79
    PACK_ALIGNMENT        = 0x0D05
    PACK_LSB_FIRST        = 0x0D01
    PACK_ROW_LENGTH       = 0x0D02
    PACK_SKIP_PIXELS      = 0x0D04
    PACK_SKIP_ROWS        = 0x0D03
    PACK_SWAP_BYTES       = 0x0D00
    UNPACK_ALIGNMENT      = 0x0CF5
    UNPACK_LSB_FIRST      = 0x0CF1
    UNPACK_ROW_LENGTH     = 0x0CF2
    UNPACK_SKIP_PIXELS    = 0x0CF4
    UNPACK_SKIP_ROWS      = 0x0CF3
    UNPACK_SWAP_BYTES     = 0x0CF0
    ZOOM_X                = 0x0D16
    ZOOM_Y                = 0x0D17

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
    # OpenGL 1.2
    #
    RESCALE_NORMAL                = 0x803A
    CLAMP_TO_EDGE                 = 0x812F
    MAX_ELEMENTS_VERTICES         = 0x80E8
    MAX_ELEMENTS_INDICES          = 0x80E9
    BGR                           = 0x80E0
    BGRA                          = 0x80E1
    UNSIGNED_BYTE_3_3_2           = 0x8032
    UNSIGNED_BYTE_2_3_3_REV       = 0x8362
    UNSIGNED_SHORT_5_6_5          = 0x8363
    UNSIGNED_SHORT_5_6_5_REV      = 0x8364
    UNSIGNED_SHORT_4_4_4_4        = 0x8033
    UNSIGNED_SHORT_4_4_4_4_REV    = 0x8365
    UNSIGNED_SHORT_5_5_5_1        = 0x8034
    UNSIGNED_SHORT_1_5_5_5_REV    = 0x8366
    UNSIGNED_INT_8_8_8_8          = 0x8035
    UNSIGNED_INT_8_8_8_8_REV      = 0x8367
    UNSIGNED_INT_10_10_10_2       = 0x8036
    UNSIGNED_INT_2_10_10_10_REV   = 0x8368
    LIGHT_MODEL_COLOR_CONTROL     = 0x81F8
    SINGLE_COLOR                  = 0x81F9
    SEPARATE_SPECULAR_COLOR       = 0x81FA
    TEXTURE_MIN_LOD               = 0x813A
    TEXTURE_MAX_LOD               = 0x813B
    TEXTURE_BASE_LEVEL            = 0x813C
    TEXTURE_MAX_LEVEL             = 0x813D
    SMOOTH_POINT_SIZE_RANGE       = 0x0B12
    SMOOTH_POINT_SIZE_GRANULARITY = 0x0B13
    SMOOTH_LINE_WIDTH_RANGE       = 0x0B22
    SMOOTH_LINE_WIDTH_GRANULARITY = 0x0B23
    ALIASED_POINT_SIZE_RANGE      = 0x846D
    ALIASED_LINE_WIDTH_RANGE      = 0x846E
    PACK_SKIP_IMAGES              = 0x806B
    PACK_IMAGE_HEIGHT             = 0x806C
    UNPACK_SKIP_IMAGES            = 0x806D
    UNPACK_IMAGE_HEIGHT           = 0x806E
    TEXTURE_3D                    = 0x806F
    PROXY_TEXTURE_3D              = 0x8070
    TEXTURE_DEPTH                 = 0x8071
    TEXTURE_WRAP_R                = 0x8072
    MAX_3D_TEXTURE_SIZE           = 0x8073
    TEXTURE_BINDING_3D            = 0x806A

    # --------------------------------------
    # GL_ARB_imaging
    #
    CONSTANT_COLOR                      = 0x8001
    ONE_MINUS_CONSTANT_COLOR            = 0x8002
    CONSTANT_ALPHA                      = 0x8003
    ONE_MINUS_CONSTANT_ALPHA            = 0x8004
    COLOR_TABLE                         = 0x80D0
    POST_CONVOLUTION_COLOR_TABLE        = 0x80D1
    POST_COLOR_MATRIX_COLOR_TABLE       = 0x80D2
    PROXY_COLOR_TABLE                   = 0x80D3
    PROXY_POST_CONVOLUTION_COLOR_TABLE  = 0x80D4
    PROXY_POST_COLOR_MATRIX_COLOR_TABLE = 0x80D5
    COLOR_TABLE_SCALE                   = 0x80D6
    COLOR_TABLE_BIAS                    = 0x80D7
    COLOR_TABLE_FORMAT                  = 0x80D8
    COLOR_TABLE_WIDTH                   = 0x80D9
    COLOR_TABLE_RED_SIZE                = 0x80DA
    COLOR_TABLE_GREEN_SIZE              = 0x80DB
    COLOR_TABLE_BLUE_SIZE               = 0x80DC
    COLOR_TABLE_ALPHA_SIZE              = 0x80DD
    COLOR_TABLE_LUMINANCE_SIZE          = 0x80DE
    COLOR_TABLE_INTENSITY_SIZE          = 0x80DF
    CONVOLUTION_1D                      = 0x8010
    CONVOLUTION_2D                      = 0x8011
    SEPARABLE_2D                        = 0x8012
    CONVOLUTION_BORDER_MODE             = 0x8013
    CONVOLUTION_FILTER_SCALE            = 0x8014
    CONVOLUTION_FILTER_BIAS             = 0x8015
    REDUCE                              = 0x8016
    CONVOLUTION_FORMAT                  = 0x8017
    CONVOLUTION_WIDTH                   = 0x8018
    CONVOLUTION_HEIGHT                  = 0x8019
    MAX_CONVOLUTION_WIDTH               = 0x801A
    MAX_CONVOLUTION_HEIGHT              = 0x801B
    POST_CONVOLUTION_RED_SCALE          = 0x801C
    POST_CONVOLUTION_GREEN_SCALE        = 0x801D
    POST_CONVOLUTION_BLUE_SCALE         = 0x801E
    POST_CONVOLUTION_ALPHA_SCALE        = 0x801F
    POST_CONVOLUTION_RED_BIAS           = 0x8020
    POST_CONVOLUTION_GREEN_BIAS         = 0x8021
    POST_CONVOLUTION_BLUE_BIAS          = 0x8022
    POST_CONVOLUTION_ALPHA_BIAS         = 0x8023
    CONSTANT_BORDER                     = 0x8151
    REPLICATE_BORDER                    = 0x8153
    CONVOLUTION_BORDER_COLOR            = 0x8154
    COLOR_MATRIX                        = 0x80B1
    COLOR_MATRIX_STACK_DEPTH            = 0x80B2
    MAX_COLOR_MATRIX_STACK_DEPTH        = 0x80B3
    POST_COLOR_MATRIX_RED_SCALE         = 0x80B4
    POST_COLOR_MATRIX_GREEN_SCALE       = 0x80B5
    POST_COLOR_MATRIX_BLUE_SCALE        = 0x80B6
    POST_COLOR_MATRIX_ALPHA_SCALE       = 0x80B7
    POST_COLOR_MATRIX_RED_BIAS          = 0x80B8
    POST_COLOR_MATRIX_GREEN_BIAS        = 0x80B9
    POST_COLOR_MATRIX_BLUE_BIAS         = 0x80BA
    POST_COLOR_MATRIX_ALPHA_BIAS        = 0x80BB
    HISTOGRAM                           = 0x8024
    PROXY_HISTOGRAM                     = 0x8025
    HISTOGRAM_WIDTH                     = 0x8026
    HISTOGRAM_FORMAT                    = 0x8027
    HISTOGRAM_RED_SIZE                  = 0x8028
    HISTOGRAM_GREEN_SIZE                = 0x8029
    HISTOGRAM_BLUE_SIZE                 = 0x802A
    HISTOGRAM_ALPHA_SIZE                = 0x802B
    HISTOGRAM_LUMINANCE_SIZE            = 0x802C
    HISTOGRAM_SINK                      = 0x802D
    MINMAX                              = 0x802E
    MINMAX_FORMAT                       = 0x802F
    MINMAX_SINK                         = 0x8030
    TABLE_TOO_LARGE                     = 0x8031
    BLEND_EQUATION                      = 0x8009
    MIN                                 = 0x8007
    MAX                                 = 0x8008
    FUNC_ADD                            = 0x8006
    FUNC_SUBTRACT                       = 0x800A
    FUNC_REVERSE_SUBTRACT               = 0x800B
    BLEND_COLOR                         = 0x8005

    # --------------------------------------
    # multitexture
    #
    TEXTURE0              = 0x84C0
    TEXTURE1              = 0x84C1
    TEXTURE2              = 0x84C2
    TEXTURE3              = 0x84C3
    TEXTURE4              = 0x84C4
    TEXTURE5              = 0x84C5
    TEXTURE6              = 0x84C6
    TEXTURE7              = 0x84C7
    TEXTURE8              = 0x84C8
    TEXTURE9              = 0x84C9
    TEXTURE10             = 0x84CA
    TEXTURE11             = 0x84CB
    TEXTURE12             = 0x84CC
    TEXTURE13             = 0x84CD
    TEXTURE14             = 0x84CE
    TEXTURE15             = 0x84CF
    TEXTURE16             = 0x84D0
    TEXTURE17             = 0x84D1
    TEXTURE18             = 0x84D2
    TEXTURE19             = 0x84D3
    TEXTURE20             = 0x84D4
    TEXTURE21             = 0x84D5
    TEXTURE22             = 0x84D6
    TEXTURE23             = 0x84D7
    TEXTURE24             = 0x84D8
    TEXTURE25             = 0x84D9
    TEXTURE26             = 0x84DA
    TEXTURE27             = 0x84DB
    TEXTURE28             = 0x84DC
    TEXTURE29             = 0x84DD
    TEXTURE30             = 0x84DE
    TEXTURE31             = 0x84DF
    ACTIVE_TEXTURE        = 0x84E0
    CLIENT_ACTIVE_TEXTURE = 0x84E1
    MAX_TEXTURE_UNITS     = 0x84E2

    # --------------------------------------
    # texture_cube_map
    #
    NORMAL_MAP                  = 0x8511
    REFLECTION_MAP              = 0x8512
    TEXTURE_CUBE_MAP            = 0x8513
    TEXTURE_BINDING_CUBE_MAP    = 0x8514
    TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515
    TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516
    TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517
    TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518
    TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519
    TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851A
    PROXY_TEXTURE_CUBE_MAP      = 0x851B
    MAX_CUBE_MAP_TEXTURE_SIZE   = 0x851C

    # --------------------------------------
    # texture_compression
    #
    COMPRESSED_ALPHA               = 0x84E9
    COMPRESSED_LUMINANCE           = 0x84EA
    COMPRESSED_LUMINANCE_ALPHA     = 0x84EB
    COMPRESSED_INTENSITY           = 0x84EC
    COMPRESSED_RGB                 = 0x84ED
    COMPRESSED_RGBA                = 0x84EE
    TEXTURE_COMPRESSION_HINT       = 0x84EF
    TEXTURE_COMPRESSED_IMAGE_SIZE  = 0x86A0
    TEXTURE_COMPRESSED             = 0x86A1
    NUM_COMPRESSED_TEXTURE_FORMATS = 0x86A2
    COMPRESSED_TEXTURE_FORMATS     = 0x86A3

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

    # --------------------------------------
    # transpose_matrix
    #
    TRANSPOSE_MODELVIEW_MATRIX  = 0x84E3
    TRANSPOSE_PROJECTION_MATRIX = 0x84E4
    TRANSPOSE_TEXTURE_MATRIX    = 0x84E5
    TRANSPOSE_COLOR_MATRIX      = 0x84E6

    # --------------------------------------
    # texture_env_combine
    #
    COMBINE        = 0x8570
    COMBINE_RGB    = 0x8571
    COMBINE_ALPHA  = 0x8572
    SOURCE0_RGB    = 0x8580
    SOURCE1_RGB    = 0x8581
    SOURCE2_RGB    = 0x8582
    SOURCE0_ALPHA  = 0x8588
    SOURCE1_ALPHA  = 0x8589
    SOURCE2_ALPHA  = 0x858A
    OPERAND0_RGB   = 0x8590
    OPERAND1_RGB   = 0x8591
    OPERAND2_RGB   = 0x8592
    OPERAND0_ALPHA = 0x8598
    OPERAND1_ALPHA = 0x8599
    OPERAND2_ALPHA = 0x859A
    RGB_SCALE      = 0x8573
    ADD_SIGNED     = 0x8574
    INTERPOLATE    = 0x8575
    SUBTRACT       = 0x84E7
    CONSTANT       = 0x8576
    PRIMARY_COLOR  = 0x8577
    PREVIOUS       = 0x8578

    # --------------------------------------
    # texture_env_dot3
    #
    DOT3_RGB  = 0x86AE
    DOT3_RGBA = 0x86AF

    # --------------------------------------
    # texture_border_clamp
    #
    CLAMP_TO_BORDER = 0x812D

    # --------------------------------------
    # OpenGL 1.4
    #
    BLEND_DST_RGB                 = 0x80C8
    BLEND_SRC_RGB                 = 0x80C9
    BLEND_DST_ALPHA               = 0x80CA
    BLEND_SRC_ALPHA               = 0x80CB
    POINT_FADE_THRESHOLD_SIZE     = 0x8128
    DEPTH_COMPONENT16             = 0x81A5
    DEPTH_COMPONENT24             = 0x81A6
    DEPTH_COMPONENT32             = 0x81A7
    MIRRORED_REPEAT               = 0x8370
    MAX_TEXTURE_LOD_BIAS          = 0x84FD
    TEXTURE_LOD_BIAS              = 0x8501
    INCR_WRAP                     = 0x8507
    DECR_WRAP                     = 0x8508
    TEXTURE_DEPTH_SIZE            = 0x884A
    TEXTURE_COMPARE_MODE          = 0x884C
    TEXTURE_COMPARE_FUNC          = 0x884D
    POINT_SIZE_MIN                = 0x8126
    POINT_SIZE_MAX                = 0x8127
    POINT_DISTANCE_ATTENUATION    = 0x8129
    GENERATE_MIPMAP               = 0x8191
    GENERATE_MIPMAP_HINT          = 0x8192
    FOG_COORDINATE_SOURCE         = 0x8450
    FOG_COORDINATE                = 0x8451
    FRAGMENT_DEPTH                = 0x8452
    CURRENT_FOG_COORDINATE        = 0x8453
    FOG_COORDINATE_ARRAY_TYPE     = 0x8454
    FOG_COORDINATE_ARRAY_STRIDE   = 0x8455
    FOG_COORDINATE_ARRAY_POINTER  = 0x8456
    FOG_COORDINATE_ARRAY          = 0x8457
    COLOR_SUM                     = 0x8458
    CURRENT_SECONDARY_COLOR       = 0x8459
    SECONDARY_COLOR_ARRAY_SIZE    = 0x845A
    SECONDARY_COLOR_ARRAY_TYPE    = 0x845B
    SECONDARY_COLOR_ARRAY_STRIDE  = 0x845C
    SECONDARY_COLOR_ARRAY_POINTER = 0x845D
    SECONDARY_COLOR_ARRAY         = 0x845E
    TEXTURE_FILTER_CONTROL        = 0x8500
    DEPTH_TEXTURE_MODE            = 0x884B
    COMPARE_R_TO_TEXTURE          = 0x884E
    FUNC_ADD                      = 0x8006
    FUNC_SUBTRACT                 = 0x800A
    FUNC_REVERSE_SUBTRACT         = 0x800B
    MIN                           = 0x8007
    MAX                           = 0x8008
    CONSTANT_COLOR                = 0x8001
    ONE_MINUS_CONSTANT_COLOR      = 0x8002
    CONSTANT_ALPHA                = 0x8003
    ONE_MINUS_CONSTANT_ALPHA      = 0x8004

    # --------------------------------------
    # OpenGL 1.5
    #
    BUFFER_SIZE                          = 0x8764
    BUFFER_USAGE                         = 0x8765
    QUERY_COUNTER_BITS                   = 0x8864
    CURRENT_QUERY                        = 0x8865
    QUERY_RESULT                         = 0x8866
    QUERY_RESULT_AVAILABLE               = 0x8867
    ARRAY_BUFFER                         = 0x8892
    ELEMENT_ARRAY_BUFFER                 = 0x8893
    ARRAY_BUFFER_BINDING                 = 0x8894
    ELEMENT_ARRAY_BUFFER_BINDING         = 0x8895
    VERTEX_ATTRIB_ARRAY_BUFFER_BINDING   = 0x889F
    READ_ONLY                            = 0x88B8
    WRITE_ONLY                           = 0x88B9
    READ_WRITE                           = 0x88BA
    BUFFER_ACCESS                        = 0x88BB
    BUFFER_MAPPED                        = 0x88BC
    BUFFER_MAP_POINTER                   = 0x88BD
    STREAM_DRAW                          = 0x88E0
    STREAM_READ                          = 0x88E1
    STREAM_COPY                          = 0x88E2
    STATIC_DRAW                          = 0x88E4
    STATIC_READ                          = 0x88E5
    STATIC_COPY                          = 0x88E6
    DYNAMIC_DRAW                         = 0x88E8
    DYNAMIC_READ                         = 0x88E9
    DYNAMIC_COPY                         = 0x88EA
    SAMPLES_PASSED                       = 0x8914
    SRC1_ALPHA                           = 0x8589
    VERTEX_ARRAY_BUFFER_BINDING          = 0x8896
    NORMAL_ARRAY_BUFFER_BINDING          = 0x8897
    COLOR_ARRAY_BUFFER_BINDING           = 0x8898
    INDEX_ARRAY_BUFFER_BINDING           = 0x8899
    TEXTURE_COORD_ARRAY_BUFFER_BINDING   = 0x889A
    EDGE_FLAG_ARRAY_BUFFER_BINDING       = 0x889B
    SECONDARY_COLOR_ARRAY_BUFFER_BINDING = 0x889C
    FOG_COORDINATE_ARRAY_BUFFER_BINDING  = 0x889D
    WEIGHT_ARRAY_BUFFER_BINDING          = 0x889E
    FOG_COORD_SRC                        = 0x8450
    FOG_COORD                            = 0x8451
    CURRENT_FOG_COORD                    = 0x8453
    FOG_COORD_ARRAY_TYPE                 = 0x8454
    FOG_COORD_ARRAY_STRIDE               = 0x8455
    FOG_COORD_ARRAY_POINTER              = 0x8456
    FOG_COORD_ARRAY                      = 0x8457
    FOG_COORD_ARRAY_BUFFER_BINDING       = 0x889D
    SRC0_RGB                             = 0x8580
    SRC1_RGB                             = 0x8581
    SRC2_RGB                             = 0x8582
    SRC0_ALPHA                           = 0x8588
    SRC2_ALPHA                           = 0x858A

    # --------------------------------------
    # OpenGL 2.0
    #
    BLEND_EQUATION_RGB               = 0x8009
    VERTEX_ATTRIB_ARRAY_ENABLED      = 0x8622
    VERTEX_ATTRIB_ARRAY_SIZE         = 0x8623
    VERTEX_ATTRIB_ARRAY_STRIDE       = 0x8624
    VERTEX_ATTRIB_ARRAY_TYPE         = 0x8625
    CURRENT_VERTEX_ATTRIB            = 0x8626
    VERTEX_PROGRAM_POINT_SIZE        = 0x8642
    VERTEX_ATTRIB_ARRAY_POINTER      = 0x8645
    STENCIL_BACK_FUNC                = 0x8800
    STENCIL_BACK_FAIL                = 0x8801
    STENCIL_BACK_PASS_DEPTH_FAIL     = 0x8802
    STENCIL_BACK_PASS_DEPTH_PASS     = 0x8803
    MAX_DRAW_BUFFERS                 = 0x8824
    DRAW_BUFFER0                     = 0x8825
    DRAW_BUFFER1                     = 0x8826
    DRAW_BUFFER2                     = 0x8827
    DRAW_BUFFER3                     = 0x8828
    DRAW_BUFFER4                     = 0x8829
    DRAW_BUFFER5                     = 0x882A
    DRAW_BUFFER6                     = 0x882B
    DRAW_BUFFER7                     = 0x882C
    DRAW_BUFFER8                     = 0x882D
    DRAW_BUFFER9                     = 0x882E
    DRAW_BUFFER10                    = 0x882F
    DRAW_BUFFER11                    = 0x8830
    DRAW_BUFFER12                    = 0x8831
    DRAW_BUFFER13                    = 0x8832
    DRAW_BUFFER14                    = 0x8833
    DRAW_BUFFER15                    = 0x8834
    BLEND_EQUATION_ALPHA             = 0x883D
    MAX_VERTEX_ATTRIBS               = 0x8869
    VERTEX_ATTRIB_ARRAY_NORMALIZED   = 0x886A
    MAX_TEXTURE_IMAGE_UNITS          = 0x8872
    FRAGMENT_SHADER                  = 0x8B30
    VERTEX_SHADER                    = 0x8B31
    MAX_FRAGMENT_UNIFORM_COMPONENTS  = 0x8B49
    MAX_VERTEX_UNIFORM_COMPONENTS    = 0x8B4A
    MAX_VARYING_FLOATS               = 0x8B4B
    MAX_VERTEX_TEXTURE_IMAGE_UNITS   = 0x8B4C
    MAX_COMBINED_TEXTURE_IMAGE_UNITS = 0x8B4D
    SHADER_TYPE                      = 0x8B4F
    FLOAT_VEC2                       = 0x8B50
    FLOAT_VEC3                       = 0x8B51
    FLOAT_VEC4                       = 0x8B52
    INT_VEC2                         = 0x8B53
    INT_VEC3                         = 0x8B54
    INT_VEC4                         = 0x8B55
    BOOL                             = 0x8B56
    BOOL_VEC2                        = 0x8B57
    BOOL_VEC3                        = 0x8B58
    BOOL_VEC4                        = 0x8B59
    FLOAT_MAT2                       = 0x8B5A
    FLOAT_MAT3                       = 0x8B5B
    FLOAT_MAT4                       = 0x8B5C
    SAMPLER_1D                       = 0x8B5D
    SAMPLER_2D                       = 0x8B5E
    SAMPLER_3D                       = 0x8B5F
    SAMPLER_CUBE                     = 0x8B60
    SAMPLER_1D_SHADOW                = 0x8B61
    SAMPLER_2D_SHADOW                = 0x8B62
    DELETE_STATUS                    = 0x8B80
    COMPILE_STATUS                   = 0x8B81
    LINK_STATUS                      = 0x8B82
    VALIDATE_STATUS                  = 0x8B83
    INFO_LOG_LENGTH                  = 0x8B84
    ATTACHED_SHADERS                 = 0x8B85
    ACTIVE_UNIFORMS                  = 0x8B86
    ACTIVE_UNIFORM_MAX_LENGTH        = 0x8B87
    SHADER_SOURCE_LENGTH             = 0x8B88
    ACTIVE_ATTRIBUTES                = 0x8B89
    ACTIVE_ATTRIBUTE_MAX_LENGTH      = 0x8B8A
    FRAGMENT_SHADER_DERIVATIVE_HINT  = 0x8B8B
    SHADING_LANGUAGE_VERSION         = 0x8B8C
    CURRENT_PROGRAM                  = 0x8B8D
    POINT_SPRITE_COORD_ORIGIN        = 0x8CA0
    LOWER_LEFT                       = 0x8CA1
    UPPER_LEFT                       = 0x8CA2
    STENCIL_BACK_REF                 = 0x8CA3
    STENCIL_BACK_VALUE_MASK          = 0x8CA4
    STENCIL_BACK_WRITEMASK           = 0x8CA5
    VERTEX_PROGRAM_TWO_SIDE          = 0x8643
    POINT_SPRITE                     = 0x8861
    COORD_REPLACE                    = 0x8862
    MAX_TEXTURE_COORDS               = 0x8871

    # --------------------------------------
    # OpenGL 2.1
    #
    PIXEL_PACK_BUFFER              = 0x88EB
    PIXEL_UNPACK_BUFFER            = 0x88EC
    PIXEL_PACK_BUFFER_BINDING      = 0x88ED
    PIXEL_UNPACK_BUFFER_BINDING    = 0x88EF
    FLOAT_MAT2x3                   = 0x8B65
    FLOAT_MAT2x4                   = 0x8B66
    FLOAT_MAT3x2                   = 0x8B67
    FLOAT_MAT3x4                   = 0x8B68
    FLOAT_MAT4x2                   = 0x8B69
    FLOAT_MAT4x3                   = 0x8B6A
    SRGB                           = 0x8C40
    SRGB8                          = 0x8C41
    SRGB_ALPHA                     = 0x8C42
    SRGB8_ALPHA8                   = 0x8C43
    COMPRESSED_SRGB                = 0x8C48
    COMPRESSED_SRGB_ALPHA          = 0x8C49
    CURRENT_RASTER_SECONDARY_COLOR = 0x845F
    SLUMINANCE_ALPHA               = 0x8C44
    SLUMINANCE8_ALPHA8             = 0x8C45
    SLUMINANCE                     = 0x8C46
    SLUMINANCE8                    = 0x8C47
    COMPRESSED_SLUMINANCE          = 0x8C4A
    COMPRESSED_SLUMINANCE_ALPHA    = 0x8C4B

    # --------------------------------------
    # OpenGL 3.0
    #
    COMPARE_REF_TO_TEXTURE                        = 0x884E
    CLIP_DISTANCE0                                = 0x3000
    CLIP_DISTANCE1                                = 0x3001
    CLIP_DISTANCE2                                = 0x3002
    CLIP_DISTANCE3                                = 0x3003
    CLIP_DISTANCE4                                = 0x3004
    CLIP_DISTANCE5                                = 0x3005
    CLIP_DISTANCE6                                = 0x3006
    CLIP_DISTANCE7                                = 0x3007
    MAX_CLIP_DISTANCES                            = 0x0D32
    MAJOR_VERSION                                 = 0x821B
    MINOR_VERSION                                 = 0x821C
    NUM_EXTENSIONS                                = 0x821D
    CONTEXT_FLAGS                                 = 0x821E
    COMPRESSED_RED                                = 0x8225
    COMPRESSED_RG                                 = 0x8226
    CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT           = 0x00000001
    RGBA32F                                       = 0x8814
    RGB32F                                        = 0x8815
    RGBA16F                                       = 0x881A
    RGB16F                                        = 0x881B
    VERTEX_ATTRIB_ARRAY_INTEGER                   = 0x88FD
    MAX_ARRAY_TEXTURE_LAYERS                      = 0x88FF
    MIN_PROGRAM_TEXEL_OFFSET                      = 0x8904
    MAX_PROGRAM_TEXEL_OFFSET                      = 0x8905
    CLAMP_READ_COLOR                              = 0x891C
    FIXED_ONLY                                    = 0x891D
    MAX_VARYING_COMPONENTS                        = 0x8B4B
    TEXTURE_1D_ARRAY                              = 0x8C18
    PROXY_TEXTURE_1D_ARRAY                        = 0x8C19
    TEXTURE_2D_ARRAY                              = 0x8C1A
    PROXY_TEXTURE_2D_ARRAY                        = 0x8C1B
    TEXTURE_BINDING_1D_ARRAY                      = 0x8C1C
    TEXTURE_BINDING_2D_ARRAY                      = 0x8C1D
    R11F_G11F_B10F                                = 0x8C3A
    UNSIGNED_INT_10F_11F_11F_REV                  = 0x8C3B
    RGB9_E5                                       = 0x8C3D
    UNSIGNED_INT_5_9_9_9_REV                      = 0x8C3E
    TEXTURE_SHARED_SIZE                           = 0x8C3F
    TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH         = 0x8C76
    TRANSFORM_FEEDBACK_BUFFER_MODE                = 0x8C7F
    MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS    = 0x8C80
    TRANSFORM_FEEDBACK_VARYINGS                   = 0x8C83
    TRANSFORM_FEEDBACK_BUFFER_START               = 0x8C84
    TRANSFORM_FEEDBACK_BUFFER_SIZE                = 0x8C85
    PRIMITIVES_GENERATED                          = 0x8C87
    TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN         = 0x8C88
    RASTERIZER_DISCARD                            = 0x8C89
    MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS = 0x8C8A
    MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS       = 0x8C8B
    INTERLEAVED_ATTRIBS                           = 0x8C8C
    SEPARATE_ATTRIBS                              = 0x8C8D
    TRANSFORM_FEEDBACK_BUFFER                     = 0x8C8E
    TRANSFORM_FEEDBACK_BUFFER_BINDING             = 0x8C8F
    RGBA32UI                                      = 0x8D70
    RGB32UI                                       = 0x8D71
    RGBA16UI                                      = 0x8D76
    RGB16UI                                       = 0x8D77
    RGBA8UI                                       = 0x8D7C
    RGB8UI                                        = 0x8D7D
    RGBA32I                                       = 0x8D82
    RGB32I                                        = 0x8D83
    RGBA16I                                       = 0x8D88
    RGB16I                                        = 0x8D89
    RGBA8I                                        = 0x8D8E
    RGB8I                                         = 0x8D8F
    RED_INTEGER                                   = 0x8D94
    GREEN_INTEGER                                 = 0x8D95
    BLUE_INTEGER                                  = 0x8D96
    RGB_INTEGER                                   = 0x8D98
    RGBA_INTEGER                                  = 0x8D99
    BGR_INTEGER                                   = 0x8D9A
    BGRA_INTEGER                                  = 0x8D9B
    SAMPLER_1D_ARRAY                              = 0x8DC0
    SAMPLER_2D_ARRAY                              = 0x8DC1
    SAMPLER_1D_ARRAY_SHADOW                       = 0x8DC3
    SAMPLER_2D_ARRAY_SHADOW                       = 0x8DC4
    SAMPLER_CUBE_SHADOW                           = 0x8DC5
    UNSIGNED_INT_VEC2                             = 0x8DC6
    UNSIGNED_INT_VEC3                             = 0x8DC7
    UNSIGNED_INT_VEC4                             = 0x8DC8
    INT_SAMPLER_1D                                = 0x8DC9
    INT_SAMPLER_2D                                = 0x8DCA
    INT_SAMPLER_3D                                = 0x8DCB
    INT_SAMPLER_CUBE                              = 0x8DCC
    INT_SAMPLER_1D_ARRAY                          = 0x8DCE
    INT_SAMPLER_2D_ARRAY                          = 0x8DCF
    UNSIGNED_INT_SAMPLER_1D                       = 0x8DD1
    UNSIGNED_INT_SAMPLER_2D                       = 0x8DD2
    UNSIGNED_INT_SAMPLER_3D                       = 0x8DD3
    UNSIGNED_INT_SAMPLER_CUBE                     = 0x8DD4
    UNSIGNED_INT_SAMPLER_1D_ARRAY                 = 0x8DD6
    UNSIGNED_INT_SAMPLER_2D_ARRAY                 = 0x8DD7
    QUERY_WAIT                                    = 0x8E13
    QUERY_NO_WAIT                                 = 0x8E14
    QUERY_BY_REGION_WAIT                          = 0x8E15
    QUERY_BY_REGION_NO_WAIT                       = 0x8E16
    BUFFER_ACCESS_FLAGS                           = 0x911F
    BUFFER_MAP_LENGTH                             = 0x9120
    BUFFER_MAP_OFFSET                             = 0x9121
    DEPTH_COMPONENT32F                            = 0x8CAC
    DEPTH32F_STENCIL8                             = 0x8CAD
    FLOAT_32_UNSIGNED_INT_24_8_REV                = 0x8DAD
    INVALID_FRAMEBUFFER_OPERATION                 = 0x0506
    FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING         = 0x8210
    FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE         = 0x8211
    FRAMEBUFFER_ATTACHMENT_RED_SIZE               = 0x8212
    FRAMEBUFFER_ATTACHMENT_GREEN_SIZE             = 0x8213
    FRAMEBUFFER_ATTACHMENT_BLUE_SIZE              = 0x8214
    FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE             = 0x8215
    FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE             = 0x8216
    FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE           = 0x8217
    FRAMEBUFFER_DEFAULT                           = 0x8218
    FRAMEBUFFER_UNDEFINED                         = 0x8219
    DEPTH_STENCIL_ATTACHMENT                      = 0x821A
    MAX_RENDERBUFFER_SIZE                         = 0x84E8
    DEPTH_STENCIL                                 = 0x84F9
    UNSIGNED_INT_24_8                             = 0x84FA
    DEPTH24_STENCIL8                              = 0x88F0
    TEXTURE_STENCIL_SIZE                          = 0x88F1
    TEXTURE_RED_TYPE                              = 0x8C10
    TEXTURE_GREEN_TYPE                            = 0x8C11
    TEXTURE_BLUE_TYPE                             = 0x8C12
    TEXTURE_ALPHA_TYPE                            = 0x8C13
    TEXTURE_DEPTH_TYPE                            = 0x8C16
    UNSIGNED_NORMALIZED                           = 0x8C17
    FRAMEBUFFER_BINDING                           = 0x8CA6
    DRAW_FRAMEBUFFER_BINDING                      = 0x8CA6
    RENDERBUFFER_BINDING                          = 0x8CA7
    READ_FRAMEBUFFER                              = 0x8CA8
    DRAW_FRAMEBUFFER                              = 0x8CA9
    READ_FRAMEBUFFER_BINDING                      = 0x8CAA
    RENDERBUFFER_SAMPLES                          = 0x8CAB
    FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE            = 0x8CD0
    FRAMEBUFFER_ATTACHMENT_OBJECT_NAME            = 0x8CD1
    FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL          = 0x8CD2
    FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE  = 0x8CD3
    FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER          = 0x8CD4
    FRAMEBUFFER_COMPLETE                          = 0x8CD5
    FRAMEBUFFER_INCOMPLETE_ATTACHMENT             = 0x8CD6
    FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT     = 0x8CD7
    FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER            = 0x8CDB
    FRAMEBUFFER_INCOMPLETE_READ_BUFFER            = 0x8CDC
    FRAMEBUFFER_UNSUPPORTED                       = 0x8CDD
    MAX_COLOR_ATTACHMENTS                         = 0x8CDF
    COLOR_ATTACHMENT0                             = 0x8CE0
    COLOR_ATTACHMENT1                             = 0x8CE1
    COLOR_ATTACHMENT2                             = 0x8CE2
    COLOR_ATTACHMENT3                             = 0x8CE3
    COLOR_ATTACHMENT4                             = 0x8CE4
    COLOR_ATTACHMENT5                             = 0x8CE5
    COLOR_ATTACHMENT6                             = 0x8CE6
    COLOR_ATTACHMENT7                             = 0x8CE7
    COLOR_ATTACHMENT8                             = 0x8CE8
    COLOR_ATTACHMENT9                             = 0x8CE9
    COLOR_ATTACHMENT10                            = 0x8CEA
    COLOR_ATTACHMENT11                            = 0x8CEB
    COLOR_ATTACHMENT12                            = 0x8CEC
    COLOR_ATTACHMENT13                            = 0x8CED
    COLOR_ATTACHMENT14                            = 0x8CEE
    COLOR_ATTACHMENT15                            = 0x8CEF
    DEPTH_ATTACHMENT                              = 0x8D00
    STENCIL_ATTACHMENT                            = 0x8D20
    FRAMEBUFFER                                   = 0x8D40
    RENDERBUFFER                                  = 0x8D41
    RENDERBUFFER_WIDTH                            = 0x8D42
    RENDERBUFFER_HEIGHT                           = 0x8D43
    RENDERBUFFER_INTERNAL_FORMAT                  = 0x8D44
    STENCIL_INDEX1                                = 0x8D46
    STENCIL_INDEX4                                = 0x8D47
    STENCIL_INDEX8                                = 0x8D48
    STENCIL_INDEX16                               = 0x8D49
    RENDERBUFFER_RED_SIZE                         = 0x8D50
    RENDERBUFFER_GREEN_SIZE                       = 0x8D51
    RENDERBUFFER_BLUE_SIZE                        = 0x8D52
    RENDERBUFFER_ALPHA_SIZE                       = 0x8D53
    RENDERBUFFER_DEPTH_SIZE                       = 0x8D54
    RENDERBUFFER_STENCIL_SIZE                     = 0x8D55
    FRAMEBUFFER_INCOMPLETE_MULTISAMPLE            = 0x8D56
    MAX_SAMPLES                                   = 0x8D57
    INDEX                                         = 0x8222
    TEXTURE_LUMINANCE_TYPE                        = 0x8C14
    TEXTURE_INTENSITY_TYPE                        = 0x8C15
    FRAMEBUFFER_SRGB                              = 0x8DB9
    HALF_FLOAT                                    = 0x140B
    MAP_READ_BIT                                  = 0x0001
    MAP_WRITE_BIT                                 = 0x0002
    MAP_INVALIDATE_RANGE_BIT                      = 0x0004
    MAP_INVALIDATE_BUFFER_BIT                     = 0x0008
    MAP_FLUSH_EXPLICIT_BIT                        = 0x0010
    MAP_UNSYNCHRONIZED_BIT                        = 0x0020
    COMPRESSED_RED_RGTC1                          = 0x8DBB
    COMPRESSED_SIGNED_RED_RGTC1                   = 0x8DBC
    COMPRESSED_RG_RGTC2                           = 0x8DBD
    COMPRESSED_SIGNED_RG_RGTC2                    = 0x8DBE
    RG                                            = 0x8227
    RG_INTEGER                                    = 0x8228
    R8                                            = 0x8229
    R16                                           = 0x822A
    RG8                                           = 0x822B
    RG16                                          = 0x822C
    R16F                                          = 0x822D
    R32F                                          = 0x822E
    RG16F                                         = 0x822F
    RG32F                                         = 0x8230
    R8I                                           = 0x8231
    R8UI                                          = 0x8232
    R16I                                          = 0x8233
    R16UI                                         = 0x8234
    R32I                                          = 0x8235
    R32UI                                         = 0x8236
    RG8I                                          = 0x8237
    RG8UI                                         = 0x8238
    RG16I                                         = 0x8239
    RG16UI                                        = 0x823A
    RG32I                                         = 0x823B
    RG32UI                                        = 0x823C
    VERTEX_ARRAY_BINDING                          = 0x85B5
    CLAMP_VERTEX_COLOR                            = 0x891A
    CLAMP_FRAGMENT_COLOR                          = 0x891B
    ALPHA_INTEGER                                 = 0x8D97

    # --------------------------------------
    # OpenGL 3.1
    #
    SAMPLER_2D_RECT                             = 0x8B63
    SAMPLER_2D_RECT_SHADOW                      = 0x8B64
    SAMPLER_BUFFER                              = 0x8DC2
    INT_SAMPLER_2D_RECT                         = 0x8DCD
    INT_SAMPLER_BUFFER                          = 0x8DD0
    UNSIGNED_INT_SAMPLER_2D_RECT                = 0x8DD5
    UNSIGNED_INT_SAMPLER_BUFFER                 = 0x8DD8
    TEXTURE_BUFFER                              = 0x8C2A
    MAX_TEXTURE_BUFFER_SIZE                     = 0x8C2B
    TEXTURE_BINDING_BUFFER                      = 0x8C2C
    TEXTURE_BUFFER_DATA_STORE_BINDING           = 0x8C2D
    TEXTURE_RECTANGLE                           = 0x84F5
    TEXTURE_BINDING_RECTANGLE                   = 0x84F6
    PROXY_TEXTURE_RECTANGLE                     = 0x84F7
    MAX_RECTANGLE_TEXTURE_SIZE                  = 0x84F8
    R8_SNORM                                    = 0x8F94
    RG8_SNORM                                   = 0x8F95
    RGB8_SNORM                                  = 0x8F96
    RGBA8_SNORM                                 = 0x8F97
    R16_SNORM                                   = 0x8F98
    RG16_SNORM                                  = 0x8F99
    RGB16_SNORM                                 = 0x8F9A
    RGBA16_SNORM                                = 0x8F9B
    SIGNED_NORMALIZED                           = 0x8F9C
    PRIMITIVE_RESTART                           = 0x8F9D
    PRIMITIVE_RESTART_INDEX                     = 0x8F9E
    COPY_READ_BUFFER                            = 0x8F36
    COPY_WRITE_BUFFER                           = 0x8F37
    UNIFORM_BUFFER                              = 0x8A11
    UNIFORM_BUFFER_BINDING                      = 0x8A28
    UNIFORM_BUFFER_START                        = 0x8A29
    UNIFORM_BUFFER_SIZE                         = 0x8A2A
    MAX_VERTEX_UNIFORM_BLOCKS                   = 0x8A2B
    MAX_FRAGMENT_UNIFORM_BLOCKS                 = 0x8A2D
    MAX_COMBINED_UNIFORM_BLOCKS                 = 0x8A2E
    MAX_UNIFORM_BUFFER_BINDINGS                 = 0x8A2F
    MAX_UNIFORM_BLOCK_SIZE                      = 0x8A30
    MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS      = 0x8A31
    MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS    = 0x8A33
    UNIFORM_BUFFER_OFFSET_ALIGNMENT             = 0x8A34
    ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH        = 0x8A35
    ACTIVE_UNIFORM_BLOCKS                       = 0x8A36
    UNIFORM_TYPE                                = 0x8A37
    UNIFORM_SIZE                                = 0x8A38
    UNIFORM_NAME_LENGTH                         = 0x8A39
    UNIFORM_BLOCK_INDEX                         = 0x8A3A
    UNIFORM_OFFSET                              = 0x8A3B
    UNIFORM_ARRAY_STRIDE                        = 0x8A3C
    UNIFORM_MATRIX_STRIDE                       = 0x8A3D
    UNIFORM_IS_ROW_MAJOR                        = 0x8A3E
    UNIFORM_BLOCK_BINDING                       = 0x8A3F
    UNIFORM_BLOCK_DATA_SIZE                     = 0x8A40
    UNIFORM_BLOCK_NAME_LENGTH                   = 0x8A41
    UNIFORM_BLOCK_ACTIVE_UNIFORMS               = 0x8A42
    UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES        = 0x8A43
    UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER   = 0x8A44
    UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER = 0x8A46
    INVALID_INDEX                               = 0xFFFFFFFF

    # --------------------------------------
    # OpenGL 3.2
    #
    CONTEXT_CORE_PROFILE_BIT                  = 0x00000001
    CONTEXT_COMPATIBILITY_PROFILE_BIT         = 0x00000002
    LINES_ADJACENCY                           = 0x000A
    LINE_STRIP_ADJACENCY                      = 0x000B
    TRIANGLES_ADJACENCY                       = 0x000C
    TRIANGLE_STRIP_ADJACENCY                  = 0x000D
    PROGRAM_POINT_SIZE                        = 0x8642
    MAX_GEOMETRY_TEXTURE_IMAGE_UNITS          = 0x8C29
    FRAMEBUFFER_ATTACHMENT_LAYERED            = 0x8DA7
    FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS      = 0x8DA8
    GEOMETRY_SHADER                           = 0x8DD9
    GEOMETRY_VERTICES_OUT                     = 0x8916
    GEOMETRY_INPUT_TYPE                       = 0x8917
    GEOMETRY_OUTPUT_TYPE                      = 0x8918
    MAX_GEOMETRY_UNIFORM_COMPONENTS           = 0x8DDF
    MAX_GEOMETRY_OUTPUT_VERTICES              = 0x8DE0
    MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS      = 0x8DE1
    MAX_VERTEX_OUTPUT_COMPONENTS              = 0x9122
    MAX_GEOMETRY_INPUT_COMPONENTS             = 0x9123
    MAX_GEOMETRY_OUTPUT_COMPONENTS            = 0x9124
    MAX_FRAGMENT_INPUT_COMPONENTS             = 0x9125
    CONTEXT_PROFILE_MASK                      = 0x9126
    DEPTH_CLAMP                               = 0x864F
    QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION  = 0x8E4C
    FIRST_VERTEX_CONVENTION                   = 0x8E4D
    LAST_VERTEX_CONVENTION                    = 0x8E4E
    PROVOKING_VERTEX                          = 0x8E4F
    TEXTURE_CUBE_MAP_SEAMLESS                 = 0x884F
    MAX_SERVER_WAIT_TIMEOUT                   = 0x9111
    OBJECT_TYPE                               = 0x9112
    SYNC_CONDITION                            = 0x9113
    SYNC_STATUS                               = 0x9114
    SYNC_FLAGS                                = 0x9115
    SYNC_FENCE                                = 0x9116
    SYNC_GPU_COMMANDS_COMPLETE                = 0x9117
    UNSIGNALED                                = 0x9118
    SIGNALED                                  = 0x9119
    ALREADY_SIGNALED                          = 0x911A
    TIMEOUT_EXPIRED                           = 0x911B
    CONDITION_SATISFIED                       = 0x911C
    WAIT_FAILED                               = 0x911D
    TIMEOUT_IGNORED                           = 0xFFFFFFFFFFFFFFFF
    SYNC_FLUSH_COMMANDS_BIT                   = 0x00000001
    SAMPLE_POSITION                           = 0x8E50
    SAMPLE_MASK                               = 0x8E51
    SAMPLE_MASK_VALUE                         = 0x8E52
    MAX_SAMPLE_MASK_WORDS                     = 0x8E59
    TEXTURE_2D_MULTISAMPLE                    = 0x9100
    PROXY_TEXTURE_2D_MULTISAMPLE              = 0x9101
    TEXTURE_2D_MULTISAMPLE_ARRAY              = 0x9102
    PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY        = 0x9103
    TEXTURE_BINDING_2D_MULTISAMPLE            = 0x9104
    TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY      = 0x9105
    TEXTURE_SAMPLES                           = 0x9106
    TEXTURE_FIXED_SAMPLE_LOCATIONS            = 0x9107
    SAMPLER_2D_MULTISAMPLE                    = 0x9108
    INT_SAMPLER_2D_MULTISAMPLE                = 0x9109
    UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE       = 0x910A
    SAMPLER_2D_MULTISAMPLE_ARRAY              = 0x910B
    INT_SAMPLER_2D_MULTISAMPLE_ARRAY          = 0x910C
    UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY = 0x910D
    MAX_COLOR_TEXTURE_SAMPLES                 = 0x910E
    MAX_DEPTH_TEXTURE_SAMPLES                 = 0x910F
    MAX_INTEGER_SAMPLES                       = 0x9110

    # --------------------------------------
    # OpenGL 3.3
    #
    VERTEX_ATTRIB_ARRAY_DIVISOR  = 0x88FE
    SRC1_COLOR                   = 0x88F9
    ONE_MINUS_SRC1_COLOR         = 0x88FA
    ONE_MINUS_SRC1_ALPHA         = 0x88FB
    MAX_DUAL_SOURCE_DRAW_BUFFERS = 0x88FC
    ANY_SAMPLES_PASSED           = 0x8C2F
    SAMPLER_BINDING              = 0x8919
    RGB10_A2UI                   = 0x906F
    TEXTURE_SWIZZLE_R            = 0x8E42
    TEXTURE_SWIZZLE_G            = 0x8E43
    TEXTURE_SWIZZLE_B            = 0x8E44
    TEXTURE_SWIZZLE_A            = 0x8E45
    TEXTURE_SWIZZLE_RGBA         = 0x8E46
    TIME_ELAPSED                 = 0x88BF
    TIMESTAMP                    = 0x8E28
    INT_2_10_10_10_REV           = 0x8D9F

    # --------------------------------------
    # OpenGL 4.0
    #
    SAMPLE_SHADING                                     = 0x8C36
    MIN_SAMPLE_SHADING_VALUE                           = 0x8C37
    MIN_PROGRAM_TEXTURE_GATHER_OFFSET                  = 0x8E5E
    MAX_PROGRAM_TEXTURE_GATHER_OFFSET                  = 0x8E5F
    TEXTURE_CUBE_MAP_ARRAY                             = 0x9009
    TEXTURE_BINDING_CUBE_MAP_ARRAY                     = 0x900A
    PROXY_TEXTURE_CUBE_MAP_ARRAY                       = 0x900B
    SAMPLER_CUBE_MAP_ARRAY                             = 0x900C
    SAMPLER_CUBE_MAP_ARRAY_SHADOW                      = 0x900D
    INT_SAMPLER_CUBE_MAP_ARRAY                         = 0x900E
    UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY                = 0x900F
    DRAW_INDIRECT_BUFFER                               = 0x8F3F
    DRAW_INDIRECT_BUFFER_BINDING                       = 0x8F43
    GEOMETRY_SHADER_INVOCATIONS                        = 0x887F
    MAX_GEOMETRY_SHADER_INVOCATIONS                    = 0x8E5A
    MIN_FRAGMENT_INTERPOLATION_OFFSET                  = 0x8E5B
    MAX_FRAGMENT_INTERPOLATION_OFFSET                  = 0x8E5C
    FRAGMENT_INTERPOLATION_OFFSET_BITS                 = 0x8E5D
    MAX_VERTEX_STREAMS                                 = 0x8E71
    DOUBLE_VEC2                                        = 0x8FFC
    DOUBLE_VEC3                                        = 0x8FFD
    DOUBLE_VEC4                                        = 0x8FFE
    DOUBLE_MAT2                                        = 0x8F46
    DOUBLE_MAT3                                        = 0x8F47
    DOUBLE_MAT4                                        = 0x8F48
    DOUBLE_MAT2x3                                      = 0x8F49
    DOUBLE_MAT2x4                                      = 0x8F4A
    DOUBLE_MAT3x2                                      = 0x8F4B
    DOUBLE_MAT3x4                                      = 0x8F4C
    DOUBLE_MAT4x2                                      = 0x8F4D
    DOUBLE_MAT4x3                                      = 0x8F4E
    ACTIVE_SUBROUTINES                                 = 0x8DE5
    ACTIVE_SUBROUTINE_UNIFORMS                         = 0x8DE6
    ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS                = 0x8E47
    ACTIVE_SUBROUTINE_MAX_LENGTH                       = 0x8E48
    ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH               = 0x8E49
    MAX_SUBROUTINES                                    = 0x8DE7
    MAX_SUBROUTINE_UNIFORM_LOCATIONS                   = 0x8DE8
    NUM_COMPATIBLE_SUBROUTINES                         = 0x8E4A
    COMPATIBLE_SUBROUTINES                             = 0x8E4B
    PATCHES                                            = 0x000E
    PATCH_VERTICES                                     = 0x8E72
    PATCH_DEFAULT_INNER_LEVEL                          = 0x8E73
    PATCH_DEFAULT_OUTER_LEVEL                          = 0x8E74
    TESS_CONTROL_OUTPUT_VERTICES                       = 0x8E75
    TESS_GEN_MODE                                      = 0x8E76
    TESS_GEN_SPACING                                   = 0x8E77
    TESS_GEN_VERTEX_ORDER                              = 0x8E78
    TESS_GEN_POINT_MODE                                = 0x8E79
    ISOLINES                                           = 0x8E7A
    FRACTIONAL_ODD                                     = 0x8E7B
    FRACTIONAL_EVEN                                    = 0x8E7C
    MAX_PATCH_VERTICES                                 = 0x8E7D
    MAX_TESS_GEN_LEVEL                                 = 0x8E7E
    MAX_TESS_CONTROL_UNIFORM_COMPONENTS                = 0x8E7F
    MAX_TESS_EVALUATION_UNIFORM_COMPONENTS             = 0x8E80
    MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS               = 0x8E81
    MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS            = 0x8E82
    MAX_TESS_CONTROL_OUTPUT_COMPONENTS                 = 0x8E83
    MAX_TESS_PATCH_COMPONENTS                          = 0x8E84
    MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS           = 0x8E85
    MAX_TESS_EVALUATION_OUTPUT_COMPONENTS              = 0x8E86
    MAX_TESS_CONTROL_UNIFORM_BLOCKS                    = 0x8E89
    MAX_TESS_EVALUATION_UNIFORM_BLOCKS                 = 0x8E8A
    MAX_TESS_CONTROL_INPUT_COMPONENTS                  = 0x886C
    MAX_TESS_EVALUATION_INPUT_COMPONENTS               = 0x886D
    MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS       = 0x8E1E
    MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS    = 0x8E1F
    UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER    = 0x84F0
    UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER = 0x84F1
    TESS_EVALUATION_SHADER                             = 0x8E87
    TESS_CONTROL_SHADER                                = 0x8E88
    TRANSFORM_FEEDBACK                                 = 0x8E22
    TRANSFORM_FEEDBACK_BUFFER_PAUSED                   = 0x8E23
    TRANSFORM_FEEDBACK_BUFFER_ACTIVE                   = 0x8E24
    TRANSFORM_FEEDBACK_BINDING                         = 0x8E25
    MAX_TRANSFORM_FEEDBACK_BUFFERS                     = 0x8E70

    # --------------------------------------
    # OpenGL 4.1
    #
    FIXED                            = 0x140C
    IMPLEMENTATION_COLOR_READ_TYPE   = 0x8B9A
    IMPLEMENTATION_COLOR_READ_FORMAT = 0x8B9B
    LOW_FLOAT                        = 0x8DF0
    MEDIUM_FLOAT                     = 0x8DF1
    HIGH_FLOAT                       = 0x8DF2
    LOW_INT                          = 0x8DF3
    MEDIUM_INT                       = 0x8DF4
    HIGH_INT                         = 0x8DF5
    SHADER_COMPILER                  = 0x8DFA
    SHADER_BINARY_FORMATS            = 0x8DF8
    NUM_SHADER_BINARY_FORMATS        = 0x8DF9
    MAX_VERTEX_UNIFORM_VECTORS       = 0x8DFB
    MAX_VARYING_VECTORS              = 0x8DFC
    MAX_FRAGMENT_UNIFORM_VECTORS     = 0x8DFD
    RGB565                           = 0x8D62
    PROGRAM_BINARY_RETRIEVABLE_HINT  = 0x8257
    PROGRAM_BINARY_LENGTH            = 0x8741
    NUM_PROGRAM_BINARY_FORMATS       = 0x87FE
    PROGRAM_BINARY_FORMATS           = 0x87FF
    VERTEX_SHADER_BIT                = 0x00000001
    FRAGMENT_SHADER_BIT              = 0x00000002
    GEOMETRY_SHADER_BIT              = 0x00000004
    TESS_CONTROL_SHADER_BIT          = 0x00000008
    TESS_EVALUATION_SHADER_BIT       = 0x00000010
    ALL_SHADER_BITS                  = 0xFFFFFFFF
    PROGRAM_SEPARABLE                = 0x8258
    ACTIVE_PROGRAM                   = 0x8259
    PROGRAM_PIPELINE_BINDING         = 0x825A
    MAX_VIEWPORTS                    = 0x825B
    VIEWPORT_SUBPIXEL_BITS           = 0x825C
    VIEWPORT_BOUNDS_RANGE            = 0x825D
    LAYER_PROVOKING_VERTEX           = 0x825E
    VIEWPORT_INDEX_PROVOKING_VERTEX  = 0x825F
    UNDEFINED_VERTEX                 = 0x8260

    # --------------------------------------
    # OpenGL 4.2
    #
    UNPACK_COMPRESSED_BLOCK_WIDTH                              = 0x9127
    UNPACK_COMPRESSED_BLOCK_HEIGHT                             = 0x9128
    UNPACK_COMPRESSED_BLOCK_DEPTH                              = 0x9129
    UNPACK_COMPRESSED_BLOCK_SIZE                               = 0x912A
    PACK_COMPRESSED_BLOCK_WIDTH                                = 0x912B
    PACK_COMPRESSED_BLOCK_HEIGHT                               = 0x912C
    PACK_COMPRESSED_BLOCK_DEPTH                                = 0x912D
    PACK_COMPRESSED_BLOCK_SIZE                                 = 0x912E
    NUM_SAMPLE_COUNTS                                          = 0x9380
    MIN_MAP_BUFFER_ALIGNMENT                                   = 0x90BC
    ATOMIC_COUNTER_BUFFER                                      = 0x92C0
    ATOMIC_COUNTER_BUFFER_BINDING                              = 0x92C1
    ATOMIC_COUNTER_BUFFER_START                                = 0x92C2
    ATOMIC_COUNTER_BUFFER_SIZE                                 = 0x92C3
    ATOMIC_COUNTER_BUFFER_DATA_SIZE                            = 0x92C4
    ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS               = 0x92C5
    ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES        = 0x92C6
    ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER          = 0x92C7
    ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER    = 0x92C8
    ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER = 0x92C9
    ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER        = 0x92CA
    ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER        = 0x92CB
    MAX_VERTEX_ATOMIC_COUNTER_BUFFERS                          = 0x92CC
    MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS                    = 0x92CD
    MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS                 = 0x92CE
    MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS                        = 0x92CF
    MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS                        = 0x92D0
    MAX_COMBINED_ATOMIC_COUNTER_BUFFERS                        = 0x92D1
    MAX_VERTEX_ATOMIC_COUNTERS                                 = 0x92D2
    MAX_TESS_CONTROL_ATOMIC_COUNTERS                           = 0x92D3
    MAX_TESS_EVALUATION_ATOMIC_COUNTERS                        = 0x92D4
    MAX_GEOMETRY_ATOMIC_COUNTERS                               = 0x92D5
    MAX_FRAGMENT_ATOMIC_COUNTERS                               = 0x92D6
    MAX_COMBINED_ATOMIC_COUNTERS                               = 0x92D7
    MAX_ATOMIC_COUNTER_BUFFER_SIZE                             = 0x92D8
    MAX_ATOMIC_COUNTER_BUFFER_BINDINGS                         = 0x92DC
    ACTIVE_ATOMIC_COUNTER_BUFFERS                              = 0x92D9
    UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX                        = 0x92DA
    UNSIGNED_INT_ATOMIC_COUNTER                                = 0x92DB
    VERTEX_ATTRIB_ARRAY_BARRIER_BIT                            = 0x00000001
    ELEMENT_ARRAY_BARRIER_BIT                                  = 0x00000002
    UNIFORM_BARRIER_BIT                                        = 0x00000004
    TEXTURE_FETCH_BARRIER_BIT                                  = 0x00000008
    SHADER_IMAGE_ACCESS_BARRIER_BIT                            = 0x00000020
    COMMAND_BARRIER_BIT                                        = 0x00000040
    PIXEL_BUFFER_BARRIER_BIT                                   = 0x00000080
    TEXTURE_UPDATE_BARRIER_BIT                                 = 0x00000100
    BUFFER_UPDATE_BARRIER_BIT                                  = 0x00000200
    FRAMEBUFFER_BARRIER_BIT                                    = 0x00000400
    TRANSFORM_FEEDBACK_BARRIER_BIT                             = 0x00000800
    ATOMIC_COUNTER_BARRIER_BIT                                 = 0x00001000
    ALL_BARRIER_BITS                                           = 0xFFFFFFFF
    MAX_IMAGE_UNITS                                            = 0x8F38
    MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS              = 0x8F39
    IMAGE_BINDING_NAME                                         = 0x8F3A
    IMAGE_BINDING_LEVEL                                        = 0x8F3B
    IMAGE_BINDING_LAYERED                                      = 0x8F3C
    IMAGE_BINDING_LAYER                                        = 0x8F3D
    IMAGE_BINDING_ACCESS                                       = 0x8F3E
    IMAGE_1D                                                   = 0x904C
    IMAGE_2D                                                   = 0x904D
    IMAGE_3D                                                   = 0x904E
    IMAGE_2D_RECT                                              = 0x904F
    IMAGE_CUBE                                                 = 0x9050
    IMAGE_BUFFER                                               = 0x9051
    IMAGE_1D_ARRAY                                             = 0x9052
    IMAGE_2D_ARRAY                                             = 0x9053
    IMAGE_CUBE_MAP_ARRAY                                       = 0x9054
    IMAGE_2D_MULTISAMPLE                                       = 0x9055
    IMAGE_2D_MULTISAMPLE_ARRAY                                 = 0x9056
    INT_IMAGE_1D                                               = 0x9057
    INT_IMAGE_2D                                               = 0x9058
    INT_IMAGE_3D                                               = 0x9059
    INT_IMAGE_2D_RECT                                          = 0x905A
    INT_IMAGE_CUBE                                             = 0x905B
    INT_IMAGE_BUFFER                                           = 0x905C
    INT_IMAGE_1D_ARRAY                                         = 0x905D
    INT_IMAGE_2D_ARRAY                                         = 0x905E
    INT_IMAGE_CUBE_MAP_ARRAY                                   = 0x905F
    INT_IMAGE_2D_MULTISAMPLE                                   = 0x9060
    INT_IMAGE_2D_MULTISAMPLE_ARRAY                             = 0x9061
    UNSIGNED_INT_IMAGE_1D                                      = 0x9062
    UNSIGNED_INT_IMAGE_2D                                      = 0x9063
    UNSIGNED_INT_IMAGE_3D                                      = 0x9064
    UNSIGNED_INT_IMAGE_2D_RECT                                 = 0x9065
    UNSIGNED_INT_IMAGE_CUBE                                    = 0x9066
    UNSIGNED_INT_IMAGE_BUFFER                                  = 0x9067
    UNSIGNED_INT_IMAGE_1D_ARRAY                                = 0x9068
    UNSIGNED_INT_IMAGE_2D_ARRAY                                = 0x9069
    UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY                          = 0x906A
    UNSIGNED_INT_IMAGE_2D_MULTISAMPLE                          = 0x906B
    UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY                    = 0x906C
    MAX_IMAGE_SAMPLES                                          = 0x906D
    IMAGE_BINDING_FORMAT                                       = 0x906E
    IMAGE_FORMAT_COMPATIBILITY_TYPE                            = 0x90C7
    IMAGE_FORMAT_COMPATIBILITY_BY_SIZE                         = 0x90C8
    IMAGE_FORMAT_COMPATIBILITY_BY_CLASS                        = 0x90C9
    MAX_VERTEX_IMAGE_UNIFORMS                                  = 0x90CA
    MAX_TESS_CONTROL_IMAGE_UNIFORMS                            = 0x90CB
    MAX_TESS_EVALUATION_IMAGE_UNIFORMS                         = 0x90CC
    MAX_GEOMETRY_IMAGE_UNIFORMS                                = 0x90CD
    MAX_FRAGMENT_IMAGE_UNIFORMS                                = 0x90CE
    MAX_COMBINED_IMAGE_UNIFORMS                                = 0x90CF
    COMPRESSED_RGBA_BPTC_UNORM                                 = 0x8E8C
    COMPRESSED_SRGB_ALPHA_BPTC_UNORM                           = 0x8E8D
    COMPRESSED_RGB_BPTC_SIGNED_FLOAT                           = 0x8E8E
    COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT                         = 0x8E8F
    TEXTURE_IMMUTABLE_FORMAT                                   = 0x912F

    # --------------------------------------
    # OpenGL 4.3
    #
    NUM_SHADING_LANGUAGE_VERSIONS                      = 0x82E9
    VERTEX_ATTRIB_ARRAY_LONG                           = 0x874E
    COMPRESSED_RGB8_ETC2                               = 0x9274
    COMPRESSED_SRGB8_ETC2                              = 0x9275
    COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2           = 0x9276
    COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2          = 0x9277
    COMPRESSED_RGBA8_ETC2_EAC                          = 0x9278
    COMPRESSED_SRGB8_ALPHA8_ETC2_EAC                   = 0x9279
    COMPRESSED_R11_EAC                                 = 0x9270
    COMPRESSED_SIGNED_R11_EAC                          = 0x9271
    COMPRESSED_RG11_EAC                                = 0x9272
    COMPRESSED_SIGNED_RG11_EAC                         = 0x9273
    PRIMITIVE_RESTART_FIXED_INDEX                      = 0x8D69
    ANY_SAMPLES_PASSED_CONSERVATIVE                    = 0x8D6A
    MAX_ELEMENT_INDEX                                  = 0x8D6B
    COMPUTE_SHADER                                     = 0x91B9
    MAX_COMPUTE_UNIFORM_BLOCKS                         = 0x91BB
    MAX_COMPUTE_TEXTURE_IMAGE_UNITS                    = 0x91BC
    MAX_COMPUTE_IMAGE_UNIFORMS                         = 0x91BD
    MAX_COMPUTE_SHARED_MEMORY_SIZE                     = 0x8262
    MAX_COMPUTE_UNIFORM_COMPONENTS                     = 0x8263
    MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS                 = 0x8264
    MAX_COMPUTE_ATOMIC_COUNTERS                        = 0x8265
    MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS            = 0x8266
    MAX_COMPUTE_WORK_GROUP_INVOCATIONS                 = 0x90EB
    MAX_COMPUTE_WORK_GROUP_COUNT                       = 0x91BE
    MAX_COMPUTE_WORK_GROUP_SIZE                        = 0x91BF
    COMPUTE_WORK_GROUP_SIZE                            = 0x8267
    UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER         = 0x90EC
    ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER = 0x90ED
    DISPATCH_INDIRECT_BUFFER                           = 0x90EE
    DISPATCH_INDIRECT_BUFFER_BINDING                   = 0x90EF
    DEBUG_OUTPUT_SYNCHRONOUS                           = 0x8242
    DEBUG_NEXT_LOGGED_MESSAGE_LENGTH                   = 0x8243
    DEBUG_CALLBACK_FUNCTION                            = 0x8244
    DEBUG_CALLBACK_USER_PARAM                          = 0x8245
    DEBUG_SOURCE_API                                   = 0x8246
    DEBUG_SOURCE_WINDOW_SYSTEM                         = 0x8247
    DEBUG_SOURCE_SHADER_COMPILER                       = 0x8248
    DEBUG_SOURCE_THIRD_PARTY                           = 0x8249
    DEBUG_SOURCE_APPLICATION                           = 0x824A
    DEBUG_SOURCE_OTHER                                 = 0x824B
    DEBUG_TYPE_ERROR                                   = 0x824C
    DEBUG_TYPE_DEPRECATED_BEHAVIOR                     = 0x824D
    DEBUG_TYPE_UNDEFINED_BEHAVIOR                      = 0x824E
    DEBUG_TYPE_PORTABILITY                             = 0x824F
    DEBUG_TYPE_PERFORMANCE                             = 0x8250
    DEBUG_TYPE_OTHER                                   = 0x8251
    MAX_DEBUG_MESSAGE_LENGTH                           = 0x9143
    MAX_DEBUG_LOGGED_MESSAGES                          = 0x9144
    DEBUG_LOGGED_MESSAGES                              = 0x9145
    DEBUG_SEVERITY_HIGH                                = 0x9146
    DEBUG_SEVERITY_MEDIUM                              = 0x9147
    DEBUG_SEVERITY_LOW                                 = 0x9148
    DEBUG_TYPE_MARKER                                  = 0x8268
    DEBUG_TYPE_PUSH_GROUP                              = 0x8269
    DEBUG_TYPE_POP_GROUP                               = 0x826A
    DEBUG_SEVERITY_NOTIFICATION                        = 0x826B
    MAX_DEBUG_GROUP_STACK_DEPTH                        = 0x826C
    DEBUG_GROUP_STACK_DEPTH                            = 0x826D
    BUFFER                                             = 0x82E0
    SHADER                                             = 0x82E1
    PROGRAM                                            = 0x82E2
    QUERY                                              = 0x82E3
    PROGRAM_PIPELINE                                   = 0x82E4
    SAMPLER                                            = 0x82E6
    MAX_LABEL_LENGTH                                   = 0x82E8
    DEBUG_OUTPUT                                       = 0x92E0
    CONTEXT_FLAG_DEBUG_BIT                             = 0x00000002
    MAX_UNIFORM_LOCATIONS                              = 0x826E
    FRAMEBUFFER_DEFAULT_WIDTH                          = 0x9310
    FRAMEBUFFER_DEFAULT_HEIGHT                         = 0x9311
    FRAMEBUFFER_DEFAULT_LAYERS                         = 0x9312
    FRAMEBUFFER_DEFAULT_SAMPLES                        = 0x9313
    FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS         = 0x9314
    MAX_FRAMEBUFFER_WIDTH                              = 0x9315
    MAX_FRAMEBUFFER_HEIGHT                             = 0x9316
    MAX_FRAMEBUFFER_LAYERS                             = 0x9317
    MAX_FRAMEBUFFER_SAMPLES                            = 0x9318
    INTERNALFORMAT_SUPPORTED                           = 0x826F
    INTERNALFORMAT_PREFERRED                           = 0x8270
    INTERNALFORMAT_RED_SIZE                            = 0x8271
    INTERNALFORMAT_GREEN_SIZE                          = 0x8272
    INTERNALFORMAT_BLUE_SIZE                           = 0x8273
    INTERNALFORMAT_ALPHA_SIZE                          = 0x8274
    INTERNALFORMAT_DEPTH_SIZE                          = 0x8275
    INTERNALFORMAT_STENCIL_SIZE                        = 0x8276
    INTERNALFORMAT_SHARED_SIZE                         = 0x8277
    INTERNALFORMAT_RED_TYPE                            = 0x8278
    INTERNALFORMAT_GREEN_TYPE                          = 0x8279
    INTERNALFORMAT_BLUE_TYPE                           = 0x827A
    INTERNALFORMAT_ALPHA_TYPE                          = 0x827B
    INTERNALFORMAT_DEPTH_TYPE                          = 0x827C
    INTERNALFORMAT_STENCIL_TYPE                        = 0x827D
    MAX_WIDTH                                          = 0x827E
    MAX_HEIGHT                                         = 0x827F
    MAX_DEPTH                                          = 0x8280
    MAX_LAYERS                                         = 0x8281
    MAX_COMBINED_DIMENSIONS                            = 0x8282
    COLOR_COMPONENTS                                   = 0x8283
    DEPTH_COMPONENTS                                   = 0x8284
    STENCIL_COMPONENTS                                 = 0x8285
    COLOR_RENDERABLE                                   = 0x8286
    DEPTH_RENDERABLE                                   = 0x8287
    STENCIL_RENDERABLE                                 = 0x8288
    FRAMEBUFFER_RENDERABLE                             = 0x8289
    FRAMEBUFFER_RENDERABLE_LAYERED                     = 0x828A
    FRAMEBUFFER_BLEND                                  = 0x828B
    READ_PIXELS                                        = 0x828C
    READ_PIXELS_FORMAT                                 = 0x828D
    READ_PIXELS_TYPE                                   = 0x828E
    TEXTURE_IMAGE_FORMAT                               = 0x828F
    TEXTURE_IMAGE_TYPE                                 = 0x8290
    GET_TEXTURE_IMAGE_FORMAT                           = 0x8291
    GET_TEXTURE_IMAGE_TYPE                             = 0x8292
    MIPMAP                                             = 0x8293
    MANUAL_GENERATE_MIPMAP                             = 0x8294
    AUTO_GENERATE_MIPMAP                               = 0x8295
    COLOR_ENCODING                                     = 0x8296
    SRGB_READ                                          = 0x8297
    SRGB_WRITE                                         = 0x8298
    FILTER                                             = 0x829A
    VERTEX_TEXTURE                                     = 0x829B
    TESS_CONTROL_TEXTURE                               = 0x829C
    TESS_EVALUATION_TEXTURE                            = 0x829D
    GEOMETRY_TEXTURE                                   = 0x829E
    FRAGMENT_TEXTURE                                   = 0x829F
    COMPUTE_TEXTURE                                    = 0x82A0
    TEXTURE_SHADOW                                     = 0x82A1
    TEXTURE_GATHER                                     = 0x82A2
    TEXTURE_GATHER_SHADOW                              = 0x82A3
    SHADER_IMAGE_LOAD                                  = 0x82A4
    SHADER_IMAGE_STORE                                 = 0x82A5
    SHADER_IMAGE_ATOMIC                                = 0x82A6
    IMAGE_TEXEL_SIZE                                   = 0x82A7
    IMAGE_COMPATIBILITY_CLASS                          = 0x82A8
    IMAGE_PIXEL_FORMAT                                 = 0x82A9
    IMAGE_PIXEL_TYPE                                   = 0x82AA
    SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST                = 0x82AC
    SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST              = 0x82AD
    SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE               = 0x82AE
    SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE             = 0x82AF
    TEXTURE_COMPRESSED_BLOCK_WIDTH                     = 0x82B1
    TEXTURE_COMPRESSED_BLOCK_HEIGHT                    = 0x82B2
    TEXTURE_COMPRESSED_BLOCK_SIZE                      = 0x82B3
    CLEAR_BUFFER                                       = 0x82B4
    TEXTURE_VIEW                                       = 0x82B5
    VIEW_COMPATIBILITY_CLASS                           = 0x82B6
    FULL_SUPPORT                                       = 0x82B7
    CAVEAT_SUPPORT                                     = 0x82B8
    IMAGE_CLASS_4_X_32                                 = 0x82B9
    IMAGE_CLASS_2_X_32                                 = 0x82BA
    IMAGE_CLASS_1_X_32                                 = 0x82BB
    IMAGE_CLASS_4_X_16                                 = 0x82BC
    IMAGE_CLASS_2_X_16                                 = 0x82BD
    IMAGE_CLASS_1_X_16                                 = 0x82BE
    IMAGE_CLASS_4_X_8                                  = 0x82BF
    IMAGE_CLASS_2_X_8                                  = 0x82C0
    IMAGE_CLASS_1_X_8                                  = 0x82C1
    IMAGE_CLASS_11_11_10                               = 0x82C2
    IMAGE_CLASS_10_10_10_2                             = 0x82C3
    VIEW_CLASS_128_BITS                                = 0x82C4
    VIEW_CLASS_96_BITS                                 = 0x82C5
    VIEW_CLASS_64_BITS                                 = 0x82C6
    VIEW_CLASS_48_BITS                                 = 0x82C7
    VIEW_CLASS_32_BITS                                 = 0x82C8
    VIEW_CLASS_24_BITS                                 = 0x82C9
    VIEW_CLASS_16_BITS                                 = 0x82CA
    VIEW_CLASS_8_BITS                                  = 0x82CB
    VIEW_CLASS_S3TC_DXT1_RGB                           = 0x82CC
    VIEW_CLASS_S3TC_DXT1_RGBA                          = 0x82CD
    VIEW_CLASS_S3TC_DXT3_RGBA                          = 0x82CE
    VIEW_CLASS_S3TC_DXT5_RGBA                          = 0x82CF
    VIEW_CLASS_RGTC1_RED                               = 0x82D0
    VIEW_CLASS_RGTC2_RG                                = 0x82D1
    VIEW_CLASS_BPTC_UNORM                              = 0x82D2
    VIEW_CLASS_BPTC_FLOAT                              = 0x82D3
    UNIFORM                                            = 0x92E1
    UNIFORM_BLOCK                                      = 0x92E2
    PROGRAM_INPUT                                      = 0x92E3
    PROGRAM_OUTPUT                                     = 0x92E4
    BUFFER_VARIABLE                                    = 0x92E5
    SHADER_STORAGE_BLOCK                               = 0x92E6
    VERTEX_SUBROUTINE                                  = 0x92E8
    TESS_CONTROL_SUBROUTINE                            = 0x92E9
    TESS_EVALUATION_SUBROUTINE                         = 0x92EA
    GEOMETRY_SUBROUTINE                                = 0x92EB
    FRAGMENT_SUBROUTINE                                = 0x92EC
    COMPUTE_SUBROUTINE                                 = 0x92ED
    VERTEX_SUBROUTINE_UNIFORM                          = 0x92EE
    TESS_CONTROL_SUBROUTINE_UNIFORM                    = 0x92EF
    TESS_EVALUATION_SUBROUTINE_UNIFORM                 = 0x92F0
    GEOMETRY_SUBROUTINE_UNIFORM                        = 0x92F1
    FRAGMENT_SUBROUTINE_UNIFORM                        = 0x92F2
    COMPUTE_SUBROUTINE_UNIFORM                         = 0x92F3
    TRANSFORM_FEEDBACK_VARYING                         = 0x92F4
    ACTIVE_RESOURCES                                   = 0x92F5
    MAX_NAME_LENGTH                                    = 0x92F6
    MAX_NUM_ACTIVE_VARIABLES                           = 0x92F7
    MAX_NUM_COMPATIBLE_SUBROUTINES                     = 0x92F8
    NAME_LENGTH                                        = 0x92F9
    TYPE                                               = 0x92FA
    ARRAY_SIZE                                         = 0x92FB
    OFFSET                                             = 0x92FC
    BLOCK_INDEX                                        = 0x92FD
    ARRAY_STRIDE                                       = 0x92FE
    MATRIX_STRIDE                                      = 0x92FF
    IS_ROW_MAJOR                                       = 0x9300
    ATOMIC_COUNTER_BUFFER_INDEX                        = 0x9301
    BUFFER_BINDING                                     = 0x9302
    BUFFER_DATA_SIZE                                   = 0x9303
    NUM_ACTIVE_VARIABLES                               = 0x9304
    ACTIVE_VARIABLES                                   = 0x9305
    REFERENCED_BY_VERTEX_SHADER                        = 0x9306
    REFERENCED_BY_TESS_CONTROL_SHADER                  = 0x9307
    REFERENCED_BY_TESS_EVALUATION_SHADER               = 0x9308
    REFERENCED_BY_GEOMETRY_SHADER                      = 0x9309
    REFERENCED_BY_FRAGMENT_SHADER                      = 0x930A
    REFERENCED_BY_COMPUTE_SHADER                       = 0x930B
    TOP_LEVEL_ARRAY_SIZE                               = 0x930C
    TOP_LEVEL_ARRAY_STRIDE                             = 0x930D
    LOCATION                                           = 0x930E
    LOCATION_INDEX                                     = 0x930F
    IS_PER_PATCH                                       = 0x92E7
    SHADER_STORAGE_BUFFER                              = 0x90D2
    SHADER_STORAGE_BUFFER_BINDING                      = 0x90D3
    SHADER_STORAGE_BUFFER_START                        = 0x90D4
    SHADER_STORAGE_BUFFER_SIZE                         = 0x90D5
    MAX_VERTEX_SHADER_STORAGE_BLOCKS                   = 0x90D6
    MAX_GEOMETRY_SHADER_STORAGE_BLOCKS                 = 0x90D7
    MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS             = 0x90D8
    MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS          = 0x90D9
    MAX_FRAGMENT_SHADER_STORAGE_BLOCKS                 = 0x90DA
    MAX_COMPUTE_SHADER_STORAGE_BLOCKS                  = 0x90DB
    MAX_COMBINED_SHADER_STORAGE_BLOCKS                 = 0x90DC
    MAX_SHADER_STORAGE_BUFFER_BINDINGS                 = 0x90DD
    MAX_SHADER_STORAGE_BLOCK_SIZE                      = 0x90DE
    SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT             = 0x90DF
    SHADER_STORAGE_BARRIER_BIT                         = 0x00002000
    MAX_COMBINED_SHADER_OUTPUT_RESOURCES               = 0x8F39
    DEPTH_STENCIL_TEXTURE_MODE                         = 0x90EA
    TEXTURE_BUFFER_OFFSET                              = 0x919D
    TEXTURE_BUFFER_SIZE                                = 0x919E
    TEXTURE_BUFFER_OFFSET_ALIGNMENT                    = 0x919F
    TEXTURE_VIEW_MIN_LEVEL                             = 0x82DB
    TEXTURE_VIEW_NUM_LEVELS                            = 0x82DC
    TEXTURE_VIEW_MIN_LAYER                             = 0x82DD
    TEXTURE_VIEW_NUM_LAYERS                            = 0x82DE
    TEXTURE_IMMUTABLE_LEVELS                           = 0x82DF
    VERTEX_ATTRIB_BINDING                              = 0x82D4
    VERTEX_ATTRIB_RELATIVE_OFFSET                      = 0x82D5
    VERTEX_BINDING_DIVISOR                             = 0x82D6
    VERTEX_BINDING_OFFSET                              = 0x82D7
    VERTEX_BINDING_STRIDE                              = 0x82D8
    MAX_VERTEX_ATTRIB_RELATIVE_OFFSET                  = 0x82D9
    MAX_VERTEX_ATTRIB_BINDINGS                         = 0x82DA
    VERTEX_BINDING_BUFFER                              = 0x8F4F
    DISPLAY_LIST                                       = 0x82E7

    # --------------------------------------
    # OpenGL 4.4
    #
    MAX_VERTEX_ATTRIB_STRIDE                = 0x82E5
    PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED = 0x8221
    TEXTURE_BUFFER_BINDING                  = 0x8C2A
    MAP_PERSISTENT_BIT                      = 0x0040
    MAP_COHERENT_BIT                        = 0x0080
    DYNAMIC_STORAGE_BIT                     = 0x0100
    CLIENT_STORAGE_BIT                      = 0x0200
    CLIENT_MAPPED_BUFFER_BARRIER_BIT        = 0x00004000
    BUFFER_IMMUTABLE_STORAGE                = 0x821F
    BUFFER_STORAGE_FLAGS                    = 0x8220
    CLEAR_TEXTURE                           = 0x9365
    LOCATION_COMPONENT                      = 0x934A
    TRANSFORM_FEEDBACK_BUFFER_INDEX         = 0x934B
    TRANSFORM_FEEDBACK_BUFFER_STRIDE        = 0x934C
    QUERY_BUFFER                            = 0x9192
    QUERY_BUFFER_BARRIER_BIT                = 0x00008000
    QUERY_BUFFER_BINDING                    = 0x9193
    QUERY_RESULT_NO_WAIT                    = 0x9194
    MIRROR_CLAMP_TO_EDGE                    = 0x8743

def Accum(unsigned int op, float value):
    GL_Accum(op, value)

def ActiveShaderProgram(unsigned int pipeline, unsigned int program):
    GL_ActiveShaderProgram(pipeline, program)

def ActiveTexture(unsigned int texture):
    GL_ActiveTexture(texture)

def AlphaFunc(unsigned int func, float ref):
    GL_AlphaFunc(func, ref)

def AreTexturesResident(int n, size_t tex, size_t res):
    # TODO: use Array for texture and residence arrays? IsTextureResident wrapper?
    return GL_AreTexturesResident(n, <const unsigned int*>tex, <unsigned char*>res)

def ArrayElement(int i):
    GL_ArrayElement(i)

def AttachShader(unsigned int program, unsigned int shader):
    GL_AttachShader(program, shader)

def Begin(unsigned int mode):
    GL_Begin(mode)

def BeginConditionalRender(unsigned int id, unsigned int mode):
    GL_BeginConditionalRender(id, mode)

def BeginQuery(unsigned int target, unsigned int id):
    GL_BeginQuery(target, id)

def BeginQueryIndexed(unsigned int target, unsigned int index, unsigned int id):
    GL_BeginQueryIndexed(target, index, id)

def BeginTransformFeedback(unsigned int primitive_mode):
    GL_BeginTransformFeedback(primitive_mode)

def BindAttribLocation(unsigned int program, unsigned int index, str name):
    cdef bytes b_name

    if sys.version_info.major > 2:
        b_name = <bytes>name.encode('utf-8')
    else:
        b_name = <bytes>name

    GL_BindAttribLocation(program, index, <const char*>b_name)

def BindBuffer(unsigned int target, unsigned int buf):
    GL_BindBuffer(target, buf)

def BindTexture(unsigned int target, unsigned int texture):
    GL_BindTexture(target, texture)

def BindVertexArray(unsigned int array):
    GL_BindVertexArray(array)

def Bitmap(int width, int height, float x_orig, float y_orig,
                float x_move, float y_move, size_t bitmap_p):
    # TODO: use Image?
    GL_Bitmap(width, height, x_orig, y_orig, x_move, y_move,
                            <const unsigned char *>bitmap_p)

def BlendColor(float red, float green, float blue, float alpha):
    GL_BlendColor(red, green, blue, alpha)

def BlendEquation(unsigned int mode):
    GL_BlendEquation(mode)

def BlendEquationSeparate(unsigned int modeRGB, unsigned int modeA):
    GL_BlendEquationSeparate(modeRGB, modeA)

def BlendFunc(unsigned int src, unsigned int dst):
    GL_BlendFunc(src, dst)

def BlendFuncSeparate(unsigned sRGB, unsigned dRGB, unsigned sA, unsigned dA):
    GL_BlendFuncSeparate(sRGB, dRGB, sA, dA)

def BufferData(unsigned int target, ptrdiff_t size, size_t data, unsigned int usage):
    GL_BufferData(target, size, <const void*>data, usage)

def CallList(unsigned int list_id):
    GL_CallList(list_id)

def CallLists(int n, unsigned int type, size_t lists):
    GL_CallLists(n, type, <const void*>lists) # TODO: use Array?

def Clear(unsigned int mask):
    GL_Clear(mask)

def ClearAccum(float r, float g, float b, float a):
    GL_ClearAccum(r, g, b, a)

def ClearColor(float r, float g, float b, float a):
    GL_ClearColor(r, g, b, a)

def ClearDepth(double depth):
    GL_ClearDepth(depth)

def ClearIndex(float c):
    GL_ClearIndex(c)

def ClearStencil(int s):
    GL_ClearStencil(s)

def ClipPlane(unsigned int plane, size_t equation):
    GL_ClipPlane(plane, < const double* >equation)

def Color3b(char red, char green, char blue):
    GL_Color3b(red, green, blue)

def Color3bv(size_t v):
    GL_Color3bv(<const char*>v)

def Color3d(double red, double green, double blue):
    GL_Color3d(red, green, blue)

def Color3dv(size_t v):
    GL_Color3dv(<const double*>v)

def Color3f(float r, float g, float b):
    GL_Color3f(r, g, b)

def Color3fv(Vec3 rgb):
    GL_Color3fv(rgb.v)

def Color3i(int red, int green, int blue):
    GL_Color3i(red, green, blue)

def Color3iv(size_t v):
    GL_Color3iv(<const int*>v)

def Color3s(short red, short green, short blue):
    GL_Color3s(red, green, blue)

def Color3sv(size_t v):
    GL_Color3sv(<const short*>v)

def Color3ub(unsigned char red, unsigned char green, unsigned char blue):
    GL_Color3ub(red, green, blue)

def Color3ubv(size_t v):
    GL_Color3ubv(<const unsigned char*>v)

def Color3ui(unsigned int red, unsigned int green, unsigned int blue):
    GL_Color3ui(red, green, blue)

def Color3uiv(size_t v):
    GL_Color3uiv(<const unsigned int*>v)

def Color3us(unsigned short red, unsigned short green, unsigned short blue):
    GL_Color3us(red, green, blue)

def Color3usv(size_t v):
    GL_Color3usv(<const unsigned short*>v)

def Color4b(char red, char green, char blue, char alpha):
    GL_Color4b(red, green, blue, alpha)

def Color4bv(size_t v):
    GL_Color4bv(<const char*>v)

def Color4d(double red, double green, double blue, double alpha):
    GL_Color4d(red, green, blue, alpha)

def Color4dv(size_t v):
    GL_Color4dv(<const double*>v)

def Color4f(float r, float g, float b, float a):
    GL_Color4f(r, g, b, a)

def Color4fv(Vec4 rgba):
    GL_Color4fv(rgba.v)

def Color4i(int red, int green, int blue, int alpha):
    GL_Color4i(red, green, blue, alpha)

def Color4iv(size_t v):
    GL_Color4iv(<const int*>v)

def Color4s(short red, short green, short blue, short alpha):
    GL_Color4s(red, green, blue, alpha)

def Color4sv(size_t v):
    GL_Color4sv(<const short*>v)

def Color4ub(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha):
    GL_Color4ub(red, green, blue, alpha)

def Color4ubv(size_t v):
    GL_Color4ubv(<const unsigned char*>v)

def Color4ui(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha):
    GL_Color4ui(red, green, blue, alpha)

def Color4uiv(size_t v):
    GL_Color4uiv(<const unsigned int*>v)

def Color4us(unsigned short red, unsigned short green, unsigned short blue, unsigned short alpha):
    GL_Color4us(red, green, blue, alpha)

def Color4usv(size_t v):
    GL_Color4usv(<const unsigned short*>v)

def ColorMask(unsigned char r, unsigned char g, unsigned char b, unsigned char a):
    GL_ColorMask(r, g, b, a)

def ColorMaterial(unsigned int face, unsigned int mode):
    GL_ColorMaterial(face, mode)

def ColorPointer(int size, unsigned int type_id, int stride, size_t data):
    GL_ColorPointer(size, type_id, stride, <const void*>data)

def CompileShader(unsigned int shader):
    GL_CompileShader(shader)

def CopyPixels(int x, int y, int width, int height, unsigned int type_id):
    GL_CopyPixels(x, y, width, height, type_id)

def CopyTexImage1D( unsigned int target, int level, unsigned int internal_format,
                                            int x, int y, int width, int border):
    GL_CopyTexImage1D(target, level, internal_format, x, y, width, border)

def CopyTexImage2D( unsigned int target, int level, unsigned int internal_format,
                                int x, int y, int width, int height, int border):
    GL_CopyTexImage2D(target, level, internal_format, x, y, width, height, border)

def CopyTexSubImage1D(unsigned int target, int level, int xoffset,
                                        int x, int y, int width):
    GL_CopyTexSubImage1D(target, level, xoffset, x, y, width)

def CopyTexSubImage2D(unsigned int target, int level, int xoffset,
                int yoffset, int x, int y, int width, int height):
    GL_CopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height)

def CreateProgram():
    return GL_CreateProgram()

def CreateShader(unsigned int shader_type):
    return GL_CreateShader(shader_type)

def CullFace(unsigned int face):
    GL_CullFace(face)

def DeleteBuffers(int n, size_t buffers):
    GL_DeleteBuffers(n, <const unsigned int*>buffers) # TODO: use Array here?

def DeleteLists(unsigned int id_list, int id_range):
    GL_DeleteLists(id_list, id_range) # names altered to avoid builtin conflicts

def DeleteProgram(unsigned int program):
    GL_DeleteProgram(program) # TODO: wrap in a plural form (GL_DeletePrograms)?

def DeleteTextures(int n, size_t textures):
    GL_DeleteTextures(n, <const unsigned int*>textures) # TODO: use Array here?

def DeleteVertexArrays(int n, size_t arrays):
    GL_DeleteVertexArrays(n, <const unsigned int*>arrays) # TODO: use Array here?

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

def DisableVertexAttribArray(unsigned int index):
    GL_DisableVertexAttribArray(index)

def DrawArrays(unsigned int mode, int first, int count):
    GL_DrawArrays(mode, first, count)

def DrawBuffer(unsigned int mode):
    GL_DrawBuffer(mode)

def DrawElements(unsigned int mode, int num, unsigned int itype, size_t inds):
    GL_DrawElements(mode, num, itype, <const void*>inds) # TODO: use an Array?

def DrawPixels(int w, int h, unsigned int fmt, unsigned int type_id, size_t data):
    GL_DrawPixels(w, h, fmt, type_id, <const void*>data) # TODO: use an Image?

def EdgeFlag(unsigned char flag):
    GL_EdgeFlag(flag)

def EdgeFlagPointer(int stride, size_t ptr):
    GL_EdgeFlagPointer(stride, <const void*>ptr)

def EdgeFlagv(size_t flag):
    GL_EdgeFlagv(<const unsigned char*>flag)

def Enable(unsigned int cap):
    GL_Enable(cap)

def EnableClientState(unsigned int cap):
    GL_EnableClientState(cap)

def EnableVertexAttribArray(unsigned int index):
    GL_EnableVertexAttribArray(index)

def End():
    GL_End()

def EndConditionalRender():
    GL_EndConditionalRender()

def EndList():
    GL_EndList()

def EndQuery(unsigned int target):
    GL_EndQuery(target)

def EndQueryIndexed(unsigned int target, unsigned int index):
    GL_EndQueryIndexed(target, index)

def EndTransformFeedback():
    GL_EndTransformFeedback()

def EvalCoord1d(double u):
    GL_EvalCoord1d(u)

def EvalCoord1dv(size_t u):
    GL_EvalCoord1dv(<const double*>u)

def EvalCoord1f(float u):
    GL_EvalCoord1f(u)

def EvalCoord1fv(size_t u):
    GL_EvalCoord1fv(<const float*>u)

def EvalCoord2d(double u, double v):
    GL_EvalCoord2d(u, v)

def EvalCoord2dv(size_t u):
    GL_EvalCoord2dv(<const double*>u)

def EvalCoord2f(float u, float v):
    GL_EvalCoord2f(u, v)

def EvalCoord2fv(Vec2 u):
    GL_EvalCoord2fv(u.v)

def EvalMesh1(unsigned int mode, int i1, int i2):
    GL_EvalMesh1(mode, i1, i2)

def EvalMesh2(unsigned int mode, int i1, int i2, int j1, int j2):
    GL_EvalMesh2(mode, i1, i2, j1, j2)

def EvalPoint1(int i):
    GL_EvalPoint1(i)

def EvalPoint2(int i, int j):
    GL_EvalPoint2(i, j)

def FeedbackBuffer(int size, unsigned int b_type, size_t data):
    GL_FeedbackBuffer(size, b_type, <float*>data)

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

def GenBuffers(int n, size_t buffers):
    GL_GenBuffers(n, <unsigned int*>buffers) # TODO: use Array?

def GenLists(int list_range):
    return GL_GenLists(list_range) # TODO: wrap in a singular form

def GenTextures(int n, size_t textures):
    GL_GenTextures(n, <unsigned int*>textures) # TODO: use Array?

def GenVertexArrays(int n, size_t arrays):
    GL_GenVertexArrays(n, <unsigned int*>arrays) # TODO: use Array?

def GetAttribLocation(unsigned int program, str name):
    cdef bytes b_name

    if sys.version_info.major > 2:
        b_name = <bytes>name.encode('utf-8')
    else:
        b_name = <bytes>name

    return GL_GetAttribLocation(program, <const char*>b_name)

def GetBooleanv(unsigned int pname, size_t params):
    GL_GetBooleanv(pname, <unsigned char *>params)

def GetClipPlane(unsigned int plane, size_t equation):
    GL_GetClipPlane(plane, <double *>equation)

def GetDoublev(unsigned int pname, size_t params):
    GL_GetDoublev(pname, <double *>params)

def GetError():
    return GL_GetError()

def GetFloatv(unsigned int pname, size_t params):
    GL_GetFloatv(pname, <float*>params)

def GetIntegerv(unsigned int pname, size_t params):
    GL_GetIntegerv(pname, <int*>params)

def GetLightfv(unsigned int light, unsigned int pname, size_t params):
    GL_GetLightfv(light, pname, <float*>params)

def GetLightiv(unsigned int light, unsigned int pname, size_t params):
    GL_GetLightiv(light, pname, <int*>params)

def GetMapdv(unsigned int target, unsigned int query, size_t v):
    GL_GetMapdv(target, query, <double*>v)

def GetMapfv(unsigned int target, unsigned int query, size_t v):
    GL_GetMapfv(target, query, <float*>v)

def GetMapiv(unsigned int target, unsigned int query, size_t v):
    GL_GetMapiv(target, query, <int*>v)

def GetMaterialfv(unsigned int face, unsigned int pname, size_t params):
    GL_GetMaterialfv(face, pname, <float*>params)

def GetMaterialiv(unsigned int face, unsigned int pname, size_t params):
    GL_GetMaterialiv(face, pname, <int*>params)

def GetPixelMapfv(unsigned int pmap, size_t values):
    GL_GetPixelMapfv(pmap, <float*>values)

def GetPixelMapuiv(unsigned int pmap, size_t values):
    GL_GetPixelMapuiv(pmap, <unsigned int *> values)

def GetPixelMapusv(unsigned int pmap, size_t values):
    GL_GetPixelMapusv(pmap, <unsigned short*>values)

def GetPointerv(unsigned int pname, size_t params):
    GL_GetPointerv(pname, <void **> params)

def GetPolygonStipple(size_t mask):
    GL_GetPolygonStipple(<unsigned char *>mask)

def GetProgramiv(unsigned int program, unsigned int pname, size_t params):
    GL_GetProgramiv(program, pname, <int*>params)

# TODO: GetProgramInfoLog (get info log length and allocate string)

def GetShaderiv(unsigned int shader, unsigned int pname, size_t params):
    GL_GetShaderiv(shader, pname, <int*>params)

# TODO: GetShaderInfoLog (get info log length and allocate string)

def GetString(unsigned int name):
    cdef bytes b = GL_GetString(name) # convert to unicode
    return b.decode() if sys.version_info.major > 2 else b

def GetTexEnvfv(unsigned int target, unsigned int pname, size_t params):
    GL_GetTexEnvfv(target, pname, <float*>params)

def GetTexEnviv(unsigned int target, unsigned int pname, size_t params):
    GL_GetTexEnviv(target, pname, <int*>params)

def GetTexGendv(unsigned int coord, unsigned int pname, size_t params):
    GL_GetTexGendv(coord, pname, <double*>params)

def GetTexGenfv(unsigned int coord, unsigned int pname, size_t params):
    GL_GetTexGenfv(coord, pname, <float*>params)

def GetTexGeniv(unsigned int coord, unsigned int pname, size_t params):
    GL_GetTexGeniv(coord, pname, <int*>params)

def GetTexParameterfv(unsigned int target, unsigned int pname, size_t params):
    GL_GetTexParameterfv(target, pname, <float*>params)

def GetTexParameteriv(unsigned int target, unsigned int pname, size_t params):
    GL_GetTexParameteriv(target, pname, <int*>params)

def Hint(unsigned int target, unsigned int mode):
    GL_Hint(target, mode)

def InterleavedArrays(unsigned int format, int stride, size_t data):
    GL_InterleavedArrays(format, stride, <const void*>data)

def IsBuffer(unsigned int buffer_id):
    return GL_IsBuffer(buffer_id)

def IsEnabled(unsigned int cap):
    return GL_IsEnabled(cap)

def IsList(unsigned int list_id):
    return GL_IsList(list_id)

def IsProgram(unsigned int program_id):
    return GL_IsProgram(program_id)

def IsShader(unsigned int shader_id):
    return GL_IsShader(shader_id)

def IsTexture(unsigned int texture_id):
    return GL_IsTexture(texture_id)

def IsVertexArray(unsigned int array_id):
    return GL_IsVertexArray(array_id)

def Lightfv(unsigned int light, unsigned int pname, size_t params):
    GL_Lightfv(light, pname, <const float*>params)

def Lightf(unsigned int light, unsigned int pname, float param):
    GL_Lightf(light, pname, param)

def LineWidth(float width):
    GL_LineWidth(width)

def LinkProgram(unsigned int program):
    GL_LinkProgram(program)

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

def PrioritizeTextures(int n, size_t textures, size_t priorities):
    # TODO: use Array for textures and priorities arrays? GL_PrioritizeTexture wrapper?
    GL_PrioritizeTextures(n, <const unsigned int *>textures, <const float *>priorities)

def ProvokingVertex(unsigned int mode):
    GL_ProvokingVertex(mode)

def RenderMode(unsigned int mode):
    return GL_RenderMode(mode)

def Rotatef(float theta, float x, float y, float z):
    GL_Rotatef(theta, x, y, z)

def Scalef(float x, float y, float z):
    GL_Scalef(x, y, z)

def Scissor(int x, int y, int width, int height):
    GL_Scissor(x, y, width, height)

def ShadeModel(unsigned int mode):
    GL_ShadeModel(mode)

# TODO: ShaderSource (handle source string argument and length argument)

def TexCoord2f(float s, float t):
    GL_TexCoord2f(s, t)

def TexCoord2fv(Vec2 st):
    GL_TexCoord2fv(st.v)

def TexCoord3f(float s, float t, float r):
    GL_TexCoord3f(s, t, r)

def TexCoord3fv(Vec3 str):
    GL_TexCoord3fv(str.v)

def TexCoord4f(float s, float t, float r, float q):
    GL_TexCoord4f(s, t, r, q)

def TexCoord4fv(Vec4 strq):
    GL_TexCoord4fv(strq.v)

def TexCoordPointer(int size, unsigned int type, int stride, size_t data):
    GL_TexCoordPointer(size, type, stride, <const void*>data)

def TexEnvfv(unsigned int target, unsigned int pname, size_t params):
    GL_TexEnvfv(target, pname, <const float*>params)

def TexEnviv(unsigned int target, unsigned int pname, size_t params):
    GL_TexEnviv(target, pname, <const int*>params)

def TexEnvf(unsigned int target, unsigned int pname, float param):
    GL_TexEnvf(target, pname, param)

def TexEnvi(unsigned int target, unsigned int pname, int param):
    GL_TexEnvi(target, pname, param)

def TexGeniv(unsigned int coord, unsigned int pname, size_t params):
    GL_TexGeniv(coord, pname, <const int*>params)

def TexGenfv(unsigned int coord, unsigned int pname, size_t params):
    GL_TexGenfv(coord, pname, <const float*>params)

def TexGendv(unsigned int coord, unsigned int pname, size_t params):
    GL_TexGendv(coord, pname, <const double*>params)

def TexGeni(unsigned int coord, unsigned int pname, int param):
    GL_TexGeni(coord, pname, param)

def TexGenf(unsigned int coord, unsigned int pname, float param):
    GL_TexGenf(coord, pname, param)

def TexGend(unsigned int coord, unsigned int pname, double param):
    GL_TexGend(coord, pname, param)

def TexImage2D( unsigned int target, int level, int internal_format, int w, int h, int border,
                                            unsigned int fmt, unsigned int ptype, size_t pix):
    # TODO: take Image arg instead?
    GL_TexImage2D(target, level, internal_format, w, h, border, fmt, ptype, <const void*>pix)

def TexParameteri(unsigned int target, unsigned int pname, int param):
    GL_TexParameteri(target, pname, param)

def TexSubImage2D( unsigned int target, int level, int xoffset, int yoffset, int w, int h,
                                        unsigned int fmt, unsigned int ptype, size_t pix):
    # TODO: take Image arg instead?
    GL_TexSubImage2D(target, level, xoffset, yoffset, w, h, fmt, ptype, <const void*>pix)

def Translatef(float x, float y, float z):
    GL_Translatef(x, y, z)

def UseProgram(unsigned int program):
    GL_UseProgram(program)

def Vertex2f(float x, float y):
    GL_Vertex2f(x, y)

def Vertex2fv(Vec2 xy):
    GL_Vertex2fv(xy.v)

def Vertex3f(float x, float y, float z):
    GL_Vertex3f(x, y, z)

def Vertex3fv(Vec3 xyz):
    GL_Vertex3fv(xyz.v)

def Vertex4f(float x, float y, float z, float w):
    GL_Vertex4f(x, y, z, w)

def Vertex4fv(Vec4 xyzw):
    GL_Vertex4fv(xyzw.v)

def VertexAttribPointer(unsigned int index, int size, unsigned int type,
                    unsigned char normalized, int stride, size_t d_ptr):
    # TODO: take Array arg instead?
    GL_VertexAttribPointer(index, size, type, normalized, stride,
                                            < const void* >d_ptr)

def VertexPointer(int size, unsigned int type, int stride, size_t data):
    GL_VertexPointer(size, type, stride, <const void*>data)

def Viewport(int x, int y, int width, int height):
    GL_Viewport(x, y, width, height)

# ==============================================================================
# ~ [ GL state logging ]
# ==============================================================================
# TODO: convert _ex state logging methods to handle unicode strings for python 3
# ------------------------------------------------------------------------------

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

    # ===== [ 2D shapes ] ======================================================

    @staticmethod
    def rect_ex(FloatRect rect, Vec4 bl, Vec4 br, Vec4 tr, Vec4 tl,
                bint line, float line_width):
        """
        Draw the standard "Hello OpenGL" gradient quadrilateral, or something.
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

    # ===== [ 3D shapes ] ======================================================

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
    """
    GL transform coordinate with position, euler rotation angles, and scale.
    If a transform is absolute, it starts from the origin (identity matrix).
    """
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
    def print_all(): gl_object_print_all()

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

        cdef str nstr # TODO: move all string code after unknown detection

        if object_type == GL_OBJECT_TYPE_UNKNOWN:
            return self

        # XXX: is there a way to detect version num without calling python?
        if sys.version_info.major > 2:
            nstr = name.decode() # convert ascii type name to UTF for py3k
        else:
            nstr = <str>name # keep the oldschool byte string for python 2

        return globals()[nstr.replace('_', ' ').title().replace(' ', '')] \
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
    """
    An image resident in server-side (GPU) memory for fast hardware rendering.
    """
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
    def print_all(): gl_texture_print_all()

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
            cdef bytes s = gl_texture_get_str(self.texture, GL_TEXTURE_PROPERTY_STATUS)
            return s.decode() if sys.version_info.major > 2 else s # convert to unicode

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_texture_set_str(self.texture, GL_TEXTURE_PROPERTY_STATUS, <char*>string)

    property path:
        def __get__(self):
            cdef bytes s = gl_texture_get_str(self.texture, GL_TEXTURE_PROPERTY_PATH)
            return s.decode() if sys.version_info.major > 2 else s # convert to utf-8

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_texture_set_str(self.texture, GL_TEXTURE_PROPERTY_PATH, <char*>string)

    property name:
        def __get__(self):
            cdef bytes s = gl_texture_get_str(self.texture, GL_TEXTURE_PROPERTY_NAME)
            return s.decode() if sys.version_info.major > 2 else s # convert to utf-8

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_texture_set_str(self.texture, GL_TEXTURE_PROPERTY_NAME, <char*>string)

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

        # convert ascii error message to unicode (in python 3 only)
        cdef bytes error_bstring

        if not self.open:
            self.texture = gl_texture_load_from_memory_ex(<void*>ptr,
                                                    length, &error)

            if error != AE_IMAGE_SUCCESS:
                error_bstring = ae_image_error_message(error, NULL)

                if sys.version_info.major > 2:
                    raise IOError(error_bstring.decode())
                else:
                    raise IOError(error_bstring)

            # convenient way to set some texture attributes inline
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def load_from_array(self, Array data, **kwargs):
        """
        Load a texture from an array, likely a subview pointing to an archive slot.
        """
        return self.load_from_memory(data.address(), len(data), **kwargs)

    def load_from_bytes(self, bytes data, **kwargs):
        """
        Load a texture from a byte string, likely via open(filename, 'rb').read().
        """
        return self.load_from_memory(<size_t>(<char*>data), len(data), **kwargs)

    def load(self, str filename, **kwargs):
        """
        Load an image file into a texture. Raises IOError if image loading fails.
        """
        cdef ae_image_error_t error = AE_IMAGE_NO_CODEC # the default for stubs

        cdef bytes b_filename
        cdef bytes err_string

        # convert the filename from a unicode string in python 3 to ascii bytes
        if sys.version_info.major > 2:
            b_filename = <bytes>filename.encode('utf-8')
        else:
            b_filename = <bytes>filename

        if not self.open:
            self.texture = gl_texture_load_ex(<char*>b_filename, &error)

            if error != AE_IMAGE_SUCCESS:
                err_string = ae_image_error_message(error, <char*>b_filename)

                if sys.version_info.major > 2:
                    raise IOError(err_string.decode()) # convert to unicode
                else:
                    raise IOError(err_string) # use oldschool python 2 str

            # convenient way to set some texture attributes inline
            for key, val in kwargs.items(): setattr(self, key, val)

        return self

    def bind(self):
        """
        Make this the active texture in the current GL context, or disable textures.
        """
        gl_texture_bind(self.texture); return self

    def draw(self, float x, float y):
        """
        Draw a texture at (x, y). The origin is the texture's lower left corner.
        """
        gl_texture_draw(self.texture, x, y); return self

    def draw_ex(self, float x, float y, Vec4 rgba):
        """
        Draw a translucent, color-modulated texture at (x, y). Lower-left origin.
        """
        gl_texture_draw_ex(self.texture, x, y, rgba.v); return self

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

    @staticmethod
    def draw_skybox_ex(list textures, Coord3D coord, Vec4 rgba):
        """
        Draw an infinitely-distant, color-modulated textured cube around viewer.
        """
        cdef Texture ft = textures[0]
        cdef Texture bk = textures[1]
        cdef Texture lf = textures[2]
        cdef Texture rt = textures[3]
        cdef Texture up = textures[4]
        cdef Texture dn = textures[5]

        gl_texture_draw_skybox_ex(  ft.texture, bk.texture,
                                    lf.texture, rt.texture,
                                    up.texture, dn.texture,
                                    &coord.coord, rgba.v  )

# ==============================================================================
# ~ [ material system ]
# ==============================================================================

cdef class Material:
    """
    Defines the interaction between light and a surface (including texturing).
    """
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
    def print_all(): gl_material_print_all()

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
            cdef bytes s = gl_material_get_str(self.material, GL_MATERIAL_PROPERTY_STATUS)
            return s.decode() if sys.version_info.major > 2 else s

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_material_set_str(self.material, GL_MATERIAL_PROPERTY_STATUS, <char*>string)

    property path:
        def __get__(self):
            cdef bytes s = gl_material_get_str(self.material, GL_MATERIAL_PROPERTY_PATH)
            return s.decode() if sys.version_info.major > 2 else s # convert to unicode

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_material_set_str(self.material, GL_MATERIAL_PROPERTY_PATH, <char*>string)

    property name:
        def __get__(self):
            cdef bytes s = gl_material_get_str(self.material, GL_MATERIAL_PROPERTY_NAME)
            return s.decode() if sys.version_info.major > 2 else s # convert to unicode

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_material_set_str(self.material, GL_MATERIAL_PROPERTY_NAME, <char*>string)

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
    """
    An ambient per-vertex light in the scene - can also be used as a spotlight.
    """
    cdef gl_light_t* light

    def __init__(self, *args, **kwargs):
        if 'reference' in kwargs:
            self.light = <gl_light_t*>(<size_t>kwargs['reference'])
        else:
            self.light = gl_light_create() # set default properties

            if args: self.name = args[0] # easy init (Light("foo"))
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
    def close_all(): gl_light_print_all()

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
            cdef bytes s = gl_light_get_str(self.light, GL_LIGHT_PROPERTY_STATUS)
            return s.decode() if sys.version_info.major > 2 else s # char* -> utf

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_light_set_str(self.light, GL_LIGHT_PROPERTY_STATUS, <char*>string)

    property path:
        def __get__(self):
            cdef bytes s = gl_light_get_str(self.light, GL_LIGHT_PROPERTY_PATH)
            return s.decode() if sys.version_info.major > 2 else s # to unicode

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_light_set_str(self.light, GL_LIGHT_PROPERTY_PATH, <char*>string)

    property name:
        def __get__(self):
            cdef bytes s = gl_light_get_str(self.light, GL_LIGHT_PROPERTY_NAME)
            return s.decode() if sys.version_info.major > 2 else s # to unicode

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_light_set_str(self.light, GL_LIGHT_PROPERTY_NAME, <char*>string)

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

def Mesh(str mode, *args, **kwargs):
    """
    Convenience method for constructing buffers. gl.Object is extremely slow!!!
    """
    return AutoObject(getattr(Buffer(), "create_{0}".format(mode))
                                                (*args, **kwargs))

cdef class Buffer:
    """
    Triangle mesh for rendering and other tasks - optionally hardware-resident.
    """
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
    def print_all(): gl_buffer_print_all()

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
            cdef bytes s = gl_buffer_get_str(self.buffer, GL_BUFFER_PROPERTY_VERTEX_FORMAT)
            return s.decode() if sys.version_info.major > 2 else s # convert bytes to utf-8

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_buffer_set_str(self.buffer, GL_BUFFER_PROPERTY_VERTEX_FORMAT, <char*>string)

    property index_type:
        def __get__(self):
            return gl_buffer_get_int(self.buffer, GL_BUFFER_PROPERTY_INDEX_TYPE)

        def __set__(self, int value):
            gl_buffer_set_int(self.buffer, GL_BUFFER_PROPERTY_INDEX_TYPE, value)

    property index_type_name:
        def __get__(self):
            cdef bytes s = gl_buffer_get_str(self.buffer, GL_BUFFER_PROPERTY_INDEX_TYPE)
            return s.decode() if sys.version_info.major > 2 else s # convert bytes to utf

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_buffer_set_str(self.buffer, GL_BUFFER_PROPERTY_INDEX_TYPE, <char*>string)

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
            cdef bytes s = gl_buffer_get_str(self.buffer, GL_BUFFER_PROPERTY_STATUS)
            return s.decode() if sys.version_info.major > 2 else s # bytes to utf-8

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_buffer_set_str(self.buffer, GL_BUFFER_PROPERTY_STATUS, <char*>string)

    property path:
        def __get__(self):
            cdef bytes s = gl_buffer_get_str(self.buffer, GL_BUFFER_PROPERTY_PATH)
            return s.decode() if sys.version_info.major > 2 else s # bytes to utf-8

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_buffer_set_str(self.buffer, GL_BUFFER_PROPERTY_PATH, <char*>string)

    property name:
        def __get__(self):
            cdef bytes s = gl_buffer_get_str(self.buffer, GL_BUFFER_PROPERTY_NAME)
            return s.decode() if sys.version_info.major > 2 else s # bytes to utf-8

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_buffer_set_str(self.buffer, GL_BUFFER_PROPERTY_NAME, <char*>string)

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

    def create_lsystem(self, str program, int slices, int max_depth, **kwargs):
        """
        Execute a recursive turtle graphics program to make trees or vegetation.
        """
        cdef bytes bstring

        if sys.version_info.major > 2:
            bstring = <bytes>program.encode('utf-8')
        else:
            bstring = <bytes>program

        if not self.open:
            self.buffer = gl_buffer_create_lsystem(bstring, slices, max_depth)

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

    def draw_aabbox_ex(self, Vec4 rgba, float width=1.0):
        gl_buffer_draw_aabbox_ex(self.buffer, rgba.v, width); return self

    def draw_aabbox(self, Vec4 rgba):
        gl_buffer_draw_aabbox(self.buffer, rgba.v); return self

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
    Convenience method for porting an old game - define this for other classes?
    """
    return AutoObject(ParticleEmitter(*args, **kwargs))

cdef class ParticleEmitter:
    """
    Particle system for smoke, fire, snow, water spray, volumetric effects, etc.
    """
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
    def print_all(): gl_particle_emitter_print_all()

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
            cdef bytes s = gl_particle_emitter_get_str(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_VELGEN_MODE)
            return s.decode() if sys.version_info.major > 2 else s # convert velgen mode to utf-8 in python 3

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_particle_emitter_set_str(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_VELGEN_MODE, <char*>string)

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
            cdef bytes s = gl_particle_emitter_get_str(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_STATUS)
            return s.decode() if sys.version_info.major > 2 else s # convert to utf-8 string in python 3

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_particle_emitter_set_str(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_STATUS, <char*>string)

    property path:
        def __get__(self):
            cdef bytes s = gl_particle_emitter_get_str(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_PATH)
            return s.decode() if sys.version_info.major > 2 else s # convert to utf-8 string in python 3

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_particle_emitter_set_str(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_PATH, <char*>string)

    property name:
        def __get__(self):
            cdef bytes s = gl_particle_emitter_get_str(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_NAME)
            return s.decode() if sys.version_info.major > 2 else s # convert to utf-8 string in python 3

        def __set__(self, str value):
            cdef bytes string

            if sys.version_info.major > 2:
                string = <bytes>value.encode('utf-8')
            else:
                string = <bytes>value

            gl_particle_emitter_set_str(self.emitter, GL_PARTICLE_EMITTER_PROPERTY_NAME, <char*>string)

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

    def mesh_cover_face(self, Buffer buffer, size_t particles_per_triangle):
        """
        Randomly distribute emitter particles over the surface of each triangle.
        New particles are appended to the emitter, for generating large meshes.
        """
        gl_particle_emitter_mesh_cover_face(self.emitter, buffer.buffer, particles_per_triangle)
        return self

    def mesh_same_edges(self, Buffer buffer, size_t particles_per_triangle):
        """
        Randomly distribute particles along the edges of each triangle. Particles
        will not cross the face of the triangle in animations (same edges picked).
        """
        gl_particle_emitter_mesh_same_edges(self.emitter, buffer.buffer, particles_per_triangle)
        return self

    def mesh_rand_edges(self, Buffer buffer, size_t particles_per_triangle):
        """
        Randomly distribute particles along the edge of each triangle. The chosen edge
        for each new particle will be different every time (anim lerp crosses face).
        """
        gl_particle_emitter_mesh_rand_edges(self.emitter, buffer.buffer, particles_per_triangle)
        return self

    def mesh_given_edge(self, Buffer buffer, size_t particles_per_triangle, size_t index):
        """
        Randomly distribute particles along a single edge of each mesh triangle.
        This is a pretty weird-looking effect, included mostly for completeness.
        """
        gl_particle_emitter_mesh_given_edge(self.emitter, buffer.buffer, particles_per_triangle, index)
        return self

    def mesh_rad_vertex(self, Buffer buffer, size_t particles_per_triangle, float radius):
        """
        Creates a sphere of particles around every vertex in the triangle mesh.
        If the radius is 0 and particles per tri is 3, a fast path may be used.
        """
        gl_particle_emitter_mesh_rad_vertex(self.emitter, buffer.buffer, particles_per_triangle, radius)
        return self

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

    def draw_aabbox_ex(self, Vec4 rgba, float width):
        gl_particle_emitter_draw_aabbox_ex(self.emitter, rgba.v, width); return self

    def draw_aabbox(self, Vec4 rgba):
        gl_particle_emitter_draw_aabbox(self.emitter, rgba.v); return self

    def draw_velocity_ex(self, Vec4 start_color, Vec4 end_color, float width=1.0):
        """
        Draw the velocity of each particle as a line. Mostly for debugging.
        """
        gl_particle_emitter_draw_velocity_ex(self.emitter, start_color.v,
                                        end_color.v, width); return self

    def draw_velocity(self, Vec4 rgba):
        gl_particle_emitter_draw_velocity(self.emitter, rgba.v); return self
