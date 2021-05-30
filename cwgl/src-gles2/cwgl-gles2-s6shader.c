#include "cwgl-gles2-priv.h"

// 6.1.8 Shader and Program Queries
CWGL_API int 
cwgl_isShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    int r;
    GLuint name;
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, shader);
    r = glIsShader(name);
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getShaderParameter_i1(cwgl_ctx_t* ctx, cwgl_Shader_t* shader, 
                           cwgl_enum_t pname, int32_t* x){
    cwgl_query_result_t r;
    GLint i;
    GLuint name;
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, shader);
    glGetShaderiv(name, pname, &i);
    *x = i;
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API int 
cwgl_isProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    int r;
    GLuint name;
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, program);
    r = glIsProgram(name);
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getProgramParameter_i1(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                            cwgl_enum_t pname, int32_t* x){
    cwgl_query_result_t r;
    GLint i;
    GLuint name;
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, program);
    glGetProgramiv(name, pname, &i);
    *x = i;
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getAttachedShaders_cwgl(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                             cwgl_Shader_t** vertex_shader, 
                             cwgl_Shader_t** fragment_shader){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_string_t* 
cwgl_getProgramInfoLog(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    cwgl_string_t* r;
    GLuint name;
    GLsizei len = 0;
    GLsizei maxlen = (512*1024);
    void* buf = malloc(maxlen);
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, program);
    glGetProgramInfoLog(name, maxlen, &len, buf);
    CTX_LEAVE(ctx);
    if(len == 0){
        free(buf);
        return NULL;
    }else{
        r = cwgl_priv_alloc_string(ctx, buf, len + 1);
        free(buf);
        return r;
    }
}

CWGL_API cwgl_string_t* 
cwgl_getShaderInfoLog(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    cwgl_string_t* r;
    GLuint name;
    GLsizei len = 0;
    GLsizei maxlen = (512*1024);
    void* buf = malloc(maxlen);
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, shader);
    glGetShaderInfoLog(name, maxlen, &len, buf);
    CTX_LEAVE(ctx);
    if(len == 0){
        free(buf);
        return NULL;
    }else{
        r = cwgl_priv_alloc_string(ctx, buf, len + 1);
        free(buf);
        return r;
    }
}

CWGL_API cwgl_string_t* 
cwgl_getShaderSource(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    // FIXME: Implement this
    return NULL;
}

CWGL_API cwgl_query_result_t 
cwgl_getShaderPrecisionFormat(cwgl_ctx_t* ctx, cwgl_enum_t shadertype, 
                              cwgl_enum_t precisiontype, 
                              int32_t* rangeMin, int32_t* rangeMax, 
                              int32_t* precision){
    cwgl_query_result_t r;
    GLint range[2];
    GLint prec;
    CTX_ENTER(ctx);
    glGetShaderPrecisionFormat(shadertype, precisiontype, range, &prec);
    r = CWGL_QR_SUCCESS;
    CTX_LEAVE(ctx);
    *rangeMin = range[0];
    *rangeMax = range[1];
    *precision = prec;
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getVertexAttrib_i1(cwgl_ctx_t* ctx, uint32_t index, 
                        cwgl_enum_t pname, int32_t* x){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getVertexAttrib_f4(cwgl_ctx_t* ctx, uint32_t index, 
                        cwgl_enum_t pname, 
                        float* x, float* y, float* z, float* w){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getVertexAttrib_Buffer(cwgl_ctx_t* ctx, uint32_t index, 
                            cwgl_enum_t pname, cwgl_Buffer_t** buffer){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API uint32_t 
cwgl_getVertexAttribOffset(cwgl_ctx_t* ctx, uint32_t index, cwgl_enum_t pname){
    void* rr = NULL;
    uint32_t r;
    CTX_ENTER(ctx);
    glGetVertexAttribPointerv(index, pname, &rr);
    r = (uint32_t)(uintptr_t)rr;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_i1(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, int32_t* x){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_i2(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, int32_t* x, int32_t* y){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_i3(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, 
                   int32_t* x, int32_t* y, int32_t* z){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_i4(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, 
                   int32_t* x, int32_t* y, int32_t* z, int32_t* w){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_f1(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, float* x){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_f2(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, float* x, float* y){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_f3(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, 
                   float* x, float* y, float* z){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_f4(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, 
                   float* x, float* y, float* z, float* w){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_m2(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, float* x){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_m3(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, float* x){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_m4(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, float* x){
    cwgl_query_result_t r;
    CTX_ENTER(ctx);
    r = CWGL_QR_UNIMPLEMENTED;
    CTX_LEAVE(ctx);
    return r;
}


