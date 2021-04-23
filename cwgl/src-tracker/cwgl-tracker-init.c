#include "cwgl-tracker-priv.h"

#define SETB1(name, x) ctx->state.glo.name = x ? CWGL_TRUE : CWGL_FALSE
void
cwgl_priv_ctx_init(cwgl_ctx_t* ctx){
    CTX_SET_ERROR(ctx, NO_ERROR);

    /* enable */
    SETB1(BLEND,0);
    SETB1(CULL_FACE,0);
    SETB1(DEPTH_TEST,0);
    SETB1(DITHER,1);
    SETB1(POLYGON_OFFSET_FILL,0);
    SETB1(SAMPLE_ALPHA_TO_COVERAGE,0);
    SETB1(SAMPLE_COVERAGE,0);
    SETB1(SCISSOR_TEST,0);
    SETB1(STENCIL_TEST,0);

    ctx->state.glo.ACTIVE_TEXTURE = TEXTURE0;
    ctx->state.glo.BLEND_COLOR[0] = 0.0f;
    ctx->state.glo.BLEND_COLOR[1] = 0.0f;
    ctx->state.glo.BLEND_COLOR[2] = 0.0f;
    ctx->state.glo.BLEND_COLOR[3] = 0.0f;
    ctx->state.glo.BLEND_EQUATION_RGB = FUNC_ADD;
    ctx->state.glo.BLEND_EQUATION_ALPHA = FUNC_ADD;
    ctx->state.glo.BLEND_SRC_RGB = ONE;
    ctx->state.glo.BLEND_SRC_ALPHA = ONE;
    ctx->state.glo.BLEND_DST_RGB = ZERO;
    ctx->state.glo.BLEND_DST_ALPHA = ZERO;
    
}

