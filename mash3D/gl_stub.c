/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __GL_CORE_H__
#include <gl_core.h>
#endif

/*
================================================================================
 * ~~ [ init & quit ] ~~ *
--------------------------------------------------------------------------------
*/

const char* gl_implementation(void)
{
    AE_STUB(); return "";
}

int gl_is_init(void)
{
    AE_STUB(); return 0;
}

void gl_init(void)
{
    AE_STUB();
}

void gl_quit(void)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ OpenGL wrapper ] ~~ *
--------------------------------------------------------------------------------
*/

void GL_Accum(unsigned int op, float value)
{
    AE_STUB();
}

void GL_ActiveTexture(unsigned int texture)
{
    AE_STUB();
}

void GL_AlphaFunc(unsigned int func, float ref)
{
    AE_STUB();
}

void GL_Begin(unsigned int mode)
{
    AE_STUB();
}

void GL_BindTexture(unsigned int target, unsigned int texture)
{
    AE_STUB();
}

void GL_BlendEquation(unsigned int mode)
{
    AE_STUB();
}

void GL_BlendEquationSeparate(unsigned int modeRGB, unsigned int modeA)
{
    AE_STUB();
}

void GL_BlendFunc(unsigned int src, unsigned int dst)
{
    AE_STUB();
}

void GL_BlendFuncSeparate(unsigned int srcRGB, unsigned int dstRGB,
                            unsigned int srcA, unsigned int dstA)
{
    AE_STUB();
}

void GL_Clear(unsigned int mask)
{
    AE_STUB();
}

void GL_ClearAccum(float r, float g, float b, float a)
{
    AE_STUB();
}

void GL_ClearColor(float r, float g, float b, float a)
{
    AE_STUB();
}

void GL_ClearDepth(double depth)
{
    AE_STUB();
}

void GL_ClearIndex(float c)
{
    AE_STUB();
}

void GL_Color3f(float r, float g, float b)
{
    AE_STUB();
}

void GL_Color3fv(const float* rgb)
{
    AE_STUB();
}

void GL_Color4f(float r, float g, float b, float a)
{
    AE_STUB();
}

void GL_Color4fv(const float* rgba)
{
    AE_STUB();
}

void GL_ColorMask(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    AE_STUB();
}

void GL_ColorMaterial(unsigned int face, unsigned int mode)
{
    AE_STUB();
}

void GL_ColorPointer(int size, unsigned int type, int stride, const void* data)
{
    AE_STUB();
}

void GL_CullFace(unsigned int face)
{
    AE_STUB();
}

void GL_DeleteTextures(int n, const unsigned int* textures)
{
    AE_STUB();
}

void GL_DepthFunc(unsigned int func)
{
    AE_STUB();
}

void GL_DepthMask(unsigned char flag)
{
    AE_STUB();
}

void GL_DepthRange(double near_val, double far_val)
{
    AE_STUB();
}

void GL_Disable(unsigned int cap)
{
    AE_STUB();
}

void GL_DisableClientState(unsigned int cap)
{
    AE_STUB();
}

void GL_DrawArrays(unsigned int mode, int first, int count)
{
    AE_STUB();
}

void GL_DrawElements(unsigned int mode, int n, unsigned int type, const void* inds)
{
    AE_STUB();
}

void GL_Enable(unsigned int cap)
{
    AE_STUB();
}

void GL_EnableClientState(unsigned int cap)
{
    AE_STUB();
}

void GL_End(void)
{
    AE_STUB();
}

void GL_Finish(void)
{
    AE_STUB();
}

void GL_Flush(void)
{
    AE_STUB();
}

void GL_Fogfv(unsigned int pname, const float* params)
{
    AE_STUB();
}

void GL_Fogiv(unsigned int pname, const int* params)
{
    AE_STUB();
}

void GL_Fogf(unsigned int pname, float param)
{
    AE_STUB();
}

void GL_Fogi(unsigned int pname, int param)
{
    AE_STUB();
}

