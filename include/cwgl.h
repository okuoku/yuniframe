#ifndef __YUNI_CWGL_H
#define __YUNI_CWGL_H

#ifdef __cplusplus
extern "C" {
#endif
// }

#ifdef CWGL_DLL
/* Win32 DLL */
#ifdef CWGL_SHARED_BUILD
#ifdef _WIN32
#define CWGL_API __declspec(dllexport)
#else
#define CWGL_API __attribute__ ((visibility ("default")))
#endif /* _WIN32 */
#else
#ifdef _WIN32
#define CWGL_API __declspec(dllimport)
#else
#define CWGL_API 
#endif /* _WIN32 */
#endif
#else
/* Generic static-library */
#define CWGL_API
#endif

#include <stdint.h>
#include <stddef.h>

/* Context */
struct cwgl_ctx_s;
typedef struct cwgl_ctx_s cwgl_ctx_t;

#define CWGL_CTX_FLAG_HAS_ALPHA (1<<0)
#define CWGL_CTX_FLAG_HAS_DEPTH (1<<1)
#define CWGL_CTX_FLAG_HAS_STENCIL (1<<2)
#define CWGL_CTX_FLAG_ANTIALIAS (1<<3)
#define CWGL_CTX_FLAG_PREMULTIPLIEDALPHA  (1<<4)
#define CWGL_CTX_FLAG_PRESERVEDRAWINGBUFFER (1<<5)

enum cwgl_query_result_e {
    CWGL_QR_SUCCESS = 0,
    CWGL_QR_GLERROR = 1,
    CWGL_QR_INSUFFICIENT_BUFFER = 2,
    CWGL_QR_UNIMPLEMENTED = 3,

    /* Type queries */
    CWGL_QR_TYPE_BOOL = 0x10,
    /* Short vectors */
    CWGL_QR_TYPE_I1 = 0x11,
    CWGL_QR_TYPE_I2 = 0x12,
    CWGL_QR_TYPE_I3 = 0x13,
    CWGL_QR_TYPE_I4 = 0x14,
    CWGL_QR_TYPE_F1 = 0x15,
    CWGL_QR_TYPE_F2 = 0x16,
    CWGL_QR_TYPE_F3 = 0x17,
    CWGL_QR_TYPE_F4 = 0x18,
    /* Matrix */
    CWGL_QR_TYPE_M2 = 0x22,
    CWGL_QR_TYPE_M3 = 0x23,
    CWGL_QR_TYPE_M4 = 0x24,
    /* Objects */
    CWGL_QR_TYPE_BUFFER = 0x30,
    CWGL_QR_TYPE_SHADER = 0x31,
    CWGL_QR_TYPE_PROGRAM = 0x32,
    CWGL_QR_TYPE_TEXTURE = 0x33,
    CWGL_QR_TYPE_FRAMEBUFFER = 0x34,
    CWGL_QR_TYPE_RENDERBUFFER = 0x35
};
typedef enum cwgl_query_result_e cwgl_query_result_t;

#ifdef CWGL_DECL_ENUMS
#include "cwgl_enums.h"
#else
enum cwgl_enum_e{
    CWGL_ENUM_DUMMY = 0x7fffffff
};
#endif

typedef enum cwgl_enum_e cwgl_enum_t;

/* Heap Objects */
struct cwgl_string_s;
typedef struct cwgl_string_s cwgl_string_t;
CWGL_API size_t cwgl_string_size(cwgl_ctx_t* ctx, cwgl_string_t* str);
CWGL_API cwgl_query_result_t cwgl_string_read(cwgl_ctx_t* ctx, cwgl_string_t* str, char* buf, size_t buflen);
CWGL_API void cwgl_string_release(cwgl_ctx_t* ctx, cwgl_string_t* str);

struct cwgl_Buffer_s;
struct cwgl_Shader_s;
struct cwgl_Program_s;
struct cwgl_Texture_s;
struct cwgl_Framebuffer_s;
struct cwgl_Renderbuffer_s;
struct cwgl_VertexArrayObject_s;

typedef struct cwgl_Buffer_s cwgl_Buffer_t;
typedef struct cwgl_Shader_s cwgl_Shader_t;
typedef struct cwgl_Program_s cwgl_Program_t;
typedef struct cwgl_Texture_s cwgl_Texture_t;
typedef struct cwgl_Framebuffer_s cwgl_Framebuffer_t;
typedef struct cwgl_Renderbuffer_s cwgl_Renderbuffer_t;
typedef struct cwgl_VertexArrayObject_s cwgl_VertexArrayObject_t;

CWGL_API void cwgl_Buffer_release(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer);
CWGL_API void cwgl_Shader_release(cwgl_ctx_t* ctx, cwgl_Shader_t* shader);
CWGL_API void cwgl_Program_release(cwgl_ctx_t* ctx, cwgl_Program_t* program);
CWGL_API void cwgl_Texture_release(cwgl_ctx_t* ctx, cwgl_Texture_t* texture);
CWGL_API void cwgl_Framebuffer_release(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer);
CWGL_API void cwgl_Renderbuffer_release(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer);

struct cwgl_UniformLocation_s;
typedef struct cwgl_UniformLocation_s cwgl_UniformLocation_t;
CWGL_API void cwgl_UniformLocation_release(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* uniformlocation);

/* Context, Platform */
CWGL_API int32_t cwgl_getContextAttributes(cwgl_ctx_t* ctx);
CWGL_API int cwgl_isContextLost(cwgl_ctx_t* ctx);
CWGL_API cwgl_query_result_t cwgl_getSupportedExtensions(cwgl_ctx_t* ctx, cwgl_string_t** str, size_t buflen, size_t* out_reslen);
//object? getExtension(DOMString name);

/* OpenGL State */
CWGL_API void cwgl_disable(cwgl_ctx_t* ctx, cwgl_enum_t cap);
CWGL_API void cwgl_enable(cwgl_ctx_t* ctx, cwgl_enum_t cap);

// 2.5 GL Errors
CWGL_API cwgl_enum_t cwgl_getError(cwgl_ctx_t* ctx);
// 2.7 Current Vertex State
CWGL_API void cwgl_vertexAttrib1f(cwgl_ctx_t* ctx, uint32_t indx, float x);
CWGL_API void cwgl_vertexAttrib2f(cwgl_ctx_t* ctx, uint32_t indx, float x, float y);
CWGL_API void cwgl_vertexAttrib3f(cwgl_ctx_t* ctx, uint32_t indx, float x, float y, float z);
CWGL_API void cwgl_vertexAttrib4f(cwgl_ctx_t* ctx, uint32_t indx, float x, float y, float z, float w);
// 2.8 Vertex Arrays
CWGL_API void cwgl_vertexAttribPointer(cwgl_ctx_t* ctx, uint32_t indx, int32_t size, cwgl_enum_t type, int normalized, uint32_t stride, uint32_t offset);
CWGL_API void cwgl_enableVertexAttribArray(cwgl_ctx_t* ctx, uint32_t index);
CWGL_API void cwgl_disableVertexAttribArray(cwgl_ctx_t* ctx, uint32_t index);
CWGL_API void cwgl_drawArrays(cwgl_ctx_t* ctx, cwgl_enum_t mode, int32_t first, uint32_t count);
CWGL_API void cwgl_drawElements(cwgl_ctx_t* ctx, cwgl_enum_t mode, uint32_t count, cwgl_enum_t type, uint32_t offset);
// 2.9 Buffer Objects
CWGL_API void cwgl_bindBuffer(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_Buffer_t* buffer);
CWGL_API void cwgl_deleteBuffer(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer);
CWGL_API cwgl_Buffer_t* cwgl_createBuffer(cwgl_ctx_t* ctx);
CWGL_API void cwgl_bufferData(cwgl_ctx_t* ctx, cwgl_enum_t target, uint32_t size, void* data, cwgl_enum_t usage);
CWGL_API void cwgl_bufferSubData(cwgl_ctx_t* ctx, cwgl_enum_t target, uint32_t offset, void* data, size_t buflen);
// 2.10 Vertex shaders
// 2.10.1 Loading and Creating Shader Source
CWGL_API cwgl_Shader_t* cwgl_createShader(cwgl_ctx_t* ctx, cwgl_enum_t type);
CWGL_API void cwgl_shaderSource(cwgl_ctx_t* ctx, cwgl_Shader_t* shader, const char* source, size_t sourcelen);
CWGL_API void cwgl_compileShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader);
CWGL_API void cwgl_deleteShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader);
// 2.10.3 Program Objects
CWGL_API cwgl_Program_t* cwgl_createProgram(cwgl_ctx_t* ctx);
CWGL_API void cwgl_attachShader(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_Shader_t* shader);
CWGL_API void cwgl_detachShader(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_Shader_t* shader);
CWGL_API void cwgl_linkProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program);
CWGL_API void cwgl_useProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program);
CWGL_API void cwgl_deleteProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program);
// 2.10.4 Shader Variables
CWGL_API cwgl_query_result_t cwgl_getActiveAttrib(cwgl_ctx_t* ctx, cwgl_Program_t* program, int32_t index, int32_t* out_size, int32_t* type, cwgl_string_t** name);
CWGL_API int32_t cwgl_getAttribLocation(cwgl_ctx_t* ctx, cwgl_Program_t* program, const char* name);
CWGL_API void cwgl_bindAttribLocation(cwgl_ctx_t* ctx, cwgl_Program_t* program, uint32_t index, const char* name);
CWGL_API cwgl_UniformLocation_t* cwgl_getUniformLocation(cwgl_ctx_t* ctx, cwgl_Program_t* program, const char* name);
CWGL_API cwgl_query_result_t cwgl_getActiveUniform(cwgl_ctx_t* ctx, cwgl_Program_t* program, int32_t index, int32_t* out_size, int32_t* type, cwgl_string_t** name);
CWGL_API void cwgl_uniform1f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, float x);
CWGL_API void cwgl_uniform1i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, int32_t x);
CWGL_API void cwgl_uniform2f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, float x, float y);
CWGL_API void cwgl_uniform2i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, int32_t x, int32_t y);
CWGL_API void cwgl_uniform3f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, float x, float y, float z);
CWGL_API void cwgl_uniform3i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, int32_t x, int32_t y, int32_t z);
CWGL_API void cwgl_uniform4f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, float x, float y, float z, float w);
CWGL_API void cwgl_uniform4i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, int32_t x, int32_t y, int32_t z, int32_t w);
CWGL_API void cwgl_uniform1fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, float* values, size_t count);
CWGL_API void cwgl_uniform1iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, int32_t* values, size_t count);
CWGL_API void cwgl_uniform2fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, float* values, size_t count);
CWGL_API void cwgl_uniform2iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, int32_t* values, size_t count);
CWGL_API void cwgl_uniform3fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, float* values, size_t count);
CWGL_API void cwgl_uniform3iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, int32_t* values, size_t count);
CWGL_API void cwgl_uniform4fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, float* values, size_t count);
CWGL_API void cwgl_uniform4iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, int32_t* values, size_t count);
CWGL_API void cwgl_uniformMatrix2fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, int transpose, float* value, size_t count);
CWGL_API void cwgl_uniformMatrix3fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, int transpose, float* value, size_t count);
CWGL_API void cwgl_uniformMatrix4fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, int transpose, float* value, size_t count);
// 2.10.5 Shader Execution
CWGL_API void cwgl_validateProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program);
// 2.12.1 Controlling the Viewport
CWGL_API void cwgl_depthRange(cwgl_ctx_t* ctx, float zNear, float zFar);
CWGL_API void cwgl_viewport(cwgl_ctx_t* ctx, int32_t x, int32_t y, int32_t width, int32_t height);

