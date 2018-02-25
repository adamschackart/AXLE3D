/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
--------------------------------------------------------------------------------
--- TODO: state tracking? don't rebind the same material or texture redundantly
----------------------------------------------------------------------------- */
#ifndef __GL_CORE_H__
#include <gl_core.h>
#endif

// TODO: use the system GL.h
#include <SDL2/SDL_opengl.h>

/*
================================================================================
 * ~~ [ OpenGL wrapper ] ~~ *
--------------------------------------------------------------------------------
TODO: use a script like gl3w to automatically generate this from OpenGL headers
TODO: if we can't find functions, try appending EXT and then ARB to their names
TODO: GL_SetError, which sets a global we check before ever calling GL_GetError
TODO: wrap all plural deletes in singular form (GL_DeleteTexture function etc.)
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

void GL_Accum(unsigned int op, float value)
{
    X(); glAccum(op, value);
}

void GL_ActiveShaderProgram(unsigned int pipeline, unsigned int program)
{
    /* TODO allow this function ptr to be reset to NULL on gl_quit */
    static PFNGLACTIVESHADERPROGRAMPROC exActiveShaderProgram = NULL;

    X(); if (exActiveShaderProgram)
    {
        exActiveShaderProgram(pipeline, program);
    }
    else
    {
        exActiveShaderProgram = ( PFNGLACTIVESHADERPROGRAMPROC )
                                gl_func("glActiveShaderProgram");

        if (exActiveShaderProgram)
        {
            exActiveShaderProgram(pipeline, program);
        }
        else
        {
            AE_WARN("glActiveShaderProgram not found");
        }
    }
}

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

unsigned char GL_AreTexturesResident(int n, const unsigned int* tex_array,
                                                unsigned char* residences)
{
    X(); return glAreTexturesResident(n, tex_array, residences);
}

void GL_ArrayElement(int i)
{
    X(); glArrayElement(i);
}

void GL_AttachShader(unsigned int program, unsigned int shader)
{
    // TODO: allow this to be reset to NULL on gl_quit
    static PFNGLATTACHSHADERPROC exAttachShader = NULL;

    X(); if (exAttachShader)
    {
        exAttachShader(program, shader);
    }
    else
    {
        exAttachShader = ( PFNGLATTACHSHADERPROC )
                        gl_func("glAttachShader");

        if (exAttachShader)
        {
            exAttachShader(program, shader);
        }
        else
        {
            AE_WARN("glAttachShader not found");
        }
    }
}

void GL_Begin(unsigned int mode)
{
    X(); glBegin(mode);
}

void GL_BeginConditionalRender(unsigned int id, unsigned int mode)
{
    // TODO: allow this gl function pointer to be reset to NULL on gl_quit
    static PFNGLBEGINCONDITIONALRENDERPROC exBeginConditionalRender = NULL;

    X(); if (exBeginConditionalRender)
    {
        exBeginConditionalRender(id, mode);
    }
    else
    {
        exBeginConditionalRender = (PFNGLBEGINCONDITIONALRENDERPROC)
                                gl_func("glBeginConditionalRender");

        if (exBeginConditionalRender)
        {
            exBeginConditionalRender(id, mode);
        }
        else
        {
            AE_WARN("glBeginConditionalRender not found");
        }
    }
}

void GL_BeginQuery(unsigned int target, unsigned int id)
{
    /* TODO let this be reset to NULL on gl_quit */
    static PFNGLBEGINQUERYPROC exBeginQuery = NULL;

    X(); if (exBeginQuery)
    {
        exBeginQuery(target, id);
    }
    else
    {
        exBeginQuery = (PFNGLBEGINQUERYPROC)
                    gl_func("glBeginQuery");

        if (exBeginQuery)
        {
            exBeginQuery(target, id);
        }
        else
        {
            AE_WARN("glBeginQuery not found");
        }
    }
}

void GL_BeginQueryIndexed(unsigned int target, unsigned int index, unsigned int id)
{
    // TODO allow this gl function to be reset to NULL on gl_quit
    static PFNGLBEGINQUERYINDEXEDPROC exBeginQueryIndexed = NULL;

    X(); if (exBeginQueryIndexed)
    {
        exBeginQueryIndexed(target, index, id);
    }
    else
    {
        exBeginQueryIndexed = (PFNGLBEGINQUERYINDEXEDPROC)
                            gl_func("glBeginQueryIndexed");

        if (exBeginQueryIndexed)
        {
            exBeginQueryIndexed(target, index, id);
        }
        else
        {
            AE_WARN("glBeginQueryIndexed not found");
        }
    }
}

void GL_BeginTransformFeedback(unsigned int primitive_mode)
{
    // TODO: allow this gl function pointer to be reset to NULL on gl_quit
    static PFNGLBEGINTRANSFORMFEEDBACKPROC exBeginTransformFeedback = NULL;

    X(); if (exBeginTransformFeedback)
    {
        exBeginTransformFeedback(primitive_mode);
    }
    else
    {
        exBeginTransformFeedback = (PFNGLBEGINTRANSFORMFEEDBACKPROC)
                                gl_func("glBeginTransformFeedback");

        if (exBeginTransformFeedback)
        {
            exBeginTransformFeedback(primitive_mode);
        }
        else
        {
            AE_WARN("glBeginTransformFeedback not found");
        }
    }
}

void GL_BindAttribLocation(unsigned int program, unsigned int index, const char *name)
{
    // TODO: allow this gl function to be reset to NULL on gl_quit
    static PFNGLBINDATTRIBLOCATIONPROC exBindAttribLocation = NULL;

    X(); if (exBindAttribLocation)
    {
        exBindAttribLocation(program, index, name);
    }
    else
    {
        exBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)
                            gl_func("glBindAttribLocation");

        if (exBindAttribLocation)
        {
            exBindAttribLocation(program, index, name);
        }
        else
        {
            AE_WARN("glBindAttribLocation not found");
        }
    }
}

void GL_BindBuffer(unsigned int target, unsigned int buffer)
{
    /* TODO: let this be set to NULL on gl_quit */
    static PFNGLBINDBUFFERPROC exBindBuffer = NULL;

    X(); if (exBindBuffer)
    {
        exBindBuffer(target, buffer);
    }
    else
    {
        exBindBuffer = (PFNGLBINDBUFFERPROC)
                    gl_func("glBindBuffer");

        if (exBindBuffer)
        {
            exBindBuffer(target, buffer);
        }
        else
        {
            AE_WARN("glBindBuffer not found");
        }
    }
}

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

void GL_BindVertexArray(unsigned int array)
{
    /* TODO allow this func to be reset to NULL on gl_quit */
    static PFNGLBINDVERTEXARRAYPROC exBindVertexArray = NULL;

    X(); if (exBindVertexArray)
    {
        exBindVertexArray(array);
    }
    else
    {
        exBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)
                        gl_func("glBindVertexArray");

        if (exBindVertexArray)
        {
            exBindVertexArray(array);
        }
        else
        {
            AE_WARN("glBindVertexArray not found");
        }
    }
}

// TODO: GL_BindVertexBuffer

// TODO: GL_BindVertexBuffers

void GL_Bitmap(int width, int height, float x_origin, float y_origin,
            float x_move, float y_move, const unsigned char * bitmap)
{
    X(); glBitmap(width, height, x_origin, y_origin, x_move, y_move, bitmap);
}

void GL_BlendColor(float red, float green, float blue, float alpha)
{
    /* TODO let this be reset to NULL on gl_quit */
    static PFNGLBLENDCOLORPROC exBlendColor = NULL;

    X(); if (exBlendColor)
    {
        exBlendColor(red, green, blue, alpha);
    }
    else
    {
        exBlendColor = (PFNGLBLENDCOLORPROC)
                    gl_func("glBlendColor");

        if (exBlendColor)
        {
            exBlendColor(red, green, blue, alpha);
        }
        else
        {
            AE_WARN("glBlendColor not found");
        }
    }
}

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

