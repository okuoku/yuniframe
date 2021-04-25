#include "cwgl-tracker-priv.h"
#include <stdlib.h>

/* Current VAO for attrib ops */
static cwgl_VertexArrayObject_t*
current_vao(cwgl_ctx_t* ctx){
    if(ctx->state.bin.VERTEX_ARRAY_BINDING){
        return ctx->state.bin.VERTEX_ARRAY_BINDING;
    }else{
        return ctx->state.default_vao;
    }
}


// 2.5 GL Errors
CWGL_API cwgl_enum_t 
cwgl_getError(cwgl_ctx_t* ctx){
    cwgl_enum_t current_error;
    current_error = ctx->state.err;
    CTX_SET_ERROR(ctx, NO_ERROR);
    return current_error;
}

// 2.7 Current Vertex State
static int
valid_attrib_index_p(uint32_t i){
    // FIXME: Should also check against MAX_VERTEX_ATTRIBS
    if(i < CWGL_MAX_VAO_SIZE){
        return 1;
    }
    return 0;
}

CWGL_API void 
cwgl_vertexAttrib1f(cwgl_ctx_t* ctx, uint32_t indx, float x){
    cwgl_VertexArrayObject_t* vao;
    vao = current_vao(ctx);
    if(! valid_attrib_index_p(indx)){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return;
    }
    vao->attrib[indx].CURRENT_VERTEX_ATTRIB[0] = x;
}

CWGL_API void 
cwgl_vertexAttrib2f(cwgl_ctx_t* ctx, uint32_t indx, float x, float y){
    cwgl_VertexArrayObject_t* vao;
    vao = current_vao(ctx);
    if(! valid_attrib_index_p(indx)){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return;
    }
    vao->attrib[indx].CURRENT_VERTEX_ATTRIB[0] = x;
    vao->attrib[indx].CURRENT_VERTEX_ATTRIB[1] = y;
}

CWGL_API void 
cwgl_vertexAttrib3f(cwgl_ctx_t* ctx, uint32_t indx, float x, float y, float z){
    cwgl_VertexArrayObject_t* vao;
    vao = current_vao(ctx);
    if(! valid_attrib_index_p(indx)){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return;
    }
    vao->attrib[indx].CURRENT_VERTEX_ATTRIB[0] = x;
    vao->attrib[indx].CURRENT_VERTEX_ATTRIB[1] = y;
    vao->attrib[indx].CURRENT_VERTEX_ATTRIB[2] = z;
}

CWGL_API void 
cwgl_vertexAttrib4f(cwgl_ctx_t* ctx, uint32_t indx, 
                    float x, float y, float z, float w){
    cwgl_VertexArrayObject_t* vao;
    vao = current_vao(ctx);
    if(! valid_attrib_index_p(indx)){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return;
    }
    vao->attrib[indx].CURRENT_VERTEX_ATTRIB[0] = x;
    vao->attrib[indx].CURRENT_VERTEX_ATTRIB[1] = y;
    vao->attrib[indx].CURRENT_VERTEX_ATTRIB[2] = z;
    vao->attrib[indx].CURRENT_VERTEX_ATTRIB[3] = w;
}

// 2.8 Vertex Arrays
CWGL_API void 
cwgl_vertexAttribPointer(cwgl_ctx_t* ctx, uint32_t indx, 
                         int32_t size, cwgl_enum_t type, int normalized, 
                         uint32_t stride, uint32_t offset){
    cwgl_Buffer_t* buf;
    cwgl_VertexArrayObject_t* vao;
    vao = current_vao(ctx);
    if(! valid_attrib_index_p(indx)){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return;
    }
    switch(type){
        case BYTE:
        case UNSIGNED_BYTE:
        case SHORT:
        case UNSIGNED_SHORT:
        case FLOAT:
            /* WebGL: FIXED is not accepted here. */
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    switch(size){
        case 1:
        case 2:
        case 3:
        case 4:
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_VALUE);
            return;
    }
    if(stride > 255){
        /* WebGL: Don't support large stride */
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return;
    }
    buf = ctx->state.bin.ARRAY_BUFFER_BINDING;
    if(! buf){
        /* WebGL: Require buffer bound. */
        /* FIXME: Do we really need to check offset here..? */
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return;

    }
    vao->attrib[indx].VERTEX_ATTRIB_ARRAY_SIZE = size;
    vao->attrib[indx].VERTEX_ATTRIB_ARRAY_TYPE = type;
    vao->attrib[indx].VERTEX_ATTRIB_ARRAY_NORMALIZED = normalized ? 
        CWGL_TRUE : CWGL_FALSE;
    vao->attrib[indx].VERTEX_ATTRIB_ARRAY_STRIDE = stride;
    vao->attrib[indx].VERTEX_ATTRIB_ARRAY_POINTER = offset;

    if(vao->attrib[indx].VERTEX_ATTRIB_ARRAY_BUFFER_BINDING){
        cwgl_priv_buffer_release(buf);
        vao->attrib[indx].VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = NULL;
    }
    cwgl_priv_objhdr_retain(&buf->hdr);
    vao->attrib[indx].VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = buf;
}

