#include "cwgl-backend-vk-priv.h"
#include <stdlib.h>
#include <stdio.h>

#include "shxm_private.h"

// FIXME: Consider where to decl APIs
cwgl_string_t* cwgl_priv_alloc_string(cwgl_ctx_t* ctx, const char* str, size_t buflen);

void
cwgl_vkpriv_destroy_program(cwgl_ctx_t* ctx, cwgl_backend_Program_t* program_backend){
    cwgl_backend_ctx_t* backend;
    backend = ctx->backend;
    // FIXME: Free SHXM object here.
    cwgl_vkpriv_destroy_buffer(ctx, &program_backend->uniform_buffer);
    cwgl_vkpriv_destroy_buffer(ctx, &program_backend->attribute_registers);
    if(program_backend->allocated){
        vkDestroyShaderModule(backend->device, program_backend->pixel_shader, NULL);
        vkDestroyShaderModule(backend->device, program_backend->vertex_shader, NULL);
        program_backend->allocated = 0;
    }
}

int
cwgl_backend_compileShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    int r;
    size_t srcsize;
    size_t infosize;
    char* buf;
    shxm_ctx_t* shxm;
    shxm_shader_t* s;
    shxm = ctx->backend->shxm_ctx;
    s = shader->backend->shader;
    // FIXME: Move this to tracker
    cwgl_string_release(ctx, shader->state.infolog);
    shader->state.infolog = NULL;
    if(! shader->state.source){
        shader->state.COMPILE_STATUS = CWGL_FALSE;
        return 1;
    }
    srcsize = cwgl_string_size(ctx, shader->state.source);
    if(! srcsize){
        return 1;
    }
    buf = malloc(srcsize);
    cwgl_string_read(ctx, shader->state.source,
                     buf, srcsize);
    shxm_shader_source(shxm, s, buf, srcsize-1);
    free(buf);
    r = shxm_shader_compile(shxm, s);
    shader->state.COMPILE_STATUS = (r == 0) ? CWGL_TRUE: CWGL_FALSE;
    // FIXME: Implement infolog
    return 0;
}


static cwgl_enum_t
to_gltype(cwgl_var_type_t type){
    switch(type){
        default:
            printf("INVALID TYPE!!\n");
            return FLOAT;
        case CWGL_VAR_FLOAT:
            return FLOAT;
        case CWGL_VAR_FLOAT_VEC2:
            return FLOAT_VEC2;
        case CWGL_VAR_FLOAT_VEC3:
            return FLOAT_VEC3;
        case CWGL_VAR_FLOAT_VEC4:
            return FLOAT_VEC4;
        case CWGL_VAR_INT:
            return INT;
        case CWGL_VAR_INT_VEC2:
            return INT_VEC2;
        case CWGL_VAR_INT_VEC3:
            return INT_VEC3;
        case CWGL_VAR_INT_VEC4:
            return INT_VEC4;
        case CWGL_VAR_BOOL:
            return BOOL;
        case CWGL_VAR_BOOL_VEC2:
            return BOOL_VEC2;
        case CWGL_VAR_BOOL_VEC3:
            return BOOL_VEC3;
        case CWGL_VAR_BOOL_VEC4:
            return BOOL_VEC4;
        case CWGL_VAR_FLOAT_MAT2:
            return FLOAT_MAT2;
        case CWGL_VAR_FLOAT_MAT3:
            return FLOAT_MAT3;
        case CWGL_VAR_FLOAT_MAT4:
            return FLOAT_MAT4;
        case CWGL_VAR_SAMPLER_2D:
            return SAMPLER_2D;
        case CWGL_VAR_SAMPLER_CUBE:
            return SAMPLER_CUBE;
    }
}

static int
to_activesize(int s){
    if(! s){
        return 1;
    }else{
        return s;
    }
}

