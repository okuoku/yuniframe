#include "cwgl-backend-vk-priv.h"

#include <stdlib.h>
#include <stdio.h>

/* In platform/src-sdl2 */
int cwgl_integ_vkpriv_getextensions(cwgl_ctx_t* ctx, int* out_count, const char** out_extensions);
int cwgl_integ_vkpriv_createsurface(cwgl_ctx_t* ctx, VkInstance instance, VkSurfaceKHR* surface);

int
cwgl_backend_ctx_init(cwgl_ctx_t* ctx){
    VkInstance instance;
    VkApplicationInfo ai;
    VkInstanceCreateInfo ci;
    uint32_t gpus;
    VkPhysicalDevice* devices;
    VkPhysicalDeviceMemoryProperties memory_properties;
    VkPhysicalDevice physical_device;
    uint32_t qfs;
    uint32_t i;
    int32_t queue_index;
    VkQueueFamilyProperties* qfp;
    VkDeviceQueueCreateInfo qi;
    VkDeviceCreateInfo di;
    VkDevice device;
    VkQueue queue;
    VkBool32 valid_surface;
    VkSurfaceKHR surface;
    int instance_extensions_count;
    char** instance_extensions;
    const float queue_priorities = 0.0;
    VkCommandPoolCreateInfo cpi;
    VkCommandPool command_pool;
    VkCommandBufferAllocateInfo cbi;
    VkCommandBuffer command_buffer;
    VkSwapchainCreateInfoKHR sci;
    VkSwapchainKHR swapchain;
    char** device_extensions;
    VkSemaphoreCreateInfo si;

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
        /* Init flags */
        c->queue_active = 0;
        c->queue_has_command = 0;
        c->framebuffer_allocated = 0;

        /* Pass1: Get extensions count */
        cwgl_integ_vkpriv_getextensions(ctx, 
                                        &instance_extensions_count,
                                        NULL);
        instance_extensions = malloc(sizeof(char*)*instance_extensions_count);
        /* Pass2: Get extensions */
        cwgl_integ_vkpriv_getextensions(ctx, 
                                        &instance_extensions_count,
                                        instance_extensions);

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
        ci.enabledExtensionCount = instance_extensions_count;
        ci.ppEnabledExtensionNames = instance_extensions;
        r = vkCreateInstance(&ci, NULL, &instance);
        free(instance_extensions);
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
        physical_device = devices[0];
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
        qi.flags = 0;
        qi.queueFamilyIndex = queue_index;
        qi.queueCount = 1;
        qi.pQueuePriorities = &queue_priorities;
        di.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        di.pNext = NULL;
        di.flags = 0;
        di.queueCreateInfoCount = 1;
        di.pQueueCreateInfos = &qi;
        di.enabledExtensionCount = 1;
        const char* swapchain_extension = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
        device_extensions = &swapchain_extension;
        di.ppEnabledExtensionNames = device_extensions;
        di.enabledLayerCount = 0;
        di.ppEnabledLayerNames = NULL;
        di.pEnabledFeatures = NULL;
        r = vkCreateDevice(devices[0], &di, NULL, &device);
        free(devices);
        /* Vulkan: Create command buffer */
        cpi.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        cpi.pNext = NULL;
        cpi.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        cpi.queueFamilyIndex = queue_index;
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
        /* Create surface */
        // FIXME: Y-Flip surface?
        cwgl_integ_vkpriv_createsurface(ctx, instance, &surface);
        /* Validate surface */
        r = vkGetPhysicalDeviceSurfaceSupportKHR(physical_device,
                                                 queue_index,
                                                 surface,
                                                 &valid_surface);
        if(r != VK_SUCCESS){
            goto initfail_command_pool;
        }
        if(! valid_surface){
            goto initfail_command_pool;
        }
        /* Create Swapchain */
        // FIXME: We assume selected Graphics queue also supports presentation
        // FIXME: Adjust image size and format
        sci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        sci.pNext = NULL;
        sci.flags = 0;
        sci.surface = surface;
        sci.minImageCount = 2;
        sci.imageFormat = VK_FORMAT_B8G8R8A8_UNORM;
        sci.imageColorSpace = 0; /* sRGB, gamma */
        sci.imageExtent.width = 1280;
        sci.imageExtent.height = 720;
        sci.imageArrayLayers = 1;
        sci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
        sci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        sci.queueFamilyIndexCount = 0;
        sci.pQueueFamilyIndices = NULL;
        sci.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
        sci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        sci.presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR; /* HFLIP */
        sci.clipped = VK_TRUE;
        sci.oldSwapchain = VK_NULL_HANDLE;
        r = vkCreateSwapchainKHR(device, &sci, NULL, &swapchain);
        if(r != VK_SUCCESS){
            goto initfail_command_pool;
        }
        si.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        si.pNext = NULL;
        si.flags = 0;
        vkCreateSemaphore(device, &si, NULL, &c->sem_fb);

        c->queue = queue;
        c->memory_properties = memory_properties;
        c->queue_family_index = queue_index;
        c->command_buffer = command_buffer;
        c->command_pool = command_pool;
        c->instance = instance;
        c->device = device;
        c->surface = surface;
        c->swapchain = swapchain;
        /* SHXM */
        c->shxm_ctx = shxm_init();
    }
    // FIXME: We should emit resize event instead..?
    cwgl_vkpriv_prepare_fb(ctx);
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
    cwgl_vkpriv_destroy_buffer(ctx, buffer->backend);
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
    cwgl_vkpriv_destroy_texture(ctx, texture->backend);
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


