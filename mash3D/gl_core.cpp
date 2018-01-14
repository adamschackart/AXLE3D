/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
--- NOTE: all internal classes must remain POD (no virtual method definitions!)
--------------------------------------------------------------------------------
--- TODO: GL_(DEBUG)_LOG_ERROR_STATE_SCOPE; check gl error status at scope exit
--- TODO: should we disable depth masking before drawing emitter/buffer normals?
--- TODO: debug setting for drawing all buffer & emitter aabboxes, normals, etc.
--- TODO: should we disable fog before enter2D? does lighting off disable fog?
--- TODO: allow objects to be copyable and serializable (especially materials!)
--- TODO: when saving a material, store texture path with the base path removed
--- TODO: declare library functions as pointers through a GL_FUNCTIONS_N macro
--- TODO: state tracking; don't rebind the same material or texture redundantly
--- TODO: values aren't clamped when setting properties for lights or materials
--- TODO: lights and particle emitters should probably have path str properties
--- TODO: test buffer against ellipsoid/line - check aabbox, then convex polys
--- TODO: "burn barrel" particle effect - sphere clamp x, aabbox clamp bottom y
--- TODO: function for checking if an extension is supported (internal strset?)
--------------------------------------------------------------------------------

--- TODO: put all internal object structures into a union and block-allocate it.
- - - - - object allocation will have to check that memory blocks it gets back
 - - - -  actually reside in the global internal object array, as the open check
- - - - - must test for array residence with ptr_in_array before dereferencing.

            typedef struct gl_internal_object_t
            {
                gl_object_type_t type; // UNKNOWN indicates object is closed.

                // closed objects will also have NULL next and previous links.
                gl_internal_object_t* next;
                gl_internal_object_t* prev;

                union
                {
                    #define N(cap, low) struct gl_internal_ ## low ## _t low;
                    GL_OBJECT_TYPE_N
                    #undef N
                };
            } \
                gl_internal_object_t;

            // links not required, but it speeds up object iteration, as the
            // alternative is traversing the object array and type checking.
            // this also eliminates the need to sort in object list calls, as
            // all objects are already listed in the order they were created.

            static gl_internal_object_t *head, *tail;

----------------------------------------------------------------------------- */
#ifndef __GL_CORE_H__
#include <gl_core.h>
#endif

// This is just to detect leaks. Feel free to bump if you legitimately hit this.
#define GL_OBJECT_COUNT_LIMIT 1024

/* Global sets for each object type, so we can check objects for liveness, plus a
 * bunch of other neat stuff. Renamed due to gl_particle_emitter_set being taken.
 */
#define N(cap, low) static ae_ptrset_t gl_ ## low ## _lst;
GL_OBJECT_TYPE_N
#undef N

#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>

// CPP headers aren't included in ae_core.h due to long template compile times.
#include <new>

/*
================================================================================
 * ~~ [ object types ] ~~ *
--------------------------------------------------------------------------------
*/

