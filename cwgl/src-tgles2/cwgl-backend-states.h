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

int cwgl_backend_ctx_release(cwgl_ctx_t* ctx);
int cwgl_backend_Buffer_release(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer);
int cwgl_backend_Shader_release(cwgl_ctx_t* ctx, cwgl_Shader_t* shader);
int cwgl_backend_Program_release(cwgl_ctx_t* ctx, cwgl_Program_t* program);
int cwgl_backend_Texture_release(cwgl_ctx_t* ctx, cwgl_Texture_t* texture);
int cwgl_backend_Renderbuffer_release(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer);
int cwgl_backend_Framebuffer_release(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer);

// {
#ifdef __cplusplus
};
#endif

#endif
