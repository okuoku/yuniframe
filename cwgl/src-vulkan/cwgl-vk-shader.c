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
        vkDestroyPipelineLayout(backend->device, program_backend->pipeline_layout, NULL);
        vkDestroyDescriptorSetLayout(backend->device, program_backend->desc_set_layout, NULL);
        vkResetDescriptorPool(backend->device, program_backend->desc_pool, 0);
        vkDestroyDescriptorPool(backend->device, program_backend->desc_pool, NULL);
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
        /* Generate descriptors and layouts */
        {
            VkDescriptorPoolCreateInfo dpi;
            VkDescriptorPoolSize two[2]; /* Uniform, and opaques */
            VkDescriptorSetAllocateInfo dai;
            VkDescriptorSetLayoutCreateInfo dli;
            VkDescriptorSetLayoutBinding dlb[32+1]; /* 32 opaques and 1 UBO */
            VkPipelineLayoutCreateInfo pli;
            if(p->opaque_count > 32){
                printf("Too many opaque fields\n");
                return -1;
            }
            two[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            two[0].descriptorCount = 1; /* No UBO atm */
            two[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            two[1].descriptorCount = p->opaque_count;
            dpi.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            dpi.pNext = NULL;
            dpi.flags = 0;
            dpi.maxSets = 1;
            dpi.poolSizeCount = 2;
            dpi.pPoolSizes = two;
            rv = vkCreateDescriptorPool(backend->device, &dpi, NULL, &program_backend->desc_pool);
            if(rv != VK_SUCCESS){
                printf("Failed to create descriptor pool\n");
            }
            dlb[0].binding = 0;
            dlb[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            dlb[0].descriptorCount = 1;
            dlb[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
            dlb[0].pImmutableSamplers = NULL;
            for(i=0;i!=p->opaque_count;i++){
                dlb[i+1].binding = i+1;
                dlb[i+1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                dlb[i+1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
                dlb[i+1].descriptorCount = 1;
                dlb[i+1].pImmutableSamplers = NULL;
            }
            dli.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
            dli.pNext = NULL;
            dli.flags = 0;
            dli.bindingCount = p->opaque_count + 1;
            dli.pBindings = dlb;
            rv = vkCreateDescriptorSetLayout(backend->device, &dli, NULL, &program_backend->desc_set_layout);
            if(rv != VK_SUCCESS){
                printf("Failed to create descriptor set layout\n");
            }
            dai.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
            dai.pNext = NULL;
            dai.descriptorPool = program_backend->desc_pool;
            dai.descriptorSetCount = 1;
            dai.pSetLayouts = &program_backend->desc_set_layout;
            rv = vkAllocateDescriptorSets(backend->device, &dai, &program_backend->desc_set);
            if(rv != VK_SUCCESS){
                printf("Failed to create descriptor set\n");
            }
            pli.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pli.pNext = NULL;
            pli.flags = 0;
            pli.pushConstantRangeCount = 0;
            pli.pPushConstantRanges = NULL;
            pli.setLayoutCount = 1;
            pli.pSetLayouts = &program_backend->desc_set_layout;
            rv = vkCreatePipelineLayout(backend->device, &pli, NULL, &program_backend->pipeline_layout);
            if(rv != VK_SUCCESS){
                printf("Failed to create pipeline layout\n");
            }
        }
        /* Allocate uniform buffer */
        {
            int memtypeidx;
            VkBufferCreateInfo bi;
            VkMemoryAllocateInfo ai;
            VkWriteDescriptorSet w;
            VkDescriptorBufferInfo dbi;
            bi.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bi.pNext = NULL;
            bi.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
            bi.flags = 0;
            bi.size = p->uniform_size;
            bi.queueFamilyIndexCount = 1;
            bi.pQueueFamilyIndices = &backend->queue_family_index;
            bi.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            rv = vkCreateBuffer(backend->device, &bi, NULL, &program_backend->uniform_buffer.buffer);
            if(rv != VK_SUCCESS){
                printf("Failed to create uniform buffer\n");
            }
            ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            ai.pNext = NULL;
            memtypeidx = cwgl_vkpriv_select_memory_type(ctx, UINT32_MAX,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            ai.memoryTypeIndex = memtypeidx;
            ai.allocationSize = p->uniform_size;
            if(i < 0){
                printf("Could not find memtype\n");
            }
            rv = vkAllocateMemory(backend->device, &ai, NULL, &program_backend->uniform_buffer.device_memory);
            if(rv != VK_SUCCESS){
                printf("Failed to allocate memory\n");
            }
            rv = vkBindBufferMemory(backend->device,
                                    program_backend->uniform_buffer.buffer,
                                    program_backend->uniform_buffer.device_memory, 0);
            if(rv != VK_SUCCESS){
                printf("Failed to bind memory\n");
            }
            program_backend->uniform_buffer.allocated = 1;
            /* Preconfigure uniform buffer to descriptor */
            w.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            w.pNext = NULL;
            w.dstSet = program_backend->desc_set;
            w.descriptorCount = 1;
            w.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            dbi.buffer = program_backend->uniform_buffer.buffer;
            dbi.offset = 0;
            dbi.range = p->uniform_size;
            w.pBufferInfo = &dbi;
            w.dstArrayElement = 0;
            w.dstBinding = 0;
            vkUpdateDescriptorSets(backend->device, 1, &w, 0, NULL);
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
