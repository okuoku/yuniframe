#include "cwgl-tracker-priv.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
        cwgl_priv_buffer_release(ctx, buf);
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
    cwgl_backend_drawArrays(ctx, mode, first, count);
}

CWGL_API void 
cwgl_drawElements(cwgl_ctx_t* ctx, cwgl_enum_t mode, 
                  uint32_t count, cwgl_enum_t type, uint32_t offset){
    cwgl_backend_drawElements(ctx, mode, count, type, offset);
}


// 2.9 Buffer Objects
static void
free_buffer(cwgl_Buffer_t* buffer){
    free(buffer);
}

static void
unbind_buffer(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer){
    int i;
    cwgl_VertexArrayObject_t* vao;
    if(buffer){
        if(ctx->state.bin.ARRAY_BUFFER_BINDING == buffer){
            cwgl_priv_buffer_release(ctx, buffer);
            ctx->state.bin.ARRAY_BUFFER_BINDING = NULL;
        }
        vao = current_vao(ctx);
        if(vao->state.ELEMENT_ARRAY_BUFFER_BINDING == buffer){
            cwgl_priv_buffer_release(ctx, buffer);
            vao->state.ELEMENT_ARRAY_BUFFER_BINDING = NULL;
        }
        for(i=0;i!=CWGL_MAX_VAO_SIZE;i++){
            if(vao->attrib[i].VERTEX_ATTRIB_ARRAY_BUFFER_BINDING == buffer){
                cwgl_priv_buffer_release(ctx, buffer);
                vao->attrib[i].VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = NULL;
            }
        }
    }
}

void /* exported to cwgl-tracker-vao.c */
cwgl_priv_buffer_release(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer){
    uintptr_t v;
    if(buffer){
        v = cwgl_priv_objhdr_release(&buffer->hdr);
        if(!v){
            cwgl_backend_Buffer_release(ctx, buffer);
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
        cwgl_priv_buffer_release(ctx, *point);
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
        cwgl_backend_Buffer_init(ctx, buf);
    }
    return buf;
}

CWGL_API void
cwgl_Buffer_release(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer){
    cwgl_priv_buffer_release(ctx, buffer);
}

CWGL_API void 
cwgl_bufferData(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                uint32_t size, void* data, cwgl_enum_t usage){
    cwgl_VertexArrayObject_t* vao;
    switch(target){
        case ARRAY_BUFFER:
            if(! ctx->state.bin.ARRAY_BUFFER_BINDING){
                CTX_SET_ERROR(ctx, INVALID_OPERATION);
                return;
            }
            break;
        case ELEMENT_ARRAY_BUFFER:
            vao = current_vao(ctx);
            if(! vao->state.ELEMENT_ARRAY_BUFFER_BINDING){
                CTX_SET_ERROR(ctx, INVALID_OPERATION);
                return;
            }
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    switch(usage){
        case STATIC_DRAW:
        case DYNAMIC_DRAW:
            /* Do nothing */
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    cwgl_backend_bufferData(ctx, target, size, data, usage);
}

CWGL_API void 
cwgl_bufferSubData(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                   uint32_t offset, void* data, size_t buflen){
    cwgl_backend_bufferSubData(ctx, target, offset, data, buflen);
}

// 2.10.1 Loading and Creating Shader Source
static void
release_shader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    uintptr_t v;
    if(shader){
        v = cwgl_priv_objhdr_release(&shader->hdr);
        if(! v){
            cwgl_string_release(ctx, shader->state.source);
            cwgl_string_release(ctx, shader->state.infolog);
            cwgl_backend_Shader_release(ctx, shader);
            free(shader);
        }
    }
}

CWGL_API cwgl_Shader_t* 
cwgl_createShader(cwgl_ctx_t* ctx, cwgl_enum_t type){
    cwgl_Shader_t* shader;
    switch(type){
        case VERTEX_SHADER:
        case FRAGMENT_SHADER:
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return NULL;
    }
    shader = malloc(sizeof(cwgl_Shader_t));
    if(shader){
        cwgl_priv_objhdr_init(ctx, &shader->hdr, CWGL_OBJ_SHADER);
        shader->state.SHADER_TYPE = type;
        shader->state.DELETE_STATUS = CWGL_FALSE;
        shader->state.COMPILE_STATUS = CWGL_FALSE;
        shader->state.source = NULL;
        shader->state.infolog = NULL;
        cwgl_backend_Shader_init(ctx, shader);
    }
    return shader;
}

CWGL_API void
cwgl_Shader_release(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    release_shader(ctx, shader);
}

CWGL_API void 
cwgl_shaderSource(cwgl_ctx_t* ctx, cwgl_Shader_t* shader, const char* source,
                  size_t sourcelen){
    cwgl_string_t* src;
    cwgl_string_release(ctx, shader->state.source);
    src = cwgl_priv_alloc_string(ctx, source, sourcelen);
    shader->state.source = src;
    return;
}

CWGL_API void 
cwgl_compileShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    cwgl_backend_compileShader(ctx, shader);
}

CWGL_API void 
cwgl_deleteShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    if(shader){
        shader->state.DELETE_STATUS = CWGL_TRUE;
    }
}