int
cwgl_backend_linkProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    int i;
    int b;
    int o;
    int r;
    char* buf;
    cwgl_activeinfo_t* a;
    size_t namelen;
    cwgl_backend_ctx_t* backend;
    cwgl_backend_Program_t* program_backend;
    shxm_ctx_t* shxm;
    shxm_program_t* p;
    VkResult rv;
    VkShaderModuleCreateInfo mi;
    backend = ctx->backend;
    program_backend = program->backend;
    shxm = ctx->backend->shxm_ctx;
    p = program->backend->program;
    // FIXME: Move this to tracker
    cwgl_string_release(ctx, program->state.infolog);
    program->state.infolog = NULL;

    /* Attach shaders */
    // FIXME: Implement infolog
    // FIXME: Detach program if needed (Review SHXM interface)
    shxm_program_attach(shxm, p, 
                        program->state.vertex_shader->backend->shader);
    shxm_program_attach(shxm, p, 
                        program->state.fragment_shader->backend->shader);
    /* Perform link */
    r = shxm_program_link(shxm, p);
    program->state.LINK_STATUS = (r == 0) ? CWGL_TRUE : CWGL_FALSE;
    if(r){
        return r;
    }

    /* Populate Uniform/Attributes */
    r = cwgl_integ_program_setup(ctx, program,
                                 p->uniform_count + p->opaque_count,
                                 p->input_count);
    if(!r){
        a = program->state.attributes;
        for(i=0;i!=p->input_count;i++){
            namelen = strnlen(p->input[i].slot->name, 1024);
            a[i].type = to_gltype(p->input[i].slot->type);
            a[i].size = to_activesize(p->input[i].slot->array_length);
            a[i].name = cwgl_priv_alloc_string(ctx, p->input[i].slot->name,
                                               namelen);
            a[i].offset = 0;
            a[i].location = p->input[i].location;

        }
        a = program->state.uniforms;
        for(i=0;i!=p->uniform_count;i++){
            namelen = strnlen(p->uniform[i].slot->name, 1024);
            a[i].type = to_gltype(p->uniform[i].slot->type);
            a[i].size = to_activesize(p->uniform[i].slot->array_length);
            a[i].name = cwgl_priv_alloc_string(ctx, p->uniform[i].slot->name,
                                               namelen);
            a[i].offset = p->uniform[i].offset;
            a[i].location = p->uniform[i].offset; // FIXME: ??
        }
        o = p->uniform_size;
        o = (o + 7) & ~7;
        // FIXME: Every opaque fields needs to fill in 64bits for now
        // FIXME: This does not handle texture arrays
        b = p->uniform_count;
        for(i=0;i!=p->opaque_count;i++){
            namelen = strnlen(p->opaque[i].slot->name, 1024);
            a[b+i].type = to_gltype(p->opaque[i].slot->type);
            a[b+i].size = to_activesize(p->opaque[i].slot->array_length);
            a[b+i].name = cwgl_priv_alloc_string(ctx, p->opaque[i].slot->name,
                                               namelen);
            a[b+i].offset = o; // FIXME: ??
            a[b+i].location = p->uniform[i].offset; // FIXME: ??
            o += 8;
        }
        program->state.uniform_buffer_size = o;

        /* Generate backend program object */
        cwgl_vkpriv_destroy_program(ctx, program_backend);
        mi.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        mi.pNext = NULL;
        mi.flags = 0;
        mi.codeSize = program_backend->program->vertex_ir_len * sizeof(uint32_t);
        mi.pCode = program_backend->program->vertex_ir;
        rv = vkCreateShaderModule(backend->device, &mi, NULL, &program_backend->vertex_shader);
        if(rv != VK_SUCCESS){
            printf("Failed to load vertex shader module.\n");
        }
        mi.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        mi.pNext = NULL;
        mi.flags = 0;
        mi.codeSize = program_backend->program->fragment_ir_len * sizeof(uint32_t);
        mi.pCode = program_backend->program->fragment_ir;
        rv = vkCreateShaderModule(backend->device, &mi, NULL, &program_backend->pixel_shader);
        if(rv != VK_SUCCESS){
            printf("Failed to load fragment shader module.\n");
        }
        // FIXME: Allocate attribute_registers
        program_backend->input_count = p->input_count;
        program_backend->allocated = 1;
    }

    return 0;
}

int
cwgl_backend_validateProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    // FIXME: Implement this
    program->state.VALIDATE_STATUS = CWGL_TRUE;
    return 0;
}