void GL_BufferData(unsigned int target, ptrdiff_t size,
                const void * data, unsigned int usage)
{
    /* TODO: let this be set to NULL on gl_quit */
    static PFNGLBUFFERDATAPROC exBufferData = NULL;

    X(); if (exBufferData)
    {
        exBufferData(target, size, data, usage);
    }
    else
    {
        exBufferData = (PFNGLBUFFERDATAPROC)
                    gl_func("glBufferData");

        if (exBufferData)
        {
            exBufferData(target, size, data, usage);
        }
        else
        {
            AE_WARN("glBufferData not found");
        }
    }
}

// TODO: GL_BufferStorage

void GL_BufferSubData(unsigned int target, ptrdiff_t offset,
                        ptrdiff_t size, const void * data)
{
    // TODO: let this pointer be reset to NULL on gl_quit
    static PFNGLBUFFERSUBDATAPROC exBufferSubData = NULL;

    X(); if (exBufferSubData)
    {
        exBufferSubData(target, offset, size, data);
    }
    else
    {
        exBufferSubData = (PFNGLBUFFERSUBDATAPROC)
                        gl_func("glBufferSubData");

        if (exBufferSubData)
        {
            exBufferSubData(target, offset, size, data);
        }
        else
        {
            AE_WARN("glBufferSubData not found");
        }
    }
}

void GL_CallList(unsigned int list)
{
    X(); glCallList(list);
}

void GL_CallLists(int n, unsigned int type, const void *lists)
{
    X(); glCallLists(n, type, lists);
}

// TODO: GL_CheckFramebufferStatus

// TODO: GL_ClampColor

void GL_Clear(unsigned int mask)
{
    X(); glClear(mask);
}

void GL_ClearAccum(float r, float g, float b, float a)
{
    X(); glClearAccum(r, g, b, a);
}

// TODO: GL_ClearBufferData

// TODO: GL_ClearBufferSubData

// TODO: GL_ClearBufferfi

// TODO: GL_ClearBufferfv

// TODO: GL_ClearBufferiv

// TODO: GL_ClearBufferuiv

void GL_ClearColor(float r, float g, float b, float a)
{
    X(); glClearColor(r, g, b, a);
}

void GL_ClearDepth(double depth)
{
    X(); glClearDepth(depth);
}

// TODO: GL_ClearDepthf

void GL_ClearIndex(float c)
{
    X(); glClearIndex(c);
}

void GL_ClearStencil(int s)
{
    X(); glClearStencil(s);
}

// TODO: GL_ClearTexImage

// TODO: GL_ClearTexSubImage

// TODO: GL_ClientActiveTexture

// TODO: GL_ClientWaitSync

void GL_ClipPlane(unsigned int plane, const double *equation)
{
    X(); glClipPlane(plane, equation);
}

void GL_Color3b(char red, char green, char blue)
{
    X(); glColor3b(red, green, blue);
}

void GL_Color3bv(const char *v)
{
    X(); glColor3bv((const GLbyte*)v);
}

void GL_Color3d(double red, double green, double blue)
{
    X(); glColor3d(red, green, blue);
}

void GL_Color3dv(const double *v)
{
    X(); glColor3dv(v);
}

void GL_Color3f(float r, float g, float b)
{
    X(); glColor3f(r, g, b);
}

void GL_Color3fv(const float* rgb)
{
    X(); glColor3fv(rgb);
}

void GL_Color3i(int red, int green, int blue)
{
    X(); glColor3i(red, green, blue);
}

void GL_Color3iv(const int* v)
{
    X(); glColor3iv(v);
}

void GL_Color3s(short red, short green, short blue)
{
    X(); glColor3s(red, green, blue);
}

void GL_Color3sv(const short* v)
{
    X(); glColor3sv(v);
}

void GL_Color3ub(unsigned char red, unsigned char green, unsigned char blue)
{
    X(); glColor3ub(red, green, blue);
}

void GL_Color3ubv(const unsigned char* v)
{
    X(); glColor3ubv(v);
}

void GL_Color3ui(unsigned int red, unsigned int green, unsigned int blue)
{
    X(); glColor3ui(red, green, blue);
}

void GL_Color3uiv(const unsigned int* v)
{
    X(); glColor3uiv(v);
}

void GL_Color3us(unsigned short red, unsigned short green, unsigned short blue)
{
    X(); glColor3us(red, green, blue);
}

void GL_Color3usv(const unsigned short* v)
{
    X(); glColor3usv(v);
}

void GL_Color4b(char red, char green, char blue, char alpha)
{
    X(); glColor4b(red, green, blue, alpha);
}

void GL_Color4bv(const char* v)
{
    X(); glColor4bv((const GLbyte*)v);
}

void GL_Color4d(double red, double green, double blue, double alpha)
{
    X(); glColor4d(red, green, blue, alpha);
}

void GL_Color4dv(const double* v)
{
    X(); glColor4dv(v);
}

void GL_Color4f(float r, float g, float b, float a)
{
    X(); glColor4f(r, g, b, a);
}

void GL_Color4fv(const float* rgba)
{
    X(); glColor4fv(rgba);
}

void GL_Color4i(int red, int green, int blue, int alpha)
{
    X(); glColor4i(red, green, blue, alpha);
}

void GL_Color4iv(const int* v)
{
    X(); glColor4iv(v);
}

void GL_Color4s(short red, short green, short blue, short alpha)
{
    X(); glColor4s(red, green, blue, alpha);
}

void GL_Color4sv(const short* v)
{
    X(); glColor4sv(v);
}

void GL_Color4ub(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
    X(); glColor4ub(red, green, blue, alpha);
}

void GL_Color4ubv(const unsigned char* v)
{
    X(); glColor4ubv(v);
}

void GL_Color4ui(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha)
{
    X(); glColor4ui(red, green, blue, alpha);
}

void GL_Color4uiv(const unsigned int* v)
{
    X(); glColor4uiv(v);
}

void GL_Color4us(unsigned short red, unsigned short green, unsigned short blue, unsigned short alpha)
{
    X(); glColor4us(red, green, blue, alpha);
}

void GL_Color4usv(const unsigned short* v)
{
    X(); glColor4usv(v);
}

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

void GL_CompileShader(unsigned int shader)
{
    /* TODO: allow this to be reset to NULL on gl_quit */
    static PFNGLCOMPILESHADERPROC exCompileShader = NULL;

    X(); if (exCompileShader)
    {
        exCompileShader(shader);
    }
    else
    {
        exCompileShader = (PFNGLCOMPILESHADERPROC)
                        gl_func("glCompileShader");

        if (exCompileShader)
        {
            exCompileShader(shader);
        }
        else
        {
            AE_WARN("glCompileShader not found");
        }
    }
}

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

void GL_CopyPixels(int x, int y, int width, int height, unsigned int type)
{
    X(); glCopyPixels(x, y, width, height, type);
}

void GL_CopyTexImage1D(unsigned int target, int level, unsigned int internal_format,
                                                int x, int y, int width, int border)
{
    X(); glCopyTexImage1D(target, level, internal_format, x, y, width, border);
}

void GL_CopyTexImage2D(unsigned int target, int level, unsigned int internal_format,
                                    int x, int y, int width, int height, int border)
{
    X(); glCopyTexImage2D(target, level, internal_format, x, y, width, height, border);
}

// TODO: GL_CopyTexImage3D

void GL_CopyTexSubImage1D(unsigned int target, int level, int xoffset,
                                            int x, int y, int width)
{
    X(); glCopyTexSubImage1D(target, level, xoffset, x, y, width);
}

