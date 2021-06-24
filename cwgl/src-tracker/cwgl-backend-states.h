#ifndef __YUNI_CWGL_BACKEND_GLES2_STATES_H
#define __YUNI_CWGL_BACKEND_GLES2_STATES_H

#ifdef __cplusplus
extern "C" {
#endif
// }

struct cwgl_backend_ctx_s;
struct cwgl_backend_Buffer_s;
struct cwgl_backend_Shader_s;
struct cwgl_backend_Program_s;
struct cwgl_backend_Texture_s;
struct cwgl_backend_Renderbuffer_s;
struct cwgl_backend_Framebuffer_s;

typedef struct cwgl_backend_ctx_s cwgl_backend_ctx_t;
typedef struct cwgl_backend_Buffer_s cwgl_backend_Buffer_t;
typedef struct cwgl_backend_Shader_s cwgl_backend_Shader_t;
typedef struct cwgl_backend_Program_s cwgl_backend_Program_t;
typedef struct cwgl_backend_Texture_s cwgl_backend_Texture_t;
typedef struct cwgl_backend_Renderbuffer_s cwgl_backend_Renderbuffer_t;
typedef struct cwgl_backend_Framebuffer_s cwgl_backend_Framebuffer_t;

int cwgl_backend_cfg_init(cwgl_ctx_config_t* cfg);
int cwgl_backend_ctx_init(cwgl_ctx_t* ctx);
int cwgl_backend_Buffer_init(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer);
int cwgl_backend_Shader_init(cwgl_ctx_t* ctx, cwgl_Shader_t* shader);
int cwgl_backend_Program_init(cwgl_ctx_t* ctx, cwgl_Program_t* program);
int cwgl_backend_Texture_init(cwgl_ctx_t* ctx, cwgl_Texture_t* texture);
int cwgl_backend_Renderbuffer_init(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer);
int cwgl_backend_Framebuffer_init(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer);

// FIXME: Make these void function
int cwgl_backend_ctx_release(cwgl_ctx_t* ctx);
int cwgl_backend_Buffer_release(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer);
int cwgl_backend_Shader_release(cwgl_ctx_t* ctx, cwgl_Shader_t* shader);
int cwgl_backend_Program_release(cwgl_ctx_t* ctx, cwgl_Program_t* program);
int cwgl_backend_Texture_release(cwgl_ctx_t* ctx, cwgl_Texture_t* texture);
int cwgl_backend_Renderbuffer_release(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer);
int cwgl_backend_Framebuffer_release(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer);

/* Draw */
int cwgl_backend_drawArrays(cwgl_ctx_t* ctx, cwgl_enum_t mode, int32_t first, uint32_t count);
int cwgl_backend_drawElements(cwgl_ctx_t* ctx, cwgl_enum_t mode, uint32_t count, cwgl_enum_t type, uint32_t offset);
int cwgl_backend_clear(cwgl_ctx_t* ctx, uint32_t mask);

/* buffer */
int cwgl_backend_bufferData(cwgl_ctx_t* ctx, cwgl_enum_t target, uint32_t size, void* data, cwgl_enum_t usage);
int cwgl_backend_bufferSubData(cwgl_ctx_t* ctx, cwgl_enum_t target, uint32_t offset, void* data, size_t buflen);

/* texture */
int cwgl_backend_texImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, cwgl_enum_t internalformat, uint32_t width, uint32_t height, int32_t border, cwgl_enum_t format, cwgl_enum_t type, const void* buf, size_t buflen);
int cwgl_backend_copyTexImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, cwgl_enum_t internalformat, int32_t x, int32_t y, uint32_t width, uint32_t height, int32_t border);
int cwgl_backend_texSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height, cwgl_enum_t format, cwgl_enum_t type, const void* buf, size_t buflen);
int cwgl_backend_copyTexSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, int32_t x, int32_t y, uint32_t width, uint32_t height);
int cwgl_backend_compressedTexImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, cwgl_enum_t internalformat, uint32_t width, uint32_t height, int32_t border, const void* buf, size_t buflen);
int cwgl_backend_compressedTexSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height, cwgl_enum_t format, const void* buf, size_t buflen);

/* mipmap */
int cwgl_backend_generateMipmap(cwgl_ctx_t* ctx, cwgl_enum_t target);

/* framebuffer */
int cwgl_backend_readPixels(cwgl_ctx_t* ctx, int32_t x, int32_t y, uint32_t width, uint32_t height, cwgl_enum_t format, cwgl_enum_t type, void* buf, size_t buflen);
int cwgl_backend_renderbufferStorage(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t internalformat, uint32_t width, uint32_t height);
int cwgl_backend_configure_framebuffer(cwgl_ctx_t* ctx, cwgl_enum_t* out_state);

/* Shader */
int cwgl_backend_compileShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader);
int cwgl_backend_linkProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program);
int cwgl_backend_validateProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program);

/* queue */
int cwgl_backend_finish(cwgl_ctx_t* ctx);
int cwgl_backend_flush(cwgl_ctx_t* ctx);

// {
#ifdef __cplusplus
};
#endif

#endif
