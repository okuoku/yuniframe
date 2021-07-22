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
    return 0;
}

int 
cwgl_backend_renderbufferStorage(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                                 cwgl_enum_t internalformat, 
                                 uint32_t width, uint32_t height){
    GLint p;
    GLuint name;
    cwgl_Renderbuffer_t* rb;
    rb = ctx->state.bin.RENDERBUFFER_BINDING;
    name = rb->backend->name;
    glBindRenderbuffer(GL_RENDERBUFFER, name);
    glRenderbufferStorage(GL_RENDERBUFFER,
                          internalformat, width, height);
    /* Update renderbuffer states */
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &p);
    rb->state.RENDERBUFFER_WIDTH = p;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &p);
    rb->state.RENDERBUFFER_HEIGHT = p;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_INTERNAL_FORMAT, &p);
    rb->state.RENDERBUFFER_INTERNAL_FORMAT = p;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_RED_SIZE, &p);
    rb->state.RENDERBUFFER_RED_SIZE = p;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_GREEN_SIZE, &p);
    rb->state.RENDERBUFFER_GREEN_SIZE = p;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_BLUE_SIZE, &p);
    rb->state.RENDERBUFFER_BLUE_SIZE = p;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_ALPHA_SIZE, &p);
    rb->state.RENDERBUFFER_ALPHA_SIZE = p;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_DEPTH_SIZE, &p);
    rb->state.RENDERBUFFER_DEPTH_SIZE = p;
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_STENCIL_SIZE, &p);
    rb->state.RENDERBUFFER_STENCIL_SIZE = p;

    // FIXME: Do we need this?
    cwgl_backend_configure_framebuffer(ctx, NULL);
    return 0;
}

static void
configure_attachment(cwgl_framebuffer_attachment_state_t* state,
                     cwgl_enum_t pos){
    GLuint name;
    cwgl_enum_t type;
    type = state->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE;
    switch(type){
        default:
            name = 0;
            type = NONE;
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, pos, GL_RENDERBUFFER, 0);
            break;
        case RENDERBUFFER:
            name = state->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asRenderbuffer->backend->name;
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, pos, GL_RENDERBUFFER, name);
            break;
        case TEXTURE:
            name = state->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asTexture->backend->name;
            // FIXME: Need to check if cube-map??
            glFramebufferTexture2D(GL_FRAMEBUFFER,
                                   pos,
                                   state->FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE,
                                   name,
                                   state->FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL);
            break;
    }
}


int
cwgl_backend_configure_framebuffer(cwgl_ctx_t* ctx, cwgl_enum_t* out_state){
    cwgl_enum_t state;
    cwgl_Framebuffer_t* fb;
    GLuint name;
    GLint p;
    fb = ctx->state.bin.FRAMEBUFFER_BINDING;
    if(!fb){
        name = 0;
    }else{
        name = fb->backend->name;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, name);

    if(name){
        /* If non-default framebuffer selected,
         * configure framebuffer attachments */
        configure_attachment(&fb->state.COLOR_ATTACHMENT0, COLOR_ATTACHMENT0);
        configure_attachment(&fb->state.DEPTH_ATTACHMENT, DEPTH_ATTACHMENT);
        configure_attachment(&fb->state.STENCIL_ATTACHMENT, STENCIL_ATTACHMENT);
    }
    /* Check framebuffer completeness */
    state = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(out_state){
        *out_state = state;
    }
    /* Update framebuffer states */
    if(name){
        glGetIntegerv(GL_RED_BITS, &p);
        fb->state.RED_BITS = p;
        glGetIntegerv(GL_GREEN_BITS, &p);
        fb->state.GREEN_BITS = p;
        glGetIntegerv(GL_BLUE_BITS, &p);
        fb->state.BLUE_BITS = p;
        glGetIntegerv(GL_ALPHA_BITS, &p);
        fb->state.ALPHA_BITS = p;
        glGetIntegerv(GL_DEPTH_BITS, &p);
        fb->state.DEPTH_BITS = p;
        glGetIntegerv(GL_STENCIL_BITS, &p);
        fb->state.STENCIL_BITS = p;
        glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_TYPE, &p);
        fb->state.IMPLEMENTATION_COLOR_READ_TYPE = p;
        glGetIntegerv(GL_IMPLEMENTATION_COLOR_READ_FORMAT, &p);
        fb->state.IMPLEMENTATION_COLOR_READ_FORMAT = p;
    }
    return 0;
}