void GL_CopyTexSubImage2D(unsigned int target, int level, int xoffset,
                    int yoffset, int x, int y, int width, int height)
{
    X(); glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

// TODO: GL_CopyTexSubImage3D

unsigned int GL_CreateProgram(void)
{
    /* TODO: allow this to be reset to NULL on gl_quit */
    static PFNGLCREATEPROGRAMPROC exCreateProgram = NULL;

    X(); if (exCreateProgram)
    {
        return exCreateProgram();
    }
    else
    {
        exCreateProgram = (PFNGLCREATEPROGRAMPROC)
                        gl_func("glCreateProgram");

        if (exCreateProgram)
        {
            return exCreateProgram();
        }
        else
        {
            AE_WARN("glCreateProgram not found");
        }
    }

    return 0;
}

unsigned int GL_CreateShader(unsigned int type)
{
    /* TODO: allow this to be set to NULL on gl_quit */
    static PFNGLCREATESHADERPROC exCreateShader = NULL;

    X(); if (exCreateShader)
    {
        return exCreateShader(type);
    }
    else
    {
        exCreateShader = ( PFNGLCREATESHADERPROC )
                        gl_func("glCreateShader");

        if (exCreateShader)
        {
            return exCreateShader(type);
        }
        else
        {
            AE_WARN("glCreateShader not found");
        }
    }

    return 0;
}

// TODO: GL_CreateShaderProgramv

void GL_CullFace(unsigned int face)
{
    X(); glCullFace(face);
}

// TODO: GL_DebugMessageCallback

// TODO: GL_DebugMessageControl

// TODO: GL_DebugMessageInsert

void GL_DeleteBuffers(int n, const unsigned int* buffers)
{
    /* TODO: allow this to be reset to NULL on gl_quit */
    static PFNGLDELETEBUFFERSPROC exDeleteBuffers = NULL;

    X(); if (exDeleteBuffers)
    {
        exDeleteBuffers(n, buffers);
    }
    else
    {
        exDeleteBuffers = (PFNGLDELETEBUFFERSPROC)
                        gl_func("glDeleteBuffers");

        if (exDeleteBuffers)
        {
            exDeleteBuffers(n, buffers);
        }
        else
        {
            AE_WARN("glDeleteBuffers not found");
        }
    }
}

// TODO: GL_DeleteFramebuffers

void GL_DeleteLists(unsigned int list, int range)
{
    X(); glDeleteLists(list, range);
}

void GL_DeleteProgram(unsigned int program)
{
    /* TODO: allow this to be reset to NULL on gl_quit */
    static PFNGLDELETEPROGRAMPROC exDeleteProgram = NULL;

    X(); if (exDeleteProgram)
    {
        exDeleteProgram(program);
    }
    else
    {
        exDeleteProgram = (PFNGLDELETEPROGRAMPROC)
                        gl_func("glDeleteProgram");

        if (exDeleteProgram)
        {
            exDeleteProgram(program);
        }
        else
        {
            AE_WARN("glDeleteProgram not found");
        }
    }
}

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

void GL_DeleteVertexArrays(int n, const unsigned int* arrays)
{
    /* TODO: allow this function to be reset to NULL on gl_quit */
    static PFNGLDELETEVERTEXARRAYSPROC exDeleteVertexArrays = NULL;

    X(); if (exDeleteVertexArrays)
    {
        exDeleteVertexArrays(n, arrays);
    }
    else
    {
        exDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)
                            gl_func("glDeleteVertexArrays");

        if (exDeleteVertexArrays)
        {
            exDeleteVertexArrays(n, arrays);
        }
        else
        {
            AE_WARN("glDeleteVertexArrays not found");
        }
    }
}

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

void GL_DisableClientState(unsigned int cap)
{
    X(); glDisableClientState(cap);
}

void GL_DisableVertexAttribArray(unsigned int index)
{
    /* TODO: allow this gl function pointer to be reset to NULL on gl_quit */
    static PFNGLDISABLEVERTEXATTRIBARRAYPROC exDisableVertexAttribArray = NULL;

    X(); if (exDisableVertexAttribArray)
    {
        exDisableVertexAttribArray(index);
    }
    else
    {
        exDisableVertexAttribArray = ( PFNGLDISABLEVERTEXATTRIBARRAYPROC )
                                    gl_func("glDisableVertexAttribArray");

        if (exDisableVertexAttribArray)
        {
            exDisableVertexAttribArray(index);
        }
        else
        {
            AE_WARN("glDisableVertexAttribArray not found");
        }
    }
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

void GL_DrawBuffer(unsigned int mode)
{
    X(); glDrawBuffer(mode);
}

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

void GL_DrawPixels(int width, int height, unsigned int format,
                        unsigned int type, const void* pixels)
{
    X(); glDrawPixels(width, height, format, type, pixels);
}

// TODO: GL_DrawRangeElements

// TODO: GL_DrawRangeElementsBaseVertex

// TODO: GL_DrawTransformFeedback

// TODO: GL_DrawTransformFeedbackInstanced

// TODO: GL_DrawTransformFeedbackStream

// TODO: GL_DrawTransformFeedbackStreamInstanced

void GL_EdgeFlag(unsigned char flag)
{
    X(); glEdgeFlag(flag);
}

void GL_EdgeFlagPointer(int stride, const void* ptr)
{
    X(); glEdgeFlagPointer(stride, ptr);
}

void GL_EdgeFlagv(const unsigned char* flag)
{
    X(); glEdgeFlagv(flag);
}

void GL_Enable(unsigned int cap)
{
    X(); glEnable(cap);
}

// TODO: GL_Enablei

void GL_EnableClientState(unsigned int cap)
{
    X(); glEnableClientState(cap);
}

void GL_EnableVertexAttribArray(unsigned int index)
{
    /* TODO: allow this gl function pointer to be reset to NULL on gl_quit */
    static PFNGLENABLEVERTEXATTRIBARRAYPROC exEnableVertexAttribArray = NULL;

    X(); if (exEnableVertexAttribArray)
    {
        exEnableVertexAttribArray(index);
    }
    else
    {
        exEnableVertexAttribArray = ( PFNGLENABLEVERTEXATTRIBARRAYPROC )
                                    gl_func("glEnableVertexAttribArray");

        if (exEnableVertexAttribArray)
        {
            exEnableVertexAttribArray(index);
        }
        else
        {
            AE_WARN("glEnableVertexAttribArray not found");
        }
    }
}

void GL_End(void)
{
    X(); glEnd();
}

void GL_EndConditionalRender(void)
{
    // TODO: allow this function pointer to be reset to NULL on gl_quit
    static PFNGLENDCONDITIONALRENDERPROC exEndConditionalRender = NULL;

    X(); if (exEndConditionalRender)
    {
        exEndConditionalRender();
    }
    else
    {
        exEndConditionalRender = ( PFNGLENDCONDITIONALRENDERPROC )
                                gl_func("glEndConditionalRender");

        if (exEndConditionalRender)
        {
            exEndConditionalRender();
        }
        else
        {
            AE_WARN("glEndConditionalRender not found");
        }
    }
}

void GL_EndList(void)
{
    X(); glEndList();
}

void GL_EndQuery(unsigned int target)
{
    // TODO: let this be reset to NULL on quit
    static PFNGLENDQUERYPROC exEndQuery = NULL;

    X(); if (exEndQuery)
    {
        exEndQuery(target);
    }
    else
    {
        exEndQuery = ( PFNGLENDQUERYPROC )
                    gl_func("glEndQuery");

        if (exEndQuery)
        {
            exEndQuery(target);
        }
        else
        {
            AE_WARN("glEndQuery not found");
        }
    }
}

void GL_EndQueryIndexed(unsigned int target, unsigned int index)
{
    /* TODO allow this func to be reset to NULL on gl_quit */
    static PFNGLENDQUERYINDEXEDPROC exEndQueryIndexed = NULL;

    X(); if (exEndQueryIndexed)
    {
        exEndQueryIndexed(target, index);
    }
    else
    {
        exEndQueryIndexed = ( PFNGLENDQUERYINDEXEDPROC )
                            gl_func("glEndQueryIndexed");

        if (exEndQueryIndexed)
        {
            exEndQueryIndexed(target, index);
        }
        else
        {
            AE_WARN("glEndQueryIndexed not found");
        }
    }
}

void GL_EndTransformFeedback(void)
{
    // TODO: allow this function pointer to be reset to NULL on gl_quit
    static PFNGLENDTRANSFORMFEEDBACKPROC exEndTransformFeedback = NULL;

    X(); if (exEndTransformFeedback)
    {
        exEndTransformFeedback();
    }
    else
    {
        exEndTransformFeedback = ( PFNGLENDTRANSFORMFEEDBACKPROC )
                                gl_func("glEndTransformFeedback");

        if (exEndTransformFeedback)
        {
            exEndTransformFeedback();
        }
        else
        {
            AE_WARN("glEndTransformFeedback not found");
        }
    }
}

void GL_EvalCoord1d(double u)
{
    X(); glEvalCoord1d(u);
}

void GL_EvalCoord1dv(const double* u)
{
    X(); glEvalCoord1dv(u);
}

void GL_EvalCoord1f(float u)
{
    X(); glEvalCoord1f(u);
}

void GL_EvalCoord1fv(const float* u)
{
    X(); glEvalCoord1fv(u);
}

void GL_EvalCoord2d(double u, double v)
{
    X(); glEvalCoord2d(u, v);
}

void GL_EvalCoord2dv(const double* u)
{
    X(); glEvalCoord2dv(u);
}

void GL_EvalCoord2f(float u, float v)
{
    X(); glEvalCoord2f(u, v);
}

void GL_EvalCoord2fv(const float* u)
{
    X(); glEvalCoord2fv(u);
}

void GL_EvalMesh1(unsigned int mode, int i1, int i2)
{
    X(); glEvalMesh1(mode, i1, i2);
}

void GL_EvalMesh2(unsigned int mode, int i1, int i2, int j1, int j2)
{
    X(); glEvalMesh2(mode, i1, i2, j1, j2);
}

void GL_EvalPoint1(int i)
{
    X(); glEvalPoint1(i);
}

void GL_EvalPoint2(int i, int j)
{
    X(); glEvalPoint2(i, j);
}

void GL_FeedbackBuffer(int size, unsigned int type, float *buffer)
{
    X(); glFeedbackBuffer(size, type, buffer);
}

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

void GL_GenBuffers(int n, unsigned int *buffers)
{
    /* TODO: let this be set to NULL on gl_quit */
    static PFNGLGENBUFFERSPROC exGenBuffers = NULL;

    X(); if (exGenBuffers)
    {
        exGenBuffers(n, buffers);
    }
    else
    {
        exGenBuffers = (PFNGLGENBUFFERSPROC)
                    gl_func("glGenBuffers");

        if (exGenBuffers)
        {
            exGenBuffers(n, buffers);
        }
        else
        {
            AE_WARN("glGenBuffers not found");
        }
    }
}

// TODO: GL_GenFramebuffers

unsigned int GL_GenLists(int range)
{
    X(); return glGenLists(range);
}

// TODO: GL_GenProgramPipelines

// TODO: GL_GenQueries

// TODO: GL_GenRenderbuffers

// TODO: GL_GenSamplers

void GL_GenTextures(int n, unsigned int* textures)
{
    X(); glGenTextures(n, textures);
}

// TODO: GL_GenTransformFeedbacks

void GL_GenVertexArrays(int n, unsigned int *arrays)
{
    /* TODO: allow this func to be set to NULL on gl_quit */
    static PFNGLGENVERTEXARRAYSPROC exGenVertexArrays = NULL;

    X(); if (exGenVertexArrays)
    {
        exGenVertexArrays(n, arrays);
    }
    else
    {
        exGenVertexArrays = ( PFNGLGENVERTEXARRAYSPROC )
                            gl_func("glGenVertexArrays");

        if (exGenVertexArrays)
        {
            exGenVertexArrays(n, arrays);
        }
        else
        {
            AE_WARN("glGenVertexArrays not found");
        }
    }
}

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

int GL_GetAttribLocation(unsigned int program, const char* name)
{
    /* TODO: allow this gl func to be reset to NULL on gl_quit */
    static PFNGLGETATTRIBLOCATIONPROC exGetAttribLocation = NULL;

    X(); if (exGetAttribLocation)
    {
        return exGetAttribLocation(program, name);
    }
    else
    {
        exGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)
                            gl_func("glGetAttribLocation");

        if (exGetAttribLocation)
        {
            return exGetAttribLocation(program, name);
        }
        else
        {
            AE_WARN("glGetAttribLocation not found");
        }
    }

    return -1;
}