// 3.4 Line Segments
CWGL_API void cwgl_lineWidth(cwgl_ctx_t* ctx, float width);
// 3.5 Polygons
CWGL_API void cwgl_frontFace(cwgl_ctx_t* ctx, cwgl_enum_t mode);
CWGL_API void cwgl_cullFace(cwgl_ctx_t* ctx, cwgl_enum_t mode);
// 3.5.2 Depth offset
CWGL_API void cwgl_polygonOffset(cwgl_ctx_t* ctx, float factor, float units);
// 3.6.1 Pixel Storage Modes
CWGL_API void cwgl_pixelStorei(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t param);
// 3.7 Texturing
CWGL_API void cwgl_activeTexture(cwgl_ctx_t* ctx, cwgl_enum_t texture);
// 3.7.1 Texture Image Specification
CWGL_API void cwgl_texImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, cwgl_enum_t internalformat, uint32_t width, uint32_t height, int32_t border, cwgl_enum_t format, cwgl_enum_t type, const void* buf, size_t buflen);
// 3.7.2 Alternate Texture Image Specification Commands
CWGL_API void cwgl_copyTexImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, cwgl_enum_t internalformat, int32_t x, int32_t y, uint32_t width, uint32_t height, int32_t border);
CWGL_API void cwgl_texSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height, cwgl_enum_t format, cwgl_enum_t type, const void* buf, size_t buflen);
CWGL_API void cwgl_copyTexSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, int32_t x, int32_t y, uint32_t width, uint32_t height);
// 3.7.3 Compressed Texture Images
CWGL_API void cwgl_compressedTexImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, cwgl_enum_t internalformat, uint32_t width, uint32_t height, int32_t border, const void* buf, size_t buflen);
CWGL_API void cwgl_compressedTexSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height, cwgl_enum_t format, const void* buf, size_t buflen);
// 3.7.4 Texture Parameters
CWGL_API void cwgl_texParameterf(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t pname, float param);
CWGL_API void cwgl_texParameteri(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t pname, int32_t param);
// 3.7.11 Mipmap Generation
CWGL_API void cwgl_generateMipmap(cwgl_ctx_t* ctx, cwgl_enum_t target);
// 3.7.13 Texture Objects
CWGL_API void cwgl_bindTexture(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_Texture_t* texture);
CWGL_API void cwgl_deleteTexture(cwgl_ctx_t* ctx, cwgl_Texture_t* texture);
CWGL_API cwgl_Texture_t* cwgl_createTexture(cwgl_ctx_t* ctx);

