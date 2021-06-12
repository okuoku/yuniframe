#include "cwgl-tracker-priv.h"
#include <stdlib.h>

// 4.1.2 Scissor Test
CWGL_API void
cwgl_scissor(cwgl_ctx_t* ctx, 
             int32_t x, int32_t y, uint32_t width, uint32_t height){
    /* GLES2: CWGL never return INVALID_VALUE */
    ctx->state.glo.SCISSOR_BOX[0] = x;
    ctx->state.glo.SCISSOR_BOX[1] = y;
    ctx->state.glo.SCISSOR_BOX[2] = width;
    ctx->state.glo.SCISSOR_BOX[3] = height;
}

// 4.1.3 Multisample Fragment Operations
CWGL_API void 
cwgl_sampleCoverage(cwgl_ctx_t* ctx, float value, int invert){
    ctx->state.glo.SAMPLE_COVERAGE_VALUE = value;
    ctx->state.glo.SAMPLE_COVERAGE_INVERT = invert ? CWGL_TRUE : CWGL_FALSE;
}

// 4.1.4 Stencil Test
CWGL_API void 
cwgl_stencilFunc(cwgl_ctx_t* ctx, 
                 cwgl_enum_t func, int32_t ref, uint32_t mask){
    switch(func){
        case NEVER:
        case LESS:
        case EQUAL:
        case LEQUAL:
        case GREATER:
        case NOTEQUAL:
        case GEQUAL:
        case ALWAYS:
            ctx->state.glo.STENCIL_FUNC = func;
            ctx->state.glo.STENCIL_VALUE_MASK = mask;
            ctx->state.glo.STENCIL_REF = ref;
            ctx->state.glo.STENCIL_BACK_FUNC = func;
            ctx->state.glo.STENCIL_BACK_VALUE_MASK = mask;
            ctx->state.glo.STENCIL_BACK_REF = ref;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            break;
    }

}

CWGL_API void 
cwgl_stencilFuncSeparate(cwgl_ctx_t* ctx, 
                         cwgl_enum_t face, cwgl_enum_t func, 
                         int32_t ref, uint32_t mask){
    switch(func){
        case NEVER:
        case LESS:
        case EQUAL:
        case LEQUAL:
        case GREATER:
        case NOTEQUAL:
        case GEQUAL:
        case ALWAYS:
            switch(face){
                case FRONT:
                    ctx->state.glo.STENCIL_FUNC = func;
                    ctx->state.glo.STENCIL_VALUE_MASK = mask;
                    ctx->state.glo.STENCIL_REF = ref;
                    break;
                case BACK:
                    ctx->state.glo.STENCIL_BACK_FUNC = func;
                    ctx->state.glo.STENCIL_BACK_VALUE_MASK = mask;
                    ctx->state.glo.STENCIL_BACK_REF = ref;
                    break;
                case FRONT_AND_BACK:
                    ctx->state.glo.STENCIL_FUNC = func;
                    ctx->state.glo.STENCIL_VALUE_MASK = mask;
                    ctx->state.glo.STENCIL_REF = ref;
                    ctx->state.glo.STENCIL_BACK_FUNC = func;
                    ctx->state.glo.STENCIL_BACK_VALUE_MASK = mask;
                    ctx->state.glo.STENCIL_BACK_REF = ref;
                    break;
                default:
                    CTX_SET_ERROR(ctx, INVALID_ENUM);
                    break;
            }
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            break;
    }
}