void GL_GetBooleanv(unsigned int pname, unsigned char* params)
{
    X(); glGetBooleanv(pname, params);
}

// TODO: GL_GetBooleani_v

// TODO: GL_GetBufferParameteri64v

// TODO: GL_GetBufferParameteriv

// TODO: GL_GetBufferPointerv

// TODO: GL_GetBufferSubData

void GL_GetClipPlane(unsigned int plane, double* equation)
{
    X(); glGetClipPlane(plane, equation);
}

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

void GL_GetDoublev(unsigned int pname, double* params)
{
    X(); glGetDoublev(pname, params);
}

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

void GL_GetLightiv(unsigned int light, unsigned int pname, int* params)
{
    X(); glGetLightiv(light, pname, params);
}

void GL_GetMapdv(unsigned int target, unsigned int query, double* v)
{
    X(); glGetMapdv(target, query, v);
}

void GL_GetMapfv(unsigned int target, unsigned int query, float* v)
{
    X(); glGetMapfv(target, query, v);
}

void GL_GetMapiv(unsigned int target, unsigned int query, int* v)
{
    X(); glGetMapiv(target, query, v);
}

void GL_GetMaterialfv(unsigned int face, unsigned int pname, float* params)
{
    X(); glGetMaterialfv(face, pname, params);
}

void GL_GetMaterialiv(unsigned int face, unsigned int pname, int* params)
{
    X(); glGetMaterialiv(face, pname, params);
}

// TODO: GL_GetMinmax

// TODO: GL_GetMinmaxParameterfv

// TODO: GL_GetMinmaxParameteriv

// TODO: GL_GetMultisamplefv

// TODO: GL_GetObjectLabel

// TODO: GL_GetObjectPtrLabel

void GL_GetPixelMapfv(unsigned int map, float* values)
{
    X(); glGetPixelMapfv(map, values);
}

void GL_GetPixelMapuiv(unsigned int map, unsigned int* values)
{
    X(); glGetPixelMapuiv(map, values);
}

void GL_GetPixelMapusv(unsigned int map, unsigned short* values)
{
    X(); glGetPixelMapusv(map, values);
}

// TODO: GL_GetPixelMapxv

void GL_GetPointerv(unsigned int pname, void** params)
{
    X(); glGetPointerv(pname, params);
}

void GL_GetPolygonStipple(unsigned char* mask)
{
    X(); glGetPolygonStipple(mask);
}

// TODO: GL_GetProgramBinary

void GL_GetProgramiv(unsigned int program, unsigned int pname, int* params)
{
    /* TODO: allow this to be set to NULL on gl_quit */
    static PFNGLGETPROGRAMIVPROC exGetProgramiv = NULL;

    X(); if (exGetProgramiv)
    {
        exGetProgramiv(program, pname, params);
    }
    else
    {
        exGetProgramiv = ( PFNGLGETPROGRAMIVPROC )
                        gl_func("glGetProgramiv");

        if (exGetProgramiv)
        {
            exGetProgramiv(program, pname, params);
        }
        else
        {
            AE_WARN("glGetProgramiv not found");
        }
    }
}

void GL_GetProgramInfoLog(unsigned int program, int buf_size, int* length, char* info_log)
{
    /* TODO: allow this function to be reset to NULL on gl_quit */
    static PFNGLGETPROGRAMINFOLOGPROC exGetProgramInfoLog = NULL;

    X(); if (exGetProgramInfoLog)
    {
        exGetProgramInfoLog(program, buf_size, length, info_log);
    }
    else
    {
        exGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)
                            gl_func("glGetProgramInfoLog");

        if (exGetProgramInfoLog)
        {
            exGetProgramInfoLog(program, buf_size, length, info_log);
        }
        else
        {
            AE_WARN("glGetProgramInfoLog not found");
        }
    }
}

// TODO: GL_GetProgramInterfaceiv

// TODO: GL_GetProgramPipelineInfoLog

// TODO: GL_GetProgramPipelineiv

// TODO: GL_GetProgramResourceIndex

// TODO: GL_GetProgramResourceLocation

// TODO: GL_GetProgramResourceLocationIndex

// TODO: GL_GetProgramResourceName

// TODO: GL_GetProgramResourceiv

// TODO: GL_GetProgramStageiv

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