gl_object_type_t gl_object_type(void* object)
{
    #define N(cap, low)                                             \
                                                                    \
        ae_if (gl_ ## low ## _get_open((gl_ ## low ## _t *)object)) \
        {                                                           \
            return GL_OBJECT_TYPE_ ## cap;                          \
        }                                                           \

    GL_OBJECT_TYPE_N
    #undef N

    return GL_OBJECT_TYPE_UNKNOWN;
}

size_t gl_object_count_all(void)
{
    return( // accumulate
            #define N(cap, low) gl_ ## low ## _count_all() +

            GL_OBJECT_TYPE_N
            #undef N
        0);
}

void gl_object_list_all(void** objects)
{
    #define N(cap, low)                                         \
                                                                \
        gl_ ## low ## _list_all((gl_ ## low ## _t **)objects);  \
        objects += gl_ ## low ## _count_all();                  \

    GL_OBJECT_TYPE_N
    #undef N
}

void gl_object_close_all(void)
{
    #define N(cap, low) gl_ ## low ## _close_all();
    GL_OBJECT_TYPE_N
    #undef N
}

#define N(cap, low)                                             \
                                                                \
    static int gl_ ## low ## _compare(const void*,const void*); \
                                                                \
    void gl_ ## low ## _list_all(gl_ ## low ## _t ** pointers)  \
    {                                                           \
        ae_ptrset_list(&gl_ ## low ## _lst, (void **)pointers); \
                                                                \
        qsort(pointers, gl_ ## low ## _count_all(),             \
            sizeof(gl_ ## low ## _t*), gl_ ## low ## _compare); \
    }                                                           \

GL_OBJECT_TYPE_N
#undef N

#define N(cap, low)                                                         \
                                                                            \
    void gl_ ## low ## _close_all(void)                                     \
    {                                                                       \
        aeTempArray<gl_ ## low ## _t*> objects(gl_ ## low ## _count_all()); \
        gl_ ## low ## _list_all(objects.data());                            \
                                                                            \
        for (size_t i = 0, n = gl_ ## low ## _count_all(); i < n; i++)      \
        {                                                                   \
            gl_ ## low ## _close(objects[i]);                               \
        }                                                                   \
    }                                                                       \

GL_OBJECT_TYPE_N
#undef N

/*
================================================================================
 * ~~ [ OpenGL wrapper ] ~~ *
--------------------------------------------------------------------------------
TODO: use a script like gl3w to automatically generate this from OpenGL headers
TODO: if we can't find functions, try appending EXT and then ARB to their names
--------------------------------------------------------------------------------
*/

static void* gl_proc(void* library, const char* name)
{
    void* res = NULL; // TODO: this code lacks logging and non-crappiness!

    #if defined(_WIN32)
        res = (void*)wglGetProcAddress(name);

        if (res == NULL)
        {
            res = (void*)GetProcAddress((HMODULE)library, (LPCSTR)name);
        }
    #elif defined(__APPLE__)
        res = (void*)dlsym(library, name);
    #else
        typedef void (*p_func)(void); // TODO: include header defining this
        typedef p_func (*PFNGLXGETPROCADDRESSPROC)(const GLubyte *procName);

        static PFNGLXGETPROCADDRESSPROC glx_get_proc_address;

        if (glx_get_proc_address == NULL)
        {
            glx_get_proc_address = (PFNGLXGETPROCADDRESSPROC) \
                        dlsym(library, "glXGetProcAddressARB");

            assert(glx_get_proc_address);
        }

        res = (void*)glx_get_proc_address((const GLubyte*)name);

        if (res == NULL)
        {
            res = (void*)dlsym(library, name);
        }
    #endif
    return res;
}

static void* gl_func(const char* name)
{
    static void* lib_gl = NULL; // FIXME: leaked! this code sucks and was written in haste!

    #if defined(_WIN32)
        if (lib_gl == NULL)
        {
            lib_gl = ae_library_open("opengl32.dll");
        }
    #elif defined(__APPLE__)
        if (lib_gl == NULL)
        {
            lib_gl = ae_library_open("/System/Library/Frameworks/OpenGL.framework/OpenGL");
        }
    #else
        if (lib_gl == NULL)
        {
            lib_gl = ae_library_open("libGL.so");
        }
    #endif

    return gl_proc(lib_gl, name);
}

#define X() AE_PROFILE_SCOPE()

// TODO: GL_Accum

// TODO: GL_ActiveShaderProgram

void GL_ActiveTexture(unsigned int texture)
{
    /* TODO: allow this to be reset to NULL on gl_quit */
    static PFNGLACTIVETEXTUREPROC exActiveTexture = NULL;

    X(); if (exActiveTexture)
    {
        exActiveTexture(texture);
    }
    else
    {
        exActiveTexture = (PFNGLACTIVETEXTUREPROC)
                        gl_func("glActiveTexture");

        if (exActiveTexture)
        {
            exActiveTexture(texture);
        }
        else
        {
            AE_WARN("glActiveTexture not found");
        }
    }
}

void GL_AlphaFunc(unsigned int func, float ref)
{
    X(); glAlphaFunc(func, ref);
}

// TODO: GL_AreTexturesResident

// TODO: GL_ArrayElement

// TODO: GL_AttachShader

void GL_Begin(unsigned int mode)
{
    X(); glBegin(mode);
}

// TODO: GL_BeginConditionalRender

// TODO: GL_BeginQuery

// TODO: GL_BeginQueryIndexed

// TODO: GL_BeginTransformFeedback

// TODO: GL_BindAttribLocation

// TODO: GL_BindBuffer

// TODO: GL_BindBufferBase

// TODO: GL_BindBufferRange

// TODO: GL_BindBuffersBase

// TODO: GL_BindBuffersRange

// TODO: GL_BindFragDataLocation

// TODO: GL_BindFragDataLocationIndexed

// TODO: GL_BindFramebuffer

// TODO: GL_BindImageTexture

// TODO: GL_BindImageTextures

// TODO: GL_BindProgramPipeline

// TODO: GL_BindRenderbuffer

// TODO: GL_BindSampler

// TODO: GL_BindSamplers

void GL_BindTexture(unsigned int target, unsigned int texture)
{
    X(); glBindTexture(target, texture);
}

// TODO: GL_BindTextures

// TODO: GL_BindTransformFeedback

// TODO: GL_BindVertexArray

// TODO: GL_BindVertexBuffer

// TODO: GL_BindVertexBuffers

// TODO: GL_Bitmap

// TODO: GL_BlendColor

void GL_BlendEquation(unsigned int mode)
{
    /* TODO: allow this to be reset to NULL on gl_quit */
    static PFNGLBLENDEQUATIONPROC exBlendEquation = NULL;

    X(); if (exBlendEquation)
    {
        exBlendEquation(mode);
    }
    else
    {
        exBlendEquation = (PFNGLBLENDEQUATIONPROC)
                        gl_func("glBlendEquation");

        if (exBlendEquation)
        {
            exBlendEquation(mode);
        }
        else
        {
            AE_WARN("glBlendEquation not found");
        }
    }
}

void GL_BlendEquationSeparate(unsigned int modeRGB, unsigned int modeA)
{
    /* TODO: allow this ptr to be reset to NULL on gl_quit for safety */
    static PFNGLBLENDEQUATIONSEPARATEPROC exBlendEquationSeparate = NULL;

    X(); if (exBlendEquationSeparate)
    {
        exBlendEquationSeparate(modeRGB, modeA);
    }
    else
    {
        exBlendEquationSeparate = (PFNGLBLENDEQUATIONSEPARATEPROC)
                                gl_func("glBlendEquationSeparate");

        if (exBlendEquationSeparate)
        {
            exBlendEquationSeparate(modeRGB, modeA);
        }
        else
        {
            AE_WARN("glBlendEquationSeparate not found");
        }
    }
}

// TODO: GL_BlendEquationi

// TODO: GL_BlendEquationSeparatei

void GL_BlendFunc(unsigned int src, unsigned int dst)
{
    X(); glBlendFunc(src, dst);
}

void GL_BlendFuncSeparate(unsigned int srcRGB, unsigned int dstRGB,
                            unsigned int srcA, unsigned int dstA)
{
    /* TODO: allow this to be NULLed out on gl_quit for safety */
    static PFNGLBLENDFUNCSEPARATEPROC exBlendFuncSeparate = NULL;

    X(); if (exBlendFuncSeparate)
    {
        exBlendFuncSeparate(srcRGB, dstRGB, srcA, dstA);
    }
    else
    {
        exBlendFuncSeparate = (PFNGLBLENDFUNCSEPARATEPROC)
                            gl_func("glBlendFuncSeparate");

        if (exBlendFuncSeparate)
        {
            exBlendFuncSeparate(srcRGB, dstRGB, srcA, dstA);
        }
        else
        {
            AE_WARN("glBlendFuncSeparate not found");
        }
    }
}

// TODO: GL_BlendFunci

// TODO: GL_BlendFuncSeparatei

// TODO: GL_BlitFramebuffer

// TODO: GL_BufferData

// TODO: GL_BufferStorage

// TODO: GL_BufferSubData

// TODO: GL_CallList

// TODO: GL_CallLists

// TODO: GL_CheckFramebufferStatus

// TODO: GL_ClampColor

void GL_Clear(unsigned int mask)
{
    X(); glClear(mask);
}

// TODO: GL_ClearBufferData

// TODO: GL_ClearBufferSubData

// TODO: GL_ClearBufferfi

// TODO: GL_ClearBufferfv

// TODO: GL_ClearBufferiv

// TODO: GL_ClearBufferuiv

// TODO: GL_ClearDepthf

// TODO: GL_ClearTexImage

// TODO: GL_ClearTexSubImage

// TODO: GL_ClientActiveTexture

// TODO: GL_ClientWaitSync

// TODO: GL_ClipPlane

// TODO: GL_ClearAccum

void GL_ClearColor(float r, float g, float b, float a)
{
    X(); glClearColor(r, g, b, a);
}

// TODO: GL_ClearDepth

// TODO: GL_ClearIndex

void GL_Color3f(float r, float g, float b)
{
    X(); glColor3f(r, g, b);
}

void GL_Color3fv(const float* rgb)
{
    X(); glColor3fv(rgb);
}

// TODO: GL_Color3ub

// TODO: GL_Color3ubv

void GL_Color4f(float r, float g, float b, float a)
{
    X(); glColor4f(r, g, b, a);
}

void GL_Color4fv(const float* rgba)
{
    X(); glColor4fv(rgba);
}

// TODO: GL_Color4ub

// TODO: GL_Color4ubv

void GL_ColorMask(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    X(); glColorMask(r, g, b, a);
}

// TODO: GL_ColorMaski

void GL_ColorMaterial(unsigned int face, unsigned int mode)
{
    X(); glColorMaterial(face, mode);
}

// TODO: GL_ColorP3ui

// TODO: GL_ColorP3uiv

// TODO: GL_ColorP4ui

// TODO: GL_ColorP4uiv

void GL_ColorPointer(int size, unsigned int type, int stride, const void* data)
{
    X(); glColorPointer(size, type, stride, data);
}

// TODO: GL_ColorSubTable

// TODO: GL_ColorTable

// TODO: GL_ColorTableParameteriv

// TODO: GL_ColorTableParameterfv

// TODO: GL_CompileShader

// TODO: GL_CompressedTexImage1D

// TODO: GL_CompressedTexImage2D

// TODO: GL_CompressedTexImage3D

// TODO: GL_CompressedTexSubImage1D

// TODO: GL_CompressedTexSubImage2D

// TODO: GL_CompressedTexSubImage3D

// TODO: GL_ConvolutionFilter1D

// TODO: GL_ConvolutionFilter2D

// TODO: GL_ConvolutionParameterf

// TODO: GL_ConvolutionParameterfv

// TODO: GL_ConvolutionParameteri

// TODO: GL_ConvolutionParameteriv

// TODO: GL_CopyBufferSubData

// TODO: GL_CopyColorSubTable

// TODO: GL_CopyColorTable

// TODO: GL_CopyConvolutionFilter1D

// TODO: GL_CopyConvolutionFilter2D

// TODO: GL_CopyImageSubData

// TODO: GL_CopyPixels

// TODO: GL_CopyTexImage1D

// TODO: GL_CopyTexImage2D

// TODO: GL_CopyTexImage3D

// TODO: GL_CopyTexSubImage1D

// TODO: GL_CopyTexSubImage2D

// TODO: GL_CopyTexSubImage3D

// TODO: GL_CreateProgram

// TODO: GL_CreateShader

// TODO: GL_CreateShaderProgramv

void GL_CullFace(unsigned int face)
{
    X(); glCullFace(face);
}

// TODO: GL_DebugMessageCallback

// TODO: GL_DebugMessageControl

// TODO: GL_DebugMessageInsert

// TODO: GL_DeleteBuffers

// TODO: GL_DeleteFramebuffers

// TODO: GL_DeleteLists

// TODO: GL_DeleteProgram

// TODO: GL_DeleteProgramPipelines

// TODO: GL_DeleteQueries

// TODO: GL_DeleteRenderbuffers

// TODO: GL_DeleteSamplers

// TODO: GL_DeleteShader

// TODO: GL_DeleteSync

void GL_DeleteTextures(int n, const unsigned int* textures)
{
    X(); glDeleteTextures(n, textures);
}

// TODO: GL_DeleteTransformFeedbacks

// TODO: GL_DeleteVertexArrays

void GL_DepthFunc(unsigned int func)
{
    X(); glDepthFunc(func);
}

void GL_DepthMask(unsigned char flag)
{
    X(); glDepthMask(flag);
}

void GL_DepthRange(double near_val, double far_val)
{
    X(); glDepthRange(near_val, far_val);
}

// TODO: GL_DepthRangeArrayv

// TODO: GL_DepthRangeIndexed

// TODO: GL_DepthRangef

// TODO: GL_DetachShader

void GL_Disable(unsigned int cap)
{
    X(); glDisable(cap);
}

// TODO: GL_Disablei

// TODO: GL_DisableVertexAttribArray

void GL_DisableClientState(unsigned int cap)
{
    X(); glDisableClientState(cap);
}

// TODO: GL_DispatchCompute

// TODO: GL_DispatchComputeIndirect

void GL_DrawArrays(unsigned int mode, int first, int count)
{
    X(); glDrawArrays(mode, first, count);
}

// TODO: GL_DrawArraysIndirect

// TODO: GL_DrawArraysInstanced

// TODO: GL_DrawArraysInstancedBaseInstance

// TODO: GL_DrawBuffer

// TODO: GL_DrawBuffers

void GL_DrawElements(unsigned int mode, int n, unsigned int type, const void* inds)
{
    X(); glDrawElements(mode, n, type, inds);
}

// TODO: GL_DrawElementsBaseVertex

// TODO: GL_DrawElementsIndirect

// TODO: GL_DrawElementsInstanced

// TODO: GL_DrawElementsInstancedBaseInstance

// TODO: GL_DrawElementsInstancedBaseVertex

// TODO: GL_DrawElementsInstancedBaseVertexBaseInstance

// TODO: GL_DrawPixels

// TODO: GL_DrawRangeElements

// TODO: GL_DrawRangeElementsBaseVertex

// TODO: GL_DrawTransformFeedback

// TODO: GL_DrawTransformFeedbackInstanced

// TODO: GL_DrawTransformFeedbackStream

// TODO: GL_DrawTransformFeedbackStreamInstanced

// TODO: GL_EdgeFlagv

// TODO: GL_EdgeFlag

// TODO: GL_EdgeFlagPointer

void GL_Enable(unsigned int cap)
{
    X(); glEnable(cap);
}

// TODO: GL_Enablei

void GL_EnableClientState(unsigned int cap)
{
    X(); glEnableClientState(cap);
}

// TODO: GL_EnableVertexAttribArray

void GL_End(void)
{
    X(); glEnd();
}

// TODO: GL_EndConditionalRender

// TODO: GL_EndList

// TODO: GL_EndQuery

// TODO: GL_EndQueryIndexed

// TODO: GL_EndTransformFeedback

// TODO: GL_FeedbackBuffer

// TODO: GL_FenceSync

void GL_Finish(void)
{
    X(); glFinish();
}

void GL_Flush(void)
{
    X(); glFlush();
}

// TODO: GL_FlushMappedBufferRange

// TODO: GL_FogCoordd

// TODO: GL_FogCoorddv

// TODO: GL_FogCoordf

// TODO: GL_FogCoordfv

// TODO: GL_FogCoordPointer

void GL_Fogfv(unsigned int pname, const float* params)
{
    X(); glFogfv(pname, params);
}

void GL_Fogiv(unsigned int pname, const int* params)
{
    X(); glFogiv(pname, params);
}

void GL_Fogf(unsigned int pname, float param)
{
    X(); glFogf(pname, param);
}

void GL_Fogi(unsigned int pname, int param)
{
    X(); glFogi(pname, param);
}

// TODO: GL_FramebufferParameteri

// TODO: GL_FramebufferRenderbuffer

// TODO: GL_FramebufferTexture

// TODO: GL_FramebufferTexture1D

// TODO: GL_FramebufferTexture2D

// TODO: GL_FramebufferTexture3D

// TODO: GL_FramebufferTextureLayer

void GL_FrontFace(unsigned int mode)
{
    X(); glFrontFace(mode);
}

void GL_Frustum(double lf, double rt, double bt, double tp, double nr, double fr)
{
    X(); glFrustum(lf, rt, bt, tp, nr, fr);
}

// TODO: GL_GenBuffers

// TODO: GL_GenFramebuffers

// TODO: GL_GenLists

// TODO: GL_GenProgramPipelines

// TODO: GL_GenQueries

// TODO: GL_GenRenderbuffers

// TODO: GL_GenSamplers

void GL_GenTextures(int n, unsigned int* textures)
{
    X(); glGenTextures(n, textures);
}

// TODO: GL_GenTransformFeedbacks

// TODO: GL_GenVertexArrays

// TODO: GL_GenerateMipmap

// TODO: GL_GetActiveAtomicCounterBufferiv

// TODO: GL_GetActiveAttrib

// TODO: GL_GetActiveSubroutineName

// TODO: GL_GetActiveSubroutineUniformName

// TODO: GL_GetActiveSubroutineUniformiv

// TODO: GL_GetActiveUniform

// TODO: GL_GetActiveUniformBlockName

// TODO: GL_GetActiveUniformBlockiv

// TODO: GL_GetActiveUniformName

// TODO: GL_GetActiveUniformsiv

// TODO: GL_GetAttachedShaders

// TODO: GL_GetAttribLocation

// TODO: GL_GetBooleanv

// TODO: GL_GetBooleani_v

// TODO: GL_GetBufferParameteri64v

// TODO: GL_GetBufferParameteriv

// TODO: GL_GetBufferPointerv

// TODO: GL_GetBufferSubData

// TODO: GL_GetClipPlane

// TODO: GL_GetColorTable

// TODO: GL_GetColorTableParameterfv

// TODO: GL_GetColorTableParameteriv

// TODO: GL_GetConvolutionFilter

// TODO: GL_GetConvolutionParameterfv

// TODO: GL_GetConvolutionParameteriv

// TODO: GL_GetDebugMessageLog

unsigned int GL_GetError(void)
{
    X(); return glGetError();
}

// TODO: GL_GetCompressedTexImage

// TODO: GL_GetDoublev

// TODO: GL_GetDoublei_v

void GL_GetFloatv(unsigned int pname, float* data)
{
    X(); glGetFloatv(pname, data);
}

// TODO: GL_GetFloati_v

// TODO: GL_GetFragDataIndex

// TODO: GL_GetFragDataLocation

// TODO: GL_GetFramebufferAttachmentParameteriv

// TODO: GL_GetFramebufferParameteriv

// TODO: GL_GetHistogram

// TODO: GL_GetHistogramParameterfv

// TODO: GL_GetHistogramParameteriv

void GL_GetIntegerv(unsigned int pname, int* data)
{
    X(); glGetIntegerv(pname, data);
}

// TODO: GL_GetIntegeri_v

// TODO: GL_GetInteger64v

// TODO: GL_GetInteger64i_v

// TODO: GL_GetInternalformativ

// TODO: GL_GetInternalformati64v

void GL_GetLightfv(unsigned int light, unsigned int pname, float* params)
{
    X(); glGetLightfv(light, pname, params);
}

// TODO: GL_GetLightiv

void GL_GetMaterialfv(unsigned int face, unsigned int pname, float* params)
{
    X(); glGetMaterialfv(face, pname, params);
}

// TODO: GL_GetMaterialiv

// TODO: GL_GetMinmax

// TODO: GL_GetMinmaxParameterfv

// TODO: GL_GetMinmaxParameteriv

// TODO: GL_GetMultisamplefv

// TODO: GL_GetObjectLabel

// TODO: GL_GetObjectPtrLabel

// TODO: GL_GetPixelMapfv

// TODO: GL_GetPixelMapuiv

// TODO: GL_GetPixelMapusv

// TODO: GL_GetPixelMapxv

// TODO: GL_GetPointerv

// TODO: GL_GetPolygonStipple

// TODO: GL_GetProgramBinary

// TODO: GL_GetProgramInfoLog

// TODO: GL_GetProgramInterfaceiv

// TODO: GL_GetProgramPipelineInfoLog

// TODO: GL_GetProgramPipelineiv

// TODO: GL_GetProgramResourceIndex

// TODO: GL_GetProgramResourceLocation

// TODO: GL_GetProgramResourceLocationIndex

// TODO: GL_GetProgramResourceName

// TODO: GL_GetProgramResourceiv

// TODO: GL_GetProgramStageiv

// TODO: GL_GetProgramiv

// TODO: GL_GetQueryIndexediv

// TODO: GL_GetQueryObjecti64v

// TODO: GL_GetQueryObjectiv

// TODO: GL_GetQueryObjectui64v

// TODO: GL_GetQueryObjectuiv

// TODO: GL_GetQueryiv

// TODO: GL_GetRenderbufferParameteriv

// TODO: GL_GetSamplerParameterIiv

// TODO: GL_GetSamplerParameterIuiv

// TODO: GL_GetSamplerParameterfv

// TODO: GL_GetSamplerParameteriv

// TODO: GL_GetSeparableFilter

// TODO: GL_GetShaderInfoLog

// TODO: GL_GetShaderPrecisionFormat

// TODO: GL_GetShaderSource

// TODO: GL_GetShaderiv

const char* GL_GetString(unsigned int name)
{
    X(); return (const char*)glGetString(name);
}

// TODO: GL_GetStringi

// TODO: GL_GetSubroutineIndex

// TODO: GL_GetSubroutineUniformLocation

// TODO: GL_GetSynciv

void GL_GetTexEnvfv(unsigned int target, unsigned int pname, float* params)
{
    X(); glGetTexEnvfv(target, pname, params);
}

void GL_GetTexEnviv(unsigned int target, unsigned int pname, int* params)
{
    X(); glGetTexEnviv(target, pname, params);
}

void GL_GetTexGendv(unsigned int coord, unsigned int pname, double* params)
{
    X(); glGetTexGendv(coord, pname, params);
}

void GL_GetTexGenfv(unsigned int coord, unsigned int pname, float* params)
{
    X(); glGetTexGenfv(coord, pname, params);
}

void GL_GetTexGeniv(unsigned int coord, unsigned int pname, int* params)
{
    X(); glGetTexGeniv(coord, pname, params);
}

// TODO: GL_GetTexImage

// TODO: GL_GetTexLevelParameterfv

// TODO: GL_GetTexLevelParameteriv

void GL_GetTexParameterfv(unsigned int target, unsigned int pname, float* params)
{
    X(); glGetTexParameterfv(target, pname, params);
}

void GL_GetTexParameteriv(unsigned int target, unsigned int pname, int* params)
{
    X(); glGetTexParameteriv(target, pname, params);
}

// TODO: GL_GetTexParameterIiv

// TODO: GL_GetTexParameterIuiv

// TODO: GL_GetTransformFeedbackVarying

// TODO: GL_GetUniformBlockIndex

// TODO: GL_GetUniformIndices

// TODO: GL_GetUniformLocation

// TODO: GL_GetUniformSubroutineuiv

// TODO: GL_GetUniformdv

// TODO: GL_GetUniformfv

// TODO: GL_GetUniformiv

// TODO: GL_GetUniformuiv

// TODO: GL_GetVertexAttribIiv

// TODO: GL_GetVertexAttribIuiv

// TODO: GL_GetVertexAttribLdv

// TODO: GL_GetVertexAttribPointerv

// TODO: GL_GetVertexAttribdv

// TODO: GL_GetVertexAttribfv

// TODO: GL_GetVertexAttribiv

// TODO: GL_Hint

// TODO: GL_Histogram

// TODO: GL_IndexMask

// TODO: GL_IndexPointer

// TODO: GL_InitNames

void GL_InterleavedArrays(unsigned int format, int stride, const void* pointer)
{
    X(); glInterleavedArrays(format, stride, pointer);
}

// TODO: GL_InvalidateBufferData

// TODO: GL_InvalidateBufferSubData

// TODO: GL_InvalidateFramebuffer

// TODO: GL_InvalidateSubFramebuffer

// TODO: GL_InvalidateTexImage

// TODO: GL_InvalidateTexSubImage

// TODO: GL_IsBuffer

unsigned char GL_IsEnabled(unsigned int cap)
{
    X(); return glIsEnabled(cap);
}

// TODO: GL_IsEnabledi

// TODO: GL_IsFramebuffer

// TODO: GL_IsList

// TODO: GL_IsProgram

// TODO: GL_IsProgramPipeline

// TODO: GL_IsQuery

// TODO: GL_IsRenderbuffer

// TODO: GL_IsSampler

// TODO: GL_IsShader

// TODO: GL_IsSync

// TODO: GL_IsTransformFeedback

unsigned char GL_IsTexture(unsigned int texture)
{
    X(); return glIsTexture(texture);
}

// TODO: GL_IsVertexArray

void GL_Lightfv(unsigned int light, unsigned int pname, const float* params)
{
    X(); glLightfv(light, pname, params);
}

void GL_Lightf(unsigned int light, unsigned int pname, float param)
{
    X(); glLightf(light, pname, param);
}

// TODO: GL_Lightiv

// TODO: GL_Lighti

// TODO: GL_LightModelfv

// TODO: GL_LightModelf

// TODO: GL_LightModeliv

// TODO: GL_LightModeli

// TODO: GL_LineStipple

void GL_LineWidth(float width)
{
    X(); glLineWidth(width);
}

// TODO: GL_LinkProgram

// TODO: GL_ListBase

void GL_LoadIdentity(void)
{
    X(); glLoadIdentity();
}

// TODO: GL_LoadMatrixd

// TODO: GL_LoadMatrixf

// TODO: GL_LoadName

// TODO: GL_LoadTransposeMatrixd

// TODO: GL_LoadTransposeMatrixf

void GL_LogicOp(unsigned int opcode)
{
    X(); glLogicOp(opcode);
}

// TODO: GL_MapBuffer

// TODO: GL_MapBufferRange

void GL_Materialfv(unsigned int face, unsigned int pname, const float* params)
{
    X(); glMaterialfv(face, pname, params);
}

void GL_Materialf(unsigned int face, unsigned int pname, float param)
{
    X(); glMaterialf(face, pname, param);
}

// TODO: GL_Materialiv

// TODO: GL_Materiali

void GL_MatrixMode(unsigned int mode)
{
    X(); glMatrixMode(mode);
}

// TODO: GL_MemoryBarrier

// TODO: GL_Minmax

// TODO: GL_MinSampleShading

// TODO: GL_MultiDrawArrays

// TODO: GL_MultiDrawArraysIndirect

// TODO: GL_MultiDrawElements

// TODO: GL_MultiDrawElementsBaseVertex

// TODO: GL_MultiDrawElementsIndirect

// TODO: GL_MultiTexCoord1d

// TODO: GL_MultiTexCoord1dv

// TODO: GL_MultiTexCoord1f

// TODO: GL_MultiTexCoord1fv

// TODO: GL_MultiTexCoord1i

// TODO: GL_MultiTexCoord1iv

// TODO: GL_MultiTexCoord1s

// TODO: GL_MultiTexCoord1sv

// TODO: GL_MultiTexCoord2d

// TODO: GL_MultiTexCoord2dv

// TODO: GL_MultiTexCoord2f

// TODO: GL_MultiTexCoord2fv

// TODO: GL_MultiTexCoord2i

// TODO: GL_MultiTexCoord2iv

// TODO: GL_MultiTexCoord2s

// TODO: GL_MultiTexCoord2sv

// TODO: GL_MultiTexCoord3d

// TODO: GL_MultiTexCoord3dv

// TODO: GL_MultiTexCoord3f

// TODO: GL_MultiTexCoord3fv

// TODO: GL_MultiTexCoord3i

// TODO: GL_MultiTexCoord3iv

// TODO: GL_MultiTexCoord3s

// TODO: GL_MultiTexCoord3sv

// TODO: GL_MultiTexCoord4d

// TODO: GL_MultiTexCoord4dv

// TODO: GL_MultiTexCoord4f

// TODO: GL_MultiTexCoord4fv

// TODO: GL_MultiTexCoord4i

// TODO: GL_MultiTexCoord4iv

// TODO: GL_MultiTexCoord4s

// TODO: GL_MultiTexCoord4sv

// TODO: GL_MultiTexCoordP1ui

// TODO: GL_MultiTexCoordP1uiv

// TODO: GL_MultiTexCoordP2ui

// TODO: GL_MultiTexCoordP2uiv

// TODO: GL_MultiTexCoordP3ui

// TODO: GL_MultiTexCoordP3uiv

// TODO: GL_MultiTexCoordP4ui

// TODO: GL_MultiTexCoordP4uiv

// TODO: GL_MultMatrixd

void GL_MultMatrixf(const float* matrix)
{
    X(); glMultMatrixf(matrix);
}

// TODO: GL_MultTransposeMatrixd

// TODO: GL_MultTransposeMatrixf

// TODO: GL_NewList

void GL_Normal3f(float x, float y, float z)
{
    X(); glNormal3f(x, y, z);
}

void GL_Normal3fv(const float* normal)
{
    X(); glNormal3fv(normal);
}

// TODO: GL_NormalP3ui

// TODO: GL_NormalP3uiv

void GL_NormalPointer(unsigned int type, int stride, const void* data)
{
    X(); glNormalPointer(type, stride, data);
}

// TODO: GL_ObjectLabel

// TODO: GL_ObjectPtrLabel

void GL_Ortho(  double left, double right, double bottom,
                double top, double near_v, double far_v )
{
    X(); glOrtho(left, right, bottom, top, near_v, far_v);
}

// TODO: GL_PassThrough

// TODO: GL_PatchParameterfv

// TODO: GL_PatchParameteri

// TODO: GL_PauseTransformFeedback

// TODO: GL_PixelMapfv

// TODO: GL_PixelMapuiv

// TODO: GL_PixelMapusv

// TODO: GL_PixelMapx

// TODO: GL_PixelStoref

// TODO: GL_PixelStorei

// TODO: GL_PixelStorex

// TODO: GL_PixelTransferf

// TODO: GL_PixelTransferi

// TODO: GL_PixelZoom

void GL_PointParameterfv(unsigned int pname, const float* data)
{
    // TODO: allow this to be NULLed out on gl_quit for safety
    static PFNGLPOINTPARAMETERFVPROC exPointParameterfv = NULL;

    X(); if (exPointParameterfv)
    {
        exPointParameterfv(pname, data);
    }
    else
    {
        exPointParameterfv = (PFNGLPOINTPARAMETERFVPROC) \
                            gl_func("glPointParameterfv");

        if (exPointParameterfv)
        {
            exPointParameterfv(pname, data);
        }
        else
        {
            AE_WARN("glPointParameterfv not found");
        }
    }
}

void GL_PointParameterf(unsigned int pname, float param)
{
    // TODO: allow this to be NULLed out on gl_quit for safety
    static PFNGLPOINTPARAMETERFPROC exPointParameterf = NULL;

    X(); if (exPointParameterf)
    {
        exPointParameterf(pname, param);
    }
    else
    {
        exPointParameterf = (PFNGLPOINTPARAMETERFPROC) \
                            gl_func("glPointParameterf");

        if (exPointParameterf)
        {
            exPointParameterf(pname, param);
        }
        else
        {
            AE_WARN("glPointParameterf not found");
        }
    }
}

// TODO: GL_PointParameteriv

// TODO: GL_PointParameteri

void GL_PointSize(float size)
{
    X(); glPointSize(size);
}

void GL_PolygonMode(unsigned int face, unsigned int mode)
{
    X(); glPolygonMode(face, mode);
}

void GL_PolygonOffset(float factor, float units)
{
    X(); glPolygonOffset(factor, units);
}

// TODO: GL_PolygonStipple

void GL_PopAttrib(void)
{
    X(); glPopAttrib();
}

void GL_PushAttrib(unsigned int mask)
{
    X(); glPushAttrib(mask);
}

void GL_PopClientAttrib(void)
{
    X(); glPopClientAttrib();
}

void GL_PushClientAttrib(unsigned int mask)
{
    X(); glPushClientAttrib(mask);
}

// TODO: GL_PopDebugGroup

// TODO: GL_PushDebugGroup

void GL_PopMatrix(void)
{
    X(); glPopMatrix();
}

void GL_PushMatrix(void)
{
    X(); glPushMatrix();
}

// TODO: GL_PopName

// TODO: GL_PushName

// TODO: GL_PrimitiveRestartIndex

// TODO: GL_PrioritizeTextures

// TODO: GL_ProgramBinary

// TODO: GL_ProgramParameteri

// TODO: GL_ProgramUniform1d

// TODO: GL_ProgramUniform1dv

// TODO: GL_ProgramUniform1f

// TODO: GL_ProgramUniform1fv

// TODO: GL_ProgramUniform1i

// TODO: GL_ProgramUniform1iv

// TODO: GL_ProgramUniform1ui

// TODO: GL_ProgramUniform1uiv

// TODO: GL_ProgramUniform2d

// TODO: GL_ProgramUniform2dv

// TODO: GL_ProgramUniform2f

// TODO: GL_ProgramUniform2fv

// TODO: GL_ProgramUniform2i

// TODO: GL_ProgramUniform2iv

// TODO: GL_ProgramUniform2ui

// TODO: GL_ProgramUniform2uiv

// TODO: GL_ProgramUniform3d

// TODO: GL_ProgramUniform3dv

// TODO: GL_ProgramUniform3f

// TODO: GL_ProgramUniform3fv

// TODO: GL_ProgramUniform3i

// TODO: GL_ProgramUniform3iv

// TODO: GL_ProgramUniform3ui

// TODO: GL_ProgramUniform3uiv

// TODO: GL_ProgramUniform4d

// TODO: GL_ProgramUniform4dv

// TODO: GL_ProgramUniform4f

// TODO: GL_ProgramUniform4fv

// TODO: GL_ProgramUniform4i

// TODO: GL_ProgramUniform4iv

// TODO: GL_ProgramUniform4ui

// TODO: GL_ProgramUniform4uiv

// TODO: GL_ProgramUniformMatrix2dv

// TODO: GL_ProgramUniformMatrix2fv

// TODO: GL_ProgramUniformMatrix2x3dv

// TODO: GL_ProgramUniformMatrix2x3fv

// TODO: GL_ProgramUniformMatrix2x4dv

// TODO: GL_ProgramUniformMatrix2x4fv

// TODO: GL_ProgramUniformMatrix3dv

// TODO: GL_ProgramUniformMatrix3fv

// TODO: GL_ProgramUniformMatrix3x2dv

// TODO: GL_ProgramUniformMatrix3x2fv

// TODO: GL_ProgramUniformMatrix3x4dv

// TODO: GL_ProgramUniformMatrix3x4fv

// TODO: GL_ProgramUniformMatrix4dv

// TODO: GL_ProgramUniformMatrix4fv

// TODO: GL_ProgramUniformMatrix4x2dv

// TODO: GL_ProgramUniformMatrix4x2fv

// TODO: GL_ProgramUniformMatrix4x3dv

// TODO: GL_ProgramUniformMatrix4x3fv

// TODO: GL_ProvokingVertex

// TODO: GL_QueryCounter

// TODO: GL_ReadBuffer

// TODO: GL_ReadPixels

// TODO: GL_Rectf

// TODO: GL_Rectfv

// TODO: GL_ReleaseShaderCompiler

// TODO: GL_RenderbufferStorage

// TODO: GL_RenderbufferStorageMultisample

// TODO: GL_RenderMode

// TODO: GL_ResetHistogram

// TODO: GL_ResetMinmax

// TODO: GL_ResumeTransformFeedback

// TODO: GL_Rotated

void GL_Rotatef(float theta, float x, float y, float z)
{
    X(); glRotatef(theta, x, y, z);
}

// TODO: GL_SampleCoverage

// TODO: GL_SampleMaski

// TODO: GL_SamplerParameterIiv

// TODO: GL_SamplerParameterIuiv

// TODO: GL_SamplerParameterf

// TODO: GL_SamplerParameterfv

// TODO: GL_SamplerParameteri

// TODO: GL_SamplerParameteriv

// TODO: GL_Scaled

void GL_Scalef(float x, float y, float z)
{
    X(); glScalef(x, y, z);
}

void GL_Scissor(int x, int y, int width, int height)
{
    X(); glScissor(x, y, width, height);
}

// TODO: GL_ScissorArrayv

// TODO: GL_ScissorIndexed

// TODO: GL_ScissorIndexedv

// TODO: GL_SecondaryColor3b

// TODO: GL_SecondaryColor3bv

// TODO: GL_SecondaryColor3d

// TODO: GL_SecondaryColor3dv

// TODO: GL_SecondaryColor3f

// TODO: GL_SecondaryColor3fv

// TODO: GL_SecondaryColor3i

// TODO: GL_SecondaryColor3iv

// TODO: GL_SecondaryColor3s

// TODO: GL_SecondaryColor3sv

// TODO: GL_SecondaryColor3ub

// TODO: GL_SecondaryColor3ubv

// TODO: GL_SecondaryColor3ui

// TODO: GL_SecondaryColor3uiv

// TODO: GL_SecondaryColor3us

// TODO: GL_SecondaryColor3usv

// TODO: GL_SecondaryColorP3ui

// TODO: GL_SecondaryColorP3uiv

// TODO: GL_SecondaryColorPointer

// TODO: GL_SelectBuffer

// TODO: GL_SeparableFilter2D

void GL_ShadeModel(unsigned int mode)
{
    X(); glShadeModel(mode);
}

// TODO: GL_ShaderBinary

// TODO: GL_ShaderSource

// TODO: GL_ShaderStorageBlockBinding

// TODO: GL_StencilFunc

// TODO: GL_StencilFuncSeparate

// TODO: GL_StencilMask

// TODO: GL_StencilMaskSeparate

// TODO: GL_StencilOp

// TODO: GL_StencilOpSeparate

// TODO: GL_TexBuffer

// TODO: GL_TexBufferRange

// TODO: GL_TexCoord1f

// TODO: GL_TexCoord1fv

void GL_TexCoord2f(float s, float t)
{
    X(); glTexCoord2f(s, t);
}

void GL_TexCoord2fv(const float* st)
{
    X(); glTexCoord2fv(st);
}

void GL_TexCoord3f(float s, float t, float r)
{
    X(); glTexCoord3f(s, t, r);
}

void GL_TexCoord3fv(const float* str)
{
    X(); glTexCoord3fv(str);
}

void GL_TexCoord4f(float s, float t, float r, float q)
{
    X(); glTexCoord4f(s, t, r, q);
}

void GL_TexCoord4fv(const float* strq)
{
    X(); glTexCoord4fv(strq);
}

// TODO: GL_TexCoordP1ui

// TODO: GL_TexCoordP1uiv

// TODO: GL_TexCoordP2ui

// TODO: GL_TexCoordP2uiv

// TODO: GL_TexCoordP3ui

// TODO: GL_TexCoordP3uiv

// TODO: GL_TexCoordP4ui

// TODO: GL_TexCoordP4uiv

void GL_TexCoordPointer(int size, unsigned int type, int stride, const void* data)
{
    X(); glTexCoordPointer(size, type, stride, data);
}

void GL_TexEnvfv(unsigned int target, unsigned int pname, const float *params)
{
    X(); glTexEnvfv(target, pname, params);
}

void GL_TexEnviv(unsigned int target, unsigned int pname, const int *params)
{
    X(); glTexEnviv(target, pname, params);
}

void GL_TexEnvf(unsigned int target, unsigned int pname, float param)
{
    X(); glTexEnvf(target, pname, param);
}

void GL_TexEnvi(unsigned int target, unsigned int pname, int param)
{
    X(); glTexEnvi(target, pname, param);
}

void GL_TexGeniv(unsigned int coord, unsigned int pname, const int *params)
{
    X(); glTexGeniv(coord, pname, params);
}

void GL_TexGenfv(unsigned int coord, unsigned int pname, const float *params)
{
    X(); glTexGenfv(coord, pname, params);
}

void GL_TexGendv(unsigned int coord, unsigned int pname, const double *params)
{
    X(); glTexGendv(coord, pname, params);
}

void GL_TexGeni(unsigned int coord, unsigned int pname, int param)
{
    X(); glTexGeni(coord, pname, param);
}

void GL_TexGenf(unsigned int coord, unsigned int pname, float param)
{
    X(); glTexGenf(coord, pname, param);
}

void GL_TexGend(unsigned int coord, unsigned int pname, double param)
{
    X(); glTexGend(coord, pname, param);
}

// TODO: GL_TexImage1D

void GL_TexImage2D(unsigned int target, int level, int internal_format, int w,
        int h, int border, unsigned int fmt, unsigned int type, const void* p)
{
    X(); glTexImage2D(target, level, internal_format, w, h, border, fmt, type, p);
}

// TODO: GL_TexImage2DMultisample

// TODO: GL_TexImage3D

// TODO: GL_TexImage3DMultisample

// TODO: GL_TexParameterfv

// TODO: GL_TexParameterf

// TODO: GL_TexParameteriv

void GL_TexParameteri(unsigned int target, unsigned int pname, int param)
{
    X(); glTexParameteri(target, pname, param);
}

// TODO: GL_TexParameterIiv

// TODO: GL_TexParameterIuiv

// TODO: GL_TexStorage1D

// TODO: GL_TexStorage2D

// TODO: GL_TexStorage2DMultisample

// TODO: GL_TexStorage3D

// TODO: GL_TexStorage3DMultisample

// TODO: GL_TexSubImage1D

void GL_TexSubImage2D(unsigned int target, int level, int xoffset, int yoffset,
        int w, int h, unsigned int format, unsigned int type, const void* pix)
{
    X(); glTexSubImage2D(target, level, xoffset, yoffset, w, h, format, type, pix);
}

// TODO: GL_TexSubImage3D

// TODO: GL_TextureView

// TODO: GL_TransformFeedbackVaryings

// TODO: GL_Translated

void GL_Translatef(float x, float y, float z)
{
    X(); glTranslatef(x, y, z);
}

// TODO: GL_Uniform1d

// TODO: GL_Uniform1dv

// TODO: GL_Uniform1f

// TODO: GL_Uniform1fv

// TODO: GL_Uniform1i

// TODO: GL_Uniform1iv

// TODO: GL_Uniform1ui

// TODO: GL_Uniform1uiv

// TODO: GL_Uniform2d

// TODO: GL_Uniform2dv

// TODO: GL_Uniform2f

// TODO: GL_Uniform2fv

// TODO: GL_Uniform2i

// TODO: GL_Uniform2iv

// TODO: GL_Uniform2ui

// TODO: GL_Uniform2uiv

// TODO: GL_Uniform3d

// TODO: GL_Uniform3dv

// TODO: GL_Uniform3f

// TODO: GL_Uniform3fv

// TODO: GL_Uniform3i

// TODO: GL_Uniform3iv

// TODO: GL_Uniform3ui

// TODO: GL_Uniform3uiv

// TODO: GL_Uniform4d

// TODO: GL_Uniform4dv

// TODO: GL_Uniform4f

// TODO: GL_Uniform4fv

// TODO: GL_Uniform4i

// TODO: GL_Uniform4iv

// TODO: GL_Uniform4ui

// TODO: GL_Uniform4uiv

// TODO: GL_UniformBlockBinding

// TODO: GL_UniformMatrix2dv

// TODO: GL_UniformMatrix2fv

// TODO: GL_UniformMatrix2x3dv

// TODO: GL_UniformMatrix2x3fv

// TODO: GL_UniformMatrix2x4dv

// TODO: GL_UniformMatrix2x4fv

// TODO: GL_UniformMatrix3dv

// TODO: GL_UniformMatrix3fv

// TODO: GL_UniformMatrix3x2dv

// TODO: GL_UniformMatrix3x2fv

// TODO: GL_UniformMatrix3x4dv

// TODO: GL_UniformMatrix3x4fv

// TODO: GL_UniformMatrix4dv

// TODO: GL_UniformMatrix4fv

// TODO: GL_UniformMatrix4x2dv

// TODO: GL_UniformMatrix4x2fv

// TODO: GL_UniformMatrix4x3dv

// TODO: GL_UniformMatrix4x3fv

// TODO: GL_UniformSubroutinesuiv

// TODO: GL_UnmapBuffer

// TODO: GL_UseProgram

// TODO: GL_UseProgramStages

// TODO: GL_ValidateProgram

// TODO: GL_ValidateProgramPipeline

void GL_Vertex2f(float x, float y)
{
    X(); glVertex2f(x, y);
}

void GL_Vertex2fv(const float* xy)
{
    X(); glVertex2fv(xy);
}

void GL_Vertex3f(float x, float y, float z)
{
    X(); glVertex3f(x, y, z);
}

void GL_Vertex3fv(const float* xyz)
{
    X(); glVertex3fv(xyz);
}

// TODO: GL_Vertex4f

// TODO: GL_Vertex4fv

// TODO: GL_VertexAttrib1d

// TODO: GL_VertexAttrib1dv

// TODO: GL_VertexAttrib1f

// TODO: GL_VertexAttrib1fv

// TODO: GL_VertexAttrib1s

// TODO: GL_VertexAttrib1sv

// TODO: GL_VertexAttrib2d

// TODO: GL_VertexAttrib2dv

// TODO: GL_VertexAttrib2f

// TODO: GL_VertexAttrib2fv

// TODO: GL_VertexAttrib2s

// TODO: GL_VertexAttrib2sv

// TODO: GL_VertexAttrib3d

// TODO: GL_VertexAttrib3dv

// TODO: GL_VertexAttrib3f

// TODO: GL_VertexAttrib3fv

// TODO: GL_VertexAttrib3s

// TODO: GL_VertexAttrib3sv

// TODO: GL_VertexAttrib4Nbv

// TODO: GL_VertexAttrib4Niv

// TODO: GL_VertexAttrib4Nsv

// TODO: GL_VertexAttrib4Nub

// TODO: GL_VertexAttrib4Nubv

// TODO: GL_VertexAttrib4Nuiv

// TODO: GL_VertexAttrib4Nusv

// TODO: GL_VertexAttrib4bv

// TODO: GL_VertexAttrib4d

// TODO: GL_VertexAttrib4dv

// TODO: GL_VertexAttrib4f

// TODO: GL_VertexAttrib4fv

// TODO: GL_VertexAttrib4iv

// TODO: GL_VertexAttrib4s

// TODO: GL_VertexAttrib4sv

// TODO: GL_VertexAttrib4ubv

// TODO: GL_VertexAttrib4uiv

// TODO: GL_VertexAttrib4usv

// TODO: GL_VertexAttribBinding

// TODO: GL_VertexAttribDivisor

// TODO: GL_VertexAttribFormat

// TODO: GL_VertexAttribI1i

// TODO: GL_VertexAttribI1iv

// TODO: GL_VertexAttribI1ui

// TODO: GL_VertexAttribI1uiv

// TODO: GL_VertexAttribI2i

// TODO: GL_VertexAttribI2iv

// TODO: GL_VertexAttribI2ui

// TODO: GL_VertexAttribI2uiv

// TODO: GL_VertexAttribI3i

// TODO: GL_VertexAttribI3iv

// TODO: GL_VertexAttribI3ui

// TODO: GL_VertexAttribI3uiv

// TODO: GL_VertexAttribI4bv

// TODO: GL_VertexAttribI4i

// TODO: GL_VertexAttribI4iv

// TODO: GL_VertexAttribI4sv

// TODO: GL_VertexAttribI4ubv

// TODO: GL_VertexAttribI4ui

// TODO: GL_VertexAttribI4uiv

// TODO: GL_VertexAttribI4usv

// TODO: GL_VertexAttribIFormat

// TODO: GL_VertexAttribIPointer

// TODO: GL_VertexAttribL1d

// TODO: GL_VertexAttribL1dv

// TODO: GL_VertexAttribL2d

// TODO: GL_VertexAttribL2dv

// TODO: GL_VertexAttribL3d

// TODO: GL_VertexAttribL3dv

// TODO: GL_VertexAttribL4d

// TODO: GL_VertexAttribL4dv

// TODO: GL_VertexAttribLFormat

// TODO: GL_VertexAttribLPointer

// TODO: GL_VertexAttribP1ui

// TODO: GL_VertexAttribP1uiv

// TODO: GL_VertexAttribP2ui

// TODO: GL_VertexAttribP2uiv

// TODO: GL_VertexAttribP3ui

// TODO: GL_VertexAttribP3uiv

// TODO: GL_VertexAttribP4ui

// TODO: GL_VertexAttribP4uiv

// TODO: GL_VertexAttribPointer

// TODO: GL_VertexBindingDivisor

// TODO: GL_VertexP2ui

// TODO: GL_VertexP2uiv

// TODO: GL_VertexP3ui

// TODO: GL_VertexP3uiv

// TODO: GL_VertexP4ui

// TODO: GL_VertexP4uiv

void GL_VertexPointer(int size, unsigned int type, int stride, const void* data)
{
    X(); glVertexPointer(size, type, stride, data);
}

void GL_Viewport(int x, int y, int width, int height)
{
    X(); glViewport(x, y, width, height);
}

// TODO: GL_ViewportArrayv

// TODO: GL_ViewportIndexedf

// TODO: GL_ViewportIndexedfv

// TODO: GL_WaitSync

// TODO: GL_WindowPos2d

// TODO: GL_WindowPos2dv

// TODO: GL_WindowPos2f

// TODO: GL_WindowPos2fv

// TODO: GL_WindowPos2i

// TODO: GL_WindowPos2iv

// TODO: GL_WindowPos2s

// TODO: GL_WindowPos2sv

// TODO: GL_WindowPos3d

// TODO: GL_WindowPos3dv

// TODO: GL_WindowPos3f

// TODO: GL_WindowPos3fv

// TODO: GL_WindowPos3i

// TODO: GL_WindowPos3iv

// TODO: GL_WindowPos3s

// TODO: GL_WindowPos3sv

#undef X

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
 * ~~ [ texture renderer ] ~~ *
--------------------------------------------------------------------------------
TODO: count bind calls for each texture, get and set global texture bind counter
TODO: framebuffer (render target / render to texture) support similar to xl_core
--------------------------------------------------------------------------------
*/

struct gl_internal_texture_t
{
    double time_created;
    int width, height, comp;

    GLenum target;
    GLuint id;

    const char* path;
    const char* name;

    gl_texture_t* wrap()
    {
        return static_cast<gl_texture_t*>(this);
    }
};

#define X(p) ((gl_internal_texture_t*)(p))

static int gl_texture_compare(const void* a, const void* b)
{
    if (X(a)->time_created < X(b)->time_created) return -1;
    if (X(a)->time_created > X(b)->time_created) return +1;

    return 0;
}

void
gl_texture_set_int(gl_texture_t* texture, gl_texture_property_t property, int value)
{
    ae_switch (property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix)
    {
        case GL_TEXTURE_PROPERTY_DEFAULT_COPY:
        {
            AE_CASE_STUB(property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix);
        }
        break;

        case GL_TEXTURE_PROPERTY_COPY_ENABLED:
        {
            AE_CASE_STUB(property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix);
        }
        break;

        case GL_TEXTURE_PROPERTY_OPEN:
        {
            if (ae_likely(value == 0 && gl_texture_get_open(texture)))
            {
                AE_PROFILE_NAMED(gl_texture_close);

                ae_string_free(const_cast<char*>(X(texture)->path));
                ae_string_free(const_cast<char*>(X(texture)->name));

                GL_DeleteTextures(1, &X(texture)->id);

                // call any internal C++ object destructors
                X(texture)->~gl_internal_texture_t();

                ae_ptrset_remove(&gl_texture_lst, texture);
                ae_free(texture);
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_texture_property_name[property], __FUNCTION__);
        }
        break;
    }
}

int
gl_texture_get_int(gl_texture_t* texture, gl_texture_property_t property)
{
    ae_switch (property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix)
    {
        case GL_TEXTURE_PROPERTY_TOTAL:
        {
            return gl_texture_lst.count;
        }
        break;

        case GL_TEXTURE_PROPERTY_ID:
        {
            if (gl_texture_get_open(texture))
            {
                return static_cast<int>(X(texture)->id);
            }
        }
        break;

        case GL_TEXTURE_PROPERTY_TARGET:
        {
            // NOTE: fatal error here in non-OpenGL implementations of this
            if (gl_texture_get_open(texture))
            {
                return static_cast<int>(X(texture)->target);
            }
            else
            {
                return GL_TEXTURE_2D;
            }
        }
        break;

        case GL_TEXTURE_PROPERTY_WIDTH:
        {
            if (gl_texture_get_open(texture)) return X(texture)->width;
        }
        break;

        case GL_TEXTURE_PROPERTY_HEIGHT:
        {
            if (gl_texture_get_open(texture)) return X(texture)->height;
        }
        break;

        case GL_TEXTURE_PROPERTY_COMP:
        {
            if (gl_texture_get_open(texture)) return X(texture)->comp;
        }
        break;

        case GL_TEXTURE_PROPERTY_DEFAULT_COPY:
        {
            AE_CASE_STUB(property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix);
        }
        break;

        case GL_TEXTURE_PROPERTY_COPY_ENABLED:
        {
            AE_CASE_STUB(property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix);
        }
        break;

        case GL_TEXTURE_PROPERTY_OPEN:
        {
            return gl_is_init() && ae_ptrset_contains(&gl_texture_lst, texture);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_texture_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0;
}

void
gl_texture_set_str(gl_texture_t* texture, gl_texture_property_t property, const char* value)
{
    ae_switch (property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix)
    {
        case GL_TEXTURE_PROPERTY_PATH:
        {
            if (gl_texture_get_open(texture))
            {
                ae_string_free(const_cast<char*>(X(texture)->path));
                X(texture)->path = NULL;

                if (value != NULL)
                {
                    X(texture)->path = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        case GL_TEXTURE_PROPERTY_NAME:
        {
            if (gl_texture_get_open(texture))
            {
                ae_string_free(const_cast<char*>(X(texture)->name));
                X(texture)->name = NULL;

                if (value != NULL)
                {
                    X(texture)->name = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_texture_property_name[property], __FUNCTION__);
        }
        break;
    }
}

const char*
gl_texture_get_str(gl_texture_t* texture, gl_texture_property_t property)
{
    ae_switch (property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix)
    {
        case GL_TEXTURE_PROPERTY_STATUS:
        {
            if (gl_texture_get_open(texture))
            {
                static char gl_texture_status[1024];
                const char* name = X(texture)->name;

                if (!name || !name[0]) name = "?";

                char pixel_format[8]; // HACK truncate
                strcpy(pixel_format, "RGBA");
                pixel_format[X(texture)->comp] = '\0';

                if (AE_SNPRINTF( gl_texture_status, "\"%s\", %ix%i %s", name,
                    X(texture)->width, X(texture)->height, pixel_format) < 0)
                {
                    AE_WARN("%u bytes is not enough for texture status!",
                                (unsigned int)sizeof(gl_texture_status));
                }

                return (const char*)gl_texture_status;
            }
            else
            {
                return "closed";
            }
        }
        break;

        case GL_TEXTURE_PROPERTY_PATH:
        {
            if (gl_texture_get_open(texture) && X(texture)->path != NULL)
            {
                return X(texture)->path;
            }
        }
        break;

        case GL_TEXTURE_PROPERTY_NAME:
        {
            if (gl_texture_get_open(texture) && X(texture)->name != NULL)
            {
                return X(texture)->name;
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_texture_property_name[property], __FUNCTION__);
        }
        break;
    }

    return "";
}

void
gl_texture_set_img(gl_texture_t* texture, gl_texture_property_t property, ae_image_t* value)
{
    ae_switch (property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix)
    {
        case GL_TEXTURE_PROPERTY_IMAGE:
        {
            /* TODO: modern opengl implementations seem to handle non-power-of-2
             * textures gracefully, but we should have an explicit path for them.
             */
            if (gl_texture_get_open(texture))
            {
                AE_PROFILE_NAMED(gl_texture_set_image);

                gl_internal_texture_t* data = X(texture);
                GL_BindTexture(data->target, data->id);

                /* TODO: does this belong here? make it configurable per texture
                 */
                GL_TexParameteri(data->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                GL_TexParameteri(data->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                assert(value->width == (size_t)data->width);
                assert(value->height == (size_t)data->height);
                assert(value->type == AE_IMAGE_TYPE_U8);

                /* TODO: allow float textures
                 */
                GLenum gl_type = GL_UNSIGNED_BYTE;
                GLenum gl_format = GL_RGB;
                GLint gl_internal_format = GL_RGB;

                switch (data->comp)
                {
                    case 3: break;
                    case 4:
                    {
                        gl_internal_format = GL_RGBA;
                        gl_format = GL_RGBA;
                    }
                    break;

                    default: ae_assert(0, "%i", data->comp); break;
                }

                /* Flip the source image upside-down for OpenGL, then back again.
                 */
                ae_image_flip_y(value, NULL, 1, 1, 1, 1);

                GL_TexImage2D( data->target, 0, gl_internal_format, data->width,
                            data->height, 0, gl_format, gl_type, value->pixels);

                GL_Flush();
                ae_image_flip_y(value, NULL, 1, 1, 1, 1);
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_texture_property_name[property], __FUNCTION__);
        }
        break;
    }
}

ae_image_t*
gl_texture_get_img(gl_texture_t* texture, gl_texture_property_t property)
{
    ae_switch (property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix)
    {
        case GL_TEXTURE_PROPERTY_IMAGE:
        {
            AE_CASE_STUB(property, gl_texture_property, GL_TEXTURE_PROPERTY, suffix);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_texture_property_name[property], __FUNCTION__);
        }
        break;
    }

    return NULL;
}

gl_texture_t* gl_texture_create(int width, int height, int comp)
{
    AE_PROFILE_SCOPE();
    gl_init();

    ae_assert(gl_texture_count_all() <= GL_OBJECT_COUNT_LIMIT,
                    "hit texture object count limit - leak?");

    gl_internal_texture_t * data = ae_create(gl_internal_texture_t, clear);
    gl_texture_t * texture = (gl_texture_t *)data;

    if (ae_ptrset_add(&gl_texture_lst, data) == 0)
    {
        AE_WARN("GL texture is not new to the set (is set code stubbed?)");
    }

    data->time_created = ae_seconds();
    data->width = width;
    data->height = height;
    data->comp = comp;
    data->target = GL_TEXTURE_2D;
    GL_GenTextures(1, &data->id);

    // call constructor, which calls member object constructors
    new (data) gl_internal_texture_t;

    #if defined(AE_DEBUG)
    {
        AE_PROFILE_NAMED(gl_texture_create_debug);

        /* TODO: do we need to set the texture target to GL_RECTANGLE_2D?
         * TODO: make sure that texture dimensions don't exceed maximums
         */
        GL_BindTexture(data->target, data->id);

        /* TODO: does this belong here? make it configurable per texture
         */
        GL_TexParameteri(data->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        GL_TexParameteri(data->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLint gl_internal_format = 0;

        ae_image_format_t ae_format = AE_IMAGE_FORMAT_MONO;
        GLenum gl_format = 0;

        /* TODO: allow support for floating-point textures in this path
         */
        ae_image_type_t ae_type = AE_IMAGE_TYPE_U8;
        GLenum gl_type = GL_UNSIGNED_BYTE;

        switch (comp)
        {
            case 3:
            {
                gl_internal_format = GL_RGB;
                gl_format = GL_RGB;
                ae_format = AE_IMAGE_FORMAT_RGB;
            }
            break;

            case 4:
            {
                gl_internal_format = GL_RGBA;
                gl_format = GL_RGBA;
                ae_format = AE_IMAGE_FORMAT_RGBA;
            }
            break;

            default: ae_assert(0, "%i", comp); break;
        }

        // allocate image & fill it in with some noticeable default color
        ae_image_t blank =
        {
            NULL, (size_t)width, (size_t)height, ae_format, ae_type, NULL,
        };

        ae_image_alloc(&blank);

        const float color[4] = { 1.0f, 0.0f, 1.0f, 1.0f }; // set to pink
        ae_image_set_color(&blank, NULL, color, 1, 1, 1, 1);

        GL_TexImage2D(data->target, 0, gl_internal_format, width, height,
                                    0, gl_format, gl_type, blank.pixels);

        GL_Flush();
        ae_image_free(&blank);
    }
    #endif
    return texture;
}

gl_texture_t* gl_texture_load_from_memory(void* buf, size_t len)
{
    AE_PROFILE_SCOPE();

    ae_image_error_t error = AE_IMAGE_NO_CODEC;
    gl_texture_t * texture = gl_texture_load_from_memory_ex(buf, len, &error);

    ae_if (error != AE_IMAGE_SUCCESS)
    {
        ae_error("%s", ae_image_error_message(error, NULL));
    }

    return texture;
}

gl_texture_t* gl_texture_load_from_memory_ex(void* buf, size_t len,
                                    ae_image_error_t* error_status)
{
    AE_PROFILE_SCOPE(); // TODO: load compressed textures directly from DDS files

    ae_image_t image = AE_ZERO_STRUCT;
    gl_texture_t* texture = NULL;

    *error_status = ae_image_load_from_memory(&image, buf, len);

    ae_if (*error_status == AE_IMAGE_SUCCESS)
    {
        texture = gl_texture_create(image.width, image.height,
                    ae_image_format_components[image.format]);

        gl_texture_set_image(texture, &image);
        ae_image_free(&image);
    }

    return texture;
}

gl_texture_t* gl_texture_load(const char* filename)
{
    AE_PROFILE_SCOPE();

    ae_image_error_t error = AE_IMAGE_NO_CODEC; // for stub calls
    gl_texture_t * texture = gl_texture_load_ex(filename, &error);

    ae_if (error != AE_IMAGE_SUCCESS)
    {
        ae_error("%s", ae_image_error_message(error, filename));
    }

    return texture;
}

gl_texture_t*
gl_texture_load_ex(const char* filename, ae_image_error_t* error_status)
{
    AE_PROFILE_SCOPE(); // TODO: load compressed textures directly from DDS files

    ae_image_t image = AE_ZERO_STRUCT;
    gl_texture_t* texture = NULL;

    *error_status = ae_image_load(&image, filename);

    ae_if (*error_status == AE_IMAGE_SUCCESS)
    {
        texture = gl_texture_create(image.width, image.height,
                    ae_image_format_components[image.format]);

        gl_texture_set_image(texture, &image);
        ae_image_free(&image);

        char tex_name[1024 * 4]; // TODO: use "safer" sized string functions here
        ae_split_file_extension(ae_filename_from_path(filename), tex_name, NULL);

        gl_texture_set_path(texture, filename);
        gl_texture_set_name(texture, tex_name);
    }

    return texture;
}

void gl_texture_bind(gl_texture_t* texture)
{
    AE_PROFILE_SCOPE();
    gl_init();

    ae_if (gl_texture_get_open(texture))
    {
        GL_Enable(X(texture)->target);
        GL_BindTexture(X(texture)->target, X(texture)->id);
    }
    else
    {
        GL_Disable(GL_TEXTURE_2D);
    }

    gl_debug_log_error_state();
}

void gl_texture_draw_ex(gl_texture_t* texture, float x, float y, const float rgba[4])
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_texture_get_open(texture)))
    {
        GL_PushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
        {
            if (X(texture)->comp == 4 || ae_fabsf(1.0f - rgba[3]) > GL_DEFAULT_EPSILON)
            {
                GL_Enable(GL_BLEND);
                GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                GL_Color4fv(rgba);
            }

            GL_Enable(X(texture)->target);
            GL_BindTexture(X(texture)->target, X(texture)->id);

            GL_PushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
            {
                const float x1 = x, x2 = x + X(texture)->width;
                const float y1 = y, y2 = y + X(texture)->height;

                float tex[8] =
                {
                    0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
                };

                float array[32] =
                {
                    tex[0], tex[1], 0.0f, 1.0f, x1, y1, 0.0f, 1.0f,
                    tex[2], tex[3], 0.0f, 1.0f, x2, y1, 0.0f, 1.0f,
                    tex[4], tex[5], 0.0f, 1.0f, x2, y2, 0.0f, 1.0f,
                    tex[6], tex[7], 0.0f, 1.0f, x1, y2, 0.0f, 1.0f,
                };

                GL_InterleavedArrays(GL_T4F_V4F, 0, array);
                GL_DrawArrays(GL_QUADS, 0, 4);
            }
            GL_PopClientAttrib();
        }
        GL_PopAttrib();

        gl_debug_log_error_state();
    }
}

void gl_texture_draw(gl_texture_t* texture, float x, float y)
{
    gl_texture_draw_ex(texture, x, y, ae_color_white);
}

void gl_texture_draw_skybox(gl_texture_t* front,
                            gl_texture_t* back,
                            gl_texture_t* left,
                            gl_texture_t* right,
                            gl_texture_t* up,
                            gl_texture_t* down,
                            gl_coord3D_t* viewer)
{
    AE_PROFILE_SCOPE();
    gl_init();

    GL_PushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
    GL_Disable(GL_DEPTH_TEST);
    GL_Disable(GL_LIGHTING);

    float white[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GL_Color3fv(white);

    GL_PushMatrix();
    GL_LoadIdentity();
    {
        const float heading = viewer->heading;
        const float pitch = viewer->pitch;

        GL_Rotatef(-pitch  , 1.0f, 0.0f, 0.0f);
        GL_Rotatef(-heading, 0.0f, 1.0f, 0.0f);

        const float coords[6][4][3] =
        {
            {{ 1, -1, -1}, { 1,  1, -1}, {-1,  1, -1}, {-1, -1, -1}},
            {{-1, -1,  1}, {-1,  1,  1}, { 1,  1,  1}, { 1, -1,  1}},
            {{-1, -1, -1}, {-1,  1, -1}, {-1,  1,  1}, {-1, -1,  1}},
            {{ 1, -1,  1}, { 1,  1,  1}, { 1,  1, -1}, { 1, -1, -1}},
            {{ 1,  1,  1}, {-1,  1,  1}, {-1,  1, -1}, { 1,  1, -1}},
            {{-1, -1, -1}, {-1, -1,  1}, { 1, -1,  1}, { 1, -1, -1}},
        };

        const float texcoords[4][2] =
        {
            { 0, 0 },
            { 0, 1 },
            { 1, 1 },
            { 1, 0 },
        };

        gl_texture_t* textures[6] =
        {
            front, back, left, right, up, down,
        };

        for (size_t i = 0; i < 6; i++)
        {
            GLenum target = (GLenum)gl_texture_get_target(textures[i]);
            gl_texture_bind(textures[i]);

            GL_TexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            GL_TexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            GL_Begin(GL_QUADS);
            {
                GL_TexCoord2fv(texcoords[0]); GL_Vertex3fv(coords[i][0]);
                GL_TexCoord2fv(texcoords[1]); GL_Vertex3fv(coords[i][1]);
                GL_TexCoord2fv(texcoords[2]); GL_Vertex3fv(coords[i][2]);
                GL_TexCoord2fv(texcoords[3]); GL_Vertex3fv(coords[i][3]);
            }
            GL_End();
        }
    }
    GL_PopMatrix();
    GL_PopAttrib();
}

#undef X

/*
================================================================================
 * ~~ [ material system ] ~~ *
--------------------------------------------------------------------------------
TODO: count apply calls per material, get and set global mtl application counter
--------------------------------------------------------------------------------
*/

struct gl_internal_material_t
{
    gl_texture_t* texture;
    int id, owns_texture;
    double time_created;

    float diffuse[4];
    float ambient[4];
    float specular[4];
    float emission[4];

    int light_enabled;
    u64 flags;

    float opacity;
    float shininess;

    const char* path;
    const char* name;

    gl_material_t* wrap()
    {
        return static_cast<gl_material_t*>(this);
    }
};

#define X(p) ((gl_internal_material_t*)(p))

static int gl_material_compare(const void* a, const void* b)
{
    if (X(a)->time_created < X(b)->time_created) return -1;
    if (X(a)->time_created > X(b)->time_created) return +1;

    return 0;
}

void
gl_material_set_int(gl_material_t* material, gl_material_property_t property,
                    int value)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_OWNS_TEXTURE:
        {
            if (gl_material_get_open(material)) X(material)->owns_texture = value;
        }
        break;

        case GL_MATERIAL_PROPERTY_LIGHT_ENABLED:
        {
            if (gl_material_get_open(material)) X(material)->light_enabled = value;
        }
        break;

        case GL_MATERIAL_PROPERTY_OPEN:
        {
            if (ae_likely(value == 0 && gl_material_get_open(material)))
            {
                AE_PROFILE_NAMED(gl_material_close);

                ae_if (X(material)->owns_texture)
                {
                    gl_texture_close(X(material)->texture);
                }

                ae_string_free(const_cast<char*>(X(material)->path));
                ae_string_free(const_cast<char*>(X(material)->name));

                // call any internal C++ object destructors
                X(material)->~gl_internal_material_t();

                ae_ptrset_remove(&gl_material_lst, material);
                ae_free(material);
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }
}

int
gl_material_get_int(gl_material_t* material, gl_material_property_t property)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_TOTAL:
        {
            return gl_material_lst.count;
        }
        break;

        case GL_MATERIAL_PROPERTY_ID:
        {
            if (gl_material_get_open(material)) return X(material)->id;
        }
        break;

        case GL_MATERIAL_PROPERTY_TEXTURE:
        {
            if (gl_material_get_open(material)) return gl_texture_get_id(X(material)->texture);
        }
        break;

        case GL_MATERIAL_PROPERTY_OWNS_TEXTURE:
        {
            if (gl_material_get_open(material)) return X(material)->owns_texture;
        }
        break;

        case GL_MATERIAL_PROPERTY_LIGHT_ENABLED:
        {
            if (gl_material_get_open(material)) return X(material)->light_enabled;
        }
        break;

        case GL_MATERIAL_PROPERTY_OPEN:
        {
            return gl_is_init() && ae_ptrset_contains(&gl_material_lst, material);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0;
}

void
gl_material_set_u64(gl_material_t* material, gl_material_property_t property,
                    u64 value)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_FLAGS:
        {
            if (gl_material_get_open(material)) X(material)->flags = value;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }
}

u64
gl_material_get_u64(gl_material_t* material, gl_material_property_t property)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_FLAGS:
        {
            if (gl_material_get_open(material)) return X(material)->flags;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0;
}

void
gl_material_set_flt(gl_material_t* material, gl_material_property_t property,
                    float value)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_SHININESS:
        {
            if (gl_material_get_open(material)) X(material)->shininess = value;
        }
        break;

        case GL_MATERIAL_PROPERTY_OPACITY:
        {
            if (gl_material_get_open(material)) X(material)->opacity = value;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }
}

float
gl_material_get_flt(gl_material_t* material, gl_material_property_t property)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_SHININESS:
        {
            if (gl_material_get_open(material)) return X(material)->shininess;
        }
        break;

        case GL_MATERIAL_PROPERTY_OPACITY:
        {
            if (gl_material_get_open(material)) return X(material)->opacity;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0.0f;
}

void
gl_material_set_vec(gl_material_t* material, gl_material_property_t property,
                    float* value)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_DIFFUSE:
        {
            if (gl_material_get_open(material)) vec3copy(X(material)->diffuse, value);
        }
        break;

        case GL_MATERIAL_PROPERTY_AMBIENT:
        {
            if (gl_material_get_open(material)) vec3copy(X(material)->ambient, value);
        }
        break;

        case GL_MATERIAL_PROPERTY_SPECULAR:
        {
            if (gl_material_get_open(material)) vec3copy(X(material)->specular, value);
        }
        break;

        case GL_MATERIAL_PROPERTY_EMISSION:
        {
            if (gl_material_get_open(material)) vec3copy(X(material)->emission, value);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }
}

float*
gl_material_get_vec(gl_material_t* material, gl_material_property_t property)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_DIFFUSE:
        {
            if (gl_material_get_open(material)) return X(material)->diffuse;
        }
        break;

        case GL_MATERIAL_PROPERTY_AMBIENT:
        {
            if (gl_material_get_open(material)) return X(material)->ambient;
        }
        break;

        case GL_MATERIAL_PROPERTY_SPECULAR:
        {
            if (gl_material_get_open(material)) return X(material)->specular;
        }
        break;

        case GL_MATERIAL_PROPERTY_EMISSION:
        {
            if (gl_material_get_open(material)) return X(material)->emission;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }

    static const float zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    return const_cast<float*>(zero);
}

void
gl_material_set_str(gl_material_t* material, gl_material_property_t property,
                    const char* value)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_PATH:
        {
            if (gl_material_get_open(material))
            {
                ae_string_free(const_cast<char*>(X(material)->path));
                X(material)->path = NULL;

                if (value != NULL)
                {
                    X(material)->path = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        case GL_MATERIAL_PROPERTY_NAME:
        {
            if (gl_material_get_open(material))
            {
                ae_string_free(const_cast<char*>(X(material)->name));
                X(material)->name = NULL;

                if (value != NULL)
                {
                    X(material)->name = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }
}

const char*
gl_material_get_str(gl_material_t* material, gl_material_property_t property)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_STATUS:
        {
            if (gl_material_get_open(material))
            {
                static char gl_material_status[1024];
                const char* name = X(material)->name;

                if (name && name[0])
                {
                    if (AE_SNPRINTF(gl_material_status, "\"%s\"", name) < 0)
                    {
                        AE_WARN("%u bytes is not enough for material status!",
                                    (unsigned int)sizeof(gl_material_status));
                    }

                    return (const char*)gl_material_status;
                }
                else
                {
                    return "open";
                }
            }
            else
            {
                return "closed";
            }
        }
        break;

        case GL_MATERIAL_PROPERTY_PATH:
        {
            if (gl_material_get_open(material) && X(material)->path != NULL)
            {
                return X(material)->path;
            }
        }
        break;

        case GL_MATERIAL_PROPERTY_NAME:
        {
            // TODO: if the material doesn't have a name, return the texture's
            if (gl_material_get_open(material) && X(material)->name != NULL)
            {
                return X(material)->name;
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }

    return "";
}

void
gl_material_set_tex(gl_material_t* material, gl_material_property_t property,
                    gl_texture_t* value)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_TEXTURE:
        {
            if (gl_material_get_open(material)) X(material)->texture = value;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }
}

gl_texture_t*
gl_material_get_tex(gl_material_t* material, gl_material_property_t property)
{
    ae_switch (property, gl_material_property, GL_MATERIAL_PROPERTY, suffix)
    {
        case GL_MATERIAL_PROPERTY_TEXTURE:
        {
            if (gl_material_get_open(material)) return X(material)->texture;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_material_property_name[property], __FUNCTION__);
        }
        break;
    }

    return NULL;
}

gl_material_t* gl_material_create(void)
{
    AE_PROFILE_SCOPE();
    gl_init();

    ae_assert(gl_material_count_all() <= GL_OBJECT_COUNT_LIMIT,
                    "hit material object count limit - leak?");

    gl_internal_material_t* data = ae_create(gl_internal_material_t, clear);
    gl_material_t* material = (gl_material_t*)data;

    data->time_created = ae_seconds();
    data->id = (int)ae_random_u32();

    data->diffuse[0] = data->diffuse[1] = data->diffuse[2] = 0.8f;
    data->ambient[0] = data->ambient[1] = data->ambient[2] = 0.2f;

    data->specular[0] = data->specular[1] = data->specular[2] = 0.0f;
    data->emission[0] = data->emission[1] = data->emission[2] = 0.0f;

    data->light_enabled = 1;
    data->opacity = 1.0f;
    data->shininess = 0.0f;

    // call constructor, which calls member object constructors
    new (data) gl_internal_material_t;

    if (ae_ptrset_add(&gl_material_lst, data) == 0)
    {
        AE_WARN("GL material is not new to the set (is set code stubbed?)");
    }

    return material;
}

void gl_material_set_all(gl_material_t* material, const float* value)
{
    if (ae_likely(gl_material_get_open(material)))
    {
        vec3copy(X(material)->ambient, value);
        vec3copy(X(material)->diffuse, value);

        // vec3copy(X(material)->specular, value);
        // vec3copy(X(material)->emission, value);
    }
}

void gl_material_lerp(gl_material_t* dst, gl_material_t* a, gl_material_t* b, float t)
{
    AE_STUB();
}

void gl_material_apply_ex(gl_material_t* material, gl_buffer_t* buffer)
{
    AE_PROFILE_SCOPE();
    gl_init();

    if (ae_likely(gl_material_get_open(material)))
    {
        gl_texture_bind(X(material)->texture); // can disable texturing

        if (X(material)->light_enabled)
        {
            GL_Enable(GL_LIGHTING);

            X(material)->diffuse [3] =
            X(material)->ambient [3] =
            X(material)->specular[3] =
            X(material)->emission[3] = X(material)->opacity;

            GL_Materialfv(GL_FRONT, GL_DIFFUSE , X(material)->diffuse );
            GL_Materialfv(GL_FRONT, GL_AMBIENT , X(material)->ambient );
            GL_Materialfv(GL_FRONT, GL_SPECULAR, X(material)->specular);
            GL_Materialfv(GL_FRONT, GL_EMISSION, X(material)->emission);

            GL_Materialf(GL_FRONT, GL_SHININESS, X(material)->shininess);
        }
        else
        {
            // TODO: push/pop instead of apply would be better for this
            GL_Disable(GL_LIGHTING);
        }

        // FIXME: when one translucent object is drawn, all other objects
        // become slightly translucent. so for now, everything is opaque.
        // TODO should we disable depth masking for translucent materials?

    #if 0
        if (ae_fabsf(X(material)->opacity - 1.0f) > 0.0001f ||
            gl_texture_get_comp(X(material)->texture) == 4)
        {
            // TODO: custom blending modes (use ae_image blitter enums
            // translated for glBlendFunc and glBlendEquationSeparate).

            GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            GL_Enable(GL_BLEND);
        }
        else
        {
            GL_Disable(GL_BLEND);
        }
    #endif

        ae_vertex_format_t vertex_format = static_cast<ae_vertex_format_t>( \
                                        gl_buffer_get_vertex_format(buffer));

        ae_if (ae_vertex_format_has_color(vertex_format))
        {
            // TODO: mode property (requires enum definition?)
            GL_ColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

            GL_Enable(GL_COLOR_MATERIAL);
        }
        else
        {
            GL_Disable(GL_COLOR_MATERIAL);
        }
    }

    gl_debug_log_error_state();
}

void gl_material_apply(gl_material_t* material)
{
    gl_material_apply_ex(material, NULL);
}

#undef X

/*
================================================================================
 * ~~ [ lighting system ] ~~ *
--------------------------------------------------------------------------------
*/

struct gl_internal_light_t
{
    double time_created;
    int id;

    float ambient[4];
    float diffuse[4];
    float specular[4];
    float position[4];
    float spot_direction[4];
    float spot_exponent;
    float spot_cutoff;
    float constant_attenuation;
    float linear_attenuation;
    float quadratic_attenuation;

    gl_light_t* wrap()
    {
        return static_cast<gl_light_t*>(this);
    }
};

#define X(p) ((gl_internal_light_t*)(p))

static int gl_light_compare(const void* a, const void* b)
{
    if (X(a)->time_created < X(b)->time_created) return -1;
    if (X(a)->time_created > X(b)->time_created) return +1;

    return 0;
}

void
gl_light_set_int(gl_light_t* light, gl_light_property_t property, int value)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_OPEN:
        {
            if (ae_likely(value == 0 && gl_light_get_open(light)))
            {
                AE_PROFILE_NAMED(gl_light_close);

                // call any internal C++ object destructors
                X(light)->~gl_internal_light_t();

                ae_ptrset_remove(&gl_light_lst, light);
                ae_free(light);
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }
}

int
gl_light_get_int(gl_light_t* light, gl_light_property_t property)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_TOTAL:
        {
            return gl_light_lst.count;
        }
        break;

        case GL_LIGHT_PROPERTY_ID:
        {
            if (gl_light_get_open(light)) return X(light)->id;
        }
        break;

        case GL_LIGHT_PROPERTY_OPEN:
        {
            return gl_is_init() && ae_ptrset_contains(&gl_light_lst, light);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0;
}

void
gl_light_set_str(gl_light_t* light, gl_light_property_t property, const char* value)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_NAME:
        {
            AE_CASE_STUB(property, gl_light_property, GL_LIGHT_PROPERTY, suffix);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }
}

const char*
gl_light_get_str(gl_light_t* light, gl_light_property_t property)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_STATUS:
        {
            // TODO: show more information here (position, name, color)
            return gl_light_get_open(light) ? "open" : "closed";
        }
        break;

        case GL_LIGHT_PROPERTY_NAME:
        {
            AE_CASE_STUB(property, gl_light_property, GL_LIGHT_PROPERTY, suffix);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }

    return "";
}

void
gl_light_set_vec(gl_light_t* light, gl_light_property_t property, float* value)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_AMBIENT:
        {
            if (gl_light_get_open(light)) vec4copy(X(light)->ambient, value);
        }
        break;

        case GL_LIGHT_PROPERTY_DIFFUSE:
        {
            if (gl_light_get_open(light)) vec4copy(X(light)->diffuse, value);
        }
        break;

        case GL_LIGHT_PROPERTY_SPECULAR:
        {
            if (gl_light_get_open(light)) vec4copy(X(light)->specular, value);
        }
        break;

        case GL_LIGHT_PROPERTY_POSITION:
        {
            if (gl_light_get_open(light)) vec4copy(X(light)->position, value);
        }
        break;

        case GL_LIGHT_PROPERTY_SPOT_DIRECTION:
        {
            if (gl_light_get_open(light)) vec3copy(X(light)->spot_direction, value);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }
}

float*
gl_light_get_vec(gl_light_t* light, gl_light_property_t property)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_AMBIENT:
        {
            if (gl_light_get_open(light)) return X(light)->ambient;
        }
        break;

        case GL_LIGHT_PROPERTY_DIFFUSE:
        {
            if (gl_light_get_open(light)) return X(light)->diffuse;
        }
        break;

        case GL_LIGHT_PROPERTY_SPECULAR:
        {
            if (gl_light_get_open(light)) return X(light)->specular;
        }
        break;

        case GL_LIGHT_PROPERTY_POSITION:
        {
            if (gl_light_get_open(light)) return X(light)->position;
        }
        break;

        case GL_LIGHT_PROPERTY_SPOT_DIRECTION:
        {
            if (gl_light_get_open(light)) return X(light)->spot_direction;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }

    static const float zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    return const_cast<float*>(zero);
}

void
gl_light_set_flt(gl_light_t* light, gl_light_property_t property, float value)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_SPOT_EXPONENT:
        {
            if (gl_light_get_open(light)) X(light)->spot_exponent = value;
        }
        break;

        case GL_LIGHT_PROPERTY_SPOT_CUTOFF:
        {
            if (gl_light_get_open(light)) X(light)->spot_cutoff = value;
        }
        break;

        case GL_LIGHT_PROPERTY_CONSTANT_ATTENUATION:
        {
            if (gl_light_get_open(light)) X(light)->constant_attenuation = value;
        }
        break;

        case GL_LIGHT_PROPERTY_LINEAR_ATTENUATION:
        {
            if (gl_light_get_open(light)) X(light)->linear_attenuation = value;
        }
        break;

        case GL_LIGHT_PROPERTY_QUADRATIC_ATTENUATION:
        {
            if (gl_light_get_open(light)) X(light)->quadratic_attenuation = value;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }
}

float
gl_light_get_flt(gl_light_t* light, gl_light_property_t property)
{
    ae_switch (property, gl_light_property, GL_LIGHT_PROPERTY, suffix)
    {
        case GL_LIGHT_PROPERTY_SPOT_EXPONENT:
        {
            if (gl_light_get_open(light)) return X(light)->spot_exponent;
        }
        break;

        case GL_LIGHT_PROPERTY_SPOT_CUTOFF:
        {
            if (gl_light_get_open(light)) return X(light)->spot_cutoff;
        }
        break;

        case GL_LIGHT_PROPERTY_CONSTANT_ATTENUATION:
        {
            if (gl_light_get_open(light)) return X(light)->constant_attenuation;
        }
        break;

        case GL_LIGHT_PROPERTY_LINEAR_ATTENUATION:
        {
            if (gl_light_get_open(light)) return X(light)->linear_attenuation;
        }
        break;

        case GL_LIGHT_PROPERTY_QUADRATIC_ATTENUATION:
        {
            if (gl_light_get_open(light)) return X(light)->quadratic_attenuation;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_light_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0.0f;
}

gl_light_t* gl_light_create(void)
{
    AE_PROFILE_SCOPE();
    gl_init();

    ae_assert(gl_light_count_all() <= 8, "hit light object count limit");

    gl_internal_light_t* data = ae_create(gl_internal_light_t, clear);
    gl_light_t* light = (gl_light_t*)data;

    data->time_created = ae_seconds();
    data->id = (int)ae_random_u32();

    #define ENTRY(name, r, g, b, a) do  \
    {                                   \
        float name[4] = { r, g, b, a }; \
        vec4copy(data->name, name);     \
    }                                   \
    while (AE_NULL_WHILE_LOOP_CONDITION)

    ENTRY(ambient, 0, 0, 0, 1);
    ENTRY(diffuse, 0, 0, 0, 0);
    ENTRY(specular, 0, 0, 0, 0);
    ENTRY(position, 0, 0, 1, 0);
    ENTRY(spot_direction, 0, 0, -1, 0);

    #undef ENTRY

    data->spot_exponent = 0.0f;
    data->spot_cutoff = 180.0f;
    data->constant_attenuation = 1.0f;
    data->linear_attenuation = 0.0f;
    data->quadratic_attenuation = 0.0f;

    // call constructor, which calls member object constructors
    new (data) gl_internal_light_t;

    if (ae_ptrset_add(&gl_light_lst, data) == 0)
    {
        AE_WARN("GL light is not new to the set (is set code stubbed?)");
    }

    return light;
}

void gl_light_set_all(gl_light_t* light, const float* value)
{
    if (ae_likely(gl_light_get_open(light)))
    {
        vec4copy(X(light)->ambient, value);
        vec4copy(X(light)->diffuse, value);
    }
}

void gl_light_lerp(gl_light_t* light, gl_light_t* a, gl_light_t* b, float t)
{
    AE_STUB();
}

void gl_light_default_scene(void)
{
    AE_PROFILE_SCOPE();
    gl_init();
    gl_light_close_all();

    #define ENTRY(name, r, g, b, a) do  \
    {                                   \
        float name[4] = { r, g, b, a }; \
        vec4copy(data->name, name);     \
    }                                   \
    while (AE_NULL_WHILE_LOOP_CONDITION)

    if (1) // one medium white light from above
    {
        gl_light_t* light = gl_light_create();
        gl_internal_light_t * data = X(light);

        ENTRY(position, 0.0f, 1.0f, 0.0f, 0.0f);
        ENTRY(ambient, 0.2f, 0.2f, 0.2f, 1.0f);
        ENTRY(diffuse, 0.5f, 0.5f, 0.5f, 1.0f);
        ENTRY(specular, 0.0f, 0.0f, 0.0f, 1.0f);
    }

    if (1) // one softer white light from below
    {
        gl_light_t* light = gl_light_create();
        gl_internal_light_t * data = X(light);

        ENTRY(position, 0.0f, -1.0f, 0.1f, 0.0f);
        ENTRY(ambient, 0.0f, 0.0f, 0.0f, 1.0f);
        ENTRY(diffuse, 0.2f, 0.2f, 0.2f, 1.0f);
        ENTRY(specular, 0.0f, 0.0f, 0.0f, 1.0f);
    }

    #undef ENTRY
}

void gl_light_apply_all(void)
{
    AE_PROFILE_SCOPE();
    gl_init();

    gl_light_t* lights[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
    gl_light_list_all(lights);

    for (size_t i = 0; i < 8; i++)
    {
        const GLenum gl_light = GL_LIGHT0 + i;
        gl_light_t* light = lights[i];

        if (gl_light_get_open(light))
        {
            GL_Enable(gl_light);

            GL_Lightfv(gl_light, GL_AMBIENT, X(light)->ambient);
            GL_Lightfv(gl_light, GL_DIFFUSE, X(light)->diffuse);
            GL_Lightfv(gl_light, GL_SPECULAR, X(light)->specular);
            GL_Lightfv(gl_light, GL_POSITION, X(light)->position);

            GL_Lightfv(gl_light, GL_SPOT_DIRECTION, X(light)->spot_direction);
            GL_Lightf(gl_light, GL_SPOT_EXPONENT, X(light)->spot_exponent);
            GL_Lightf(gl_light, GL_SPOT_CUTOFF, X(light)->spot_cutoff);

            GL_Lightf(gl_light, GL_CONSTANT_ATTENUATION, X(light)->constant_attenuation);
            GL_Lightf(gl_light, GL_LINEAR_ATTENUATION, X(light)->linear_attenuation);
            GL_Lightf(gl_light, GL_QUADRATIC_ATTENUATION, X(light)->quadratic_attenuation);
        }
        else
        {
            GL_Disable(gl_light);
        }
    }

    gl_debug_log_error_state();
}

#undef X

/*
================================================================================
 * ~~ [ vertex buffers ] ~~ *
--------------------------------------------------------------------------------
TODO: count draw calls for each buffer, multiply by verts for global tri counter
TODO: readonly lock and unlock; take CRC in debug mode to ensure no writes occur
TODO: (d/r)einterleave: split vertices into separate element channels and rejoin
TODO: gl_buffer_change_vertex_format: add or remove elements, new elems are zero
TODO: tessellate triangles above a given surface area threshold, for light & fog
--------------------------------------------------------------------------------
*/

struct gl_internal_buffer_t
{
    int locked, line_mode, id, compute_aabbox, smooth_shading;
    double time_created;

    ae_vertex_format_t vertex_format;
    ae_array_t vertices;

    ae_index_type_t index_type;
    ae_array_t indices;

    float minv[3];
    float maxv[3];

    // usually triangles, but we can use this to draw lines too
    GLenum draw_mode;

    gl_buffer_t* wrap()
    {
        return static_cast<gl_buffer_t*>(this);
    }
};

#define X(p) ((gl_internal_buffer_t*)(p))

static int gl_buffer_compare(const void* a, const void* b)
{
    if (X(a)->time_created < X(b)->time_created) return -1;
    if (X(a)->time_created > X(b)->time_created) return +1;

    return 0;
}

void
gl_buffer_set_int(gl_buffer_t* buffer, gl_buffer_property_t property, int value)
{
    ae_switch (property, gl_buffer_property, GL_BUFFER_PROPERTY, suffix)
    {
        case GL_BUFFER_PROPERTY_VERTEX_FORMAT:
        {
            if (gl_buffer_get_open(buffer))
            {
                X(buffer)->vertex_format = (ae_vertex_format_t)value;
            }
        }
        break;

        case GL_BUFFER_PROPERTY_INDEX_TYPE:
        {
            if (gl_buffer_get_open(buffer))
            {
                X(buffer)->index_type = (ae_index_type_t)value;
            }
        }
        break;

        case GL_BUFFER_PROPERTY_STATIC_STORAGE:
        /*{
            gl_buffer_set_compute_aabbox(buffer, value); // TODO use GL VBOs here
        }
        break;
        */
        case GL_BUFFER_PROPERTY_COMPUTE_AABBOX:
        {
            if (gl_buffer_get_open(buffer) && value != X(buffer)->compute_aabbox)
            {
                X(buffer)->compute_aabbox = value;

                ae_if (value)
                {
                    ae_array_t v, i; // lock & unlock to recompute aabbox
                    gl_buffer_lock(buffer, &v, &i);

                    gl_buffer_unlock(buffer, &v, X(buffer)->vertex_format,
                        &i, X(buffer)->index_type);
                }
                else
                {
                    vec3zero(X(buffer)->minv);
                    vec3zero(X(buffer)->maxv);
                }
            }
        }
        break;

        case GL_BUFFER_PROPERTY_SMOOTH_SHADING:
        {
            if (gl_buffer_get_open(buffer)) X(buffer)->smooth_shading = value;
        }
        break;

        case GL_BUFFER_PROPERTY_LINE_MODE:
        {
            if (gl_buffer_get_open(buffer)) X(buffer)->line_mode = value;
        }
        break;

        case GL_BUFFER_PROPERTY_OPEN:
        {
            if (ae_likely(value == 0 && gl_buffer_get_open(buffer)))
            {
                AE_PROFILE_NAMED(gl_buffer_close);

                ae_assert(!X(buffer)->locked, "freed locked buffer");

                ae_array_free(&X(buffer)->vertices);
                ae_array_free(&X(buffer)->indices);

                // call any internal C++ object destructors
                X(buffer)->~gl_internal_buffer_t();

                ae_ptrset_remove(&gl_buffer_lst, buffer);
                ae_free(buffer);
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_buffer_property_name[property], __FUNCTION__);
        }
        break;
    }
}

int
gl_buffer_get_int(gl_buffer_t* buffer, gl_buffer_property_t property)
{
    ae_switch (property, gl_buffer_property, GL_BUFFER_PROPERTY, suffix)
    {
        case GL_BUFFER_PROPERTY_TOTAL:
        {
            return gl_buffer_lst.count;
        }
        break;

        case GL_BUFFER_PROPERTY_ID:
        {
            if (gl_buffer_get_open(buffer)) return X(buffer)->id;
        }
        break;

        case GL_BUFFER_PROPERTY_VERTEX_FORMAT:
        {
            if (gl_buffer_get_open(buffer))
            {
                return static_cast<int>(X(buffer)->vertex_format);
            }
            else
            {
                return AE_V3F;
            }
        }
        break;

        case GL_BUFFER_PROPERTY_INDEX_TYPE:
        {
            if (gl_buffer_get_open(buffer))
            {
                return static_cast<int>(X(buffer)->index_type);
            }
        }
        break;

        case GL_BUFFER_PROPERTY_VERTEX_COUNT:
        {
            if (gl_buffer_get_open(buffer))
            {
                return X(buffer)->vertices.size / (sizeof( float ) * \
                    ae_vertex_format_size[X(buffer)->vertex_format]);
            }
        }
        break;

        case GL_BUFFER_PROPERTY_INDEX_COUNT:
        {
            if (gl_buffer_get_open(buffer))
            {
                return X(buffer)->indices.size / ae_index_type_bytes[ \
                                                X(buffer)->index_type];
            }
        }
        break;

        case GL_BUFFER_PROPERTY_TRIANGLE_COUNT:
        {
            if (gl_buffer_get_open(buffer))
            {
                int index_count = gl_buffer_get_index_count(buffer);
                if( index_count )
                {
                    return index_count / 3;
                }
                else
                {
                    return gl_buffer_get_vertex_count(buffer) / 3;
                }
            }
        }
        break;

        case GL_BUFFER_PROPERTY_STATIC_STORAGE:
        /*{
            return gl_buffer_get_compute_aabbox(buffer); // TODO use GL VBOs
        }
        break;
        */
        case GL_BUFFER_PROPERTY_COMPUTE_AABBOX:
        {
            if (gl_buffer_get_open(buffer)) return X(buffer)->compute_aabbox;
        }
        break;

        case GL_BUFFER_PROPERTY_LOCKED:
        {
            if (gl_buffer_get_open(buffer)) return X(buffer)->locked;
        }
        break;

        case GL_BUFFER_PROPERTY_SMOOTH_SHADING:
        {
            if (gl_buffer_get_open(buffer)) return X(buffer)->smooth_shading;
        }
        break;

        case GL_BUFFER_PROPERTY_LINE_MODE:
        {
            // XXX: should line and shading modes be material properties?
            if (gl_buffer_get_open(buffer)) return X(buffer)->line_mode;
        }
        break;

        case GL_BUFFER_PROPERTY_OPEN:
        {
            return gl_is_init() && ae_ptrset_contains(&gl_buffer_lst, buffer);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_buffer_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0;
}

void
gl_buffer_set_str(gl_buffer_t* buffer, gl_buffer_property_t property, const char* value)
{
    ae_switch (property, gl_buffer_property, GL_BUFFER_PROPERTY, suffix)
    {
        case GL_BUFFER_PROPERTY_VERTEX_FORMAT:
        {
            if (gl_buffer_get_open(buffer))
            {
                // TODO: allow for partial valid names (see index setter)
                size_t i = 0, n = AE_VERTEX_FORMAT_COUNT;

                for (; i < n; i++)
                {
                    if (strcmp(ae_vertex_format_name[i] + 3, value) == 0)
                    {
                        X(buffer)->vertex_format = (ae_vertex_format_t)i;
                        return;
                    }
                }

                AE_WARN("invalid vertex format: %s", value);
            }
        }
        break;

        case GL_BUFFER_PROPERTY_INDEX_TYPE:
        {
            if (gl_buffer_get_open(buffer))
            {
                if (strstr(value, "8")) // major hackitude!!!
                    X(buffer)->index_type = AE_INDEX_TYPE_U8;

                else if (strstr(value, "16"))
                    X(buffer)->index_type = AE_INDEX_TYPE_U16;

                else if (strstr(value, "32"))
                    X(buffer)->index_type = AE_INDEX_TYPE_U32;

                else
                    AE_WARN("invalid index type: %s", value);
            }
        }
        break;

        case GL_BUFFER_PROPERTY_PATH:
        {
            AE_CASE_STUB(property, gl_buffer_property, GL_BUFFER_PROPERTY, suffix);
        }
        break;

        case GL_BUFFER_PROPERTY_NAME:
        {
            AE_CASE_STUB(property, gl_buffer_property, GL_BUFFER_PROPERTY, suffix);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_buffer_property_name[property], __FUNCTION__);
        }
        break;
    }
}

const char*
gl_buffer_get_str(gl_buffer_t* buffer, gl_buffer_property_t property)
{
    ae_switch (property, gl_buffer_property, GL_BUFFER_PROPERTY, suffix)
    {
        case GL_BUFFER_PROPERTY_VERTEX_FORMAT:
        {
            if (gl_buffer_get_open(buffer))
            {
                return ae_vertex_format_name[X(buffer)->vertex_format] + strlen("AE_");
            }
        }
        break;

        case GL_BUFFER_PROPERTY_INDEX_TYPE:
        {
            if (gl_buffer_get_open(buffer))
            {
                return ae_index_type_name[X(buffer)->index_type] + strlen("AE_");
            }
        }
        break;

        case GL_BUFFER_PROPERTY_STATUS:
        {
            if (gl_buffer_get_open(buffer))
            {
                static char gl_buffer_status[1024];

                const char* vn = ae_vertex_format_name[X(buffer)->vertex_format];
                const char* in = ae_index_type_name[X(buffer)->index_type];

                u32 vc = X(buffer)->vertices.size / (sizeof(float) * \
                        ae_vertex_format_size[X(buffer)->vertex_format]);

                u32 ic = X(buffer)->indices.size / ae_index_type_bytes \
                                                [X(buffer)->index_type];

                if (AE_SNPRINTF(gl_buffer_status, "%u \"%s\" vtx, %u \"%s\" idx",
                            vc, vn + strlen("AE_"), ic, in + strlen("AE_")) < 0)
                {
                    AE_WARN("%u bytes is not enough for buffer status!",
                                (unsigned int)sizeof(gl_buffer_status));
                }

                return (const char*)gl_buffer_status;
            }
            else
            {
                return "closed";
            }
        }
        break;

        case GL_BUFFER_PROPERTY_PATH:
        {
            AE_CASE_STUB(property, gl_buffer_property, GL_BUFFER_PROPERTY, suffix);
        }
        break;

        case GL_BUFFER_PROPERTY_NAME:
        {
            AE_CASE_STUB(property, gl_buffer_property, GL_BUFFER_PROPERTY, suffix);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_buffer_property_name[property], __FUNCTION__);
        }
        break;
    }

    return "";
}

gl_buffer_t* gl_buffer_create_empty(void)
{
    AE_PROFILE_SCOPE();
    gl_init();

    ae_assert(gl_buffer_count_all() <= GL_OBJECT_COUNT_LIMIT,
                    "hit buffer object count limit - leak?");

    gl_internal_buffer_t * data = ae_create(gl_internal_buffer_t, clear);
    gl_buffer_t * buffer = (gl_buffer_t *)data;

    data->time_created = ae_seconds();
    data->id = (int)ae_random_u32();

    // call constructor, which calls member object constructors
    new (data) gl_internal_buffer_t;

    if (ae_ptrset_add(&gl_buffer_lst, data) == 0)
    {
        AE_WARN("GL buffer is not new to the set (is set code stubbed?)");
    }

    // for internal line rendering stuff
    data->draw_mode = GL_TRIANGLES;

    data->smooth_shading = 1;

    data->vertex_format = AE_T2F_N3F_V3F;
    data->index_type = AE_INDEX_TYPE_U32;

    return buffer;
}

gl_buffer_t* gl_buffer_create_from(ae_array_t* vertices, ae_vertex_format_t vertex_format,
                                ae_array_t* indices, ae_index_type_t index_type, int copy)
{
    AE_PROFILE_SCOPE();

    gl_buffer_t* buffer = gl_buffer_create_empty();
    ae_array_t v, i;

    gl_buffer_lock(buffer, &v, &i);
    assert(v.data == NULL && i.data == NULL);

    ae_if (copy)
    {
        ae_array_copy(&v, vertices);
        ae_array_copy(&i, indices);

        gl_buffer_unlock(buffer, &v, vertex_format, &i, index_type);
    }
    else
    {
        gl_buffer_unlock(buffer, vertices, vertex_format, indices, index_type);
    }

    return buffer;
}

void gl_buffer_lock(gl_buffer_t* buffer, ae_array_t* vertices, ae_array_t* indices)
{
    // I feel like the standard graphics nomenclature is backwards here.
    // You should unlock objects to gain access to their internals, and
    // lock them to return to a valid state in which they can be drawn.
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        ae_assert(!X(buffer)->locked, "buffer is already locked!");

        assert(vertices); *vertices = X(buffer)->vertices;
        memset(&X(buffer)->vertices, 0, sizeof(ae_array_t));

        assert(indices); *indices = X(buffer)->indices;
        memset(&X(buffer)->indices, 0, sizeof(ae_array_t));

        X(buffer)->locked = 1;
    }
}

void gl_buffer_unlock(gl_buffer_t* buffer, ae_array_t* vertices, ae_vertex_format_t v_format,
                                            ae_array_t* indices, ae_index_type_t index_type)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        ae_assert(X(buffer)->locked, "buffer is already unlocked!");

        X(buffer)->vertex_format = v_format;
        X(buffer)->index_type = index_type;

        assert(vertices); X(buffer)->vertices = *vertices;
        memset(vertices, 0, sizeof(ae_array_t));

        assert(indices); X(buffer)->indices = *indices;
        memset(indices, 0, sizeof(ae_array_t));

        if (ae_likely(X(buffer)->compute_aabbox))
        {
            vtx_aabbox3_ex( X(buffer)->minv, X(buffer)->maxv,
                            (float*)X(buffer)->vertices.data,

                            X(buffer)->vertices.size / sizeof( float ),
                            ae_vertex_format_position_offset[v_format],

                            ae_vertex_format_size[v_format]);
        }
        else
        {
            vec3zero(X(buffer)->minv);
            vec3zero(X(buffer)->maxv);
        }

        X(buffer)->locked = 0;
    }
}

void gl_buffer_get_vertex(gl_buffer_t* buffer, size_t which,
                    float* t, float* c, float* n, float* p)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked);

        #define GL_BUFFER_TOTAL_VERTEX_FORMAT_INFO()                                    \
                                                                                        \
            const ae_vertex_format_t vertex_format = X(buffer)->vertex_format;          \
            const size_t vertex_size = ae_vertex_format_size[vertex_format];            \
                                                                                        \
            const size_t float_count = X(buffer)->vertices.size / sizeof(float);        \
            const size_t vertex_count = float_count / vertex_size;                      \
                                                                                        \
            const size_t tex_offset = ae_vertex_format_texcoord_offset [vertex_format]; \
            const size_t col_offset = ae_vertex_format_color_offset    [vertex_format]; \
            const size_t nrm_offset = ae_vertex_format_normal_offset   [vertex_format]; \
            const size_t pos_offset = ae_vertex_format_position_offset [vertex_format]; \
                                                                                        \
            const size_t tex_size = ae_vertex_format_texcoord_size [vertex_format];     \
            const size_t col_size = ae_vertex_format_color_size    [vertex_format];     \
            const size_t nrm_size = ae_vertex_format_normal_size   [vertex_format];     \
            const size_t pos_size = ae_vertex_format_position_size [vertex_format];     \

        GL_BUFFER_TOTAL_VERTEX_FORMAT_INFO();
        ae_assert(which < vertex_count, "%u >= %u", (u32)which, (u32)vertex_count);

        const float* const vertex = &((const float* const)
                                    X(buffer)->vertices.data)[which * vertex_size];

        if (t) vec_copy(t, vertex + tex_offset, tex_size);
        if (c) vec_copy(c, vertex + col_offset, col_size);
        if (n) vec_copy(n, vertex + nrm_offset, nrm_size);
        if (p) vec_copy(p, vertex + pos_offset, pos_size);
    }
}

void gl_buffer_set_vertex(gl_buffer_t* buffer, size_t which,
                    float* t, float* c, float* n, float* p)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        GL_BUFFER_TOTAL_VERTEX_FORMAT_INFO();
        assert(!X(buffer)->locked);

        ae_assert(which < vertex_count, "%u >= %u", (u32)which, (u32)vertex_count);
        float * vertex = ((float *)X(buffer)->vertices.data) + which * vertex_size;

        if (t) vec_copy(vertex + tex_offset, t, tex_size);
        if (c) vec_copy(vertex + col_offset, c, col_size);
        if (n) vec_copy(vertex + nrm_offset, n, nrm_size);
        if (p) vec_copy(vertex + pos_offset, p, pos_size);
    }
}

