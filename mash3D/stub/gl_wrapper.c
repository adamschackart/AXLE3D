/* -----------------------------------------------------------------------------
--- Copyright (c) 2012-2017 Adam Schackart / "AJ Hackman", all rights reserved.
--- Distributed under the BSD license v2 (opensource.org/licenses/BSD-3-Clause)
----------------------------------------------------------------------------- */
#ifndef __GL_CORE_H__
#include <gl_core.h>
#endif

/*
================================================================================
 * ~~ [ OpenGL wrapper ] ~~ *
--------------------------------------------------------------------------------
*/

void GL_Accum(unsigned int op, float value)
{
    AE_STUB();
}

void GL_ActiveShaderProgram(unsigned int pipeline, unsigned int program)
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

unsigned char GL_AreTexturesResident(int n, const unsigned int* tex_array,
                                                unsigned char* residences)
{
    AE_STUB(); return 0;
}

void GL_ArrayElement(int i)
{
    AE_STUB();
}

void GL_AttachShader(unsigned int program, unsigned int shader)
{
    AE_STUB();
}

void GL_Begin(unsigned int mode)
{
    AE_STUB();
}

void GL_BeginConditionalRender(unsigned int id, unsigned int mode)
{
    AE_STUB();
}

void GL_BeginQuery(unsigned int target, unsigned int id)
{
    AE_STUB();
}

void GL_BeginQueryIndexed(unsigned int target, unsigned int index, unsigned int id)
{
    AE_STUB();
}

void GL_BeginTransformFeedback(unsigned int primitiveMode)
{
    AE_STUB();
}

void GL_BindAttribLocation(unsigned int program, unsigned int index, const char *name)
{
    AE_STUB();
}

void GL_BindBuffer(unsigned int target, unsigned int buffer)
{
    AE_STUB();
}

void GL_BindTexture(unsigned int target, unsigned int texture)
{
    AE_STUB();
}

void GL_BindVertexArray(unsigned int array)
{
    AE_STUB();
}

void GL_Bitmap(int width, int height, float x_origin, float y_origin,
            float x_move, float y_move, const unsigned char * bitmap)
{
    AE_STUB();
}

void GL_BlendColor(float red, float green, float blue, float alpha)
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

void GL_BufferData(unsigned int target, ptrdiff_t size,
                const void * data, unsigned int usage)
{
    AE_STUB();
}

void GL_BufferSubData(unsigned int target, ptrdiff_t offset,
                        ptrdiff_t size, const void * data)
{
    AE_STUB();
}

void GL_CallList(unsigned int list)
{
    AE_STUB();
}

void GL_CallLists(int n, unsigned int type, const void *lists)
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

void GL_ClearStencil(int s)
{
    AE_STUB();
}

void GL_ClipPlane(unsigned int plane, const double *equation)
{
    AE_STUB();
}

void GL_Color3b(char red, char green, char blue)
{
    AE_STUB();
}

void GL_Color3bv(const char *v)
{
    AE_STUB();
}

void GL_Color3d(double red, double green, double blue)
{
    AE_STUB();
}

void GL_Color3dv(const double *v)
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

void GL_Color3i(int red, int green, int blue)
{
    AE_STUB();
}

void GL_Color3iv(const int *v)
{
    AE_STUB();
}

void GL_Color3s(short red, short green, short blue)
{
    AE_STUB();
}

void GL_Color3sv(const short *v)
{
    AE_STUB();
}

void GL_Color3ub(unsigned char red, unsigned char green, unsigned char blue)
{
    AE_STUB();
}

void GL_Color3ubv(const unsigned char *v)
{
    AE_STUB();
}

void GL_Color3ui(unsigned int red, unsigned int green, unsigned int blue)
{
    AE_STUB();
}

void GL_Color3uiv(const unsigned int *v)
{
    AE_STUB();
}

void GL_Color3us(unsigned short red, unsigned short green, unsigned short blue)
{
    AE_STUB();
}

void GL_Color3usv(const unsigned short *v)
{
    AE_STUB();
}

void GL_Color4b(char red, char green, char blue, char alpha)
{
    AE_STUB();
}

