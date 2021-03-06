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
GL_Accum(unsigned int op, float value);

GL_DECL void GL_CALL
GL_ActiveShaderProgram(unsigned int pipeline, unsigned int program);

GL_DECL void GL_CALL
GL_ActiveTexture(unsigned int texture);

GL_DECL void GL_CALL
GL_AlphaFunc(unsigned int func, float ref);

GL_DECL unsigned char GL_CALL
GL_AreTexturesResident(int n, const unsigned int* tex, unsigned char* res);

GL_DECL void GL_CALL
GL_ArrayElement(int i);

GL_DECL void GL_CALL
GL_AttachShader(unsigned int program, unsigned int shader);

GL_DECL void GL_CALL
GL_Begin(unsigned int mode);

GL_DECL void GL_CALL
GL_BeginConditionalRender(unsigned int id, unsigned int mode);

GL_DECL void GL_CALL
GL_BeginQuery(unsigned int target, unsigned int id);

GL_DECL void GL_CALL
GL_BeginQueryIndexed(unsigned int target, unsigned int index, unsigned int id);

GL_DECL void GL_CALL
GL_BeginTransformFeedback(unsigned int primitiveMode);

GL_DECL void GL_CALL
GL_BindAttribLocation(unsigned int program, unsigned int index, const char *name);

GL_DECL void GL_CALL
GL_BindBuffer(unsigned int target, unsigned int buffer);

GL_DECL void GL_CALL
GL_BindTexture(unsigned int target, unsigned int texture);

GL_DECL void GL_CALL
GL_BindVertexArray(unsigned int array);

GL_DECL void GL_CALL
GL_Bitmap(int width, int height, float x_origin, float y_origin,
        float x_move, float y_move, const unsigned char* bitmap);

GL_DECL void GL_CALL
GL_BlendColor(float red, float green, float blue, float alpha);

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
GL_BufferData(unsigned int target, ptrdiff_t size, const void *data, unsigned int usage);

GL_DECL void GL_CALL
GL_BufferSubData(unsigned int target, ptrdiff_t offset, ptrdiff_t size, const void* data);

GL_DECL void GL_CALL
GL_CallList(unsigned int list);

GL_DECL void GL_CALL
GL_CallLists(int n, unsigned int type, const void *lists);

GL_DECL void GL_CALL
GL_Clear(unsigned int mask);

GL_DECL void GL_CALL
GL_ClearAccum(float r, float g, float b, float a);

GL_DECL void GL_CALL
GL_ClearColor(float r, float g, float b, float a);

GL_DECL void GL_CALL
GL_ClearDepth(double depth);

GL_DECL void GL_CALL
GL_ClearIndex(float c);

GL_DECL void GL_CALL
GL_ClearStencil(int s);

GL_DECL void GL_CALL
GL_ClipPlane(unsigned int plane, const double *equation);

GL_DECL void GL_CALL
GL_Color3b(char red, char green, char blue);

GL_DECL void GL_CALL
GL_Color3bv(const char *v);

GL_DECL void GL_CALL
GL_Color3d(double red, double green, double blue);

GL_DECL void GL_CALL
GL_Color3dv(const double *v);

GL_DECL void GL_CALL
GL_Color3f(float r, float g, float b);

GL_DECL void GL_CALL
GL_Color3fv(const float* rgb);

GL_DECL void GL_CALL
GL_Color3i(int red, int green, int blue);

GL_DECL void GL_CALL
GL_Color3iv(const int *v);

GL_DECL void GL_CALL
GL_Color3s(short red, short green, short blue);

GL_DECL void GL_CALL
GL_Color3sv(const short *v);

GL_DECL void GL_CALL
GL_Color3ub(unsigned char red, unsigned char green, unsigned char blue);

GL_DECL void GL_CALL
GL_Color3ubv(const unsigned char *v);

GL_DECL void GL_CALL
GL_Color3ui(unsigned int red, unsigned int green, unsigned int blue);

GL_DECL void GL_CALL
GL_Color3uiv(const unsigned int *v);

GL_DECL void GL_CALL
GL_Color3us(unsigned short red, unsigned short green, unsigned short blue);

GL_DECL void GL_CALL
GL_Color3usv(const unsigned short *v);

GL_DECL void GL_CALL
GL_Color4b(char red, char green, char blue, char alpha);

GL_DECL void GL_CALL
GL_Color4bv(const char *v);

GL_DECL void GL_CALL
GL_Color4d(double red, double green, double blue, double alpha);

GL_DECL void GL_CALL
GL_Color4dv(const double *v);

GL_DECL void GL_CALL
GL_Color4f(float r, float g, float b, float a);

GL_DECL void GL_CALL
GL_Color4fv(const float* rgba);

GL_DECL void GL_CALL
GL_Color4i(int red, int green, int blue, int alpha);

GL_DECL void GL_CALL
GL_Color4iv(const int *v);

GL_DECL void GL_CALL
GL_Color4s(short red, short green, short blue, short alpha);

GL_DECL void GL_CALL
GL_Color4sv(const short *v);

GL_DECL void GL_CALL
GL_Color4ub(unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha);

GL_DECL void GL_CALL
GL_Color4ubv(const unsigned char *v);

GL_DECL void GL_CALL
GL_Color4ui(unsigned int red, unsigned int green, unsigned int blue, unsigned int alpha);

GL_DECL void GL_CALL
GL_Color4uiv(const unsigned int *v);

GL_DECL void GL_CALL
GL_Color4us(unsigned short red, unsigned short green, unsigned short blue, unsigned short alpha);