// 4.1.2 Scissor Test
CWGL_API void cwgl_scissor(cwgl_ctx_t* ctx, int32_t x, int32_t y, uint32_t width, uint32_t height);
// 4.1.3 Multisample Fragment Operations
CWGL_API void cwgl_sampleCoverage(cwgl_ctx_t* ctx, float value, int invert);
// 4.1.4 Stencil Test
CWGL_API void cwgl_stencilFunc(cwgl_ctx_t* ctx, cwgl_enum_t func, int32_t ref, uint32_t mask);
CWGL_API void cwgl_stencilFuncSeparate(cwgl_ctx_t* ctx, cwgl_enum_t face, cwgl_enum_t func, int32_t ref, uint32_t mask);
CWGL_API void cwgl_stencilOp(cwgl_ctx_t* ctx, cwgl_enum_t fail, cwgl_enum_t zfail, cwgl_enum_t zpass);
CWGL_API void cwgl_stencilOpSeparate(cwgl_ctx_t* ctx, cwgl_enum_t face, cwgl_enum_t fail, cwgl_enum_t zfail, cwgl_enum_t zpass);
// 4.1.5 Depth Buffer Test
CWGL_API void cwgl_depthFunc(cwgl_ctx_t* ctx, cwgl_enum_t func);
// 4.1.6 Blending
CWGL_API void cwgl_blendEquation(cwgl_ctx_t* ctx, cwgl_enum_t mode);
CWGL_API void cwgl_blendEquationSeparate(cwgl_ctx_t* ctx, cwgl_enum_t modeRGB, cwgl_enum_t modeAlpha);
CWGL_API void cwgl_blendFuncSeparate(cwgl_ctx_t* ctx, cwgl_enum_t srcRGB, cwgl_enum_t dstRGB, cwgl_enum_t srcAlpha, cwgl_enum_t dstAlpha);
CWGL_API void cwgl_blendFunc(cwgl_ctx_t* ctx, cwgl_enum_t sfactor, cwgl_enum_t dfactor);
CWGL_API void cwgl_blendColor(cwgl_ctx_t* ctx, float red, float green, float blue, float alpha);
// 4.2.2 Fine Control of Buffer Updates
CWGL_API void cwgl_colorMask(cwgl_ctx_t* ctx, int red, int green, int blue, int alpha);
CWGL_API void cwgl_depthMask(cwgl_ctx_t* ctx, int flag);
CWGL_API void cwgl_stencilMask(cwgl_ctx_t* ctx, uint32_t mask);
CWGL_API void cwgl_stencilMaskSeparate(cwgl_ctx_t* ctx, cwgl_enum_t face, uint32_t mask);
// 4.2.3 Clearing the Buffers
CWGL_API void cwgl_clear(cwgl_ctx_t* ctx, uint32_t mask);
CWGL_API void cwgl_clearColor(cwgl_ctx_t* ctx, float red, float green, float blue, float alpha);
CWGL_API void cwgl_clearDepth(cwgl_ctx_t* ctx, float depth);
CWGL_API void cwgl_clearStencil(cwgl_ctx_t* ctx, int32_t s);
// 4.3.1 Reading Pixels
CWGL_API void cwgl_readPixels(cwgl_ctx_t* ctx, int32_t x, int32_t y, uint32_t width, uint32_t height, cwgl_enum_t format, cwgl_enum_t type, void* buf, size_t buflen);
// 4.4.1 Binding and Managing Framebuffer Objects
CWGL_API void cwgl_bindFramebuffer(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_Framebuffer_t* framebuffer);
CWGL_API void cwgl_deleteFramebuffer(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer);
CWGL_API cwgl_Framebuffer_t* cwgl_createFramebuffer(cwgl_ctx_t* ctx);
// 4.4.3 Renderbuffer Objects
CWGL_API void cwgl_bindRenderbuffer(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_Renderbuffer_t* renderbuffer);
CWGL_API void cwgl_deleteRenderbuffer(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer);
CWGL_API cwgl_Renderbuffer_t* cwgl_createRenderbuffer(cwgl_ctx_t* ctx);
CWGL_API void cwgl_renderbufferStorage(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t internalformat, uint32_t width, uint32_t height);
CWGL_API void cwgl_framebufferRenderbuffer(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t attachment, cwgl_enum_t renderbuffertarget, cwgl_Renderbuffer_t* renderbuffer);
CWGL_API void cwgl_framebufferTexture2D(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t attachment, cwgl_enum_t textarget, cwgl_Texture_t* texture, int32_t level);
// 4.4.5 Framebuffer Completeness
CWGL_API cwgl_enum_t cwgl_checkFramebufferStatus(cwgl_ctx_t* ctx, cwgl_enum_t target);

