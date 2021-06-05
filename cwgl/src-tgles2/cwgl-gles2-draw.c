#include "cwgl-backend-gles2-priv.h"

static void
configure_drawstate(cwgl_ctx_t* ctx, cwgl_enum_t primitive){
    int line = 0;
    int point = 0;
    int triangle = 0;
    int clear = 0;
    cwgl_ctx_global_state_t* s;
    /* Determine draw type */
    switch(primitive){
        case POINTS:
            point = 1;
            break;
        case LINE_STRIP:
        case LINE_LOOP:
        case LINES:
            line = 1;
            break;
        case TRIANGLE_STRIP:
        case TRIANGLE_FAN:
        case TRIANGLES:
            triangle = 1;
            break;
        default:
            clear = 1;
            break;
    }

    s = &ctx->state.glo;

    glViewport(s->VIEWPORT[0], s->VIEWPORT[1], s->VIEWPORT[2], s->VIEWPORT[3]);
    if(line){
        glLineWidth(s->LINE_WIDTH);
    }
    if(triangle){
        if(s->CULL_FACE){
            glEnable(GL_CULL_FACE);
            glCullFace(s->CULL_FACE_MODE);
            glFrontFace(s->FRONT_FACE);
        }else{
            glDisable(GL_CULL_FACE);
        }
    }
    if(! clear){
        glDepthRangef(s->DEPTH_RANGE[0], s->DEPTH_RANGE[1]);
        if(s->POLYGON_OFFSET_FILL){
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(s->POLYGON_OFFSET_FACTOR, s->POLYGON_OFFSET_UNITS);
        }else{
            glDisable(GL_POLYGON_OFFSET_FILL);
        }
        if(s->SAMPLE_ALPHA_TO_COVERAGE){
            glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        }else{
            glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        }
        if(s->SAMPLE_COVERAGE){
            glEnable(GL_SAMPLE_COVERAGE);
        }else{
            glDisable(GL_SAMPLE_COVERAGE);
        }
        if(s->SAMPLE_COVERAGE || s->SAMPLE_ALPHA_TO_COVERAGE){
            glSampleCoverage(s->SAMPLE_COVERAGE_VALUE,
                             s->SAMPLE_COVERAGE_INVERT ? GL_TRUE : GL_FALSE);
        }
        if(s->STENCIL_TEST){
            glEnable(GL_STENCIL_TEST);
            glStencilFuncSeparate(GL_FRONT,
                                  s->STENCIL_FUNC,
                                  s->STENCIL_REF,
                                  s->STENCIL_VALUE_MASK);
            glStencilFuncSeparate(GL_BACK,
                                  s->STENCIL_BACK_FUNC,
                                  s->STENCIL_BACK_REF,
                                  s->STENCIL_BACK_VALUE_MASK);
            glStencilOpSeparate(GL_FRONT,
                                s->STENCIL_FAIL,
                                s->STENCIL_PASS_DEPTH_FAIL,
                                s->STENCIL_PASS_DEPTH_PASS);
            glStencilOpSeparate(GL_BACK,
                                s->STENCIL_BACK_FAIL,
                                s->STENCIL_BACK_PASS_DEPTH_FAIL,
                                s->STENCIL_BACK_PASS_DEPTH_PASS);
        }else{
            glDisable(GL_STENCIL_TEST);
        }
        if(s->DEPTH_TEST){
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(s->DEPTH_FUNC);
        }else{
            glDisable(GL_DEPTH_TEST);
        }
        if(s->BLEND){
            glEnable(BLEND);
            glBlendFuncSeparate(s->BLEND_SRC_RGB,
                                s->BLEND_SRC_ALPHA,
                                s->BLEND_DST_RGB,
                                s->BLEND_DST_ALPHA);
            glBlendEquationSeparate(s->BLEND_EQUATION_RGB,
                                    s->BLEND_EQUATION_ALPHA);
            glBlendColor(s->BLEND_COLOR[0], s->BLEND_COLOR[1],
                         s->BLEND_COLOR[2], s->BLEND_COLOR[3]);
        }else{
            glDisable(BLEND);
        }
    }
    if(s->SCISSOR_TEST){
        glEnable(GL_SCISSOR_TEST);
        glScissor(s->SCISSOR_BOX[0], s->SCISSOR_BOX[1], s->SCISSOR_BOX[2],
                  s->SCISSOR_BOX[3]);
    }else{
        glDisable(GL_SCISSOR_TEST);
    }
    if(s->DITHER){
        glEnable(GL_DITHER);
    }else{
        glDisable(GL_DITHER);
    }
    glColorMask(s->COLOR_WRITEMASK[0] ? GL_TRUE : GL_FALSE,
                s->COLOR_WRITEMASK[1] ? GL_TRUE : GL_FALSE,
                s->COLOR_WRITEMASK[2] ? GL_TRUE : GL_FALSE,
                s->COLOR_WRITEMASK[3] ? GL_TRUE : GL_FALSE);
    glDepthMask(s->DEPTH_WRITEMASK ? GL_TRUE : GL_FALSE);
    glStencilMaskSeparate(GL_FRONT, s->STENCIL_WRITEMASK);
    glStencilMaskSeparate(GL_BACK, s->STENCIL_WRITEMASK);
}

