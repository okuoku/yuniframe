#include <string.h>
#include "cwgl-tracker-priv.h"

// 6.1.1 Simple Queries
CWGL_API cwgl_query_result_t 
cwgl_getParameter_b1(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t* x){
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_b4(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                     int32_t* x, int32_t* y, int32_t* z, int32_t* w){
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_i1(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t* x){
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_i2(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                     int32_t* x, int32_t* y){
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_i4(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                     int32_t* x, int32_t* y, int32_t* z, int32_t* w){
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_f1(cwgl_ctx_t* ctx, cwgl_enum_t pname, float* x){
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_f2(cwgl_ctx_t* ctx, cwgl_enum_t pname, float* x, float* y){
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_f4(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                     float* x, float* y, float* z, float* w){
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_str(cwgl_ctx_t* ctx, cwgl_enum_t pname, cwgl_string_t** str){
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Buffer(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                         cwgl_Buffer_t** buffer){
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Program(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                          cwgl_Program_t** program){
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Framebuffer(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                              cwgl_Framebuffer_t** framebuffer){
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Renderbuffer(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                               cwgl_Renderbuffer_t** renderbuffer){
}

CWGL_API cwgl_query_result_t 
cwgl_getParameter_Texture(cwgl_ctx_t* ctx, cwgl_enum_t pname, 
                          cwgl_Texture_t** texture){
}

CWGL_API int 
cwgl_isEnabled(cwgl_ctx_t* ctx, cwgl_enum_t cap){
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
