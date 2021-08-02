#include "cwgl-backend-vk-priv.h"
#include <string.h>
#include <stdio.h>

void 
cwgl_vkpriv_destroy_texture(cwgl_ctx_t* ctx, cwgl_backend_Texture_t* texture_backend){
    vkDestroyImage(ctx->backend->device, texture_backend->image, NULL);
    vkFreeMemory(ctx->backend->device, texture_backend->device_memory, NULL);
    texture_backend->allocated = 0;
}

static cwgl_Texture_t*
current_texture(cwgl_ctx_t* ctx, cwgl_enum_t target){
    int id;
    cwgl_texture_unit_state_t* s;
    id = (int)ctx->state.glo.ACTIVE_TEXTURE - (int)TEXTURE0;
    s = &ctx->state.bin.texture_unit[id];
    if(target == TEXTURE_2D){
        return s->TEXTURE_BINDING_2D;
    }else{
        return s->TEXTURE_BINDING_CUBE_MAP;
    }
}

// From Vulkan 1.1 spec
int32_t 
cwgl_vkpriv_select_memory_type(cwgl_ctx_t* ctx, uint32_t requirement,
                                       VkMemoryPropertyFlags request){
    VkPhysicalDeviceMemoryProperties* props;
    int count;
    int i;
    props = &ctx->backend->memory_properties;
    count = props->memoryTypeCount;
    for(i=0;i!=count;i++){
        const uint32_t memoryTypeBits = (1 << i);
        const int isRequiredMemoryType = requirement & memoryTypeBits;
        const VkMemoryPropertyFlags properties =
            props->memoryTypes[i].propertyFlags;
        const int hasRequiredProperties = 
            (properties & request) == request;
        if(isRequiredMemoryType && hasRequiredProperties){
            return i;
        }
    }
    return -1;
}

static VkFormat
to_vulkan_format(cwgl_enum_t format, cwgl_enum_t type){
    switch(type){
        case UNSIGNED_BYTE:
            switch(format){
                case ALPHA:
                    return VK_FORMAT_R8_UNORM;
                case RGB:
                case RGBA:
                    return VK_FORMAT_R8G8B8A8_UNORM;
                case LUMINANCE:
                    return VK_FORMAT_R8_UNORM;
                case LUMINANCE_ALPHA:
                default:
                    return VK_FORMAT_UNDEFINED;
            }
            break;
        case UNSIGNED_SHORT_5_6_5:
            switch(format){
                case ALPHA:
                case RGB:
                case RGBA:
                case LUMINANCE:
                case LUMINANCE_ALPHA:
                    return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
                default:
                    return VK_FORMAT_UNDEFINED;
            }
            break;
        case UNSIGNED_SHORT_4_4_4_4:
            switch(format){
                case ALPHA:
                case RGB:
                case RGBA:
                case LUMINANCE:
                case LUMINANCE_ALPHA:
                    return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
                default:
                    return VK_FORMAT_UNDEFINED;
            }
            break;
        case UNSIGNED_SHORT_5_5_5_1:
            switch(format){
                case ALPHA:
                case RGB:
                case RGBA:
                case LUMINANCE:
                case LUMINANCE_ALPHA:
                default:
                    return VK_FORMAT_UNDEFINED;
            }
            break;
        default:
            return VK_FORMAT_UNDEFINED;
    }
}

