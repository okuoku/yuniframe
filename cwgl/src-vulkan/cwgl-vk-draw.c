#include "cwgl-backend-vk-priv.h"
#include <stdio.h>
#include <string.h>

#include "shxm_private.h"

// FIXME: Copied from s2
static cwgl_VertexArrayObject_t*
current_vao(cwgl_ctx_t* ctx){
    if(ctx->state.bin.VERTEX_ARRAY_BINDING){
        return ctx->state.bin.VERTEX_ARRAY_BINDING;
    }else{
        return ctx->state.default_vao;
    }
}

static cwgl_Texture_t*
current_texture(cwgl_ctx_t* ctx, int id, cwgl_enum_t slot){
    cwgl_texture_unit_state_t* s;
    s = ctx->state.bin.texture_unit;
    if(id > 32){
        return NULL;
    }
    switch(slot){
        case TEXTURE_2D:
            return s[id].TEXTURE_BINDING_2D;
        case TEXTURE_CUBE_MAP:
            return s[id].TEXTURE_BINDING_CUBE_MAP;
    }
    return NULL;
}

static int
update_texture(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
    cwgl_backend_Texture_t* texture_backend;
    cwgl_backend_ctx_t* backend;
    VkSamplerCreateInfo si;
    VkResult r;
    texture_backend = texture->backend;
    backend = ctx->backend;
    if(texture_backend->sampler_allocated){
        if(! memcmp(&texture_backend->cached_tracker_state,
                    &texture->state,
                    sizeof(cwgl_texture_state_t))){
            /* Cache hit */
            return 0;
        }else{
            vkDestroySampler(backend->device, texture_backend->sampler, NULL);
            texture_backend->sampler_allocated = 0;
        }
    }

    texture_backend->cached_tracker_state = texture->state;
    si.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    si.pNext = NULL;
    si.flags = 0;
    switch(texture->state.TEXTURE_MIN_FILTER){
        default:
        case NEAREST:
        case LINEAR:
        case NEAREST_MIPMAP_NEAREST:
        case LINEAR_MIPMAP_NEAREST:
        case NEAREST_MIPMAP_LINEAR:
        case LINEAR_MIPMAP_LINEAR:
            // FIXME: Debug
            si.minFilter = VK_FILTER_LINEAR;
            si.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            break;
    }
    switch(texture->state.TEXTURE_MAG_FILTER){
        default:
        case NEAREST:
        case LINEAR:
            // FIXME: Debug
            si.magFilter = VK_FILTER_LINEAR;
            break;
    }
    switch(texture->state.TEXTURE_WRAP_S){
        default:
        case CLAMP_TO_EDGE:
            si.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            break;
        case MIRRORED_REPEAT:
            si.addressModeU = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
            break;
        case REPEAT:
            si.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            break;
    }
    switch(texture->state.TEXTURE_WRAP_T){
        default:
        case CLAMP_TO_EDGE:
            si.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
            break;
        case MIRRORED_REPEAT:
            si.addressModeV = VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
            break;
        case REPEAT:
            si.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            break;
    }
    si.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    si.mipLodBias = 0.0f;
    si.anisotropyEnable = VK_FALSE;
    si.maxAnisotropy = VK_FALSE;
    si.compareEnable = VK_FALSE;
    si.compareOp = 0;
    si.minLod = 0;
    si.maxLod = 0;
    si.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
    si.unnormalizedCoordinates = VK_FALSE;
    r = vkCreateSampler(backend->device, &si, NULL, &texture_backend->sampler);
    if(r != VK_SUCCESS){
        printf("Failed to create sampler");
        return -1;
    }
    texture_backend->sampler_allocated = 1;
    return 0;
}

static int
create_framebuffer(cwgl_ctx_t* ctx, 
                   const cwgl_backend_Framebuffer_t* framebuffer_backend,
                   int is_framebuffer, VkRenderPass renderpass, 
                   VkFramebuffer* out_framebuffer){
    VkResult r;
    VkFramebufferCreateInfo fi;
    cwgl_backend_ctx_t* backend;
    VkImageView as[2];
    backend = ctx->backend;
    int has_color0;
    int has_depthstencil;


    fi.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fi.pNext = NULL;
    fi.flags = 0;
    fi.renderPass = renderpass;
    if(is_framebuffer){
        fi.attachmentCount = 2;
        as[0] = backend->cb_view[backend->current_image_index];
        as[1] = backend->depth.image_view;
        fi.pAttachments = as;
    }else{
        if(framebuffer_backend->rb_color0){
            as[0] = framebuffer_backend->rb_color0->image_view;
            has_color0 = 1;
        }
        if(framebuffer_backend->texture_color0){
            as[0] = framebuffer_backend->texture_color0->view;
            has_color0 = 1;
        }
        if(framebuffer_backend->rb_depth_stencil){
            as[1] = framebuffer_backend->rb_depth_stencil->image_view;
            has_depthstencil = 1;
        }
        if(framebuffer_backend->texture_depth_stencil){
            as[1] = framebuffer_backend->texture_depth_stencil->view;
            has_depthstencil = 1;
        }
        fi.pAttachments = NULL;
        if(has_color0){
            fi.pAttachments = as;
        }else if(has_depthstencil){
            fi.pAttachments = &as[1];
        }
        fi.attachmentCount = has_color0 + has_depthstencil;

    }
    fi.width = framebuffer_backend->width;
    fi.height = framebuffer_backend->height;
    fi.layers = 1;
    r = vkCreateFramebuffer(backend->device,
                            &fi,
                            NULL,
                            out_framebuffer);
    if(r != VK_SUCCESS){
        printf("Failed to create framebuffer\n");
        return -1;
    }
    return 0;
}