GL_DECL void GL_CALL
GL_Color4usv(const unsigned short *v);

GL_DECL void GL_CALL
GL_ColorMask(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

GL_DECL void GL_CALL
GL_ColorMaterial(unsigned int face, unsigned int mode);

GL_DECL void GL_CALL
GL_ColorPointer(int size, unsigned int type, int stride, const void* data);

GL_DECL void GL_CALL
GL_CompileShader(unsigned int shader);

GL_DECL void GL_CALL
GL_CopyPixels(int x, int y, int width, int height, unsigned int type);

GL_DECL void GL_CALL
GL_CopyTexImage1D(unsigned int target, int level, unsigned int internalformat,
                                        int x, int y, int width, int border);

GL_DECL void GL_CALL
GL_CopyTexImage2D(unsigned int target, int level, unsigned int internalformat,
                            int x, int y, int width, int height, int border);

GL_DECL void GL_CALL
GL_CopyTexSubImage1D( unsigned int target, int level, int xoffset,
                                        int x, int y, int width);

GL_DECL void GL_CALL
GL_CopyTexSubImage2D( unsigned int target, int level, int xoffset,
                int yoffset, int x, int y, int width, int height);

GL_DECL unsigned int GL_CALL
GL_CreateProgram(void);

GL_DECL unsigned int GL_CALL
GL_CreateShader(unsigned int type);

GL_DECL void GL_CALL
GL_CullFace(unsigned int face);

GL_DECL void GL_CALL
GL_DeleteBuffers(int n, const unsigned int* buffers);

GL_DECL void GL_CALL
GL_DeleteLists(unsigned int list, int range);

GL_DECL void GL_CALL
GL_DeleteProgram(unsigned int program);

GL_DECL void GL_CALL
GL_DeleteTextures(int n, const unsigned int* textures);

GL_DECL void GL_CALL
GL_DeleteVertexArrays(int n, const unsigned int *arrays);

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
GL_DisableVertexAttribArray(unsigned int index);

GL_DECL void GL_CALL
GL_DrawArrays(unsigned int mode, int first, int count);

GL_DECL void GL_CALL
GL_DrawBuffer(unsigned int mode);

GL_DECL void GL_CALL
GL_DrawElements(unsigned int mode, int n, unsigned int type, const void* inds);

GL_DECL void GL_CALL
GL_DrawPixels(int width, int height, unsigned int format, unsigned int type,
                                                        const void *pixels);

GL_DECL void GL_CALL
GL_EdgeFlag(unsigned char flag);

GL_DECL void GL_CALL
GL_EdgeFlagPointer(int stride, const void *ptr);

GL_DECL void GL_CALL
GL_EdgeFlagv(const unsigned char *flag);

GL_DECL void GL_CALL
GL_Enable(unsigned int cap);

GL_DECL void GL_CALL
GL_EnableClientState(unsigned int cap);

GL_DECL void GL_CALL
GL_EnableVertexAttribArray(unsigned int index);

GL_DECL void GL_CALL
GL_End(void);

GL_DECL void GL_CALL
GL_EndConditionalRender(void);

GL_DECL void GL_CALL
GL_EndList(void);

GL_DECL void GL_CALL
GL_EndQuery(unsigned int target);

GL_DECL void GL_CALL
GL_EndQueryIndexed(unsigned int target, unsigned int index);

GL_DECL void GL_CALL
GL_EndTransformFeedback(void);

GL_DECL void GL_CALL
GL_EvalCoord1d(double u);

GL_DECL void GL_CALL
GL_EvalCoord1dv(const double *u);

GL_DECL void GL_CALL
GL_EvalCoord1f(float u);

GL_DECL void GL_CALL
GL_EvalCoord1fv(const float *u);

GL_DECL void GL_CALL
GL_EvalCoord2d(double u, double v);

GL_DECL void GL_CALL
GL_EvalCoord2dv(const double *u);

GL_DECL void GL_CALL
GL_EvalCoord2f(float u, float v);

GL_DECL void GL_CALL
GL_EvalCoord2fv(const float *u);

GL_DECL void GL_CALL
GL_EvalMesh1(unsigned int mode, int i1, int i2);

GL_DECL void GL_CALL
GL_EvalMesh2(unsigned int mode, int i1, int i2, int j1, int j2);

GL_DECL void GL_CALL
GL_EvalPoint1(int i);

GL_DECL void GL_CALL
GL_EvalPoint2(int i, int j);

GL_DECL void GL_CALL
GL_FeedbackBuffer(int size, unsigned int type, float *buffer);

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
GL_GenBuffers(int n, unsigned int *buffers);

GL_DECL unsigned int GL_CALL
GL_GenLists(int range);

GL_DECL void GL_CALL
GL_GenTextures(int n, unsigned int* textures);

GL_DECL void GL_CALL
GL_GenVertexArrays(int n, unsigned int *arrays);

GL_DECL int GL_CALL
GL_GetAttribLocation(unsigned int program, const char *name);

GL_DECL void GL_CALL
GL_GetBooleanv(unsigned int pname, unsigned char *params);

GL_DECL void GL_CALL
GL_GetClipPlane(unsigned int plane, double *equation);

GL_DECL void GL_CALL
GL_GetDoublev(unsigned int pname, double *params);

GL_DECL unsigned int GL_CALL
GL_GetError(void);

GL_DECL void GL_CALL
GL_GetFloatv(unsigned int pname, float* data);

GL_DECL void GL_CALL
GL_GetIntegerv(unsigned int pname, int* data);

GL_DECL void GL_CALL
GL_GetLightfv(unsigned int light, unsigned int pname, float* params);

GL_DECL void GL_CALL
GL_GetLightiv(unsigned int light, unsigned int pname, int* params);

GL_DECL void GL_CALL
GL_GetMapdv(unsigned int target, unsigned int query, double* v);

GL_DECL void GL_CALL
GL_GetMapfv(unsigned int target, unsigned int query, float* v);

GL_DECL void GL_CALL
GL_GetMapiv(unsigned int target, unsigned int query, int* v);

GL_DECL void GL_CALL
GL_GetMaterialfv(unsigned int face, unsigned int pname, float* params);

GL_DECL void GL_CALL
GL_GetMaterialiv(unsigned int face, unsigned int pname, int* params);

GL_DECL void GL_CALL
GL_GetPixelMapfv(unsigned int map, float* values);

GL_DECL void GL_CALL
GL_GetPixelMapuiv(unsigned int map, unsigned int* values);

GL_DECL void GL_CALL
GL_GetPixelMapusv(unsigned int map, unsigned short* values);

GL_DECL void GL_CALL
GL_GetPointerv(unsigned int pname, void** params);

GL_DECL void GL_CALL
GL_GetPolygonStipple(unsigned char* mask);

GL_DECL void GL_CALL
GL_GetProgramiv(unsigned int program, unsigned int pname, int* params);

GL_DECL void GL_CALL
GL_GetProgramInfoLog(unsigned int program, int buf_size, int* length, char* info_log);

GL_DECL void GL_CALL
GL_GetShaderiv(unsigned int shader, unsigned int pname, int* params);

GL_DECL void GL_CALL
GL_GetShaderInfoLog(unsigned int shader, int buf_size, int* length, char* info_log);

GL_DECL const char* GL_CALL
GL_GetString(unsigned int name);

GL_DECL void GL_CALL
GL_GetTexEnvfv(unsigned int target, unsigned int pname, float* params);

GL_DECL void GL_CALL
GL_GetTexEnviv(unsigned int target, unsigned int pname, int* params);

GL_DECL void GL_CALL
GL_GetTexGendv(unsigned int coord, unsigned int pname, double* params);

GL_DECL void GL_CALL
GL_GetTexGenfv(unsigned int coord, unsigned int pname, float* params);

GL_DECL void GL_CALL
GL_GetTexGeniv(unsigned int coord, unsigned int pname, int* params);

GL_DECL void GL_CALL
GL_GetTexImage(unsigned int target, int level, unsigned int format, unsigned int type, void *pixels);

GL_DECL void GL_CALL
GL_GetTexLevelParameterfv(unsigned int target, int level, unsigned int pname, float *params);

GL_DECL void GL_CALL
GL_GetTexLevelParameteriv(unsigned int target, int level, unsigned int pname, int *params);

GL_DECL void GL_CALL
GL_GetTexParameterfv(unsigned int target, unsigned int pname, float* params);

GL_DECL void GL_CALL
GL_GetTexParameteriv(unsigned int target, unsigned int pname, int* params);

GL_DECL void GL_CALL
GL_Hint(unsigned int target, unsigned int mode);

GL_DECL void GL_CALL
GL_IndexMask(unsigned int mask);

GL_DECL void GL_CALL
GL_IndexPointer(unsigned int type, int stride, const void* ptr);

GL_DECL void GL_CALL
GL_Indexd(double c);

GL_DECL void GL_CALL
GL_Indexdv(const double* c);

GL_DECL void GL_CALL
GL_Indexf(float c);

GL_DECL void GL_CALL
GL_Indexfv(const float* c);

GL_DECL void GL_CALL
GL_Indexi(int c);

GL_DECL void GL_CALL
GL_Indexiv(const int* c);

GL_DECL void GL_CALL
GL_Indexs(short c);

GL_DECL void GL_CALL
GL_Indexsv(const short* c);

GL_DECL void GL_CALL
GL_Indexub(unsigned char c);

GL_DECL void GL_CALL
GL_Indexubv(const unsigned char* c);

GL_DECL void GL_CALL
GL_InitNames(void);

GL_DECL void GL_CALL
GL_InterleavedArrays(unsigned int format, int stride, const void* pointer);

GL_DECL unsigned char GL_CALL
GL_IsBuffer(unsigned int buffer);

GL_DECL unsigned char GL_CALL
GL_IsEnabled(unsigned int cap);

GL_DECL unsigned char GL_CALL
GL_IsList(unsigned int list);

GL_DECL unsigned char GL_CALL
GL_IsProgram(unsigned int program);

GL_DECL unsigned char GL_CALL
GL_IsShader(unsigned int shader);

GL_DECL unsigned char GL_CALL
GL_IsTexture(unsigned int texture);

GL_DECL unsigned char GL_CALL
GL_IsVertexArray(unsigned int array);

GL_DECL void GL_CALL
GL_LightModelf(unsigned int pname, float param);

GL_DECL void GL_CALL
GL_LightModelfv(unsigned int pname, const float* params);

GL_DECL void GL_CALL
GL_LightModeli(unsigned int pname, int param);

GL_DECL void GL_CALL
GL_LightModeliv(unsigned int pname, const int* params);

GL_DECL void GL_CALL
GL_Lightf(unsigned int light, unsigned int pname, float param);

GL_DECL void GL_CALL
GL_Lightfv(unsigned int light, unsigned int pname, const float* params);

GL_DECL void GL_CALL
GL_Lighti(unsigned int light, unsigned int pname, int param);

GL_DECL void GL_CALL
GL_Lightiv(unsigned int light, unsigned int pname, const int* params);

GL_DECL void GL_CALL
GL_LineStipple(int factor, unsigned short pattern);

GL_DECL void GL_CALL
GL_LineWidth(float width);

GL_DECL void GL_CALL
GL_LinkProgram(unsigned int program);

GL_DECL void GL_CALL
GL_ListBase(unsigned int base);

GL_DECL void GL_CALL
GL_LoadIdentity(void);

GL_DECL void GL_CALL
GL_LoadMatrixd(const double* m);

GL_DECL void GL_CALL
GL_LoadMatrixf(const float* m);

GL_DECL void GL_CALL
GL_LoadName(unsigned int name);

GL_DECL void GL_CALL
GL_LogicOp(unsigned int opcode);

GL_DECL void GL_CALL
GL_Map1d(unsigned int target, double u1, double u2, int stride, int order, const double* points);

GL_DECL void GL_CALL
GL_Map1f(unsigned int target, float u1, float u2, int stride, int order, const float* points);

GL_DECL void GL_CALL
GL_Map2d(unsigned int target, double u1, double u2, int ustride, int uorder,
        double v1, double v2, int vstride, int vorder, const double* points);

GL_DECL void GL_CALL
GL_Map2f(unsigned int target, float u1, float u2, int ustride, int uorder,
        float v1, float v2, int vstride, int vorder, const float* points);

GL_DECL void* GL_CALL
GL_MapBuffer(unsigned int target, unsigned int access);

GL_DECL void* GL_CALL
GL_MapBufferRange(unsigned int target, ptrdiff_t offset, ptrdiff_t length, unsigned int access);

GL_DECL void GL_CALL
GL_MapGrid1d(int un, double u1, double u2);

GL_DECL void GL_CALL
GL_MapGrid1f(int un, float u1, float u2);

GL_DECL void GL_CALL
GL_MapGrid2d(int un, double u1, double u2, int vn, double v1, double v2);

GL_DECL void GL_CALL
GL_MapGrid2f(int un, float u1, float u2, int vn, float v1, float v2);

GL_DECL void GL_CALL
GL_Materialf(unsigned int face, unsigned int pname, float param);

GL_DECL void GL_CALL
GL_Materialfv(unsigned int face, unsigned int pname, const float* params);

GL_DECL void GL_CALL
GL_Materiali(unsigned int face, unsigned int pname, int param);

GL_DECL void GL_CALL
GL_Materialiv(unsigned int face, unsigned int pname, const int *params);

GL_DECL void GL_CALL
GL_MatrixMode(unsigned int mode);

GL_DECL void GL_CALL
GL_MultMatrixd(const double *m);

GL_DECL void GL_CALL
GL_MultMatrixf(const float* m);

GL_DECL void GL_CALL
GL_NewList(unsigned int list, unsigned int mode);

GL_DECL void GL_CALL
GL_Normal3b(char nx, char ny, char nz);

GL_DECL void GL_CALL
GL_Normal3bv(const char *v);

GL_DECL void GL_CALL
GL_Normal3d(double nx, double ny, double nz);

GL_DECL void GL_CALL
GL_Normal3dv(const double *v);

GL_DECL void GL_CALL
GL_Normal3f(float x, float y, float z);

GL_DECL void GL_CALL
GL_Normal3fv(const float* normal);

GL_DECL void GL_CALL
GL_Normal3i(int nx, int ny, int nz);

GL_DECL void GL_CALL
GL_Normal3iv(const int* v);

GL_DECL void GL_CALL
GL_Normal3s(short nx, short ny, short nz);

GL_DECL void GL_CALL
GL_Normal3sv(const short* v);

GL_DECL void GL_CALL
GL_NormalPointer(unsigned int type, int stride, const void* data);

GL_DECL void GL_CALL
GL_Ortho(double lf, double rt, double btm, double top, double near, double far);

GL_DECL void GL_CALL
GL_PassThrough(float token);

GL_DECL void GL_CALL
GL_PixelMapfv(unsigned int map, int mapsize, const float* values);

GL_DECL void GL_CALL
GL_PixelMapuiv(unsigned int map, int mapsize, const unsigned int* values);

GL_DECL void GL_CALL
GL_PixelMapusv(unsigned int map, int mapsize, const unsigned short* values);

GL_DECL void GL_CALL
GL_PixelStoref(unsigned int pname, float param);

GL_DECL void GL_CALL
GL_PixelStorei(unsigned int pname, int param);

GL_DECL void GL_CALL
GL_PixelTransferf(unsigned int pname, float param);

GL_DECL void GL_CALL
GL_PixelTransferi(unsigned int pname, int param);

GL_DECL void GL_CALL
GL_PixelZoom(float xfactor, float yfactor);

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
GL_PolygonStipple(const unsigned char* mask);

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
GL_PopName(void);

GL_DECL void GL_CALL
GL_PushName(unsigned int name);

GL_DECL void GL_CALL
GL_PrioritizeTextures(int n, const unsigned int* tex, const float* priorities);

GL_DECL void GL_CALL
GL_ProvokingVertex(unsigned int mode);

GL_DECL void GL_CALL
GL_RasterPos2d(double x, double y);

GL_DECL void GL_CALL
GL_RasterPos2dv(const double* v);

GL_DECL void GL_CALL
GL_RasterPos2f(float x, float y);

GL_DECL void GL_CALL
GL_RasterPos2fv(const float* v);

GL_DECL void GL_CALL
GL_RasterPos2i(int x, int y);

GL_DECL void GL_CALL
GL_RasterPos2iv(const int* v);

GL_DECL void GL_CALL
GL_RasterPos2s(short x, short y);

GL_DECL void GL_CALL
GL_RasterPos2sv(const short* v);

GL_DECL void GL_CALL
GL_RasterPos3d(double x, double y, double z);

GL_DECL void GL_CALL
GL_RasterPos3dv(const double* v);

GL_DECL void GL_CALL
GL_RasterPos3f(float x, float y, float z);

GL_DECL void GL_CALL
GL_RasterPos3fv(const float* v);

GL_DECL void GL_CALL
GL_RasterPos3i(int x, int y, int z);

GL_DECL void GL_CALL
GL_RasterPos3iv(const int* v);

GL_DECL void GL_CALL
GL_RasterPos3s(short x, short y, short z);

GL_DECL void GL_CALL
GL_RasterPos3sv(const short* v);

GL_DECL void GL_CALL
GL_RasterPos4d(double x, double y, double z, double w);

GL_DECL void GL_CALL
GL_RasterPos4dv(const double* v);

GL_DECL void GL_CALL
GL_RasterPos4f(float x, float y, float z, float w);

GL_DECL void GL_CALL
GL_RasterPos4fv(const float* v);

GL_DECL void GL_CALL
GL_RasterPos4i(int x, int y, int z, int w);

GL_DECL void GL_CALL
GL_RasterPos4iv(const int* v);

GL_DECL void GL_CALL
GL_RasterPos4s(short x, short y, short z, short w);

GL_DECL void GL_CALL
GL_RasterPos4sv(const short* v);

GL_DECL void GL_CALL
GL_ReadBuffer(unsigned int mode);

GL_DECL void GL_CALL
GL_ReadPixels(int x, int y, int width, int height, unsigned int format, unsigned int type, void *pixels);

GL_DECL void GL_CALL
GL_Rectd(double x1, double y1, double x2, double y2);

GL_DECL void GL_CALL
GL_Rectdv(const double* v1, const double* v2);

GL_DECL void GL_CALL
GL_Rectf(float x1, float y1, float x2, float y2);

GL_DECL void GL_CALL
GL_Rectfv(const float* v1, const float* v2);

GL_DECL void GL_CALL
GL_Recti(int x1, int y1, int x2, int y2);

GL_DECL void GL_CALL
GL_Rectiv(const int* v1, const int* v2);

GL_DECL void GL_CALL
GL_Rects(short x1, short y1, short x2, short y2);

GL_DECL void GL_CALL
GL_Rectsv(const short* v1, const short* v2);

GL_DECL int GL_CALL
GL_RenderMode(unsigned int mode);

GL_DECL void GL_CALL
GL_Rotated(double angle, double x, double y, double z);

GL_DECL void GL_CALL
GL_Rotatef(float theta, float x, float y, float z);

GL_DECL void GL_CALL
GL_Scaled(double x, double y, double z);

GL_DECL void GL_CALL
GL_Scalef(float x, float y, float z);

GL_DECL void GL_CALL
GL_Scissor(int x, int y, int width, int height);

GL_DECL void GL_CALL
GL_SelectBuffer(int size, unsigned int* buffer);

GL_DECL void GL_CALL
GL_ShadeModel(unsigned int mode);

GL_DECL void GL_CALL
GL_ShaderSource(unsigned int shader, int count, const char ** const string,
                                                        const int *length);

GL_DECL void GL_CALL
GL_StencilFunc(unsigned int func, int ref, unsigned int mask);

GL_DECL void GL_CALL
GL_StencilMask(unsigned int mask);

GL_DECL void GL_CALL
GL_StencilOp(unsigned int fail, unsigned int zfail, unsigned int zpass);

GL_DECL void GL_CALL
GL_TexCoord1d(double s);

GL_DECL void GL_CALL
GL_TexCoord1dv(const double* v);

GL_DECL void GL_CALL
GL_TexCoord1f(float s);

GL_DECL void GL_CALL
GL_TexCoord1fv(const float* v);

GL_DECL void GL_CALL
GL_TexCoord1i(int s);

GL_DECL void GL_CALL
GL_TexCoord1iv(const int* v);

GL_DECL void GL_CALL
GL_TexCoord1s(short s);

GL_DECL void GL_CALL
GL_TexCoord1sv(const short* v);

GL_DECL void GL_CALL
GL_TexCoord2d(double s, double t);

GL_DECL void GL_CALL
GL_TexCoord2dv(const double* v);

GL_DECL void GL_CALL
GL_TexCoord2f(float s, float t);

GL_DECL void GL_CALL
GL_TexCoord2fv(const float* st);

GL_DECL void GL_CALL
GL_TexCoord2i(int s, int t);

GL_DECL void GL_CALL
GL_TexCoord2iv(const int* v);

GL_DECL void GL_CALL
GL_TexCoord2s(short s, short t);

GL_DECL void GL_CALL
GL_TexCoord2sv(const short* v);

GL_DECL void GL_CALL
GL_TexCoord3d(double s, double t, double r);

GL_DECL void GL_CALL
GL_TexCoord3dv(const double* v);

GL_DECL void GL_CALL
GL_TexCoord3f(float s, float t, float r);

GL_DECL void GL_CALL
GL_TexCoord3fv(const float* str);

GL_DECL void GL_CALL
GL_TexCoord3i(int s, int t, int r);

GL_DECL void GL_CALL
GL_TexCoord3iv(const int* v);

GL_DECL void GL_CALL
GL_TexCoord3s(short s, short t, short r);

GL_DECL void GL_CALL
GL_TexCoord3sv(const short* v);

GL_DECL void GL_CALL
GL_TexCoord4d(double s, double t, double r, double q);

GL_DECL void GL_CALL
GL_TexCoord4dv(const double* v);

GL_DECL void GL_CALL
GL_TexCoord4f(float s, float t, float r, float q);

GL_DECL void GL_CALL
GL_TexCoord4fv(const float* strq);

GL_DECL void GL_CALL
GL_TexCoord4i(int s, int t, int r, int q);

GL_DECL void GL_CALL
GL_TexCoord4iv(const int* v);

GL_DECL void GL_CALL
GL_TexCoord4s(short s, short t, short r, short q);

GL_DECL void GL_CALL
GL_TexCoord4sv(const short* v);

GL_DECL void GL_CALL
GL_TexCoordPointer(int size, unsigned int type, int stride, const void* data);

GL_DECL void GL_CALL
GL_TexEnvfv(unsigned int target, unsigned int pname, const float* params);

GL_DECL void GL_CALL
GL_TexEnviv(unsigned int target, unsigned int pname, const int* params);

GL_DECL void GL_CALL
GL_TexEnvf(unsigned int target, unsigned int pname, float param);

GL_DECL void GL_CALL
GL_TexEnvi(unsigned int target, unsigned int pname, int param);

GL_DECL void GL_CALL
GL_TexGeniv(unsigned int coord, unsigned int pname, const int* params);

GL_DECL void GL_CALL
GL_TexGenfv(unsigned int coord, unsigned int pname, const float* params);

GL_DECL void GL_CALL
GL_TexGendv(unsigned int coord, unsigned int pname, const double* params);

GL_DECL void GL_CALL
GL_TexGeni(unsigned int coord, unsigned int pname, int param);

GL_DECL void GL_CALL
GL_TexGenf(unsigned int coord, unsigned int pname, float param);

GL_DECL void GL_CALL
GL_TexGend(unsigned int coord, unsigned int pname, double param);

GL_DECL void GL_CALL
GL_TexImage1D(unsigned int target, int level, int internal_format, int w, int border,
                            unsigned int format, unsigned int type, const void* pix);

GL_DECL void GL_CALL
GL_TexImage2D(unsigned int target, int level, int internal_format, int w, int h,
            int border, unsigned int format, unsigned int type, const void * p);

GL_DECL void GL_CALL
GL_TexParameterf(unsigned int target, unsigned int pname, float param);

GL_DECL void GL_CALL
GL_TexParameterfv(unsigned int target, unsigned int pname, const float *params);

GL_DECL void GL_CALL
GL_TexParameteri(unsigned int target, unsigned int pname, int param);

GL_DECL void GL_CALL
GL_TexParameteriv(unsigned int target, unsigned int pname, const int *params);

GL_DECL void GL_CALL
GL_TexSubImage1D(unsigned int target, int level, int xoffset, int width,
            unsigned int format, unsigned int type, const void* pixels);

GL_DECL void GL_CALL
GL_TexSubImage2D(unsigned int target, int level, int xoffset, int yoffset, int width,
                int height, unsigned int format, unsigned int type, const void *pix);

GL_DECL void GL_CALL
GL_Translated(double x, double y, double z);

GL_DECL void GL_CALL
GL_Translatef(float x, float y, float z);

GL_DECL unsigned char GL_CALL
GL_UnmapBuffer(unsigned int target);

GL_DECL void GL_CALL
GL_UseProgram(unsigned int program);

GL_DECL void GL_CALL
GL_ValidateProgram(unsigned int program);

GL_DECL void GL_CALL
GL_Vertex2d(double x, double y);

GL_DECL void GL_CALL
GL_Vertex2dv(const double* v);

GL_DECL void GL_CALL
GL_Vertex2f(float x, float y);

GL_DECL void GL_CALL
GL_Vertex2fv(const float* xy);

GL_DECL void GL_CALL
GL_Vertex2i(int x, int y);

GL_DECL void GL_CALL
GL_Vertex2iv(const int* v);

GL_DECL void GL_CALL
GL_Vertex2s(short x, short y);

GL_DECL void GL_CALL
GL_Vertex2sv(const short* v);

GL_DECL void GL_CALL
GL_Vertex3d(double x, double y, double z);

GL_DECL void GL_CALL
GL_Vertex3dv(const double* v);

GL_DECL void GL_CALL
GL_Vertex3f(float x, float y, float z);

GL_DECL void GL_CALL
GL_Vertex3fv(const float* xyz);

GL_DECL void GL_CALL
GL_Vertex3i(int x, int y, int z);

GL_DECL void GL_CALL
GL_Vertex3iv(const int* v);

GL_DECL void GL_CALL
GL_Vertex3s(short x, short y, short z);

GL_DECL void GL_CALL
GL_Vertex3sv(const short* v);

GL_DECL void GL_CALL
GL_Vertex4d(double x, double y, double z, double w);

GL_DECL void GL_CALL
GL_Vertex4dv(const double* v);

GL_DECL void GL_CALL
GL_Vertex4f(float x, float y, float z, float w);

GL_DECL void GL_CALL
GL_Vertex4fv(const float* xyzw);

GL_DECL void GL_CALL
GL_Vertex4i(int x, int y, int z, int w);

GL_DECL void GL_CALL
GL_Vertex4iv(const int* v);

GL_DECL void GL_CALL
GL_Vertex4s(short x, short y, short z, short w);

GL_DECL void GL_CALL
GL_Vertex4sv(const short* v);

GL_DECL void GL_CALL
GL_VertexAttribPointer(unsigned int index, int size, unsigned int type,
            unsigned char normalized, int stride, const void* pointer);

GL_DECL void GL_CALL
GL_VertexPointer(int size, unsigned int type, int stride, const void* data);

GL_DECL void GL_CALL
GL_Viewport(int x, int y, int width, int height);

/* ===== [ C++ scopes ] ===================================================== */

#if defined(__cplusplus)
    // server (GPU) attribute stack
    #define GL_ATTRIB(...) glAttrib _gl_attrib(__VA_ARGS__)

    struct glAttrib
    {
        glAttrib(unsigned int mask) { GL_PushAttrib(mask); }
        ~glAttrib() { GL_PopAttrib(); }
    };

    // client (CPU) attribute stack
    #define GL_CLIENT_ATTRIB(...) glClientAttrib _gl_client_attrib(__VA_ARGS__)

    struct glClientAttrib
    {
        glClientAttrib(unsigned int mask) { GL_PushClientAttrib(mask); }
        ~glClientAttrib() { GL_PopClientAttrib(); }
    };

    // oldschool vertex specification (not a stack!)
    #define GL_IMMEDIATE(...) glImmediate _gl_immediate(__VA_ARGS__)

    struct glImmediate
    {
        glImmediate(unsigned int mode) { GL_Begin(mode); }
        ~glImmediate() { GL_End(); }
    };

    // matrix stack
    #define GL_MATRIX(...) glMatrix _gl_matrix(__VA_ARGS__)

    struct glMatrix
    {
        glMatrix() { GL_PushMatrix(); }
        ~glMatrix() { GL_PopMatrix(); }
    };
#endif

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

#if defined(__cplusplus)
    /*
        This leaves the 2D/3D scene automatically at a scope exit. Note that on
        many C++ compilers, the constructor is always executed at scope entry,
        not where you actually declare the object (use funcs for custom blocks).
    */
    #define GL_SCENE3D(...) glScene3D _gl_scene3D(__VA_ARGS__)
    #define GL_SCENE2D(...) glScene2D _gl_scene2D(__VA_ARGS__)

    struct glScene3D
    {
        glScene3D(int w, int h, double FOV, double znear, double zfar)
        {
            gl_enter3D(w, h, FOV, znear, zfar);
        }

        ~glScene3D()
        {
            gl_leave3D();
        }
    };

    struct glScene2D
    {
        glScene2D(int w, int h)
        {
            gl_enter2D(w, h);
        }

        ~glScene2D()
        {
            gl_leave2D();
        }
    };
#endif

/* ===== [ 2D shapes ] ====================================================== */

GL_DECL void GL_CALL gl_rect_ex(float* rect, const float* bl, const float* br,
                const float* tr, const float* tl, int line, float line_width);

GL_DECL void GL_CALL gl_outline(float* rect, const float* rgba);
GL_DECL void GL_CALL gl_rect(float* rect, const float* rgba);

/* ===== [ 3D shapes ] ====================================================== */

GL_DECL void GL_CALL // render a 3-dimensional bounding box outline (debug bounds)
gl_aabbox_ex(float* minv, float* maxv, const float* rgba, float width);

// render a 3-dimensional bounding box outline with a line width of 1.0
GL_DECL void GL_CALL gl_aabbox(float* minv, float* maxv, const float* rgba);

// render a 3-dimensional capsule shape in wireframe mode (debug bounds)
GL_DECL void GL_CALL gl_ellipsoid(float* center, float* extent, const float* rgba);

// render a 3-dimensional spherical shape in wireframe mode (debug bounds)
GL_DECL void GL_CALL gl_sphere(float* center, float radius, const float* rgba);

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
GL_DECL size_t GL_CALL gl_object_count_all(void); // total
GL_DECL void   GL_CALL gl_object_list_all (void** objects);
GL_DECL void   GL_CALL gl_object_print_all(void); // debug
GL_DECL void   GL_CALL gl_object_close_all(void); // clean

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
    /* origin on the filesystem, stripped file name */          \
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
GL_DECL void GL_CALL gl_texture_draw(gl_texture_t* texture, float x, float y);

GL_DECL void GL_CALL /* Draw a translucent, color-modulated texture. */
gl_texture_draw_ex(gl_texture_t* tex, float x, float y, const float rgba[4]);

/* Draw an infinitely-distant textured cube around the viewer.
 */
GL_DECL void GL_CALL gl_texture_draw_skybox_ex( gl_texture_t* front,
                                                gl_texture_t* back,
                                                gl_texture_t* left,
                                                gl_texture_t* right,
                                                gl_texture_t* up,
                                                gl_texture_t* down,
                                                gl_coord3D_t* viewer,
                                                const float color[4]);

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

GL_DECL void GL_CALL gl_texture_print_all(void);
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
                                                                    \
    /* alpha value for diffuse, ambient, specular, and emission */  \
    N(GL_MATERIAL_PROPERTY_OPACITY, float, flt, opacity)            \
                                                                    \
    /* any interesting info we want to print (usually name str) */  \
    N(GL_MATERIAL_PROPERTY_STATUS, const char*, str, status)        \
                                                                    \
    /* origin on the filesystem, filename without the extension */  \
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

// Sets the diffuse, ambient, specular, and emissive components to the same value.
GL_DECL void GL_CALL gl_material_set_all(gl_material_t* material, const float* v);

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

GL_DECL void GL_CALL gl_material_print_all(void);
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
                                                                                    \
    N(GL_LIGHT_PROPERTY_PATH, const char*, str, path)                               \
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
GL_DECL void GL_CALL gl_light_set_all(gl_light_t* light, const float* value);

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

GL_DECL void GL_CALL gl_light_print_all(void);
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
                                                                    \
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
GL_DECL void GL_CALL gl_buffer_draw_ex(gl_buffer_t* buffer, gl_material_t* mtl);
GL_DECL void GL_CALL gl_buffer_draw(gl_buffer_t* buffer);

GL_DECL void GL_CALL gl_buffer_draw_aabbox_ex( gl_buffer_t* buffer, // debugging
                                            const float* rgba, float line_width);

GL_DECL void GL_CALL gl_buffer_draw_aabbox(gl_buffer_t* buffer,
                                            const float* rgba);

GL_DECL void GL_CALL gl_buffer_draw_normals_ex(gl_buffer_t* buffer, // debugging
            const float* start_color, const float* end_color, float line_width);

GL_DECL void GL_CALL gl_buffer_draw_normals(gl_buffer_t* buffer,
                                            const float* color);

static c_inline size_t gl_buffer_count_all(void)
{
    return (size_t)gl_buffer_get_total(NULL);
}

GL_DECL void GL_CALL gl_buffer_list_all(gl_buffer_t** buffers);

GL_DECL void GL_CALL gl_buffer_print_all(void);
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
                                                                                \
    N(GL_PARTICLE_EMITTER_PROPERTY_PATH, const char*, str, path)                \
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

/* Randomly distribute particles over the surface of each tri.
 * Particles in the emitter are appended to for mesh building.
 */
GL_DECL void GL_CALL gl_particle_emitter_mesh_cover_face(gl_particle_emitter_t* emitter,
                                    gl_buffer_t* buffer, size_t particles_per_triangle);

/* Randomly distribute particles along each triangle edge. The
 * particles will not cross the face of the tri in animations.
 */
GL_DECL void GL_CALL gl_particle_emitter_mesh_same_edges(gl_particle_emitter_t* emitter,
                                    gl_buffer_t* buffer, size_t particles_per_triangle);

/* Randomly distribute particles along each triangle edge. The
 * chosen edge for each particle will be different every time.
 */
GL_DECL void GL_CALL gl_particle_emitter_mesh_rand_edges(gl_particle_emitter_t* emitter,
                                    gl_buffer_t* buffer, size_t particles_per_triangle);

/* Randomly distribute particles along a single triangle edge.
 * This is a bizarre effect, included mostly for completeness.
 */
GL_DECL void GL_CALL gl_particle_emitter_mesh_given_edge(gl_particle_emitter_t* emitter,
                    gl_buffer_t* buffer, size_t particles_per_triangle, size_t index);

/* Creates a particle sphere around every vertex in the group.
 * If the radius is 0 and particles is 3, a fast path is used.
 */
GL_DECL void GL_CALL gl_particle_emitter_mesh_rad_vertex(gl_particle_emitter_t* emitter,
                    gl_buffer_t* buffer, size_t particles_per_triangle, float radius);

GL_DECL void GL_CALL gl_particle_emitter_reset_all(void); // zero particle count
GL_DECL void GL_CALL gl_particle_emitter_reset(gl_particle_emitter_t* emitter);

GL_DECL void GL_CALL gl_particle_emitter_fill_all(void); // generate all particles
GL_DECL void GL_CALL gl_particle_emitter_fill(gl_particle_emitter_t* emitter);

GL_DECL void GL_CALL gl_particle_emitter_update_all(double dt); // simulate
GL_DECL void GL_CALL gl_particle_emitter_update(gl_particle_emitter_t* emitter, double dt);

GL_DECL void GL_CALL gl_particle_emitter_draw_all(void); // render particles
GL_DECL void GL_CALL gl_particle_emitter_draw(gl_particle_emitter_t* emitter);

GL_DECL void GL_CALL gl_particle_emitter_draw_aabbox_ex(gl_particle_emitter_t* emitter,
                                                const float* rgba, float line_width);

GL_DECL void GL_CALL // draw the axial bounding box of all particles in the emitter
gl_particle_emitter_draw_aabbox(gl_particle_emitter_t* emitter, const float* rgba);

GL_DECL void GL_CALL // draw debug velocity lines with a color gradient & width
gl_particle_emitter_draw_velocity_ex( gl_particle_emitter_t * emitter,
    const float* start_rgba, const float* end_rgba, float line_width);

GL_DECL void GL_CALL // draw a line at each particle to visualize its velocity
gl_particle_emitter_draw_velocity(gl_particle_emitter_t* e, const float* rgba);

static c_inline size_t gl_particle_emitter_count_all(void)
{
    return (size_t)gl_particle_emitter_get_total(NULL);
}

GL_DECL void GL_CALL gl_particle_emitter_list_all(gl_particle_emitter_t**);

GL_DECL void GL_CALL gl_particle_emitter_print_all(void);
GL_DECL void GL_CALL gl_particle_emitter_close_all(void);

// NOTE avoiding linker issues when changing build configurations,
// in case somebody accidentally types this into a header file...
#undef GL_DECL

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* __GL_CORE_H__ */
