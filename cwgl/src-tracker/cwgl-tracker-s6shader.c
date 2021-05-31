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
    *vertex_shader = program->state.vertex_shader;
    *fragment_shader = program->state.fragment_shader;
    return CWGL_QR_SUCCESS;
}

CWGL_API cwgl_string_t* 
cwgl_getProgramInfoLog(cwgl_ctx_t* ctx, cwgl_Program_t* program){
}

CWGL_API cwgl_string_t* 
cwgl_getShaderInfoLog(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
}

CWGL_API cwgl_string_t* 
cwgl_getShaderSource(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    if(shader->state.source){
        return cwgl_priv_string_dup(ctx, shader->state.source);
    }else{
        return NULL;
    }
}

CWGL_API cwgl_query_result_t 
cwgl_getShaderPrecisionFormat(cwgl_ctx_t* ctx, cwgl_enum_t shadertype, 
                              cwgl_enum_t precisiontype, 
                              int32_t* rangeMin, int32_t* rangeMax, 
                              int32_t* precision){
    cwgl_ctx_config_shaderprecisiontype_t* t;
    cwgl_ctx_config_shaderprecisionformat_t* p;
    switch(shadertype){
        case VERTEX_SHADER:
            t = &ctx->state.cfg.shader_precision_format.VERTEX_SHADER;
            break;
        case FRAGMENT_SHADER:
            t = &ctx->state.cfg.shader_precision_format.FRAGMENT_SHADER;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return CWGL_QR_GLERROR;
    }

    switch(precisiontype){
        case LOW_FLOAT:
            p = &t->LOW_FLOAT;
            break;
        case MEDIUM_FLOAT:
            p = &t->MEDIUM_FLOAT;
            break;
        case HIGH_FLOAT:
            p = &t->HIGH_FLOAT;
            break;
        case LOW_INT:
            p = &t->LOW_INT;
            break;
        case MEDIUM_INT:
            p = &t->MEDIUM_INT;
            break;
        case HIGH_INT:
            p = &t->HIGH_INT;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return CWGL_QR_GLERROR;
    }

    *rangeMin = p->rangeMin;
    *rangeMax = p->rangeMax;
    *precision = p->precision;

    return CWGL_QR_SUCCESS;
}

// FIXME: Copied from s6obj
static cwgl_VertexArrayObject_t*
current_vao(cwgl_ctx_t* ctx){
    if(ctx->state.bin.VERTEX_ARRAY_BINDING){
        return ctx->state.bin.VERTEX_ARRAY_BINDING;
    }else{
        return ctx->state.default_vao;
    }
}

CWGL_API cwgl_query_result_t 
cwgl_getVertexAttrib_i1(cwgl_ctx_t* ctx, uint32_t index, 
                        cwgl_enum_t pname, int32_t* x){
    cwgl_VertexArrayObject_t* vao;
    vao = current_vao(ctx);

    if(index >= CWGL_MAX_VAO_SIZE){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return CWGL_QR_GLERROR;
    }

    switch(pname){
        case VERTEX_ATTRIB_ARRAY_SIZE:
            *x = vao->attrib[index].VERTEX_ATTRIB_ARRAY_SIZE;
            break;
        case VERTEX_ATTRIB_ARRAY_STRIDE:
            *x = vao->attrib[index].VERTEX_ATTRIB_ARRAY_STRIDE;
            break;
        case VERTEX_ATTRIB_ARRAY_TYPE:
            *x = vao->attrib[index].VERTEX_ATTRIB_ARRAY_TYPE;
            break;
        case VERTEX_ATTRIB_ARRAY_NORMALIZED:
            *x = vao->attrib[index].VERTEX_ATTRIB_ARRAY_NORMALIZED;
            break;
        case VERTEX_ATTRIB_ARRAY_ENABLED:
            *x = vao->attrib[index].VERTEX_ATTRIB_ARRAY_ENABLED;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return CWGL_QR_GLERROR;
    }
    return CWGL_QR_SUCCESS;
}

CWGL_API cwgl_query_result_t 
cwgl_getVertexAttrib_f4(cwgl_ctx_t* ctx, uint32_t index, 
                        cwgl_enum_t pname, 
                        float* x, float* y, float* z, float* w){
    cwgl_VertexArrayObject_t* vao;
    vao = current_vao(ctx);

    if(index >= CWGL_MAX_VAO_SIZE){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return CWGL_QR_GLERROR;
    }

    switch(pname){
        case CURRENT_VERTEX_ATTRIB:
            *x = vao->attrib[index].CURRENT_VERTEX_ATTRIB[0];
            *y = vao->attrib[index].CURRENT_VERTEX_ATTRIB[1];
            *z = vao->attrib[index].CURRENT_VERTEX_ATTRIB[2];
            *w = vao->attrib[index].CURRENT_VERTEX_ATTRIB[3];
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return CWGL_QR_GLERROR;
    }
    return CWGL_QR_SUCCESS;
}

CWGL_API cwgl_query_result_t 
cwgl_getVertexAttrib_Buffer(cwgl_ctx_t* ctx, uint32_t index, 
                            cwgl_enum_t pname, cwgl_Buffer_t** buffer){
    cwgl_VertexArrayObject_t* vao;
    vao = current_vao(ctx);

    if(index >= CWGL_MAX_VAO_SIZE){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return CWGL_QR_GLERROR;
    }

    switch(pname){
        case VERTEX_ATTRIB_ARRAY_BUFFER_BINDING:
            *buffer = vao->attrib[index].VERTEX_ATTRIB_ARRAY_BUFFER_BINDING;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return CWGL_QR_GLERROR;
    }
    return CWGL_QR_SUCCESS;

}

CWGL_API uint32_t 
cwgl_getVertexAttribOffset(cwgl_ctx_t* ctx, uint32_t index, cwgl_enum_t pname){
        cwgl_VertexArrayObject_t* vao;
    vao = current_vao(ctx);

    if(index >= CWGL_MAX_VAO_SIZE){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return 0;
    }

    switch(pname){
        case VERTEX_ATTRIB_ARRAY_POINTER:
            return vao->attrib[index].VERTEX_ATTRIB_ARRAY_POINTER;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return 0;
    }
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


