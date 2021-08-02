#include "cwgl-backend-vk-priv.h"

#include <stdlib.h>
#include <stdio.h>

int
cwgl_backend_ctx_init(cwgl_ctx_t* ctx){
    VkInstance instance;
    VkApplicationInfo ai;
    VkInstanceCreateInfo ci;
    uint32_t gpus;
    VkPhysicalDevice* devices;
    VkPhysicalDeviceMemoryProperties memory_properties;
    uint32_t qfs;
    uint32_t i;
    int32_t queue_index;
    VkQueueFamilyProperties* qfp;
    VkDeviceQueueCreateInfo qi;
    VkDeviceCreateInfo di;
    VkDevice device;
    VkQueue queue;
    const float queue_priorities = 0.0;
    VkCommandPoolCreateInfo cpi;
    VkCommandPool command_pool;
    VkCommandBufferAllocateInfo cbi;
    VkCommandBuffer command_buffer;

    VkResult r;
    cwgl_backend_ctx_t* c;
    /* Detect Vulkan */
#ifdef CWGL_EXPERIMENTAL_USE_VOLK
    r = volkInitialize();
    if(r != VK_SUCCESS){
        return -1;
    }
#else
#error UNIMPL
#endif
    c = malloc(sizeof(cwgl_backend_ctx_t));
    ctx->backend = c;
    if(c){
        /* Vulkan: Init instance */
        ai.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        ai.pNext = NULL;
        ai.pApplicationName = "cwgl";
        ai.applicationVersion = 0;
        ai.pEngineName = "yfrm";
        ai.engineVersion =  0;
        ai.apiVersion = VK_API_VERSION_1_1;
        ci.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        ci.pNext = NULL;
        ci.flags = 0;
        ci.pApplicationInfo = &ai;
        ci.enabledLayerCount = 0;
        ci.ppEnabledLayerNames = NULL;
        ci.enabledExtensionCount = 0;
        ci.ppEnabledExtensionNames = 0;
        r = vkCreateInstance(&ci, NULL, &instance);
        if(r != VK_SUCCESS){
            goto initfail;
        }
#ifdef CWGL_EXPERIMENTAL_USE_VOLK
        volkLoadInstance(instance);
#endif
        /* Vulkan: Count GPUs */
        r = vkEnumeratePhysicalDevices(instance, &gpus, NULL);
        if(r != VK_SUCCESS){
            goto initfail_instance;
        }
        devices = malloc(sizeof(VkPhysicalDevice) * gpus);
        if(! devices){
            goto initfail_instance;
        }
        r = vkEnumeratePhysicalDevices(instance, &gpus, devices);
        if(r != VK_SUCCESS){
            free(devices);
            goto initfail_instance;
        }
        /* Vulkan: Search appropriate queue */
        vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &qfs, NULL);
        qfp = malloc(sizeof(VkQueueFamilyProperties)*qfs);
        vkGetPhysicalDeviceMemoryProperties(devices[0], &memory_properties);
        vkGetPhysicalDeviceQueueFamilyProperties(devices[0], &qfs, qfp);
        queue_index = -1;
        for(i=0;i!=qfs;i++){
            /* Pickup a graphics queue */
            if(qfp[i].queueFlags & VK_QUEUE_GRAPHICS_BIT){
                queue_index = i;
                break;
            }
        }
        free(qfp);
        if(queue_index < 0){
            goto initfail_instance;
        }
        /* Vulkan: Create device */
        qi.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        qi.pNext = NULL;
        qi.queueFamilyIndex = queue_index;
        qi.queueCount = 1;
        qi.pQueuePriorities = &queue_priorities;
        di.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        di.pNext = NULL;
        di.queueCreateInfoCount = 1;
        di.pQueueCreateInfos = &qi;
        di.enabledExtensionCount = 0;
        di.ppEnabledExtensionNames = NULL;
        di.enabledLayerCount = 0;
        di.ppEnabledLayerNames = NULL;
        di.pEnabledFeatures = NULL;
        r = vkCreateDevice(devices[0], &di, NULL, &device);
        free(devices);
        /* Vulkan: Create command buffer */
        cpi.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        cpi.pNext = NULL;
        cpi.queueFamilyIndex = queue_index;
        cpi.flags = 0;
        r = vkCreateCommandPool(device, &cpi, NULL, &command_pool);
        if(r != VK_SUCCESS){
            goto initfail_device;
        }
        cbi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cbi.pNext = NULL;
        cbi.commandPool = command_pool;
        cbi.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        cbi.commandBufferCount = 1;
        r = vkAllocateCommandBuffers(device, &cbi, &command_buffer);
        if(r != VK_SUCCESS){
            goto initfail_command_pool;
        }
        vkGetDeviceQueue(device, queue_index, 0, &queue);

        c->queue = queue;
        c->queue_active = 0;
        c->queue_has_command = 0;
        c->memory_properties = memory_properties;
        c->queue_family_index = queue_index;
        c->command_buffer = command_buffer;
        c->command_pool = command_pool;
        c->instance = instance;
        c->device = device;
        /* SHXM */
        c->shxm_ctx = shxm_init();
    }
    return 0;
