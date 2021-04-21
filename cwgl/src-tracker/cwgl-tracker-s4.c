#include "cwgl-tracker-priv.h"

// 4.1.2 Scissor Test
CWGL_API void
cwgl_scissor(cwgl_ctx_t* ctx, 
             int32_t x, int32_t y, uint32_t width, uint32_t height){
}

// 4.1.3 Multisample Fragment Operations
CWGL_API void 
cwgl_sampleCoverage(cwgl_ctx_t* ctx, float value, int invert){
}

// 4.1.4 Stencil Test
CWGL_API void 
cwgl_stencilFunc(cwgl_ctx_t* ctx, 
                 cwgl_enum_t func, int32_t ref, uint32_t mask){
}

CWGL_API void 
cwgl_stencilFuncSeparate(cwgl_ctx_t* ctx, 
                         cwgl_enum_t face, cwgl_enum_t func, 
                         int32_t ref, uint32_t mask){
}

CWGL_API void 
cwgl_stencilOp(cwgl_ctx_t* ctx, 
               cwgl_enum_t fail, cwgl_enum_t zfail, cwgl_enum_t zpass){
}

CWGL_API void 
cwgl_stencilOpSeparate(cwgl_ctx_t* ctx,
                       cwgl_enum_t face, cwgl_enum_t fail, 
                       cwgl_enum_t zfail, cwgl_enum_t zpass){
}

// 4.1.5 Depth Buffer Test
CWGL_API void 
cwgl_depthFunc(cwgl_ctx_t* ctx, cwgl_enum_t func){
}

// 4.1.6 Blending
CWGL_API void 
cwgl_blendEquation(cwgl_ctx_t* ctx, cwgl_enum_t mode){
}

CWGL_API void 
cwgl_blendEquationSeparate(cwgl_ctx_t* ctx,
                           cwgl_enum_t modeRGB, cwgl_enum_t modeAlpha){
}

CWGL_API void 
cwgl_blendFuncSeparate(cwgl_ctx_t* ctx, cwgl_enum_t srcRGB, cwgl_enum_t dstRGB, cwgl_enum_t srcAlpha, cwgl_enum_t dstAlpha){
}

CWGL_API void 
cwgl_blendFunc(cwgl_ctx_t* ctx, cwgl_enum_t sfactor, cwgl_enum_t dfactor){
}

CWGL_API void 
cwgl_blendColor(cwgl_ctx_t* ctx, 
                float red, float green, float blue, float alpha){
}

// 4.2.2 Fine Control of Buffer Updates
CWGL_API void 
cwgl_colorMask(cwgl_ctx_t* ctx, int red, int green, int blue, int alpha){
}

CWGL_API void 
cwgl_depthMask(cwgl_ctx_t* ctx, int flag){
}

CWGL_API void 
cwgl_stencilMask(cwgl_ctx_t* ctx, uint32_t mask){
}

CWGL_API void 
cwgl_stencilMaskSeparate(cwgl_ctx_t* ctx, cwgl_enum_t face, uint32_t mask){
}


// 4.2.3 Clearing the Buffers
CWGL_API void 
cwgl_clear(cwgl_ctx_t* ctx, uint32_t mask){
}

CWGL_API void 
cwgl_clearColor(cwgl_ctx_t* ctx, 
                float red, float green, float blue, float alpha){
}

CWGL_API void 
cwgl_clearDepth(cwgl_ctx_t* ctx, float depth){
}

CWGL_API void 
cwgl_clearStencil(cwgl_ctx_t* ctx, int32_t s){
}


// 4.3.1 Reading Pixels
CWGL_API void 
cwgl_readPixels(cwgl_ctx_t* ctx, int32_t x, int32_t y, uint32_t width, uint32_t height, cwgl_enum_t format, cwgl_enum_t type, void* buf, size_t buflen){
}

// 4.4.1 Binding and Managing Framebuffer Objects
CWGL_API void 
cwgl_bindFramebuffer(cwgl_ctx_t* ctx, 
                     cwgl_enum_t target, cwgl_Framebuffer_t* framebuffer){
}

CWGL_API void 
cwgl_deleteFramebuffer(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer){
}

CWGL_API cwgl_Framebuffer_t* 
cwgl_createFramebuffer(cwgl_ctx_t* ctx){
}

CWGL_API void
cwgl_Framebuffer_release(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer){
}

// 4.4.3 Renderbuffer Objects
CWGL_API void 
cwgl_bindRenderbuffer(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                      cwgl_Renderbuffer_t* renderbuffer){
}

CWGL_API void 
cwgl_deleteRenderbuffer(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer){
}

CWGL_API cwgl_Renderbuffer_t* 
cwgl_createRenderbuffer(cwgl_ctx_t* ctx){
}

CWGL_API void
cwgl_Renderbuffer_release(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer){
}

CWGL_API void 
cwgl_renderbufferStorage(cwgl_ctx_t* ctx, 
                         cwgl_enum_t target, cwgl_enum_t internalformat, 
                         uint32_t width, uint32_t height){
}

CWGL_API void 
cwgl_framebufferRenderbuffer(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                             cwgl_enum_t attachment, 
                             cwgl_enum_t renderbuffertarget, 
                             cwgl_Renderbuffer_t* renderbuffer){
}

CWGL_API void 
cwgl_framebufferTexture2D(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                          cwgl_enum_t attachment, cwgl_enum_t textarget, 
                          cwgl_Texture_t* texture, int32_t level){
}


// 4.4.5 Framebuffer Completeness
CWGL_API cwgl_enum_t 
cwgl_checkFramebufferStatus(cwgl_ctx_t* ctx, cwgl_enum_t target){
}