void GL_Color4bv(const char *v)
{
    AE_STUB();
}

void GL_Color4d(double red, double green, double blue, double alpha)
{
    AE_STUB();
}

void GL_Color4dv(const double *v)
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

void GL_Color4i(int red, int green, int blue, int alpha)
{
    AE_STUB();
}

void GL_Color4iv(const int *v)
{
    AE_STUB();
}

void GL_Color4s(short red, short green, short blue, short alpha)
{
    AE_STUB();
}

void GL_Color4sv(const short *v)
{
    AE_STUB();
}

void GL_Color4ub(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
    AE_STUB();
}

void GL_Color4ubv(const unsigned char *v)
{
    AE_STUB();
}

void GL_Color4ui(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha)
{
    AE_STUB();
}

void GL_Color4uiv(const unsigned int *v)
{
    AE_STUB();
}

void GL_Color4us(unsigned short red, unsigned short green, unsigned short blue, unsigned short alpha)
{
    AE_STUB();
}

void GL_Color4usv(const unsigned short *v)
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

void GL_CompileShader(unsigned int shader)
{
    AE_STUB();
}

void GL_CopyPixels(int x, int y, int width, int height, unsigned int type)
{
    AE_STUB();
}

void GL_CopyTexImage1D(unsigned int target, int level, unsigned int internal_format,
                                                int x, int y, int width, int border)
{
    AE_STUB();
}

void GL_CopyTexImage2D(unsigned int target, int level, unsigned int internal_format,
                                    int x, int y, int width, int height, int border)
{
    AE_STUB();
}

void GL_CopyTexSubImage1D(unsigned int target, int level, int xoffset,
                                            int x, int y, int width)
{
    AE_STUB();
}

void GL_CopyTexSubImage2D(unsigned int target, int level, int xoffset,
                    int yoffset, int x, int y, int width, int height)
{
    AE_STUB();
}

unsigned int GL_CreateProgram(void)
{
    AE_STUB(); return 0;
}

unsigned int GL_CreateShader(unsigned int type)
{
    AE_STUB(); return 0;
}

void GL_CullFace(unsigned int face)
{
    AE_STUB();
}

void GL_DeleteBuffers(int n, const unsigned int* buffers)
{
    AE_STUB();
}

void GL_DeleteLists(unsigned int list, int range)
{
    AE_STUB();
}

void GL_DeleteProgram(unsigned int program)
{
    AE_STUB();
}

void GL_DeleteTextures(int n, const unsigned int* textures)
{
    AE_STUB();
}

void GL_DeleteVertexArrays(int n, const unsigned int *arrays)
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

void GL_DisableVertexAttribArray(unsigned int index)
{
    AE_STUB();
}

void GL_DrawArrays(unsigned int mode, int first, int count)
{
    AE_STUB();
}

void GL_DrawBuffer(unsigned int mode)
{
    AE_STUB();
}

void GL_DrawElements(unsigned int mode, int n, unsigned int type, const void* inds)
{
    AE_STUB();
}

void GL_DrawPixels(int width, int height, unsigned int format,
                        unsigned int type, const void *pixels)
{
    AE_STUB();
}

void GL_EdgeFlag(unsigned char flag)
{
    AE_STUB();
}

void GL_EdgeFlagPointer(int stride, const void *ptr)
{
    AE_STUB();
}

void GL_EdgeFlagv(const unsigned char *flag)
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

void GL_EnableVertexAttribArray(unsigned int index)
{
    AE_STUB();
}

void GL_End(void)
{
    AE_STUB();
}

void GL_EndConditionalRender(void)
{
    AE_STUB();
}

void GL_EndList(void)
{
    AE_STUB();
}

void GL_EndQuery(unsigned int target)
{
    AE_STUB();
}

void GL_EndQueryIndexed(unsigned int target, unsigned int index)
{
    AE_STUB();
}

void GL_EndTransformFeedback(void)
{
    AE_STUB();
}

void GL_EvalCoord1d(double u)
{
    AE_STUB();
}

void GL_EvalCoord1dv(const double *u)
{
    AE_STUB();
}

