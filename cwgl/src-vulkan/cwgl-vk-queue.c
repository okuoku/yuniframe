#include "cwgl-backend-vk-priv.h"
#include <stdio.h>

void
cwgl_vkpriv_graphics_wait(cwgl_ctx_t* ctx){
    cwgl_backend_ctx_t* backend;
    backend = ctx->backend;
    if(backend->queue_active){
        // FIXME: We need some strategy here..
        vkQueueWaitIdle(backend->queue);
    }
    backend->queue_active = 0;
}

void
cwgl_vkpriv_graphics_submit(cwgl_ctx_t* ctx){
    VkResult r;
    VkSubmitInfo si;
    cwgl_backend_ctx_t* backend;
    backend = ctx->backend;
    if(backend->queue_has_command){
        if(backend->queue_active){
            cwgl_vkpriv_graphics_wait(ctx);
        }
        si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        si.pNext = NULL;
        si.waitSemaphoreCount = 0;
        si.pWaitSemaphores = 0;
        si.pWaitDstStageMask = NULL;
        si.commandBufferCount = 1;
        si.pCommandBuffers = &backend->command_buffer;
        si.signalSemaphoreCount = 0;
        si.pSignalSemaphores = NULL;
        r = vkQueueSubmit(backend->queue, 1, &si, NULL);
        if(r == VK_SUCCESS){
            backend->queue_active = 1;
        }else{
            printf("SUBMIT FAILED!\n");
        }
        backend->queue_has_command = 0;
    }
}

int
cwgl_backend_beginframe(cwgl_ctx_t* ctx){
    VkResult r;
    uint32_t current_image_index;
    cwgl_backend_ctx_t* backend;
    backend = ctx->backend;
    r = vkAcquireNextImageKHR(backend->device,
                              backend->swapchain,
                              0,
                              VK_NULL_HANDLE,
                              VK_NULL_HANDLE,
                              &current_image_index);
    if(r != VK_SUCCESS){
        printf("Could not flip fb\n");
        current_image_index = 0;
    }
    backend->current_image_index = current_image_index;

    return 0;
}

int
cwgl_backend_endframe(cwgl_ctx_t* ctx){
    VkResult r;
    VkPresentInfoKHR pi;
    cwgl_backend_ctx_t* backend;
    backend = ctx->backend;
    cwgl_vkpriv_graphics_wait(ctx);
    pi.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    pi.pNext = NULL;
    pi.swapchainCount = 1;
    pi.pSwapchains = &backend->swapchain;
    pi.pImageIndices = &backend->current_image_index;
    pi.pWaitSemaphores = NULL;
    pi.waitSemaphoreCount = 0;
    pi.pResults = NULL;
    r = vkQueuePresentKHR(backend->queue, &pi);
    if(r != VK_SUCCESS){
        printf("Failed to flip fb\n");
    }

    return 0;
}

int
cwgl_backend_finish(cwgl_ctx_t* ctx){
    cwgl_vkpriv_graphics_submit(ctx);
    cwgl_vkpriv_graphics_wait(ctx);
    return 0;
}

int
cwgl_backend_flush(cwgl_ctx_t* ctx){
    cwgl_vkpriv_graphics_submit(ctx);
    return 0;
}