void GL_FrontFace(unsigned int mode)
{
    AE_STUB();
}

void GL_Frustum(double lf, double rt, double bt, double tp, double nr, double fr)
{
    AE_STUB();
}

void GL_GenTextures(int n, unsigned int* textures)
{
    AE_STUB();
}

unsigned int GL_GetError(void)
{
    AE_STUB(); return 0;
}

void GL_GetFloatv(unsigned int pname, float* data)
{
    AE_STUB();
}

void GL_GetIntegerv(unsigned int pname, int* data)
{
    AE_STUB();
}

void GL_GetLightfv(unsigned int light, unsigned int pname, float* params)
{
    AE_STUB();
}

void GL_GetLightiv(unsigned int light, unsigned int pname, int* params)
{
    AE_STUB();
}

void GL_GetMaterialfv(unsigned int face, unsigned int pname, float* params)
{
    AE_STUB();
}

const char* GL_GetString(unsigned int name)
{
    AE_STUB(); return "";
}

void GL_GetTexEnvfv(unsigned int target, unsigned int pname, float* params)
{
    AE_STUB();
}

void GL_GetTexEnviv(unsigned int target, unsigned int pname, int* params)
{
    AE_STUB();
}

void GL_GetTexGendv(unsigned int coord, unsigned int pname, double* params)
{
    AE_STUB();
}

void GL_GetTexGenfv(unsigned int coord, unsigned int pname, float* params)
{
    AE_STUB();
}

void GL_GetTexGeniv(unsigned int coord, unsigned int pname, int* params)
{
    AE_STUB();
}

void GL_GetTexParameterfv(unsigned int target, unsigned int pname, float* params)
{
    AE_STUB();
}

void GL_GetTexParameteriv(unsigned int target, unsigned int pname, int* params)
{
    AE_STUB();
}

void GL_Hint(unsigned int target, unsigned int mode)
{
    AE_STUB();
}

void GL_InterleavedArrays(unsigned int format, int stride, const void* pointer)
{
    AE_STUB();
}

unsigned char GL_IsEnabled(unsigned int cap)
{
    AE_STUB(); return 0;
}

unsigned char GL_IsTexture(unsigned int texture)
{
    AE_STUB(); return 0;
}

void GL_Lightfv(unsigned int light, unsigned int pname, const float* params)
{
    AE_STUB();
}

void GL_Lightf(unsigned int light, unsigned int pname, float param)
{
    AE_STUB();
}

void GL_LineWidth(float width)
{
    AE_STUB();
}

void GL_LoadIdentity(void)
{
    AE_STUB();
}

void GL_LogicOp(unsigned int opcode)
{
    AE_STUB();
}

void GL_Materialfv(unsigned int face, unsigned int pname, const float* params)
{
    AE_STUB();
}

void GL_Materialf(unsigned int face, unsigned int pname, float param)
{
    AE_STUB();
}

void GL_MatrixMode(unsigned int mode)
{
    AE_STUB();
}

void GL_MultMatrixf(const float* matrix)
{
    AE_STUB();
}

void GL_Normal3f(float x, float y, float z)
{
    AE_STUB();
}

void GL_Normal3fv(const float* normal)
{
    AE_STUB();
}

void GL_NormalPointer(unsigned int type, int stride, const void* data)
{
    AE_STUB();
}

void GL_Ortho(double lf, double rt, double btm, double tp, double nr, double fr)
{
    AE_STUB();
}

void GL_PointParameterfv(unsigned int pname, const float* data)
{
    AE_STUB();
}

void GL_PointParameterf(unsigned int pname, float param)
{
    AE_STUB();
}

void GL_PointSize(float size)
{
    AE_STUB();
}

void GL_PolygonMode(unsigned int face, unsigned int mode)
{
    AE_STUB();
}

void GL_PolygonOffset(float factor, float units)
{
    AE_STUB();
}

void GL_PopAttrib(void)
{
    AE_STUB();
}

