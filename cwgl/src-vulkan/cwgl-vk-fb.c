#include "cwgl-backend-vk-priv.h"
#include <stdio.h>

static void
destroy_rb(cwgl_ctx_t* ctx, cwgl_backend_Renderbuffer_t* rb){
    vkDestroyImageView(ctx->backend->device, rb->image_view, NULL);
    vkDestroyImage(ctx->backend->device, rb->image, NULL);
    vkFreeMemory(ctx->backend->device, rb->device_memory, NULL);
    rb->allocated = 0;
}

static VkFormat
to_vulkan_rb_format(cwgl_enum_t fmt){
    switch(fmt){
        case RGBA4:
            return VK_FORMAT_R8G8B8A8_UNORM;
        case RGB565:
            return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
        case RGB5_A1:
            return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
        case DEPTH_COMPONENT16:
            return VK_FORMAT_D16_UNORM;
        case 0x88f0: //FIXME: WebGL2 DEPTH24_STENCIL8
        case DEPTH_STENCIL:
            // FIXME: Use D24 where applicable
            return VK_FORMAT_D32_SFLOAT_S8_UINT;
            //return VK_FORMAT_D24_UNORM_S8_UINT;
        case STENCIL_INDEX8:
            return VK_FORMAT_S8_UINT;
        default:
            break;
    }
    return VK_FORMAT_UNDEFINED;
}

