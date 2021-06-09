#include "cwgl-backend-gles2-priv.h"

static void
configure_tu(cwgl_ctx_t* ctx, cwgl_enum_t target){
    cwgl_texture_unit_state_t* s;
    int id;
    id = (int)ctx->state.glo.ACTIVE_TEXTURE - (int)TEXTURE0;
    glActiveTexture(GL_TEXTURE0);
    if(target == TEXTURE_2D){
        /* 2D */
        glBindTexture(GL_TEXTURE_2D,
                      s[id].TEXTURE_BINDING_2D->backend->name);
    }else{
        /* Cube map */
        glBindTexture(GL_TEXTURE_CUBE_MAP,
                      s[id].TEXTURE_BINDING_CUBE_MAP->backend->name);
    }
    // FIXME: Apply texparameter
}

static void
configure_fb(cwgl_ctx_t* ctx){
    // FIXME: Configure fb here
}

int 
cwgl_backend_texImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                        int32_t level, cwgl_enum_t internalformat, 
                        uint32_t width, uint32_t height, int32_t border, 
                        cwgl_enum_t format, cwgl_enum_t type, 
                        const void* buf, size_t buflen){
    // FIXME: WebGL pixelstore??
    configure_tu(ctx, target);
    glPixelStorei(GL_UNPACK_ALIGNMENT,
                  ctx->state.glo.UNPACK_ALIGNMENT);
    glTexImage2D(target, level, internalformat,
                 width, height, border,
                 format, type, buf);
    return 0;
}

int 
cwgl_backend_copyTexImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                            int32_t level, cwgl_enum_t internalformat, 
                            int32_t x, int32_t y, 
                            uint32_t width, uint32_t height, int32_t border){
    configure_tu(ctx, target);
    configure_fb(ctx);
    glCopyTexImage2D(target, level, internalformat, x, y,
                     width, height, border);
    return 0;
}

int 
cwgl_backend_texSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                           int32_t level, int32_t xoffset, int32_t yoffset, 
                           uint32_t width, uint32_t height, 
                           cwgl_enum_t format, cwgl_enum_t type, 
                           const void* buf, size_t buflen){
    configure_tu(ctx, target);
    glPixelStorei(GL_UNPACK_ALIGNMENT,
                  ctx->state.glo.UNPACK_ALIGNMENT);
    glTexSubImage2D(target, level, xoffset, yoffset,
                    width, height, format, type, buf);
    return 0;
}

int 
cwgl_backend_copyTexSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                               int32_t level, int32_t xoffset, int32_t yoffset, 
                               int32_t x, int32_t y, 
                               uint32_t width, uint32_t height){
    configure_tu(ctx, target);
    configure_fb(ctx);
    glCopyTexSubImage2D(target, level, xoffset, yoffset,
                        x, y, width, height);
    return 0;
}

int 
cwgl_backend_compressedTexImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                                  int32_t level, cwgl_enum_t internalformat, 
                                  uint32_t width, uint32_t height, 
                                  int32_t border, 
                                  const void* buf, size_t buflen){
    configure_tu(ctx, target);
    glCompressedTexImage2D(target, level,
                           internalformat, width, height,
                           border, buflen, buf);
    return 0;
}

int 
cwgl_backend_compressedTexSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                                     int32_t level, 
                                     int32_t xoffset, int32_t yoffset, 
                                     uint32_t width, uint32_t height, 
                                     cwgl_enum_t format, 
                                     const void* buf, size_t buflen){
    configure_tu(ctx, target);
    glCompressedTexSubImage2D(target, level,
                              xoffset, yoffset, width, height,
                              format, buflen, buf);
    return 0;
}