void GL_GetShaderiv(unsigned int shader, unsigned int pname, int* params)
{
    /* TODO: let this be reset to NULL on gl_quit */
    static PFNGLGETSHADERIVPROC exGetShaderiv = NULL;

    X(); if (exGetShaderiv)
    {
        exGetShaderiv(shader, pname, params);
    }
    else
    {
        exGetShaderiv = ( PFNGLGETSHADERIVPROC )
                        gl_func("glGetShaderiv");

        if (exGetShaderiv)
        {
            exGetShaderiv(shader, pname, params);
        }
        else
        {
            AE_WARN("glGetShaderiv not found");
        }
    }
}

void GL_GetShaderInfoLog(unsigned int shader, int buf_size, int* length, char* info_log)
{
    // TODO: allow this function to be reset to NULL on gl_quit
    static PFNGLGETSHADERINFOLOGPROC exGetShaderInfoLog = NULL;

    X(); if (exGetShaderInfoLog)
    {
        exGetShaderInfoLog(shader, buf_size, length, info_log);
    }
    else
    {
        exGetShaderInfoLog = ( PFNGLGETSHADERINFOLOGPROC )
                            gl_func("glGetShaderInfoLog");

        if (exGetShaderInfoLog)
        {
            exGetShaderInfoLog(shader, buf_size, length, info_log);
        }
        else
        {
            AE_WARN("glGetShaderInfoLog not found");
        }
    }
}

// TODO: GL_GetShaderPrecisionFormat

// TODO: GL_GetShaderSource

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

void GL_GetTexImage(unsigned int target, int level, unsigned int format,
                                        unsigned int type, void* pixels)
{
    X(); glGetTexImage(target, level, format, type, pixels);
}

void GL_GetTexLevelParameterfv(unsigned int target, int level,
                            unsigned int pname, float* params)
{
    X(); glGetTexLevelParameterfv(target, level, pname, params);
}

void GL_GetTexLevelParameteriv(unsigned int target, int level,
                            unsigned int pname, int * params)
{
    X(); glGetTexLevelParameteriv(target, level, pname, params);
}

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

void GL_Hint(unsigned int target, unsigned int mode)
{
    X(); glHint(target, mode);
}

// TODO: GL_Histogram

void GL_IndexMask(unsigned int mask)
{
    X(); glIndexMask(mask);
}

void GL_IndexPointer(unsigned int type, int stride, const void* ptr)
{
    X(); glIndexPointer(type, stride, ptr);
}

void GL_Indexd(double c)
{
    X(); glIndexd(c);
}

void GL_Indexdv(const double* c)
{
    X(); glIndexdv(c);
}

void GL_Indexf(float c)
{
    X(); glIndexf(c);
}

void GL_Indexfv(const float* c)
{
    X(); glIndexfv(c);
}

void GL_Indexi(int c)
{
    X(); glIndexi(c);
}

void GL_Indexiv(const int* c)
{
    X(); glIndexiv(c);
}

void GL_Indexs(short c)
{
    X(); glIndexs(c);
}

void GL_Indexsv(const short* c)
{
    X(); glIndexsv(c);
}

void GL_Indexub(unsigned char c)
{
    X(); glIndexub(c);
}

void GL_Indexubv(const unsigned char* c)
{
    X(); glIndexubv(c);
}

void GL_InitNames(void)
{
    X(); glInitNames();
}

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

unsigned char GL_IsBuffer(unsigned int buffer)
{
    // TODO: let this be reset to NULL on gl_quit
    static PFNGLISBUFFERPROC exIsBuffer = NULL;

    X(); if (exIsBuffer)
    {
        return exIsBuffer(buffer);
    }
    else
    {
        exIsBuffer = ( PFNGLISBUFFERPROC )
                    gl_func("glIsBuffer");

        if (exIsBuffer)
        {
            return exIsBuffer(buffer);
        }
        else
        {
            AE_WARN("glIsBuffer not found");
        }
    }

    return 0;
}

unsigned char GL_IsEnabled(unsigned int cap)
{
    X(); return glIsEnabled(cap);
}

// TODO: GL_IsEnabledi

// TODO: GL_IsFramebuffer

unsigned char GL_IsList(unsigned int list)
{
    X(); return glIsList(list);
}

unsigned char GL_IsProgram(unsigned int program)
{
    // TODO: let this be reset to NULL on gl_quit
    static PFNGLISPROGRAMPROC exIsProgram = NULL;

    X(); if (exIsProgram)
    {
        return exIsProgram(program);
    }
    else
    {
        exIsProgram = (PFNGLISPROGRAMPROC)
                    gl_func("glIsProgram");

        if (exIsProgram)
        {
            return exIsProgram(program);
        }
        else
        {
            AE_WARN("glIsProgram not found");
        }
    }

    return 0;
}

// TODO: GL_IsProgramPipeline

// TODO: GL_IsQuery

// TODO: GL_IsRenderbuffer

// TODO: GL_IsSampler

unsigned char GL_IsShader(unsigned int shader)
{
    // TODO: let this be set to NULL on gl_quit
    static PFNGLISSHADERPROC exIsShader = NULL;

    X(); if (exIsShader)
    {
        return exIsShader(shader);
    }
    else
    {
        exIsShader = ( PFNGLISSHADERPROC )
                    gl_func("glIsShader");

        if (exIsShader)
        {
            return exIsShader(shader);
        }
        else
        {
            AE_WARN("glIsShader not found");
        }
    }

    return 0;
}

// TODO: GL_IsSync

// TODO: GL_IsTransformFeedback

unsigned char GL_IsTexture(unsigned int texture)
{
    X(); return glIsTexture(texture);
}

unsigned char GL_IsVertexArray(unsigned int array)
{
    /* TODO: allow this to be reset to NULL on gl_quit */
    static PFNGLISVERTEXARRAYPROC exIsVertexArray = NULL;

    X(); if (exIsVertexArray)
    {
        return exIsVertexArray(array);
    }
    else
    {
        exIsVertexArray = ( PFNGLISVERTEXARRAYPROC )
                    gl_func("glIsVertexArray");

        if (exIsVertexArray)
        {
            return exIsVertexArray(array);
        }
        else
        {
            AE_WARN("glIsVertexArray not found");
        }
    }

    return 0;
}

void GL_LightModelf(unsigned int pname, float param)
{
    X(); glLightModelf(pname, param);
}

void GL_LightModelfv(unsigned int pname, const float* params)
{
    X(); glLightModelfv(pname, params);
}

void GL_LightModeli(unsigned int pname, int param)
{
    X(); glLightModeli(pname, param);
}

void GL_LightModeliv(unsigned int pname, const int* params)
{
    X(); glLightModeliv(pname, params);
}

void GL_Lightf(unsigned int light, unsigned int pname, float param)
{
    X(); glLightf(light, pname, param);
}

void GL_Lightfv(unsigned int light, unsigned int pname, const float* params)
{
    X(); glLightfv(light, pname, params);
}

void GL_Lighti(unsigned int light, unsigned int pname, int param)
{
    X(); glLighti(light, pname, param);
}

void GL_Lightiv(unsigned int light, unsigned int pname, const int* params)
{
    X(); glLightiv(light, pname, params);
}

void GL_LineStipple(int factor, unsigned short pattern)
{
    X(); glLineStipple(factor, pattern);
}

void GL_LineWidth(float width)
{
    X(); glLineWidth(width);
}

void GL_LinkProgram(unsigned int program)
{
    /* TODO: let this be reset to NULL on gl_quit */
    static PFNGLLINKPROGRAMPROC exLinkProgram = NULL;

    X(); if (exLinkProgram)
    {
        exLinkProgram(program);
    }
    else
    {
        exLinkProgram = ( PFNGLLINKPROGRAMPROC )
                        gl_func("glLinkProgram");

        if (exLinkProgram)
        {
            exLinkProgram(program);
        }
        else
        {
            AE_WARN("glLinkProgram not found");
        }
    }
}

void GL_ListBase(unsigned int base)
{
    X(); glListBase(base);
}

void GL_LoadIdentity(void)
{
    X(); glLoadIdentity();
}

void GL_LoadMatrixd(const double* m)
{
    X(); glLoadMatrixd(m);
}