void GL_EvalCoord1f(float u)
{
    AE_STUB();
}

void GL_EvalCoord1fv(const float *u)
{
    AE_STUB();
}

void GL_EvalCoord2d(double u, double v)
{
    AE_STUB();
}

void GL_EvalCoord2dv(const double *u)
{
    AE_STUB();
}

void GL_EvalCoord2f(float u, float v)
{
    AE_STUB();
}

void GL_EvalCoord2fv(const float *u)
{
    AE_STUB();
}

void GL_EvalMesh1(unsigned int mode, int i1, int i2)
{
    AE_STUB();
}

void GL_EvalMesh2(unsigned int mode, int i1, int i2, int j1, int j2)
{
    AE_STUB();
}

void GL_EvalPoint1(int i)
{
    AE_STUB();
}

void GL_EvalPoint2(int i, int j)
{
    AE_STUB();
}

void GL_FeedbackBuffer(int size, unsigned int type, float *buffer)
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

void GL_GenBuffers(int n, unsigned int *buffers)
{
    AE_STUB();
}

unsigned int GL_GenLists(int range)
{
    AE_STUB(); return 0;
}

void GL_GenTextures(int n, unsigned int* textures)
{
    AE_STUB();
}

void GL_GenVertexArrays(int n, unsigned int *arrays)
{
    AE_STUB();
}

int GL_GetAttribLocation(unsigned int program, const char *name)
{
    AE_STUB(); return -1;
}

void GL_GetBooleanv(unsigned int pname, unsigned char *params)
{
    AE_STUB();
}

void GL_GetClipPlane(unsigned int plane, double *equation)
{
    AE_STUB();
}

void GL_GetDoublev(unsigned int pname, double *params)
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

void GL_GetMapdv(unsigned int target, unsigned int query, double *v)
{
    AE_STUB();
}

void GL_GetMapfv(unsigned int target, unsigned int query, float *v)
{
    AE_STUB();
}

void GL_GetMapiv(unsigned int target, unsigned int query, int *v)
{
    AE_STUB();
}

void GL_GetMaterialfv(unsigned int face, unsigned int pname, float* params)
{
    AE_STUB();
}

void GL_GetMaterialiv(unsigned int face, unsigned int pname, int* params)
{
    AE_STUB();
}

void GL_GetPixelMapfv(unsigned int map, float *values)
{
    AE_STUB();
}

void GL_GetPixelMapuiv(unsigned int map, unsigned int *values)
{
    AE_STUB();
}

void GL_GetPixelMapusv(unsigned int map, unsigned short *values)
{
    AE_STUB();
}

void GL_GetPointerv(unsigned int pname, void **params)
{
    AE_STUB();
}

void GL_GetPolygonStipple(unsigned char *mask)
{
    AE_STUB();
}

void GL_GetProgramiv(unsigned int program, unsigned int pname, int *params)
{
    AE_STUB();
}

void GL_GetProgramInfoLog(unsigned int program, int buf_size, int *length, char *info_log)
{
    AE_STUB();
}

void GL_GetShaderiv(unsigned int shader, unsigned int pname, int *params)
{
    AE_STUB();
}

void GL_GetShaderInfoLog(unsigned int shader, int bufSize, int *length, char *infoLog)
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

void GL_GetTexImage(unsigned int target, int level, unsigned int format,
                                        unsigned int type, void *pixels)
{
    AE_STUB();
}

void GL_GetTexLevelParameterfv(unsigned int target, int level,
                            unsigned int pname, float *params)
{
    AE_STUB();
}

void GL_GetTexLevelParameteriv(unsigned int target, int level,
                            unsigned int pname, int * params)
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

void GL_IndexMask(unsigned int mask)
{
    AE_STUB();
}

void GL_IndexPointer(unsigned int type, int stride, const void* ptr)
{
    AE_STUB();
}

void GL_Indexd(double c)
{
    AE_STUB();
}

void GL_Indexdv(const double* c)
{
    AE_STUB();
}

void GL_Indexf(float c)
{
    AE_STUB();
}

void GL_Indexfv(const float* c)
{
    AE_STUB();
}

