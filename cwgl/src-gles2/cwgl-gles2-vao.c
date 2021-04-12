#include "cwgl-gles2-priv.h"

CWGL_API cwgl_VertexArrayObject_t* 
cwgl_createVertexArray(cwgl_ctx_t* ctx){
    GLuint name = 0;
    cwgl_VertexArrayObject_t* o;
    CTX_ENTER(ctx);
    glGenVertexArraysOES(1, &name);
    o = CTX_ALLOC(ctx, VertexArrayObject);
    CTX_SETNAME(ctx, o, name);
    CTX_LEAVE(ctx);
    return o;
}


CWGL_API void 
cwgl_bindVertexArray(cwgl_ctx_t* ctx, cwgl_VertexArrayObject_t* obj){
    GLuint name;
    CTX_ENTER(ctx);
    if(obj){
        name = CTX_GETNAME(ctx, obj);
    }else{
        name = 0;
    }
    glBindVertexArrayOES(name);
    CTX_LEAVE(ctx);
}


