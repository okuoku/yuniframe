#include "cwgl-tracker-priv.h"

#define CTXSET(name,x) \
    case name: \
      ctx->state.glo.name = x ? CWGL_TRUE : CWGL_FALSE; break

#define UNSET(x) CTXSET(x,0)
#define SET(x) CTXSET(x,1)

/* OpenGL State */
CWGL_API void 
cwgl_disable(cwgl_ctx_t* ctx, cwgl_enum_t cap){
    switch(cap){
        UNSET(BLEND);
        UNSET(CULL_FACE);
        UNSET(DEPTH_TEST);
        UNSET(DITHER);
        UNSET(POLYGON_OFFSET_FILL);
        UNSET(SAMPLE_ALPHA_TO_COVERAGE);
        UNSET(SAMPLE_COVERAGE);
        UNSET(SCISSOR_TEST);
        UNSET(STENCIL_TEST);
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        break;
    }
}

CWGL_API void 
cwgl_enable(cwgl_ctx_t* ctx, cwgl_enum_t cap){
    switch(cap){
        SET(BLEND);
        SET(CULL_FACE);
        SET(DEPTH_TEST);
        SET(DITHER);
        SET(POLYGON_OFFSET_FILL);
        SET(SAMPLE_ALPHA_TO_COVERAGE);
        SET(SAMPLE_COVERAGE);
        SET(SCISSOR_TEST);
        SET(STENCIL_TEST);
        default:
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        break;
    }
}

