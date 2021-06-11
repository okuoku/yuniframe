#include "cwgl-backend-gles2-priv.h"

// FIXME: Copied from s2
static cwgl_VertexArrayObject_t*
current_vao(cwgl_ctx_t* ctx){
    if(ctx->state.bin.VERTEX_ARRAY_BINDING){
        return ctx->state.bin.VERTEX_ARRAY_BINDING;
    }else{
        return ctx->state.default_vao;
    }
}


static void
configure_buffer(cwgl_ctx_t* ctx, cwgl_enum_t target){
    cwgl_VertexArrayObject_t* vao;
    switch(target){
        default:
            /* Do nothing */
            break;
        case ARRAY_BUFFER:
            glBindBuffer(GL_ARRAY_BUFFER,
                         ctx->state.bin.ARRAY_BUFFER_BINDING->backend->name);
            break;
        case ELEMENT_ARRAY_BUFFER:
            vao = current_vao(ctx);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                         vao->state.ELEMENT_ARRAY_BUFFER_BINDING->backend->name);
            break;
    }
}

static void
update_bufferdata(cwgl_ctx_t* ctx, cwgl_enum_t target){
    cwgl_VertexArrayObject_t* vao;
    cwgl_Buffer_t* buf = NULL;
    GLint p;
    switch(target){
        default:
            /* Do nothing */
            break;
        case ARRAY_BUFFER:
            buf = ctx->state.bin.ARRAY_BUFFER_BINDING;
            break;
        case ELEMENT_ARRAY_BUFFER:
            vao = current_vao(ctx);
            buf = vao->state.ELEMENT_ARRAY_BUFFER_BINDING;
            break;
    }

    if(buf){
        glGetBufferParameteriv(target, GL_BUFFER_SIZE, &p);
        buf->state.BUFFER_SIZE = p;
        glGetBufferParameteriv(target, GL_BUFFER_USAGE, &p);
        buf->state.BUFFER_USAGE = p;
    }

}

int 
cwgl_backend_bufferData(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                        uint32_t size, void* data, cwgl_enum_t usage){
    configure_buffer(ctx, target);
    glBufferData(target, size, data, usage);
    update_bufferdata(ctx, target);
    return 0;
}

int 
cwgl_backend_bufferSubData(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                           uint32_t offset, void* data, size_t buflen){
    configure_buffer(ctx, target);
    glBufferSubData(target, offset, buflen, data);
    update_bufferdata(ctx, target);
    return 0;
}

