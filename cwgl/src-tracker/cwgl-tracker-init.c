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

}