CWGL_API void 
cwgl_stencilOp(cwgl_ctx_t* ctx, 
               cwgl_enum_t fail, cwgl_enum_t zfail, cwgl_enum_t zpass){
    switch(fail){
        case KEEP:
        case ZERO:
        case REPLACE:
        case INCR:
        case INCR_WRAP:
        case DECR:
        case DECR_WRAP:
        case INVERT:
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    switch(zfail){
        case KEEP:
        case ZERO:
        case REPLACE:
        case INCR:
        case INCR_WRAP:
        case DECR:
        case DECR_WRAP:
        case INVERT:
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    switch(zpass){
        case KEEP:
        case ZERO:
        case REPLACE:
        case INCR:
        case INCR_WRAP:
        case DECR:
        case DECR_WRAP:
        case INVERT:
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }

    ctx->state.glo.STENCIL_FAIL = fail;
    ctx->state.glo.STENCIL_PASS_DEPTH_FAIL = zfail;
    ctx->state.glo.STENCIL_PASS_DEPTH_PASS = zpass;

    ctx->state.glo.STENCIL_BACK_FAIL = fail;
    ctx->state.glo.STENCIL_BACK_PASS_DEPTH_FAIL = zfail;
    ctx->state.glo.STENCIL_BACK_PASS_DEPTH_PASS = zpass;

}

CWGL_API void 
cwgl_stencilOpSeparate(cwgl_ctx_t* ctx,
                       cwgl_enum_t face, cwgl_enum_t fail, 
                       cwgl_enum_t zfail, cwgl_enum_t zpass){
    switch(fail){
        case KEEP:
        case ZERO:
        case REPLACE:
        case INCR:
        case INCR_WRAP:
        case DECR:
        case DECR_WRAP:
        case INVERT:
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    switch(zfail){
        case KEEP:
        case ZERO:
        case REPLACE:
        case INCR:
        case INCR_WRAP:
        case DECR:
        case DECR_WRAP:
        case INVERT:
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    switch(zpass){
        case KEEP:
        case ZERO:
        case REPLACE:
        case INCR:
        case INCR_WRAP:
        case DECR:
        case DECR_WRAP:
        case INVERT:
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    switch(face){
        case FRONT:
            ctx->state.glo.STENCIL_FAIL = fail;
            ctx->state.glo.STENCIL_PASS_DEPTH_FAIL = zfail;
            ctx->state.glo.STENCIL_PASS_DEPTH_PASS = zpass;
            break;
        case BACK:
            ctx->state.glo.STENCIL_BACK_FAIL = fail;
            ctx->state.glo.STENCIL_BACK_PASS_DEPTH_FAIL = zfail;
            ctx->state.glo.STENCIL_BACK_PASS_DEPTH_PASS = zpass;
            break;
        case FRONT_AND_BACK:
            ctx->state.glo.STENCIL_FAIL = fail;
            ctx->state.glo.STENCIL_PASS_DEPTH_FAIL = zfail;
            ctx->state.glo.STENCIL_PASS_DEPTH_PASS = zpass;
            ctx->state.glo.STENCIL_BACK_FAIL = fail;
            ctx->state.glo.STENCIL_BACK_PASS_DEPTH_FAIL = zfail;
            ctx->state.glo.STENCIL_BACK_PASS_DEPTH_PASS = zpass;
            break;
    }
}

// 4.1.5 Depth Buffer Test
CWGL_API void 
cwgl_depthFunc(cwgl_ctx_t* ctx, cwgl_enum_t func){
    switch(func){
        case NEVER:
        case LESS:
        case EQUAL:
        case LEQUAL:
        case GREATER:
        case NOTEQUAL:
        case GEQUAL:
        case ALWAYS:
            ctx->state.glo.DEPTH_FUNC = func;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            break;
    }
}

// 4.1.6 Blending
CWGL_API void 
cwgl_blendEquation(cwgl_ctx_t* ctx, cwgl_enum_t mode){
    switch(mode){
        case FUNC_ADD:
        case FUNC_SUBTRACT:
        case FUNC_REVERSE_SUBTRACT:
            ctx->state.glo.BLEND_EQUATION_RGB = mode;
            ctx->state.glo.BLEND_EQUATION_ALPHA = mode;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            break;
    }
}

CWGL_API void 
cwgl_blendEquationSeparate(cwgl_ctx_t* ctx,
                           cwgl_enum_t modeRGB, cwgl_enum_t modeAlpha){
    switch(modeRGB){
        case FUNC_ADD:
        case FUNC_SUBTRACT:
        case FUNC_REVERSE_SUBTRACT:
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    switch(modeAlpha){
        case FUNC_ADD:
        case FUNC_SUBTRACT:
        case FUNC_REVERSE_SUBTRACT:
            ctx->state.glo.BLEND_EQUATION_RGB = modeRGB;
            ctx->state.glo.BLEND_EQUATION_ALPHA = modeAlpha;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            break;
    }
}

CWGL_API void 
cwgl_blendFuncSeparate(cwgl_ctx_t* ctx, cwgl_enum_t srcRGB, cwgl_enum_t dstRGB, cwgl_enum_t srcAlpha, cwgl_enum_t dstAlpha){
    switch(srcRGB){
        case ZERO:
        case ONE:
        case SRC_COLOR:
        case ONE_MINUS_SRC_COLOR:
        case DST_COLOR:
        case ONE_MINUS_DST_COLOR:
        case SRC_ALPHA:
        case ONE_MINUS_SRC_ALPHA:
        case DST_ALPHA:
        case ONE_MINUS_DST_ALPHA:
        case CONSTANT_COLOR:
        case ONE_MINUS_CONSTANT_COLOR:
        case CONSTANT_ALPHA:
        case ONE_MINUS_CONSTANT_ALPHA:
        case SRC_ALPHA_SATURATE:
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    switch(srcAlpha){
        case ZERO:
        case ONE:
        case SRC_COLOR:
        case ONE_MINUS_SRC_COLOR:
        case DST_COLOR:
        case ONE_MINUS_DST_COLOR:
        case SRC_ALPHA:
        case ONE_MINUS_SRC_ALPHA:
        case DST_ALPHA:
        case ONE_MINUS_DST_ALPHA:
        case CONSTANT_COLOR:
        case ONE_MINUS_CONSTANT_COLOR:
        case CONSTANT_ALPHA:
        case ONE_MINUS_CONSTANT_ALPHA:
        case SRC_ALPHA_SATURATE:
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    switch(dstRGB){
        case ZERO:
        case ONE:
        case SRC_COLOR:
        case ONE_MINUS_SRC_COLOR:
        case DST_COLOR:
        case ONE_MINUS_DST_COLOR:
        case SRC_ALPHA:
        case ONE_MINUS_SRC_ALPHA:
        case DST_ALPHA:
        case ONE_MINUS_DST_ALPHA:
        case CONSTANT_COLOR:
        case ONE_MINUS_CONSTANT_COLOR:
        case CONSTANT_ALPHA:
        case ONE_MINUS_CONSTANT_ALPHA:
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    switch(dstAlpha){
        case ZERO:
        case ONE:
        case SRC_COLOR:
        case ONE_MINUS_SRC_COLOR:
        case DST_COLOR:
        case ONE_MINUS_DST_COLOR:
        case SRC_ALPHA:
        case ONE_MINUS_SRC_ALPHA:
        case DST_ALPHA:
        case ONE_MINUS_DST_ALPHA:
        case CONSTANT_COLOR:
        case ONE_MINUS_CONSTANT_COLOR:
        case CONSTANT_ALPHA:
        case ONE_MINUS_CONSTANT_ALPHA:
            ctx->state.glo.BLEND_SRC_RGB = srcRGB;
            ctx->state.glo.BLEND_SRC_ALPHA = srcAlpha;
            ctx->state.glo.BLEND_DST_RGB = dstRGB;
            ctx->state.glo.BLEND_DST_ALPHA = dstAlpha;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
}

CWGL_API void 
cwgl_blendFunc(cwgl_ctx_t* ctx, cwgl_enum_t sfactor, cwgl_enum_t dfactor){
    switch(sfactor){
        case ZERO:
        case ONE:
        case SRC_COLOR:
        case ONE_MINUS_SRC_COLOR:
        case DST_COLOR:
        case ONE_MINUS_DST_COLOR:
        case SRC_ALPHA:
        case ONE_MINUS_SRC_ALPHA:
        case DST_ALPHA:
        case ONE_MINUS_DST_ALPHA:
        case CONSTANT_COLOR:
        case ONE_MINUS_CONSTANT_COLOR:
        case CONSTANT_ALPHA:
        case ONE_MINUS_CONSTANT_ALPHA:
        case SRC_ALPHA_SATURATE:
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    switch(dfactor){
        case ZERO:
        case ONE:
        case SRC_COLOR:
        case ONE_MINUS_SRC_COLOR:
        case DST_COLOR:
        case ONE_MINUS_DST_COLOR:
        case SRC_ALPHA:
        case ONE_MINUS_SRC_ALPHA:
        case DST_ALPHA:
        case ONE_MINUS_DST_ALPHA:
        case CONSTANT_COLOR:
        case ONE_MINUS_CONSTANT_COLOR:
        case CONSTANT_ALPHA:
        case ONE_MINUS_CONSTANT_ALPHA:
            ctx->state.glo.BLEND_SRC_RGB = sfactor;
            ctx->state.glo.BLEND_SRC_ALPHA = sfactor;
            ctx->state.glo.BLEND_DST_RGB = dfactor;
            ctx->state.glo.BLEND_DST_ALPHA = dfactor;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
}

CWGL_API void 
cwgl_blendColor(cwgl_ctx_t* ctx, 
                float red, float green, float blue, float alpha){
    ctx->state.glo.BLEND_COLOR[0] = red;
    ctx->state.glo.BLEND_COLOR[1] = green;
    ctx->state.glo.BLEND_COLOR[2] = blue;
    ctx->state.glo.BLEND_COLOR[3] = alpha;
}

// 4.2.2 Fine Control of Buffer Updates
CWGL_API void 
cwgl_colorMask(cwgl_ctx_t* ctx, int red, int green, int blue, int alpha){
    ctx->state.glo.COLOR_WRITEMASK[0] = red ? CWGL_TRUE : CWGL_FALSE;
    ctx->state.glo.COLOR_WRITEMASK[1] = green ? CWGL_TRUE : CWGL_FALSE;
    ctx->state.glo.COLOR_WRITEMASK[2] = blue ? CWGL_TRUE : CWGL_FALSE;
    ctx->state.glo.COLOR_WRITEMASK[3] = alpha ? CWGL_TRUE : CWGL_FALSE;
}

CWGL_API void 
cwgl_depthMask(cwgl_ctx_t* ctx, int flag){
    ctx->state.glo.DEPTH_WRITEMASK = flag ? CWGL_TRUE : CWGL_FALSE;
}

CWGL_API void 
cwgl_stencilMask(cwgl_ctx_t* ctx, uint32_t mask){
    ctx->state.glo.STENCIL_WRITEMASK = mask;
    ctx->state.glo.STENCIL_BACK_WRITEMASK = mask;
}

CWGL_API void 
cwgl_stencilMaskSeparate(cwgl_ctx_t* ctx, cwgl_enum_t face, uint32_t mask){
    switch(mask){
        case FRONT:
            ctx->state.glo.STENCIL_WRITEMASK = mask;
            break;
        case BACK:
            ctx->state.glo.STENCIL_BACK_WRITEMASK = mask;
            break;
        case FRONT_AND_BACK:
            ctx->state.glo.STENCIL_WRITEMASK = mask;
            ctx->state.glo.STENCIL_BACK_WRITEMASK = mask;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            break;
    }
}


// 4.2.3 Clearing the Buffers
CWGL_API void 
cwgl_clear(cwgl_ctx_t* ctx, uint32_t mask){
    cwgl_backend_clear(ctx, mask);
}

CWGL_API void 
cwgl_clearColor(cwgl_ctx_t* ctx, 
                float red, float green, float blue, float alpha){
    ctx->state.glo.COLOR_CLEAR_VALUE[0] = red;
    ctx->state.glo.COLOR_CLEAR_VALUE[1] = green;
    ctx->state.glo.COLOR_CLEAR_VALUE[2] = blue;
    ctx->state.glo.COLOR_CLEAR_VALUE[3] = alpha;
}

CWGL_API void 
cwgl_clearDepth(cwgl_ctx_t* ctx, float depth){
    if(depth > 1.0f){
        ctx->state.glo.DEPTH_CLEAR_VALUE = 1.0f;
    }else if(depth < 0.0f){
        ctx->state.glo.DEPTH_CLEAR_VALUE = 0;
    }else{
        ctx->state.glo.DEPTH_CLEAR_VALUE = depth;
    }
}

CWGL_API void 
cwgl_clearStencil(cwgl_ctx_t* ctx, int32_t s){
    /* FIXME: GLES2 requires implicit AND against with current STENCIL_BITS */
    /*
    const unsigned int stencilwidth = ctx->state.cfg.STENCIL_BITS;
    const uint32_t stencilmask = (1 << stencilwidth) -1;

    ctx->state.glo.STENCIL_CLEAR_VALUE = (s & stencilmask);
    */
    ctx->state.glo.STENCIL_CLEAR_VALUE = s;
}


// 4.3.1 Reading Pixels
CWGL_API void 
cwgl_readPixels(cwgl_ctx_t* ctx, int32_t x, int32_t y, uint32_t width, uint32_t height, cwgl_enum_t format, cwgl_enum_t type, void* buf, size_t buflen){
    cwgl_backend_readPixels(ctx, x, y, width, height, format, type, buf, buflen);
}

// 4.4.1 Binding and Managing Framebuffer Objects
static void release_renderbuffer(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* rb);
static void
release_framebuffer_attachment(cwgl_ctx_t* ctx, cwgl_framebuffer_attachment_state_t* a){
    switch(a->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE){
        case RENDERBUFFER:
            release_renderbuffer(ctx, a->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asRenderbuffer);
            break;
        case TEXTURE:
            cwgl_priv_texture_release(ctx, a->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asTexture);
            break;
        default:
            /* Do nothing */
            break;
    }
}

static void
release_framebuffer(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* fb){
    uintptr_t v;
    if(fb){
        v = cwgl_priv_objhdr_release(&fb->hdr);
        if(!v){
            release_framebuffer_attachment(ctx, &fb->state.COLOR_ATTACHMENT0);
            release_framebuffer_attachment(ctx, &fb->state.DEPTH_ATTACHMENT);
            release_framebuffer_attachment(ctx, &fb->state.STENCIL_ATTACHMENT);
            cwgl_backend_Framebuffer_release(ctx, fb);
            free(fb);
        }
    }
}
static void
unbind_framebuffer(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* fb){
    if(fb){
        if(ctx->state.bin.FRAMEBUFFER_BINDING == fb){
            release_framebuffer(ctx, fb);
            ctx->state.bin.FRAMEBUFFER_BINDING = NULL;
        }
    }
}
CWGL_API void 
cwgl_bindFramebuffer(cwgl_ctx_t* ctx, 
                     cwgl_enum_t target, cwgl_Framebuffer_t* framebuffer){
    switch(target){
        case FRAMEBUFFER:
            unbind_framebuffer(ctx, ctx->state.bin.FRAMEBUFFER_BINDING);
            if(framebuffer){
                cwgl_priv_objhdr_retain(&framebuffer->hdr);
            }
            ctx->state.bin.FRAMEBUFFER_BINDING = framebuffer;
            cwgl_backend_configure_framebuffer(ctx, NULL);
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            break;
    }
}

CWGL_API void 
cwgl_deleteFramebuffer(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer){
    unbind_framebuffer(ctx, framebuffer);
}

CWGL_API cwgl_Framebuffer_t* 
cwgl_createFramebuffer(cwgl_ctx_t* ctx){
    cwgl_Framebuffer_t* fb;
    fb = malloc(sizeof(cwgl_Framebuffer_t));
    if(fb){
        cwgl_priv_objhdr_init(ctx, &fb->hdr, CWGL_OBJ_FRAMEBUFFER);
        cwgl_priv_framebuffer_attachment_init(&fb->state.COLOR_ATTACHMENT0);
        cwgl_priv_framebuffer_attachment_init(&fb->state.DEPTH_ATTACHMENT);
        cwgl_priv_framebuffer_attachment_init(&fb->state.STENCIL_ATTACHMENT);
        cwgl_backend_Framebuffer_init(ctx, fb);
    }
    return fb;
}

CWGL_API void
cwgl_Framebuffer_release(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer){
    release_framebuffer(ctx, framebuffer);
}

// 4.4.3 Renderbuffer Objects
static void
release_renderbuffer(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* rb){
    uintptr_t v;
    if(rb){
        v = cwgl_priv_objhdr_release(&rb->hdr);
        if(! v){
            cwgl_backend_Renderbuffer_release(ctx, rb);
            free(rb);
        }
    }
}
static void
unbind_renderbuffer(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* rb){
    if(rb){
        if(ctx->state.bin.RENDERBUFFER_BINDING == rb){
            release_renderbuffer(ctx, rb);
            ctx->state.bin.RENDERBUFFER_BINDING = NULL;
        }
    }
}

CWGL_API void 
cwgl_bindRenderbuffer(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                      cwgl_Renderbuffer_t* renderbuffer){
    switch(target){
        case RENDERBUFFER:
            unbind_renderbuffer(ctx, ctx->state.bin.RENDERBUFFER_BINDING);
            if(renderbuffer){
                cwgl_priv_objhdr_retain(&renderbuffer->hdr);
            }
            ctx->state.bin.RENDERBUFFER_BINDING = renderbuffer;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            break;
    }
}

CWGL_API void 
cwgl_deleteRenderbuffer(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer){
    release_renderbuffer(ctx, renderbuffer);
}

CWGL_API cwgl_Renderbuffer_t* 
cwgl_createRenderbuffer(cwgl_ctx_t* ctx){
    cwgl_Renderbuffer_t* rb;
    rb = malloc(sizeof(cwgl_Renderbuffer_t));
    if(rb){
        cwgl_priv_objhdr_init(ctx, &rb->hdr, CWGL_OBJ_RENDERBUFFER);
        cwgl_priv_renderbuffer_init(&rb->state);
        cwgl_backend_Renderbuffer_init(ctx, rb);
    }
    return rb;
}

CWGL_API void
cwgl_Renderbuffer_release(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer){
    release_renderbuffer(ctx, renderbuffer);
}

CWGL_API void 
cwgl_renderbufferStorage(cwgl_ctx_t* ctx, 
                         cwgl_enum_t target, cwgl_enum_t internalformat, 
                         uint32_t width, uint32_t height){
    if(target != RENDERBUFFER){
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return;
    }
    if(! ctx->state.bin.RENDERBUFFER_BINDING){
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return;
    }
    cwgl_backend_renderbufferStorage(ctx, target,
                                     internalformat, width, height);
}

CWGL_API void 
cwgl_framebufferRenderbuffer(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                             cwgl_enum_t attachment, 
                             cwgl_enum_t renderbuffertarget, 
                             cwgl_Renderbuffer_t* renderbuffer){
    cwgl_Framebuffer_t* fb;
    cwgl_framebuffer_attachment_state_t* point;
    if(renderbuffertarget != RENDERBUFFER){
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return;
    }
    if(target != FRAMEBUFFER){
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return;
    }
    fb = ctx->state.bin.FRAMEBUFFER_BINDING;
    if(! fb){
        /* WebGL: No default framebuffer */
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return;
    }
    switch(attachment){
        case COLOR_ATTACHMENT0:
            point = &fb->state.COLOR_ATTACHMENT0;
            break;
        case DEPTH_ATTACHMENT:
            point = &fb->state.DEPTH_ATTACHMENT;
            break;
        case STENCIL_ATTACHMENT:
            point = &fb->state.STENCIL_ATTACHMENT;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    release_framebuffer_attachment(ctx, point);
    if(renderbuffer){
        cwgl_priv_objhdr_retain(&renderbuffer->hdr);
        point->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE = RENDERBUFFER;
    }else{
        cwgl_priv_framebuffer_attachment_init(point);
    }
    point->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asRenderbuffer = renderbuffer;
    cwgl_backend_configure_framebuffer(ctx, NULL);
}

CWGL_API void 
cwgl_framebufferTexture2D(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                          cwgl_enum_t attachment, cwgl_enum_t textarget, 
                          cwgl_Texture_t* texture, int32_t level){
    cwgl_Framebuffer_t* fb;
    cwgl_framebuffer_attachment_state_t* point;
    if(texture){
        if(level != 0){
            CTX_SET_ERROR(ctx, INVALID_VALUE);
            return;
        }
    }
    switch(textarget){
        case TEXTURE_2D:
        case TEXTURE_CUBE_MAP_POSITIVE_X:
        case TEXTURE_CUBE_MAP_NEGATIVE_X:
        case TEXTURE_CUBE_MAP_POSITIVE_Y:
        case TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case TEXTURE_CUBE_MAP_POSITIVE_Z:
        case TEXTURE_CUBE_MAP_NEGATIVE_Z:
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    if(target != FRAMEBUFFER){
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return;
    }
    fb = ctx->state.bin.FRAMEBUFFER_BINDING;
    if(! fb){
        /* WebGL: No default framebuffer */
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return;
    }
    switch(attachment){
        case COLOR_ATTACHMENT0:
            point = &fb->state.COLOR_ATTACHMENT0;
            break;
        case DEPTH_ATTACHMENT:
            point = &fb->state.DEPTH_ATTACHMENT;
            break;
        case STENCIL_ATTACHMENT:
            point = &fb->state.STENCIL_ATTACHMENT;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    release_framebuffer_attachment(ctx, point);
    if(texture){
        cwgl_priv_objhdr_retain(&texture->hdr);
        point->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE = TEXTURE;
        point->FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL = level;
        point->FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE = textarget;
    }else{
        cwgl_priv_framebuffer_attachment_init(point);
    }
    point->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asTexture = texture;
    cwgl_backend_configure_framebuffer(ctx, NULL);
}


// 4.4.5 Framebuffer Completeness
CWGL_API cwgl_enum_t 
cwgl_checkFramebufferStatus(cwgl_ctx_t* ctx, cwgl_enum_t target){
    cwgl_enum_t e;
    cwgl_backend_configure_framebuffer(ctx, &e);
    return e;
}