void gl_buffer_append_vertex(gl_buffer_t* buffer, float* t, float* c, float* n, float* p)
{
    /* NOTE: in a VBO environment, this could buffer soft verts and inds until the
     * VBO is actually required for drawing, at which point the data is committed.
     */
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked);

        ae_vertex_format_t f = X(buffer)->vertex_format;
        assert(ae_vertex_format_position_size[f] == 3);

        assert(ae_vertex_format_has_texcoord(f) ? t != NULL : 1);
        assert(ae_vertex_format_has_color(f) ? c != NULL : 1);
        assert(ae_vertex_format_has_normal(f) ? n != NULL : 1);
        assert(ae_vertex_format_has_position(f) ? p != NULL : 1);

        if (ae_likely(X(buffer)->compute_aabbox))
        {
            if (ae_likely(X(buffer)->vertices.size != 0))
            {
                vec3min_vec(X(buffer)->minv, X(buffer)->minv, p);
                vec3max_vec(X(buffer)->maxv, X(buffer)->maxv, p);
            }
            else
            {
                vec3copy(X(buffer)->minv, p);
                vec3copy(X(buffer)->maxv, p);
            }
        }
        else
        {
            vec3zero(X(buffer)->minv);
            vec3zero(X(buffer)->maxv);
        }

        ae_array_reserve(&X(buffer)->vertices, sizeof(float) *
                                    ae_vertex_format_size[f]);

        ae_array_append(&X(buffer)->vertices, t, sizeof(float) *
                            ae_vertex_format_texcoord_size[f]);

        ae_array_append(&X(buffer)->vertices, c, sizeof(float) *
                                ae_vertex_format_color_size[f]);

        ae_array_append(&X(buffer)->vertices, n, sizeof(float) *
                                ae_vertex_format_normal_size[f]);

        ae_array_append(&X(buffer)->vertices, p, sizeof(float) *
                            ae_vertex_format_position_size[f]);
    }
}

