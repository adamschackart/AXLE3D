/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __GL_CORE_H__
#define __GL_CORE_H__

#ifndef __AE_CORE_H__
#include <ae_core.h>
#endif

#ifdef __cplusplus
    extern "C" {
#endif

/*
================================================================================
 * ~~ [ macros & init ] ~~ *
--------------------------------------------------------------------------------
*/

#if defined(GL_COMPILING_EXE)
    #define GL_DECL
    #define GL_CALL
#endif

#if !defined(GL_DECL)
    #if defined(GL_COMPILING_DLL)
        #define GL_DECL AE_EXPORT
    #else
        #define GL_DECL AE_IMPORT
    #endif
#endif

#if !defined(GL_CALL)
    #define GL_CALL AE_CALL
#endif

#define GL_DEFAULT_EPSILON 0.00001f

GL_DECL int GL_CALL gl_is_init(void); // safer hacks
GL_DECL const char* GL_CALL gl_implementation(void);

GL_DECL void GL_CALL gl_init(void);
GL_DECL void GL_CALL gl_quit(void);

/*
================================================================================
 * ~~ [ OpenGL wrapper ] ~~ *
--------------------------------------------------------------------------------
NOTE: Production games should avoid calling these directly - they're not always
guaranteed to be implemented. These are for bootstrapping and prototyping stuff.
The enum constants these functions rely on are not exposed in this header file.
--------------------------------------------------------------------------------
*/

GL_DECL void GL_CALL
GL_AlphaFunc(unsigned int func, float ref);

GL_DECL void GL_CALL
GL_Begin(unsigned int mode);

GL_DECL void GL_CALL
GL_BindTexture(unsigned int target, unsigned int texture);

GL_DECL void GL_CALL
GL_BlendEquation(unsigned int mode);

GL_DECL void GL_CALL
GL_BlendEquationSeparate(unsigned int modeRGB, unsigned int modeA);

GL_DECL void GL_CALL
GL_BlendFunc(unsigned int src, unsigned int dst);

GL_DECL void GL_CALL
GL_BlendFuncSeparate(unsigned int srcRGB, unsigned int dstRGB,
                        unsigned int srcA, unsigned int dstA);

GL_DECL void GL_CALL
GL_Clear(unsigned int mask);

GL_DECL void GL_CALL
GL_ClearColor(float r, float g, float b, float a);

GL_DECL void GL_CALL
GL_Color3f(float r, float g, float b);

GL_DECL void GL_CALL
GL_Color4f(float r, float g, float b, float a);

GL_DECL void GL_CALL
GL_Color3fv(const float* rgb);

GL_DECL void GL_CALL
GL_Color4fv(const float* rgba);

GL_DECL void GL_CALL
GL_ColorMask(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

GL_DECL void GL_CALL
GL_ColorMaterial(unsigned int face, unsigned int mode);

GL_DECL void GL_CALL
GL_ColorPointer(int size, unsigned int type, int stride, const void* data);

GL_DECL void GL_CALL
GL_CullFace(unsigned int face);

GL_DECL void GL_CALL
GL_DeleteTextures(int n, const unsigned int* textures);

GL_DECL void GL_CALL
GL_DepthFunc(unsigned int func);

GL_DECL void GL_CALL
GL_DepthMask(unsigned char flag);

GL_DECL void GL_CALL
GL_DepthRange(double near_val, double far_val);

GL_DECL void GL_CALL
GL_Disable(unsigned int cap);

GL_DECL void GL_CALL
GL_DisableClientState(unsigned int cap);

GL_DECL void GL_CALL
GL_DrawArrays(unsigned int mode, int first, int count);

GL_DECL void GL_CALL
GL_DrawElements(unsigned int mode, int n, unsigned int type, const void* inds);

GL_DECL void GL_CALL
GL_Enable(unsigned int cap);

GL_DECL void GL_CALL
GL_EnableClientState(unsigned int cap);

GL_DECL void GL_CALL
GL_End(void);

GL_DECL void GL_CALL
GL_Finish(void);

GL_DECL void GL_CALL
GL_Flush(void);

GL_DECL void GL_CALL
GL_Fogfv(unsigned int pname, const float* params);

GL_DECL void GL_CALL
GL_Fogiv(unsigned int pname, const int* params);

GL_DECL void GL_CALL
GL_Fogf(unsigned int pname, float param);

GL_DECL void GL_CALL
GL_Fogi(unsigned int pname, int param);

GL_DECL void GL_CALL
GL_FrontFace(unsigned int mode);

GL_DECL void GL_CALL
GL_Frustum(double lf, double rt, double bt, double tp, double nr, double fr);

GL_DECL void GL_CALL
GL_GenTextures(int n, unsigned int* textures);

GL_DECL unsigned int GL_CALL
GL_GetError(void);

GL_DECL void GL_CALL
GL_GetFloatv(unsigned int pname, float* data);

GL_DECL void GL_CALL
GL_GetIntegerv(unsigned int pname, int* data);

GL_DECL void GL_CALL
GL_GetLightfv(unsigned int light, unsigned int pname, float* params);

GL_DECL void GL_CALL
GL_GetMaterialfv(unsigned int face, unsigned int pname, float* params);

GL_DECL const char* GL_CALL
GL_GetString(unsigned int name);

GL_DECL void GL_CALL
GL_GetTexParameteriv(unsigned int target, unsigned int pname, int* params);

GL_DECL void GL_CALL
GL_InterleavedArrays(unsigned int format, int stride, const void* pointer);

GL_DECL unsigned char GL_CALL
GL_IsEnabled(unsigned int cap);

GL_DECL unsigned char GL_CALL
GL_IsTexture(unsigned int texture);

GL_DECL void GL_CALL
GL_Lightfv(unsigned int light, unsigned int pname, const float* params);

GL_DECL void GL_CALL
GL_Lightf(unsigned int light, unsigned int pname, float param);

GL_DECL void GL_CALL
GL_LineWidth(float width);

GL_DECL void GL_CALL
GL_LoadIdentity(void);

GL_DECL void GL_CALL
GL_LogicOp(unsigned int opcode);

GL_DECL void GL_CALL
GL_Materialfv(unsigned int face, unsigned int pname, const float* params);

GL_DECL void GL_CALL
GL_Materialf(unsigned int face, unsigned int pname, float param);

GL_DECL void GL_CALL
GL_MatrixMode(unsigned int mode);

GL_DECL void GL_CALL
GL_MultMatrixf(const float* m);

GL_DECL void GL_CALL
GL_Normal3f(float x, float y, float z);

GL_DECL void GL_CALL
GL_Normal3fv(const float* normal);

GL_DECL void GL_CALL
GL_NormalPointer(unsigned int type, int stride, const void* data);

GL_DECL void GL_CALL
GL_Ortho(double lf, double rt, double btm, double top, double near, double far);

GL_DECL void GL_CALL
GL_PointParameterfv(unsigned int pname, const float* data);

GL_DECL void GL_CALL
GL_PointParameterf(unsigned int pname, float param);

GL_DECL void GL_CALL
GL_PointSize(float size);

GL_DECL void GL_CALL
GL_PolygonMode(unsigned int face, unsigned int mode);

GL_DECL void GL_CALL
GL_PolygonOffset(float factor, float units);

GL_DECL void GL_CALL
GL_PopAttrib(void);

GL_DECL void GL_CALL
GL_PushAttrib(unsigned int mask);

GL_DECL void GL_CALL
GL_PopClientAttrib(void);

GL_DECL void GL_CALL
GL_PushClientAttrib(unsigned int mask);

GL_DECL void GL_CALL
GL_PopMatrix(void);

GL_DECL void GL_CALL
GL_PushMatrix(void);

GL_DECL void GL_CALL
GL_Rotatef(float theta, float x, float y, float z);

GL_DECL void GL_CALL
GL_Scalef(float x, float y, float z);

GL_DECL void GL_CALL
GL_Scissor(int x, int y, int width, int height);

GL_DECL void GL_CALL
GL_ShadeModel(unsigned int mode);

GL_DECL void GL_CALL
GL_TexCoord2f(float s, float t);

GL_DECL void GL_CALL
GL_TexCoord3f(float s, float t, float r);

GL_DECL void GL_CALL
GL_TexCoord4f(float s, float t, float r, float q);

GL_DECL void GL_CALL
GL_TexCoord2fv(const float* st);

GL_DECL void GL_CALL
GL_TexCoord3fv(const float* str);

GL_DECL void GL_CALL
GL_TexCoord4fv(const float* strq);

GL_DECL void GL_CALL
GL_TexCoordPointer(int size, unsigned int type, int stride, const void* data);

GL_DECL void GL_CALL
GL_TexEnvfv(unsigned int target, unsigned int pname, const float *params);

GL_DECL void GL_CALL
GL_TexEnviv(unsigned int target, unsigned int pname, const int *params);

GL_DECL void GL_CALL
GL_TexEnvf(unsigned int target, unsigned int pname, float param);

GL_DECL void GL_CALL
GL_TexEnvi(unsigned int target, unsigned int pname, int param);

GL_DECL void GL_CALL
GL_TexImage2D(unsigned int target, int level, int internal_format, int w, int h,
            int border, unsigned int format, unsigned int type, const void * p);

GL_DECL void GL_CALL
GL_TexParameteri(unsigned int target, unsigned int pname, int param);

GL_DECL void GL_CALL
GL_TexSubImage2D(unsigned int target, int level, int xoffset, int yoffset, int width,
                int height, unsigned int format, unsigned int type, const void *pix);

GL_DECL void GL_CALL
GL_Translatef(float x, float y, float z);

GL_DECL void GL_CALL
GL_Vertex2f(float x, float y);

GL_DECL void GL_CALL
GL_Vertex3f(float x, float y, float z);

GL_DECL void GL_CALL
GL_Vertex2fv(const float* xy);

GL_DECL void GL_CALL
GL_Vertex3fv(const float* xyz);

GL_DECL void GL_CALL
GL_VertexPointer(int size, unsigned int type, int stride, const void* data);

GL_DECL void GL_CALL
GL_Viewport(int x, int y, int width, int height);

/*
================================================================================
 * ~~ [ GL state logging ] ~~ *
--------------------------------------------------------------------------------
*/

#define gl_log_error_state() gl_log_error_state_ex(__FILE__, __FUNCTION__)

#if defined(AE_DEBUG)
    #define gl_debug_log_error_state() gl_log_error_state()
#else
    #define gl_debug_log_error_state()
#endif

GL_DECL void GL_CALL
gl_log_error_state_ex(const char* filename, const char* funcname);

#define gl_log_texture_state() gl_log_texture_state_ex(__FILE__, __FUNCTION__)

#if defined(AE_DEBUG)
    #define gl_debug_log_texture_state() gl_log_texture_state()
#else
    #define gl_debug_log_texture_state()
#endif

GL_DECL void GL_CALL
gl_log_texture_state_ex(const char* filename, const char* funcname);

#define gl_log_blend_state() gl_log_blend_state_ex(__FILE__, __FUNCTION__)

#if defined(AE_DEBUG)
    #define gl_debug_log_blend_state() gl_log_blend_state()
#else
    #define gl_debug_log_blend_state()
#endif

GL_DECL void GL_CALL
gl_log_blend_state_ex(const char* filename, const char* funcname);

#define gl_log_material_state() gl_log_material_state_ex(__FILE__, __FUNCTION__)

#if defined(AE_DEBUG)
    #define gl_debug_log_material_state() gl_log_material_state()
#else
    #define gl_debug_log_material_state()
#endif

GL_DECL void GL_CALL
gl_log_material_state_ex(const char* filename, const char* funcname);

#define gl_log_light_state() gl_log_light_state_ex(__FILE__, __FUNCTION__)

#if defined(AE_DEBUG)
    #define gl_debug_log_light_state() gl_log_light_state()
#else
    #define gl_debug_log_light_state()
#endif

GL_DECL void GL_CALL
gl_log_light_state_ex(const char* filename, const char* funcname);

#define gl_log_internal_state() gl_log_internal_state_ex(__FILE__, __FUNCTION__)

#if defined(AE_DEBUG)
    #define gl_debug_log_internal_state() gl_log_internal_state()
#else
    #define gl_debug_log_internal_state()
#endif

GL_DECL void GL_CALL
gl_log_internal_state_ex(const char* filename, const char* funcname);

/*
================================================================================
 * ~~ [ utilities ] ~~ *
--------------------------------------------------------------------------------
*/

GL_DECL void GL_CALL gl_enter3D(int w, int h, double FOV, double znear, double zfar);
GL_DECL void GL_CALL gl_leave3D(void);

// NOTE: when rendering a 2D scene in an XL window using GL textures, one must
// call this function first, otherwise graphics could be displayed upside-down.
// this disables XL's resolution-independent renderer until leave2D is called.

GL_DECL void GL_CALL gl_enter2D(int w, int h);
GL_DECL void GL_CALL gl_leave2D(void);

GL_DECL void GL_CALL gl_rect_ex(float* rect, float* bl, float* br,
                float* tr, float* tl, int line, float line_width);

GL_DECL void GL_CALL gl_outline(float* rect, float* rgba);
GL_DECL void GL_CALL gl_rect(float* rect, float* rgba);

GL_DECL void GL_CALL // render a 3-dimensional bounding box outline
gl_aabbox_ex(float* minv, float* maxv, float* rgba, float width);

GL_DECL void GL_CALL gl_aabbox(float* minv, float* maxv, float* rgba);

// render a 3-dimensional capsule shape in wireframe mode
GL_DECL void GL_CALL gl_ellipsoid(float* center, float* extent, float* rgba);

// render a 3-dimensional spherical shape in wireframe mode
GL_DECL void GL_CALL gl_sphere(float* center, float radius, float* rgba);

/*
================================================================================
 * ~~ [ coordinate system ] ~~ *
--------------------------------------------------------------------------------
NOTE: +Y is up, matrices are col-major, rotation is in counter-clockwise degrees
--------------------------------------------------------------------------------
*/

typedef struct gl_coord3D_t
{
    union
    {
        float position[3];
        float pos[3];

        float translation[3];
        float xyz[3];

        struct
        {
            float x, y, z;
        };
    };

    union
    {
        float rotation[3];
        float rot[3];

        // TODO: aliases for yaw, attitude, bank
        struct
        {
            float pitch;
            float heading;
            float roll;
        };
    };

    union
    {
        float scale[3];
        float scl[3];

        struct
        {
            float scale_x;
            float scale_y;
            float scale_z;
        };
    };
} \
    gl_coord3D_t;

static c_inline void gl_coord3D_init(gl_coord3D_t* coord)
{
    vec3zero(coord->position);
    vec3zero(coord->rotation);

    coord->scl[0] = coord->scl[1] = coord->scl[2] = 1.0f;
}

static c_inline void gl_coord3D_copy(gl_coord3D_t* dst, gl_coord3D_t* src)
{
    memmove(dst, src, sizeof(gl_coord3D_t));
}

static c_inline void gl_coord3D_normalize(gl_coord3D_t* coord,
                        const float min_v, const float max_v)
{
    while (coord->pitch < min_v) coord->pitch += 360.0f;
    while (coord->pitch > max_v) coord->pitch -= 360.0f;

    while (coord->heading < min_v) coord->heading += 360.0f;
    while (coord->heading > max_v) coord->heading -= 360.0f;

    while (coord->roll < min_v) coord->roll += 360.0f;
    while (coord->roll > max_v) coord->roll -= 360.0f;
}

static c_inline void gl_coord3D_normalize180(gl_coord3D_t* coord)
{
    gl_coord3D_normalize(coord, -180.0f, 180.0f);
}

static c_inline void gl_coord3D_normalize360(gl_coord3D_t* coord)
{
    gl_coord3D_normalize(coord, 0.0f, 360.0f);
}

/* ===== [ modelview transforms ] =========================================== */

GL_DECL void GL_CALL // viewer
gl_coord3D_apply_camera_transform_ex(gl_coord3D_t* coord, int absolute);

GL_DECL void GL_CALL
gl_coord3D_push_camera_transform_ex(gl_coord3D_t* coord, int absolute);

static c_inline void gl_coord3D_apply_camera_transform(gl_coord3D_t* coord)
{
    gl_coord3D_apply_camera_transform_ex(coord, 1);
}

static c_inline void gl_coord3D_push_camera_transform(gl_coord3D_t* coord)
{
    gl_coord3D_push_camera_transform_ex(coord, 1);
}

GL_DECL void GL_CALL gl_coord3D_pop_camera_transform(void);

GL_DECL void GL_CALL // entity
gl_coord3D_apply_object_transform_ex(gl_coord3D_t* coord, int absolute);

GL_DECL void GL_CALL
gl_coord3D_push_object_transform_ex(gl_coord3D_t* coord, int absolute);

static c_inline void gl_coord3D_apply_object_transform(gl_coord3D_t* coord)
{
    gl_coord3D_apply_object_transform_ex(coord, 0);
}

static c_inline void gl_coord3D_push_object_transform(gl_coord3D_t* coord)
{
    gl_coord3D_push_object_transform_ex(coord, 0);
}

GL_DECL void GL_CALL gl_coord3D_pop_object_transform(void);

/* ===== [ conversion ] ===================================================== */

GL_DECL void GL_CALL gl_coord3D_heading_towards_point(gl_coord3D_t*, float* p);
GL_DECL void GL_CALL gl_coord3D_pitch_towards_point(gl_coord3D_t*, float* p);

// set facing normal towards point
GL_DECL void GL_CALL gl_coord3D_face_towards_point(gl_coord3D_t*, float* p);

// column-major 4x4 modelview matrix
GL_DECL void GL_CALL gl_coord3D_get_matrix(gl_coord3D_t*, float* matrix);
GL_DECL void GL_CALL gl_coord3D_set_matrix(gl_coord3D_t*, float* matrix);

// facing quaternion
GL_DECL void GL_CALL gl_coord3D_get_quat(gl_coord3D_t*, float* quat);
GL_DECL void GL_CALL gl_coord3D_set_quat(gl_coord3D_t*, float* quat);

// facing normal vector
GL_DECL void GL_CALL gl_coord3D_get_normal(gl_coord3D_t*, float* normal);
GL_DECL void GL_CALL gl_coord3D_set_normal(gl_coord3D_t*, float* normal);

// facing ray
GL_DECL void GL_CALL gl_coord3D_get_ray(gl_coord3D_t*, float* p, float* v);
GL_DECL void GL_CALL gl_coord3D_set_ray(gl_coord3D_t*, float* p, float* v);

// vector pointing up from entity's head
GL_DECL void GL_CALL gl_coord3D_get_up_vector(gl_coord3D_t*, float* v);
GL_DECL void GL_CALL gl_coord3D_set_up_vector(gl_coord3D_t*, float* v);

// vector pointing down to entity's feet
GL_DECL void GL_CALL gl_coord3D_get_down_vector(gl_coord3D_t*, float* v);
GL_DECL void GL_CALL gl_coord3D_set_down_vector(gl_coord3D_t*, float* v);

// vector pointing from entity's left shoulder
GL_DECL void GL_CALL gl_coord3D_get_left_vector(gl_coord3D_t*, float* v);
GL_DECL void GL_CALL gl_coord3D_set_left_vector(gl_coord3D_t*, float* v);

// vector pointing from entity's right shoulder
GL_DECL void GL_CALL gl_coord3D_get_right_vector(gl_coord3D_t*, float* v);
GL_DECL void GL_CALL gl_coord3D_set_right_vector(gl_coord3D_t*, float* v);

/*
================================================================================
 * ~~ [ object types ] ~~ *
--------------------------------------------------------------------------------
*/

/* All objects are represented by opaque handles to implementation-defined data.
 */
#define GL_OBJECT_TYPE_N                    \
                                            \
    N(TEXTURE, texture)                     \
    N(MATERIAL, material)                   \
    N(LIGHT, light)                         \
    N(BUFFER, buffer)                       \
    N(PARTICLE_EMITTER, particle_emitter)   \

#if 0
#define N(cap, low) typedef struct gl_internal_ ## low ## _t gl_ ## low ## _t;
#else
#define N(cap, low) typedef void gl_ ## low ## _t;
#endif
GL_OBJECT_TYPE_N
#undef N

typedef enum gl_object_type_t
{
    GL_OBJECT_TYPE_UNKNOWN,

    #define N(cap, low) GL_OBJECT_TYPE_ ## cap,
    GL_OBJECT_TYPE_N
    #undef N

    GL_OBJECT_TYPE_COUNT,
} \
    gl_object_type_t;

static const char* gl_object_type_name[] =
{
    "GL_OBJECT_TYPE_UNKNOWN",

    #define N(cap, low) AE_STRINGIFY(GL_OBJECT_TYPE_ ## cap),
    GL_OBJECT_TYPE_N
    #undef N

    "GL_OBJECT_TYPE_COUNT",
};

static const char* gl_object_type_short_name[] =
{
    "unknown",

    #define N(cap, low) #low,
    GL_OBJECT_TYPE_N
    #undef N

    "count",
};

/* Identify an object. Invalid, closed, or NULL objects are of "unknown" type.
 */
GL_DECL gl_object_type_t GL_CALL gl_object_type(void* object);

/* These functions can be used to manage all objects tracked by this system.
 */
GL_DECL size_t GL_CALL gl_object_count_all(void);
GL_DECL void   GL_CALL gl_object_list_all (void** objects);
GL_DECL void   GL_CALL gl_object_close_all(void);

/*
================================================================================
 * ~~ [ texture renderer ] ~~ *
--------------------------------------------------------------------------------
*/

#define GL_TEXTURE_PROPERTY_N                                   \
                                                                \
    /* the total number of active textures */                   \
    N(GL_TEXTURE_PROPERTY_TOTAL, int, int, total)               \
                                                                \
    /* GL handle */                                             \
    N(GL_TEXTURE_PROPERTY_ID, int, int, id)                     \
                                                                \
    /* GL texture type (likely GL_TEXTURE_2D) */                \
    N(GL_TEXTURE_PROPERTY_TARGET, int, int, target)             \
                                                                \
    /* the dimensions of the texture */                         \
    N(GL_TEXTURE_PROPERTY_WIDTH , int, int, width )             \
    N(GL_TEXTURE_PROPERTY_HEIGHT, int, int, height)             \
                                                                \
    /* color depth / number of channels / components */         \
    N(GL_TEXTURE_PROPERTY_COMP, int, int, comp)                 \
                                                                \
    /* pixel data */                                            \
    N(GL_TEXTURE_PROPERTY_IMAGE, ae_image_t*, img, image)       \
                                                                \
    /* when new textures are created, is copying enabled? */    \
    N(GL_TEXTURE_PROPERTY_DEFAULT_COPY, int, int, default_copy) \
                                                                \
    /* allows gl_texture_get_image by copying uploaded data */  \
    N(GL_TEXTURE_PROPERTY_COPY_ENABLED, int, int, copy_enabled) \
                                                                \
    /* whatever interesting info we want to display */          \
    N(GL_TEXTURE_PROPERTY_STATUS, const char*, str, status)     \
                                                                \
    N(GL_TEXTURE_PROPERTY_PATH, const char*, str, path)         \
    N(GL_TEXTURE_PROPERTY_NAME, const char*, str, name)         \
                                                                \
    /* texture is valid */                                      \
    N(GL_TEXTURE_PROPERTY_OPEN, int, int, open)                 \
                                                                \
    /* the number of texture properties */                      \
    N(GL_TEXTURE_PROPERTY_COUNT, int, int, _)                   \

typedef enum gl_texture_property_t
{
    #define N(x, t, s, n) x,
    GL_TEXTURE_PROPERTY_N
    #undef N
} \
    gl_texture_property_t;

static const char* gl_texture_property_name[] =
{
    #define N(x, t, s, n) #x,
    GL_TEXTURE_PROPERTY_N
    #undef N
};

static const char* gl_texture_property_type[] =
{
    #define N(x, t, s, n) #s,
    GL_TEXTURE_PROPERTY_N
    #undef N
};

GL_DECL void GL_CALL
gl_texture_set_int(gl_texture_t* texture, gl_texture_property_t property, int value);

GL_DECL int GL_CALL
gl_texture_get_int(gl_texture_t* texture, gl_texture_property_t property);

GL_DECL void GL_CALL
gl_texture_set_str(gl_texture_t* texture, gl_texture_property_t property, const char* value);

GL_DECL const char* GL_CALL
gl_texture_get_str(gl_texture_t* texture, gl_texture_property_t property);

GL_DECL void GL_CALL
gl_texture_set_img(gl_texture_t* texture, gl_texture_property_t property, ae_image_t* value);

GL_DECL ae_image_t* GL_CALL
gl_texture_get_img(gl_texture_t* texture, gl_texture_property_t property);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C code.
 * i.e. gl_texture_set_int(t, GL_TEXTURE_PROPERTY_OPEN, v) = gl_texture_set_open(t, v).
 */
#define N(x, type, short_type, name)                                                    \
                                                                                        \
    static c_inline void gl_texture_set_ ## name (gl_texture_t* texture, type value)    \
    {                                                                                   \
        gl_texture_set_ ## short_type (texture, x, value);                              \
    }                                                                                   \
                                                                                        \
    static c_inline type gl_texture_get_ ## name (gl_texture_t* texture)                \
    {                                                                                   \
        return gl_texture_get_ ## short_type (texture, x);                              \
    }                                                                                   \

GL_TEXTURE_PROPERTY_N
#undef N

static c_inline void gl_texture_close(gl_texture_t* texture)
{
    gl_texture_set_open(texture, 0);
}

/* Manually create a new OpenGL texture (set pixel data with the image property).
 * Comp is the number of color channels (3 makes a GL_RGB texture, for example).
 */
GL_DECL gl_texture_t* GL_CALL gl_texture_create(int width, int height, int comp);

/* Load a compressed texture from a memory blok, with and without auto error handling.
 */
GL_DECL gl_texture_t* GL_CALL gl_texture_load_from_memory(void* buf, size_t size);

GL_DECL gl_texture_t* GL_CALL
gl_texture_load_from_memory_ex(void* buf, size_t size, ae_image_error_t* error_status);

/* Load a texture from a file, with and without automatic error handling.
 */
GL_DECL gl_texture_t* GL_CALL gl_texture_load(const char* filename);

GL_DECL gl_texture_t* GL_CALL
gl_texture_load_ex(const char* filename, ae_image_error_t* error_status);

/* Make this the active texture in the current GL context. */
GL_DECL void GL_CALL gl_texture_bind(gl_texture_t* texture);

/* Draw a texture at (x, y) --- origin is the texture's lower left corner. */
GL_DECL void GL_CALL gl_texture_blit(gl_texture_t* texture, float x, float y);

GL_DECL void GL_CALL /* Draw a translucent, color-modulated texture. */
gl_texture_blit_ex(gl_texture_t* texture, float x, float y, float rgba[4]);

GL_DECL void GL_CALL gl_texture_draw_skybox(gl_texture_t* front,
                                            gl_texture_t* back,
                                            gl_texture_t* left,
                                            gl_texture_t* right,
                                            gl_texture_t* up,
                                            gl_texture_t* down,
                                            gl_coord3D_t* viewer);

static c_inline size_t gl_texture_count_all(void)
{
    return (size_t)gl_texture_get_total(NULL);
}

GL_DECL void GL_CALL gl_texture_list_all(gl_texture_t** textures);
GL_DECL void GL_CALL gl_texture_close_all(void);

/*
================================================================================
 * ~~ [ material system ] ~~ *
--------------------------------------------------------------------------------
*/

#define GL_MATERIAL_PROPERTY_N                                      \
                                                                    \
    N(GL_MATERIAL_PROPERTY_TOTAL, int, int, total)                  \
    N(GL_MATERIAL_PROPERTY_ID, int, int, id)                        \
                                                                    \
    /* game-specific flags (surface contact does damage, etc.) */   \
    N(GL_MATERIAL_PROPERTY_FLAGS, u64, u64, flags)                  \
                                                                    \
    /* diffuse texture */                                           \
    N(GL_MATERIAL_PROPERTY_TEXTURE, gl_texture_t*, tex, texture)    \
                                                                    \
    /* is the texture closed when the material is closed? */        \
    N(GL_MATERIAL_PROPERTY_OWNS_TEXTURE, int, int, owns_texture)    \
                                                                    \
    N(GL_MATERIAL_PROPERTY_DIFFUSE, float*, vec, diffuse)           \
    N(GL_MATERIAL_PROPERTY_AMBIENT, float*, vec, ambient)           \
    N(GL_MATERIAL_PROPERTY_SPECULAR, float*, vec, specular)         \
    N(GL_MATERIAL_PROPERTY_EMISSION, float*, vec, emission)         \
                                                                    \
    /* full bright white (for stuff like computer screens etc.) */  \
    N(GL_MATERIAL_PROPERTY_LIGHT_ENABLED, int, int, light_enabled)  \
                                                                    \
    /* In simple lighting models, the brightness of a specular      \
     * highlight is computed as cos(theta) ^ N, where theta is      \
     * the angle of incidence and N is the shininess value. The     \
     * higher the value, the "tighter" the highlight will be.       \
     * Unlike most values here, this is in the [0, 128] range.      \
     */                                                             \
    N(GL_MATERIAL_PROPERTY_SHININESS, float, flt, shininess)        \
    N(GL_MATERIAL_PROPERTY_OPACITY, float, flt, opacity)            \
                                                                    \
    N(GL_MATERIAL_PROPERTY_STATUS, const char*, str, status)        \
    N(GL_MATERIAL_PROPERTY_PATH, const char*, str, path)            \
    N(GL_MATERIAL_PROPERTY_NAME, const char*, str, name)            \
                                                                    \
    N(GL_MATERIAL_PROPERTY_OPEN, int, int, open)                    \
    N(GL_MATERIAL_PROPERTY_COUNT, int, int, _)                      \

typedef enum gl_material_property_t
{
    #define N(x, t, s, n) x,
    GL_MATERIAL_PROPERTY_N
    #undef N
} \
    gl_material_property_t;

static const char* gl_material_property_name[] =
{
    #define N(x, t, s, n) #x,
    GL_MATERIAL_PROPERTY_N
    #undef N
};

static const char* gl_material_property_type[] =
{
    #define N(x, t, s, n) #s,
    GL_MATERIAL_PROPERTY_N
    #undef N
};

GL_DECL void GL_CALL
gl_material_set_int(gl_material_t* material, gl_material_property_t property, int value);

GL_DECL int GL_CALL
gl_material_get_int(gl_material_t* material, gl_material_property_t property);

GL_DECL void GL_CALL
gl_material_set_u64(gl_material_t* material, gl_material_property_t property, u64 value);

GL_DECL u64 GL_CALL
gl_material_get_u64(gl_material_t* material, gl_material_property_t property);

GL_DECL void GL_CALL
gl_material_set_flt(gl_material_t* material, gl_material_property_t property, float value);

GL_DECL float GL_CALL
gl_material_get_flt(gl_material_t* material, gl_material_property_t property);

GL_DECL void GL_CALL
gl_material_set_vec(gl_material_t* material, gl_material_property_t property, float* value);

GL_DECL float* GL_CALL
gl_material_get_vec(gl_material_t* material, gl_material_property_t property);

GL_DECL void GL_CALL
gl_material_set_str(gl_material_t* material, gl_material_property_t property, const char* value);

GL_DECL const char* GL_CALL
gl_material_get_str(gl_material_t* material, gl_material_property_t property);

GL_DECL void GL_CALL
gl_material_set_tex(gl_material_t* material, gl_material_property_t property, gl_texture_t* value);

GL_DECL gl_texture_t* GL_CALL
gl_material_get_tex(gl_material_t* material, gl_material_property_t property);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C code.
 */
#define N(x, type, short_type, name)                                                \
                                                                                    \
    static c_inline void gl_material_set_ ## name (gl_material_t* mtl, type value)  \
    {                                                                               \
        gl_material_set_ ## short_type (mtl, x, value);                             \
    }                                                                               \
                                                                                    \
    static c_inline type gl_material_get_ ## name (gl_material_t* mtl)              \
    {                                                                               \
        return gl_material_get_ ## short_type (mtl, x);                             \
    }                                                                               \

GL_MATERIAL_PROPERTY_N
#undef N

static c_inline void gl_material_close(gl_material_t* material)
{
    gl_material_set_open(material, 0);
}

// Create a new material with default properties and no texture.
GL_DECL gl_material_t* GL_CALL gl_material_create(void);

// Set diffuse, ambient, specular, and emissive components to the same value.
GL_DECL void GL_CALL gl_material_set_all(gl_material_t* material, float* v);

GL_DECL void GL_CALL // interpolate properties of A and B into dst (for animation)
gl_material_lerp(gl_material_t* dst, gl_material_t* a, gl_material_t* b, float t);

// Bind material properties (with optional buffer information).
GL_DECL void GL_CALL gl_material_apply_ex(gl_material_t* material, gl_buffer_t*);

// Bind material properties (including alpha blend and texture).
GL_DECL void GL_CALL gl_material_apply(gl_material_t* material);

static c_inline size_t gl_material_count_all(void)
{
    return (size_t)gl_material_get_total(NULL);
}

GL_DECL void GL_CALL gl_material_list_all(gl_material_t** mtls);
GL_DECL void GL_CALL gl_material_close_all(void);

/*
================================================================================
 * ~~ [ lighting system ] ~~ *
--------------------------------------------------------------------------------
*/

#define GL_LIGHT_PROPERTY_N                                                         \
                                                                                    \
    N(GL_LIGHT_PROPERTY_TOTAL, int, int, total)                                     \
    N(GL_LIGHT_PROPERTY_ID, int, int, id)                                           \
                                                                                    \
    N(GL_LIGHT_PROPERTY_AMBIENT, float*, vec, ambient)                              \
    N(GL_LIGHT_PROPERTY_DIFFUSE, float*, vec, diffuse)                              \
    N(GL_LIGHT_PROPERTY_SPECULAR, float*, vec, specular)                            \
    N(GL_LIGHT_PROPERTY_POSITION, float*, vec, position)                            \
                                                                                    \
    N(GL_LIGHT_PROPERTY_SPOT_DIRECTION, float*, vec, spot_direction)                \
    N(GL_LIGHT_PROPERTY_SPOT_EXPONENT, float, flt, spot_exponent)                   \
    N(GL_LIGHT_PROPERTY_SPOT_CUTOFF, float, flt, spot_cutoff)                       \
                                                                                    \
    N(GL_LIGHT_PROPERTY_CONSTANT_ATTENUATION, float, flt, constant_attenuation)     \
    N(GL_LIGHT_PROPERTY_LINEAR_ATTENUATION, float, flt, linear_attenuation)         \
    N(GL_LIGHT_PROPERTY_QUADRATIC_ATTENUATION, float, flt, quadratic_attenuation)   \
                                                                                    \
    N(GL_LIGHT_PROPERTY_STATUS, const char*, str, status)                           \
    N(GL_LIGHT_PROPERTY_NAME, const char*, str, name)                               \
                                                                                    \
    N(GL_LIGHT_PROPERTY_OPEN, int, int, open)                                       \
    N(GL_LIGHT_PROPERTY_COUNT, int, int, _)                                         \

typedef enum gl_light_property_t
{
    #define N(x, t, s, n) x,
    GL_LIGHT_PROPERTY_N
    #undef N
} \
    gl_light_property_t;

static const char* gl_light_property_name[] =
{
    #define N(x, t, s, n) #x,
    GL_LIGHT_PROPERTY_N
    #undef N
};

static const char* gl_light_property_type[] =
{
    #define N(x, t, s, n) #s,
    GL_LIGHT_PROPERTY_N
    #undef N
};

GL_DECL void GL_CALL
gl_light_set_int(gl_light_t* light, gl_light_property_t property, int value);

GL_DECL int GL_CALL
gl_light_get_int(gl_light_t* light, gl_light_property_t property);

GL_DECL void GL_CALL
gl_light_set_str(gl_light_t* light, gl_light_property_t property, const char* value);

GL_DECL const char* GL_CALL
gl_light_get_str(gl_light_t* light, gl_light_property_t property);

GL_DECL void GL_CALL
gl_light_set_vec(gl_light_t* light, gl_light_property_t property, float* value);

GL_DECL float* GL_CALL
gl_light_get_vec(gl_light_t* light, gl_light_property_t property);

GL_DECL void GL_CALL
gl_light_set_flt(gl_light_t* light, gl_light_property_t property, float value);

GL_DECL float GL_CALL
gl_light_get_flt(gl_light_t* light, gl_light_property_t property);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C code.
 */
#define N(x, type, short_type, name)                                            \
                                                                                \
    static c_inline void gl_light_set_ ## name (gl_light_t* light, type value)  \
    {                                                                           \
        gl_light_set_ ## short_type (light, x, value);                          \
    }                                                                           \
                                                                                \
    static c_inline type gl_light_get_ ## name (gl_light_t* light)              \
    {                                                                           \
        return gl_light_get_ ## short_type (light, x);                          \
    }                                                                           \

GL_LIGHT_PROPERTY_N
#undef N

static c_inline void gl_light_close(gl_light_t* light)
{
    gl_light_set_open(light, 0);
}

// Add a new light to the current global scene and give it default properties.
GL_DECL gl_light_t* GL_CALL gl_light_create(void);

// Set ambient and diffuse properties; set the light to a solid color profile.
GL_DECL void GL_CALL gl_light_set_all(gl_light_t* light, float* value);

// Interpolate the properties of lights A and B into the destination light.
GL_DECL void GL_CALL
gl_light_lerp(gl_light_t* light, gl_light_t* a, gl_light_t* b, float t);

// Create a standard ambient lighting setup (medium-soft white light).
GL_DECL void GL_CALL gl_light_default_scene(void);

// Set all hardware light state for the current scene (call once per frame?).
GL_DECL void GL_CALL gl_light_apply_all(void);

static c_inline size_t gl_light_count_all(void)
{
    return (size_t)gl_light_get_total(NULL);
}

GL_DECL void GL_CALL gl_light_list_all(gl_light_t** lights);
GL_DECL void GL_CALL gl_light_close_all(void);

/*
================================================================================
 * ~~ [ vertex buffers ] ~~ *
--------------------------------------------------------------------------------
*/

#define GL_BUFFER_PROPERTY_N                                        \
                                                                    \
    N(GL_BUFFER_PROPERTY_TOTAL, int, int, total)                    \
    N(GL_BUFFER_PROPERTY_ID, int, int, id)                          \
                                                                    \
    N(GL_BUFFER_PROPERTY_VERTEX_FORMAT, int, int, vertex_format)    \
    N(GL_BUFFER_PROPERTY_INDEX_TYPE, int, int, index_type)          \
                                                                    \
    N(GL_BUFFER_PROPERTY_VERTEX_COUNT, int, int, vertex_count)      \
    N(GL_BUFFER_PROPERTY_INDEX_COUNT, int, int, index_count)        \
    N(GL_BUFFER_PROPERTY_TRIANGLE_COUNT, int, int, triangle_count)  \
                                                                    \
    N(GL_BUFFER_PROPERTY_STATIC_STORAGE, int, int, static_storage)  \
    N(GL_BUFFER_PROPERTY_COMPUTE_AABBOX, int, int, compute_aabbox)  \
    N(GL_BUFFER_PROPERTY_LOCKED, int, int, locked)                  \
                                                                    \
    N(GL_BUFFER_PROPERTY_SMOOTH_SHADING, int, int, smooth_shading)  \
    N(GL_BUFFER_PROPERTY_LINE_MODE, int, int, line_mode)            \
                                                                    \
    N(GL_BUFFER_PROPERTY_STATUS, const char*, str, status)          \
    N(GL_BUFFER_PROPERTY_PATH, const char*, str, path)              \
    N(GL_BUFFER_PROPERTY_NAME, const char*, str, name)              \
                                                                    \
    N(GL_BUFFER_PROPERTY_OPEN, int, int, open)                      \
    N(GL_BUFFER_PROPERTY_COUNT, int, int, _)                        \

typedef enum gl_buffer_property_t
{
    #define N(x, t, s, n) x,
    GL_BUFFER_PROPERTY_N
    #undef N
} \
    gl_buffer_property_t;

static const char* gl_buffer_property_name[] =
{
    #define N(x, t, s, n) #x,
    GL_BUFFER_PROPERTY_N
    #undef N
};

static const char* gl_buffer_property_type[] =
{
    #define N(x, t, s, n) #s,
    GL_BUFFER_PROPERTY_N
    #undef N
};

GL_DECL void GL_CALL
gl_buffer_set_int(gl_buffer_t* buffer, gl_buffer_property_t property, int value);

GL_DECL int GL_CALL
gl_buffer_get_int(gl_buffer_t* buffer, gl_buffer_property_t property);

GL_DECL void GL_CALL
gl_buffer_set_str(gl_buffer_t* buffer, gl_buffer_property_t property, const char* value);

GL_DECL const char* GL_CALL
gl_buffer_get_str(gl_buffer_t* buffer, gl_buffer_property_t property);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C code.
 */
#define N(x, type, short_type, name)                                                \
                                                                                    \
    static c_inline void gl_buffer_set_ ## name (gl_buffer_t* buffer, type value)   \
    {                                                                               \
        gl_buffer_set_ ## short_type (buffer, x, value);                            \
    }                                                                               \
                                                                                    \
    static c_inline type gl_buffer_get_ ## name (gl_buffer_t* buffer)               \
    {                                                                               \
        return gl_buffer_get_ ## short_type (buffer, x);                            \
    }                                                                               \

GL_BUFFER_PROPERTY_N
#undef N

static c_inline void gl_buffer_close(gl_buffer_t* buffer)
{
    gl_buffer_set_open(buffer, 0);
}

GL_DECL gl_buffer_t* GL_CALL gl_buffer_create_empty(void);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_from( ae_array_t* vertices, ae_vertex_format_t vertex_format,
                    ae_array_t* indices, ae_index_type_t index_type, int copy);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_cylinder(int slices, int stacks);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_torus(int slices, int stacks, float radius);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_parametric_sphere(int slices, int stacks);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_subdivided_sphere(int nsubdivisions);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_klein_bottle(int slices, int stacks);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_trefoil_knot(int slices, int stacks, float radius);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_hemisphere(int slices, int stacks);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_plane(int slices, int stacks);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_parametric_shape(void (*func)(float const* uv, float* xyz, void* data),
                                                int slices, int stacks, void* userdata);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_icosahedron(void);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_dodecahedron(void);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_octahedron(void);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_tetrahedron(void);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_cube(void);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_disk(float radius, int slices, float const* center, float const* normal);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_rock(int seed, int nsubdivisions);

GL_DECL gl_buffer_t* GL_CALL
gl_buffer_create_lsystem(char const* program, int slices, int maxdepth);

GL_DECL void GL_CALL // acquire buffer memory
gl_buffer_lock(gl_buffer_t* buffer, ae_array_t* vertices, ae_array_t* indices);

GL_DECL void GL_CALL // release buffer memory
gl_buffer_unlock(gl_buffer_t* buffer, ae_array_t* vertices, ae_vertex_format_t vertex_format,
                                            ae_array_t* indices, ae_index_type_t index_type);

GL_DECL void GL_CALL gl_buffer_get_vertex(gl_buffer_t* buffer,
        size_t which, float* t, float* c, float* n, float* p);

GL_DECL void GL_CALL gl_buffer_set_vertex(gl_buffer_t* buffer,
        size_t which, float* t, float* c, float* n, float* p);

GL_DECL void GL_CALL // verts
gl_buffer_append_vertex(gl_buffer_t* buffer, float* t, float* c, float* n, float* p);

GL_DECL void GL_CALL gl_buffer_get_face(gl_buffer_t* buffer,
        size_t which, size_t *v0, size_t *v1, size_t *v2);

GL_DECL void GL_CALL gl_buffer_set_face(gl_buffer_t* buffer,
        size_t which, size_t iv0, size_t iv1, size_t iv2);

GL_DECL void GL_CALL // inds
gl_buffer_append_face(gl_buffer_t* buffer, size_t v0, size_t v1, size_t v2);

GL_DECL void GL_CALL // add to scene
gl_buffer_merge(gl_buffer_t* dst, gl_buffer_t* src, int copy);

GL_DECL void GL_CALL // bounds
gl_buffer_get_aabbox(gl_buffer_t* buffer, float* minv, float* maxv);

GL_DECL void GL_CALL // move
gl_buffer_translate(gl_buffer_t* buffer, const float v[3]);

GL_DECL void GL_CALL // grow or shrink
gl_buffer_scale(gl_buffer_t* buffer, const float v[3]);

GL_DECL void GL_CALL // interpolate
gl_buffer_lerp(gl_buffer_t* buffer, gl_buffer_t* a, gl_buffer_t* b, float t);

GL_DECL void GL_CALL // row-major matrix multiply
gl_buffer_transform(gl_buffer_t* buffer, const float m[16], int normalize);

GL_DECL void GL_CALL // move center to (0, 0, 0)
gl_buffer_center_at_origin(gl_buffer_t* buffer);

GL_DECL float GL_CALL // 0 = all verts are shared, 1 = all unique
gl_buffer_unique_vertex_ratio(gl_buffer_t* buffer, float epsilon);

GL_DECL void GL_CALL // gen indices (compare)
gl_buffer_weld(gl_buffer_t* buffer, float epsilon);

GL_DECL void GL_CALL // gen indices (distance)
gl_buffer_weld_dist2(gl_buffer_t* buffer, float threshold2);

GL_DECL void GL_CALL // remove indices
gl_buffer_unweld(gl_buffer_t* buffer);

GL_DECL void GL_CALL // optimize index size
gl_buffer_shrink_indices(gl_buffer_t* buffer);

GL_DECL void GL_CALL // re-order for cache
gl_buffer_vertex_cache_optimize(gl_buffer_t* buffer);

GL_DECL double GL_CALL // ACMR
gl_buffer_vertex_cache_miss_ratio(gl_buffer_t* buffer, int cache_size, int FIFO);

GL_DECL int GL_CALL // raycasting
gl_buffer_intersect_ray3(gl_buffer_t * buffer,

                        const float ray_pos[3],
                        const float ray_dir[3],

                        u32 hit_indices[3],
                        float * hit_point,
                        float * t,
                        float * u,
                        float * v,

                        const float epsilon);

GL_DECL void GL_CALL // alpha sort
gl_buffer_depthsort(gl_buffer_t* buffer, const float p[3]);

GL_DECL void GL_CALL // reverse windings
gl_buffer_invert_faces(gl_buffer_t* buffer);

// emit GL calls
GL_DECL void GL_CALL gl_buffer_draw_ex(gl_buffer_t* buffer, gl_material_t*);
GL_DECL void GL_CALL gl_buffer_draw(gl_buffer_t* buffer);

GL_DECL void GL_CALL gl_buffer_draw_normals_ex(gl_buffer_t* buffer, // debugging
                        float* start_color, float* end_color, float line_width);

GL_DECL void GL_CALL gl_buffer_draw_normals(gl_buffer_t* buffer, float* rgba);

static c_inline size_t gl_buffer_count_all(void)
{
    return (size_t)gl_buffer_get_total(NULL);
}

GL_DECL void GL_CALL gl_buffer_list_all(gl_buffer_t** buffers);
GL_DECL void GL_CALL gl_buffer_close_all(void);

/*
================================================================================
 * ~~ [ particle system ] ~~ *
--------------------------------------------------------------------------------
*/

#define GL_PARTICLE_EMITTER_PROPERTY_N                                          \
                                                                                \
    N(GL_PARTICLE_EMITTER_PROPERTY_TOTAL, int, int, total)                      \
    N(GL_PARTICLE_EMITTER_PROPERTY_ID, int, int, id)                            \
                                                                                \
    N(GL_PARTICLE_EMITTER_PROPERTY_TEXTURE, gl_texture_t*, tex, texture)        \
    N(GL_PARTICLE_EMITTER_PROPERTY_OWNS_TEXTURE, int, int, owns_texture)        \
    N(GL_PARTICLE_EMITTER_PROPERTY_COLOR, float*, vec, color)                   \
                                                                                \
    N(GL_PARTICLE_EMITTER_PROPERTY_MIN_POINT_SIZE, float, flt, min_point_size)  \
    N(GL_PARTICLE_EMITTER_PROPERTY_MAX_POINT_SIZE, float, flt, max_point_size)  \
                                                                                \
    N(GL_PARTICLE_EMITTER_PROPERTY_POSITION, float*, vec, position)             \
    N(GL_PARTICLE_EMITTER_PROPERTY_POSITION_SIGMA, float*, vec, position_sigma) \
    N(GL_PARTICLE_EMITTER_PROPERTY_VELOCITY, float*, vec, velocity)             \
    N(GL_PARTICLE_EMITTER_PROPERTY_VELOCITY_SIGMA, float*, vec, velocity_sigma) \
    N(GL_PARTICLE_EMITTER_PROPERTY_GRAVITY, float*, vec, gravity)               \
                                                                                \
    /* velocity generation mode when filling / covering shapes or meshes.       \
     * this has nothing to do with how velocity is generated on update.         \
     */                                                                         \
    N(GL_PARTICLE_EMITTER_PROPERTY_VELGEN_MODE, int, int, velgen_mode)          \
    N(GL_PARTICLE_EMITTER_PROPERTY_VELGEN_SCALE, float, flt, velgen_scale)      \
                                                                                \
    N(GL_PARTICLE_EMITTER_PROPERTY_FROZEN, int, int, frozen)                    \
    N(GL_PARTICLE_EMITTER_PROPERTY_BURST_SIZE, int, int, burst_size)            \
    N(GL_PARTICLE_EMITTER_PROPERTY_NUM_PARTICLES, int, int, num_particles)      \
    N(GL_PARTICLE_EMITTER_PROPERTY_MAX_PARTICLES, int, int, max_particles)      \
    N(GL_PARTICLE_EMITTER_PROPERTY_PERIOD, double, dbl, period)                 \
                                                                                \
    N(GL_PARTICLE_EMITTER_PROPERTY_STATUS, const char*, str, status)            \
    N(GL_PARTICLE_EMITTER_PROPERTY_NAME, const char*, str, name)                \
                                                                                \
    N(GL_PARTICLE_EMITTER_PROPERTY_OPEN, int, int, open)                        \
    N(GL_PARTICLE_EMITTER_PROPERTY_COUNT, int, int, _)                          \

typedef enum gl_particle_emitter_property_t
{
    #define N(x, t, s, n) x,
    GL_PARTICLE_EMITTER_PROPERTY_N
    #undef N
} \
    gl_particle_emitter_property_t;

static const char* gl_particle_emitter_property_name[] =
{
    #define N(x, t, s, n) #x,
    GL_PARTICLE_EMITTER_PROPERTY_N
    #undef N
};

static const char* gl_particle_emitter_property_type[] =
{
    #define N(x, t, s, n) #s,
    GL_PARTICLE_EMITTER_PROPERTY_N
    #undef N
};

GL_DECL void GL_CALL
gl_particle_emitter_set_int(gl_particle_emitter_t*, gl_particle_emitter_property_t, int);

GL_DECL int GL_CALL
gl_particle_emitter_get_int(gl_particle_emitter_t*, gl_particle_emitter_property_t);

GL_DECL void GL_CALL
gl_particle_emitter_set_str(gl_particle_emitter_t*, gl_particle_emitter_property_t, const char*);

GL_DECL const char* GL_CALL
gl_particle_emitter_get_str(gl_particle_emitter_t*, gl_particle_emitter_property_t);

GL_DECL void GL_CALL
gl_particle_emitter_set_vec(gl_particle_emitter_t*, gl_particle_emitter_property_t, float*);

GL_DECL float* GL_CALL
gl_particle_emitter_get_vec(gl_particle_emitter_t*, gl_particle_emitter_property_t);

GL_DECL void GL_CALL
gl_particle_emitter_set_flt(gl_particle_emitter_t*, gl_particle_emitter_property_t, float);

GL_DECL float GL_CALL
gl_particle_emitter_get_flt(gl_particle_emitter_t*, gl_particle_emitter_property_t);

GL_DECL void GL_CALL
gl_particle_emitter_set_dbl(gl_particle_emitter_t*, gl_particle_emitter_property_t, double);

GL_DECL double GL_CALL
gl_particle_emitter_get_dbl(gl_particle_emitter_t*, gl_particle_emitter_property_t);

GL_DECL void GL_CALL
gl_particle_emitter_set_tex(gl_particle_emitter_t*, gl_particle_emitter_property_t, gl_texture_t*);

GL_DECL gl_texture_t* GL_CALL
gl_particle_emitter_get_tex(gl_particle_emitter_t*, gl_particle_emitter_property_t);

/* Make shorthand wrappers so we don't have to type SUPER_GIGANTIC_ENUM_NAMES in C code.
 */
#define N(x, typ, short_type, name)                                                         \
                                                                                            \
    static c_inline void gl_particle_emitter_set_ ## name (gl_particle_emitter_t* e, typ v) \
    {                                                                                       \
        gl_particle_emitter_set_ ## short_type (e, x, v);                                   \
    }                                                                                       \
                                                                                            \
    static c_inline typ gl_particle_emitter_get_ ## name (gl_particle_emitter_t* e)         \
    {                                                                                       \
        return gl_particle_emitter_get_ ## short_type (e, x);                               \
    }                                                                                       \

GL_PARTICLE_EMITTER_PROPERTY_N
#undef N

static c_inline void gl_particle_emitter_close(gl_particle_emitter_t* emitter)
{
    gl_particle_emitter_set_open(emitter, 0);
}

GL_DECL gl_particle_emitter_t* GL_CALL gl_particle_emitter_create(void);

GL_DECL void GL_CALL gl_particle_emitter_lock( gl_particle_emitter_t * emitter,
                                ae_array_t* vel_array, ae_array_t* particles);

GL_DECL void GL_CALL gl_particle_emitter_unlock(gl_particle_emitter_t* emitter,
                                ae_array_t* vel_array, ae_array_t* particles);

GL_DECL void GL_CALL gl_particle_emitter_get(gl_particle_emitter_t* emitter,
                                    size_t which, float * vel, float * pos);

GL_DECL void GL_CALL gl_particle_emitter_set(gl_particle_emitter_t* emitter,
                                    size_t which, float * vel, float * pos);

GL_DECL void GL_CALL gl_particle_emitter_append( gl_particle_emitter_t* emitter,
                                            float* vel, float* pos, int expand);

GL_DECL void GL_CALL gl_particle_emitter_merge( gl_particle_emitter_t * emitter,
                                        gl_particle_emitter_t* other, int copy);

GL_DECL void GL_CALL // this computes the axis-aligned bounding box of all active particles
gl_particle_emitter_get_aabbox(gl_particle_emitter_t* emitter, float* minv, float* maxv);

GL_DECL void GL_CALL // move all particles immediately (velocity not affected)
gl_particle_emitter_translate(gl_particle_emitter_t* emitter, float* vector);

GL_DECL void GL_CALL // scale the particle cloud (velocity unaffected)
gl_particle_emitter_scale(gl_particle_emitter_t* emitter, float* xyz);

GL_DECL void GL_CALL gl_particle_emitter_lerp(gl_particle_emitter_t* emitter,
                gl_particle_emitter_t* a, gl_particle_emitter_t* b, float t);

GL_DECL void GL_CALL // transform particles (velocity is not affected)
gl_particle_emitter_transform(gl_particle_emitter_t* emitter, float* matrix);

GL_DECL void GL_CALL // move the center of the cloud to (0, 0, 0)
gl_particle_emitter_center_at_origin(gl_particle_emitter_t* emitter);

#define GL_PARTICLE_EMITTER_VELGEN_MODE_N                               \
                                                                        \
    /* don't generate any particle velocity */                          \
    N(NONE, none)                                                       \
                                                                        \
    /* polygon particle velocity = scaled face normal */                \
    N(FACE_NORMAL, face_normal)                                         \
                                                                        \
    /* same method as above with a randomly scaled velocity */          \
    N(FACE_NORMAL_RANDOM, face_normal_random)                           \
                                                                        \
    /* same method as above with a random sign flip / inversion */      \
    N(FACE_NORMAL_BI_RANDOM, face_normal_bi_random)                     \
                                                                        \
    /* each entire triangle has a random scale */                       \
    N(FACE_NORMAL_TRIANGLE_RANDOM, face_normal_triangle_random)         \
                                                                        \
    /* same method as above with a random sign flip / inversion */      \
    N(FACE_NORMAL_TRIANGLE_BI_RANDOM, face_normal_triangle_bi_random)   \
                                                                        \
    /* each particle moves towards a random point on the unit sphere */ \
    N(RANDOM, random)                                                   \
                                                                        \
    /* zero out each velocity element for later regeneration */         \
    N(ZERO, zero)                                                       \
                                                                        \
    /* the number of velocity generation modes */                       \
    N(COUNT, _)                                                         \

typedef enum gl_particle_emitter_velgen_mode_t
{
    #define N(cap, low) GL_PARTICLE_EMITTER_VELGEN_MODE_ ## cap,
    GL_PARTICLE_EMITTER_VELGEN_MODE_N
    #undef N
} \
    gl_particle_emitter_velgen_mode_t;

static const char* gl_particle_emitter_velgen_mode_name[] =
{
    #define N(cap, low) AE_STRINGIFY(GL_PARTICLE_EMITTER_VELGEN_MODE_ ## cap),
    GL_PARTICLE_EMITTER_VELGEN_MODE_N
    #undef N
};

static const char* gl_particle_emitter_velgen_mode_short_name[] =
{
    #define N(cap, low) #low,
    GL_PARTICLE_EMITTER_VELGEN_MODE_N
    #undef N
};

GL_DECL gl_particle_emitter_velgen_mode_t GL_CALL // mode enum from string
        gl_particle_emitter_velgen_mode_from_short_name(const char* name);

GL_DECL void GL_CALL // if any particle is outside box, translate back to point inside box
gl_particle_emitter_aabbox_limit(gl_particle_emitter_t* emitter, float* minv, float* maxv);

GL_DECL void GL_CALL // clamp all particle positions to an axis-aligned bounding box
gl_particle_emitter_aabbox_clamp(gl_particle_emitter_t* emitter, float* minv, float* maxv);

GL_DECL void GL_CALL // set the position of each particle to a random pos on a box's face
gl_particle_emitter_aabbox_reset(gl_particle_emitter_t* emitter, float* minv, float* maxv);

GL_DECL void GL_CALL // if any particle is outside sphere, translate to point inside sphere
gl_particle_emitter_sphere_limit(gl_particle_emitter_t* emitter, float* center, float radius);

GL_DECL void GL_CALL // clamp particle positions to the face of a sphere (snowglobe effect)
gl_particle_emitter_sphere_clamp(gl_particle_emitter_t* emitter, float* center, float radius);

GL_DECL void GL_CALL // set the position of each particle to a random pos on a sphere's face
gl_particle_emitter_sphere_reset(gl_particle_emitter_t* emitter, float* center, float radius);

GL_DECL void GL_CALL
gl_particle_emitter_radius_limit(gl_particle_emitter_t* emitter, float radius);

GL_DECL void GL_CALL
gl_particle_emitter_radius_clamp(gl_particle_emitter_t* emitter, float radius);

GL_DECL void GL_CALL
gl_particle_emitter_radius_reset(gl_particle_emitter_t* emitter, float radius);

GL_DECL void GL_CALL // set the position of each particle to a random pos on a line segment
gl_particle_emitter_line_reset(gl_particle_emitter_t* emitter, float* minv, float* maxv);

GL_DECL void GL_CALL gl_particle_emitter_reset_all(void); // zero particle count
GL_DECL void GL_CALL gl_particle_emitter_reset(gl_particle_emitter_t* emitter);

GL_DECL void GL_CALL gl_particle_emitter_fill_all(void); // generate all particles
GL_DECL void GL_CALL gl_particle_emitter_fill(gl_particle_emitter_t* emitter);

GL_DECL void GL_CALL gl_particle_emitter_update_all(double dt); // simulate
GL_DECL void GL_CALL gl_particle_emitter_update(gl_particle_emitter_t* emitter, double dt);

GL_DECL void GL_CALL gl_particle_emitter_draw_all(void); // render particles
GL_DECL void GL_CALL gl_particle_emitter_draw(gl_particle_emitter_t* emitter);

GL_DECL void GL_CALL
gl_particle_emitter_draw_velocity_ex(gl_particle_emitter_t* emitter,
            float* start_color, float* end_color, float line_width);

GL_DECL void GL_CALL
gl_particle_emitter_draw_velocity(gl_particle_emitter_t* emitter, float* rgba);

static c_inline size_t gl_particle_emitter_count_all(void)
{
    return (size_t)gl_particle_emitter_get_total(NULL);
}

GL_DECL void GL_CALL gl_particle_emitter_list_all(gl_particle_emitter_t**);
GL_DECL void GL_CALL gl_particle_emitter_close_all(void);

// NOTE avoiding linker issues when changing build configurations,
// in case somebody accidentally types this into a header file...
#undef GL_DECL

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __GL_CORE_H__ */