// 2.10.3 Program Objects
static void
release_activeinfo(cwgl_ctx_t* ctx, cwgl_activeinfo_t* a, uint32_t count){
    int i;
    if(a){
        for(i=0;i!=count;i++){
            cwgl_string_release(ctx, a[i].name);
        }
        free(a);
    }
}

static void
clear_attriblocations(cwgl_ctx_t* ctx, cwgl_attriblocation_t* loc){
    int i;
    for(i=0;i!=CWGL_MAX_VAO_SIZE;i++){
        cwgl_string_release(ctx, loc[i].name);
        loc[i].name = NULL;
        loc[i].active_index = -1;
    }
}

int /* Exported to backend */
cwgl_integ_program_setup(cwgl_ctx_t* ctx, cwgl_Program_t* program,
                         uint32_t n_uniform, uint32_t n_attribute){
    uint32_t uniformcount;
    uint32_t attributecount;
    cwgl_activeinfo_t* uniforms;
    cwgl_activeinfo_t* attributes;
    release_activeinfo(ctx, program->state.uniforms, 
                       program->state.ACTIVE_UNIFORMS);
    release_activeinfo(ctx, program->state.attributes, 
                       program->state.ACTIVE_ATTRIBUTES);
    program->state.ACTIVE_ATTRIBUTES = n_attribute;
    program->state.ACTIVE_UNIFORMS = n_uniform;
    uniformcount = program->state.ACTIVE_UNIFORMS;
    attributecount = program->state.ACTIVE_ATTRIBUTES;
    uniforms = malloc(sizeof(cwgl_activeinfo_t)*uniformcount);
    if(uniforms){
        memset(uniforms,0,sizeof(cwgl_activeinfo_t)*uniformcount);
    }
    attributes = malloc(sizeof(cwgl_activeinfo_t)*attributecount);
    if(attributes){
        memset(attributes,0,sizeof(cwgl_activeinfo_t)*attributecount);
    }
    program->state.uniforms = uniforms;
    program->state.attributes = attributes;
    return 0;
}

static void
release_program(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    uintptr_t v;
    if(program){
        v = cwgl_priv_objhdr_release(&program->hdr);
        if(! v){
            release_activeinfo(ctx, program->state.uniforms, 
                               program->state.ACTIVE_UNIFORMS);
            release_activeinfo(ctx, program->state.attributes, 
                               program->state.ACTIVE_ATTRIBUTES);
            release_shader(ctx, program->state.vertex_shader);
            release_shader(ctx, program->state.fragment_shader);
            cwgl_string_release(ctx, program->state.infolog);
            cwgl_backend_Program_release(ctx, program);
            clear_attriblocations(ctx, program->state.attriblocations);
            free(program);
        }
    }
}