static int
create_renderpass(cwgl_ctx_t* ctx, 
                  const cwgl_backend_Framebuffer_t* framebuffer_backend,
                  int is_framebuffer, VkRenderPass* out_renderpass){
    VkResult r;
    int has_color = 0;
    int has_depth = 0;
    int has_stencil = 0;
    int has_depthstencil = 0;
    VkAttachmentDescription ads[2];
    VkAttachmentReference cr;
    VkAttachmentReference dr;
    VkRenderPassCreateInfo ri;
    VkSubpassDescription sp;
    VkSubpassDependency dep;
    cwgl_backend_ctx_t* backend;
    backend = ctx->backend;

    if(is_framebuffer){
        has_color = 1;
        has_depth = 1;
        has_stencil = 1;
        has_depthstencil = 1;
    }else{
        has_color = framebuffer_backend->has_color0;
        has_depth = framebuffer_backend->has_depth;
        has_stencil = framebuffer_backend->has_stencil;
        has_depthstencil = (has_depth || has_stencil) ? 1 : 0;
    }

    /* Configure color attachment */
    if(is_framebuffer){
        ads[0].format = backend->cb_format;
    }else{
        if(framebuffer_backend->texture_color0){
            ads[0].format = framebuffer_backend->texture_color0->format;
        }
        if(framebuffer_backend->rb_color0){
            ads[0].format = framebuffer_backend->rb_color0->format;
        }
    }
    ads[0].flags = 0;
    ads[0].samples = VK_SAMPLE_COUNT_1_BIT;
    ads[0].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    ads[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    ads[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    ads[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    ads[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    ads[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    /* Configure depth+stencil attachment */
    if(is_framebuffer){
        ads[1].format = backend->depth.format;
    }else{
        if(framebuffer_backend->texture_depth_stencil){
            ads[1].format = framebuffer_backend->texture_depth_stencil->format;
        }
        if(framebuffer_backend->rb_depth_stencil){
            ads[1].format = framebuffer_backend->rb_depth_stencil->format;
        }
    }
    ads[1].flags = 0;
    ads[1].samples = VK_SAMPLE_COUNT_1_BIT;
    ads[1].loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    ads[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    ads[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    ads[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
    ads[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    ads[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    /* Configure subpass */
    cr.attachment = 0;
    cr.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    dr.attachment = 1;
    dr.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    sp.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    sp.flags = 0;
    sp.inputAttachmentCount = 0;
    sp.pInputAttachments = NULL;
    sp.colorAttachmentCount = 1;
    sp.pColorAttachments = &cr;
    sp.pResolveAttachments = NULL;
    sp.pDepthStencilAttachment = &dr;
    sp.preserveAttachmentCount = 0;
    sp.pPreserveAttachments = NULL;

    dep.srcSubpass = VK_SUBPASS_EXTERNAL;
    dep.dstSubpass = 0;
    dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dep.srcAccessMask = 0;
    dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    dep.dependencyFlags = 0;

    ri.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    ri.pNext = NULL;
    ri.flags = 0;
    ri.attachmentCount = has_color + has_depthstencil;
    if(has_color){
        ri.pAttachments = ads;
    }else{
        ri.pAttachments = &ads[1];
    }

    ri.subpassCount = 1;
    ri.pSubpasses = &sp;
    ri.dependencyCount = 1;
    ri.pDependencies = &dep;

    r = vkCreateRenderPass(backend->device, &ri, NULL, out_renderpass);
    if(r != VK_SUCCESS){
        printf("Failed to create renderpass\n");
        return -1;
    }else{
        return 0;
    }
}

static void
update_framebuffer(cwgl_ctx_t* ctx, 
                   cwgl_backend_Framebuffer_t* framebuffer_backend, 
                   int is_framebuffer){
    uint64_t next_color_texture_ident0 = 0;
    uint64_t next_color_renderbuffer_ident0 = 0;
    uint64_t next_depth_stencil_texture_ident = 0;
    uint64_t next_depth_stencil_renderbuffer_ident = 0;
    uint32_t next_width = 0;
    uint32_t next_height = 0;
    int need_update = 0;
    VkRenderPass renderpass;
    VkFramebuffer framebuffer;
    cwgl_backend_ctx_t* backend;
    backend = ctx->backend;
    if(is_framebuffer){
        next_color_renderbuffer_ident0 = backend->framebuffer_ident;
        next_depth_stencil_renderbuffer_ident = backend->depth.ident;
        next_width = 1280;
        next_height = 720;
    }else{
        if(framebuffer_backend->current_ident == framebuffer_backend->configuration_ident){
            /* Early cut */
            // FIXME: Too optimized..?
            return;
        }
        if(framebuffer_backend->rb_color0){
            next_color_renderbuffer_ident0 = framebuffer_backend->rb_color0->ident;
            next_width = framebuffer_backend->rb_color0->width;
            next_height = framebuffer_backend->rb_color0->height;
        }
        if(framebuffer_backend->rb_depth_stencil){
            next_depth_stencil_renderbuffer_ident = framebuffer_backend->rb_depth_stencil->ident;
            next_width = framebuffer_backend->rb_depth_stencil->width;
            next_height = framebuffer_backend->rb_depth_stencil->height;
        }
        if(framebuffer_backend->texture_color0){
            next_color_texture_ident0 = framebuffer_backend->texture_color0->ident;
            next_width = framebuffer_backend->texture_color0->width;
            next_height = framebuffer_backend->texture_color0->height;
        }
        if(framebuffer_backend->texture_depth_stencil){
            next_depth_stencil_texture_ident = framebuffer_backend->texture_depth_stencil->ident;
            next_width = framebuffer_backend->texture_depth_stencil->width;
            next_height = framebuffer_backend->texture_depth_stencil->height;
        }
    }

    if(framebuffer_backend->allocated){
        if(framebuffer_backend->color_texture_ident0 != 
           next_color_texture_ident0){
            need_update = 1;
        }
        if(framebuffer_backend->color_renderbuffer_ident0 !=
           next_color_renderbuffer_ident0){
            need_update = 1;
        }
        if(framebuffer_backend->depth_stencil_texture_ident !=
           next_depth_stencil_texture_ident){
            need_update = 1;
        }
        if(framebuffer_backend->depth_stencil_renderbuffer_ident !=
           next_depth_stencil_renderbuffer_ident){
            need_update = 1;
        }
        if(! framebuffer_backend->allocated){
            need_update = 1;
        }
        if(! need_update){
            /* Early cut */
            framebuffer_backend->current_ident = framebuffer_backend->configuration_ident;
            return;
        }

        vkDestroyFramebuffer(backend->device, framebuffer_backend->framebuffer, NULL);
        vkDestroyRenderPass(backend->device, framebuffer_backend->renderpass, NULL);
        framebuffer_backend->allocated = 0;
    }
    framebuffer_backend->width = next_width;
    framebuffer_backend->height = next_height;
    create_renderpass(ctx, framebuffer_backend, is_framebuffer, &renderpass);
    create_framebuffer(ctx, framebuffer_backend, is_framebuffer, renderpass, &framebuffer);
    framebuffer_backend->color_texture_ident0 = next_color_texture_ident0;
    framebuffer_backend->color_renderbuffer_ident0 = next_color_renderbuffer_ident0;
    framebuffer_backend->depth_stencil_texture_ident = next_depth_stencil_texture_ident;
    framebuffer_backend->depth_stencil_renderbuffer_ident = next_depth_stencil_renderbuffer_ident;
    framebuffer_backend->framebuffer = framebuffer;
    framebuffer_backend->renderpass = renderpass;
    framebuffer_backend->allocated = 1;
    framebuffer_backend->current_ident = framebuffer_backend->configuration_ident;
    framebuffer_backend->ident = cwgl_vkpriv_newident(ctx);
}

static VkFormat
to_vk_value_format(int size, cwgl_enum_t type, int normalized){
    switch(size){
        default:
        case 1:
            switch(type){
                case BYTE:
                    if(normalized){
                        return VK_FORMAT_R8_SNORM;
                    }else{
                        return VK_FORMAT_R8_SINT;
                    }
                    break;
                case UNSIGNED_BYTE:
                    if(normalized){
                        return VK_FORMAT_R8_UNORM;
                    }else{
                        return VK_FORMAT_R8_UINT;
                    }
                    break;
                case SHORT:
                    if(normalized){
                        return VK_FORMAT_R16_SNORM;
                    }else{
                        return VK_FORMAT_R16_SINT;
                    }
                    break;
                case UNSIGNED_SHORT:
                    if(normalized){
                        return VK_FORMAT_R16_UNORM;
                    }else{
                        return VK_FORMAT_R16_UINT;
                    }
                    break;
                default:
                case FLOAT:
                    return VK_FORMAT_R32_SFLOAT;
            }
        case 2:
            switch(type){
                case BYTE:
                    if(normalized){
                        return VK_FORMAT_R8G8_SNORM;
                    }else{
                        return VK_FORMAT_R8G8_SINT;
                    }
                    break;
                case UNSIGNED_BYTE:
                    if(normalized){
                        return VK_FORMAT_R8G8_UNORM;
                    }else{
                        return VK_FORMAT_R8G8_UINT;
                    }
                    break;
                case SHORT:
                    if(normalized){
                        return VK_FORMAT_R16G16_SNORM;
                    }else{
                        return VK_FORMAT_R16G16_SINT;
                    }
                    break;
                case UNSIGNED_SHORT:
                    if(normalized){
                        return VK_FORMAT_R16G16_UNORM;
                    }else{
                        return VK_FORMAT_R16G16_UINT;
                    }
                    break;
                default:
                case FLOAT:
                    return VK_FORMAT_R32G32_SFLOAT;
            }
        case 3:
            switch(type){
                case BYTE:
                    if(normalized){
                        return VK_FORMAT_R8G8B8_SNORM;
                    }else{
                        return VK_FORMAT_R8G8B8_SINT;
                    }
                    break;
                case UNSIGNED_BYTE:
                    if(normalized){
                        return VK_FORMAT_R8G8B8_UNORM;
                    }else{
                        return VK_FORMAT_R8G8B8_UINT;
                    }
                    break;
                case SHORT:
                    if(normalized){
                        return VK_FORMAT_R16G16B16_SNORM;
                    }else{
                        return VK_FORMAT_R16G16B16_SINT;
                    }
                    break;
                case UNSIGNED_SHORT:
                    if(normalized){
                        return VK_FORMAT_R16G16B16_UNORM;
                    }else{
                        return VK_FORMAT_R16G16B16_UINT;
                    }
                    break;
                default:
                case FLOAT:
                    return VK_FORMAT_R32G32B32_SFLOAT;
            }
        case 4:
            switch(type){
                case BYTE:
                    if(normalized){
                        return VK_FORMAT_R8G8B8A8_SNORM;
                    }else{
                        return VK_FORMAT_R8G8B8A8_SINT;
                    }
                    break;
                case UNSIGNED_BYTE:
                    if(normalized){
                        return VK_FORMAT_R8G8B8A8_UNORM;
                    }else{
                        return VK_FORMAT_R8G8B8A8_UINT;
                    }
                    break;
                case SHORT:
                    if(normalized){
                        return VK_FORMAT_R16G16B16A16_SNORM;
                    }else{
                        return VK_FORMAT_R16G16B16A16_SINT;
                    }
                    break;
                case UNSIGNED_SHORT:
                    if(normalized){
                        return VK_FORMAT_R16G16B16A16_UNORM;
                    }else{
                        return VK_FORMAT_R16G16B16A16_UINT;
                    }
                    break;
                default:
                case FLOAT:
                    return VK_FORMAT_R32G32B32A32_SFLOAT;
            }
    }
}

static VkFormat
to_vk_value_format_floatreg(cwgl_enum_t type){
    switch(type){
        default:
            return 0;
        case FLOAT:
            return VK_FORMAT_R32_SFLOAT;
        case FLOAT_VEC2:
        case FLOAT_MAT2:
            return VK_FORMAT_R32G32_SFLOAT;
        case FLOAT_VEC3:
        case FLOAT_MAT3:
            return VK_FORMAT_R32G32B32_SFLOAT;
        case FLOAT_VEC4:
        case FLOAT_MAT4:
            return VK_FORMAT_R32G32B32A32_SFLOAT;
    }

}

struct cwgl_backend_pipeline_vtxbinds_s {
    VkBuffer bind_buffers[CWGL_MAX_VAO_SIZE]; /* Cache */
    VkDeviceSize bind_offsets[CWGL_MAX_VAO_SIZE]; /* Cache */
};

typedef struct cwgl_backend_pipeline_vtxbinds_s cwgl_backend_pipeline_vtxbinds_t;

static void
configure_shaders(cwgl_ctx_t* ctx, VkPipelineShaderStageCreateInfo* vxi,
                  VkPipelineShaderStageCreateInfo* pxi,
                  VkPipelineVertexInputStateCreateInfo* vii,
                  cwgl_backend_pipeline_identity_t* id,
                  cwgl_backend_pipeline_vtxbinds_t* vtx){
    int i;
    int ai;
    int bind_at;
    int use_attrib_register;
    int location;
    cwgl_Program_t* program;
    cwgl_backend_Program_t* program_backend;
    cwgl_VertexArrayObject_t* vao;
    cwgl_vao_attrib_state_t* attrib;
    cwgl_activeinfo_t* a;

    program = ctx->state.bin.CURRENT_PROGRAM;
    program_backend = program->backend;

    vao = current_vao(ctx);
    use_attrib_register = 0;
    bind_at = 0;
    a = program->state.attributes;

    for(i=0;i!=CWGL_MAX_VAO_SIZE;i++){
        ai = program->state.attriblocations[i].active_index;
        if(ai >= 0){
            location = a[ai].location;
            attrib = &vao->attrib[i];
            id->attrs[location].location = location;
            id->attrs[location].binding = bind_at;
            id->binds[bind_at].binding = bind_at;
            id->binds[bind_at].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            if(attrib->VERTEX_ATTRIB_ARRAY_ENABLED){
                id->attrs[location].format = 
                    to_vk_value_format(attrib->VERTEX_ATTRIB_ARRAY_SIZE,
                                       attrib->VERTEX_ATTRIB_ARRAY_TYPE, 
                                       attrib->VERTEX_ATTRIB_ARRAY_NORMALIZED ? 1 : 0);
                id->attrs[location].offset = 0;
                id->binds[bind_at].stride = attrib->VERTEX_ATTRIB_ARRAY_STRIDE;
                vtx->bind_buffers[bind_at] = attrib->VERTEX_ATTRIB_ARRAY_BUFFER_BINDING->backend->buffer;
                vtx->bind_offsets[bind_at] = attrib->VERTEX_ATTRIB_ARRAY_POINTER;
            }else{
                /* Use input register, already bound as uniform buffer */
                id->attrs[location].format = to_vk_value_format_floatreg(a[ai].type);
                id->attrs[location].offset = 0;
                id->binds[bind_at].stride = 0;
                vtx->bind_buffers[bind_at] = program_backend->uniform_buffer.buffer;
                vtx->bind_offsets[bind_at] = a[ai].offset;
            }
            bind_at++;
        }
    }
    id->bind_count = bind_at;

    vxi->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vxi->pNext = NULL;
    vxi->flags = 0;
    vxi->stage = VK_SHADER_STAGE_VERTEX_BIT;
    vxi->module = program_backend->vertex_shader;
    vxi->pName = "main";
    vxi->pSpecializationInfo = NULL;

    pxi->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pxi->pNext = NULL;
    pxi->flags = 0;
    pxi->stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    pxi->module = program_backend->pixel_shader;
    pxi->pName = "main";
    pxi->pSpecializationInfo = NULL;

    vii->sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vii->pNext = NULL;
    vii->flags = 0;
    vii->vertexBindingDescriptionCount = bind_at;
    vii->pVertexBindingDescriptions = id->binds;
    vii->vertexAttributeDescriptionCount = program_backend->input_count;
    vii->pVertexAttributeDescriptions = id->attrs;
}

static VkStencilOp
to_vk_stencilop(cwgl_enum_t op){
    switch(op){
        default:
        case KEEP:
            return VK_STENCIL_OP_KEEP;
        case ZERO:
            return VK_STENCIL_OP_ZERO;
        case REPLACE:
            return VK_STENCIL_OP_REPLACE;
        case INCR:
            return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
        case INCR_WRAP:
            return VK_STENCIL_OP_INCREMENT_AND_WRAP;
        case DECR:
            return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
        case DECR_WRAP:
            return VK_STENCIL_OP_DECREMENT_AND_WRAP;
        case INVERT:
            return VK_STENCIL_OP_INVERT;
    }
}
static VkCompareOp
to_vk_compareop(cwgl_enum_t op){
    switch(op){
        default:
        case NEVER:
            return VK_COMPARE_OP_NEVER;
        case LESS:
            return VK_COMPARE_OP_LESS;
        case LEQUAL:
            return VK_COMPARE_OP_LESS_OR_EQUAL;
        case GREATER:
            return VK_COMPARE_OP_GREATER;
            break;
        case NOTEQUAL:
            return VK_COMPARE_OP_NOT_EQUAL;
        case GEQUAL:
            return VK_COMPARE_OP_GREATER_OR_EQUAL;
            break;
        case ALWAYS:
            return VK_COMPARE_OP_ALWAYS;
    }
}
static VkBlendFactor
to_vk_blendfactor(cwgl_enum_t op){
    switch(op){
        default:
        case ZERO:
            return VK_BLEND_FACTOR_ZERO;
        case ONE:
            return VK_BLEND_FACTOR_ONE;
        case SRC_COLOR:
            return VK_BLEND_FACTOR_SRC_COLOR;
        case ONE_MINUS_SRC_COLOR:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
        case DST_COLOR:
            return VK_BLEND_FACTOR_DST_COLOR;
        case ONE_MINUS_DST_COLOR:
            return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
        case SRC_ALPHA:
            return VK_BLEND_FACTOR_SRC_ALPHA;
        case ONE_MINUS_SRC_ALPHA:
            return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        case DST_ALPHA:
            return VK_BLEND_FACTOR_DST_ALPHA;
        case ONE_MINUS_DST_ALPHA:
            return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        case CONSTANT_COLOR:
            return VK_BLEND_FACTOR_CONSTANT_COLOR;
        case ONE_MINUS_CONSTANT_COLOR:
            return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
        case CONSTANT_ALPHA:
            return VK_BLEND_FACTOR_CONSTANT_ALPHA;
        case ONE_MINUS_CONSTANT_ALPHA:
            return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
        case SRC_ALPHA_SATURATE:
            return VK_BLEND_FACTOR_SRC_ALPHA_SATURATE;
    }
}

static VkBlendOp
to_vk_blendop(cwgl_enum_t op){
    switch(op){
        default:
        case FUNC_ADD:
            return VK_BLEND_OP_ADD;
        case FUNC_SUBTRACT:
            return VK_BLEND_OP_SUBTRACT;
        case FUNC_REVERSE_SUBTRACT:
            return VK_BLEND_OP_REVERSE_SUBTRACT;
    }
}

static void
fill_pipeline_identity(cwgl_ctx_t* ctx,
                       cwgl_enum_t primitive,
                       cwgl_backend_pipeline_identity_t* id){

    VkPipelineRasterizationStateCreateInfo rsi = {0};
    VkPipelineMultisampleStateCreateInfo msi = {0};
    VkPipelineDepthStencilStateCreateInfo dsi = {0};
    VkPipelineInputAssemblyStateCreateInfo iai = {0};
    VkPipelineColorBlendAttachmentState cbas_color = {0};

    cwgl_ctx_global_state_t* s;
    int line = 0;
    int point = 0;
    int triangle = 0;

    /* Determine draw type */
    switch(primitive){
        case POINTS:
            point = 1;
            break;
        case LINE_STRIP:
        case LINE_LOOP:
        case LINES:
            line = 1;
            break;
        default:
        case TRIANGLE_STRIP:
        case TRIANGLE_FAN:
        case TRIANGLES:
            triangle = 1;
            break;
    }

    s = &ctx->state.glo;

    rsi.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rsi.pNext = NULL;
    rsi.flags = 0;
    rsi.depthClampEnable = VK_FALSE; // FIXME: ???
    rsi.rasterizerDiscardEnable = VK_FALSE;
    if(line){
        rsi.polygonMode = VK_POLYGON_MODE_LINE;
    }else if(point){
        rsi.polygonMode = VK_POLYGON_MODE_POINT;
    }else{
        rsi.polygonMode = VK_POLYGON_MODE_FILL;
    }
    if(triangle){
        if(s->CULL_FACE){
            switch(s->CULL_FACE_MODE){
                case FRONT:
                    rsi.cullMode = VK_CULL_MODE_FRONT_BIT;
                    break;
                case BACK:
                    rsi.cullMode = VK_CULL_MODE_BACK_BIT;
                    break;
                case FRONT_AND_BACK:
                    rsi.cullMode = VK_CULL_MODE_FRONT_AND_BACK;
                    break;
                default:
                    rsi.cullMode = VK_CULL_MODE_NONE;
                    break;
            }
            switch(s->FRONT_FACE){
                case CW:
                    rsi.frontFace = VK_FRONT_FACE_CLOCKWISE;
                    break;
                case CCW:
                    rsi.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
                    break;
                default:
                    rsi.frontFace = 0; /* CCW */
                    break;
            }
        }else{
            rsi.cullMode = VK_CULL_MODE_NONE;
            rsi.frontFace = 0; /* CCW */
        }
    }else{
        rsi.cullMode = VK_CULL_MODE_NONE;
        rsi.frontFace = 0; /* CCW */
    }
    if(s->POLYGON_OFFSET_FILL){
        rsi.depthBiasEnable = VK_TRUE;
        // FIXME: Is this OK?
        rsi.depthBiasConstantFactor = s->POLYGON_OFFSET_UNITS;
        rsi.depthBiasClamp = 0.0; /* Dynamic state VK_DYNAMIC_STATE_DEPTH_BIAS */
        rsi.depthBiasSlopeFactor = s->POLYGON_OFFSET_FACTOR;
    }else{
        rsi.depthBiasEnable = VK_FALSE;
        rsi.depthBiasConstantFactor = 0.0;
        rsi.depthBiasClamp = 0.0; /* Dynamic state VK_DYNAMIC_STATE_DEPTH_BIAS */
        rsi.depthBiasSlopeFactor = 0.0;
    }
    // FIXME: Ignore line width here.
    rsi.lineWidth = 1.0f;

    // FIXME: Implement multisample
    msi.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    msi.pNext = NULL;
    msi.flags = 0;
    msi.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    msi.sampleShadingEnable = VK_FALSE;
    msi.minSampleShading = 1.0f;
    msi.pSampleMask = NULL;
    msi.alphaToCoverageEnable = s->SAMPLE_ALPHA_TO_COVERAGE ? VK_TRUE : VK_FALSE;
    msi.alphaToOneEnable = VK_FALSE;

    dsi.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    dsi.pNext = NULL;
    dsi.flags = 0;
    dsi.depthTestEnable = s->DEPTH_TEST ? VK_TRUE : VK_FALSE;
    dsi.depthWriteEnable = s->DEPTH_WRITEMASK ? VK_TRUE : VK_FALSE;
    dsi.depthCompareOp = to_vk_compareop(s->DEPTH_FUNC);
    dsi.depthBoundsTestEnable = VK_FALSE;
    dsi.stencilTestEnable = s->STENCIL_TEST ? VK_TRUE : VK_FALSE;
    dsi.front.failOp = to_vk_stencilop(s->STENCIL_FAIL);
    dsi.front.passOp = to_vk_stencilop(s->STENCIL_PASS_DEPTH_PASS);
    dsi.front.depthFailOp = to_vk_stencilop(s->STENCIL_PASS_DEPTH_FAIL);
    dsi.front.compareOp = to_vk_compareop(s->STENCIL_FUNC);
    dsi.front.compareMask = s->STENCIL_VALUE_MASK;
    dsi.front.writeMask = s->STENCIL_WRITEMASK;
    dsi.front.reference = s->STENCIL_REF;
    dsi.back.failOp = to_vk_stencilop(s->STENCIL_BACK_FAIL);
    dsi.back.passOp = to_vk_stencilop(s->STENCIL_BACK_PASS_DEPTH_PASS);
    dsi.back.depthFailOp = to_vk_stencilop(s->STENCIL_BACK_PASS_DEPTH_FAIL);
    dsi.back.compareOp = to_vk_compareop(s->STENCIL_BACK_FUNC);
    dsi.back.compareMask = s->STENCIL_BACK_VALUE_MASK;
    dsi.back.writeMask = s->STENCIL_BACK_WRITEMASK;
    dsi.back.reference = s->STENCIL_BACK_REF;
    dsi.minDepthBounds = 0.0f;
    dsi.maxDepthBounds = 1.0f;

    cbas_color.blendEnable = s->BLEND ? VK_TRUE : VK_FALSE;
    cbas_color.srcColorBlendFactor = to_vk_blendfactor(s->BLEND_SRC_RGB);
    cbas_color.dstColorBlendFactor = to_vk_blendfactor(s->BLEND_DST_RGB);
    cbas_color.colorBlendOp = to_vk_blendop(s->BLEND_EQUATION_RGB);
    cbas_color.srcAlphaBlendFactor = to_vk_blendfactor(s->BLEND_SRC_ALPHA);
    cbas_color.dstAlphaBlendFactor = to_vk_blendfactor(s->BLEND_DST_ALPHA);
    cbas_color.alphaBlendOp = to_vk_blendop(s->BLEND_EQUATION_ALPHA);
    cbas_color.colorWriteMask = 0;
    if(s->COLOR_WRITEMASK[0]){
        cbas_color.colorWriteMask |= VK_COLOR_COMPONENT_R_BIT;
    }
    if(s->COLOR_WRITEMASK[1]){
        cbas_color.colorWriteMask |= VK_COLOR_COMPONENT_G_BIT;
    }
    if(s->COLOR_WRITEMASK[2]){
        cbas_color.colorWriteMask |= VK_COLOR_COMPONENT_B_BIT;
    }
    if(s->COLOR_WRITEMASK[3]){
        cbas_color.colorWriteMask |= VK_COLOR_COMPONENT_A_BIT;
    }

    id->blend_color[0] = s->BLEND_COLOR[0];
    id->blend_color[1] = s->BLEND_COLOR[1];
    id->blend_color[2] = s->BLEND_COLOR[2];
    id->blend_color[3] = s->BLEND_COLOR[3];

    iai.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    iai.pNext = NULL;
    iai.flags = 0;
    switch(primitive){
        default:
        case POINTS:
            iai.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
            break;
        case LINE_STRIP:
            iai.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
            break;
        case LINE_LOOP:
            // FIXME: ???
            iai.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
            break;
        case LINES:
            iai.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
            break;
        case TRIANGLE_STRIP:
            iai.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
            break;
        case TRIANGLE_FAN:
            iai.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
            break;
        case TRIANGLES:
            iai.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            break;
    }
    iai.primitiveRestartEnable = VK_FALSE; // FIXME: For WebGL2

    id->rsi = rsi;
    id->msi = msi;
    id->dsi = dsi;
    id->iai = iai;
    id->cbas_color = cbas_color;
}

static cwgl_backend_pipeline_t*
lookup_pipeline(cwgl_backend_ctx_t* backend,
                cwgl_backend_pipeline_identity_t* query,
                int* out_hit){
    int first_idx;
    uint64_t first_ident = UINT64_MAX;
    int i;
    for(i=0;i!=CWGL_PIPELINE_CACHE_SIZE;i++){
        if(backend->pipelines[i].allocated){
            if(! memcmp(&backend->pipelines[i].cache, query,
                        sizeof(cwgl_backend_pipeline_identity_t))){
                break;
            }else{
                if(first_ident > backend->pipelines[i].ident){
                    first_ident = backend->pipelines[i].ident;
                    first_idx = i;
                }
            }
        }else{
            /* Pick as free area */
            first_ident = 0;
            first_idx = i;
        }
    }
    if(i==CWGL_PIPELINE_CACHE_SIZE){
        *out_hit = 0;
        if(backend->pipelines[first_idx].allocated){
            vkDestroyPipeline(backend->device,
                              backend->pipelines[first_idx].pipeline,
                              NULL);
            printf("Pipeline destroy at %d\n",first_idx);
        }else{
            printf("Pipeline new at %d\n",first_idx);
        }
        *out_hit = 0;
        return &backend->pipelines[first_idx];
    }else{
        //printf("Pipeline cache hit at %d\n", i);
        *out_hit = 1;
        return &backend->pipelines[i];
    }
}

static int
create_pipeline(cwgl_ctx_t* ctx, cwgl_enum_t primitive, 
                cwgl_backend_Framebuffer_t* fb,
                cwgl_backend_Program_t* prog,
                cwgl_backend_pipeline_t** out_ppipe,
                cwgl_backend_pipeline_vtxbinds_t* out_vtx){
    int cache_hit = 0;
    VkPipeline pipeline;
    VkRenderPass renderpass; 
    VkPipelineLayout layout;
    cwgl_backend_pipeline_identity_t id = {0};
    cwgl_backend_pipeline_t* p;

    VkGraphicsPipelineCreateInfo pi;
    VkPipelineShaderStageCreateInfo ssci[2];
    VkPipelineVertexInputStateCreateInfo vii;

    VkPipelineViewportStateCreateInfo vsi;
    VkPipelineColorBlendStateCreateInfo cbi;
    VkPipelineDynamicStateCreateInfo dyi;
    VkDynamicState dys[2];
    cwgl_backend_ctx_t* backend;
    VkResult r;
    backend = ctx->backend;

    fill_pipeline_identity(ctx, primitive, &id);
    configure_shaders(ctx, &ssci[0], &ssci[1], &vii, &id, out_vtx);
    id.framebuffer_ident = fb->ident;
    id.program_ident = prog->ident;

    p = lookup_pipeline(backend, &id, &cache_hit);
    *out_ppipe = p;
    if(cache_hit){
        return 0;
    }

    renderpass = fb->renderpass;
    layout = prog->pipeline_layout;

    vsi.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    vsi.pNext = NULL;
    vsi.flags = 0;
    vsi.viewportCount = 1;
    vsi.pScissors = NULL;
    vsi.scissorCount = 1;
    vsi.pViewports = NULL;

    cbi.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    cbi.pNext = NULL;
    cbi.flags = 0;
    cbi.logicOpEnable = VK_FALSE;
    cbi.logicOp = VK_LOGIC_OP_COPY;
    cbi.attachmentCount = 1;
    cbi.pAttachments = &id.cbas_color;
    cbi.blendConstants[0] = id.blend_color[0];
    cbi.blendConstants[1] = id.blend_color[1];
    cbi.blendConstants[2] = id.blend_color[2];
    cbi.blendConstants[3] = id.blend_color[3];

    dys[0] = VK_DYNAMIC_STATE_VIEWPORT;
    dys[1] = VK_DYNAMIC_STATE_SCISSOR;

    dyi.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dyi.pNext = NULL;
    dyi.flags = 0;
    dyi.dynamicStateCount = 2;
    dyi.pDynamicStates = dys;

    pi.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pi.pNext = NULL;
    pi.flags = 0;
    pi.stageCount = 2;
    pi.pStages = ssci;
    pi.pVertexInputState = &vii;
    pi.pInputAssemblyState = &id.iai;
    pi.pTessellationState = NULL;
    pi.pViewportState = &vsi;
    pi.pRasterizationState = &id.rsi;
    pi.pMultisampleState = &id.msi;
    pi.pDepthStencilState = &id.dsi;
    pi.pColorBlendState = &cbi;
    pi.pDynamicState = &dyi;
    pi.layout = layout;
    pi.renderPass = renderpass;
    pi.subpass = 0;
    pi.basePipelineHandle = 0;
    pi.basePipelineIndex = 0;
    
    r = vkCreateGraphicsPipelines(backend->device,
                                  backend->pipeline_cache,
                                  1,
                                  &pi,
                                  NULL,
                                  &pipeline);
    if(r != VK_SUCCESS){
        printf("Failed to create pipeline\n");
        return -1;
    }
    p->allocated = 1;
    p->ident = cwgl_vkpriv_newident(ctx);
    p->pipeline = pipeline;
    memset(&p->cache, 0, sizeof(cwgl_backend_pipeline_identity_t));
    p->cache = id;
    return 0;
}

static void
begin_cmd(cwgl_ctx_t* ctx){
    VkCommandBufferBeginInfo bi;
    cwgl_backend_ctx_t* backend;
    backend = ctx->backend;
    bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    bi.pNext = NULL;
    bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    bi.pInheritanceInfo = NULL;
    vkBeginCommandBuffer(backend->command_buffer, &bi);
}

static void
begin_renderpass(cwgl_ctx_t* ctx, cwgl_backend_Framebuffer_t* framebuffer_backend,
                 VkRenderPass renderpass, VkFramebuffer framebuffer){
    VkRenderPassBeginInfo bi;
    cwgl_backend_ctx_t* backend;
    backend = ctx->backend;
    bi.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    bi.pNext = NULL;
    bi.renderPass = renderpass;
    bi.framebuffer = framebuffer;
    bi.renderArea.offset.x = 0;
    bi.renderArea.offset.y = 0;
    bi.renderArea.extent.width = framebuffer_backend->width;
    bi.renderArea.extent.height = framebuffer_backend->height;
    bi.clearValueCount = 0;
    bi.pClearValues = 0;
    vkCmdBeginRenderPass(backend->command_buffer, &bi, VK_SUBPASS_CONTENTS_INLINE);
}

static void
transfer_opaques(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    cwgl_backend_Program_t* program_backend;
    cwgl_backend_ctx_t* backend;
    shxm_program_t* p;
    cwgl_activeinfo_t* u;
    cwgl_uniformcontent_t* uc;

    VkDescriptorImageInfo di;
    VkWriteDescriptorSet ws;
    cwgl_Texture_t* texture;
    int32_t sampler_id;
    int i;

    backend = ctx->backend;
    program_backend = program->backend;
    p = program_backend->program;
    u = program->state.uniforms;
    uc = program->state.uniformcontents;
    /* Fill in Opaques */
    for(i=0;i!=program->state.ACTIVE_UNIFORMS;i++){
        switch(u[i].type){
            default:
                /* Do nothing */
                break;
            /* Samplers */
            case SAMPLER_2D:
            case SAMPLER_CUBE:
                sampler_id = uc[u[i].offset].asInt;
                if(u[i].type == SAMPLER_2D){
                    texture = current_texture(ctx, sampler_id, TEXTURE_2D);
                }else{
                    texture = current_texture(ctx, sampler_id, TEXTURE_CUBE_MAP);
                }
                update_texture(ctx, texture);
                di.sampler = texture->backend->sampler;
                di.imageView = texture->backend->view;
                di.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                ws.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ws.pNext = NULL;
                ws.dstSet = program_backend->desc_set;
                ws.dstBinding = u[i].location;
                ws.dstArrayElement = 0;
                ws.descriptorCount = 1;
                ws.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                ws.pImageInfo = &di;
                ws.pBufferInfo = NULL;
                ws.pTexelBufferView = NULL;
                vkUpdateDescriptorSets(backend->device, 1, &ws, 0, NULL);
                break;
        }
    }
}

static void
transfer_uniforms(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    VkResult r;
    cwgl_backend_Program_t* program_backend;
    cwgl_backend_ctx_t* backend;
    uint8_t* device_memory_addr;
    shxm_program_t* p;
    cwgl_activeinfo_t* u;
    cwgl_uniformcontent_t* uc;

    backend = ctx->backend;
    program_backend = program->backend;
    p = program_backend->program;
    u = program->state.uniforms;
    uc = program->state.uniformcontents;

    r = vkMapMemory(backend->device, program_backend->uniform_buffer.device_memory,
                    0, program_backend->program->uniform_size, 0, &device_memory_addr);
    if(r != VK_SUCCESS){
        printf("Failed to map uniform buffer\n");
        return;
    }
    /* Fill in Uniforms */
    memcpy(device_memory_addr, program->state.uniformcontents,
           program_backend->program->uniform_size);
    vkUnmapMemory(backend->device, program_backend->uniform_buffer.device_memory);
}


int
cwgl_backend_drawArrays(cwgl_ctx_t* ctx, cwgl_enum_t mode,
                        int32_t first, uint32_t count){
    return 0;
}


int
cwgl_backend_drawElements(cwgl_ctx_t* ctx, cwgl_enum_t mode,
                          uint32_t count, cwgl_enum_t type, uint32_t offset){
    VkRenderPass renderpass;
    VkFramebuffer framebuffer;
    int is_framebuffer;
    cwgl_backend_Framebuffer_t* framebuffer_backend;
    cwgl_VertexArrayObject_t* vao;
    cwgl_Program_t* program;
    cwgl_backend_Program_t* program_backend;
    cwgl_backend_ctx_t* backend;
    cwgl_ctx_global_state_t* s;
    cwgl_backend_pipeline_vtxbinds_t vtxbinds;
    cwgl_backend_pipeline_t* pipeline_backend;
    s = &ctx->state.glo;
    backend = ctx->backend;
    program = ctx->state.bin.CURRENT_PROGRAM;
    program_backend = program->backend;
    is_framebuffer = ctx->state.bin.FRAMEBUFFER_BINDING ? 0 : 1;
    framebuffer_backend = is_framebuffer ?
        &backend->default_fb :
        ctx->state.bin.FRAMEBUFFER_BINDING->backend;
    transfer_uniforms(ctx, program);
    transfer_opaques(ctx, program);
    update_framebuffer(ctx, framebuffer_backend, is_framebuffer);
    renderpass = framebuffer_backend->renderpass;
    framebuffer = framebuffer_backend->framebuffer;
    create_pipeline(ctx, mode, framebuffer_backend, program_backend, &pipeline_backend, &vtxbinds);
    begin_cmd(ctx);
    begin_renderpass(ctx, framebuffer_backend, renderpass, framebuffer);
    vkCmdBindPipeline(backend->command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline_backend->pipeline);
    vkCmdBindDescriptorSets(backend->command_buffer,
                            VK_PIPELINE_BIND_POINT_GRAPHICS,
                            program_backend->pipeline_layout,
                            0,
                            1,
                            &program_backend->desc_set,
                            0,
                            NULL);
    vkCmdBindVertexBuffers(backend->command_buffer, 
                           0,
                           pipeline_backend->cache.bind_count,
                           vtxbinds.bind_buffers,
                           vtxbinds.bind_offsets);
    /* Setup Viewport and Scissor */
    {
        VkViewport vp;
        VkRect2D sc;
        vp.x = s->VIEWPORT[0];
        vp.y = s->VIEWPORT[3] - s->VIEWPORT[1];
        vp.width = s->VIEWPORT[2];
        vp.height = -s->VIEWPORT[3];
        vp.minDepth = s->DEPTH_RANGE[0];
        vp.maxDepth = s->DEPTH_RANGE[1];
        if(s->SCISSOR_TEST){
            sc.offset.x = s->SCISSOR_BOX[0];
            sc.offset.y = (s->VIEWPORT[3] - s->SCISSOR_BOX[1]) - s->SCISSOR_BOX[3];
            sc.extent.width = s->SCISSOR_BOX[2];
            sc.extent.height = s->SCISSOR_BOX[3];
        }else{
            sc.offset.x = 0;
            sc.offset.y = 0;
            sc.extent.width = s->VIEWPORT[2];
            sc.extent.height = s->VIEWPORT[3];
        }
        vkCmdSetViewport(backend->command_buffer,
                         0, 1, &vp);
        vkCmdSetScissor(backend->command_buffer,
                        0, 1, &sc);
    }
    /* Draw */
    {
        vao = current_vao(ctx);
        vkCmdBindIndexBuffer(backend->command_buffer,
                             vao->state.ELEMENT_ARRAY_BUFFER_BINDING->backend->buffer,
                             offset,
                             VK_INDEX_TYPE_UINT16);
        vkCmdDrawIndexed(backend->command_buffer,
                         count, 1, 0, 0, 0);
    }
    vkCmdEndRenderPass(backend->command_buffer);
    vkEndCommandBuffer(backend->command_buffer);
    backend->queue_has_command = 1; // FIXME: Tentative
    cwgl_vkpriv_graphics_submit(ctx);
    cwgl_vkpriv_graphics_wait(ctx);
    return 0;
}

#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_COLOR_BUFFER_BIT 0x00004000

static void
cmd_barrier(cwgl_ctx_t* ctx, VkImage image, VkImageLayout from, VkImageLayout to,
            VkImageAspectFlags aspect){
    cwgl_backend_ctx_t* backend;
    VkImageMemoryBarrier ib;
    backend = ctx->backend;
    ib.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    ib.pNext = NULL;
    ib.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    ib.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
    ib.oldLayout = from;
    ib.newLayout = to;
    ib.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    ib.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    ib.subresourceRange.aspectMask = aspect;
    ib.subresourceRange.baseMipLevel = 0;
    ib.subresourceRange.levelCount = 1;
    ib.subresourceRange.baseArrayLayer = 0;
    ib.subresourceRange.layerCount = 1;
    ib.image = image;

    vkCmdPipelineBarrier(backend->command_buffer,
                         VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0,
                         NULL, 0, NULL, 1, &ib);
}

int
cwgl_backend_clear(cwgl_ctx_t* ctx, uint32_t mask){
    // FIXME: Consider SCISSOR_TEST and its rect
    // FIXME: Implement mipmap/cubemap clear
    /* Outside of renderpass */
    VkClearColorValue c;
    VkClearDepthStencilValue d;
    VkImageSubresourceRange r;
    VkImage img = 0;
    int is_framebuffer;
    cwgl_backend_Framebuffer_t* fb;

    cwgl_backend_ctx_t* backend;
    cwgl_ctx_global_state_t* s;
    backend = ctx->backend;
    s = &ctx->state.glo;

    if(!(mask & (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT))){
        /* Early cut */
        return 0;
    }
    if(ctx->state.bin.FRAMEBUFFER_BINDING){
        is_framebuffer = 0;
        fb = ctx->state.bin.FRAMEBUFFER_BINDING->backend;
    }else{
        is_framebuffer = 1;
        fb = &backend->default_fb;
    }
    begin_cmd(ctx);
    if(mask & GL_COLOR_BUFFER_BIT){
        c.float32[0] = s->COLOR_CLEAR_VALUE[0];
        c.float32[1] = s->COLOR_CLEAR_VALUE[1];
        c.float32[2] = s->COLOR_CLEAR_VALUE[2];
        c.float32[3] = s->COLOR_CLEAR_VALUE[3];
        r.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        r.baseMipLevel = 0;
        r.levelCount = 1;
        r.baseArrayLayer = 0;
        r.layerCount = 1;
        if(is_framebuffer){
            img = backend->cb[backend->current_image_index];
        }else{
            if(fb->rb_color0){
                img = fb->rb_color0->image;
            }else if(fb->texture_color0){
                img = fb->texture_color0->image;
            }
        }
        cmd_barrier(ctx, img,
                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    VK_IMAGE_ASPECT_COLOR_BIT);
        vkCmdClearColorImage(backend->command_buffer,
                             img,
                             VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                             &c,
                             1,
                             &r);
        cmd_barrier(ctx, img,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                    VK_IMAGE_ASPECT_COLOR_BIT);
    }
    //mask |= (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    if(mask & (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT)){
        d.depth = s->DEPTH_CLEAR_VALUE;
        d.stencil = s->STENCIL_CLEAR_VALUE;
        r.aspectMask = 0;
        if(mask & GL_DEPTH_BUFFER_BIT){
            r.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
        }
        if(mask & GL_STENCIL_BUFFER_BIT){
            r.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
        r.baseMipLevel = 0;
        r.levelCount = 1;
        r.baseArrayLayer = 0;
        r.layerCount = 1;
        if(is_framebuffer){
            img = backend->depth.image;
        }else{
            if(fb->rb_depth_stencil){
                img = fb->rb_depth_stencil->image;
            }else if(fb->texture_depth_stencil){
                img = fb->texture_depth_stencil->image;
            }
        }
        cmd_barrier(ctx, img,
                    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    r.aspectMask);
        vkCmdClearDepthStencilImage(backend->command_buffer,
                                    img,
                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                    &d,
                                    1,
                                    &r);
        cmd_barrier(ctx, img,
                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                    r.aspectMask);
    }
    backend->queue_has_command = 1; // FIXME: Tentative
    vkEndCommandBuffer(backend->command_buffer);
    cwgl_vkpriv_graphics_submit(ctx);
    cwgl_vkpriv_graphics_wait(ctx);

    return 0;
}

