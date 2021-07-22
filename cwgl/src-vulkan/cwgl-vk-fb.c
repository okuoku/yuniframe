#include "cwgl-backend-vk-priv.h"

int
cwgl_backend_readPixels(cwgl_ctx_t* ctx,
                        int32_t x, int32_t y, uint32_t width, uint32_t height,
                        cwgl_enum_t format, cwgl_enum_t type,
                        void* buf, size_t buflen){
    return 0;
}

int
cwgl_backend_renderbufferStorage(cwgl_ctx_t* ctx, cwgl_enum_t target,
                                 cwgl_enum_t internalformat,
                                 uint32_t width, uint32_t height){
    // FIXME: Do we need this?
    cwgl_backend_configure_framebuffer(ctx, NULL);
    return 0;
}

int
cwgl_backend_configure_framebuffer(cwgl_ctx_t* ctx, cwgl_enum_t* out_state){
    return 0;
}


