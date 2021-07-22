#include "cwgl-backend-vk-priv.h"

#include <stdlib.h>
#include <stdio.h>

int
cwgl_backend_ctx_init(cwgl_ctx_t* ctx){
    cwgl_backend_ctx_t* c;
    c = malloc(sizeof(cwgl_backend_ctx_t));
    ctx->backend = c;
    return 0;
}

int
cwgl_backend_Buffer_init(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer){
    cwgl_backend_Buffer_t* b;
    b = malloc(sizeof(cwgl_backend_Buffer_t));
    if(b){
        // FIXME: Init buffer content here
    }
    buffer->backend = b;
    return 0;
}

int
cwgl_backend_Shader_init(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    cwgl_backend_Shader_t* s;
    s = malloc(sizeof(cwgl_backend_Shader_t));
    if(s){
        // FIXME: Init content here
    }
    shader->backend = s;
    return 0;
}
int
cwgl_backend_Program_init(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    cwgl_backend_Program_t* p;
    p = malloc(sizeof(cwgl_backend_Program_t));
    if(p){
        // FIXME: Init content here
    }
    program->backend = p;
    return 0;
}
int
cwgl_backend_Texture_init(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
    cwgl_backend_Texture_t* t;
    t = malloc(sizeof(cwgl_backend_Texture_t));
    if(t){
        // FIXME: Init content here
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
        // FIXME: Init content here
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