static void
prepare_rb(cwgl_ctx_t* ctx, cwgl_backend_Renderbuffer_t* rb, cwgl_enum_t fmt,
           uint32_t width, uint32_t height){
    cwgl_backend_ctx_t* backend;
    VkResult r;
    VkImageCreateInfo ii;
    VkImageViewCreateInfo vi;
    VkImage image;
    VkImageView image_view;
    VkMemoryRequirements memory_requirements;
    VkMemoryAllocateInfo ai;
    VkDeviceMemory device_memory;
    VkImageUsageFlagBits usage;
    VkImageAspectFlags aspect_mask;
    VkFormat format;


    backend = ctx->backend;
    if(rb->allocated){
        destroy_rb(ctx, rb);
    }

    switch(fmt){
        case DEPTH_STENCIL:
            usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            aspect_mask = VK_IMAGE_ASPECT_STENCIL_BIT | VK_IMAGE_ASPECT_DEPTH_BIT;
            break;
        case STENCIL_INDEX8:
            usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            aspect_mask = VK_IMAGE_ASPECT_STENCIL_BIT;
            break;
        case DEPTH_COMPONENT16:
            usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            aspect_mask = VK_IMAGE_ASPECT_DEPTH_BIT;
            break;
        default: /* Colour buffer */
            usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
            aspect_mask = VK_IMAGE_ASPECT_COLOR_BIT;
            break;

    }
    format = to_vulkan_rb_format(fmt);
    ii.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    ii.pNext = NULL;
    ii.flags = 0;
    ii.imageType = VK_IMAGE_TYPE_2D;
    ii.format = format;
    ii.extent.width = width;
    ii.extent.height = height;
    ii.extent.depth = 1;
    ii.mipLevels = 1;
    ii.arrayLayers = 1;
    ii.samples = VK_SAMPLE_COUNT_1_BIT;
    ii.tiling = VK_IMAGE_TILING_OPTIMAL;
    ii.usage = usage;
    ii.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    ii.queueFamilyIndexCount = 0;
    ii.pQueueFamilyIndices = NULL;
    ii.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    r = vkCreateImage(backend->device, &ii, NULL, &image);
    if(r != VK_SUCCESS){
        printf("Failed to create fb image\n");
        return;
    }
    vkGetImageMemoryRequirements(backend->device, image, &memory_requirements);
    ai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    ai.pNext = NULL;
    ai.allocationSize = memory_requirements.size;
    ai.memoryTypeIndex =
        cwgl_vkpriv_select_memory_type(ctx,
                                       memory_requirements.memoryTypeBits,
                                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    r = vkAllocateMemory(backend->device, &ai, NULL, &device_memory);
    if(r != VK_SUCCESS){
        printf("Failed to allocate fb memory\n");
        return;
    }
    vkBindImageMemory(backend->device, image, device_memory, 0);

    vi.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    vi.pNext = NULL;
    vi.flags = 0;
    vi.image = image;
    vi.viewType = VK_IMAGE_VIEW_TYPE_2D;
    vi.format = format;
    vi.components.r = VK_COMPONENT_SWIZZLE_R;
    vi.components.g = VK_COMPONENT_SWIZZLE_G;
    vi.components.b = VK_COMPONENT_SWIZZLE_B;
    vi.components.a = VK_COMPONENT_SWIZZLE_A;
    vi.subresourceRange.aspectMask = aspect_mask;
    vi.subresourceRange.baseMipLevel = 0;
    vi.subresourceRange.levelCount = 1;
    vi.subresourceRange.baseArrayLayer = 0;
    vi.subresourceRange.layerCount = 1;
    r = vkCreateImageView(backend->device, &vi, NULL, &image_view);
    if(r != VK_SUCCESS){
        printf("Failed to create fb image view\n");
        return;
    }
    rb->image = image;
    rb->image_view = image_view;
    rb->device_memory = device_memory;
    rb->allocated = 1;
    rb->format = format;
    rb->ident = cwgl_vkpriv_newident(ctx);
    rb->width = width;
    rb->height = height;
}

void
cwgl_vkpriv_prepare_fb(cwgl_ctx_t* ctx){
    VkResult r;
    int i;
    uint32_t imagecount;
    cwgl_backend_ctx_t* backend;
    backend = ctx->backend;
    // FIXME: Consider stencil flag
    if(backend->framebuffer_allocated){
        // FIXME: In case width/height and format was changed, reallocate it
        return;
    }
    r = vkGetSwapchainImagesKHR(backend->device, 
                                backend->swapchain,
                                &imagecount,
                                NULL);
    if(r != VK_SUCCESS){
        printf("Unknown error in vkGetSwapchainImagesKHR\n");
        return;
    }
    if(imagecount != CWGL_FRAMEBUFFER_COUNT){
        printf("Invalid image count\n");
        return;
    }
    r = vkGetSwapchainImagesKHR(backend->device, 
                                backend->swapchain,
                                &imagecount,
                                backend->cb);
    if(r != VK_SUCCESS){
        printf("Unknown error in vkGetSwapchainImagesKHR\n");
        return;
    }
    for(i=0;i!=CWGL_FRAMEBUFFER_COUNT;i++){
        // FIXME: Implement destroy
        VkImageViewCreateInfo vi;

        vi.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        vi.pNext = NULL;
        vi.flags = 0;
        vi.image = backend->cb[i];
        vi.viewType = VK_IMAGE_VIEW_TYPE_2D;
        vi.format = backend->cb_format;
        vi.components.r = VK_COMPONENT_SWIZZLE_R;
        vi.components.g = VK_COMPONENT_SWIZZLE_G;
        vi.components.b = VK_COMPONENT_SWIZZLE_B;
        vi.components.a = VK_COMPONENT_SWIZZLE_A;
        vi.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        vi.subresourceRange.baseMipLevel = 0;
        vi.subresourceRange.levelCount = 1;
        vi.subresourceRange.baseArrayLayer = 0;
        vi.subresourceRange.layerCount = 1;
        r = vkCreateImageView(backend->device, &vi, NULL, &backend->cb_view[i]);
        if(r != VK_SUCCESS){
            printf("Failed to create fb image view\n");
            return;
        }
    }

    backend->depth.allocated = 0;
    prepare_rb(ctx, &backend->depth, DEPTH_STENCIL, 1280, 720);
    backend->framebuffer_allocated = 1;
    backend->framebuffer_ident = cwgl_vkpriv_newident(ctx);
}

int
cwgl_backend_readPixels(cwgl_ctx_t* ctx,
                        int32_t x, int32_t y, uint32_t width, uint32_t height,
                        cwgl_enum_t format, cwgl_enum_t type,
                        void* buf, size_t buflen){
    // FIXME: Implement this
    return 0;
}

int
cwgl_backend_renderbufferStorage(cwgl_ctx_t* ctx, cwgl_enum_t target,
                                 cwgl_enum_t internalformat,
                                 uint32_t width, uint32_t height){
    cwgl_Renderbuffer_t* rb;
    cwgl_backend_Renderbuffer_t* rb_backend;
    uint32_t red_size = 0;
    uint32_t green_size = 0;
    uint32_t blue_size = 0;
    uint32_t alpha_size = 0;
    uint32_t depth_size = 0;
    uint32_t stencil_size = 0;
    rb = ctx->state.bin.RENDERBUFFER_BINDING;
    rb_backend = rb->backend;
    prepare_rb(ctx, rb_backend, internalformat, width, height);
    switch(internalformat){
        case RGBA4:
            red_size = green_size = blue_size = alpha_size = 8;
            break;
        case RGB565:
            red_size = blue_size = 5;
            green_size = 6;
            alpha_size = 0;
            break;
        case RGB5_A1:
            red_size = blue_size = 5;
            green_size = 6;
            alpha_size = 1;
            break;
        case DEPTH_COMPONENT16:
            depth_size = 16;
            break;
        case DEPTH_STENCIL:
            depth_size = 24;
            stencil_size = 8;
            break;
        case STENCIL_INDEX8:
            stencil_size = 8;
            break;
        default:
            break;
    }
    rb->state.RENDERBUFFER_WIDTH = width;
    rb->state.RENDERBUFFER_HEIGHT = height;
    rb->state.RENDERBUFFER_INTERNAL_FORMAT = internalformat;
    rb->state.RENDERBUFFER_RED_SIZE = red_size;
    rb->state.RENDERBUFFER_GREEN_SIZE = green_size;
    rb->state.RENDERBUFFER_BLUE_SIZE = blue_size;
    rb->state.RENDERBUFFER_ALPHA_SIZE = alpha_size;
    rb->state.RENDERBUFFER_DEPTH_SIZE = depth_size;
    rb->state.RENDERBUFFER_STENCIL_SIZE = stencil_size;

    return 0;
}

int
cwgl_backend_configure_framebuffer(cwgl_ctx_t* ctx, cwgl_enum_t* out_state){
    cwgl_enum_t state;
    cwgl_Framebuffer_t* fb;
    cwgl_backend_ctx_t* backend;
    cwgl_backend_Framebuffer_t* fb_backend;
    cwgl_framebuffer_attachment_state_t* color0;
    cwgl_framebuffer_attachment_state_t* depth;
    cwgl_framebuffer_attachment_state_t* stencil;
    int has_depth = 0;
    int has_stencil = 0;
    backend = ctx->backend;
    // FIXME: Check texture/renderbuffer dimentions
    state = FRAMEBUFFER_COMPLETE;
    fb = ctx->state.bin.FRAMEBUFFER_BINDING;


    if(! fb){
        /* No valid tracker Texture objects for these */
        backend->render_to_fb = 1;
    }else{
        /* Update Framebuffer object using bound Texture/Renderbuffers */
        backend->render_to_fb = 0;
        fb_backend = fb->backend;
        fb_backend->configuration_ident = cwgl_vkpriv_newident(ctx);
        color0 = &fb->state.COLOR_ATTACHMENT0;
        fb_backend->rb_color0 = NULL;
        fb_backend->texture_color0 = NULL;
        fb_backend->rb_depth_stencil = NULL;
        fb_backend->texture_depth_stencil = NULL;
        if(color0){
            fb_backend->has_color0 = 1;
            fb_backend->color_type0 = color0->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE;
            switch(fb_backend->color_type0){
                default:
                    break;
                case RENDERBUFFER:
                    fb_backend->rb_color0 = color0->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asRenderbuffer->backend;
                    break;
                case TEXTURE:
                    fb_backend->texture_color0 = color0->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asTexture->backend;
                    break;
            }
        }else{
            fb_backend->has_color0 = 0;
            fb_backend->color_type0 = NONE;
        }
        depth = &fb->state.DEPTH_ATTACHMENT;
        stencil = &fb->state.STENCIL_ATTACHMENT;
        if(depth->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE == RENDERBUFFER ||
           depth->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE == TEXTURE){
            has_depth = 1;
        }
        if(stencil->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE == RENDERBUFFER ||
           stencil->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE == TEXTURE){
            has_stencil = 1;
        }
        /* Check validity */
        if(has_depth && has_stencil){
            if(depth->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE !=
               stencil->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE){
                printf("???: Different object type for depth-stencil\n");
            }
            if(depth->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE == RENDERBUFFER){
                if(depth->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asTexture
                   != stencil->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asTexture){
                    printf("???: Different framebuffer for depth-stencil\n");
                }
            }else{
                if(depth->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asRenderbuffer
                   != stencil->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asRenderbuffer){
                    printf("???: Different Renderbuffer for depth-stencil\n");
                }
            }
        }
        if(has_stencil){
            fb_backend->has_stencil = 1;
            fb_backend->depth_stencil_type = stencil->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE;
            switch(fb_backend->depth_stencil_type){
                default:
                    break;
                case RENDERBUFFER:
                    fb_backend->rb_depth_stencil =
                        stencil->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asRenderbuffer->backend;
                    break;
                case TEXTURE:
                    fb_backend->texture_depth_stencil =
                        stencil->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asTexture->backend;
                    break;
            }
        }
        if(has_depth){ /* Prefer depth over stencil (if error case) */
            fb_backend->has_depth = 1;
            fb_backend->depth_stencil_type = depth->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE;
            switch(fb_backend->depth_stencil_type){
                default:
                    break;
                case RENDERBUFFER:
                    fb_backend->rb_depth_stencil =
                        depth->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asRenderbuffer->backend;
                    break;
                case TEXTURE:
                    fb_backend->texture_depth_stencil =
                        depth->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asTexture->backend;
                    break;
            }
        }
        fb_backend->has_depth = has_depth;
        fb_backend->has_stencil = has_stencil;
        // FIXME: Read correct framebuffer status
        fb->state.RED_BITS = 8;
        fb->state.GREEN_BITS = 8;
        fb->state.BLUE_BITS = 8;
        fb->state.ALPHA_BITS = 8;
        if(has_depth){
            fb->state.DEPTH_BITS = 24;
        }else{
            fb->state.DEPTH_BITS = 0;
        }
        if(has_stencil){
            fb->state.STENCIL_BITS = 8;
        }else{
            fb->state.STENCIL_BITS = 0;
        }
        fb->state.IMPLEMENTATION_COLOR_READ_TYPE = 0; // FIXME: ???
        fb->state.IMPLEMENTATION_COLOR_READ_FORMAT = 0; // FIXME: ???
    }

    if(out_state){
        *out_state = state;
    }
    return 0;
}