void gl_buffer_get_face(gl_buffer_t * buffer, size_t which,
                        size_t *v0, size_t *v1, size_t *v2)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked);

        size_t index_count = static_cast<size_t>(gl_buffer_get_index_count(buffer));
        const size_t tri_index = which * 3;

        ae_index_type_t index_type = X(buffer)->index_type;
        void* indices = X(buffer)->indices.data;

        if (v0) *v0 = idx_value_get(indices, index_type, index_count, tri_index + 0);
        if (v1) *v1 = idx_value_get(indices, index_type, index_count, tri_index + 1);
        if (v2) *v2 = idx_value_get(indices, index_type, index_count, tri_index + 2);
    }
}

void gl_buffer_set_face(gl_buffer_t * buffer, size_t which,
                        size_t v0, size_t v1, size_t v2)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked);

        size_t index_count = static_cast<size_t>(gl_buffer_get_index_count(buffer));
        const size_t tri_index = which * 3;

        ae_index_type_t index_type = X(buffer)->index_type;
        void* indices = X(buffer)->indices.data;

        idx_value_set(indices, index_type, index_count, tri_index + 0, v0);
        idx_value_set(indices, index_type, index_count, tri_index + 1, v1);
        idx_value_set(indices, index_type, index_count, tri_index + 2, v2);
    }
}

