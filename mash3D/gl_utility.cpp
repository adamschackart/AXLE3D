/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
--- TODO: GL_(DEBUG)_LOG_ERROR_STATE_SCOPE; check gl error status at scope exit
--- TODO: should we disable fog before enter2D? does lighting off disable fog?
--- TODO: function for checking if an extension is supported (internal strset?)
----------------------------------------------------------------------------- */
#ifndef __GL_CORE_H__
#include <gl_core.h>
#endif

// TODO: use the system GL.h
#include <SDL2/SDL_opengl.h>

/*
================================================================================
 * ~~ [ GL state logging ] ~~ *
--------------------------------------------------------------------------------
TODO: gl_log_buffer_state and gl_log_shader_state, once those features are used
--------------------------------------------------------------------------------
*/

void gl_log_error_state_ex(const char* filename, const char* funcname)
{
    AE_PROFILE_SCOPE();

    switch (GL_GetError())
    {
        case GL_NO_ERROR: break;

        case GL_INVALID_ENUM:
            AE_WARN("GL_INVALID_ENUM (func \"%s\" file \"%s\")",
                                    funcname, filename); break;

        case GL_INVALID_VALUE:
            AE_WARN("GL_INVALID_VALUE (func \"%s\" file \"%s\")",
                                    funcname, filename); break;

        case GL_INVALID_OPERATION:
            AE_WARN("GL_INVALID_OPERATION (func \"%s\" file \"%s\")",
                                        funcname, filename); break;

        case GL_INVALID_FRAMEBUFFER_OPERATION:
            AE_WARN("GL_INVALID_FRAMEBUFFER_OPERATION (func \"%s\" file \"%s\")",
                                                    funcname, filename); break;

        case GL_OUT_OF_MEMORY:
            AE_WARN("GL_OUT_OF_MEMORY (func \"%s\" file \"%s\")",
                                    funcname, filename); break;

        case GL_STACK_UNDERFLOW:
            AE_WARN("GL_STACK_UNDERFLOW (func \"%s\" file \"%s\")",
                                        funcname, filename); break;

        case GL_STACK_OVERFLOW:
            AE_WARN("GL_STACK_OVERFLOW (func \"%s\" file \"%s\")",
                                        funcname, filename); break;

        default:
            AE_WARN("GL_UNKNOWN_ERROR (func \"%s\" file \"%s\")",
                                    funcname, filename); break;
    }
}

