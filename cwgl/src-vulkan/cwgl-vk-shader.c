#include "cwgl-backend-vk-priv.h"

// FIXME: Consider where to decl APIs
cwgl_string_t* cwgl_priv_alloc_string(cwgl_ctx_t* ctx, const char* str, size_t buflen);

int
cwgl_backend_compileShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    return 0;
}

int
cwgl_backend_linkProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    return 0;
}

int
cwgl_backend_validateProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    // FIXME: Implement this
    program->state.VALIDATE_STATUS = CWGL_TRUE;
    return 0;
}