void gl_buffer_append_face(gl_buffer_t* buffer, size_t v0, size_t v1, size_t v2)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        /* TODO: if any value exceeds possible type size, expand the index buffer
         */
        ae_index_type_t type = (X(buffer)->index_type);
        ae_array_t * indices = &X(buffer)->indices;

        /* alloc tri bytes
         */
        ae_array_reserve(indices, indices->size + 3 * ae_index_type_bytes[type]);

        /* TODO: switch once instead of three times here
         */
        idx_value_append(indices, type, v0);
        idx_value_append(indices, type, v1);
        idx_value_append(indices, type, v2);
    }
}

// TODO: implement the functional opposite of this (gl_buffer_slice), which
// would create a copy of a buffer's vertices and inds within a given range.

void gl_buffer_merge(gl_buffer_t* dst, gl_buffer_t* src, int copy)
{
    AE_PROFILE_SCOPE();

    ae_if (gl_buffer_get_open(src) && gl_buffer_get_open(dst))
    {
        #define GL_BUFFER_BINARY_VERTEX_FORMAT_INFO(src, dst)                   \
                                                                                \
            ae_array_t* src_vertices = &X(src)->vertices;                       \
            ae_array_t* dst_vertices = &X(dst)->vertices;                       \
                                                                                \
            ae_vertex_format_t src_vertex_format = X(src)->vertex_format;       \
            ae_vertex_format_t dst_vertex_format = X(dst)->vertex_format;       \
                                                                                \
            size_t src_vertex_size = ae_vertex_format_size[src_vertex_format];  \
            size_t dst_vertex_size = ae_vertex_format_size[dst_vertex_format];  \
                                                                                \
            size_t src_vertex_count = X(src)->vertices.size / (                 \
                                sizeof(float) * src_vertex_size);               \
                                                                                \
            size_t dst_vertex_count = X(dst)->vertices.size / (                 \
                                sizeof(float) * dst_vertex_size);               \
                                                                                \
            ae_array_t* src_indices = &X(src)->indices;                         \
            ae_array_t* dst_indices = &X(dst)->indices;                         \
                                                                                \
            ae_index_type_t src_index_type = X(src)->index_type;                \
            ae_index_type_t dst_index_type = X(dst)->index_type;                \
                                                                                \
            size_t src_index_type_bytes = ae_index_type_bytes[src_index_type];  \
            size_t dst_index_type_bytes = ae_index_type_bytes[dst_index_type];  \
                                                                                \
            size_t src_index_count = X(src)->indices.size /                     \
                                    src_index_type_bytes;                       \
                                                                                \
            size_t dst_index_count = X(dst)->indices.size /                     \
                                    dst_index_type_bytes;                       \

        GL_BUFFER_BINARY_VERTEX_FORMAT_INFO(src, dst);

        ae_array_reserve(dst_vertices, dst_vertices->size + // reserve vertices
                            dst_vertex_size * src_vertex_count * sizeof(float));

        ae_if (src_index_count && !dst_index_count)
        {
            dst_index_type = X(dst)->index_type = src_index_type;
            dst_index_type_bytes = src_index_type_bytes;
            dst_index_count = dst_vertex_count;

            ae_array_resize(dst_indices, dst_index_type_bytes * dst_index_count);
            idx_identity(dst_indices->data, dst_index_type, dst_index_count);
        }

        // TODO: ae_array_reserve(dst_indices, dst_indices->size + ?);
        // TODO: fast path if two u16 buffers will still fit into u16

        for (size_t i = 0, src_tris = src_index_count / 3; i < src_tris; i++)
        {
            size_t v0, v1, v2;
            gl_buffer_get_face(src, i, &v0, &v1, &v2);

            v0 += dst_vertex_count;
            v1 += dst_vertex_count;
            v2 += dst_vertex_count;

            gl_buffer_append_face(dst, v0, v1, v2);
        }

        ae_if (dst_index_count && !src_index_count)
        {
            ae_assert(0, "TODO append offset identity indices to dst buffer");
        }
    /*
        ae_if (src_vertex_count && !dst_vertex_count)
        {
            dst_vertex_size = src_vertex_size; // set empty dst buffer format
            dst_vertex_format = X(dst)->vertex_format = src_vertex_format;
        }
    */
        ae_if (src_vertex_format == dst_vertex_format)
        {
            ae_array_append(dst_vertices, src_vertices->data, src_vertices->size);

            if (X(dst)->compute_aabbox)
            {
                if (X(src)->compute_aabbox)
                {
                    vec3min_vec(X(dst)->minv, X(dst)->minv, X(src)->minv);
                    vec3max_vec(X(dst)->maxv, X(dst)->maxv, X(src)->maxv);
                }
                else
                {
                    ae_array_t temp_v, temp_i; // recompute by unlocking
                    gl_buffer_lock(dst, &temp_v, &temp_i);

                    gl_buffer_unlock(dst, &temp_v, X(dst)->vertex_format,
                                            &temp_i, X(dst)->index_type);
                }
            }
        }
        else
        {
            for (size_t i = 0; i < src_vertex_count; i++)
            {
                float tex[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
                float col[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
                float nrm[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
                float pos[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

                gl_buffer_get_vertex(src, i, tex, col, nrm, pos);
                gl_buffer_append_vertex(dst, tex, col, nrm, pos);
            }
        }

        ae_if (!copy)
        {
            gl_buffer_close(src);
        }

        ae_array_trim(&X(dst)->vertices);
        ae_array_trim(&X(dst)->indices);
    }
}

void gl_buffer_get_aabbox(gl_buffer_t* buffer, float* minv, float* maxv)
{
    #if defined(AE_DEBUG)
        AE_PROFILE_NAMED(gl_buffer_get_aabbox_debug);
    #else
        AE_PROFILE_SCOPE();
    #endif

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked);

        #if defined(AE_DEBUG)
        {
            ae_vertex_format_t vertex_format = X(buffer)->vertex_format;

            float r_min[3];
            float r_max[3];

            vtx_aabbox3_ex( r_min, r_max, (float *)X(buffer)->vertices.data,
                            X(buffer)->vertices.size / sizeof(float),

                            ae_vertex_format_position_offset[vertex_format],
                            ae_vertex_format_size[vertex_format]);

            if (vec3cmp_e(r_min, X(buffer)->minv, 0.00001f)  &&
                vec3cmp_e(r_max, X(buffer)->maxv, 0.00001f)) {} else
            {
                if (X(buffer)->compute_aabbox)
                {
                    ae_assert(0, "vtx_aabbox3_ex computed incorrectly");
                }
                else
                {
                    AE_WARN("queried for buffer aabbox (not computed)");
                }
            }
        }
        #endif

        vec3copy(minv, X(buffer)->minv);
        vec3copy(maxv, X(buffer)->maxv);
    }
    else
    {
        AE_WARN("got an empty bounding box from a closed vertex buffer");

        vec3zero(minv);
        vec3zero(maxv);
    }
}

void gl_buffer_translate(gl_buffer_t* buffer, const float v[3])
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        #define GL_POS_TRANSFORM_FUNCTION_SETUP()                                   \
                                                                                    \
            ae_vertex_format_t vertex_format = X(buffer)->vertex_format;            \
            assert(!X(buffer)->locked);                                             \
            assert(ae_vertex_format_position_size[vertex_format] == 3);             \
                                                                                    \
            const size_t size = ae_vertex_format_size[vertex_format]; /* stride */  \
            const size_t vpos = ae_vertex_format_position_offset[vertex_format];    \
                                                                                    \
            ae_array_t* array = &X(buffer)->vertices;                               \

        GL_POS_TRANSFORM_FUNCTION_SETUP();

        if (size == 3)
        {
            // legacy holdover from when this was the only possible SSE vtx path
            vtx_vec3add((float *)(array->data), v, array->size / sizeof(float));
        }
        else
        {
            vtx_vec3add_ex((float *)array->data, v, array->size / sizeof(float),
                                                                    vpos, size);
        }

        ae_if (X(buffer)->compute_aabbox)
        {
            vec3add_vec(X(buffer)->minv, X(buffer)->minv, v);
            vec3add_vec(X(buffer)->maxv, X(buffer)->maxv, v);
        }
    }
}

