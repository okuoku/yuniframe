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

