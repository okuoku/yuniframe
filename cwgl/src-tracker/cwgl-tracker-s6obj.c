#include <string.h>
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
        /* GLES2 Table 6.12 */
        CASE(RED_BITS);
        CASE(GREEN_BITS);
        CASE(BLUE_BITS);
        CASE(ALPHA_BITS);
        CASE(DEPTH_BITS);
        CASE(STENCIL_BITS);
        /* FIXME: Check framebuffer completion status
        CASE(IMPLEMENTATION_COLOR_READ_TYPE);
        CASE(IMPLEMENTATION_COLOR_READ_FORMAT);
        */
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
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Buffer(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                         cwgl_Buffer_t** buffer){
    switch(pname){
        case ARRAY_BUFFER_BINDING:
            *buffer = ctx->state.bin.ARRAY_BUFFER_BINDING;
            return CWGL_QR_SUCCESS;
        case ELEMENT_ARRAY_BUFFER_BINDING:
            *buffer = ctx->state.bin.ELEMENT_ARRAY_BUFFER_BINDING;
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
    switch(pname){
        case TEXTURE_BINDING_2D:
            *texture = ctx->state.bin.TEXTURE_BINDING_2D;
            return CWGL_QR_SUCCESS;
        case TEXTURE_BINDING_CUBE_MAP:
            *texture = ctx->state.bin.TEXTURE_BINDING_CUBE_MAP;
            return CWGL_QR_SUCCESS;
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return CWGL_QR_GLERROR;
    }
}

CWGL_API int 
cwgl_isEnabled(cwgl_ctx_t* ctx, cwgl_enum_t cap){
    // FIXME: Should not accept non enable() disable() capable enums
    int32_t r;
    cwgl_query_result_t qr;
    qr = cwgl_getParameter_b1(ctx, cap, &r);
    if(qr == CWGL_QR_SUCCESS){
        if(r){
            return 1;
        }else{
            return 0;
        }
    }else{
        return 0;
    }
}

// 6.1.3 Enumerated Queries
CWGL_API cwgl_query_result_t 
cwgl_getTexParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t pname,
                        int32_t* x){
}

CWGL_API cwgl_query_result_t 
cwgl_getBufferParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                           cwgl_enum_t pname, int32_t* x){
}

CWGL_API cwgl_query_result_t 
cwgl_getFramebufferAttachmentParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target,
                                          cwgl_enum_t attachment, 
                                          cwgl_enum_t pname, int32_t* x){
}

CWGL_API cwgl_query_result_t 
cwgl_getFramebufferAttachmentParameter_Renderbuffer(cwgl_ctx_t* ctx, 
                                                    cwgl_enum_t target, 
                                                    cwgl_enum_t attachment, 
                                                    cwgl_enum_t pname, 
                                                    cwgl_Renderbuffer_t** renderbuffer){

}

CWGL_API cwgl_query_result_t 
cwgl_getFramebufferAttachmentParameter_Texture(cwgl_ctx_t* ctx, 
                                               cwgl_enum_t target, 
                                               cwgl_enum_t attachment, 
                                               cwgl_enum_t pname, 
                                               cwgl_Texture_t** texture){
}

CWGL_API cwgl_query_result_t 
cwgl_getRenderbufferParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                                 cwgl_enum_t pname, int32_t* x){
}

// 6.1.4 Texture Queries
CWGL_API int 
cwgl_isTexture(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
}

// 6.1.6 Buffer Object Queries
CWGL_API int 
cwgl_isBuffer(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer){
}

// 6.1.7 Framebuffer Object and Renderbuffer Queries
CWGL_API int 
cwgl_isFramebuffer(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer){
}

CWGL_API int 
cwgl_isRenderbuffer(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer){
}