void gl_buffer_scale(gl_buffer_t* buffer, const float v[3])
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        GL_POS_TRANSFORM_FUNCTION_SETUP();

        if (size == 3)
        {
            // legacy holdover from when this was the only possible SSE vtx path
            vtx_vec3mul((float *)(array->data), v, array->size / sizeof(float));
        }
        else
        {
            vtx_vec3mul_ex((float *)array->data, v, array->size / sizeof(float),
                                                                    vpos, size);
        }

        ae_if (X(buffer)->compute_aabbox)
        {
            vec3mul_vec(X(buffer)->minv, X(buffer)->minv, v);
            vec3mul_vec(X(buffer)->maxv, X(buffer)->maxv, v);
        }
    }
}

void gl_buffer_lerp(gl_buffer_t* buffer, gl_buffer_t* a, gl_buffer_t* b, float t)
{
    AE_STUB();
}

void gl_buffer_transform(gl_buffer_t* buffer, const float m[16], int normalize)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        GL_POS_TRANSFORM_FUNCTION_SETUP(); // setup locals

        ae_array_t lock_v, lock_i; // recompute aabbox
        gl_buffer_lock(buffer, &lock_v, &lock_i);

        // TODO: transform vertex normals and positions in one pass for better cache use
        ae_if (ae_vertex_format_has_normal(vertex_format))
        {
            assert(ae_vertex_format_normal_size[vertex_format] == 3);

            vtx_mat4x4row_mul_vector3_ex((float *)lock_v.data, m, normalize, lock_v.size \
                    / sizeof(float), ae_vertex_format_normal_offset[vertex_format], size);
        }

        vtx_mat4x4row_mul_point3_ex((float*)lock_v.data,
            m, lock_v.size / sizeof(float), vpos, size);

        /* TODO: function to transform aabbox by matrix
         */
        gl_buffer_unlock(buffer, &lock_v, vertex_format,
                        &lock_i, X(buffer)->index_type);
    }
}

void gl_buffer_center_at_origin(gl_buffer_t* buffer)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        float min_v[3], max_v[3], center[3];

        if (X(buffer)->compute_aabbox)
        {
            gl_buffer_get_aabbox(buffer, min_v, max_v);
        }
        else
        {
            ae_vertex_format_t vertex_format = X(buffer)->vertex_format;

            vtx_aabbox3_ex( min_v, max_v, (float *)X(buffer)->vertices.data,
                            X(buffer)->vertices.size / sizeof(float),

                            ae_vertex_format_position_offset[vertex_format],
                            ae_vertex_format_size[vertex_format]);
        }

        aabbox3_get_center(center, min_v, max_v);
        vec3neg(center, center);
        gl_buffer_translate(buffer, center);
    }
}

float gl_buffer_unique_vertex_ratio(gl_buffer_t* buffer, float epsilon)
{
    AE_PROFILE_SCOPE();

    ae_if (gl_buffer_get_open(buffer) && X(buffer)->vertices.size)
    {
        assert(!X(buffer)->locked);

        /* TODO push this down into aecore, use stack
         */
        ae_array_t temp_v = AE_ZERO_STRUCT;
        ae_array_t temp_i = AE_ZERO_STRUCT;

        ae_array_copy(&temp_v, &X(buffer)->vertices);

        vtx_weld( &temp_v, X(buffer)->vertex_format,
            &temp_i, X(buffer)->index_type, epsilon);

        float ratio = (float)temp_v.size / (float)
                            X(buffer)->vertices.size;

        ae_array_free(&temp_v);
        ae_array_free(&temp_i);

        return ratio;
    }

    return 1.0f;
}

void gl_buffer_weld(gl_buffer_t* buffer, float epsilon)
{
    AE_PROFILE_SCOPE();

    ae_if (gl_buffer_get_open(buffer) && X(buffer)->indices.size == 0)
    {
        assert(!X(buffer)->locked);

        vtx_weld( &X(buffer)->vertices, X(buffer)->vertex_format,
            &X(buffer)->indices, X(buffer)->index_type, epsilon);

        // XXX: if a massive epsilon value is passed, vertices can be
        // destroyed, causing the bounding box to possibly shrink.
        // due to the rarity of this, we don't recompute in release.
        #if defined(AE_DEBUG)
        {
            if (X(buffer)->compute_aabbox)
            {
                float minv[3], maxv[3]; // checks aabbox
                gl_buffer_get_aabbox(buffer, minv, maxv);
            }
        }
        #endif
    }
}

void gl_buffer_weld_dist2(gl_buffer_t* buffer, float threshold_sq)
{
    AE_PROFILE_SCOPE();

    ae_if (gl_buffer_get_open(buffer) && X(buffer)->indices.size == 0)
    {
        assert(!X(buffer)->locked);

        vtx_weld_dist2(&X(buffer)->vertices, X(buffer)->vertex_format,
            &X(buffer)->indices, X(buffer)->index_type, threshold_sq);

        #if defined(AE_DEBUG)
        {
            if (X(buffer)->compute_aabbox)
            {
                float minv[3], maxv[3]; // checks aabbox
                gl_buffer_get_aabbox(buffer, minv, maxv);
            }
        }
        #endif
    }
}

void gl_buffer_unweld(gl_buffer_t* buffer)
{
    AE_PROFILE_SCOPE();

    ae_if (gl_buffer_get_open(buffer) && X(buffer)->indices.size != 0)
    {
        assert(!X(buffer)->locked);

        vtx_unweld(&X(buffer)->vertices, X(buffer)->vertex_format,
                    &X(buffer)->indices, X(buffer)->index_type );
    }
}

void gl_buffer_shrink_indices(gl_buffer_t* buffer)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked); // optimize index buffer without re-ordering

        size_t vertex_count = X(buffer)->vertices.size / (sizeof(float) *
                        ae_vertex_format_size[X(buffer)->vertex_format]);

        const size_t index_count = (X(buffer)->indices.size /
                ae_index_type_bytes[X(buffer)->index_type]);

        ae_if (vertex_count <= 256) // pow(2, sizeof(u8) * CHAR_BIT)
        {
            if (X(buffer)->index_type == AE_INDEX_TYPE_U32 ||
                X(buffer)->index_type == AE_INDEX_TYPE_U32)
            {
                assert( idx_smallest_fitting_type(X(buffer)->indices.data, // check
                        X(buffer)->index_type, index_count) == AE_INDEX_TYPE_U8 );

                idx_type_convert(&X(buffer)->indices, /* shrink indices */
                                X(buffer)->index_type, AE_INDEX_TYPE_U8);

                X(buffer)->index_type = AE_INDEX_TYPE_U8;
            }
        }

        else ae_if (vertex_count <= 65536) // pow(2, sizeof(u16) * CHAR_BIT)
        {
            if (X(buffer)->index_type == AE_INDEX_TYPE_U32)
            {
                assert( idx_smallest_fitting_type(X(buffer)->indices.data, // check
                        X(buffer)->index_type, index_count) == AE_INDEX_TYPE_U16);

                idx_type_convert(&X(buffer)->indices, /* shrink indices */
                                X(buffer)->index_type, AE_INDEX_TYPE_U16);

                X(buffer)->index_type = AE_INDEX_TYPE_U16;
            }
        }
    }
}

void gl_buffer_vertex_cache_optimize(gl_buffer_t* buffer)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked);

        size_t vertex_count = X(buffer)->vertices.size / (sizeof(float) *
                        ae_vertex_format_size[X(buffer)->vertex_format]);

        const size_t index_count = (X(buffer)->indices.size /
                ae_index_type_bytes[X(buffer)->index_type]);

        idx_vertex_cache_optimize_ex(X(buffer)->indices.data,
            X(buffer)->index_type, index_count, vertex_count);
    }
}

double gl_buffer_vertex_cache_miss_ratio(gl_buffer_t* buffer,
                                    int cache_size, int FIFO)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        assert(!X(buffer)->locked);

        const size_t index_count = (X(buffer)->indices.size /
                ae_index_type_bytes[X(buffer)->index_type]);

        return idx_vertex_cache_miss_ratio( X(buffer)->indices.data,
                                            X(buffer)->index_type,
                                    index_count, cache_size, FIFO);
    }

    return 0.0;
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
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        // TODO: if compute_aabbox, check the ray against the aabbox first.
        // TODO: compute shader implementation with vertex buffer objects?
        GL_POS_TRANSFORM_FUNCTION_SETUP();

        assert(ray_pos && ray_dir && hit_indices && hit_point && t && u && v);

        int hit = tri3_mesh_vs_ray_ex(
            (float*)ray_pos,
            (float*)ray_dir,
            (float*)array->data,
            array->size / sizeof(float),
            vpos,
            size,
            X(buffer)->indices.data,
            X(buffer)->index_type,
            X(buffer)->indices.size / ae_index_type_bytes[X(buffer)->index_type],
            hit_indices,
            t, u, v,
            epsilon);

        if (hit)
        {
            tri3_ray_hit_point(
                hit_point,
                ray_pos,
                ray_dir,
                &((const float*)array->data)[hit_indices[0] * size + vpos],
                &((const float*)array->data)[hit_indices[1] * size + vpos],
                &((const float*)array->data)[hit_indices[2] * size + vpos],
                *t, *u, *v);
        }

        return hit;
    }

    return 0;
}

void gl_buffer_depthsort(gl_buffer_t* buffer, const float p[3])
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        GL_POS_TRANSFORM_FUNCTION_SETUP();

        tri3_mesh_depthsort_ex(p, (float*)array->data, array->size / sizeof(float),
            vpos, size, X(buffer)->indices.data, X(buffer)->index_type,
            X(buffer)->indices.size / ae_index_type_bytes[X(buffer)->index_type]);
    }
}

void gl_buffer_invert_faces(gl_buffer_t* buffer)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_buffer_get_open(buffer)))
    {
        GL_POS_TRANSFORM_FUNCTION_SETUP();

        tri3_mesh_invert_ex( (float *) array->data, array->size / sizeof( float ),
            size, X(buffer)->indices.data, X(buffer)->index_type,
            X(buffer)->indices.size / ae_index_type_bytes[X(buffer)->index_type]);
    }
}

static GLenum ae_index_type_to_opengl(ae_index_type_t type)
{
    ae_switch (type, ae_index_type, AE_INDEX_TYPE, suffix)
    {
        case AE_INDEX_TYPE_U8: return GL_UNSIGNED_BYTE;
        case AE_INDEX_TYPE_U16: return GL_UNSIGNED_SHORT;
        case AE_INDEX_TYPE_U32: return GL_UNSIGNED_INT;

        default:
        {
            ae_assert(0, "%s", ae_index_type_name[type]);
            return 0;
        }
        break;
    }
}

static GLenum ae_vertex_format_to_opengl(ae_vertex_format_t format)
{
    ae_switch (format, ae_vertex_format, AE_VERTEX_FORMAT, suffix)
    {
        case AE_T2F_N3F_V3F: return GL_T2F_N3F_V3F; // most common

        case AE_T2F_V3F: return GL_T2F_V3F;
        case AE_C3F_V3F: return GL_C3F_V3F;
        case AE_N3F_V3F: return GL_N3F_V3F;
        case AE_T4F_V4F: return GL_T4F_V4F;

        case AE_V2F: return GL_V2F;
        case AE_V3F: return GL_V3F;

        case AE_T2F_C3F_V3F: return GL_T2F_C3F_V3F;
        case AE_C4F_N3F_V3F: return GL_C4F_N3F_V3F;

        case AE_T2F_C4F_N3F_V3F: return GL_T2F_C4F_N3F_V3F;

        default:
        {
            ae_assert(0, "%s", ae_vertex_format_name[format]);
            return 0;
        }
        break;
    }
}

void gl_buffer_draw_ex(gl_buffer_t* buffer, gl_material_t* material)
{
    AE_PROFILE_SCOPE();
    gl_init();

    if (gl_buffer_get_open(buffer))
    {
        assert(!X(buffer)->locked);

        GL_PushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
        GL_PushAttrib(GL_POLYGON_BIT | GL_LIGHTING_BIT);

        gl_material_apply_ex(material, buffer);

        if (X(buffer)->line_mode)
        {
            GL_PolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        if (!X(buffer)->smooth_shading)
        {
            GL_ShadeModel(GL_FLAT);
        }

        GLenum gl_vertex_format = ae_vertex_format_to_opengl(X(buffer)->vertex_format);
        GL_InterleavedArrays(gl_vertex_format, 0, X(buffer)->vertices.data);

        ae_if (X(buffer)->indices.size)
        {
            GLenum gl_index_type = ae_index_type_to_opengl(X(buffer)->index_type);

            GLsizei index_count = X(buffer)->indices.size /
                ae_index_type_bytes[X(buffer)->index_type];

            assert(X(buffer)->draw_mode == GL_TRIANGLES ? \
                                index_count % 3 == 0 : 1);

            GL_DrawElements(X(buffer)->draw_mode, index_count, gl_index_type,
                                                    X(buffer)->indices.data);
        }
        else
        {
            GLsizei vertex_count = X(buffer)->vertices.size / (sizeof(float) *
                            ae_vertex_format_size[X(buffer)->vertex_format]);

            assert(X(buffer)->draw_mode == GL_TRIANGLES ? \
                                vertex_count % 3 == 0 : 1);

            GL_DrawArrays(X(buffer)->draw_mode, 0, vertex_count);
        }

        GL_PopAttrib();
        GL_PopClientAttrib();

        // TODO: GL_DEBUG, draw these only if it's #defined, color/enabled properties
        #if 0
        {
            static const u32 s = ae_random_bounded_u32(AE_COLOR_COUNT);
            static const u32 e = ae_random_bounded_u32(AE_COLOR_COUNT);

            gl_buffer_draw_normals_ex(buffer, ae_color_flt[s], ae_color_flt[e], 1.0f);
        }
        #endif
    }

    gl_debug_log_error_state();
}

void gl_buffer_draw(gl_buffer_t* buffer)
{
    gl_buffer_draw_ex(buffer, NULL);
}

void gl_buffer_draw_normals_ex(gl_buffer_t* buffer, const float* start_color,
                                    const float* end_color, float line_width)
{
    AE_PROFILE_SCOPE();
    gl_init();

    ae_vertex_format_t format = static_cast<ae_vertex_format_t>(
                            gl_buffer_get_vertex_format(buffer));

    if (ae_vertex_format_has_normal(format))
    {
        assert(!X(buffer)->locked);

        GL_PushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LINE_BIT);
        {
            GL_Disable(GL_TEXTURE_2D);
            GL_Disable(GL_LIGHTING);

            GL_LineWidth(line_width);

            GL_Enable(GL_BLEND);
            GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            GL_Begin(GL_LINES);
            {
                const int num_verts = gl_buffer_get_vertex_count(buffer);
                const int vert_size = (int)ae_vertex_format_size[format];

                size_t p_offset = ae_vertex_format_position_offset[format];
                size_t n_offset = ae_vertex_format_normal_offset[format];

                for (int i = 0; i < num_verts * vert_size; i += vert_size)
                {
                    float* v = ((float*)X(buffer)->vertices.data) + i;

                    float* n = v + n_offset;
                    float* p = v + p_offset;

                    GL_Color4fv(start_color);
                    GL_Vertex3fv(p);

                    GL_Color4fv(end_color);
                    GL_Vertex3f(p[0] + n[0], p[1] + n[1], p[2] + n[2]);
                }
            }
            GL_End();
        }
        GL_PopAttrib();
    }

    gl_debug_log_error_state();
}

void gl_buffer_draw_normals(gl_buffer_t* buffer, const float* rgba)
{
    gl_buffer_draw_normals_ex(buffer, rgba, rgba, 1.0f);
}

#undef X

/* ===== [ mesh generation ] ================================================ */

#define PAR_PRINT(msg)          ae_log(OPENGL, "%s", msg)

#define PAR_PI                  M_PI /* TODO: redefine sin and cos */
#define PAR_MIN(a, b)           ((a) > (b) ? (b) : (a))
#define PAR_MAX(a, b)           ((a) > (b) ? (a) : (b))
#define PAR_SWAP(T, A, B)       { T tmp = B; B = A; A = tmp; }

#define PAR_MALLOC(T, N)        ((T*)ae_malloc((N) * sizeof(T)))
#define PAR_CALLOC(T, N)        ((T*)ae_calloc((N) * sizeof(T), 1))
#define PAR_REALLOC(T, P, N)    ((T*)ae_realloc((P), sizeof(T) * (N)))
#define PAR_FREE(P)             ae_free(P)

#define PAR_SHAPES_IMPLEMENTATION
#include <par_shapes.h>

static gl_buffer_t* gl_buffer_from_par_mesh(par_shapes_mesh* mesh)
{
    AE_PROFILE_SCOPE();

    gl_buffer_t* buffer = gl_buffer_create_empty(); // find format from mesh
    gl_internal_buffer_t* data = (gl_internal_buffer_t*)buffer;

    data->vertex_format = ae_vertex_format_from_sizes(mesh->tcoords ? 2 : 0,
                                                0, mesh->normals ? 3 : 0, 3);
    data->index_type = AE_INDEX_TYPE_U16;

    float* t = mesh->tcoords;
    float* n = mesh->normals;
    float* p = mesh->points;

    ae_array_reserve(&data->vertices, mesh->npoints * // avoid over-allocation
                    ae_vertex_format_size[data->vertex_format] * sizeof(float));

    for (int i = 0; i < mesh->npoints; i++, t += 2, n += 3, p += 3)
    {
        /* TODO: this is terribly inefficient - the first step is to allocate
         * the buffer up front and switch on vertex format, the second is to
         * consume the par_shapes_mesh directly for indices. this means we have
         * to know if array memory is allocated with ae_malloc - maybe define
         * a hackish function like ae_array_uses_global_malloc or something...
         */
        gl_buffer_append_vertex(buffer, t, NULL, n, p);
    }

    assert( data->vertices.size == mesh->npoints * ae_vertex_format_size[
                                    data->vertex_format] * sizeof(float));

    if (mesh->npoints > 256) // shrink indices if possible, otherwise copy
    {
        ae_array_append(&data->indices, mesh->triangles,
                    mesh->ntriangles * 3 * sizeof(u16));
    }
    else
    {
        const ae_index_type_t s = AE_INDEX_TYPE_U16;
        const ae_index_type_t d = AE_INDEX_TYPE_U8;

        const int c = mesh->ntriangles * 3;
        assert(idx_fits_type(mesh->triangles, c, s, d));

        ae_array_resize(&data->indices, c);
        data->index_type = d;

        idx_type_converted_ex(mesh->triangles, s, data->indices.data, d, c);
    }

    par_shapes_free_mesh(mesh);
    return buffer;
}

#define X(c) AE_PROFILE_SCOPE(); return gl_buffer_from_par_mesh(c)

gl_buffer_t* gl_buffer_create_cylinder(int slices, int stacks)
{
    X(par_shapes_create_cylinder(slices, stacks));
}

gl_buffer_t* gl_buffer_create_torus(int slices, int stacks, float radius)
{
    X(par_shapes_create_torus(slices, stacks, radius));
}

gl_buffer_t* gl_buffer_create_parametric_sphere(int slices, int stacks)
{
    X(par_shapes_create_parametric_sphere(slices, stacks));
}

gl_buffer_t* gl_buffer_create_subdivided_sphere(int nsubdivisions)
{
    X(par_shapes_create_subdivided_sphere(nsubdivisions));
}

gl_buffer_t* gl_buffer_create_klein_bottle(int slices, int stacks)
{
    X(par_shapes_create_klein_bottle(slices, stacks));
}

gl_buffer_t* gl_buffer_create_trefoil_knot(int slices, int stacks, float radius)
{
    X(par_shapes_create_trefoil_knot(slices, stacks, radius));
}

gl_buffer_t* gl_buffer_create_hemisphere(int slices, int stacks)
{
    X(par_shapes_create_hemisphere(slices, stacks));
}

gl_buffer_t* gl_buffer_create_plane(int slices, int stacks)
{
    X(par_shapes_create_plane(slices, stacks));
}

gl_buffer_t* gl_buffer_create_parametric_shape(void (*func)(float const* uv,
            float* xyz, void* data), int slices, int stacks, void* userdata)
{
    X(par_shapes_create_parametric(func, slices, stacks, userdata));
}

gl_buffer_t* gl_buffer_create_icosahedron(void)
{
    X(par_shapes_create_icosahedron());
}

gl_buffer_t* gl_buffer_create_dodecahedron(void)
{
    X(par_shapes_create_dodecahedron());
}

gl_buffer_t* gl_buffer_create_octahedron(void)
{
    X(par_shapes_create_octahedron());
}

gl_buffer_t* gl_buffer_create_tetrahedron(void)
{
    X(par_shapes_create_tetrahedron());
}

gl_buffer_t* gl_buffer_create_cube(void)
{
    // FIXME: this function doesn't generate texture coordinates!
    X(par_shapes_create_cube());
}

gl_buffer_t* gl_buffer_create_disk( float radius, int slices,
                    float const* center, float const* normal)
{
    X(par_shapes_create_disk(radius, slices, center, normal));
}

gl_buffer_t* gl_buffer_create_rock(int seed, int nsubdivisions)
{
    X(par_shapes_create_rock(seed, nsubdivisions));
}