void GL_PushAttrib(unsigned int mask)
{
    AE_STUB();
}

void GL_PopClientAttrib(void)
{
    AE_STUB();
}

void GL_PushClientAttrib(unsigned int mask)
{
    AE_STUB();
}

void GL_PopMatrix(void)
{
    AE_STUB();
}

void GL_PushMatrix(void)
{
    AE_STUB();
}

void GL_Rotatef(float theta, float x, float y, float z)
{
    AE_STUB();
}

void GL_Scalef(float x, float y, float z)
{
    AE_STUB();
}

void GL_Scissor(int x, int y, int width, int height)
{
    AE_STUB();
}

void GL_ShadeModel(unsigned int mode)
{
    AE_STUB();
}

void GL_TexCoord2f(float s, float t)
{
    AE_STUB();
}

void GL_TexCoord2fv(const float* st)
{
    AE_STUB();
}

void GL_TexCoord3f(float s, float t, float r)
{
    AE_STUB();
}

void GL_TexCoord3fv(const float* str)
{
    AE_STUB();
}

void GL_TexCoord4f(float s, float t, float r, float q)
{
    AE_STUB();
}

void GL_TexCoord4fv(const float* strq)
{
    AE_STUB();
}

void GL_TexCoordPointer(int size, unsigned int type, int stride, const void* data)
{
    AE_STUB();
}

void GL_TexEnvfv(unsigned int target, unsigned int pname, const float *params)
{
    AE_STUB();
}

void GL_TexEnviv(unsigned int target, unsigned int pname, const int *params)
{
    AE_STUB();
}

void GL_TexEnvf(unsigned int target, unsigned int pname, float param)
{
    AE_STUB();
}

void GL_TexEnvi(unsigned int target, unsigned int pname, int param)
{
    AE_STUB();
}

void GL_TexGeniv(unsigned int coord, unsigned int pname, const int *params)
{
    AE_STUB();
}

void GL_TexGenfv(unsigned int coord, unsigned int pname, const float *params)
{
    AE_STUB();
}

void GL_TexGendv(unsigned int coord, unsigned int pname, const double *params)
{
    AE_STUB();
}

void GL_TexGeni(unsigned int coord, unsigned int pname, int param)
{
    AE_STUB();
}

void GL_TexGenf(unsigned int coord, unsigned int pname, float param)
{
    AE_STUB();
}

void GL_TexGend(unsigned int coord, unsigned int pname, double param)
{
    AE_STUB();
}

void GL_TexImage2D(unsigned int target, int level, int internal_format, int w,
        int h, int border, unsigned int fmt, unsigned int type, const void* p)
{
    AE_STUB();
}

void GL_TexParameteri(unsigned int target, unsigned int pname, int param)
{
    AE_STUB();
}

void GL_TexSubImage2D(unsigned int target, int level, int xoffset, int yoffset,
        int w, int h, unsigned int format, unsigned int type, const void* pix)
{
    AE_STUB();
}

void GL_Translatef(float x, float y, float z)
{
    AE_STUB();
}

void GL_Vertex2f(float x, float y)
{
    AE_STUB();
}

void GL_Vertex2fv(const float* xy)
{
    AE_STUB();
}

void GL_Vertex3f(float x, float y, float z)
{
    AE_STUB();
}

void GL_Vertex3fv(const float* xyz)
{
    AE_STUB();
}

void GL_VertexPointer(int size, unsigned int type, int stride, const void* data)
{
    AE_STUB();
}

void GL_Viewport(int x, int y, int width, int height)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ GL state logging ] ~~ *
--------------------------------------------------------------------------------
*/

void gl_log_error_state_ex(const char* filename, const char* funcname)
{
    AE_STUB();
}

void gl_log_texture_state_ex(const char* filename, const char* funcname)
{
    AE_STUB();
}

void gl_log_blend_state_ex(const char* filename, const char* funcname)
{
    AE_STUB();
}

