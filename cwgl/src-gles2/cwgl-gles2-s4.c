#include "cwgl-gles2-priv.h"

// 4.1.2 Scissor Test
CWGL_API void
cwgl_scissor(cwgl_ctx_t* ctx, 
             int32_t x, int32_t y, uint32_t width, uint32_t height){
    CTX_ENTER(ctx);
    glScissor(x, y, width, height);
    CTX_LEAVE(ctx);
}

// 4.1.3 Multisample Fragment Operations
CWGL_API void 
cwgl_sampleCoverage(cwgl_ctx_t* ctx, float value, int invert){
    CTX_ENTER(ctx);
    glSampleCoverage(value, invert);
    CTX_LEAVE(ctx);
}

// 4.1.4 Stencil Test
CWGL_API void 
cwgl_stencilFunc(cwgl_ctx_t* ctx, 
                 cwgl_enum_t func, int32_t ref, uint32_t mask){
    CTX_ENTER(ctx);
    glStencilFunc(func, ref, mask);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_stencilFuncSeparate(cwgl_ctx_t* ctx, 
                         cwgl_enum_t face, cwgl_enum_t func, 
                         int32_t ref, uint32_t mask){
    CTX_ENTER(ctx);
    glStencilFuncSeparate(face, func, ref, mask);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_stencilOp(cwgl_ctx_t* ctx, 
               cwgl_enum_t fail, cwgl_enum_t zfail, cwgl_enum_t zpass){
    CTX_ENTER(ctx);
    glStencilOp(fail, zfail, zpass);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_stencilOpSeparate(cwgl_ctx_t* ctx,
                       cwgl_enum_t face, cwgl_enum_t fail, 
                       cwgl_enum_t zfail, cwgl_enum_t zpass){
    CTX_ENTER(ctx);
    glStencilOpSeparate(face, fail, zfail, zpass);
    CTX_LEAVE(ctx);
}

// 4.1.5 Depth Buffer Test
CWGL_API void 
cwgl_depthFunc(cwgl_ctx_t* ctx, cwgl_enum_t func){
    CTX_ENTER(ctx);
    glDepthFunc(func);
    CTX_LEAVE(ctx);
}

// 4.1.6 Blending
CWGL_API void 
cwgl_blendEquation(cwgl_ctx_t* ctx, cwgl_enum_t mode){
    CTX_ENTER(ctx);
    glBlendEquation(mode);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_blendEquationSeparate(cwgl_ctx_t* ctx,
                           cwgl_enum_t modeRGB, cwgl_enum_t modeAlpha){
    CTX_ENTER(ctx);
    glBlendEquationSeparate(modeRGB, modeAlpha);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_blendFuncSeparate(cwgl_ctx_t* ctx, cwgl_enum_t srcRGB, cwgl_enum_t dstRGB, cwgl_enum_t srcAlpha, cwgl_enum_t dstAlpha){
    CTX_ENTER(ctx);
    glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_blendFunc(cwgl_ctx_t* ctx, cwgl_enum_t sfactor, cwgl_enum_t dfactor){
    CTX_ENTER(ctx);
    glBlendFunc(sfactor, dfactor);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_blendColor(cwgl_ctx_t* ctx, 
                float red, float green, float blue, float alpha){
    CTX_ENTER(ctx);
    glBlendColor(red, green, blue, alpha);
    CTX_LEAVE(ctx);
}

// 4.2.2 Fine Control of Buffer Updates
CWGL_API void 
cwgl_colorMask(cwgl_ctx_t* ctx, int red, int green, int blue, int alpha){
    CTX_ENTER(ctx);
    glColorMask(red, green, blue, alpha);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_depthMask(cwgl_ctx_t* ctx, int flag){
    CTX_ENTER(ctx);
    glDepthMask(flag);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_stencilMask(cwgl_ctx_t* ctx, uint32_t mask){
    CTX_ENTER(ctx);
    glStencilMask(mask);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_stencilMaskSeparate(cwgl_ctx_t* ctx, cwgl_enum_t face, uint32_t mask){
    CTX_ENTER(ctx);
    glStencilMaskSeparate(face, mask);
    CTX_LEAVE(ctx);
}


// 4.2.3 Clearing the Buffers
CWGL_API void 
cwgl_clear(cwgl_ctx_t* ctx, uint32_t mask){
    CTX_ENTER(ctx);
    glClear(mask);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_clearColor(cwgl_ctx_t* ctx, 
                float red, float green, float blue, float alpha){
    CTX_ENTER(ctx);
    glClearColor(red, green, blue, alpha);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_clearDepth(cwgl_ctx_t* ctx, float depth){
    CTX_ENTER(ctx);
    glClearDepthf(depth);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_clearStencil(cwgl_ctx_t* ctx, int32_t s){
    CTX_ENTER(ctx);
    glClearStencil(s);
    CTX_LEAVE(ctx);
}


// 4.3.1 Reading Pixels
CWGL_API void 
cwgl_readPixels(cwgl_ctx_t* ctx, int32_t x, int32_t y, uint32_t width, uint32_t height, cwgl_enum_t format, cwgl_enum_t type, void* buf, size_t buflen){
    CTX_ENTER(ctx);
    glReadPixels(x, y, width, height, format, type, buf);
    CTX_LEAVE(ctx);
}

// 4.4.1 Binding and Managing Framebuffer Objects
CWGL_API void 
cwgl_bindFramebuffer(cwgl_ctx_t* ctx, 
                     cwgl_enum_t target, cwgl_Framebuffer_t* framebuffer){
    GLuint name;
    CTX_ENTER(ctx);
    if(framebuffer){
        name = CTX_GETNAME(ctx, framebuffer);
    }else{
        name = 0;
    }
    glBindFramebuffer(target, name);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_deleteFramebuffer(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer){
    GLuint name;
    CTX_ENTER(ctx);
    if(framebuffer){
        name = CTX_GETNAME(ctx, framebuffer);
        glDeleteFramebuffers(1, &name);
    }
    CTX_LEAVE(ctx);
}

CWGL_API cwgl_Framebuffer_t* 
cwgl_createFramebuffer(cwgl_ctx_t* ctx){
    GLuint name;
    cwgl_Framebuffer_t* f;
    CTX_ENTER(ctx);
    glGenFramebuffers(1, &name);
    f = CTX_ALLOC(ctx, Framebuffer);
    CTX_SETNAME(ctx, f, name);
    CTX_LEAVE(ctx);
    return f;
}

CWGL_API void
cwgl_Framebuffer_release(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer){
    //CTX_ENTER(ctx);
    if(framebuffer){
        CTX_FREE(ctx, Framebuffer, framebuffer);
    }
    //CTX_LEAVE(ctx);
}

// 4.4.3 Renderbuffer Objects
CWGL_API void 
cwgl_bindRenderbuffer(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                      cwgl_Renderbuffer_t* renderbuffer){
    GLuint name;
    CTX_ENTER(ctx);
    if(renderbuffer){
        name = CTX_GETNAME(ctx, renderbuffer);
    }else{
        name = 0;
    }
    glBindRenderbuffer(target, name);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_deleteRenderbuffer(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer){
    GLuint name;
    CTX_ENTER(ctx);
    if(renderbuffer){
        name = CTX_GETNAME(ctx, renderbuffer);
        glDeleteRenderbuffers(1, &name);
    }
    CTX_LEAVE(ctx);
}

CWGL_API cwgl_Renderbuffer_t* 
cwgl_createRenderbuffer(cwgl_ctx_t* ctx){
    GLuint name = 0;
    cwgl_Renderbuffer_t* r;
    CTX_ENTER(ctx);
    glGenRenderbuffers(1, &name);
    r = CTX_ALLOC(ctx, Renderbuffer);
    CTX_SETNAME(ctx, r, name);
    CTX_LEAVE(ctx);
    return r;

}

CWGL_API void
cwgl_Renderbuffer_release(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer){
    //CTX_ENTER(ctx);
    if(renderbuffer){
        CTX_FREE(ctx, Renderbuffer, renderbuffer);
    }
    //CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_renderbufferStorage(cwgl_ctx_t* ctx, 
                         cwgl_enum_t target, cwgl_enum_t internalformat, 
                         uint32_t width, uint32_t height){
    CTX_ENTER(ctx);
    glRenderbufferStorage(target, internalformat, width, height);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_framebufferRenderbuffer(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                             cwgl_enum_t attachment, 
                             cwgl_enum_t renderbuffertarget, 
                             cwgl_Renderbuffer_t* renderbuffer){
    GLuint name;
    CTX_ENTER(ctx);
    if(renderbuffer){
        name = CTX_GETNAME(ctx, renderbuffer);
    }else{
        name = 0;
    }
    glFramebufferRenderbuffer(target, attachment, renderbuffertarget, name);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_framebufferTexture2D(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                          cwgl_enum_t attachment, cwgl_enum_t textarget, 
                          cwgl_Texture_t* texture, int32_t level){
    GLuint name;
    CTX_ENTER(ctx);
    if(texture){
        name = CTX_GETNAME(ctx, texture);
    }else{
        name = 0;
    }
    glFramebufferTexture2D(target, attachment, textarget, name, level);
    CTX_LEAVE(ctx);
}


// 4.4.5 Framebuffer Completeness
CWGL_API cwgl_enum_t 
cwgl_checkFramebufferStatus(cwgl_ctx_t* ctx, cwgl_enum_t target){
    cwgl_enum_t e;
    CTX_ENTER(ctx);
    e = glCheckFramebufferStatus(target);
    CTX_LEAVE(ctx);
    return e;
}

