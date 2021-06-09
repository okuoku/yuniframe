#include "cwgl-backend-gles2-priv.h"

int 
cwgl_backend_generateMipmap(cwgl_ctx_t* ctx, cwgl_enum_t target){
    /* Apply hints */
    glHint(GL_GENERATE_MIPMAP_HINT,
           ctx->state.glo.GENERATE_MIPMAP_HINT);
    glGenerateMipmap(target);
    return 0;
}