void GL_LoadMatrixf(const float* m)
{
    X(); glLoadMatrixf(m);
}

void GL_LoadName(unsigned int name)
{
    X(); glLoadName(name);
}

// TODO: GL_LoadTransposeMatrixd

// TODO: GL_LoadTransposeMatrixf

void GL_LogicOp(unsigned int opcode)
{
    X(); glLogicOp(opcode);
}

void GL_Map1d(unsigned int target, double u1, double u2, int stride,
                                    int order, const double* points)
{
    X(); glMap1d(target, u1, u2, stride, order, points);
}

void GL_Map1f(unsigned int target, float u1, float u2, int stride,
                                int order, const float * points)
{
    X(); glMap1f(target, u1, u2, stride, order, points);
}

void GL_Map2d(unsigned int target, double u1, double u2, int ustride, int uorder,
            double v1, double v2, int vstride, int vorder, const double* points)
{
    X(); glMap2d(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
}

void GL_Map2f(unsigned int target, float u1, float u2, int ustride, int uorder,
            float v1, float v2, int vstride, int vorder, const float * points)
{
    X(); glMap2f(target, u1, u2, ustride, uorder, v1, v2, vstride, vorder, points);
}

void* GL_MapBuffer(unsigned int target, unsigned int access)
{
    // TODO: let this be reset to NULL on gl_quit
    static PFNGLMAPBUFFERPROC exMapBuffer = NULL;

    X(); if (exMapBuffer)
    {
        return exMapBuffer(target, access);
    }
    else
    {
        exMapBuffer = (PFNGLMAPBUFFERPROC)
                    gl_func("glMapBuffer");

        if (exMapBuffer)
        {
            return exMapBuffer(target, access);
        }
        else
        {
            AE_WARN("glMapBuffer not found");
        }
    }

    return NULL;
}

void* GL_MapBufferRange(unsigned int target, ptrdiff_t offset,
                        ptrdiff_t length, unsigned int access)
{
    /* TODO let this pointer be reset to NULL on gl_quit */
    static PFNGLMAPBUFFERRANGEPROC exMapBufferRange = NULL;

    X(); if (exMapBufferRange)
    {
        return exMapBufferRange(target, offset, length, access);
    }
    else
    {
        exMapBufferRange = (PFNGLMAPBUFFERRANGEPROC)
                        gl_func("glMapBufferRange");

        if (exMapBufferRange)
        {
            return exMapBufferRange(target, offset, length, access);
        }
        else
        {
            AE_WARN("glMapBufferRange not found");
        }
    }

    return NULL;
}

void GL_MapGrid1d(int un, double u1, double u2)
{
    X(); glMapGrid1d(un, u1, u2);
}

void GL_MapGrid1f(int un, float u1, float u2)
{
    X(); glMapGrid1f(un, u1, u2);
}

void GL_MapGrid2d(int un, double u1, double u2, int vn, double v1, double v2)
{
    X(); glMapGrid2d(un, u1, u2, vn, v1, v2);
}

void GL_MapGrid2f(int un, float u1, float u2, int vn, float v1, float v2)
{
    X(); glMapGrid2f(un, u1, u2, vn, v1, v2);
}

void GL_Materialf(unsigned int face, unsigned int pname, float param)
{
    X(); glMaterialf(face, pname, param);
}

void GL_Materialfv(unsigned int face, unsigned int pname, const float* params)
{
    X(); glMaterialfv(face, pname, params);
}

void GL_Materiali(unsigned int face, unsigned int pname, int param)
{
    X(); glMateriali(face, pname, param);
}

void GL_Materialiv(unsigned int face, unsigned int pname, const int* params)
{
    X(); glMaterialiv(face, pname, params);
}

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

void GL_MultMatrixd(const double *m)
{
    X(); glMultMatrixd(m);
}

void GL_MultMatrixf(const float* matrix)
{
    X(); glMultMatrixf(matrix);
}

// TODO: GL_MultTransposeMatrixd

// TODO: GL_MultTransposeMatrixf

void GL_NewList(unsigned int list, unsigned int mode)
{
    X(); glNewList(list, mode);
}

void GL_Normal3b(char nx, char ny, char nz)
{
    X(); glNormal3b(nx, ny, nz);
}

void GL_Normal3bv(const char *v)
{
    X(); glNormal3bv((const GLbyte*)v);
}

void GL_Normal3d(double nx, double ny, double nz)
{
    X(); glNormal3d(nx, ny, nz);
}

void GL_Normal3dv(const double *v)
{
    X(); glNormal3dv(v);
}

void GL_Normal3f(float x, float y, float z)
{
    X(); glNormal3f(x, y, z);
}

void GL_Normal3fv(const float* normal)
{
    X(); glNormal3fv(normal);
}

void GL_Normal3i(int nx, int ny, int nz)
{
    X(); glNormal3i(nx, ny, nz);
}

void GL_Normal3iv(const int* v)
{
    X(); glNormal3iv(v);
}

void GL_Normal3s(short nx, short ny, short nz)
{
    X(); glNormal3s(nx, ny, nz);
}

void GL_Normal3sv(const short* v)
{
    X(); glNormal3sv(v);
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

void GL_PassThrough(float token)
{
    X(); glPassThrough(token);
}

// TODO: GL_PatchParameterfv

// TODO: GL_PatchParameteri

// TODO: GL_PauseTransformFeedback

void GL_PixelMapfv(unsigned int map, int mapsize, const float *values)
{
    X(); glPixelMapfv(map, mapsize, values);
}

void GL_PixelMapuiv(unsigned int map, int mapsize, const unsigned int *values)
{
    X(); glPixelMapuiv(map, mapsize, values);
}

void GL_PixelMapusv(unsigned int map, int mapsize, const unsigned short *values)
{
    X(); glPixelMapusv(map, mapsize, values);
}

// TODO: GL_PixelMapx

void GL_PixelStoref(unsigned int pname, float param)
{
    X(); glPixelStoref(pname, param);
}

void GL_PixelStorei(unsigned int pname, int param)
{
    X(); glPixelStorei(pname, param);
}

// TODO: GL_PixelStorex

void GL_PixelTransferf(unsigned int pname, float param)
{
    X(); glPixelTransferf(pname, param);
}

void GL_PixelTransferi(unsigned int pname, int param)
{
    X(); glPixelTransferi(pname, param);
}

void GL_PixelZoom(float xfactor, float yfactor)
{
    X(); glPixelZoom(xfactor, yfactor);
}

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

void GL_PolygonStipple(const unsigned char *mask)
{
    X(); glPolygonStipple(mask);
}

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

void GL_PopName(void)
{
    X(); glPopName();
}

void GL_PushName(unsigned int name)
{
    X(); glPushName(name);
}

// TODO: GL_PrimitiveRestartIndex

void GL_PrioritizeTextures(int n, const unsigned int* textures, const float* priorities)
{
    X(); glPrioritizeTextures(n, textures, priorities);
}

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

void GL_ProvokingVertex(unsigned int mode)
{
    // TODO: allow this to be NULLed out on gl_quit for safety
    static PFNGLPROVOKINGVERTEXPROC exProvokingVertex = NULL;

    X(); if (exProvokingVertex)
    {
        exProvokingVertex(mode);
    }
    else
    {
        exProvokingVertex = ( PFNGLPROVOKINGVERTEXPROC )
                            gl_func("glProvokingVertex");

        if (exProvokingVertex)
        {
            exProvokingVertex(mode);
        }
        else
        {
            AE_WARN("glProvokingVertex not found");
        }
    }
}

// TODO: GL_QueryCounter

void GL_RasterPos2d(double x, double y)
{
    X(); glRasterPos2d(x, y);
}

void GL_RasterPos2dv(const double *v)
{
    X(); glRasterPos2dv(v);
}

void GL_RasterPos2f(float x, float y)
{
    X(); glRasterPos2f(x, y);
}

void GL_RasterPos2fv(const float *v)
{
    X(); glRasterPos2fv(v);
}

void GL_RasterPos2i(int x, int y)
{
    X(); glRasterPos2i(x, y);
}

void GL_RasterPos2iv(const int *v)
{
    X(); glRasterPos2iv(v);
}

void GL_RasterPos2s(short x, short y)
{
    X(); glRasterPos2s(x, y);
}

void GL_RasterPos2sv(const short *v)
{
    X(); glRasterPos2sv(v);
}

void GL_RasterPos3d(double x, double y, double z)
{
    X(); glRasterPos3d(x, y, z);
}

void GL_RasterPos3dv(const double *v)
{
    X(); glRasterPos3dv(v);
}

void GL_RasterPos3f(float x, float y, float z)
{
    X(); glRasterPos3f(x, y, z);
}

void GL_RasterPos3fv(const float *v)
{
    X(); glRasterPos3fv(v);
}

void GL_RasterPos3i(int x, int y, int z)
{
    X(); glRasterPos3i(x, y, z);
}

void GL_RasterPos3iv(const int *v)
{
    X(); glRasterPos3iv(v);
}

void GL_RasterPos3s(short x, short y, short z)
{
    X(); glRasterPos3s(x, y, z);
}

void GL_RasterPos3sv(const short *v)
{
    X(); glRasterPos3sv(v);
}

void GL_RasterPos4d(double x, double y, double z, double w)
{
    X(); glRasterPos4d(x, y, z, w);
}

void GL_RasterPos4dv(const double *v)
{
    X(); glRasterPos4dv(v);
}

void GL_RasterPos4f(float x, float y, float z, float w)
{
    X(); glRasterPos4f(x, y, z, w);
}

void GL_RasterPos4fv(const float *v)
{
    X(); glRasterPos4fv(v);
}

void GL_RasterPos4i(int x, int y, int z, int w)
{
    X(); glRasterPos4i(x, y, z, w);
}

void GL_RasterPos4iv(const int *v)
{
    X(); glRasterPos4iv(v);
}

void GL_RasterPos4s(short x, short y, short z, short w)
{
    X(); glRasterPos4s(x, y, z, w);
}

void GL_RasterPos4sv(const short *v)
{
    X(); glRasterPos4sv(v);
}

void GL_ReadBuffer(unsigned int mode)
{
    X(); glReadBuffer(mode);
}

void GL_ReadPixels(int x, int y, int width, int height, unsigned int format,
                                            unsigned int type, void *pixels)
{
    X(); glReadPixels(x, y, width, height, format, type, pixels);
}

void GL_Rectd(double x1, double y1, double x2, double y2)
{
    X(); glRectd(x1, y1, x2, y2);
}

void GL_Rectdv(const double *v1, const double *v2)
{
    X(); glRectdv(v1, v2);
}

void GL_Rectf(float x1, float y1, float x2, float y2)
{
    X(); glRectf(x1, y1, x2, y2);
}

void GL_Rectfv(const float *v1, const float *v2)
{
    X(); glRectfv(v1, v2);
}

void GL_Recti(int x1, int y1, int x2, int y2)
{
    X(); glRecti(x1, y1, x2, y2);
}

void GL_Rectiv(const int *v1, const int *v2)
{
    X(); glRectiv(v1, v2);
}

void GL_Rects(short x1, short y1, short x2, short y2)
{
    X(); glRects(x1, y1, x2, y2);
}

void GL_Rectsv(const short *v1, const short *v2)
{
    X(); glRectsv(v1, v2);
}

// TODO: GL_ReleaseShaderCompiler

// TODO: GL_RenderbufferStorage

// TODO: GL_RenderbufferStorageMultisample

int GL_RenderMode(unsigned int mode)
{
    X(); return glRenderMode(mode);
}

// TODO: GL_ResetHistogram

// TODO: GL_ResetMinmax

// TODO: GL_ResumeTransformFeedback

void GL_Rotated(double angle, double x, double y, double z)
{
    X(); glRotated(angle, x, y, z);
}

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

void GL_Scaled(double x, double y, double z)
{
    X(); glScaled(x, y, z);
}

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

void GL_SelectBuffer(int size, unsigned int* buffer)
{
    X(); glSelectBuffer(size, buffer);
}

// TODO: GL_SeparableFilter2D

void GL_ShadeModel(unsigned int mode)
{
    X(); glShadeModel(mode);
}

// TODO: GL_ShaderBinary

void GL_ShaderSource(unsigned int shader, int count, const char** const string,
                                                            const int * length)
{
    /* TODO: allow this to be set to NULL on gl_quit */
    static PFNGLSHADERSOURCEPROC exShaderSource = NULL;

    X(); if (exShaderSource)
    {
        exShaderSource(shader, count, string, length);
    }
    else
    {
        exShaderSource = ( PFNGLSHADERSOURCEPROC )
                        gl_func("glShaderSource");

        if (exShaderSource)
        {
            exShaderSource(shader, count, string, length);
        }
        else
        {
            AE_WARN("glShaderSource not found");
        }
    }
}

// TODO: GL_ShaderStorageBlockBinding

void GL_StencilFunc(unsigned int func, int ref, unsigned int mask)
{
    X(); glStencilFunc(func, ref, mask);
}

// TODO: GL_StencilFuncSeparate

void GL_StencilMask(unsigned int mask)
{
    X(); glStencilMask(mask);
}

// TODO: GL_StencilMaskSeparate

void GL_StencilOp(unsigned int fail, unsigned int zfail, unsigned int zpass)
{
    X(); glStencilOp(fail, zfail, zpass);
}

// TODO: GL_StencilOpSeparate

// TODO: GL_TexBuffer

// TODO: GL_TexBufferRange

void GL_TexCoord1d(double s)
{
    X(); glTexCoord1d(s);
}

void GL_TexCoord1dv(const double *v)
{
    X(); glTexCoord1dv(v);
}

void GL_TexCoord1f(float s)
{
    X(); glTexCoord1f(s);
}

void GL_TexCoord1fv(const float *v)
{
    X(); glTexCoord1fv(v);
}

void GL_TexCoord1i(int s)
{
    X(); glTexCoord1i(s);
}

void GL_TexCoord1iv(const int *v)
{
    X(); glTexCoord1iv(v);
}

void GL_TexCoord1s(short s)
{
    X(); glTexCoord1s(s);
}

void GL_TexCoord1sv(const short *v)
{
    X(); glTexCoord1sv(v);
}

void GL_TexCoord2d(double s, double t)
{
    X(); glTexCoord2d(s, t);
}

void GL_TexCoord2dv(const double *v)
{
    X(); glTexCoord2dv(v);
}

void GL_TexCoord2f(float s, float t)
{
    X(); glTexCoord2f(s, t);
}

void GL_TexCoord2fv(const float* st)
{
    X(); glTexCoord2fv(st);
}

void GL_TexCoord2i(int s, int t)
{
    X(); glTexCoord2i(s, t);
}

void GL_TexCoord2iv(const int *v)
{
    X(); glTexCoord2iv(v);
}

void GL_TexCoord2s(short s, short t)
{
    X(); glTexCoord2s(s, t);
}

void GL_TexCoord2sv(const short *v)
{
    X(); glTexCoord2sv(v);
}

void GL_TexCoord3d(double s, double t, double r)
{
    X(); glTexCoord3d(s, t, r);
}

void GL_TexCoord3dv(const double *v)
{
    X(); glTexCoord3dv(v);
}

void GL_TexCoord3f(float s, float t, float r)
{
    X(); glTexCoord3f(s, t, r);
}

void GL_TexCoord3fv(const float* str)
{
    X(); glTexCoord3fv(str);
}

void GL_TexCoord3i(int s, int t, int r)
{
    X(); glTexCoord3i(s, t, r);
}

void GL_TexCoord3iv(const int *v)
{
    X(); glTexCoord3iv(v);
}

void GL_TexCoord3s(short s, short t, short r)
{
    X(); glTexCoord3s(s, t, r);
}

void GL_TexCoord3sv(const short *v)
{
    X(); glTexCoord3sv(v);
}

void GL_TexCoord4d(double s, double t, double r, double q)
{
    X(); glTexCoord4d(s, t, r, q);
}

void GL_TexCoord4dv(const double *v)
{
    X(); glTexCoord4dv(v);
}

void GL_TexCoord4f(float s, float t, float r, float q)
{
    X(); glTexCoord4f(s, t, r, q);
}

void GL_TexCoord4fv(const float* strq)
{
    X(); glTexCoord4fv(strq);
}

void GL_TexCoord4i(int s, int t, int r, int q)
{
    X(); glTexCoord4i(s, t, r, q);
}

void GL_TexCoord4iv(const int *v)
{
    X(); glTexCoord4iv(v);
}

void GL_TexCoord4s(short s, short t, short r, short q)
{
    X(); glTexCoord4s(s, t, r, q);
}

void GL_TexCoord4sv(const short *v)
{
    X(); glTexCoord4sv(v);
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

void GL_TexImage1D(unsigned int target, int level, int internal_format, int w,
            int border, unsigned int format, unsigned int type, const void* p)
{
    X(); glTexImage1D(target, level, internal_format, w, border, format, type, p);
}

void GL_TexImage2D(unsigned int target, int level, int internal_format, int w,
        int h, int border, unsigned int fmt, unsigned int type, const void* p)
{
    X(); glTexImage2D(target, level, internal_format, w, h, border, fmt, type, p);
}

// TODO: GL_TexImage2DMultisample

// TODO: GL_TexImage3D

// TODO: GL_TexImage3DMultisample

void GL_TexParameterf(unsigned int target, unsigned int pname, float param)
{
    X(); glTexParameterf(target, pname, param);
}

void GL_TexParameterfv(unsigned int target, unsigned int pname, const float *params)
{
    X(); glTexParameterfv(target, pname, params);
}

void GL_TexParameteri(unsigned int target, unsigned int pname, int param)
{
    X(); glTexParameteri(target, pname, param);
}

void GL_TexParameteriv(unsigned int target, unsigned int pname, const int *params)
{
    X(); glTexParameteriv(target, pname, params);
}

// TODO: GL_TexParameterIiv

// TODO: GL_TexParameterIuiv

// TODO: GL_TexStorage1D

// TODO: GL_TexStorage2D

// TODO: GL_TexStorage2DMultisample

// TODO: GL_TexStorage3D

// TODO: GL_TexStorage3DMultisample

void GL_TexSubImage1D(unsigned int target, int level, int xoffset, int width,
                unsigned int format, unsigned int type, const void * pixels)
{
    X(); glTexSubImage1D(target, level, xoffset, width, format, type, pixels);
}

void GL_TexSubImage2D(unsigned int target, int level, int xoffset, int yoffset,
        int w, int h, unsigned int format, unsigned int type, const void* pix)
{
    X(); glTexSubImage2D(target, level, xoffset, yoffset, w, h, format, type, pix);
}

// TODO: GL_TexSubImage3D

// TODO: GL_TextureView

// TODO: GL_TransformFeedbackVaryings

void GL_Translated(double x, double y, double z)
{
    X(); glTranslated(x, y, z);
}

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

unsigned char GL_UnmapBuffer(unsigned int target)
{
    /* TODO: let this be reset to NULL on gl_quit */
    static PFNGLUNMAPBUFFERPROC exUnmapBuffer = NULL;

    X(); if (exUnmapBuffer)
    {
        return exUnmapBuffer(target);
    }
    else
    {
        exUnmapBuffer = ( PFNGLUNMAPBUFFERPROC )
                        gl_func("glUnmapBuffer");

        if (exUnmapBuffer)
        {
            return exUnmapBuffer(target);
        }
        else
        {
            AE_WARN("glUnmapBuffer not found");
        }
    }

    return 0;
}

void GL_UseProgram(unsigned int program)
{
    /* TODO: let this be set to NULL on gl_quit */
    static PFNGLUSEPROGRAMPROC exUseProgram = NULL;

    X(); if (exUseProgram)
    {
        exUseProgram(program);
    }
    else
    {
        exUseProgram = (PFNGLUSEPROGRAMPROC)
                    gl_func("glUseProgram");

        if (exUseProgram)
        {
            exUseProgram(program);
        }
        else
        {
            AE_WARN("glUseProgram not found");
        }
    }
}

// TODO: GL_UseProgramStages

void GL_ValidateProgram(unsigned int program)
{
    /* TODO: let this pointer be reset to NULL on gl_quit */
    static PFNGLVALIDATEPROGRAMPROC exValidateProgram = NULL;

    X(); if (exValidateProgram)
    {
        exValidateProgram(program);
    }
    else
    {
        exValidateProgram = ( PFNGLVALIDATEPROGRAMPROC )
                            gl_func("glValidateProgram");

        if (exValidateProgram)
        {
            exValidateProgram(program);
        }
        else
        {
            AE_WARN("glValidateProgram not found");
        }
    }
}

// TODO: GL_ValidateProgramPipeline

void GL_Vertex2d(double x, double y)
{
    X(); glVertex2d(x, y);
}

void GL_Vertex2dv(const double *v)
{
    X(); glVertex2dv(v);
}

void GL_Vertex2f(float x, float y)
{
    X(); glVertex2f(x, y);
}

void GL_Vertex2fv(const float* xy)
{
    X(); glVertex2fv(xy);
}

void GL_Vertex2i(int x, int y)
{
    X(); glVertex2i(x, y);
}

void GL_Vertex2iv(const int *v)
{
    X(); glVertex2iv(v);
}

void GL_Vertex2s(short x, short y)
{
    X(); glVertex2s(x, y);
}

void GL_Vertex2sv(const short *v)
{
    X(); glVertex2sv(v);
}

void GL_Vertex3d(double x, double y, double z)
{
    X(); glVertex3d(x, y, z);
}

void GL_Vertex3dv(const double *v)
{
    X(); glVertex3dv(v);
}

void GL_Vertex3f(float x, float y, float z)
{
    X(); glVertex3f(x, y, z);
}

void GL_Vertex3fv(const float* xyz)
{
    X(); glVertex3fv(xyz);
}

void GL_Vertex3i(int x, int y, int z)
{
    X(); glVertex3i(x, y, z);
}

void GL_Vertex3iv(const int *v)
{
    X(); glVertex3iv(v);
}

void GL_Vertex3s(short x, short y, short z)
{
    X(); glVertex3s(x, y, z);
}

void GL_Vertex3sv(const short *v)
{
    X(); glVertex3sv(v);
}

void GL_Vertex4d(double x, double y, double z, double w)
{
    X(); glVertex4d(x, y, z, w);
}

void GL_Vertex4dv(const double *v)
{
    X(); glVertex4dv(v);
}

void GL_Vertex4f(float x, float y, float z, float w)
{
    X(); glVertex4f(x, y, z, w);
}

void GL_Vertex4fv(const float* xyzw)
{
    X(); glVertex4fv(xyzw);
}

void GL_Vertex4i(int x, int y, int z, int w)
{
    X(); glVertex4i(x, y, z, w);
}

void GL_Vertex4iv(const int *v)
{
    X(); glVertex4iv(v);
}

void GL_Vertex4s(short x, short y, short z, short w)
{
    X(); glVertex4s(x, y, z, w);
}

void GL_Vertex4sv(const short *v)
{
    X(); glVertex4sv(v);
}

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

void GL_VertexAttribPointer(unsigned int index, int size, unsigned int type,
                unsigned char normalized, int stride, const void * pointer)
{
    /* TODO: allow this gl func ptr to be reset to NULL on gl_quit */
    static PFNGLVERTEXATTRIBPOINTERPROC exVertexAttribPointer = NULL;

    X(); if (exVertexAttribPointer)
    {
        exVertexAttribPointer(index, size, type, normalized, stride, pointer);
    }
    else
    {
        exVertexAttribPointer = ( PFNGLVERTEXATTRIBPOINTERPROC )
                                gl_func("glVertexAttribPointer");

        if (exVertexAttribPointer)
        {
            exVertexAttribPointer(index, size, type, normalized, stride, pointer);
        }
        else
        {
            AE_WARN("glVertexAttribPointer not found");
        }
    }
}

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
 * ~~ [ init & quit ] ~~ *
--------------------------------------------------------------------------------
*/

void gl_wrapper_init(void)
{
    //
}

void gl_wrapper_quit(void)
{
    //
}