void GL_Indexi(int c)
{
    AE_STUB();
}

void GL_Indexiv(const int* c)
{
    AE_STUB();
}

void GL_Indexs(short c)
{
    AE_STUB();
}

void GL_Indexsv(const short* c)
{
    AE_STUB();
}

void GL_Indexub(unsigned char c)
{
    AE_STUB();
}

void GL_Indexubv(const unsigned char* c)
{
    AE_STUB();
}

void GL_InitNames(void)
{
    AE_STUB();
}

void GL_InterleavedArrays(unsigned int format, int stride, const void* pointer)
{
    AE_STUB();
}

unsigned char GL_IsBuffer(unsigned int buffer)
{
    AE_STUB(); return 0;
}

unsigned char GL_IsEnabled(unsigned int cap)
{
    AE_STUB(); return 0;
}

unsigned char GL_IsList(unsigned int list)
{
    AE_STUB(); return 0;
}

unsigned char GL_IsProgram(unsigned int program)
{
    AE_STUB(); return 0;
}

unsigned char GL_IsShader(unsigned int shader)
{
    AE_STUB(); return 0;
}

unsigned char GL_IsTexture(unsigned int texture)
{
    AE_STUB(); return 0;
}

unsigned char GL_IsVertexArray(unsigned int array)
{
    AE_STUB(); return 0;
}

void GL_LightModelf(unsigned int pname, float param)
{
    AE_STUB();
}

void GL_LightModelfv(unsigned int pname, const float* params)
{
    AE_STUB();
}

void GL_LightModeli(unsigned int pname, int param)
{
    AE_STUB();
}

void GL_LightModeliv(unsigned int pname, const int* params)
{
    AE_STUB();
}

void GL_Lightf(unsigned int light, unsigned int pname, float param)
{
    AE_STUB();
}

void GL_Lightfv(unsigned int light, unsigned int pname, const float* params)
{
    AE_STUB();
}

void GL_Lighti(unsigned int light, unsigned int pname, int param)
{
    AE_STUB();
}

void GL_Lightiv(unsigned int light, unsigned int pname, const int* params)
{
    AE_STUB();
}

void GL_LineStipple(int factor, unsigned short pattern)
{
    AE_STUB();
}

void GL_LineWidth(float width)
{
    AE_STUB();
}

void GL_LinkProgram(unsigned int program)
{
    AE_STUB();
}

void GL_ListBase(unsigned int base)
{
    AE_STUB();
}

void GL_LoadIdentity(void)
{
    AE_STUB();
}

void GL_LoadMatrixd(const double *m)
{
    AE_STUB();
}

void GL_LoadMatrixf(const float *m)
{
    AE_STUB();
}

void GL_LoadName(unsigned int name)
{
    AE_STUB();
}

void GL_LogicOp(unsigned int opcode)
{
    AE_STUB();
}

void GL_Map1d(unsigned int target, double u1, double u2, int stride,
                                    int order, const double* points)
{
    AE_STUB();
}

void GL_Map1f(unsigned int target, float u1, float u2, int stride,
                                int order, const float * points)
{
    AE_STUB();
}

void GL_Map2d(unsigned int target, double u1, double u2, int ustride, int uorder,
            double v1, double v2, int vstride, int vorder, const double* points)
{
    AE_STUB();
}

void GL_Map2f(unsigned int target, float u1, float u2, int ustride, int uorder,
            float v1, float v2, int vstride, int vorder, const float * points)
{
    AE_STUB();
}

void* GL_MapBuffer(unsigned int target, unsigned int access)
{
    AE_STUB(); return NULL;
}

void* GL_MapBufferRange(unsigned int target, ptrdiff_t offset,
                        ptrdiff_t length, unsigned int access)
{
    AE_STUB(); return NULL;
}

void GL_MapGrid1d(int un, double u1, double u2)
{
    AE_STUB();
}

void GL_MapGrid1f(int un, float u1, float u2)
{
    AE_STUB();
}

void GL_MapGrid2d(int un, double u1, double u2, int vn, double v1, double v2)
{
    AE_STUB();
}