// 5.1 Flush and Finish
CWGL_API void cwgl_finish(cwgl_ctx_t* ctx);
CWGL_API void cwgl_flush(cwgl_ctx_t* ctx);
// 5.2 Hints
CWGL_API void cwgl_hint(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t mode);

// 6.1.1 Simple Queries
CWGL_API cwgl_query_result_t cwgl_getParameter_b1(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t* x);
CWGL_API cwgl_query_result_t cwgl_getParameter_b4(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t* x, int32_t* y, int32_t* z, int32_t* w);
CWGL_API cwgl_query_result_t cwgl_getParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t* x);
CWGL_API cwgl_query_result_t cwgl_getParameter_i2(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t* x, int32_t* y);
CWGL_API cwgl_query_result_t cwgl_getParameter_i4(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t* x, int32_t* y, int32_t* z, int32_t* w);
CWGL_API cwgl_query_result_t cwgl_getParameter_f1(cwgl_ctx_t* ctx, cwgl_enum_t pname, float* x);
CWGL_API cwgl_query_result_t cwgl_getParameter_f2(cwgl_ctx_t* ctx, cwgl_enum_t pname, float* x, float* y);
CWGL_API cwgl_query_result_t cwgl_getParameter_f4(cwgl_ctx_t* ctx, cwgl_enum_t pname, float* x, float* y, float* z, float* w);
CWGL_API cwgl_query_result_t cwgl_getParameter_str(cwgl_ctx_t* ctx, cwgl_enum_t pname, cwgl_string_t** str);
CWGL_API cwgl_query_result_t cwgl_getParameter_Buffer(cwgl_ctx_t* ctx, cwgl_enum_t pname, cwgl_Buffer_t** buffer);
CWGL_API cwgl_query_result_t cwgl_getParameter_Program(cwgl_ctx_t* ctx, cwgl_enum_t pname, cwgl_Program_t** program);
CWGL_API cwgl_query_result_t cwgl_getParameter_Framebuffer(cwgl_ctx_t* ctx, cwgl_enum_t pname, cwgl_Framebuffer_t** framebuffer);
CWGL_API cwgl_query_result_t cwgl_getParameter_Renderbuffer(cwgl_ctx_t* ctx, cwgl_enum_t pname, cwgl_Renderbuffer_t** renderbuffer);
CWGL_API cwgl_query_result_t cwgl_getParameter_Texture(cwgl_ctx_t* ctx, cwgl_enum_t pname, cwgl_Texture_t** texture);
CWGL_API int cwgl_isEnabled(cwgl_ctx_t* ctx, cwgl_enum_t cap);
// 6.1.3 Enumerated Queries
CWGL_API cwgl_query_result_t cwgl_getTexParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t pname, int32_t* x);
CWGL_API cwgl_query_result_t cwgl_getBufferParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t pname, int32_t* x);
CWGL_API cwgl_query_result_t cwgl_getFramebufferAttachmentParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t attachment, cwgl_enum_t pname, int32_t* x);
CWGL_API cwgl_query_result_t cwgl_getFramebufferAttachmentParameter_Renderbuffer(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t attachment, cwgl_enum_t pname, cwgl_Renderbuffer_t** renderbuffer);
CWGL_API cwgl_query_result_t cwgl_getFramebufferAttachmentParameter_Texture(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t attachment, cwgl_enum_t pname, cwgl_Texture_t** texture);
CWGL_API cwgl_query_result_t cwgl_getRenderbufferParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t pname, int32_t* x);
// 6.1.4 Texture Queries
CWGL_API int cwgl_isTexture(cwgl_ctx_t* ctx, cwgl_Texture_t* texture);
// 6.1.6 Buffer Object Queries
CWGL_API int cwgl_isBuffer(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer);
// 6.1.7 Framebuffer Object and Renderbuffer Queries
CWGL_API int cwgl_isFramebuffer(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer);
CWGL_API int cwgl_isRenderbuffer(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer);
// 6.1.8 Shader and Program Queries
CWGL_API int cwgl_isShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader);
CWGL_API cwgl_query_result_t cwgl_getShaderParameter_i1(cwgl_ctx_t* ctx, cwgl_Shader_t* shader, cwgl_enum_t pname, int32_t* x);
CWGL_API int cwgl_isProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program);
CWGL_API cwgl_query_result_t cwgl_getProgramParameter_i1(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_enum_t pname, int32_t* x);
CWGL_API cwgl_query_result_t cwgl_getAttachedShaders(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_Shader_t** shader, size_t bufcount, size_t rescount);
CWGL_API cwgl_string_t* cwgl_getProgramInfoLog(cwgl_ctx_t* ctx, cwgl_Program_t* program);
CWGL_API cwgl_string_t* cwgl_getShaderInfoLog(cwgl_ctx_t* ctx, cwgl_Shader_t* shader);
CWGL_API cwgl_string_t* cwgl_getShaderSource(cwgl_ctx_t* ctx, cwgl_Shader_t* shader);
CWGL_API cwgl_query_result_t cwgl_getShaderPrecisionFormat(cwgl_ctx_t* ctx, cwgl_enum_t shadertype, cwgl_enum_t precisiontype, int32_t* rangeMin, int32_t* rangeMax, int32_t* precision);
CWGL_API cwgl_query_result_t cwgl_getVertexAttrib_i1(cwgl_ctx_t* ctx, uint32_t index, cwgl_enum_t pname, int32_t* x);
CWGL_API cwgl_query_result_t cwgl_getVertexAttrib_f4(cwgl_ctx_t* ctx, uint32_t index, cwgl_enum_t pname, float* x, float* y, float* z, float* w);
CWGL_API cwgl_query_result_t cwgl_getVertexAttrib_Buffer(cwgl_ctx_t* ctx, uint32_t index, cwgl_enum_t pname, cwgl_Buffer_t** buffer);
CWGL_API uint32_t cwgl_getVertexAttribOffset(cwgl_ctx_t* ctx, uint32_t index, cwgl_enum_t pname);
CWGL_API cwgl_query_result_t cwgl_getUniform_i1(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_UniformLocation_t* location, int32_t* x);
CWGL_API cwgl_query_result_t cwgl_getUniform_i2(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_UniformLocation_t* location, int32_t* x, int32_t* y);
CWGL_API cwgl_query_result_t cwgl_getUniform_i3(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_UniformLocation_t* location, int32_t* x, int32_t* y, int32_t* z);
CWGL_API cwgl_query_result_t cwgl_getUniform_i4(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_UniformLocation_t* location, int32_t* x, int32_t* y, int32_t* z, int32_t* w);
CWGL_API cwgl_query_result_t cwgl_getUniform_f1(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_UniformLocation_t* location, float* x);
CWGL_API cwgl_query_result_t cwgl_getUniform_f2(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_UniformLocation_t* location, float* x, float* y);
CWGL_API cwgl_query_result_t cwgl_getUniform_f3(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_UniformLocation_t* location, float* x, float* y, float* z);
CWGL_API cwgl_query_result_t cwgl_getUniform_f4(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_UniformLocation_t* location, float* x, float* y, float* z, float* w);
CWGL_API cwgl_query_result_t cwgl_getUniform_m2(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_UniformLocation_t* location, float* x);
CWGL_API cwgl_query_result_t cwgl_getUniform_m3(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_UniformLocation_t* location, float* x);
CWGL_API cwgl_query_result_t cwgl_getUniform_m4(cwgl_ctx_t* ctx, cwgl_Program_t* program, cwgl_UniformLocation_t* location, float* x);

// OES_vertex_array_object
CWGL_API cwgl_VertexArrayObject_t* cwgl_createVertexArray(cwgl_ctx_t* ctx);
CWGL_API void cwgl_bindVertexArray(cwgl_ctx_t* ctx, cwgl_VertexArrayObject_t* obj);

// {
#ifdef __cplusplus
};
#endif

#endif