gl_buffer_t* gl_buffer_create_lsystem(char const* program, int slices, int maxdepth)
{
    X(par_shapes_create_lsystem(program, slices, maxdepth));
}

#undef X

/*
================================================================================
 * ~~ [ particle system ] ~~ *
--------------------------------------------------------------------------------
TODO: (un)lock --- should we interleave arrays in case we add a color component?
TODO: particle emitter auto-update mode similar to xl clock? (ae frame callback)
--------------------------------------------------------------------------------
*/

struct gl_internal_particle_emitter_t
{
    double time_created;
    int id, locked;
    const char* name;

    gl_texture_t* texture;
    int owns_texture;
    float color[4];

    int frozen, burst_size;
    float gravity[3];
    double period, time;

    gl_particle_emitter_velgen_mode_t velgen_mode;
    float velgen_scale;

    int num_particles;
    int max_particles;
    int current_index;

    float min_point_size;
    float max_point_size;

    ae_array_t particles;
    ae_array_t vel_array;

    float position[3];
    float position_sigma[3];
    float velocity[3];
    float velocity_sigma[3];

    gl_particle_emitter_t* wrap()
    {
        return static_cast<gl_particle_emitter_t*>(this);
    }
};

#define X(p) ((gl_internal_particle_emitter_t*)(p))

static int gl_particle_emitter_compare(const void* a, const void* b)
{
    /* TODO: attempt lexicographical name sort before comparing age?
     */
    if (X(a)->time_created < X(b)->time_created) return -1;
    if (X(a)->time_created > X(b)->time_created) return +1;

    return 0;
}

void
gl_particle_emitter_set_int(gl_particle_emitter_t* emitter, // integer
                            gl_particle_emitter_property_t property,
                            int value)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_OWNS_TEXTURE:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->owns_texture = value;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_VELGEN_MODE:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->velgen_mode = \
                                        (gl_particle_emitter_velgen_mode_t)value;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_FROZEN:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->frozen = value;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_BURST_SIZE:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->burst_size = value;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_NUM_PARTICLES:
        {
            if (gl_particle_emitter_get_open(emitter))
            {
                X(emitter)->num_particles = ae_imin(X(emitter)->max_particles, value);
            }
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_MAX_PARTICLES:
        {
            if (gl_particle_emitter_get_open(emitter))
            {
                const size_t floats = value * 3;
                const size_t bytes = floats * sizeof(float);

                ae_array_resize(&X(emitter)->particles, bytes);
                ae_array_resize(&X(emitter)->vel_array, bytes);

                X(emitter)->num_particles = ae_imin(X(emitter)->num_particles, value);
                X(emitter)->current_index = ae_imin(X(emitter)->current_index, value);

                X(emitter)->max_particles = value;
            }
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_OPEN:
        {
            if (value == 0 && gl_particle_emitter_get_open(emitter))
            {
                AE_PROFILE_NAMED(gl_particle_emitter_close);
                assert(!X(emitter)->locked);

                ae_if (X(emitter)->owns_texture)
                {
                    gl_texture_close(X(emitter)->texture);
                }

                ae_string_free(const_cast<char*>(X(emitter)->name));

                ae_array_free(&X(emitter)->particles);
                ae_array_free(&X(emitter)->vel_array);

                X(emitter)->~gl_internal_particle_emitter_t();

                ae_ptrset_remove(&gl_particle_emitter_lst, emitter);
                ae_free(emitter);
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }
}

int
gl_particle_emitter_get_int(gl_particle_emitter_t* emitter, // integer
                            gl_particle_emitter_property_t property)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_TOTAL:
        {
            return gl_particle_emitter_lst.count;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_ID:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->id;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_TEXTURE:
        {
            return gl_texture_get_id(gl_particle_emitter_get_texture(emitter));
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_OWNS_TEXTURE:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->owns_texture;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_VELGEN_MODE:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->velgen_mode;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_FROZEN:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->frozen;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_BURST_SIZE:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->burst_size;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_NUM_PARTICLES:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->num_particles;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_MAX_PARTICLES:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->max_particles;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_OPEN:
        {
            return gl_is_init() && ae_ptrset_contains(&gl_particle_emitter_lst, emitter);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0;
}

void
gl_particle_emitter_set_str(gl_particle_emitter_t* emitter, // string
                            gl_particle_emitter_property_t property,
                            const char* value)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_VELGEN_MODE:
        {
            gl_particle_emitter_set_velgen_mode(emitter, // TODO: avoid looping if closed
                                gl_particle_emitter_velgen_mode_from_short_name(value));
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_NAME:
        {
            if (gl_particle_emitter_get_open(emitter))
            {
                ae_string_free(const_cast<char*>(X(emitter)->name));
                X(emitter)->name = NULL;

                if (value != NULL)
                {
                    X(emitter)->name = ae_string_copy(const_cast<char*>(value));
                }
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }
}

const char*
gl_particle_emitter_get_str(gl_particle_emitter_t* emitter, // string
                            gl_particle_emitter_property_t property)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_VELGEN_MODE:
        {
            if (gl_particle_emitter_get_open(emitter))
            {
                return gl_particle_emitter_velgen_mode_short_name[X(emitter)->velgen_mode];
            }
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_STATUS:
        {
            // TODO: add more information here (name, num particles, max particles)
            return gl_particle_emitter_get_open(emitter) ? "open" : "closed";
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_NAME:
        {
            // TODO: if the emitter doesn't have a name, return the texture's name
            if (gl_particle_emitter_get_open(emitter) && X(emitter)->name != NULL)
            {
                return X(emitter)->name;
            }
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }

    return "";
}

void
gl_particle_emitter_set_vec(gl_particle_emitter_t* emitter, // vector
                            gl_particle_emitter_property_t property,
                            float* value)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_COLOR:
        {
            if (gl_particle_emitter_get_open(emitter)) vec4copy(X(emitter)->color, value);
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_POSITION:
        {
            if (gl_particle_emitter_get_open(emitter)) vec3copy(X(emitter)->position, value);
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_POSITION_SIGMA:
        {
            if (gl_particle_emitter_get_open(emitter)) vec3copy(X(emitter)->position_sigma, value);
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_VELOCITY:
        {
            if (gl_particle_emitter_get_open(emitter)) vec3copy(X(emitter)->velocity, value);
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_VELOCITY_SIGMA:
        {
            if (gl_particle_emitter_get_open(emitter)) vec3copy(X(emitter)->velocity_sigma, value);
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_GRAVITY:
        {
            if (gl_particle_emitter_get_open(emitter)) vec3copy(X(emitter)->gravity, value);
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }
}

float*
gl_particle_emitter_get_vec(gl_particle_emitter_t* emitter, // vector
                            gl_particle_emitter_property_t property)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_COLOR:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->color;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_POSITION:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->position;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_POSITION_SIGMA:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->position_sigma;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_VELOCITY:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->velocity;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_VELOCITY_SIGMA:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->velocity_sigma;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_GRAVITY:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->gravity;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }

    static const float zero[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    return const_cast<float*>(zero);
}

void
gl_particle_emitter_set_flt(gl_particle_emitter_t* emitter, // float
                            gl_particle_emitter_property_t property,
                            float value)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_VELGEN_SCALE:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->velgen_scale = value;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_MIN_POINT_SIZE:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->min_point_size = value;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_MAX_POINT_SIZE:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->max_point_size = value;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }
}

float
gl_particle_emitter_get_flt(gl_particle_emitter_t* emitter, // float
                            gl_particle_emitter_property_t property)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_VELGEN_SCALE:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->velgen_scale;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_MIN_POINT_SIZE:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->min_point_size;
        }
        break;

        case GL_PARTICLE_EMITTER_PROPERTY_MAX_POINT_SIZE:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->max_point_size;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0.0f;
}

void
gl_particle_emitter_set_dbl(gl_particle_emitter_t* emitter, // double
                            gl_particle_emitter_property_t property,
                            double value)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_PERIOD:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->period = value;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }
}

double
gl_particle_emitter_get_dbl(gl_particle_emitter_t* emitter, // double
                            gl_particle_emitter_property_t property)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_PERIOD:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->period;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }

    return 0.0;
}

void
gl_particle_emitter_set_tex(gl_particle_emitter_t* emitter, // texture
                            gl_particle_emitter_property_t property,
                            gl_texture_t* value)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_TEXTURE:
        {
            if (gl_particle_emitter_get_open(emitter)) X(emitter)->texture = value;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }
}

gl_texture_t*
gl_particle_emitter_get_tex(gl_particle_emitter_t* emitter, // texture
                            gl_particle_emitter_property_t property)
{
    ae_switch (property, gl_particle_emitter_property, GL_PARTICLE_EMITTER_PROPERTY, suffix)
    {
        case GL_PARTICLE_EMITTER_PROPERTY_TEXTURE:
        {
            if (gl_particle_emitter_get_open(emitter)) return X(emitter)->texture;
        }
        break;

        default:
        {
            AE_WARN("%s in %s", gl_particle_emitter_property_name[property], __FUNCTION__);
        }
        break;
    }

    return NULL;
}

gl_particle_emitter_t* gl_particle_emitter_create(void)
{
    AE_PROFILE_SCOPE();
    gl_init();

    ae_assert(gl_particle_emitter_count_all() <= GL_OBJECT_COUNT_LIMIT,
                    "hit particle emitter object count limit - leak?");

    gl_internal_particle_emitter_t* data = ae_create(gl_internal_particle_emitter_t, clear);
    gl_particle_emitter_t* emitter = data->wrap();

    data->time_created = ae_seconds();
    data->id = (int)ae_random_u32();

    data->color[0] = data->color[1] = // white
    data->color[2] = data->color[3] = 1.0f;

    data->velgen_mode = GL_PARTICLE_EMITTER_VELGEN_MODE_NONE;
    data->velgen_scale = 1.0f;

    data->burst_size = 1;
    data->period = 0.1;

    data->min_point_size = 0.0f;
    data->max_point_size = 2.0f;

    // call constructor, which calls member object constructors
    new (data) gl_internal_particle_emitter_t;

    if (ae_ptrset_add(&gl_particle_emitter_lst, data) == 0)
    {
        AE_WARN("particle emitter is not new to the set (is set code stubbed?)");
    }

    return emitter;
}

void gl_particle_emitter_lock(gl_particle_emitter_t* emitter,
                ae_array_t* vel_array, ae_array_t* particles)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        ae_assert(!X(emitter)->locked, "particle emitter is already locked!");

        assert(particles); *particles = X(emitter)->particles;
        memset(&X(emitter)->particles, 0, sizeof(ae_array_t));

        assert(vel_array); *vel_array = X(emitter)->vel_array;
        memset(&X(emitter)->vel_array, 0, sizeof(ae_array_t));

        const size_t num_bytes = X(emitter)->num_particles * sizeof(float[3]);

        ae_array_resize(particles, num_bytes);
        ae_array_resize(vel_array, num_bytes);

        X(emitter)->locked = 1;
    }
}

void gl_particle_emitter_unlock(gl_particle_emitter_t* emitter,
                ae_array_t* vel_array, ae_array_t* particles)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        // TODO: assert particle and velocity array sizes match with each other
        // TODO: set new (num/max)_particles members if the array sizes change
        ae_assert(X(emitter)->locked, "particle emitter is already unlocked!");

        const size_t num_bytes = X(emitter)->max_particles * sizeof(float[3]);

        ae_array_resize(particles, num_bytes);
        ae_array_resize(vel_array, num_bytes);

        assert(particles); X(emitter)->particles = *particles;
        memset(particles, 0, sizeof(ae_array_t));

        assert(vel_array); X(emitter)->vel_array = *vel_array;
        memset(vel_array, 0, sizeof(ae_array_t));

        X(emitter)->locked = 0;
    }
}

void gl_particle_emitter_get(gl_particle_emitter_t* emitter,
                    size_t which, float * vel, float * pos)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        ae_assert((int)which < X(emitter)->num_particles, "idx %i >= max %i",
                                    (int)which, X(emitter)->num_particles);

        assert(!X(emitter)->locked);

        vec3copy(vel, (float*)X(emitter)->vel_array.data + which * 3);
        vec3copy(pos, (float*)X(emitter)->particles.data + which * 3);
    }
    else
    {
        AE_WARN("tried to get a particle from a closed emitter");

        vec3zero(vel);
        vec3zero(pos);
    }
}

void gl_particle_emitter_set(gl_particle_emitter_t* emitter,
                    size_t which, float * vel, float * pos)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        ae_assert((int)which < X(emitter)->num_particles, "idx %i >= max %i",
                                    (int)which, X(emitter)->num_particles);

        assert(!X(emitter)->locked);

        vec3copy((float*)X(emitter)->vel_array.data + which * 3, vel);
        vec3copy((float*)X(emitter)->particles.data + which * 3, pos);
    }
    else
    {
        AE_WARN("tried to set a particle on a closed emitter");
    }
}

void gl_particle_emitter_append(gl_particle_emitter_t * emitter,
                            float* vel, float* pos, int expand)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        ae_if (X(emitter)->num_particles == X(emitter)->max_particles)
        {
            ae_if (expand)
            {
                gl_particle_emitter_set_max_particles(emitter,
                                X(emitter)->max_particles+1);

                goto copy_and_increment_num_particles;
            }
            else
            {
                ae_assert(0, "TODO: copy particle to current index?");
            }
        }
        else
        {
            copy_and_increment_num_particles:
            {
                vec3copy((float *)X(emitter)->vel_array.data +
                        X(emitter)->num_particles * 3, vel);

                vec3copy((float *)X(emitter)->particles.data +
                        X(emitter)->num_particles * 3, pos);

                X(emitter)->num_particles++;
            }
        }
    }
}

// TODO: implement the functional opposite of this (gl_particle_emitter_slice), which
// would create a copy of a particle emitter's particles & vels within a given range.

void gl_particle_emitter_merge(gl_particle_emitter_t* dst,
                    gl_particle_emitter_t* src, int copy)
{
    AE_PROFILE_SCOPE();

    ae_if (gl_particle_emitter_get_open(dst) && gl_particle_emitter_get_open(src))
    {
        for (int i = 0; i < X(src)->num_particles; i++)
        {
            float vel[3], pos[3]; // TODO: faster path!!!

            gl_particle_emitter_get(src, i, vel, pos);
            gl_particle_emitter_append(dst, vel, pos, 1);
        }

        ae_if (!copy)
        {
            gl_particle_emitter_close(src);
        }
    }
}

void
gl_particle_emitter_get_aabbox(gl_particle_emitter_t* emitter, float* minv, float* maxv)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        vtx_aabbox3(minv, maxv, (float*)X(emitter)->particles.data,
                                    X(emitter)->num_particles * 3);
    }
    else
    {
        AE_WARN("got an empty bounding box from a closed emitter");

        vec3zero(minv);
        vec3zero(maxv);
    }
}

void gl_particle_emitter_translate(gl_particle_emitter_t* emitter, float* xyz)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        vtx_vec3add(static_cast<float*>(X(emitter)->particles.data),
                                xyz, X(emitter)->num_particles * 3);
    }
}

void gl_particle_emitter_scale(gl_particle_emitter_t* emitter, float* xyz)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        vtx_vec3mul(static_cast<float*>(X(emitter)->particles.data),
                                xyz, X(emitter)->num_particles * 3);
    }
}

void gl_particle_emitter_lerp(gl_particle_emitter_t* dst_emitter,
    gl_particle_emitter_t* a, gl_particle_emitter_t* b, float t)
{
    AE_STUB();
}

void gl_particle_emitter_transform(gl_particle_emitter_t* emitter, float* matrix)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        vtx_mat4x4row_mul_point3(static_cast<float*>(X(emitter)->particles.data),
                                            matrix, X(emitter)->num_particles*3);
    }
}

void gl_particle_emitter_center_at_origin(gl_particle_emitter_t* emitter)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        float minv[3], maxv[3], center[3];
        gl_particle_emitter_get_aabbox(emitter, minv, maxv);

        aabbox3_get_center(center, minv, maxv);
        vec3neg(center, center);
        gl_particle_emitter_translate(emitter, center);
    }
}

gl_particle_emitter_velgen_mode_t
gl_particle_emitter_velgen_mode_from_short_name(const char* name)
{
    // TODO: this could be sped up with a hashtable (short name -> index)
    AE_PROFILE_SCOPE();

    size_t i = 0, n = GL_PARTICLE_EMITTER_VELGEN_MODE_COUNT;
    for (; i < n; i++)
    {
        if (!strcmp(gl_particle_emitter_velgen_mode_short_name[i], name))
        {
            return (gl_particle_emitter_velgen_mode_t)i;
        }
    }

    ae_assert(0, "\"%s\" is not a valid velgen mode", name);
    return GL_PARTICLE_EMITTER_VELGEN_MODE_COUNT;
}

void gl_particle_emitter_aabbox_limit(gl_particle_emitter_t* emitter,
                                            float* minv, float* maxv)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        /* TODO: ae_particle/pfx_aabbox3_limit, SSE path? (it's slow)
         */
        float* ptr = static_cast<float*>(X(emitter)->particles.data);
        float* end = ptr + X(emitter)->num_particles * 3;

        // select a point to return to, either emitter pos or center
        float origin[3];

        if (aabbox3_vs_point(minv, maxv, X(emitter)->position))
        {
            vec3copy(origin, X(emitter)->position);
        }
        else
        {
            aabbox3_get_center(origin, minv, maxv);
        }

        for (; ptr < end; ptr += 3)
        {
            if( ptr[0] < minv[0] ||
                ptr[1] < minv[1] ||
                ptr[2] < minv[2] ||
                ptr[0] > maxv[0] ||
                ptr[1] > maxv[1] ||
                ptr[2] > maxv[2] )
            {
                vec3copy(ptr, origin);
            }
        }
    }
}

void gl_particle_emitter_aabbox_clamp(gl_particle_emitter_t* emitter,
                                            float* minv, float* maxv)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        /* TODO: move this function to ae_particle/pfx_aabbox3_clamp
         */
        float* ptr = static_cast<float*>(X(emitter)->particles.data);
        float* end = ptr + X(emitter)->num_particles * 3;

        #if defined(__SSE__)
        if(ae_cpuinfo_sse())
        {
            if (ae_likely(num_is_aligned((size_t)ptr, 16))) // packed main loop unrolling
            {
                const size_t count = X(emitter)->num_particles;
                float *unr = ptr + (count - (count % 12));

                const __m128 min0 = _mm_setr_ps(minv[0], minv[1], minv[2], minv[0]);
                const __m128 min1 = _mm_setr_ps(minv[1], minv[2], minv[0], minv[1]);
                const __m128 min2 = _mm_setr_ps(minv[2], minv[0], minv[1], minv[2]);

                const __m128 max0 = _mm_setr_ps(maxv[0], maxv[1], maxv[2], maxv[0]);
                const __m128 max1 = _mm_setr_ps(maxv[1], maxv[2], maxv[0], maxv[1]);
                const __m128 max2 = _mm_setr_ps(maxv[2], maxv[0], maxv[1], maxv[2]);

                for (; ptr < unr; ptr += 12)
                {
                    _mm_store_ps(ptr + 0, _mm_clamp_ps(_mm_load_ps(ptr + 0), min0, max0));
                    _mm_store_ps(ptr + 4, _mm_clamp_ps(_mm_load_ps(ptr + 4), min1, max1));
                    _mm_store_ps(ptr + 8, _mm_clamp_ps(_mm_load_ps(ptr + 8), min2, max2));
                }
            }

            if (1) // unaligned / leftover path
            {
                const __m128 min0 = _mm_set_ss(minv[0]);
                const __m128 min1 = _mm_set_ss(minv[1]);
                const __m128 min2 = _mm_set_ss(minv[2]);

                const __m128 max0 = _mm_set_ss(maxv[0]);
                const __m128 max1 = _mm_set_ss(maxv[1]);
                const __m128 max2 = _mm_set_ss(maxv[2]);

                for (; ptr < end; ptr += 3)
                {
                    _mm_store_ss(ptr + 0, _mm_min_ss(_mm_max_ss(
                                _mm_set_ss(ptr[0]), min0), max0));

                    _mm_store_ss(ptr + 1, _mm_min_ss(_mm_max_ss(
                                _mm_set_ss(ptr[1]), min1), max1));

                    _mm_store_ss(ptr + 2, _mm_min_ss(_mm_max_ss(
                                _mm_set_ss(ptr[2]), min2), max2));
                }
            }
        }
        #endif

        for (; ptr < end; ptr += 3)
        {
            vec3clamp_vec(ptr, ptr, minv, maxv);
        }
    }
}

