#include "cwgl-backend-vk-priv.h"
#include <stdio.h>
#include <string.h>

// FIXME: Copied from s2
static cwgl_VertexArrayObject_t*
current_vao(cwgl_ctx_t* ctx){
    if(ctx->state.bin.VERTEX_ARRAY_BINDING){
        return ctx->state.bin.VERTEX_ARRAY_BINDING;
    }else{
        return ctx->state.default_vao;
    }
}

static cwgl_Buffer_t*
current_buffer(cwgl_ctx_t* ctx, cwgl_enum_t target){
    cwgl_VertexArrayObject_t* vao;
    switch(target){
        default:
        case ARRAY_BUFFER:
            return ctx->state.bin.ARRAY_BUFFER_BINDING;
        case ELEMENT_ARRAY_BUFFER:
            vao = current_vao(ctx);
            return vao->state.ELEMENT_ARRAY_BUFFER_BINDING;
    }
}

void
cwgl_vkpriv_destroy_buffer(cwgl_ctx_t* ctx, cwgl_backend_Buffer_t* buffer_backend){
    if(buffer_backend->allocated){
        vkDestroyBuffer(ctx->backend->device, buffer_backend->buffer, NULL);
        vkFreeMemory(ctx->backend->device, buffer_backend->device_memory, NULL);
        buffer_backend->allocated = 0;
    }
}

int
cwgl_backend_bufferData(cwgl_ctx_t* ctx, cwgl_enum_t target,
                        uint32_t size, void* data, cwgl_enum_t usage){
    cwgl_Buffer_t* buffer;
    cwgl_backend_ctx_t* backend;
    cwgl_backend_Buffer_t* buffer_backend;
    VkBuffer newbuffer;
    VkDeviceMemory device_memory;
    void* device_memory_addr;
    VkBufferCreateInfo bi;
    VkMemoryAllocateInfo ai;
    VkResult r;
    int i;
    int createbuffer;

    buffer = current_buffer(ctx, target);
    backend = ctx->backend;
    buffer_backend = buffer->backend;
    createbuffer = 1;
    if(buffer_backend->allocated){
        if(buffer->state.BUFFER_SIZE != size){
            cwgl_vkpriv_graphics_wait(ctx);
            cwgl_vkpriv_destroy_buffer(ctx, buffer_backend);
            createbuffer = 1;
        }else{
            createbuffer = 0;
        }
    }

    if(createbuffer){
        /* Allocate buffer */
        bi.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bi.pNext = NULL;
        bi.usage = target == ELEMENT_ARRAY_BUFFER ? VK_BUFFER_USAGE_INDEX_BUFFER_BIT : VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        bi.flags = 0;
        bi.size = size;
        bi.queueFamilyIndexCount = 1;
        bi.pQueueFamilyIndices = &backend->queue_family_index;
        bi.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        r = vkCreateBuffer(backend->device, &bi, NULL, &newbuffer);
        if(r != VK_SUCCESS){
            printf("FAILed to create buffer\n");
            return -1;
        }
        ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        ai.pNext = NULL;
        i = cwgl_vkpriv_select_memory_type(ctx, UINT32_MAX,
                                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        if(i < 0){
            printf("Could not find appropriate buffer type\n");
            return -1;
        }
        ai.memoryTypeIndex = i;
        ai.allocationSize = size;
        r = vkAllocateMemory(backend->device, &ai, NULL, &device_memory);
        if(r != VK_SUCCESS){
            printf("FAILed to allocate memory\n");
            return -1;
        }
        r = vkBindBufferMemory(backend->device,
                               newbuffer, device_memory, 0);
        if(r != VK_SUCCESS){
            printf("FAILed to bind buffer\n");
            return -1;
        }
    }else{
        newbuffer = buffer_backend->buffer;
        device_memory = buffer_backend->device_memory;
    }
    r = vkMapMemory(backend->device, device_memory, 0,
                    size, 0, &device_memory_addr);
    if(r != VK_SUCCESS){
        printf("FAILed to map memory\n");
        return -1;
    }
    memcpy(device_memory_addr, data, size);
    vkUnmapMemory(backend->device, device_memory);

    /* Update buffer info */
    buffer_backend->allocated = 1;
    buffer_backend->buffer = newbuffer;
    buffer_backend->device_memory = device_memory;

    /* Update buffer states */
    buffer->state.BUFFER_SIZE = size;
    buffer->state.BUFFER_USAGE = usage;

    return 0;
}

int
cwgl_backend_bufferSubData(cwgl_ctx_t* ctx, cwgl_enum_t target,
                           uint32_t offset, void* data, size_t buflen){
    return 0;
}

