#include <string.h>
#include <stdio.h> // FIXME: remove snprintf dependency
#include "cwgl-tracker-priv.h"

#define _CASE1(slot, name) case name: *x = ctx->state.slot.name; \
                                           return CWGL_QR_SUCCESS

#define _CASE2(slot, name) case name: *x = ctx->state.slot.name[0]; \
                                           *y = ctx->state.slot.name[1]; \
                                           return CWGL_QR_SUCCESS

#define _CASE4(slot, name) case name: *x = ctx->state.slot.name[0]; \
                                           *y = ctx->state.slot.name[1]; \
                                           *z = ctx->state.slot.name[2]; \
                                           *w = ctx->state.slot.name[3]; \
                                           return CWGL_QR_SUCCESS

// 6.1.1 Simple Queries
CWGL_API cwgl_query_result_t 
cwgl_getParameter_b1(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t* x){
#define GLO(x) _CASE1(glo, x)
    switch(pname){
        /* GLES2 Table 6.5 */
        GLO(CULL_FACE);
        GLO(POLYGON_OFFSET_FILL);
        /* GLES2 Table 6.6 */
        GLO(SAMPLE_ALPHA_TO_COVERAGE);
        GLO(SAMPLE_COVERAGE);
        GLO(SAMPLE_COVERAGE_INVERT);
        /* GLES2 Table 6.10 */
        GLO(SCISSOR_TEST);
        GLO(STENCIL_TEST);
        GLO(DEPTH_TEST);
        /* GLES2 Table 6.11 */
        GLO(BLEND);
        GLO(DITHER);
        /* GLES2 Table 6.12 */
        GLO(DEPTH_WRITEMASK);
        /* WebGL1 */
        GLO(UNPACK_FLIP_Y_WEBGL);
        GLO(UNPACK_PREMULTIPLY_ALPHA_WEBGL);
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
#undef GLO
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_b4(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                     int32_t* x, int32_t* y, int32_t* z, int32_t* w){
#define GLO(x) _CASE4(glo, x)
    switch(pname){
        GLO(COLOR_WRITEMASK);
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
#undef GLO
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t* x){
#define FB(n) case n: \
    if(ctx->state.bin.FRAMEBUFFER_BINDING){\
        /* FIXME: Check framebuffer completion status */ \
        *x = (int32_t)ctx->state.bin.FRAMEBUFFER_BINDING->state.n; \
        return CWGL_QR_SUCCESS; \
    } else { \
        CTX_SET_ERROR(ctx, INVALID_OPERATION); \
        return CWGL_QR_GLERROR; \
    } break

    /* Handle framebuffer status */
    switch(pname){
        /* GLES2 Table 6.21 */
        FB(RED_BITS);
        FB(GREEN_BITS);
        FB(BLUE_BITS);
        FB(ALPHA_BITS);
        FB(DEPTH_BITS);
        FB(STENCIL_BITS);
        FB(IMPLEMENTATION_COLOR_READ_TYPE);
        FB(IMPLEMENTATION_COLOR_READ_FORMAT);
        default:
            break;
    }
#undef FB
#define CASE(x) _CASE1(cfg, x)
#define GLO(x) _CASE1(glo, x)
    switch(pname){
        /* GLES2 Table 6.18 */
        CASE(SUBPIXEL_BITS);
        CASE(MAX_TEXTURE_SIZE);
        CASE(MAX_CUBE_MAP_TEXTURE_SIZE);
        CASE(SAMPLE_BUFFERS);
        CASE(SAMPLES);
        /* GLES2 Table 6.20 */
        CASE(MAX_VERTEX_ATTRIBS);
        CASE(MAX_VERTEX_UNIFORM_VECTORS);
        CASE(MAX_VARYING_VECTORS);
        CASE(MAX_COMBINED_TEXTURE_IMAGE_UNITS);
        CASE(MAX_VERTEX_TEXTURE_IMAGE_UNITS);
        CASE(MAX_TEXTURE_IMAGE_UNITS);
        CASE(MAX_FRAGMENT_UNIFORM_VECTORS);
        CASE(MAX_RENDERBUFFER_SIZE);
        /* GLES2 Table 6.5 */
        GLO(CULL_FACE_MODE);
        GLO(FRONT_FACE);
        /* GLES2 Table 6.9 */
        GLO(ACTIVE_TEXTURE);
        /* GLES2 Table 6.10 */
        GLO(STENCIL_FUNC);
        GLO(STENCIL_VALUE_MASK);
        GLO(STENCIL_REF);
        GLO(STENCIL_FAIL);
        GLO(STENCIL_PASS_DEPTH_FAIL);
        GLO(STENCIL_PASS_DEPTH_PASS);
        GLO(STENCIL_BACK_FUNC);
        GLO(STENCIL_BACK_VALUE_MASK);
        GLO(STENCIL_BACK_REF);
        GLO(STENCIL_BACK_FAIL);
        GLO(STENCIL_BACK_PASS_DEPTH_FAIL);
        GLO(STENCIL_BACK_PASS_DEPTH_PASS);
        GLO(DEPTH_FUNC);
        /* GLES2 Table 6.11 */
        GLO(BLEND_SRC_RGB);
        GLO(BLEND_SRC_ALPHA);
        GLO(BLEND_DST_RGB);
        GLO(BLEND_DST_ALPHA);
        GLO(BLEND_EQUATION_RGB);
        GLO(BLEND_EQUATION_ALPHA);
        /* GLES2 Table 6.12 */
        GLO(STENCIL_WRITEMASK);
        GLO(STENCIL_BACK_WRITEMASK);
        GLO(STENCIL_CLEAR_VALUE);
        /* GLES2 Table 6.13 */
        GLO(UNPACK_ALIGNMENT);
        GLO(PACK_ALIGNMENT);
        /* GLES2 Table 6.17 */
        GLO(GENERATE_MIPMAP_HINT);
        /* WebGL1 */
        GLO(UNPACK_COLORSPACE_CONVERSION_WEBGL);
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
#undef CASE
#undef GLO
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_i2(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                     int32_t* x, int32_t* y){
#define CASE(x) _CASE2(cfg, x)
    switch(pname){
        CASE(MAX_VIEWPORT_DIMS);
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
#undef CASE
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_i4(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                     int32_t* x, int32_t* y, int32_t* z, int32_t* w){
#define GLO(x) _CASE2(glo, x)
    switch(pname){
        GLO(VIEWPORT);
        GLO(SCISSOR_BOX);
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
#undef GLO
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_f1(cwgl_ctx_t* ctx, cwgl_enum_t pname, float* x){
#define GLO(x) _CASE1(glo, x)
    switch(pname){
        GLO(LINE_WIDTH);
        GLO(POLYGON_OFFSET_FACTOR);
        GLO(POLYGON_OFFSET_UNITS);
        GLO(SAMPLE_COVERAGE_VALUE);
        GLO(DEPTH_CLEAR_VALUE);
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
#undef GLO
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_f2(cwgl_ctx_t* ctx, cwgl_enum_t pname, float* x, float* y){
#define CASE(x) _CASE2(cfg, x)
#define GLO(x) _CASE2(glo, x)
    switch(pname){
        CASE(ALIASED_POINT_SIZE_RANGE);
        CASE(ALIASED_LINE_WIDTH_RANGE);
        GLO(DEPTH_RANGE);
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
#undef CASE
#undef GLO
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_f4(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                     float* x, float* y, float* z, float* w){
#define GLO(x) _CASE4(glo, x)
    switch(pname){
        GLO(BLEND_COLOR);
        GLO(COLOR_CLEAR_VALUE);
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
#undef GLO
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_str(cwgl_ctx_t* ctx, cwgl_enum_t pname, cwgl_string_t** str){
#define TMPBUFLEN 512
    int r;
    cwgl_string_t* out;
    char buf[TMPBUFLEN];
    switch(pname){
        case RENDERER:
            r = snprintf(buf, TMPBUFLEN, "(cwgl) %s", ctx->state.cfg.RENDERER);
            break;
        case SHADING_LANGUAGE_VERSION:
            r = snprintf(buf, TMPBUFLEN, "%s", 
                         ctx->state.cfg.SHADING_LANGUAGE_VERSION);
            break;
        case VENDOR:
            r = snprintf(buf, TMPBUFLEN, "(cwgl) %s", ctx->state.cfg.VENDOR);
            break;
        case VERSION:
            r = snprintf(buf, TMPBUFLEN, "%s", ctx->state.cfg.VERSION);
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return CWGL_QR_GLERROR;
    }

    out = cwgl_priv_alloc_string(ctx, buf, r);
    *str = out;
    return CWGL_QR_SUCCESS;
}

static cwgl_VertexArrayObject_t*
current_vao(cwgl_ctx_t* ctx){
    if(ctx->state.bin.VERTEX_ARRAY_BINDING){
        return ctx->state.bin.VERTEX_ARRAY_BINDING;
    }else{
        return ctx->state.default_vao;
    }
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Buffer(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                         cwgl_Buffer_t** buffer){
    cwgl_VertexArrayObject_t* vao;
    switch(pname){
        case ARRAY_BUFFER_BINDING:
            *buffer = ctx->state.bin.ARRAY_BUFFER_BINDING;
            return CWGL_QR_SUCCESS;
        case ELEMENT_ARRAY_BUFFER_BINDING:
            vao = current_vao(ctx);
            *buffer = vao->state.ELEMENT_ARRAY_BUFFER_BINDING;
            return CWGL_QR_SUCCESS;
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Program(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                          cwgl_Program_t** program){
    switch(pname){
        case CURRENT_PROGRAM:
            *program = ctx->state.bin.CURRENT_PROGRAM;
            return CWGL_QR_SUCCESS;
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Framebuffer(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                              cwgl_Framebuffer_t** framebuffer){
    switch(pname){
        case FRAMEBUFFER_BINDING:
            *framebuffer = ctx->state.bin.FRAMEBUFFER_BINDING;
            return CWGL_QR_SUCCESS;
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Renderbuffer(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                               cwgl_Renderbuffer_t** renderbuffer){
    switch(pname){
        case RENDERBUFFER_BINDING:
            *renderbuffer = ctx->state.bin.RENDERBUFFER_BINDING;
            return CWGL_QR_SUCCESS;
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Texture(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                          cwgl_Texture_t** texture){
    int id = (ctx->state.glo.ACTIVE_TEXTURE - TEXTURE0);
    if(id < 0){
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return CWGL_QR_GLERROR;
    }
    if(id >= CWGL_MAX_TEXTURE_UNITS){
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return CWGL_QR_GLERROR;
    }
    switch(pname){
        case TEXTURE_BINDING_2D:
            *texture = ctx->state.bin.texture_unit[id].TEXTURE_BINDING_2D;
            return CWGL_QR_SUCCESS;
        case TEXTURE_BINDING_CUBE_MAP:
            *texture = ctx->state.bin.texture_unit[id].TEXTURE_BINDING_CUBE_MAP;
            return CWGL_QR_SUCCESS;
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
}

CWGL_API int 
cwgl_isEnabled(cwgl_ctx_t* ctx, cwgl_enum_t cap){
    int r = 0;
    switch(cap){
        case BLEND:
            r = ctx->state.glo.BLEND ? 1 : 0;
            break;
        case CULL_FACE:
            r = ctx->state.glo.CULL_FACE ? 1 : 0;
            break;
        case DEPTH_TEST:
            r = ctx->state.glo.DEPTH_TEST ? 1 : 0;
            break;
        case DITHER:
            r = ctx->state.glo.DITHER ? 1 : 0;
            break;
        case POLYGON_OFFSET_FILL:
            r = ctx->state.glo.POLYGON_OFFSET_FILL ? 1 : 0;
            break;
        case SAMPLE_ALPHA_TO_COVERAGE:
            r = ctx->state.glo.SAMPLE_ALPHA_TO_COVERAGE ? 1 : 0;
            break;
        case SAMPLE_COVERAGE:
            r = ctx->state.glo.SAMPLE_COVERAGE ? 1 : 0;
            break;
        case SCISSOR_TEST:
            r = ctx->state.glo.SCISSOR_TEST ? 1 : 0;
            break;
        case STENCIL_TEST:
            r = ctx->state.glo.STENCIL_TEST ? 1 : 0;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            break;
    }
    return r;
}

// 6.1.3 Enumerated Queries
// FIXME: Copied from s3
static cwgl_texture_unit_state_t*
current_texture_unit(cwgl_ctx_t* ctx){
    int id = (ctx->state.glo.ACTIVE_TEXTURE - TEXTURE0);
    if(id < 0){
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return NULL;
    }
    if(id >= CWGL_MAX_TEXTURE_UNITS){
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return NULL;
    }
    return &ctx->state.bin.texture_unit[id];
}

CWGL_API cwgl_query_result_t 
cwgl_getTexParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t pname,
                        int32_t* x){
    cwgl_texture_unit_state_t* current;
    cwgl_Texture_t* tex;

    current = current_texture_unit(ctx);
    if(! current){
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }

    switch(target){
        case TEXTURE_2D:
            tex = current->TEXTURE_BINDING_2D;
            break;
        case TEXTURE_CUBE_MAP:
            tex = current->TEXTURE_BINDING_CUBE_MAP;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return CWGL_QR_GLERROR;
    }
    if(! tex){
        // WebGL specific: No default texture
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return CWGL_QR_GLERROR;
    }

    switch(pname){
        case TEXTURE_MAG_FILTER:
            *x = tex->state.TEXTURE_MAG_FILTER;
            break;
        case TEXTURE_MIN_FILTER:
            *x = tex->state.TEXTURE_MIN_FILTER;
            break;
        case TEXTURE_WRAP_S:
            *x = tex->state.TEXTURE_WRAP_S;
            break;
        case TEXTURE_WRAP_T:
            *x = tex->state.TEXTURE_WRAP_T;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return CWGL_QR_GLERROR;
    }
    return CWGL_QR_SUCCESS;
}

CWGL_API cwgl_query_result_t 
cwgl_getBufferParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                           cwgl_enum_t pname, int32_t* x){
    cwgl_Buffer_t* point;
    cwgl_VertexArrayObject_t* vao;
    switch(target){
        case ARRAY_BUFFER:
            point = ctx->state.bin.ARRAY_BUFFER_BINDING;
            break;
        case ELEMENT_ARRAY_BUFFER:
            vao = current_vao(ctx);
            point = vao->state.ELEMENT_ARRAY_BUFFER_BINDING;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return CWGL_QR_GLERROR;
    }

    if(! point){
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return CWGL_QR_GLERROR;
    }
    switch(pname){
        case BUFFER_SIZE:
            *x = point->state.BUFFER_SIZE;
            break;
        case BUFFER_USAGE:
            *x = point->state.BUFFER_USAGE;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return CWGL_QR_GLERROR;
    }
    return CWGL_QR_SUCCESS;
}

CWGL_API cwgl_query_result_t 
cwgl_getFramebufferAttachmentParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target,
                                          cwgl_enum_t attachment, 
                                          cwgl_enum_t pname, int32_t* x){
    cwgl_Framebuffer_t* fb;
    cwgl_framebuffer_attachment_state_t* point;

    if(target != FRAMEBUFFER){
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
    fb = ctx->state.bin.FRAMEBUFFER_BINDING;
    if(! fb){
        /* WebGL: No default framebuffer */
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return CWGL_QR_GLERROR;
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
            return CWGL_QR_GLERROR;
    }

    switch(pname){
        case FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
            *x = point->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE;
            break;
        case FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL:
            if(point->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE == TEXTURE){
                *x = point->FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL;
            }else{
                CTX_SET_ERROR(ctx, INVALID_ENUM);
                return CWGL_QR_GLERROR;
            }
            break;
        case FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE:
            if(point->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE == TEXTURE){
                *x = point->FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE;
            }else{
                CTX_SET_ERROR(ctx, INVALID_ENUM);
                return CWGL_QR_GLERROR;
            }
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return CWGL_QR_GLERROR;
    }
    return CWGL_QR_SUCCESS;
}

CWGL_API cwgl_query_result_t 
cwgl_getFramebufferAttachmentParameter_Renderbuffer(cwgl_ctx_t* ctx, 
                                                    cwgl_enum_t target, 
                                                    cwgl_enum_t attachment, 
                                                    cwgl_enum_t pname, 
                                                    cwgl_Renderbuffer_t** renderbuffer){
    cwgl_Framebuffer_t* fb;
    cwgl_framebuffer_attachment_state_t* point;

    if(target != FRAMEBUFFER){
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
    fb = ctx->state.bin.FRAMEBUFFER_BINDING;
    if(! fb){
        /* WebGL: No default framebuffer */
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return CWGL_QR_GLERROR;
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
            return CWGL_QR_GLERROR;
    }

    if(point->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE == RENDERBUFFER){
        *renderbuffer = point->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asRenderbuffer;
    }else{
        return CWGL_QR_TYPE_FRAMEBUFFER;
    }
    return CWGL_QR_SUCCESS;
}

CWGL_API cwgl_query_result_t 
cwgl_getFramebufferAttachmentParameter_Texture(cwgl_ctx_t* ctx, 
                                               cwgl_enum_t target, 
                                               cwgl_enum_t attachment, 
                                               cwgl_enum_t pname, 
                                               cwgl_Texture_t** texture){
    cwgl_Framebuffer_t* fb;
    cwgl_framebuffer_attachment_state_t* point;

    if(target != FRAMEBUFFER){
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
    fb = ctx->state.bin.FRAMEBUFFER_BINDING;
    if(! fb){
        /* WebGL: No default framebuffer */
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return CWGL_QR_GLERROR;
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
            return CWGL_QR_GLERROR;
    }

    if(point->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE == TEXTURE){
        *texture = point->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asTexture;
    }else{
        return CWGL_QR_TYPE_RENDERBUFFER;
    }
    return CWGL_QR_SUCCESS;
}

CWGL_API cwgl_query_result_t 
cwgl_getRenderbufferParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                                 cwgl_enum_t pname, int32_t* x){
    cwgl_Renderbuffer_t* rb;
    if(target != RENDERBUFFER){
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }

    rb = ctx->state.bin.RENDERBUFFER_BINDING;
    if(! rb){
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return CWGL_QR_GLERROR;
    }

    switch(pname){
        case RENDERBUFFER_WIDTH:
            *x = rb->state.RENDERBUFFER_WIDTH;
            break;
        case RENDERBUFFER_HEIGHT:
            *x = rb->state.RENDERBUFFER_HEIGHT;
            break;
        case RENDERBUFFER_INTERNAL_FORMAT:
            *x = rb->state.RENDERBUFFER_INTERNAL_FORMAT;
            break;
        case RENDERBUFFER_RED_SIZE:
            *x = rb->state.RENDERBUFFER_RED_SIZE;
            break;
        case RENDERBUFFER_GREEN_SIZE:
            *x = rb->state.RENDERBUFFER_GREEN_SIZE;
            break;
        case RENDERBUFFER_BLUE_SIZE:
            *x = rb->state.RENDERBUFFER_BLUE_SIZE;
            break;
        case RENDERBUFFER_ALPHA_SIZE:
            *x = rb->state.RENDERBUFFER_ALPHA_SIZE;
            break;
        case RENDERBUFFER_DEPTH_SIZE:
            *x = rb->state.RENDERBUFFER_DEPTH_SIZE;
            break;
        case RENDERBUFFER_STENCIL_SIZE:
            *x = rb->state.RENDERBUFFER_STENCIL_SIZE;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return CWGL_QR_GLERROR;
    }
    return CWGL_QR_SUCCESS;
}

// 6.1.4 Texture Queries
CWGL_API int 
cwgl_isTexture(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
    // FIXME: Check context lost flag
    return 1;
}

// 6.1.6 Buffer Object Queries
CWGL_API int 
cwgl_isBuffer(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer){
    // FIXME: Check context lost flag
    return 1;
}

// 6.1.7 Framebuffer Object and Renderbuffer Queries
CWGL_API int 
cwgl_isFramebuffer(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer){
    // FIXME: Check context lost flag
    return 1;
}

CWGL_API int 
cwgl_isRenderbuffer(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer){
    // FIXME: Check context lost flag
    return 1;
}