initfail_command_pool:
    vkDestroyCommandPool(device, command_pool, NULL);
initfail_device:
    vkDestroyDevice(device, NULL);
initfail_instance:
    vkDestroyInstance(instance, NULL);
initfail:
    free(c);
    ctx->backend = NULL;
    return -1;

}

int
cwgl_backend_Buffer_init(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer){
    cwgl_backend_Buffer_t* b;
    b = malloc(sizeof(cwgl_backend_Buffer_t));
    if(b){
        b->allocated = 0;
    }
    buffer->backend = b;
    return 0;
}

int
cwgl_backend_Shader_init(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    cwgl_backend_Shader_t* s;
    s = malloc(sizeof(cwgl_backend_Shader_t));
    if(s){
        if(shader->state.SHADER_TYPE == VERTEX_SHADER){
            s->shader = shxm_shader_create(ctx->backend->shxm_ctx,
                SHXM_SHADER_STAGE_VERTEX);
        }else{
            s->shader = shxm_shader_create(ctx->backend->shxm_ctx,
                SHXM_SHADER_STAGE_FRAGMENT);
        }
    }
    shader->backend = s;
    return 0;
}
int
cwgl_backend_Program_init(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    cwgl_backend_Program_t* p;
    p = malloc(sizeof(cwgl_backend_Program_t));
    if(p){
        p->program = shxm_program_create(ctx->backend->shxm_ctx);
    }
    program->backend = p;
    return 0;
}
int
cwgl_backend_Texture_init(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
    cwgl_backend_Texture_t* t;
    t = malloc(sizeof(cwgl_backend_Texture_t));
    if(t){
        t->allocated = 0;
    }
    texture->backend = t;
    return 0;
}
int
cwgl_backend_Renderbuffer_init(cwgl_ctx_t* ctx,
                               cwgl_Renderbuffer_t* renderbuffer){
    cwgl_backend_Renderbuffer_t* r;
    r = malloc(sizeof(cwgl_backend_Renderbuffer_t));
    if(r){
        r->allocated = 0;
    }
    renderbuffer->backend = r;
    return 0;
}
int
cwgl_backend_Framebuffer_init(cwgl_ctx_t* ctx,
                              cwgl_Framebuffer_t* framebuffer){
    cwgl_backend_Framebuffer_t* f;
    f = malloc(sizeof(cwgl_backend_Framebuffer_t));
    if(f){
        // FIXME: Init content here
    }
    framebuffer->backend = f;
    return 0;
}
int
cwgl_backend_ctx_release(cwgl_ctx_t* ctx){
    // FIXME: Release Vulkan objects here
    free(ctx->backend);
    ctx->backend = NULL;
    return 0;
}
int
cwgl_backend_Buffer_release(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer){
    // FIXME: Free object here
    free(buffer->backend);
    buffer->backend = NULL;
    return 0;
}
int
cwgl_backend_Shader_release(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    // FIXME: Free object here
    free(shader->backend);
    shader->backend = NULL;
    return 0;
}
int
cwgl_backend_Program_release(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    // FIXME: Free object here
    free(program->backend);
    program->backend = NULL;
    return 0;
}
int
cwgl_backend_Texture_release(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
    // FIXME: Free object here
    free(texture->backend);
    texture->backend = NULL;
    return 0;
}
int
cwgl_backend_Renderbuffer_release(cwgl_ctx_t* ctx,
                                  cwgl_Renderbuffer_t* renderbuffer){
    // FIXME: Free object here
    free(renderbuffer->backend);
    renderbuffer->backend = NULL;
    return 0;
}
int
cwgl_backend_Framebuffer_release(cwgl_ctx_t* ctx,
                                 cwgl_Framebuffer_t* framebuffer){
    // FIXME: Free object here
    free(framebuffer->backend);
    framebuffer->backend = NULL;
    return 0;
}


