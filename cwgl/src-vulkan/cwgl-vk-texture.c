#include "cwgl-backend-vk-priv.h"

int
cwgl_backend_texImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target,
                        int32_t level, cwgl_enum_t internalformat,
                        uint32_t width, uint32_t height, int32_t border,
                        cwgl_enum_t format, cwgl_enum_t type,
                        const void* buf, size_t buflen){
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