void gl_log_material_state_ex(const char* filename, const char* funcname)
{
    AE_STUB();
}

void gl_log_light_state_ex(const char* filename, const char* funcname)
{
    AE_STUB();
}

void gl_log_internal_state_ex(const char* filename, const char* funcname)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ utilities ] ~~ *
--------------------------------------------------------------------------------
*/

void gl_enter3D(int width, int height, double FOV, double znear, double zfar)
{
    AE_STUB();
}

void gl_leave3D(void)
{
    AE_STUB();
}

void gl_enter2D(int width, int height)
{
    AE_STUB();
}

void gl_leave2D(void)
{
    AE_STUB();
}

/* ===== [ 2D shapes ] ====================================================== */

void gl_rect_ex(float* rect, const float* bl, const float* br, const float* tr,
                            const float* tl, int is_outline, float line_width)
{
    AE_STUB();
}

void gl_outline(float* rect, const float* rgba)
{
    AE_STUB();
}

void gl_rect(float* rect, const float* rgba)
{
    AE_STUB();
}

/* ===== [ 3D shapes ] ====================================================== */

void gl_aabbox_ex(float* minv, float* maxv, const float* rgba, float line_width)
{
    AE_STUB();
}

void gl_aabbox(float* minv, float* maxv, const float* rgba)
{
    AE_STUB();
}

void gl_ellipsoid(float* center, float* extent, const float* rgba)
{
    AE_STUB();
}

void gl_sphere(float* center, float radius, const float* rgba)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ coordinate system ] ~~ *
--------------------------------------------------------------------------------
*/

void gl_coord3D_apply_camera_transform_ex(gl_coord3D_t* coord, int absolute)
{
    AE_STUB();
}

void gl_coord3D_push_camera_transform_ex(gl_coord3D_t* coord, int absolute)
{
    AE_STUB();
}

void gl_coord3D_pop_camera_transform(void)
{
    AE_STUB();
}

void gl_coord3D_apply_object_transform_ex(gl_coord3D_t* coord, int absolute)
{
    AE_STUB();
}

void gl_coord3D_push_object_transform_ex(gl_coord3D_t* coord, int absolute)
{
    AE_STUB();
}

void gl_coord3D_pop_object_transform(void)
{
    AE_STUB();
}

void gl_coord3D_heading_towards_point(gl_coord3D_t* coord, float* p)
{
    AE_STUB();
}

void gl_coord3D_pitch_towards_point(gl_coord3D_t* coord, float* p)
{
    AE_STUB();
}

void gl_coord3D_face_towards_point(gl_coord3D_t* coord, float* p)
{
    AE_STUB();
}

void gl_coord3D_get_matrix(gl_coord3D_t* coord, float* matrix)
{
    AE_STUB();
}

void gl_coord3D_set_matrix(gl_coord3D_t* coord, float* matrix)
{
    AE_STUB();
}

void gl_coord3D_get_quat(gl_coord3D_t* coord, float* quat)
{
    AE_STUB();
}

void gl_coord3D_set_quat(gl_coord3D_t* coord, float* quat)
{
    AE_STUB();
}

void gl_coord3D_get_normal(gl_coord3D_t* coord, float* normal)
{
    AE_STUB();
}

void gl_coord3D_set_normal(gl_coord3D_t* coord, float* normal)
{
    AE_STUB();
}

void gl_coord3D_get_ray(gl_coord3D_t* coord, float* p, float* v)
{
    AE_STUB();
}

void gl_coord3D_set_ray(gl_coord3D_t* coord, float* p, float* v)
{
    AE_STUB();
}

void gl_coord3D_get_up_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_set_up_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_get_down_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_set_down_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_get_left_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_set_left_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_get_right_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_set_right_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

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

void gl_light_lerp(gl_light_t* light, gl_light_t* a, gl_light_t* b, float t)
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

void gl_particle_emitter_close_all(void)
{
    AE_STUB();
}