CWGL_API cwgl_Program_t* 
cwgl_createProgram(cwgl_ctx_t* ctx){
    cwgl_Program_t* program;
    program = malloc(sizeof(cwgl_Program_t));
    if(program){
        cwgl_priv_objhdr_init(ctx, &program->hdr, CWGL_OBJ_PROGRAM);
        memset(program->state.attriblocations, 0, 
               sizeof(cwgl_attriblocation_t)*CWGL_MAX_VAO_SIZE);
        program->state.DELETE_STATUS = CWGL_FALSE;
        program->state.LINK_STATUS = CWGL_FALSE;
        program->state.VALIDATE_STATUS = CWGL_FALSE;
        program->state.ATTACHED_SHADERS = 0;
        program->state.ACTIVE_ATTRIBUTES = 0;
        program->state.ACTIVE_UNIFORMS = 0;
        program->state.vertex_shader = NULL;
        program->state.fragment_shader = NULL;
        program->state.infolog = NULL;
        program->state.uniforms = NULL;
        program->state.attributes = NULL;
        program->state.uniformcontents = NULL;
        program->state.age_link = 0;
        cwgl_backend_Program_init(ctx, program);
    }
    return program;
}

CWGL_API void
cwgl_Program_release(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    release_program(ctx, program);
}


CWGL_API void 
cwgl_attachShader(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                  cwgl_Shader_t* shader){
    int i;
    cwgl_enum_t type;
    type = shader->state.SHADER_TYPE;
    // FIXME: Single shader object should not be attached
    //        to multiple program
    switch(type){
        case VERTEX_SHADER:
            if(program->state.vertex_shader != NULL){
                CTX_SET_ERROR(ctx, INVALID_OPERATION);
                return;
            }
            cwgl_priv_objhdr_retain(&shader->hdr);
            program->state.vertex_shader = shader;
            break;
        case FRAGMENT_SHADER:
            if(program->state.fragment_shader != NULL){
                CTX_SET_ERROR(ctx, INVALID_OPERATION);
                return;
            }
            cwgl_priv_objhdr_retain(&shader->hdr);
            program->state.fragment_shader = shader;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_OPERATION);
            return;
    }
    i = 0;
    if(program->state.vertex_shader){
        i++;
    }
    if(program->state.fragment_shader){
        i++;
    }
    program->state.ATTACHED_SHADERS = i;
}

CWGL_API void 
cwgl_detachShader(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                  cwgl_Shader_t* shader){
    int i;
    cwgl_enum_t type;
    type = shader->state.SHADER_TYPE;
    switch(type){
        case VERTEX_SHADER:
            if(program->state.vertex_shader != shader){
                CTX_SET_ERROR(ctx, INVALID_OPERATION);
                return;
            }
            release_shader(ctx, shader);
            program->state.vertex_shader = NULL;
            break;
        case FRAGMENT_SHADER:
            if(program->state.fragment_shader != shader){
                CTX_SET_ERROR(ctx, INVALID_OPERATION);
                return;
            }
            release_shader(ctx, shader);
            program->state.fragment_shader = NULL;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_OPERATION);
            return;
    }
    i = 0;
    if(program->state.vertex_shader){
        i++;
    }
    if(program->state.fragment_shader){
        i++;
    }
    program->state.ATTACHED_SHADERS = i;
    clear_attriblocations(ctx, program->state.attriblocations);
}

