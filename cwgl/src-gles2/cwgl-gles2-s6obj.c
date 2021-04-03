#include <string.h>
#include "cwgl-gles2-priv.h"

// 6.1.1 Simple Queries
CWGL_API cwgl_query_result_t 
cwgl_getParameter_b1(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t* x){
    GLboolean b4[4] = { 0 };
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    glGetBooleanv(pname, b4);
    *x = b4[0];
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_b4(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                     int32_t* x, int32_t* y, int32_t* z, int32_t* w){
    GLboolean b4[4] = { 0 };
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    glGetBooleanv(pname, b4);
    *x = b4[0];
    *y = b4[1];
    *z = b4[2];
    *w = b4[3];
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t* x){
    GLint i4[4] = { 0 };
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    glGetIntegerv(pname, i4);
    *x = i4[0];
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_i2(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                     int32_t* x, int32_t* y){
    GLint i4[4] = { 0 };
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    glGetIntegerv(pname, i4);
    *x = i4[0];
    *y = i4[1];
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_i4(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                     int32_t* x, int32_t* y, int32_t* z, int32_t* w){
    GLint i4[4] = { 0 };
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    glGetIntegerv(pname, i4);
    *x = i4[0];
    *y = i4[1];
    *z = i4[2];
    *w = i4[3];
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_f1(cwgl_ctx_t* ctx, cwgl_enum_t pname, float* x){
    cwgl_query_result_t r;
    float f4[4] = {0.0,0.0,0.0,0.0};
    CTX_ENTER(ctx);
    glGetFloatv(pname, f4);
    *x = f4[0];
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_f2(cwgl_ctx_t* ctx, cwgl_enum_t pname, float* x, float* y){
    cwgl_query_result_t r;
    float f4[4] = {0.0,0.0,0.0,0.0};
    CTX_ENTER(ctx);
    glGetFloatv(pname, f4);
    *x = f4[0];
    *y = f4[1];
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_f4(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                     float* x, float* y, float* z, float* w){
    cwgl_query_result_t r;
    float f4[4] = {0.0,0.0,0.0,0.0};
    CTX_ENTER(ctx);
    glGetFloatv(pname, f4);
    *x = f4[0];
    *y = f4[1];
    *z = f4[2];
    *w = f4[3];
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_str(cwgl_ctx_t* ctx, cwgl_enum_t pname, cwgl_string_t** str){
    const char* res;
    size_t xlen;
    cwgl_string_t* out;
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    res = glGetString(pname);
    if(!res){
        r = CWGL_QR_GLERROR;
    }else{
        xlen = strlen(res);
        out = cwgl_priv_alloc_string(ctx, res, xlen + 1);
        *str = out;
        r = CWGL_QR_SUCCESS;
    }
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Buffer(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                         cwgl_Buffer_t** buffer){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Program(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                          cwgl_Program_t** program){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Framebuffer(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                              cwgl_Framebuffer_t** framebuffer){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Renderbuffer(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                               cwgl_Renderbuffer_t** renderbuffer){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Texture(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                          cwgl_Texture_t** texture){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API int 
cwgl_isEnabled(cwgl_ctx_t* ctx, cwgl_enum_t cap){
    int r;
    CTX_ENTER(ctx);
    r = glIsEnabled(cap);
    CTX_LEAVE(ctx);
    return r;
}

// 6.1.3 Enumerated Queries
CWGL_API cwgl_query_result_t 
cwgl_getTexParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t pname,
                        int32_t* x){
    GLint i4[4] = { 0 };
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    glGetTexParameteriv(target, pname, i4);
    *x = i4[0];
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getBufferParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                           cwgl_enum_t pname, int32_t* x){
    GLint i4[4] = { 0 };
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    glGetBufferParameteriv(target, pname, i4);
    *x = i4[0];
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getFramebufferAttachmentParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target,
                                          cwgl_enum_t attachment, 
                                          cwgl_enum_t pname, int32_t* x){
    GLint i4[4] = { 0 };
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    glGetFramebufferAttachmentParameteriv(target, attachment, pname, i4);
    *x = i4[0];
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getFramebufferAttachmentParameter_Renderbuffer(cwgl_ctx_t* ctx, 
                                                    cwgl_enum_t target, 
                                                    cwgl_enum_t attachment, 
                                                    cwgl_enum_t pname, 
                                                    cwgl_Renderbuffer_t** renderbuffer){

    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getFramebufferAttachmentParameter_Texture(cwgl_ctx_t* ctx, 
                                               cwgl_enum_t target, 
                                               cwgl_enum_t attachment, 
                                               cwgl_enum_t pname, 
                                               cwgl_Texture_t** texture){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getRenderbufferParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                                 cwgl_enum_t pname, int32_t* x){
    GLint i4[4] = { 0 };
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    glGetRenderbufferParameteriv(target, pname, i4);
    *x = i4[0];
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    return r;
}

// 6.1.4 Texture Queries
CWGL_API int 
cwgl_isTexture(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
    int r;
    GLuint name;
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, texture);
    r = glIsTexture(name);
    CTX_LEAVE(ctx);
    return r;
}

// 6.1.6 Buffer Object Queries
CWGL_API int 
cwgl_isBuffer(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer){
    int r;
    GLuint name;
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, buffer);
    r = glIsBuffer(name);
    CTX_LEAVE(ctx);
    return r;
}

// 6.1.7 Framebuffer Object and Renderbuffer Queries
CWGL_API int 
cwgl_isFramebuffer(cwgl_ctx_t* ctx, cwgl_Framebuffer_t* framebuffer){
    int r;
    GLuint name;
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, framebuffer);
    r = glIsFramebuffer(name);
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API int 
cwgl_isRenderbuffer(cwgl_ctx_t* ctx, cwgl_Renderbuffer_t* renderbuffer){
    int r;
    GLuint name;
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, renderbuffer);
    r = glIsRenderbuffer(name);
    CTX_LEAVE(ctx);
    return r;
}