void GL_MapGrid2f(int un, float u1, float u2, int vn, float v1, float v2)
{
    AE_STUB();
}

void GL_Materialf(unsigned int face, unsigned int pname, float param)
{
    AE_STUB();
}

void GL_Materialfv(unsigned int face, unsigned int pname, const float* params)
{
    AE_STUB();
}

void GL_Materiali(unsigned int face, unsigned int pname, int param)
{
    AE_STUB();
}

void GL_Materialiv(unsigned int face, unsigned int pname, const int* params)
{
    AE_STUB();
}

void GL_MatrixMode(unsigned int mode)
{
    AE_STUB();
}

void GL_MultMatrixd(const double *m)
{
    AE_STUB();
}

void GL_MultMatrixf(const float* matrix)
{
    AE_STUB();
}

void GL_NewList(unsigned int list, unsigned int mode)
{
    AE_STUB();
}

void GL_Normal3b(char nx, char ny, char nz)
{
    AE_STUB();
}

void GL_Normal3bv(const char *v)
{
    AE_STUB();
}

void GL_Normal3d(double nx, double ny, double nz)
{
    AE_STUB();
}

void GL_Normal3dv(const double *v)
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

void GL_Normal3i(int nx, int ny, int nz)
{
    AE_STUB();
}

void GL_Normal3iv(const int* v)
{
    AE_STUB();
}

void GL_Normal3s(short nx, short ny, short nz)
{
    AE_STUB();
}

void GL_Normal3sv(const short* v)
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

void GL_PassThrough(float token)
{
    AE_STUB();
}

void GL_PixelMapfv(unsigned int map, int mapsize, const float *values)
{
    AE_STUB();
}

void GL_PixelMapuiv(unsigned int map, int mapsize, const unsigned int *values)
{
    AE_STUB();
}

void GL_PixelMapusv(unsigned int map, int mapsize, const unsigned short *values)
{
    AE_STUB();
}

void GL_PixelStoref(unsigned int pname, float param)
{
    AE_STUB();
}

void GL_PixelStorei(unsigned int pname, int param)
{
    AE_STUB();
}

void GL_PixelTransferf(unsigned int pname, float param)
{
    AE_STUB();
}

void GL_PixelTransferi(unsigned int pname, int param)
{
    AE_STUB();
}

void GL_PixelZoom(float xfactor, float yfactor)
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

void GL_PolygonStipple(const unsigned char *mask)
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

void GL_PopName(void)
{
    AE_STUB();
}

void GL_PushName(unsigned int name)
{
    AE_STUB();
}

void GL_PrioritizeTextures(int n, const unsigned int* textures, const float* priorities)
{
    AE_STUB();
}

void GL_ProvokingVertex(unsigned int mode)
{
    AE_STUB();
}

void GL_RasterPos2d(double x, double y)
{
    AE_STUB();
}

void GL_RasterPos2dv(const double *v)
{
    AE_STUB();
}

void GL_RasterPos2f(float x, float y)
{
    AE_STUB();
}

void GL_RasterPos2fv(const float *v)
{
    AE_STUB();
}

void GL_RasterPos2i(int x, int y)
{
    AE_STUB();
}

void GL_RasterPos2iv(const int *v)
{
    AE_STUB();
}

void GL_RasterPos2s(short x, short y)
{
    AE_STUB();
}

void GL_RasterPos2sv(const short *v)
{
    AE_STUB();
}

void GL_RasterPos3d(double x, double y, double z)
{
    AE_STUB();
}

void GL_RasterPos3dv(const double *v)
{
    AE_STUB();
}

void GL_RasterPos3f(float x, float y, float z)
{
    AE_STUB();
}

void GL_RasterPos3fv(const float *v)
{
    AE_STUB();
}

void GL_RasterPos3i(int x, int y, int z)
{
    AE_STUB();
}

void GL_RasterPos3iv(const int *v)
{
    AE_STUB();
}

void GL_RasterPos3s(short x, short y, short z)
{
    AE_STUB();
}

void GL_RasterPos3sv(const short *v)
{
    AE_STUB();
}

