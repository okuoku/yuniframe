#include "cwgl-tracker-priv.h"
#include <stdlib.h>

CWGL_API cwgl_VertexArrayObject_t* 
cwgl_createVertexArray(cwgl_ctx_t* ctx){
    cwgl_VertexArrayObject_t* vao;
    vao = malloc(sizeof(vao));
    if(vao){
        cwgl_priv_objhdr_init(ctx, &vao->hdr, CWGL_OBJ_VAO);
        cwgl_priv_vao_init(&vao->state, vao->attrib);
    }
    return vao;
}

static void
free_vao(cwgl_VertexArrayObject_t* vao){
    int i;
    cwgl_priv_buffer_release(vao->state.ELEMENT_ARRAY_BUFFER_BINDING);
    for(i=0;i!=CWGL_MAX_VAO_SIZE;i++){
        cwgl_priv_buffer_release(vao->attrib[i].VERTEX_ATTRIB_ARRAY_BUFFER_BINDING);
    }
    free(vao);
}

static void
release_vao(cwgl_VertexArrayObject_t* vao){
    uintptr_t v;
    if(vao){
        v = cwgl_priv_objhdr_release(&vao->hdr);
        if(!v){
            free_vao(vao);
        }
    }
}

static void
unbind_vao(cwgl_ctx_t* ctx, cwgl_VertexArrayObject_t* vao){
    int i;
    if(ctx->state.bin.VERTEX_ARRAY_BINDING == vao){
        ctx->state.bin.VERTEX_ARRAY_BINDING = NULL;
        release_vao(vao);
    }
}

CWGL_API void 
cwgl_bindVertexArray(cwgl_ctx_t* ctx, cwgl_VertexArrayObject_t* obj){
    if(obj != ctx->state.bin.VERTEX_ARRAY_BINDING){
        unbind_vao(ctx, ctx->state.bin.VERTEX_ARRAY_BINDING);
        if(obj){
            cwgl_priv_objhdr_retain(&obj->hdr);
        }
        ctx->state.bin.VERTEX_ARRAY_BINDING = obj;
    }
}

CWGL_API void 
cwgl_deleteVertexArray(cwgl_ctx_t* ctx, cwgl_VertexArrayObject_t* obj){
    unbind_vao(ctx, obj);
}


CWGL_API void 
cwgl_VertexArrayObject_release(cwgl_ctx_t* ctx, cwgl_VertexArrayObject_t* vao){
    release_vao(vao);
}