CWGL_API void 
cwgl_linkProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    int i,j,r;
    unsigned int attrib_binding[CWGL_MAX_VAO_SIZE];
    cwgl_attriblocation_t* a;
    cwgl_activeinfo_t* f;
    cwgl_backend_linkProgram(ctx, program);
    program->state.age_link++;
    // FIXME: Follow WebGL alias rule
    if(program->state.ACTIVE_ATTRIBUTES > CWGL_MAX_VAO_SIZE){
        printf("CWGL: Too many active attributes ..?? %d\n",
               program->state.ACTIVE_ATTRIBUTES);
    }else{
        a = program->state.attriblocations;
        f = program->state.attributes;
        for(i=0;i!=CWGL_MAX_VAO_SIZE;i++){
            a[i].active_index = -1;
            attrib_binding[i] = -1;
        }
        /* First, bind user-specified binding */
        for(i=0;i!=CWGL_MAX_VAO_SIZE;i++){
            if(a[i].name){
                for(j=0;j!=program->state.ACTIVE_ATTRIBUTES;j++){
                    r = cwgl_priv_match_symobj(ctx, a[i].name,
                                               f[j].name, NULL, NULL);
                    if(r){
                        attrib_binding[j] = i;
                        a[i].active_index = j;
                    }
                }
            }
        }
        /* Bind rest of attributes */
        for(j=0;j!=program->state.ACTIVE_ATTRIBUTES;j++){
            if(attrib_binding[j] == -1){
                for(i=0;i!=CWGL_MAX_VAO_SIZE;i++){
                    if(! a[i].name){
                        a[i].name = cwgl_priv_string_dup(ctx, f[j].name);
                        a[i].active_index = j;
                        attrib_binding[j] = i;
                        break;
                    }
                }
            }
        }
    }
    free(program->state.uniformcontents);
    program->state.uniformcontents = malloc(sizeof(cwgl_uniformcontent_t)*
                                            program->state.uniform_buffer_size);
    memset(program->state.uniformcontents,0,sizeof(cwgl_uniformcontent_t)*
                                            program->state.uniform_buffer_size);

}

CWGL_API void 
cwgl_useProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    if(! program){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return;
    }
    cwgl_priv_objhdr_retain(&program->hdr);
    release_program(ctx, ctx->state.bin.CURRENT_PROGRAM);
    ctx->state.bin.CURRENT_PROGRAM = program;
}

CWGL_API void 
cwgl_deleteProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    if(program){
        program->state.DELETE_STATUS = CWGL_TRUE;
    }
}

// 2.10.4 Shader Variables
CWGL_API cwgl_query_result_t 
cwgl_getActiveAttrib(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                     int32_t index, int32_t* out_size, 
                     int32_t* type, cwgl_string_t** name){
    cwgl_activeinfo_t* a;
    if(! program){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return CWGL_QR_GLERROR;
    }
    if(index < 0){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return CWGL_QR_GLERROR;
    }
    if(index >= program->state.ACTIVE_ATTRIBUTES){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return CWGL_QR_GLERROR;
    }
    a = program->state.attributes;
    *out_size = a[index].size;
    *type = a[index].type;
    *name = cwgl_priv_string_dup(ctx,a[index].name);
    return CWGL_QR_SUCCESS;
}

CWGL_API int32_t 
cwgl_getAttribLocation(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                       const char* name){
    // FIXME: Check for gl_
    int i,r;
    if(! program->state.LINK_STATUS){
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return -1;
    }
    for(i=0;i!=CWGL_MAX_VAO_SIZE;i++){
        r = cwgl_priv_match_sym(ctx, 
                                program->state.attriblocations[i].name,
                                name,
                                NULL, NULL);
        if(r){
            return i;
        }
    }
    return -1;
}

CWGL_API void 
cwgl_bindAttribLocation(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                        uint32_t index, const char* name){
    // FIXME: Check for gl_
    size_t len;
    if(index >= CWGL_MAX_VAO_SIZE){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return;
    }
    len = strnlen(name, 1024);
    cwgl_string_release(ctx, program->state.attriblocations[index].name);
    program->state.attriblocations[index].name =
        cwgl_priv_alloc_string(ctx, name, len);
}