void gl_particle_emitter_aabbox_reset(gl_particle_emitter_t* emitter,
                                            float* minv, float* maxv)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        float center[3]; // get center for extent vector
        aabbox3_get_center(center, minv, maxv);

        float extent[3] =
        {
            ae_fabsf(maxv[0] - center[0]),
            ae_fabsf(maxv[1] - center[1]),
            ae_fabsf(maxv[2] - center[2]),
        };

        X(emitter)->num_particles = X(emitter)->max_particles;

        float* pos = (float*)X(emitter)->particles.data;
        float* end = pos + X(emitter)->num_particles * 3;
        float* vel = (float*)X(emitter)->vel_array.data;

        #define SIDE_UP                                             \
            pos[0] = center[0] + extent[0] * ae_random_bi_flt();    \
            pos[1] = center[1] + extent[1];                         \
            pos[2] = center[2] + extent[2] * ae_random_bi_flt();    \

        #define SIDE_DN                                             \
            pos[0] = center[0] + extent[0] * ae_random_bi_flt();    \
            pos[1] = center[1] - extent[1];                         \
            pos[2] = center[2] + extent[2] * ae_random_bi_flt();    \

        #define SIDE_BK                                             \
            pos[0] = center[0] + extent[0] * ae_random_bi_flt();    \
            pos[1] = center[1] + extent[1] * ae_random_bi_flt();    \
            pos[2] = center[2] + extent[2];                         \

        #define SIDE_FT                                             \
            pos[0] = center[0] + extent[0] * ae_random_bi_flt();    \
            pos[1] = center[1] + extent[1] * ae_random_bi_flt();    \
            pos[2] = center[2] - extent[2];                         \

        #define SIDE_RT                                             \
            pos[0] = center[0] + extent[0];                         \
            pos[1] = center[1] + extent[1] * ae_random_bi_flt();    \
            pos[2] = center[2] + extent[2] * ae_random_bi_flt();    \

        #define SIDE_LF                                             \
            pos[0] = center[0] - extent[0];                         \
            pos[1] = center[1] + extent[1] * ae_random_bi_flt();    \
            pos[2] = center[2] + extent[2] * ae_random_bi_flt();    \

        ae_switch (X(emitter)->velgen_mode, gl_particle_emitter_velgen_mode,
                                    GL_PARTICLE_EMITTER_VELGEN_MODE, suffix)
        {
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    switch (ae_random_bounded_u32(6))
                    {
                        case 0:
                        {
                            SIDE_UP;

                            vel[0] = 0.0f;
                            vel[1] = +X(emitter)->velgen_scale;
                            vel[2] = 0.0f;
                        }
                        break;

                        case 1:
                        {
                            SIDE_DN;

                            vel[0] = 0.0f;
                            vel[1] = -X(emitter)->velgen_scale;
                            vel[2] = 0.0f;
                        }
                        break;

                        case 2:
                        {
                            SIDE_BK;

                            vel[0] = 0.0f;
                            vel[1] = 0.0f;
                            vel[2] = +X(emitter)->velgen_scale;
                        }
                        break;

                        case 3:
                        {
                            SIDE_FT;

                            vel[0] = 0.0f;
                            vel[1] = 0.0f;
                            vel[2] = -X(emitter)->velgen_scale;
                        }
                        break;

                        case 4:
                        {
                            SIDE_RT;

                            vel[0] = +X(emitter)->velgen_scale;
                            vel[1] = 0.0f;
                            vel[2] = 0.0f;
                        }
                        break;

                        case 5:
                        {
                            SIDE_LF;

                            vel[0] = -X(emitter)->velgen_scale;
                            vel[1] = 0.0f;
                            vel[2] = 0.0f;
                        }
                        break;

                        default:
                        {
                            assert(0);
                        }
                        break;
                    }
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_RANDOM:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    switch (ae_random_bounded_u32(6))
                    {
                        case 0:
                        {
                            SIDE_UP;

                            vel[0] = 0.0f;
                            vel[1] = +X(emitter)->velgen_scale * ae_random_flt();
                            vel[2] = 0.0f;
                        }
                        break;

                        case 1:
                        {
                            SIDE_DN;

                            vel[0] = 0.0f;
                            vel[1] = -X(emitter)->velgen_scale * ae_random_flt();
                            vel[2] = 0.0f;
                        }
                        break;

                        case 2:
                        {
                            SIDE_BK;

                            vel[0] = 0.0f;
                            vel[1] = 0.0f;
                            vel[2] = +X(emitter)->velgen_scale * ae_random_flt();
                        }
                        break;

                        case 3:
                        {
                            SIDE_FT;

                            vel[0] = 0.0f;
                            vel[1] = 0.0f;
                            vel[2] = -X(emitter)->velgen_scale * ae_random_flt();
                        }
                        break;

                        case 4:
                        {
                            SIDE_RT;

                            vel[0] = +X(emitter)->velgen_scale * ae_random_flt();
                            vel[1] = 0.0f;
                            vel[2] = 0.0f;
                        }
                        break;

                        case 5:
                        {
                            SIDE_LF;

                            vel[0] = -X(emitter)->velgen_scale * ae_random_flt();
                            vel[1] = 0.0f;
                            vel[2] = 0.0f;
                        }
                        break;

                        default:
                        {
                            assert(0);
                        }
                        break;
                    }
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_BI_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_BI_RANDOM:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    switch (ae_random_bounded_u32(6))
                    {
                        case 0:
                        {
                            SIDE_UP;

                            vel[0] = 0.0f;
                            vel[1] = X(emitter)->velgen_scale * ae_random_bi_flt();
                            vel[2] = 0.0f;
                        }
                        break;

                        case 1:
                        {
                            SIDE_DN;

                            vel[0] = 0.0f;
                            vel[1] = X(emitter)->velgen_scale * ae_random_bi_flt();
                            vel[2] = 0.0f;
                        }
                        break;

                        case 2:
                        {
                            SIDE_BK;

                            vel[0] = 0.0f;
                            vel[1] = 0.0f;
                            vel[2] = X(emitter)->velgen_scale * ae_random_bi_flt();
                        }
                        break;

                        case 3:
                        {
                            SIDE_FT;

                            vel[0] = 0.0f;
                            vel[1] = 0.0f;
                            vel[2] = X(emitter)->velgen_scale * ae_random_bi_flt();
                        }
                        break;

                        case 4:
                        {
                            SIDE_RT;

                            vel[0] = X(emitter)->velgen_scale * ae_random_bi_flt();
                            vel[1] = 0.0f;
                            vel[2] = 0.0f;
                        }
                        break;

                        case 5:
                        {
                            SIDE_LF;

                            vel[0] = X(emitter)->velgen_scale * ae_random_bi_flt();
                            vel[1] = 0.0f;
                            vel[2] = 0.0f;
                        }
                        break;

                        default:
                        {
                            assert(0);
                        }
                        break;
                    }
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_NONE:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_ZERO:
            {
                for (; pos < end; pos += 3)
                {
                    switch (ae_random_bounded_u32(6))
                    {
                        case 0: { SIDE_UP; } break;
                        case 1: { SIDE_DN; } break;
                        case 2: { SIDE_BK; } break;
                        case 3: { SIDE_FT; } break;
                        case 4: { SIDE_RT; } break;
                        case 5: { SIDE_LF; } break;
                    }
                }

                if (X(emitter)->velgen_mode == GL_PARTICLE_EMITTER_VELGEN_MODE_RANDOM)
                {
                    end = vel + X(emitter)->num_particles * 3;

                    for (; vel < end; vel += 3)
                    {
                        vel[0] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                        vel[1] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                        vel[2] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                    }
                }
                else if (X(emitter)->velgen_mode == GL_PARTICLE_EMITTER_VELGEN_MODE_ZERO)
                {
                    memset(vel, 0, X(emitter)->num_particles * sizeof(float[3]));
                }
            }
            break;

            default:
            {
                assert(0);
            }
            break;
        }

        #undef SIDE_UP
        #undef SIDE_DN
        #undef SIDE_BK
        #undef SIDE_FT
        #undef SIDE_RT
        #undef SIDE_LF

        // X(emitter)->frozen = 1;
    }
}

void gl_particle_emitter_sphere_limit(gl_particle_emitter_t* emitter,
                                        float* center, float radius)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        // TODO: move this function to ae_particle/pfx_sphere_limit
        const float radius2 = radius * radius;

        float* ptr = static_cast<float*>(X(emitter)->particles.data);
        float* end = ptr + X(emitter)->num_particles * 3;

        for (; ptr < end; ptr += 3)
        {
            float dir[3]; // vector from center
            vec3sub_vec(dir, ptr, center);

            // TODO: if emitter pos is inside the sphere, copy that
            if (vec3mag2(dir) >= radius2) { vec3copy(ptr, center); }
        }
    }
}

void gl_particle_emitter_sphere_clamp(gl_particle_emitter_t* emitter,
                                        float* center, float radius)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        // TODO: move this function to ae_particle/pfx_sphere_clamp
        const float radius2 = radius * radius;

        float* ptr = static_cast<float*>(X(emitter)->particles.data);
        float* end = ptr + X(emitter)->num_particles * 3;

        /* NOTE: using the fast inverse square root or _mm_rsqrt_ss
         * here loses too much precision and makes particles jitter
         */
        for (; ptr < end; ptr += 3)
        {
            float dir[3]; // vector from center
            vec3sub_vec(dir, ptr, center);

            if (vec3mag2(dir) >= radius2)
            {
                vec3normalize(dir, dir);

                vec3mul_flt(ptr, dir, radius);
                vec3add_vec(ptr, ptr, center);
            }
        }
    }
}

void gl_particle_emitter_sphere_reset(gl_particle_emitter_t* emitter,
                                        float* center, float radius)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        X(emitter)->num_particles = X(emitter)->max_particles; // fill emitter

        float* pos = (float*)X(emitter)->particles.data;
        float* end = pos + X(emitter)->num_particles * 3;
        float* vel = (float*)X(emitter)->vel_array.data;

        #define PICK_POS()                  \
            ae_random_unit_vec3(pos);       \
                                            \
            vec3mul_flt(pos, pos, radius);  \
            vec3add_vec(pos, pos, center);  \

        #define SPHERE_V(s)                 \
            vec3sub_vec(vel, pos, center);  \
            vec3normalize(vel, vel);        \
            vec3mul_flt(vel, vel, X(emitter)->velgen_scale * (s));

        ae_switch (X(emitter)->velgen_mode, gl_particle_emitter_velgen_mode,
                                    GL_PARTICLE_EMITTER_VELGEN_MODE, suffix)
        {
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    PICK_POS(); // TODO: SSE normals (for velocity only)
                    SPHERE_V(1.0f);
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_RANDOM:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    PICK_POS(); // TODO: SSE normals (for velocity only)
                    SPHERE_V(ae_random_flt());
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_BI_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_BI_RANDOM:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    PICK_POS(); // TODO: SSE normals (for velocity only)
                    SPHERE_V(ae_random_bi_flt());
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_NONE:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_ZERO:
            {
                for (; pos < end; pos += 3) { PICK_POS(); }

                if (X(emitter)->velgen_mode == GL_PARTICLE_EMITTER_VELGEN_MODE_RANDOM)
                {
                    end = vel + X(emitter)->num_particles * 3;

                    for (; vel < end; vel += 3)
                    {
                        vel[0] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                        vel[1] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                        vel[2] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                    }
                }
                else if (X(emitter)->velgen_mode == GL_PARTICLE_EMITTER_VELGEN_MODE_ZERO)
                {
                    memset(vel, 0, X(emitter)->num_particles * sizeof(float[3]));
                }
            }
            break;

            default:
            {
                assert(0);
            }
            break;
        }

        #undef PICK_POS
        #undef SPHERE_V

        // X(emitter)->frozen = 1;
    }
}

void gl_particle_emitter_radius_limit(gl_particle_emitter_t* emitter, float radius)
{
    AE_PROFILE_SCOPE();

    float* v = gl_particle_emitter_get_position(emitter);
    gl_particle_emitter_sphere_limit(emitter, v, radius);
}

void gl_particle_emitter_radius_clamp(gl_particle_emitter_t* emitter, float radius)
{
    AE_PROFILE_SCOPE();

    float* v = gl_particle_emitter_get_position(emitter);
    gl_particle_emitter_sphere_clamp(emitter, v, radius);
}

void gl_particle_emitter_radius_reset(gl_particle_emitter_t* emitter, float radius)
{
    AE_PROFILE_SCOPE();

    float* v = gl_particle_emitter_get_position(emitter);
    gl_particle_emitter_sphere_reset(emitter, v, radius);
}

void gl_particle_emitter_line_reset(gl_particle_emitter_t* emitter,
                                    float* min_val, float* max_val)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        X(emitter)->num_particles = X(emitter)->max_particles;

        float* pos = (float*)X(emitter)->particles.data;
        float* end = pos + X(emitter)->num_particles * 3;
        float* vel = (float*)X(emitter)->vel_array.data;

        /* these don't really have to point up or right, as long as they're orthogonal.
         */
        const float up_dir[3] = { 0.0f, 1.0f, 0.0f };
        const float rt_dir[3] = { 1.0f, 0.0f, 0.0f };

        /* we use these min and max values to store the beginning and endpoints of the
         * line; this may look like extent calculation, but we're calculating line dir.
         */
        float dir[3];
        float adj[3];

        vec3sub_vec(dir, max_val, min_val);
        vec3normalize(dir, dir);
        vec3cross(adj, dir, up_dir);

        /* since each vector has an infinite number of perpendicular vectors, we just
         * pick a random one; this tries to ensure that our axis of rotation is always
         * perpendicular to the direction of the line, but it fails in certain cases.
         * we mitigate this by just renormalizing the perpendicular vector if need be.
         */
        ae_if (vec3mag2(adj) < 0.01f) vec3cross(adj, dir, rt_dir);
        ae_if (vec3mag2(adj) < 1.00f) vec3normalize(adj, adj);

        ae_switch (X(emitter)->velgen_mode, gl_particle_emitter_velgen_mode,
                                    GL_PARTICLE_EMITTER_VELGEN_MODE, suffix)
        {
            /*  TODO: if they're common, axial rotation cases could be sped up
                by just using sin and cos instead of arbitrary-axis rotations!
                should also see if matrix creation + multiplication is cheaper.
             */
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    vec3lerp(pos, min_val, max_val, ae_random_flt());
                    vec3rotate(vel, adj, dir, 2.0 * ae_random_flt() * M_PI);

                    vec3mul_flt(vel, vel, X(emitter)->velgen_scale);
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_BI_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_TRIANGLE_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_BI_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_FACE_NORMAL_RANDOM:
            {
                for (; pos < end; pos += 3, vel += 3)
                {
                    vec3lerp(pos, min_val, max_val, ae_random_flt()); // random scale
                    vec3rotate(vel, adj, dir, 2.0 * ae_random_flt() * M_PI);

                    vec3mul_flt(vel, vel, X(emitter)->velgen_scale * ae_random_flt());
                }
            }
            break;

            case GL_PARTICLE_EMITTER_VELGEN_MODE_NONE:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_RANDOM:
            case GL_PARTICLE_EMITTER_VELGEN_MODE_ZERO:
            {
                for (; pos < end; pos += 3)
                {
                    vec3lerp(pos, min_val, max_val, ae_random_flt());
                }

                if (X(emitter)->velgen_mode == GL_PARTICLE_EMITTER_VELGEN_MODE_RANDOM)
                {
                    end = vel + X(emitter)->num_particles * 3;

                    for (; vel < end; vel += 3)
                    {
                        vel[0] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                        vel[1] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                        vel[2] = ae_random_bi_flt() * X(emitter)->velgen_scale;
                    }
                }
                else if (X(emitter)->velgen_mode == GL_PARTICLE_EMITTER_VELGEN_MODE_ZERO)
                {
                    memset(vel, 0, X(emitter)->num_particles * sizeof(float[3]));
                }
            }
            break;

            default:
            {
                assert(0);
            }
            break;
        }

        // X(emitter)->frozen = 1;
    }
}

void gl_particle_emitter_reset_all(void)
{
    AE_PROFILE_SCOPE();
    gl_init();

    aeTempArray<gl_particle_emitter_t*> emitters(gl_particle_emitter_count_all());
    gl_particle_emitter_list_all(emitters.data());

    for (size_t i = 0, n = gl_particle_emitter_count_all(); i < n; i++)
    {
        gl_particle_emitter_reset(emitters[i]);
    }
}

void gl_particle_emitter_reset(gl_particle_emitter_t* emitter)
{
    if (gl_particle_emitter_get_open(emitter))
    {
        assert(!X(emitter)->locked);

        X(emitter)->num_particles = 0;
        X(emitter)->current_index = 0;

        X(emitter)->time = 0.0;
    }
}

void gl_particle_emitter_fill_all(void)
{
    AE_PROFILE_SCOPE();
    gl_init();

    aeTempArray<gl_particle_emitter_t*> emitters(gl_particle_emitter_count_all());
    gl_particle_emitter_list_all(emitters.data());

    for (size_t i = 0, n = gl_particle_emitter_count_all(); i < n; i++)
    {
        gl_particle_emitter_fill(emitters[i]);
    }
}

void gl_particle_emitter_fill(gl_particle_emitter_t* emitter)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        // TODO: while this should protect us from an infinite loop,
        // we should still add a timeout counter to this function...

        if (X(emitter)->frozen || X(emitter)->max_particles == 0)
        {
            return;
        }

        while (X(emitter)->num_particles < X(emitter)->max_particles)
        {
            gl_particle_emitter_update(emitter, X(emitter)->period);
        }
    }
}

void gl_particle_emitter_update_all(double dt)
{
    AE_PROFILE_SCOPE();
    gl_init();

    aeTempArray<gl_particle_emitter_t*> emitters(gl_particle_emitter_count_all());
    gl_particle_emitter_list_all(emitters.data());

    for (size_t i = 0, n = gl_particle_emitter_count_all(); i < n; i++)
    {
        gl_particle_emitter_update(emitters[i], dt);
    }
}

static void
gl_particle_emitter_generate_particles(gl_internal_particle_emitter_t* data, double dt)
{
    AE_PROFILE_SCOPE();

    while (data->time > data->period)
    {
        data->time -= data->period;

        for (int i = 0; i < data->burst_size; i++)
        {
            if (data->num_particles != data->max_particles) data->num_particles += 1;
            if (data->current_index >= data->max_particles) data->current_index  = 0;

            float* p = ((float*)data->particles.data) + data->current_index * 3;
            float* v = ((float*)data->vel_array.data) + data->current_index * 3;

            /* TODO: store const members in locals, selection of different gen algorithms
             */
            p[0] = data->position[0] + ae_random_gauss_flt(0.0f, data->position_sigma[0]);
            p[1] = data->position[1] + ae_random_gauss_flt(0.0f, data->position_sigma[1]);
            p[2] = data->position[2] + ae_random_gauss_flt(0.0f, data->position_sigma[2]);

            v[0] = data->velocity[0] + ae_random_gauss_flt(0.0f, data->velocity_sigma[0]);
            v[1] = data->velocity[1] + ae_random_gauss_flt(0.0f, data->velocity_sigma[1]);
            v[2] = data->velocity[2] + ae_random_gauss_flt(0.0f, data->velocity_sigma[2]);

            data->current_index++;
        }
    }
}

static void
gl_particle_emitter_move_particles(gl_internal_particle_emitter_t* data, double dt)
{
    AE_PROFILE_SCOPE();

    ae_if (!vec3is_zero_e(data->gravity, GL_DEFAULT_EPSILON))
    {
        float scaled[3]; // scale gravity for time
        vec3mul_flt(scaled, data->gravity, dt);

        vtx_vec3add((float*)data->vel_array.data, scaled, data->num_particles * 3);
    }

    vtx_madd( // TODO: fold add + scale into one loop
            (float*)data->particles.data,

            (const float* const)data->particles.data,
            (const float* const)data->vel_array.data,

            dt, data->num_particles * 3);
}

void gl_particle_emitter_update(gl_particle_emitter_t* emitter, double dt)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked);

        // TODO: assert that particles and vel_array sizes match
        // TODO: assert that num_particles <= max_particles

        if (X(emitter)->frozen || X(emitter)->max_particles == 0)
        {
            return;
        }

        X(emitter)->time += dt;

        gl_particle_emitter_generate_particles(X(emitter), dt);
        gl_particle_emitter_move_particles(X(emitter), dt);
    }
}

void gl_particle_emitter_draw_all(void)
{
    AE_PROFILE_SCOPE();
    gl_init();

    aeTempArray<gl_particle_emitter_t*> emitters(gl_particle_emitter_count_all());
    gl_particle_emitter_list_all(emitters.data());

    for (size_t i = 0, n = gl_particle_emitter_count_all(); i < n; i++)
    {
        gl_particle_emitter_draw(emitters[i]);
    }
}

void gl_particle_emitter_draw(gl_particle_emitter_t* emitter)
{
    AE_PROFILE_SCOPE();

    if (ae_likely(gl_particle_emitter_get_open(emitter)))
    {
        assert(!X(emitter)->locked); // don't issue GL commands on empty emitters
        ae_if (X(emitter)->num_particles == 0) { return; }

        /* FIXME: point sprites are rendered upside-down, so we'll need to push a
         * texture matrix to set the origin to the upper-left corner temporarily.
         * in future implementations, this will of course be done inside a shader.
         *
         * FIXME: on some drivers i've tested this on, particles look super wonky,
         * growing and shrinking to ridiculous sizes in seemingly random ways...
         * we should probably check for some missing extension and try a fallback.
         * my initial suspicion is something involving PointParameter* functions.
         *
         * after some further testing on my linux machine, this bizarre flickering
         * behavior also seems to be affecting line rendering mode... it appears
         * that any opengl usage beyond the beaten path is subject to glitchiness.
         */
        GL_DepthMask(GL_FALSE);
        {
            GL_PushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
            {
                GL_Color4fv(X(emitter)->color);
                GL_Disable(GL_LIGHTING);

                GL_Enable(GL_BLEND); // default alpha blending state
                GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                GL_PointSize(X(emitter)->max_point_size);
                gl_texture_bind(X(emitter)->texture);

                GL_PointParameterf(GL_POINT_SIZE_MIN, X(emitter)->min_point_size);

                float attenuation[3] = { 0.0f, 0.0f, 0.01f }; // TODO: property
                GL_PointParameterfv(GL_POINT_DISTANCE_ATTENUATION, attenuation);

                ae_if (gl_texture_get_open(X(emitter)->texture))
                {
                    GL_TexEnvf(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
                    GL_Enable(GL_POINT_SPRITE);
                }

                GL_PushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);
                {
                    GL_VertexPointer(3, GL_FLOAT, 0, X(emitter)->particles.data);
                    GL_EnableClientState(GL_VERTEX_ARRAY);
                    GL_DrawArrays(GL_POINTS, 0, X(emitter)->num_particles);
                }
                GL_PopClientAttrib();
            }
            GL_PopAttrib();
        }
        GL_DepthMask(GL_TRUE);

        // TODO: GL_DEBUG, draw only if it's #defined, color/enabled props
        #if 0
        {
            static const u32 s = ae_random_bounded_u32(AE_COLOR_COUNT);
            static const u32 e = ae_random_bounded_u32(AE_COLOR_COUNT);

            gl_particle_emitter_draw_velocity_ex(emitter,
                                ae_color_flt[s], ae_color_flt[e], 1.0f);
        }
        #endif

        gl_debug_log_error_state();
    }
}

void gl_particle_emitter_draw_velocity_ex( gl_particle_emitter_t * emitter,
        const float* start_color, const float* end_color, float line_width)
{
    AE_PROFILE_SCOPE();

    if (gl_particle_emitter_get_open(emitter))
    {
        assert(!X(emitter)->locked); // don't issue GL commands on empty emitters
        ae_if (X(emitter)->num_particles == 0) { return; }

        GL_PushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_LINE_BIT);
        {
            GL_Disable(GL_TEXTURE_2D);
            GL_Disable(GL_LIGHTING);

            GL_LineWidth(line_width);

            GL_Enable(GL_BLEND);
            GL_BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            GL_Begin(GL_LINES);
            {
                for (int i = 0; i < X(emitter)->num_particles * 3; i += 3)
                {
                    float* p = ((float*)X(emitter)->particles.data) + i;
                    float* v = ((float*)X(emitter)->vel_array.data) + i;

                    GL_Color4fv(start_color);
                    GL_Vertex3fv(p);

                    GL_Color4fv(end_color);
                    GL_Vertex3f(p[0] + v[0], p[1] + v[1], p[2] + v[2]);
                }
            }
            GL_End();
        }
        GL_PopAttrib();

        gl_debug_log_error_state();
    }
}

void gl_particle_emitter_draw_velocity(gl_particle_emitter_t* emitter,
                                                    const float* rgba)
{
    gl_particle_emitter_draw_velocity_ex(emitter, rgba, rgba, 1.0f);
}

#undef X

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

static int __glcore_is_initialized;

int gl_is_init(void)
{
    return __glcore_is_initialized;
}

void gl_init(void)
{
    if (!__glcore_is_initialized)
    {
        const double init_time = ae_internal_seconds();

        if (!ae_is_init())
        {
            AE_WARN("initialize aecore before gl (command-line args ignored)");
            ae_init(0, NULL);
        }

        ae_atexit_ex(gl_quit);

        #define N(cap, low) /* create the lists we use to track objects */  \
                                                                            \
            ae_assert(memiszero(&gl_ ## low ## _lst, sizeof(ae_ptrset_t)),  \
                                "the %s set is already initialized",#low);  \
                                                                            \
            ae_ptrset_init(&gl_ ## low ## _lst, 16);                        \

        GL_OBJECT_TYPE_N
        #undef N

        /* log a few different structure sizes for easier heap tracking.
         */
        #define N(cap, low) ae_log(MISC, "gl_%s_t:\t%u bytes", \
                #low, (u32)sizeof(gl_internal_ ## low ## _t));

        GL_OBJECT_TYPE_N
        #undef N

        ae_log( TIME, "gl_init done in %.2f milliseconds",
            (ae_internal_seconds() - init_time) * 1000.0);
    }

    __glcore_is_initialized = 1;
}

void gl_quit(void)
{
    if (__glcore_is_initialized)
    {
        const double quit_time = ae_seconds();

        // library startup and shutdown must always be stack-like
        ae_assert(ae_is_init(), "called gl_quit after ae_quit");

        /* TODO: if no GL calls have been issued, don't log internal gl state
         */
        gl_log_internal_state();
        gl_object_close_all();

        #define N(cap, low) /* free and zero out object lists */            \
                                                                            \
            ae_assert(gl_ ## low ## _count_all() == 0, "%s leaked!", #low); \
                                                                            \
            ae_ptrset_free(&gl_ ## low ## _lst);                            \
            memset(&gl_ ## low ## _lst, 0, sizeof(ae_ptrset_t));            \

        GL_OBJECT_TYPE_N
        #undef N

        ae_log(TIME, "gl_quit done in %.2f milliseconds",
                    (ae_seconds() - quit_time) * 1000.0);
    }

    __glcore_is_initialized = 0;
}