static void
configure_texture(cwgl_ctx_t* ctx){
    int i;
    int activated;
    cwgl_texture_unit_state_t* s;
    s = ctx->state.bin.texture_unit;
    for(i=0;i!=CWGL_MAX_TEXTURE_UNITS;i++){
        activated = 0;
        if(s[i].TEXTURE_BINDING_2D){
            glActiveTexture(GL_TEXTURE0 + i);
            activated = 1;
            glBindTexture(GL_TEXTURE_2D,
                          s[i].TEXTURE_BINDING_2D->backend->name);
        }
        if(s[i].TEXTURE_BINDING_CUBE_MAP){
            if(! activated){
                glActiveTexture(GL_TEXTURE0 + i);
            }
            glBindTexture(GL_TEXTURE_CUBE_MAP,
                          s[i].TEXTURE_BINDING_CUBE_MAP->backend->name);
        }
    }
}

static void
configure_shader(cwgl_ctx_t* ctx){
    // FIXME: Implement this
}

int 
cwgl_backend_drawArrays(cwgl_ctx_t* ctx, cwgl_enum_t mode, 
                        int32_t first, uint32_t count){
    configure_drawstate(ctx, mode);
    configure_texture(ctx);
    glDrawArrays(mode, first, count);
    return 0;
}


int 
cwgl_backend_drawElements(cwgl_ctx_t* ctx, cwgl_enum_t mode, 
                          uint32_t count, cwgl_enum_t type, uint32_t offset){
    configure_drawstate(ctx, mode);
    configure_texture(ctx);
    glDrawElements(mode, count, type, (void*)(uintptr_t)offset);
    return 0;
}

int 
cwgl_backend_clear(cwgl_ctx_t* ctx, uint32_t mask){
    cwgl_ctx_global_state_t* s;
    s = &ctx->state.glo;
    configure_drawstate(ctx, 0);
    if(mask & GL_COLOR_BUFFER_BIT){
        glClearColor(s->COLOR_CLEAR_VALUE[0],
                     s->COLOR_CLEAR_VALUE[1],
                     s->COLOR_CLEAR_VALUE[2],
                     s->COLOR_CLEAR_VALUE[3]);
    }
    if(mask & GL_DEPTH_BUFFER_BIT){
        glClearDepthf(s->DEPTH_CLEAR_VALUE);
    }
    if(mask & GL_STENCIL_BUFFER_BIT){
        glClearStencil(s->STENCIL_CLEAR_VALUE);
    }
    glClear(mask);
    return 0;
}

