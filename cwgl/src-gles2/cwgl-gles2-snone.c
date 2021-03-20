#include "cwgl-gles2-priv.h"

/* OpenGL State */
CWGL_API void 
cwgl_disable(cwgl_ctx_t* ctx, cwgl_enum_t cap){
    CTX_ENTER(ctx);
    glDisable(cap);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_enable(cwgl_ctx_t* ctx, cwgl_enum_t cap){
    CTX_ENTER(ctx);
    glEnable(cap);
    CTX_LEAVE(ctx);
}

