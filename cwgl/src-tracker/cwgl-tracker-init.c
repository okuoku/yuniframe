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
    ctx->state.glo.COLOR_CLEAR_VALUE[0] = 0.0f;
    ctx->state.glo.COLOR_CLEAR_VALUE[1] = 0.0f;
    ctx->state.glo.COLOR_CLEAR_VALUE[2] = 0.0f;
    ctx->state.glo.COLOR_CLEAR_VALUE[3] = 0.0f;
    ctx->state.glo.DEPTH_CLEAR_VALUE = 1.0f;
    ctx->state.glo.STENCIL_CLEAR_VALUE = 0;
    ctx->state.glo.COLOR_WRITEMASK[0] = CWGL_TRUE;
    ctx->state.glo.COLOR_WRITEMASK[1] = CWGL_TRUE;
    ctx->state.glo.COLOR_WRITEMASK[2] = CWGL_TRUE;
    ctx->state.glo.COLOR_WRITEMASK[3] = CWGL_TRUE;
    ctx->state.glo.CULL_FACE_MODE = BACK;
    ctx->state.glo.DEPTH_FUNC = LESS;
    ctx->state.glo.DEPTH_WRITEMASK = CWGL_TRUE;
    ctx->state.glo.DEPTH_RANGE[0] = 0.0f;
    ctx->state.glo.DEPTH_RANGE[1] = 1.0f;
    ctx->state.glo.FRONT_FACE = CCW;
    ctx->state.glo.GENERATE_MIPMAP_HINT = DONT_CARE;
    ctx->state.glo.LINE_WIDTH = 1.0f;
    ctx->state.glo.PACK_ALIGNMENT = 4;
    ctx->state.glo.UNPACK_ALIGNMENT = 4;
    ctx->state.glo.UNPACK_FLIP_Y_WEBGL = CWGL_FALSE;
    ctx->state.glo.UNPACK_PREMULTIPLY_ALPHA_WEBGL = CWGL_FALSE;
    ctx->state.glo.UNPACK_COLORSPACE_CONVERSION_WEBGL = BROWSER_DEFAULT_WEBGL;
    ctx->state.glo.POLYGON_OFFSET_FACTOR = 0.0f;
    ctx->state.glo.POLYGON_OFFSET_UNITS = 0.0f;
    ctx->state.glo.SAMPLE_COVERAGE_VALUE = 1.0f;
    ctx->state.glo.SAMPLE_COVERAGE_INVERT = CWGL_FALSE;
    ctx->state.glo.STENCIL_FUNC = ALWAYS;
    ctx->state.glo.STENCIL_REF = 0;
    ctx->state.glo.STENCIL_VALUE_MASK = -1;
    ctx->state.glo.STENCIL_BACK_FUNC = ALWAYS;
    ctx->state.glo.STENCIL_BACK_REF = 0;
    ctx->state.glo.STENCIL_BACK_VALUE_MASK = -1;
    ctx->state.glo.STENCIL_WRITEMASK = -1;
    ctx->state.glo.STENCIL_BACK_WRITEMASK = -1;
    ctx->state.glo.STENCIL_FAIL = KEEP;
    ctx->state.glo.STENCIL_PASS_DEPTH_FAIL = KEEP;
    ctx->state.glo.STENCIL_PASS_DEPTH_PASS = KEEP;
    ctx->state.glo.STENCIL_BACK_FAIL = KEEP;
    ctx->state.glo.STENCIL_BACK_PASS_DEPTH_FAIL = KEEP;
    ctx->state.glo.STENCIL_BACK_PASS_DEPTH_PASS = KEEP;
    ctx->state.glo.SCISSOR_BOX[0] = 0;
    ctx->state.glo.SCISSOR_BOX[1] = 0;
    ctx->state.glo.SCISSOR_BOX[2] = 0; // FIXME: Configure at window attach
    ctx->state.glo.SCISSOR_BOX[3] = 0; // FIXME: Configure at window attach
    ctx->state.glo.VIEWPORT[0] = 0;
    ctx->state.glo.VIEWPORT[1] = 0;
    ctx->state.glo.VIEWPORT[2] = 0; // FIXME: Configure at window attach
    ctx->state.glo.VIEWPORT[3] = 0; // FIXME: Configure at window attach
}

void
cwgl_priv_buffer_init(cwgl_buffer_state_t* state){
    state->BUFFER_SIZE = 0;
    state->BUFFER_USAGE = STATIC_DRAW;
}

void
cwgl_priv_framebuffer_attachment_init(cwgl_framebuffer_attachment_state_t* state){
    state->FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE = NONE;
    state->FRAMEBUFFER_ATTACHMENT_OBJECT_NAME.asVoid = 0;
    state->FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL = 0;
    state->FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE = 0;
}

void
cwgl_priv_renderbuffer_init(cwgl_renderbuffer_state_t* state){
    state->RENDERBUFFER_WIDTH = 0;
    state->RENDERBUFFER_HEIGHT = 0;
    state->RENDERBUFFER_INTERNAL_FORMAT = RGBA4;
    state->RENDERBUFFER_RED_SIZE = 0;
    state->RENDERBUFFER_GREEN_SIZE = 0;
    state->RENDERBUFFER_BLUE_SIZE = 0;
    state->RENDERBUFFER_ALPHA_SIZE = 0;
    state->RENDERBUFFER_DEPTH_SIZE = 0;
    state->RENDERBUFFER_STENCIL_SIZE = 0;
}

void
cwgl_priv_texture_init(cwgl_texture_state_t* state){
    state->TEXTURE_MAG_FILTER = LINEAR;
    state->TEXTURE_MIN_FILTER = NEAREST_MIPMAP_LINEAR;
    state->TEXTURE_WRAP_S = REPEAT;
    state->TEXTURE_WRAP_T = REPEAT;
}
