#include "cwgl-backend-gles2-priv.h"

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
    GLuint name;
    cwgl_backend_Buffer_t* b;
    b = malloc(sizeof(cwgl_backend_Buffer_t));
    if(b){
        glGenBuffers(1, &name);
        b->name = name;
    }
    buffer->backend = b;
    return 0;
}
int 
cwgl_backend_Shader_init(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    GLuint name;
    cwgl_backend_Shader_t* s;
    s = malloc(sizeof(cwgl_backend_Shader_t));
    if(s){
        if(shader->state.SHADER_TYPE == VERTEX_SHADER){
            name = glCreateShader(GL_VERTEX_SHADER);
        }else{
            name = glCreateShader(GL_FRAGMENT_SHADER);
        }
        s->name = name;
    }
    shader->backend = s;
    return 0;
}
int 
cwgl_backend_Program_init(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    GLuint name;
    cwgl_backend_Program_t* p;
    p = malloc(sizeof(cwgl_backend_Program_t));
    if(p){
        name = glCreateProgram();
        p->name = name;
        p->attached_vs = 0;
        p->attached_fs = 0;
    }
    program->backend = p;
    return 0;
}
int 
cwgl_backend_Texture_init(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
    GLuint name;
    cwgl_backend_Texture_t* t;
    t = malloc(sizeof(cwgl_backend_Texture_t));
    if(t){
        glGenTextures(1, &name);
        t->name = name;
    }
    texture->backend = t;
    return 0;
}
int 
cwgl_backend_Renderbuffer_init(cwgl_ctx_t* ctx, 
                               cwgl_Renderbuffer_t* renderbuffer){
    GLuint name;
    cwgl_backend_Renderbuffer_t* r;
    r = malloc(sizeof(cwgl_backend_Renderbuffer_t));
    if(r){
        glGenRenderbuffers(1, &name);
        r->name = name;
    }
    renderbuffer->backend = r;
    return 0;
}
int 
cwgl_backend_Framebuffer_init(cwgl_ctx_t* ctx, 
                              cwgl_Framebuffer_t* framebuffer){
    GLuint name;
    cwgl_backend_Framebuffer_t* f;
    f = malloc(sizeof(cwgl_backend_Framebuffer_t));
    if(f){
        glGenFramebuffers(1, &name);
        f->name = name;
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
    glDeleteBuffers(1, &buffer->backend->name);
    free(buffer->backend);
    buffer->backend = NULL;
    return 0;
}
int 
cwgl_backend_Shader_release(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    glDeleteShader(shader->backend->name);
    free(shader->backend);
    shader->backend = NULL;
    return 0;
}
int 
cwgl_backend_Program_release(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    glDeleteProgram(program->backend->name);
    free(program->backend);
    program->backend = NULL;
    return 0;
}
int 
cwgl_backend_Texture_release(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
    glDeleteTextures(1, &texture->backend->name);
    free(texture->backend);
    texture->backend = NULL;
    return 0;
}
int 
cwgl_backend_Renderbuffer_release(cwgl_ctx_t* ctx, 
                                  cwgl_Renderbuffer_t* renderbuffer){
    glDeleteRenderbuffers(1, &renderbuffer->backend->name);
    free(renderbuffer->backend);
    renderbuffer->backend = NULL;
    return 0;
}
int 
cwgl_backend_Framebuffer_release(cwgl_ctx_t* ctx, 
                                 cwgl_Framebuffer_t* framebuffer){
    glDeleteFramebuffers(1, &framebuffer->backend->name);
    free(framebuffer->backend);
    framebuffer->backend = NULL;
    return 0;
}