void GL_RasterPos4d(double x, double y, double z, double w)
{
    AE_STUB();
}

void GL_RasterPos4dv(const double *v)
{
    AE_STUB();
}

void GL_RasterPos4f(float x, float y, float z, float w)
{
    AE_STUB();
}

void GL_RasterPos4fv(const float *v)
{
    AE_STUB();
}

void GL_RasterPos4i(int x, int y, int z, int w)
{
    AE_STUB();
}

void GL_RasterPos4iv(const int *v)
{
    AE_STUB();
}

void GL_RasterPos4s(short x, short y, short z, short w)
{
    AE_STUB();
}

void GL_RasterPos4sv(const short *v)
{
    AE_STUB();
}

void GL_ReadBuffer(unsigned int mode)
{
    AE_STUB();
}

void GL_ReadPixels(int x, int y, int width, int height, unsigned int format, unsigned int type, void *pixels)
{
    AE_STUB();
}

void GL_Rectd(double x1, double y1, double x2, double y2)
{
    AE_STUB();
}

void GL_Rectdv(const double *v1, const double *v2)
{
    AE_STUB();
}

void GL_Rectf(float x1, float y1, float x2, float y2)
{
    AE_STUB();
}

void GL_Rectfv(const float *v1, const float *v2)
{
    AE_STUB();
}

void GL_Recti(int x1, int y1, int x2, int y2)
{
    AE_STUB();
}

void GL_Rectiv(const int *v1, const int *v2)
{
    AE_STUB();
}

void GL_Rects(short x1, short y1, short x2, short y2)
{
    AE_STUB();
}

void GL_Rectsv(const short *v1, const short *v2)
{
    AE_STUB();
}

int GL_RenderMode(unsigned int mode)
{
    AE_STUB(); return 0;
}

void GL_Rotated(double angle, double x, double y, double z)
{
    AE_STUB();
}

void GL_Rotatef(float theta, float x, float y, float z)
{
    AE_STUB();
}

void GL_Scaled(double x, double y, double z)
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

void GL_SelectBuffer(int size, unsigned int* buffer)
{
    AE_STUB();
}

void GL_ShadeModel(unsigned int mode)
{
    AE_STUB();
}

void GL_ShaderSource(unsigned int shader, int count, const char** const string,
                                                            const int * length)
{
    AE_STUB();
}

void GL_StencilFunc(unsigned int func, int ref, unsigned int mask)
{
    AE_STUB();
}

void GL_StencilMask(unsigned int mask)
{
    AE_STUB();
}

void GL_StencilOp(unsigned int fail, unsigned int zfail, unsigned int zpass)
{
    AE_STUB();
}

void GL_TexCoord1d(double s)
{
    AE_STUB();
}

void GL_TexCoord1dv(const double *v)
{
    AE_STUB();
}

void GL_TexCoord1f(float s)
{
    AE_STUB();
}

void GL_TexCoord1fv(const float *v)
{
    AE_STUB();
}

void GL_TexCoord1i(int s)
{
    AE_STUB();
}

void GL_TexCoord1iv(const int *v)
{
    AE_STUB();
}

void GL_TexCoord1s(short s)
{
    AE_STUB();
}

void GL_TexCoord1sv(const short *v)
{
    AE_STUB();
}

void GL_TexCoord2d(double s, double t)
{
    AE_STUB();
}

void GL_TexCoord2dv(const double *v)
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

void GL_TexCoord2i(int s, int t)
{
    AE_STUB();
}

void GL_TexCoord2iv(const int *v)
{
    AE_STUB();
}

void GL_TexCoord2s(short s, short t)
{
    AE_STUB();
}

void GL_TexCoord2sv(const short *v)
{
    AE_STUB();
}

void GL_TexCoord3d(double s, double t, double r)
{
    AE_STUB();
}

void GL_TexCoord3dv(const double *v)
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

void GL_TexCoord3i(int s, int t, int r)
{
    AE_STUB();
}

void GL_TexCoord3iv(const int *v)
{
    AE_STUB();
}

void GL_TexCoord3s(short s, short t, short r)
{
    AE_STUB();
}