static int
calc_type_size(cwgl_Program_t* program, int index){
    int basetype;
    cwgl_enum_t type;
    cwgl_activeinfo_t* a;
    a = program->state.uniforms;
    type = a[index].type;
    switch(type){
        default:
        case FLOAT:
        case INT:
        case BOOL:
        case SAMPLER_2D:
        case SAMPLER_CUBE:
            basetype = 1;
            break;
        case FLOAT_VEC2:
        case INT_VEC2:
        case BOOL_VEC2:
            basetype = 2;
            break;
        case FLOAT_VEC3:
        case INT_VEC3:
        case BOOL_VEC3:
            basetype = 3;
            break;
        case FLOAT_VEC4:
        case INT_VEC4:
        case BOOL_VEC4:
            basetype = 4;
            break;
        case FLOAT_MAT2:
            basetype = 2*2;
            break;
        case FLOAT_MAT3:
            basetype = 3*3;
            break;
        case FLOAT_MAT4:
            basetype = 4*4;
            break;
    }
    return basetype;
}

CWGL_API cwgl_UniformLocation_t* 
cwgl_getUniformLocation(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                        const char* name){
    int i,r;
    int ax, ay;
    int indx;
    int array_index;
    cwgl_activeinfo_t* a;
    cwgl_UniformLocation_t* u;
    if(!program->state.LINK_STATUS){
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return NULL;
    }
    /* Calc index */
    indx = -1;
    a = program->state.uniforms;
    for(i=0;i!=program->state.ACTIVE_UNIFORMS;i++){
        r = cwgl_priv_match_sym(ctx, a[i].name, name, &ax, &ay);
        if(r){
            if(ay != -1){
                array_index = ay;
            }else{
                array_index = 0;
            }
            indx = r;
            break;
        }
    }
    if(indx == -1){
        return NULL;
    }
    /* Allocate object, retain Program */
    u = malloc(sizeof(cwgl_UniformLocation_t));
    cwgl_priv_objhdr_retain(&program->hdr);
    u->index = indx;
    u->program = program;
    u->age_link = program->state.age_link;
    /* Calc storage offset */
    u->offset = a[indx].offset + 
        (array_index * calc_type_size(program, indx));
    return u;
}

CWGL_API void
cwgl_UniformLocation_release(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* u){
    release_program(ctx, u->program);
    free(u);
}

CWGL_API cwgl_query_result_t 
cwgl_getActiveUniform(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                      int32_t index, int32_t* out_size, int32_t* out_type, 
                      cwgl_string_t** name){
    cwgl_activeinfo_t* a;
    if(! program){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return CWGL_QR_GLERROR;
    }
    if(index < 0){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return CWGL_QR_GLERROR;
    }
    if(index >= program->state.ACTIVE_UNIFORMS){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
        return CWGL_QR_GLERROR;
    }
    a = program->state.uniforms;
    *out_size = a[index].size;
    *out_type = a[index].type;
    *name = cwgl_priv_string_dup(ctx,a[index].name);
    return CWGL_QR_SUCCESS;
}

static int /* bool */
valid_uniformlocation_p(cwgl_UniformLocation_t* loc, int req){
    if(loc->age_link != loc->program->state.age_link){
        return 0;
    }else{
        if(loc->program->state.uniform_buffer_size <
           req + loc->offset){
            return 0;
        }else{
            return 1;
        }
    }
}