void gl_log_texture_state_ex(const char* filename, const char* funcname)
{
    GLuint id, num_textures = 0;
    GLint i;

    gl_log_error_state_ex(filename, funcname);

    GL_GetIntegerv(GL_ACTIVE_TEXTURE, &i); // show the active texture unit
    ae_log(OPENGL, "GL_ACTIVE_TEXTURE is %i", i - GL_TEXTURE0);

    GL_GetIntegerv(GL_TEXTURE_BINDING_2D, &i); // show the current texture
    ae_log(OPENGL, "GL_TEXTURE_BINDING_2D is %i", i);

    for (id = 1; id < 10000; id++) if (GL_IsTexture(id)) num_textures++;
    ae_log(OPENGL, "active texture count is %u", num_textures);

    GL_GetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &i);
    switch (i)
    {
        case GL_NEAREST:
            ae_log(OPENGL, "GL_TEXTURE_MIN_FILTER is GL_NEAREST"); break;

        case GL_LINEAR:
            ae_log(OPENGL, "GL_TEXTURE_MIN_FILTER is GL_LINEAR"); break;

        case GL_NEAREST_MIPMAP_NEAREST:
            ae_log(OPENGL, "GL_TEXTURE_MIN_FILTER is GL_NEAREST_MIPMAP_NEAREST"); break;

        case GL_LINEAR_MIPMAP_NEAREST:
            ae_log(OPENGL, "GL_TEXTURE_MIN_FILTER is GL_LINEAR_MIPMAP_NEAREST"); break;

        case GL_NEAREST_MIPMAP_LINEAR:
            ae_log(OPENGL, "GL_TEXTURE_MIN_FILTER is GL_NEAREST_MIPMAP_LINEAR"); break;

        case GL_LINEAR_MIPMAP_LINEAR:
            ae_log(OPENGL, "GL_TEXTURE_MIN_FILTER is GL_LINEAR_MIPMAP_LINEAR"); break;

        default:
            ae_log(OPENGL, "GL_TEXTURE_MIN_FILTER is GL_UNKNOWN"); break;
    }

    GL_GetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &i);
    switch (i)
    {
        case GL_NEAREST:
            ae_log(OPENGL, "GL_TEXTURE_MAG_FILTER is GL_NEAREST"); break;

        case GL_LINEAR:
            ae_log(OPENGL, "GL_TEXTURE_MAG_FILTER is GL_LINEAR"); break;

        default:
            ae_log(OPENGL, "GL_TEXTURE_MAG_FILTER is GL_UNKNOWN"); break;
    }

    #define W(e) do                                                     \
    {                                                                   \
        GL_GetTexParameteriv(GL_TEXTURE_2D, e, &i);                     \
        switch (i)                                                      \
        {                                                               \
            case GL_CLAMP:                                              \
                ae_log(OPENGL, "%s is GL_CLAMP", #e); break;            \
                                                                        \
            case GL_CLAMP_TO_BORDER:                                    \
                ae_log(OPENGL, "%s is GL_CLAMP_TO_BORDER", #e); break;  \
                                                                        \
            case GL_CLAMP_TO_EDGE:                                      \
                ae_log(OPENGL, "%s is GL_CLAMP_TO_EDGE", #e); break;    \
                                                                        \
            case GL_MIRRORED_REPEAT:                                    \
                ae_log(OPENGL, "%s is GL_MIRRORED_REPEAT", #e); break;  \
                                                                        \
            case GL_REPEAT:                                             \
                ae_log(OPENGL, "%s is GL_REPEAT", #e); break;           \
                                                                        \
            default:                                                    \
                ae_log(OPENGL, "%s is GL_UNKNOWN", #e); break;          \
        }                                                               \
    }                                                                   \
    while (AE_NULL_WHILE_LOOP_CONDITION)

    W(GL_TEXTURE_WRAP_S);
    W(GL_TEXTURE_WRAP_T);
    W(GL_TEXTURE_WRAP_R);

    #undef W
    gl_log_error_state();
}

void gl_log_blend_state_ex(const char* filename, const char* funcname)
{
    float rgba[4];
    GLint i;

    gl_log_error_state_ex(filename, funcname);

    GL_GetFloatv(GL_BLEND_COLOR, rgba);
    ae_log(OPENGL, "blend color is { %.2f %.2f %.2f %.2f }",
                        rgba[0], rgba[1], rgba[2], rgba[3]);

    #define B(e) \
    \
    GL_GetIntegerv(e, &i); \
    switch (i) \
    { \
        case GL_ZERO: \
            ae_log(OPENGL, "%s is GL_ZERO", #e); break; \
        \
        case GL_ONE: \
            ae_log(OPENGL, "%s is GL_ONE", #e); break; \
        \
        case GL_SRC_COLOR: \
            ae_log(OPENGL, "%s is GL_SRC_COLOR", #e); break; \
        \
        case GL_ONE_MINUS_SRC_COLOR: \
            ae_log(OPENGL, "%s is GL_ONE_MINUS_SRC_COLOR", #e); break; \
        \
        case GL_DST_COLOR: \
            ae_log(OPENGL, "%s is GL_DST_COLOR", #e); break; \
        \
        case GL_ONE_MINUS_DST_COLOR: \
            ae_log(OPENGL, "%s is GL_ONE_MINUS_DST_COLOR", #e); break; \
        \
        case GL_SRC_ALPHA: \
            ae_log(OPENGL, "%s is GL_SRC_ALPHA", #e); break; \
        \
        case GL_ONE_MINUS_SRC_ALPHA: \
            ae_log(OPENGL, "%s is GL_ONE_MINUS_SRC_ALPHA", #e); break; \
        \
        case GL_DST_ALPHA: \
            ae_log(OPENGL, "%s is GL_DST_ALPHA", #e); break; \
        \
        case GL_ONE_MINUS_DST_ALPHA: \
            ae_log(OPENGL, "%s is GL_ONE_MINUS_DST_ALPHA", #e); break; \
        \
        case GL_CONSTANT_COLOR: \
            ae_log(OPENGL, "%s is GL_CONSTANT_COLOR", #e); break; \
        \
        case GL_ONE_MINUS_CONSTANT_COLOR: \
            ae_log(OPENGL, "%s is GL_ONE_MINUS_CONSTANT_COLOR", #e); break; \
        \
        case GL_CONSTANT_ALPHA: \
            ae_log(OPENGL, "%s is GL_CONSTANT_ALPHA", #e); break; \
        \
        case GL_ONE_MINUS_CONSTANT_ALPHA: \
            ae_log(OPENGL, "%s is GL_ONE_MINUS_CONSTANT_ALPHA", #e); break; \
        \
        case GL_SRC_ALPHA_SATURATE: \
            ae_log(OPENGL, "%s is GL_SRC_ALPHA_SATURATE", #e); break; \
        \
        case GL_SRC1_COLOR: \
            ae_log(OPENGL, "%s is GL_SRC1_COLOR", #e); break; \
        \
        case GL_ONE_MINUS_SRC1_COLOR: \
            ae_log(OPENGL, "%s is GL_ONE_MINUS_SRC1_COLOR", #e); break; \
        \
        case GL_SRC1_ALPHA: \
            ae_log(OPENGL, "%s is GL_SRC1_ALPHA", #e); break; \
        \
        case GL_ONE_MINUS_SRC1_ALPHA: \
            ae_log(OPENGL, "%s is GL_ONE_MINUS_SRC1_ALPHA", #e); break; \
        \
        default: \
            ae_log(OPENGL, "%s is GL_UNKNOWN", #e); break; \
    }

    B(GL_BLEND_SRC_RGB);
    B(GL_BLEND_SRC_ALPHA);
    B(GL_BLEND_DST_RGB);
    B(GL_BLEND_DST_ALPHA);

    #define C(e) \
    \
    GL_GetIntegerv(e, &i); \
    switch (i) \
    { \
        case GL_FUNC_ADD: \
            ae_log(OPENGL, "%s is GL_FUNC_ADD", #e); break; \
        \
        case GL_FUNC_SUBTRACT: \
            ae_log(OPENGL, "%s is GL_FUNC_SUBTRACT", #e); break; \
        \
        case GL_FUNC_REVERSE_SUBTRACT: \
            ae_log(OPENGL, "%s is GL_FUNC_REVERSE_SUBTRACT", #e); break; \
        \
        case GL_MIN: \
            ae_log(OPENGL, "%s is GL_MIN", #e); break; \
        \
        case GL_MAX: \
            ae_log(OPENGL, "%s is GL_MAX", #e); break; \
    }

    C(GL_BLEND_EQUATION_RGB);
    C(GL_BLEND_EQUATION_ALPHA);

    #undef B
    #undef C

    gl_log_error_state();
}

void gl_log_material_state_ex(const char* filename, const char* funcname)
{
    gl_log_error_state_ex(filename, funcname);

    #define M(e) do                                 \
    {                                               \
        float v[4]; /* RGB reflectance property */  \
        GL_GetMaterialfv(GL_FRONT, e, v);           \
                                                    \
        ae_log(OPENGL, "%s is { %.2f %.2f %.2f }",  \
                            #e, v[0], v[1], v[2]);  \
    }                                               \
    while (AE_NULL_WHILE_LOOP_CONDITION)

    M(GL_AMBIENT);
    M(GL_DIFFUSE);
    M(GL_SPECULAR);
    M(GL_EMISSION);

    #undef M
    float v;

    GL_GetMaterialfv(GL_FRONT, GL_SHININESS, &v);
    ae_log(OPENGL, "GL_SHININESS is %.2f", v);

    gl_log_error_state();
}

void gl_log_light_state_ex(const char* filename, const char* funcname)
{
    gl_log_error_state_ex(filename, funcname);

    // if light is disabled, we're likely in 2D "sprite" or "billboard" mode
    if (GL_IsEnabled(GL_LIGHTING)) ae_log(OPENGL, "GL_LIGHTING is enabled");

    // print at least one thing if lighting is off
    else ae_log(OPENGL, "GL_LIGHTING is disabled");

    GLint i = 0;

    GL_GetIntegerv(GL_LIGHT_MODEL_LOCAL_VIEWER, &i);
    if (i) ae_log(OPENGL, "GL_LIGHT_MODEL_LOCAL_VIEWER is enabled");

    GL_GetIntegerv(GL_LIGHT_MODEL_TWO_SIDE, &i);
    if (i) ae_log(OPENGL, "GL_LIGHT_MODEL_TWO_SIDE is enabled");

    #define L(e) do                                                             \
    {                                                                           \
        if (GL_IsEnabled(e))                                                    \
        {                                                                       \
            /* TODO: this is ugly! make a macro and run each entry through it.  \
             * TODO: color highlight non-default values for visual separation.  \
             */                                                                 \
            ae_log(OPENGL, "%s is enabled", #e);                                \
            float v[4];                                                         \
                                                                                \
            GL_GetLightfv(e, GL_AMBIENT, v);                                    \
            ae_log(OPENGL,"\tGL_AMBIENT { %.2f %.2f %.2f %.2f }",               \
                                        v[0], v[1], v[2], v[3]);                \
                                                                                \
            GL_GetLightfv(e, GL_DIFFUSE, v);                                    \
            ae_log(OPENGL,"\tGL_DIFFUSE { %.2f %.2f %.2f %.2f }",               \
                                        v[0], v[1], v[2], v[3]);                \
                                                                                \
            GL_GetLightfv(e, GL_SPECULAR, v);                                   \
            ae_log(OPENGL,"\tGL_SPECULAR { %.2f %.2f %.2f %.2f }",              \
                                        v[0], v[1], v[2], v[3]);                \
                                                                                \
            GL_GetLightfv(e, GL_POSITION, v);                                   \
            ae_log(OPENGL,"\tGL_POSITION { %.2f %.2f %.2f %.2f }",              \
                                        v[0], v[1], v[2], v[3]);                \
                                                                                \
            GL_GetLightfv(e, GL_SPOT_DIRECTION, v);                             \
            ae_log(OPENGL,"\tGL_SPOT_DIRECTION { %.2f %.2f %.2f %.2f }",        \
                                                v[0], v[1], v[2], v[3]);        \
                                                                                \
            GL_GetLightfv(e, GL_SPOT_EXPONENT, v);                              \
            ae_log(OPENGL,"\tGL_SPOT_EXPONENT { %.2f %.2f %.2f %.2f }",         \
                                                v[0], v[1], v[2], v[3]);        \
                                                                                \
            GL_GetLightfv(e, GL_SPOT_CUTOFF, v);                                \
            ae_log(OPENGL,"\tGL_SPOT_CUTOFF { %.2f %.2f %.2f %.2f }",           \
                                            v[0], v[1], v[2], v[3]);            \
                                                                                \
            GL_GetLightfv(e, GL_CONSTANT_ATTENUATION, v);                       \
            ae_log(OPENGL,"\tGL_CONSTANT_ATTENUATION { %.2f %.2f %.2f %.2f }",  \
                                                    v[0], v[1], v[2], v[3]);    \
                                                                                \
            GL_GetLightfv(e, GL_LINEAR_ATTENUATION, v);                         \
            ae_log(OPENGL,"\tGL_LINEAR_ATTENUATION { %.2f %.2f %.2f %.2f }",    \
                                                    v[0], v[1], v[2], v[3]);    \
                                                                                \
            GL_GetLightfv(e, GL_QUADRATIC_ATTENUATION, v);                      \
            ae_log(OPENGL,"\tGL_QUADRATIC_ATTENUATION { %.2f %.2f %.2f %.2f }", \
                                                    v[0], v[1], v[2], v[3]);    \
        }                                                                       \
    }                                                                           \
    while (AE_NULL_WHILE_LOOP_CONDITION)

    L(GL_LIGHT0);
    L(GL_LIGHT1);
    L(GL_LIGHT2);
    L(GL_LIGHT3);
    L(GL_LIGHT4);
    L(GL_LIGHT5);
    L(GL_LIGHT6);
    L(GL_LIGHT7);

    #undef L
    gl_log_error_state();
}

void gl_log_internal_state_ex(const char* filename, const char* funcname)
{
    #if 0
    #define E(e) ae_log(OPENGL, "%s is %sabled", #e, GL_IsEnabled(e) ? "en" : "dis");
    #else
    #define E(e) if (GL_IsEnabled(e)) ae_log(OPENGL, "%s is enabled", #e);
    #endif

    #define I(e) do                         \
    {                                       \
        GLint i;                            \
        GL_GetIntegerv(e, &i);              \
        ae_log(OPENGL, "%s: %i", #e, i);    \
    }                                       \
    while (AE_NULL_WHILE_LOOP_CONDITION)

    #define F(e) do                         \
    {                                       \
        GLfloat v;                          \
        GL_GetFloatv(e, &v);                \
        ae_log(OPENGL, "%s: %f", #e, v);    \
    }                                       \
    while (AE_NULL_WHILE_LOOP_CONDITION)

    gl_log_error_state_ex(filename, funcname);

    ae_log(OPENGL, // 70 char banner
    "----------------------------------------------------------------------");

    ae_log(OPENGL, "vendor is \"%s\"", GL_GetString(GL_VENDOR));
    ae_log(OPENGL, "renderer is \"%s\"", GL_GetString(GL_RENDERER));
    ae_log(OPENGL, "version is \"%s\"", GL_GetString(GL_VERSION));

    /* TODO: split up the extension string and log each entry?
     */
    ae_log(OPENGL, "shading language version is \"%s\"",
                    GL_GetString(GL_SHADING_LANGUAGE_VERSION));

    ae_log(OPENGL, // 70 char banner
    "----------------------------------------------------------------------");

    // I(GL_ACCUM_ALPHA_BITS);
    // I(GL_ACCUM_BLUE_BITS);
    // I(GL_ACCUM_GREEN_BITS);
    // I(GL_ACCUM_RED_BITS);

    // I(GL_ALPHA_BITS);
    // I(GL_BLUE_BITS);
    // I(GL_GREEN_BITS);
    // I(GL_RED_BITS);

    // I(GL_DEPTH_BITS);

    I(GL_MODELVIEW_STACK_DEPTH);
    I(GL_PROJECTION_STACK_DEPTH);
    I(GL_TEXTURE_STACK_DEPTH);

    int viewport[4]; // get current viewport (window) dimensions
    GL_GetIntegerv(GL_VIEWPORT, viewport);

    ae_log(OPENGL, "viewport is    { x: %i y: %i w: %i h: %i }",
            viewport[0], viewport[1], viewport[2], viewport[3]);

    int scissor[4]; // get the current scissoring box dimensions
    GL_GetIntegerv(GL_SCISSOR_BOX, scissor);

    ae_log(OPENGL, "scissor box is { x: %i y: %i w: %i h: %i }",
                scissor[0], scissor[1], scissor[2], scissor[3]);

    ae_log(OPENGL,
    "----------------------------------------------------------------------");

    gl_log_texture_state_ex(filename, funcname);

    ae_log(OPENGL,
    "----------------------------------------------------------------------");

    gl_log_blend_state_ex(filename, funcname);

    ae_log(OPENGL,
    "----------------------------------------------------------------------");

    gl_log_material_state_ex(filename, funcname);

    ae_log(OPENGL,
    "----------------------------------------------------------------------");

    gl_log_light_state_ex(filename, funcname);

    ae_log(OPENGL,
    "----------------------------------------------------------------------");

    F(GL_POINT_SIZE);
    F(GL_LINE_WIDTH);

    E(GL_ALPHA_TEST);
    E(GL_AUTO_NORMAL);
    E(GL_BLEND);                // TODO: move to gl_log_blend_state_ex
    E(GL_CLIP_PLANE0);
    E(GL_CLIP_PLANE1);
    E(GL_CLIP_PLANE2);
    E(GL_CLIP_PLANE3);
    E(GL_CLIP_PLANE4);
    E(GL_CLIP_PLANE5);
    E(GL_COLOR_ARRAY);
    E(GL_COLOR_LOGIC_OP);
    E(GL_COLOR_MATERIAL);       // TODO: move to gl_log_material_state_ex
    E(GL_COLOR_SUM);
    E(GL_CULL_FACE);
    E(GL_DEPTH_TEST);
    E(GL_DITHER);
    E(GL_EDGE_FLAG_ARRAY);
    E(GL_FOG);
    E(GL_FOG_COORD_ARRAY);
    E(GL_INDEX_ARRAY);
    E(GL_INDEX_LOGIC_OP);
    E(GL_LINE_SMOOTH);
    E(GL_LINE_STIPPLE);
    E(GL_MAP1_COLOR_4);
    E(GL_MAP1_INDEX);
    E(GL_MAP1_NORMAL);
    E(GL_MAP1_TEXTURE_COORD_1);
    E(GL_MAP1_TEXTURE_COORD_2);
    E(GL_MAP1_TEXTURE_COORD_3);
    E(GL_MAP1_TEXTURE_COORD_4);
    E(GL_MAP2_COLOR_4);
    E(GL_MAP2_INDEX);
    E(GL_MAP2_NORMAL);
    E(GL_MAP2_TEXTURE_COORD_1);
    E(GL_MAP2_TEXTURE_COORD_2);
    E(GL_MAP2_TEXTURE_COORD_3);
    E(GL_MAP2_TEXTURE_COORD_4);
    E(GL_MAP2_VERTEX_3);
    E(GL_MAP2_VERTEX_4);
    E(GL_MULTISAMPLE);
    E(GL_NORMAL_ARRAY);
    E(GL_NORMALIZE);
    E(GL_POINT_SMOOTH);
    E(GL_POINT_SPRITE);
    E(GL_POLYGON_SMOOTH);
    E(GL_POLYGON_OFFSET_FILL);
    E(GL_POLYGON_OFFSET_LINE);
    E(GL_POLYGON_OFFSET_POINT);
    E(GL_POLYGON_STIPPLE);
    E(GL_RESCALE_NORMAL);
    E(GL_SAMPLE_ALPHA_TO_COVERAGE);
    E(GL_SAMPLE_ALPHA_TO_ONE);
    E(GL_SAMPLE_COVERAGE);
    E(GL_SCISSOR_TEST);
    E(GL_SECONDARY_COLOR_ARRAY);
    E(GL_STENCIL_TEST);
    E(GL_TEXTURE_1D);           // TODO: move to gl_log_texture_state_ex
    E(GL_TEXTURE_2D);           // TODO: move to gl_log_texture_state_ex
    E(GL_TEXTURE_3D);           // TODO: move to gl_log_texture_state_ex
    E(GL_TEXTURE_COORD_ARRAY);  // TODO: move to gl_log_texture_state_ex
    E(GL_TEXTURE_CUBE_MAP);     // TODO: move to gl_log_texture_state_ex
    E(GL_TEXTURE_GEN_Q);        // TODO: move to gl_log_texture_state_ex
    E(GL_TEXTURE_GEN_R);        // TODO: move to gl_log_texture_state_ex
    E(GL_TEXTURE_GEN_S);        // TODO: move to gl_log_texture_state_ex
    E(GL_TEXTURE_GEN_T);        // TODO: move to gl_log_texture_state_ex
    E(GL_VERTEX_ARRAY);
    E(GL_VERTEX_PROGRAM_POINT_SIZE);
    E(GL_VERTEX_PROGRAM_TWO_SIDE);

    #undef E
    #undef I
    #undef F

    gl_log_error_state();
}

/*
================================================================================
 * ~~ [ utilities ] ~~ *
--------------------------------------------------------------------------------
*/

void gl_enter3D(int width, int height, double FOV, double znear, double zfar)
{
    AE_PROFILE_SCOPE();
    gl_init();

    // FIXME: this assert fires on win32 debug builds, so we have to hack it.
    #if 0
    ae_assert(width && height, "%i %i", width, height);
    #else
    if (width == 0 || height == 0)
    {
        AE_WARN("%s got bad viewport dimensions: %ix%i - using qHD fallback",
                                                __FUNCTION__, width, height);
        width  = 1920 / 2;
        height = 1080 / 2;
    }
    #endif
    GL_PushAttrib(GL_ALL_ATTRIB_BITS);

    GL_Viewport(0, 0, width, height);
    GL_Scissor(0, 0, width, height);

    GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    GL_DepthFunc(GL_LEQUAL);

    GL_Enable(GL_CULL_FACE);
    GL_CullFace(GL_BACK);

    GL_Enable(GL_MULTISAMPLE);
    GL_Enable(GL_DEPTH_TEST);
    GL_Enable(GL_LIGHTING);

    // this is enabled by default in some GL drivers, and not in others
    GL_Disable(GL_LIGHT0);

    GL_MatrixMode(GL_PROJECTION);
    GL_PushMatrix();
    GL_LoadIdentity();

    const double aspect = (double)width / (double)height;

    #if 0
        const double h = tan(FOV / 360.0 * M_PI) * znear;
        const double w = h * aspect;

        GL_Frustum(-w, w, -h, h, znear, zfar);
    #else
        float matrix[16] =
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1,
        };

        mat4x4col_perspective(matrix, FOV, aspect, znear, zfar);
        GL_MultMatrixf(matrix);
    #endif

    GL_MatrixMode(GL_MODELVIEW);
    GL_PushMatrix();
    GL_LoadIdentity();

    gl_debug_log_error_state();
}

void gl_leave3D(void)
{
    AE_PROFILE_SCOPE();
    gl_init();

    GL_MatrixMode(GL_PROJECTION);
    GL_PopMatrix();

    GL_MatrixMode(GL_MODELVIEW);
    GL_PopMatrix();

    GL_PopAttrib();
    gl_debug_log_error_state();
}

void gl_enter2D(int width, int height)
{
    AE_PROFILE_SCOPE();
    gl_init();

    GL_PushAttrib(GL_ENABLE_BIT);
    GL_Disable(GL_DEPTH_TEST);
    GL_Disable(GL_LIGHTING);

    GL_MatrixMode(GL_PROJECTION);
    GL_PushMatrix();
    GL_LoadIdentity();
    GL_Ortho(0, width, 0, height, -1, 1);

    GL_MatrixMode(GL_MODELVIEW);
    GL_PushMatrix();
    GL_LoadIdentity();

    gl_debug_log_error_state();
}

void gl_leave2D(void)
{
    AE_PROFILE_SCOPE();
    gl_init();

    GL_PopMatrix();
    GL_MatrixMode(GL_PROJECTION);

    GL_PopMatrix();
    GL_MatrixMode(GL_MODELVIEW);

    GL_PopAttrib();
    gl_debug_log_error_state();
}

/* ===== [ 2D shapes ] ====================================================== */

void gl_rect_ex(float* rect, const float* bl, const float* br, const float* tr,
                            const float* tl, int is_outline, float line_width)
{
    AE_PROFILE_SCOPE();
    gl_init();

    GL_PushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_COLOR_BUFFER_BIT | GL_LINE_BIT);
    {
        GL_Disable(GL_TEXTURE_2D);
        GL_LineWidth(line_width);

        GL_Enable(GL_BLEND);
        GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        GL_Begin(is_outline ? GL_LINE_STRIP : GL_QUADS);
        {
            const float x = rect[0];
            const float y = rect[1];
            const float w = rect[2];
            const float h = rect[3];

            GL_Color4fv(bl); // bottom left vertex
            GL_Vertex2f(x + 0, y + 0);

            GL_Color4fv(br); // bottom right vertex
            GL_Vertex2f(x + w, y + 0);

            GL_Color4fv(tr); // top right vertex
            GL_Vertex2f(x + w, y + h);

            GL_Color4fv(tl); // top left vertex
            GL_Vertex2f(x + 0, y + h);

            ae_if (is_outline)
            {
                // close it off
                GL_Color4fv(bl); GL_Vertex2f(x, y);
            }
        }
        GL_End();
    }
    GL_PopAttrib();

    gl_debug_log_error_state();
}

void gl_outline(float* rect, const float* rgba)
{
    gl_rect_ex(rect, rgba, rgba, rgba, rgba, 1, 1.0f);
}

void gl_rect(float* rect, const float* rgba)
{
    gl_rect_ex(rect, rgba, rgba, rgba, rgba, 0, 1.0f);
}

/* ===== [ 3D shapes ] ====================================================== */

void gl_aabbox_ex(float* minv, float* maxv, const float* rgba, float line_width)
{
    AE_PROFILE_SCOPE();
    gl_init();

    GL_PushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LINE_BIT);
    {
        GL_Disable(GL_TEXTURE_2D);
        GL_Disable(GL_LIGHTING);

        GL_LineWidth(line_width);
        GL_Color4fv(rgba);

        GL_Enable(GL_BLEND);
        GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        GL_Begin(GL_LINES);
        {
            // top
            GL_Vertex3f(minv[0], maxv[1], minv[2]);
            GL_Vertex3f(maxv[0], maxv[1], minv[2]);
            GL_Vertex3f(maxv[0], maxv[1], minv[2]);
            GL_Vertex3f(maxv[0], maxv[1], maxv[2]);
            GL_Vertex3f(maxv[0], maxv[1], maxv[2]);
            GL_Vertex3f(minv[0], maxv[1], maxv[2]);
            GL_Vertex3f(minv[0], maxv[1], maxv[2]);
            GL_Vertex3f(minv[0], maxv[1], minv[2]);

            // bottom
            GL_Vertex3f(minv[0], minv[1], minv[2]);
            GL_Vertex3f(maxv[0], minv[1], minv[2]);
            GL_Vertex3f(maxv[0], minv[1], minv[2]);
            GL_Vertex3f(maxv[0], minv[1], maxv[2]);
            GL_Vertex3f(maxv[0], minv[1], maxv[2]);
            GL_Vertex3f(minv[0], minv[1], maxv[2]);
            GL_Vertex3f(minv[0], minv[1], maxv[2]);
            GL_Vertex3f(minv[0], minv[1], minv[2]);

            // sides
            GL_Vertex3f(minv[0], minv[1], minv[2]);
            GL_Vertex3f(minv[0], maxv[1], minv[2]);
            GL_Vertex3f(maxv[0], minv[1], minv[2]);
            GL_Vertex3f(maxv[0], maxv[1], minv[2]);
            GL_Vertex3f(maxv[0], minv[1], maxv[2]);
            GL_Vertex3f(maxv[0], maxv[1], maxv[2]);
            GL_Vertex3f(minv[0], minv[1], maxv[2]);
            GL_Vertex3f(minv[0], maxv[1], maxv[2]);
        }
        GL_End();
    }
    GL_PopAttrib();

    gl_debug_log_error_state();
}

void gl_aabbox(float* minv, float* maxv, const float* rgba)
{
    gl_aabbox_ex(minv, maxv, rgba, 1.0f);
}

void gl_ellipsoid(float* center, float* extent, const float* rgba)
{
    AE_PROFILE_SCOPE();

    gl_material_t* mtl = gl_material_create();

    gl_material_set_all(mtl, rgba);
    gl_material_set_opacity(mtl, rgba[3]);

    // TODO: set the subdivision level based on the volume of the shape
    #if 0
        gl_buffer_t* buf = gl_buffer_create_parametric_sphere(12, 12);
    #else
        gl_buffer_t* buf = gl_buffer_create_subdivided_sphere(1);
    #endif

    /* TODO: do this in hardware with gl_coord3D_push_object_transform
     */
    gl_buffer_center_at_origin(buf);
    gl_buffer_scale(buf, extent);
    gl_buffer_translate(buf, center);

    gl_buffer_set_line_mode(buf, 1);
    gl_buffer_draw_ex(buf, mtl);

    gl_material_close(mtl);
    gl_buffer_close(buf);
}

void gl_sphere(float* center, float radius, const float* rgba)
{
    AE_PROFILE_SCOPE();

    float extent[3] = { radius, radius, radius };
    gl_ellipsoid(center, extent, rgba);
}

/*
================================================================================
 * ~~ [ coordinate system ] ~~ *
--------------------------------------------------------------------------------
TODO: return post-transform stack depth (first xform push would return 1, etc.)
TODO: functions to get the entire camera and object transform coordinate stacks
--------------------------------------------------------------------------------
*/

void gl_coord3D_apply_camera_transform_ex(gl_coord3D_t* coord, int absolute)
{
    AE_PROFILE_SCOPE();
    gl_init();

    gl_debug_log_error_state();

    // TODO check to make sure we're in modelview matrix mode, which
    // should be default (perspective transforms are much more rare).
    ae_if (absolute) GL_LoadIdentity();

    GL_Scalef(coord->scale_x, coord->scale_y, coord->scale_z);

    GL_Rotatef(-coord->pitch, 1.0f, 0.0f, 0.0f);
    GL_Rotatef(-coord->heading, 0.0f, 1.0f, 0.0f);
    GL_Rotatef(-coord->roll, 0.0f, 0.0f, 1.0f);

    GL_Translatef(-coord->x, -coord->y, -coord->z);

    gl_debug_log_error_state();
}

void gl_coord3D_push_camera_transform_ex(gl_coord3D_t* coord, int absolute)
{
    AE_PROFILE_SCOPE();
    gl_init();

    gl_debug_log_error_state();

    GL_PushMatrix();
    gl_coord3D_apply_camera_transform_ex(coord, absolute);

    gl_debug_log_error_state();
}

void gl_coord3D_pop_camera_transform(void)
{
    AE_PROFILE_SCOPE();
    gl_init();

    gl_debug_log_error_state();
    GL_PopMatrix();
    gl_debug_log_error_state();
}

void gl_coord3D_apply_object_transform_ex(gl_coord3D_t* coord, int absolute)
{
    AE_PROFILE_SCOPE();
    gl_init();

    gl_debug_log_error_state();

    // TODO assert that we're in modelview matrix mode (should be default)
    ae_if (absolute) GL_LoadIdentity();

    GL_Translatef(coord->x, coord->y, coord->z);

    GL_Rotatef(coord->roll, 0.0f, 0.0f, 1.0f);
    GL_Rotatef(coord->heading, 0.0f, 1.0f, 0.0f);
    GL_Rotatef(coord->pitch, 1.0f, 0.0f, 0.0f);

    /* TODO: could this be the right place to put GL_Enable(GL_NORMALIZE) in
     * the case of lighting enabled and a non-identity object scale factor?
     * this means we'd have to push to the attribute stack, which is hacky...
     */
    GL_Scalef(coord->scale_x, coord->scale_y, coord->scale_z);

    gl_debug_log_error_state();
}

void gl_coord3D_push_object_transform_ex(gl_coord3D_t* coord, int absolute)
{
    AE_PROFILE_SCOPE();
    gl_init();

    gl_debug_log_error_state();

    GL_PushMatrix();
    gl_coord3D_apply_object_transform_ex(coord, absolute);

    gl_debug_log_error_state();
}

void gl_coord3D_pop_object_transform(void)
{
    AE_PROFILE_SCOPE();
    gl_init();

    gl_debug_log_error_state();
    GL_PopMatrix();
    gl_debug_log_error_state();
}

/* ===== [ conversion ] ===================================================== */

void gl_coord3D_heading_towards_point(gl_coord3D_t* coord, float* p)
{
    float s[3];

    vec3sub_vec(s, p, coord->position);
    coord->heading = -gl_normal_to_heading_degrees(s);
}

void gl_coord3D_pitch_towards_point(gl_coord3D_t* coord, float* p)
{
    float s[3];

    vec3sub_vec(s, p, coord->position);
    coord->pitch = gl_normal_to_pitch_degrees(s);
}

void gl_coord3D_face_towards_point(gl_coord3D_t* coord, float* p)
{
    float s[3];

    vec3sub_vec(s, p, coord->position);
    gl_coord3D_set_normal(coord, s);
}

void gl_coord3D_get_matrix(gl_coord3D_t* coord, float* matrix)
{
    mat4x4identity(matrix);

    mat4x4col_rotate_euler( matrix, ae_radiansf(coord->heading),
                                    ae_radiansf(coord->roll),
                                    ae_radiansf(coord->pitch) );
}

void gl_coord3D_set_matrix(gl_coord3D_t* coord, float* matrix)
{
    AE_STUB();
}

void gl_coord3D_get_quat(gl_coord3D_t* coord, float* quat)
{
    quat_new_rotate_euler(quat, ae_radiansf(coord->heading),
                                ae_radiansf(coord->roll),
                                ae_radiansf(coord->pitch));
}

void gl_coord3D_set_quat(gl_coord3D_t* coord, float* quat)
{
    AE_STUB();
}

void gl_coord3D_get_normal(gl_coord3D_t* coord, float* normal)
{
    #if 0
        float m[16], v[3] = { 0.0f, 0.0f, -1.0f };
        gl_coord3D_get_matrix(coord, m);
        mat4x4col_mul_vector3(normal, m, v);
    #elif 0
        float q[4], v[3] = { 0.0f, 0.0f, -1.0f };
        gl_coord3D_get_quat(coord, q);
        quat_mul_vector3(normal, q, v);
    #else
        // XXX FIXME: -heading (clockwise) indicates we're wrong somewhere.
        euler_degrees_to_gl_normal(normal, -coord->heading, coord->pitch);
    #endif
}

void gl_coord3D_set_normal(gl_coord3D_t* coord, float* normal)
{
    coord->heading = -gl_normal_to_heading_degrees(normal);
    coord->pitch = gl_normal_to_pitch_degrees(normal);
}

void gl_coord3D_get_ray(gl_coord3D_t* coord, float* p, float* v)
{
    vec3copy(p, coord->translation);
    gl_coord3D_get_normal(coord, v);
}

void gl_coord3D_set_ray(gl_coord3D_t* coord, float* p, float* v)
{
    vec3copy(coord->translation, p);
    gl_coord3D_set_normal(coord, v);
}

void gl_coord3D_get_up_vector(gl_coord3D_t* coord, float* v)
{
    float obj_forward[3]; // get the facing direction
    gl_coord3D_get_normal(coord, obj_forward);

    static const float up[3] = { 0.0f, 1.0f, 0.0f };
    vec3up_from_forward(v, (float*)up, obj_forward);
}

void gl_coord3D_set_up_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_get_down_vector(gl_coord3D_t* coord, float* v)
{
    gl_coord3D_get_up_vector(coord, v);
    vec3neg(v, v);
}

void gl_coord3D_set_down_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_get_left_vector(gl_coord3D_t* coord, float* v)
{
    gl_coord3D_get_right_vector(coord, v);
    vec3neg(v, v);
}

void gl_coord3D_set_left_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

void gl_coord3D_get_right_vector(gl_coord3D_t* coord, float* v)
{
    float obj_forward[3]; // get the facing direction
    gl_coord3D_get_normal(coord, obj_forward);

    static const float up[3] = { 0.0f, 1.0f, 0.0f };
    vec3cross(v, obj_forward, (float*)up);
}

void gl_coord3D_set_right_vector(gl_coord3D_t* coord, float* v)
{
    AE_STUB();
}

/*
================================================================================
 * ~~ [ init & quit ] ~~ *
--------------------------------------------------------------------------------
*/

const char* gl_implementation(void)
{
    // TODO: in the future, this could detect system capabilities, allowing this
    // string to be checked against in a driver load phase during initialization.
    // the driver would issue GPU calls and some common code could be left here.
    return "SDL2_GL1";
}

void gl_utility_init(void)
{
    //
}

void gl_utility_quit(void)
{
    // TODO: log only if we have a context (or more easily, if no GL calls made).
    // gl_log_internal_state();
}