CWGL_API void 
cwgl_enableVertexAttribArray(cwgl_ctx_t* ctx, uint32_t index){
    cwgl_VertexArrayObject_t* vao;
    vao = current_vao(ctx);
    if(! valid_attrib_index_p(index)){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return;
    }
    vao->attrib[index].VERTEX_ATTRIB_ARRAY_ENABLED = CWGL_TRUE;
}

CWGL_API void 
cwgl_disableVertexAttribArray(cwgl_ctx_t* ctx, uint32_t index){
    cwgl_VertexArrayObject_t* vao;
    vao = current_vao(ctx);
    if(! valid_attrib_index_p(index)){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return;
    }
    vao->attrib[index].VERTEX_ATTRIB_ARRAY_ENABLED = CWGL_FALSE;
}

CWGL_API void 
cwgl_drawArrays(cwgl_ctx_t* ctx, cwgl_enum_t mode, 
                int32_t first, uint32_t count){
}

CWGL_API void 
cwgl_drawElements(cwgl_ctx_t* ctx, cwgl_enum_t mode, 
                  uint32_t count, cwgl_enum_t type, uint32_t offset){
}


// 2.9 Buffer Objects
static void
free_buffer(cwgl_Buffer_t* buffer){
    free(buffer);
    // FIXME: Teardown backend object here
}

static void
unbind_buffer(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer){
    int i;
    cwgl_VertexArrayObject_t* vao;
    if(buffer){
        if(ctx->state.bin.ARRAY_BUFFER_BINDING == buffer){
            cwgl_priv_buffer_release(buffer);
            ctx->state.bin.ARRAY_BUFFER_BINDING = NULL;
        }
        vao = current_vao(ctx);
        if(vao->state.ELEMENT_ARRAY_BUFFER_BINDING == buffer){
            cwgl_priv_buffer_release(buffer);
            vao->state.ELEMENT_ARRAY_BUFFER_BINDING = NULL;
        }
        for(i=0;i!=CWGL_MAX_VAO_SIZE;i++){
            if(vao->attrib[i].VERTEX_ATTRIB_ARRAY_BUFFER_BINDING == buffer){
                cwgl_priv_buffer_release(buffer);
                vao->attrib[i].VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = NULL;
            }
        }
    }
}

void /* exported to cwgl-tracker-vao.c */
cwgl_priv_buffer_release(cwgl_Buffer_t* buffer){
    uintptr_t v;
    if(buffer){
        v = cwgl_priv_objhdr_release(&buffer->hdr);
        if(!v){
            free_buffer(buffer);
        }
    }
}

CWGL_API void 
cwgl_bindBuffer(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_Buffer_t* buffer){
    cwgl_Buffer_t** point;
    cwgl_VertexArrayObject_t* vao;
    switch(target){
        case ARRAY_BUFFER:
            point = &ctx->state.bin.ARRAY_BUFFER_BINDING;
            break;
        case ELEMENT_ARRAY_BUFFER:
            vao = current_vao(ctx);
            point = &vao->state.ELEMENT_ARRAY_BUFFER_BINDING;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    if(*point){
        cwgl_priv_buffer_release(*point);
    }
    *point = buffer;
    if(buffer){
        cwgl_priv_objhdr_retain(&buffer->hdr);
    }
}

CWGL_API void 
cwgl_deleteBuffer(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer){
    unbind_buffer(ctx, buffer);
}

CWGL_API cwgl_Buffer_t* 
cwgl_createBuffer(cwgl_ctx_t* ctx){
    cwgl_Buffer_t* buf;
    buf = malloc(sizeof(cwgl_Buffer_t));
    if(buf){
        cwgl_priv_objhdr_init(ctx, &buf->hdr, CWGL_OBJ_BUFFER);
        cwgl_priv_buffer_init(&buf->state);
    }
    return buf;
}

CWGL_API void
cwgl_Buffer_release(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer){
    cwgl_priv_buffer_release(buffer);
}

CWGL_API void 
cwgl_bufferData(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                uint32_t size, void* data, cwgl_enum_t usage){
}

CWGL_API void 
cwgl_bufferSubData(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                   uint32_t offset, void* data, size_t buflen){
}

// 2.10.1 Loading and Creating Shader Source
CWGL_API cwgl_Shader_t* 
cwgl_createShader(cwgl_ctx_t* ctx, cwgl_enum_t type){
}

CWGL_API void
cwgl_Shader_release(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
}

CWGL_API void 
cwgl_shaderSource(cwgl_ctx_t* ctx, cwgl_Shader_t* shader, const char* source,
                  size_t sourcelen){
}

CWGL_API void 
cwgl_compileShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
}

CWGL_API void 
cwgl_deleteShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
}

// 2.10.3 Program Objects
CWGL_API cwgl_Program_t* 
cwgl_createProgram(cwgl_ctx_t* ctx){
}