void GL_TexCoord3sv(const short *v)
{
    AE_STUB();
}

void GL_TexCoord4d(double s, double t, double r, double q)
{
    AE_STUB();
}

void GL_TexCoord4dv(const double *v)
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

void GL_TexCoord4i(int s, int t, int r, int q)
{
    AE_STUB();
}

void GL_TexCoord4iv(const int* v)
{
    AE_STUB();
}

void GL_TexCoord4s(short s, short t, short r, short q)
{
    AE_STUB();
}

void GL_TexCoord4sv(const short* v)
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

void GL_TexImage1D(unsigned int target, int level, int internal_format, int w,
            int border, unsigned int format, unsigned int type, const void* p)
{
    AE_STUB();
}

void GL_TexImage2D(unsigned int target, int level, int internal_format, int w,
        int h, int border, unsigned int fmt, unsigned int type, const void* p)
{
    AE_STUB();
}

void GL_TexParameterf(unsigned int target, unsigned int pname, float param)
{
    AE_STUB();
}

void GL_TexParameterfv(unsigned int target, unsigned int pname, const float *params)
{
    AE_STUB();
}

void GL_TexParameteri(unsigned int target, unsigned int pname, int param)
{
    AE_STUB();
}

void GL_TexParameteriv(unsigned int target, unsigned int pname, const int *params)
{
    AE_STUB();
}

void GL_TexSubImage1D(unsigned int target, int level, int xoffset, int width,
                    unsigned int format, unsigned int type, const void* pix)
{
    AE_STUB();
}

void GL_TexSubImage2D(unsigned int target, int level, int xoffset, int yoffset,
        int w, int h, unsigned int format, unsigned int type, const void* pix)
{
    AE_STUB();
}

void GL_Translated(double x, double y, double z)
{
    AE_STUB();
}

void GL_Translatef(float x, float y, float z)
{
    AE_STUB();
}

unsigned char GL_UnmapBuffer(unsigned int target)
{
    AE_STUB(); return 0;
}

void GL_UseProgram(unsigned int program)
{
    AE_STUB();
}

void GL_ValidateProgram(unsigned int program)
{
    AE_STUB();
}

void GL_Vertex2d(double x, double y)
{
    AE_STUB();
}

void GL_Vertex2dv(const double *v)
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

void GL_Vertex2i(int x, int y)
{
    AE_STUB();
}

void GL_Vertex2iv(const int *v)
{
    AE_STUB();
}

void GL_Vertex2s(short x, short y)
{
    AE_STUB();
}

void GL_Vertex2sv(const short *v)
{
    AE_STUB();
}

void GL_Vertex3d(double x, double y, double z)
{
    AE_STUB();
}

void GL_Vertex3dv(const double *v)
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

void GL_Vertex3i(int x, int y, int z)
{
    AE_STUB();
}

void GL_Vertex3iv(const int *v)
{
    AE_STUB();
}

void GL_Vertex3s(short x, short y, short z)
{
    AE_STUB();
}

void GL_Vertex3sv(const short *v)
{
    AE_STUB();
}

void GL_Vertex4d(double x, double y, double z, double w)
{
    AE_STUB();
}

void GL_Vertex4dv(const double *v)
{
    AE_STUB();
}

void GL_Vertex4f(float x, float y, float z, float w)
{
    AE_STUB();
}

void GL_Vertex4fv(const float* xyzw)
{
    AE_STUB();
}

void GL_Vertex4i(int x, int y, int z, int w)
{
    AE_STUB();
}

void GL_Vertex4iv(const int *v)
{
    AE_STUB();
}

void GL_Vertex4s(short x, short y, short z, short w)
{
    AE_STUB();
}

void GL_Vertex4sv(const short *v)
{
    AE_STUB();
}

void GL_VertexAttribPointer(unsigned int index, int size, unsigned int type,
                unsigned char normalized, int stride, const void * pointer)
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
 * ~~ [ init & quit ] ~~ *
--------------------------------------------------------------------------------
*/

void gl_wrapper_init(void)
{
    // AE_STUB();
}

void gl_wrapper_quit(void)
{
    // AE_STUB();
}
