#include "cwgl-tracker-priv.h"

// 6.1.8 Shader and Program Queries
CWGL_API int 
cwgl_isShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    // FIXME: Check context lost flag
    return 1;
}

CWGL_API cwgl_query_result_t 
cwgl_getShaderParameter_i1(cwgl_ctx_t* ctx, cwgl_Shader_t* shader, 
                           cwgl_enum_t pname, int32_t* x){
}

CWGL_API int 
cwgl_isProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    // FIXME: Check context lost flag
    return 1;
}

CWGL_API cwgl_query_result_t 
cwgl_getProgramParameter_i1(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                            cwgl_enum_t pname, int32_t* x){
}

CWGL_API cwgl_query_result_t 
cwgl_getAttachedShaders(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                        cwgl_Shader_t** vertex_shader,
                        cwgl_Shader_t** fragment_shader){
}

CWGL_API cwgl_string_t* 
cwgl_getProgramInfoLog(cwgl_ctx_t* ctx, cwgl_Program_t* program){
}

CWGL_API cwgl_string_t* 
cwgl_getShaderInfoLog(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
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
}

CWGL_API cwgl_query_result_t 
cwgl_getVertexAttrib_i1(cwgl_ctx_t* ctx, uint32_t index, 
                        cwgl_enum_t pname, int32_t* x){
}

CWGL_API cwgl_query_result_t 
cwgl_getVertexAttrib_f4(cwgl_ctx_t* ctx, uint32_t index, 
                        cwgl_enum_t pname, 
                        float* x, float* y, float* z, float* w){
}

CWGL_API cwgl_query_result_t 
cwgl_getVertexAttrib_Buffer(cwgl_ctx_t* ctx, uint32_t index, 
                            cwgl_enum_t pname, cwgl_Buffer_t** buffer){
}

CWGL_API uint32_t 
cwgl_getVertexAttribOffset(cwgl_ctx_t* ctx, uint32_t index, cwgl_enum_t pname){
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_i1(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, int32_t* x){
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_i2(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, int32_t* x, int32_t* y){
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_i3(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, 
                   int32_t* x, int32_t* y, int32_t* z){
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_i4(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, 
                   int32_t* x, int32_t* y, int32_t* z, int32_t* w){
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_f1(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, float* x){
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_f2(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, float* x, float* y){
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_f3(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, 
                   float* x, float* y, float* z){
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_f4(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, 
                   float* x, float* y, float* z, float* w){
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_m2(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, float* x){
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_m3(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, float* x){
}

CWGL_API cwgl_query_result_t 
cwgl_getUniform_m4(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                   cwgl_UniformLocation_t* location, float* x){
}