int
cwgl_backend_texImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target,
                        int32_t level, cwgl_enum_t internalformat,
                        uint32_t width, uint32_t height, int32_t border,
                        cwgl_enum_t format, cwgl_enum_t type,
                        const void* buf, size_t buflen){
    // FIXME: Optimize for updating texture
    cwgl_Texture_t* texture;
    cwgl_backend_ctx_t* backend;
    cwgl_backend_Texture_t* texture_backend;
    int i;
    VkResult r;
    VkBufferCreateInfo temp_bi;
    VkMemoryAllocateInfo temp_ai;
    VkBuffer temp_buffer;
    VkDeviceMemory temp_device_memory;
    VkImageCreateInfo ii;
    VkMemoryRequirements memory_requirements;
    VkMemoryAllocateInfo ai;
    VkImage image;
    VkDeviceMemory device_memory;
    void* temp_device_memory_addr;
    backend = ctx->backend;
    texture = current_texture(ctx, target);
    texture_backend = texture->backend;

    // FIXME: Support CubeMap

    if(texture_backend->allocated){
        cwgl_vkpriv_graphics_wait(ctx);
        cwgl_vkpriv_destroy_texture(ctx, texture_backend);
    }
    /* Allocate temporary buffer */
    temp_bi.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    temp_bi.pNext = NULL;
    temp_bi.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    temp_bi.size = buflen;
    temp_bi.queueFamilyIndexCount = 1;
    temp_bi.pQueueFamilyIndices = &backend->queue_family_index;
    temp_bi.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    temp_bi.flags = 0;
    r = vkCreateBuffer(backend->device, &temp_bi, NULL, &temp_buffer);
    if(r != VK_SUCCESS){
        printf("FAILed to create temp_buffer\n");
        return -1;
    }
    temp_ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    temp_ai.pNext = NULL;
    i = cwgl_vkpriv_select_memory_type(ctx, UINT32_MAX,
                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | 
                                       VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    if(i < 0){
        printf("Could not find appropriate buffer type\n");
        return -1;
    }
    temp_ai.memoryTypeIndex = i;
    temp_ai.allocationSize = buflen;
    r = vkAllocateMemory(backend->device, &temp_ai, NULL, &temp_device_memory);
    if(r != VK_SUCCESS){
        printf("FAILed to allocate temp_buffer\n");
        return -1;
    }
    /* Upload bits to temporary buffer */
    r = vkMapMemory(backend->device, temp_device_memory, 0,
                    buflen, 0, &temp_device_memory_addr);
    if(r != VK_SUCCESS){
        printf("FAILed to map temp_buffer\n");
        return -1;
    }
    memcpy(temp_device_memory_addr, buf, buflen);
    vkUnmapMemory(backend->device, temp_device_memory);
    r = vkBindBufferMemory(backend->device, 
                           temp_buffer, temp_device_memory, 0);
    if(r != VK_SUCCESS){
        printf("FAILed to bind temp_buffer\n");
        return -1;
    }

    /* Create image */
    ii.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    ii.pNext = NULL;
    ii.flags = 0;
    ii.imageType = VK_IMAGE_TYPE_2D;
    ii.format = to_vulkan_format(format, type);
    ii.extent.width = width;
    ii.extent.height = height;
    ii.extent.depth = 1;
    ii.mipLevels = 1;
    ii.arrayLayers = 1;
    ii.samples = VK_SAMPLE_COUNT_1_BIT;
    ii.tiling = VK_IMAGE_TILING_OPTIMAL;
    ii.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    ii.queueFamilyIndexCount = 1;
    ii.pQueueFamilyIndices = &backend->queue_family_index;
    ii.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    ii.usage =  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    r = vkCreateImage(backend->device, &ii, NULL, &image);
    if(r != VK_SUCCESS){
        printf("FAILed to create image\n");
        return -1;
    }
    vkGetImageMemoryRequirements(backend->device, image, &memory_requirements);
    ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    ai.pNext = NULL;
    ai.allocationSize = memory_requirements.size;
    ai.memoryTypeIndex = 
        cwgl_vkpriv_select_memory_type(ctx,
                                       memory_requirements.memoryTypeBits,
                                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if(r = vkAllocateMemory(backend->device, &ai, NULL, &device_memory)){
        printf("FAILed to allocate image memory");
        return -1;
    }
    vkBindImageMemory(backend->device, image, device_memory, 0);

    /* Convert format */
    { /* Tentative */
        VkBufferImageCopy rgn;
        rgn.bufferOffset = 0;
        rgn.bufferRowLength = 0;
        rgn.bufferImageHeight = 0;
        rgn.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        rgn.imageSubresource.mipLevel = 0;
        rgn.imageSubresource.baseArrayLayer = 0;
        rgn.imageSubresource.layerCount = 1;
        rgn.imageOffset.x = rgn.imageOffset.y = rgn.imageOffset.z = 0;
        rgn.imageExtent.width = width;
        rgn.imageExtent.height = height;
        rgn.imageExtent.depth = 1;

        VkCommandBufferBeginInfo bi;
        bi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        bi.pNext = NULL;
        bi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(backend->command_buffer, &bi);
        vkCmdCopyBufferToImage(backend->command_buffer,
                               temp_buffer,
                               image,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                               1,
                               &rgn);
        vkEndCommandBuffer(backend->command_buffer);
        backend->queue_has_command = 1; // FIXME: Tentative
        cwgl_vkpriv_graphics_submit(ctx);
        cwgl_vkpriv_graphics_wait(ctx);
    }
    /* Destroy temp_buffer */
    texture_backend->allocated = 1;
    texture_backend->image = image;
    texture_backend->device_memory = device_memory;
    vkDestroyBuffer(backend->device, temp_buffer, NULL);
    vkFreeMemory(backend->device, temp_device_memory, NULL);

    return 0;
}

int
cwgl_backend_copyTexImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target,
                            int32_t level, cwgl_enum_t internalformat,
                            int32_t x, int32_t y,
                            uint32_t width, uint32_t height, int32_t border){
    return 0;
}

int
cwgl_backend_texSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target,
                           int32_t level, int32_t xoffset, int32_t yoffset,
                           uint32_t width, uint32_t height,
                           cwgl_enum_t format, cwgl_enum_t type,
                           const void* buf, size_t buflen){
    return 0;
}

int
cwgl_backend_copyTexSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target,
                               int32_t level, int32_t xoffset, int32_t yoffset,
                               int32_t x, int32_t y,
                               uint32_t width, uint32_t height){
    return 0;
}

int
cwgl_backend_compressedTexImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target,
                                  int32_t level, cwgl_enum_t internalformat,
                                  uint32_t width, uint32_t height,
                                  int32_t border,
                                  const void* buf, size_t buflen){
    return 0;
}

int
cwgl_backend_compressedTexSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target,
                                     int32_t level,
                                     int32_t xoffset, int32_t yoffset,
                                     uint32_t width, uint32_t height,
                                     cwgl_enum_t format,
                                     const void* buf, size_t buflen){
    return 0;
}