CWGL_API void 
cwgl_uniform1f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, float x){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,1)){
        u[off+0].asFloat = x;
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniform1i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, int32_t x){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,1)){
        u[off+0].asInt = x;
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniform2f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               float x, float y){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,2)){
        u[off+0].asFloat = x;
        u[off+1].asFloat = y;
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniform2i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               int32_t x, int32_t y){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,2)){
        u[off+0].asInt = x;
        u[off+1].asInt = y;
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniform3f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               float x, float y, float z){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,3)){
        u[off+0].asFloat = x;
        u[off+1].asFloat = y;
        u[off+2].asFloat = z;
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniform3i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               int32_t x, int32_t y, int32_t z){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,3)){
        u[off+0].asInt = x;
        u[off+1].asInt = y;
        u[off+2].asInt = z;
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniform4f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               float x, float y, float z, float w){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,4)){
        u[off+0].asFloat = x;
        u[off+1].asFloat = y;
        u[off+2].asFloat = z;
        u[off+3].asFloat = w;
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniform4i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               int32_t x, int32_t y, int32_t z, int32_t w){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,4)){
        u[off+0].asInt = x;
        u[off+1].asInt = y;
        u[off+2].asInt = z;
        u[off+3].asInt = w;
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

static void
fill_uniform_int(cwgl_UniformLocation_t* loc, int32_t* values,
                 size_t count){
    uint32_t i;
    uint32_t off;
    cwgl_uniformcontent_t* u;
    u = loc->program->state.uniformcontents;
    off = loc->offset;

    for(i=0;i!=count;i++){
        u[off+i].asInt = values[i];
    }
}

static void
fill_uniform_float(cwgl_UniformLocation_t* loc, float* values,
                   size_t count){
    uint32_t i;
    uint32_t off;
    cwgl_uniformcontent_t* u;
    u = loc->program->state.uniformcontents;
    off = loc->offset;

    for(i=0;i!=count;i++){
        u[off+i].asFloat = values[i];
    }
}

CWGL_API void 
cwgl_uniform1fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                float* values, size_t count){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,count)){
        fill_uniform_float(location, values, count);
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniform1iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                int32_t* values, size_t count){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,count)){
        fill_uniform_int(location, values, count);
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniform2fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                float* values, size_t count){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,2*count)){
        fill_uniform_float(location, values, 2*count);
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniform2iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                int32_t* values, size_t count){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,2*count)){
        fill_uniform_int(location, values, 2*count);
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniform3fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                float* values, size_t count){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,3*count)){
        fill_uniform_float(location, values, 3*count);
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniform3iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                int32_t* values, size_t count){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,3*count)){
        fill_uniform_int(location, values, 3*count);
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniform4fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                float* values, size_t count){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,4*count)){
        fill_uniform_float(location, values, 4*count);
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniform4iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                int32_t* values, size_t count){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,4*count)){
        fill_uniform_int(location, values, 4*count);
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

static void
fill_uniform_float_xpose(cwgl_UniformLocation_t* loc, float* values,
                         int matsize, int count){
    uint32_t i,j,c;
    uint32_t off,b;

    cwgl_uniformcontent_t* u;
    u = loc->program->state.uniformcontents;
    off = loc->offset;

    for(c=0;c!=count;c++){
        b = c * (matsize*matsize);
        for(j=0;j!=matsize;j++){
            for(i=0;i!=matsize;i++){
                u[b+off+(matsize*i+j)].asFloat = values[b+(matsize*j+i)];
            }
        }
    }
}

CWGL_API void 
cwgl_uniformMatrix2fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                      int transpose, float* value, size_t count){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,2*2*count)){
        if(transpose){
            fill_uniform_float_xpose(location,
                                     value, 2, 
                                     count);
        }else{
            fill_uniform_float(location, value, 2*2*count);
        }
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniformMatrix3fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                      int transpose, float* value, size_t count){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,3*3*count)){
        if(transpose){
            fill_uniform_float_xpose(location,
                                     value, 3, 
                                     count);
        }else{
            fill_uniform_float(location, value, 3*3*count);
        }
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}

CWGL_API void 
cwgl_uniformMatrix4fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                      int transpose, float* value, size_t count){
    cwgl_uniformcontent_t* u;
    uint32_t off;
    u = location->program->state.uniformcontents;
    off = location->offset;
    if(valid_uniformlocation_p(location,4*4*count)){
        if(transpose){
            fill_uniform_float_xpose(location,
                                     value, 4, 
                                     count);
        }else{
            fill_uniform_float(location, value, 4*4*count);
        }
    }else{
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
    }
}


// 2.10.5 Shader Execution
CWGL_API void 
cwgl_validateProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    cwgl_backend_validateProgram(ctx, program);
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

