#include "cwgl-backend-gles2-priv.h"

int 
cwgl_backend_readPixels(cwgl_ctx_t* ctx, 
                        int32_t x, int32_t y, uint32_t width, uint32_t height, 
                        cwgl_enum_t format, cwgl_enum_t type, 
                        void* buf, size_t buflen){
    (void)buflen;

    // FIXME: Check framebuffer completeness here
    /* Configure pixelstore */
    glPixelStorei(GL_PACK_ALIGNMENT, ctx->state.glo.PACK_ALIGNMENT);
    glReadPixels(x, y, width, height,
                 format, type, buf);
}

int 
cwgl_backend_renderbufferStorage(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                                 cwgl_enum_t internalformat, 
                                 uint32_t width, uint32_t height){
    glBindRenderbuffer(GL_RENDERBUFFER,
                       ctx->state.bin.RENDERBUFFER_BINDING->backend->name);
    glRenderbufferStorage(GL_RENDERBUFFER,
                          internalformat, width, height);
}