CWGL_API void
cwgl_Program_release(cwgl_ctx_t* ctx, cwgl_Program_t* program){
}


CWGL_API void 
cwgl_attachShader(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                  cwgl_Shader_t* shader){
}

CWGL_API void 
cwgl_detachShader(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                  cwgl_Shader_t* shader){
}

CWGL_API void 
cwgl_linkProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
}

CWGL_API void 
cwgl_useProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
}

CWGL_API void 
cwgl_deleteProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
}

// 2.10.4 Shader Variables
CWGL_API cwgl_query_result_t 
cwgl_getActiveAttrib(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                     int32_t index, int32_t* out_size, 
                     int32_t* type, cwgl_string_t** name){
    // FIXME: Implement this
    return CWGL_QR_UNIMPLEMENTED;
}
CWGL_API int32_t 
cwgl_getAttribLocation(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                       const char* name){
}

CWGL_API void 
cwgl_bindAttribLocation(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                        uint32_t index, const char* name){
}

CWGL_API cwgl_UniformLocation_t* 
cwgl_getUniformLocation(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                        const char* name){
}

CWGL_API void
cwgl_UniformLocation_release(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* u){
}

CWGL_API cwgl_query_result_t 
cwgl_getActiveUniform(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                      int32_t index, int32_t* out_size, int32_t* out_type, 
                      cwgl_string_t** name){
}

CWGL_API void 
cwgl_uniform1f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, float x){
}

CWGL_API void 
cwgl_uniform1i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, int32_t x){
}

CWGL_API void 
cwgl_uniform2f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               float x, float y){
}

CWGL_API void 
cwgl_uniform2i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               int32_t x, int32_t y){
}

CWGL_API void 
cwgl_uniform3f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               float x, float y, float z){
}

CWGL_API void 
cwgl_uniform3i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               int32_t x, int32_t y, int32_t z){
}

CWGL_API void 
cwgl_uniform4f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               float x, float y, float z, float w){
}

CWGL_API void 
cwgl_uniform4i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               int32_t x, int32_t y, int32_t z, int32_t w){
}

CWGL_API void 
cwgl_uniform1fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                float* values, size_t count){
}

CWGL_API void 
cwgl_uniform1iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                int32_t* values, size_t count){
}

CWGL_API void 
cwgl_uniform2fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                float* values, size_t count){
}

CWGL_API void 
cwgl_uniform2iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                int32_t* values, size_t count){
}

CWGL_API void 
cwgl_uniform3fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                float* values, size_t count){
}

CWGL_API void 
cwgl_uniform3iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                int32_t* values, size_t count){
}

CWGL_API void 
cwgl_uniform4fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                float* values, size_t count){
}

CWGL_API void 
cwgl_uniform4iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                int32_t* values, size_t count){
}

CWGL_API void 
cwgl_uniformMatrix2fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                      int transpose, float* value, size_t count){
}

CWGL_API void 
cwgl_uniformMatrix3fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                      int transpose, float* value, size_t count){
}

CWGL_API void 
cwgl_uniformMatrix4fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                      int transpose, float* value, size_t count){
}


// 2.10.5 Shader Execution
CWGL_API void 
cwgl_validateProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
}

// 2.12.1 Controlling the Viewport
CWGL_API void 
cwgl_depthRange(cwgl_ctx_t* ctx, float zNear, float zFar){
    float cnear;
    float cfar;
    cnear = zNear;
    cfar = zFar;
    if(cnear > 1.0f){
        cnear = 1.0f;
    }
    if(cnear < 0.0f){
        cnear = 0.0f;
    }
    if(cfar > 1.0f){
        cfar = 1.0f;
    }
    if(cfar < 0.0f){
        cfar = 0.0f;
    }
    if(cfar < cnear){
        /* WebGL1 6.14 */
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }else{
        ctx->state.glo.DEPTH_RANGE[0] = cnear;
        ctx->state.glo.DEPTH_RANGE[1] = cfar;
    }
}


CWGL_API void 
cwgl_viewport(cwgl_ctx_t* ctx, 
              int32_t x, int32_t y, int32_t width, int32_t height){
    // FIXME: width/height Signedness do not match with scissor
    int32_t cwidth;
    int32_t cheight;
    if(width < 0){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return;
    }
    if(height < 0){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return;
    }
    if(width > ctx->state.cfg.MAX_VIEWPORT_DIMS[0]){
        cwidth = ctx->state.cfg.MAX_VIEWPORT_DIMS[0];
    }else{
        cwidth = width;
    }
    if(height > ctx->state.cfg.MAX_VIEWPORT_DIMS[1]){
        cheight = ctx->state.cfg.MAX_VIEWPORT_DIMS[1];
    }else{
        cheight = height;
    }
    ctx->state.glo.VIEWPORT[0] = x;
    ctx->state.glo.VIEWPORT[1] = y;
    ctx->state.glo.VIEWPORT[2] = cwidth;
    ctx->state.glo.VIEWPORT[3] = cheight;
}

