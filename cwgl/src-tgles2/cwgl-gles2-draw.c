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
    // FIXME: Apply texparameter
}

// FIXME: Copied from s2
static cwgl_VertexArrayObject_t*
current_vao(cwgl_ctx_t* ctx){
    if(ctx->state.bin.VERTEX_ARRAY_BINDING){
        return ctx->state.bin.VERTEX_ARRAY_BINDING;
    }else{
        return ctx->state.default_vao;
    }
}

static void
configure_shader(cwgl_ctx_t* ctx){
    int i;
    int ai;
    cwgl_activeinfo_t* u;
    cwgl_activeinfo_t* a;
    cwgl_Program_t* program;
    cwgl_uniformcontent_t* uc;
    cwgl_VertexArrayObject_t* vao;
    cwgl_vao_attrib_state_t* attrib;
    GLint location;
    
    program = ctx->state.bin.CURRENT_PROGRAM;
    if(! program){
        // FIXME: Return error
        // FIXME: Check for not linked
        return;
    }

    glUseProgram(program->backend->name);

    u = program->state.uniforms;
    a = program->state.attributes;
    uc = program->state.uniformcontents;

    /* Configure uniforms */
    for(i=0;i!=program->state.ACTIVE_UNIFORMS;i++){
        switch(u[i].type){
            default:
                /* Do nothing */
                break;
            case FLOAT:
                glUniform1fv(u[i].location,
                             u[i].size,
                             (float *)&uc[u[i].offset]);
                break;
            case INT:
            case BOOL:
            case SAMPLER_2D:
            case SAMPLER_CUBE:
                glUniform1iv(u[i].location,
                             u[i].size,
                             (uint32_t *)&uc[u[i].offset]);
                break;
            case FLOAT_VEC2:
                glUniform2fv(u[i].location,
                             u[i].size,
                             (float *)&uc[u[i].offset]);
                break;
            case INT_VEC2:
            case BOOL_VEC2:
                glUniform2iv(u[i].location,
                             u[i].size,
                             (uint32_t *)&uc[u[i].offset]);
                break;
            case FLOAT_VEC3:
                glUniform3fv(u[i].location,
                             u[i].size,
                             (float *)&uc[u[i].offset]);
                break;
            case INT_VEC3:
            case BOOL_VEC3:
                glUniform3iv(u[i].location,
                             u[i].size,
                             (uint32_t *)&uc[u[i].offset]);
                break;
            case FLOAT_VEC4:
                glUniform4fv(u[i].location,
                             u[i].size,
                             (float *)&uc[u[i].offset]);
                break;
            case INT_VEC4:
            case BOOL_VEC4:
                glUniform4iv(u[i].location,
                             u[i].size,
                             (uint32_t *)&uc[u[i].offset]);
                break;
            case FLOAT_MAT2:
                glUniformMatrix2fv(u[i].location,
                                   u[i].size,
                                   GL_FALSE,
                                   (float *)&uc[u[i].offset]);
                break;
            case FLOAT_MAT3:
                glUniformMatrix3fv(u[i].location,
                                   u[i].size,
                                   GL_FALSE,
                                   (float *)&uc[u[i].offset]);
                break;
            case FLOAT_MAT4:
                glUniformMatrix4fv(u[i].location,
                                   u[i].size,
                                   GL_FALSE,
                                   (float *)&uc[u[i].offset]);
                break;
        }
    }

    /* Configure attributes */
    vao = current_vao(ctx);
    for(i=0;i!=CWGL_MAX_VAO_SIZE;i++){
        ai = program->state.attriblocations[i].active_index;
        if(ai >= 0){
            location = a[ai].location;
            attrib = &vao->attrib[i];
            if(attrib->VERTEX_ATTRIB_ARRAY_ENABLED){
                glEnableVertexAttribArray(location);
                glBindBuffer(GL_ARRAY_BUFFER,
                             attrib->VERTEX_ATTRIB_ARRAY_BUFFER_BINDING->backend->name);
                glVertexAttribPointer(location,
                                      attrib->VERTEX_ATTRIB_ARRAY_SIZE,
                                      attrib->VERTEX_ATTRIB_ARRAY_TYPE,
                                      attrib->VERTEX_ATTRIB_ARRAY_NORMALIZED ?
                                      GL_TRUE : GL_FALSE,
                                      attrib->VERTEX_ATTRIB_ARRAY_STRIDE,
                                      (void*)(uintptr_t)attrib->VERTEX_ATTRIB_ARRAY_POINTER);
            }else{
                glDisableVertexAttribArray(location);
                switch(a[ai].type){
                    default:
                        /* Do nothing */
                        break;
                    case FLOAT:
                        glVertexAttrib1f(location,
                                         attrib->CURRENT_VERTEX_ATTRIB[0]);
                        break;
                    case FLOAT_VEC2:
                        glVertexAttrib2f(location,
                                         attrib->CURRENT_VERTEX_ATTRIB[0],
                                         attrib->CURRENT_VERTEX_ATTRIB[1]);
                        break;
                    case FLOAT_VEC3:
                        glVertexAttrib3f(location,
                                         attrib->CURRENT_VERTEX_ATTRIB[0],
                                         attrib->CURRENT_VERTEX_ATTRIB[1],
                                         attrib->CURRENT_VERTEX_ATTRIB[2]);
                        break;
                    case FLOAT_VEC4:
                        glVertexAttrib4f(location,
                                         attrib->CURRENT_VERTEX_ATTRIB[0],
                                         attrib->CURRENT_VERTEX_ATTRIB[1],
                                         attrib->CURRENT_VERTEX_ATTRIB[2],
                                         attrib->CURRENT_VERTEX_ATTRIB[3]);
                        break;
                    case FLOAT_MAT2:
                        glVertexAttrib2f(location,
                                         attrib->CURRENT_VERTEX_ATTRIB[0],
                                         attrib->CURRENT_VERTEX_ATTRIB[1]);
                        attrib = &vao->attrib[i+1];
                        glVertexAttrib2f(location+1,
                                         attrib->CURRENT_VERTEX_ATTRIB[0],
                                         attrib->CURRENT_VERTEX_ATTRIB[1]);
                        break;
                    case FLOAT_MAT3:
                        glVertexAttrib3f(location,
                                         attrib->CURRENT_VERTEX_ATTRIB[0],
                                         attrib->CURRENT_VERTEX_ATTRIB[1],
                                         attrib->CURRENT_VERTEX_ATTRIB[2]);
                        attrib = &vao->attrib[i+1];
                        glVertexAttrib3f(location+1,
                                         attrib->CURRENT_VERTEX_ATTRIB[0],
                                         attrib->CURRENT_VERTEX_ATTRIB[1],
                                         attrib->CURRENT_VERTEX_ATTRIB[2]);
                        attrib = &vao->attrib[i+2];
                        glVertexAttrib3f(location+2,
                                         attrib->CURRENT_VERTEX_ATTRIB[0],
                                         attrib->CURRENT_VERTEX_ATTRIB[1],
                                         attrib->CURRENT_VERTEX_ATTRIB[2]);
                        break;
                    case FLOAT_MAT4:
                        glVertexAttrib4f(location,
                                         attrib->CURRENT_VERTEX_ATTRIB[0],
                                         attrib->CURRENT_VERTEX_ATTRIB[1],
                                         attrib->CURRENT_VERTEX_ATTRIB[2],
                                         attrib->CURRENT_VERTEX_ATTRIB[3]);
                        attrib = &vao->attrib[i+1];
                        glVertexAttrib4f(location+1,
                                         attrib->CURRENT_VERTEX_ATTRIB[0],
                                         attrib->CURRENT_VERTEX_ATTRIB[1],
                                         attrib->CURRENT_VERTEX_ATTRIB[2],
                                         attrib->CURRENT_VERTEX_ATTRIB[3]);
                        attrib = &vao->attrib[i+2];
                        glVertexAttrib4f(location+2,
                                         attrib->CURRENT_VERTEX_ATTRIB[0],
                                         attrib->CURRENT_VERTEX_ATTRIB[1],
                                         attrib->CURRENT_VERTEX_ATTRIB[2],
                                         attrib->CURRENT_VERTEX_ATTRIB[3]);
                        attrib = &vao->attrib[i+3];
                        glVertexAttrib4f(location+3,
                                         attrib->CURRENT_VERTEX_ATTRIB[0],
                                         attrib->CURRENT_VERTEX_ATTRIB[1],
                                         attrib->CURRENT_VERTEX_ATTRIB[2],
                                         attrib->CURRENT_VERTEX_ATTRIB[3]);
                        break;
                }
            }

        }
    }
}

int 
cwgl_backend_drawArrays(cwgl_ctx_t* ctx, cwgl_enum_t mode, 
                        int32_t first, uint32_t count){
    configure_drawstate(ctx, mode);
    configure_texture(ctx);
    configure_shader(ctx);
    glDrawArrays(mode, first, count);
    return 0;
}


int 
cwgl_backend_drawElements(cwgl_ctx_t* ctx, cwgl_enum_t mode, 
                          uint32_t count, cwgl_enum_t type, uint32_t offset){
    cwgl_VertexArrayObject_t* vao;

    configure_drawstate(ctx, mode);
    configure_texture(ctx);
    configure_shader(ctx);
    /* Configure element buffer */
    vao = current_vao(ctx);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,
                 vao->state.ELEMENT_ARRAY_BUFFER_BINDING->backend->name);
    glDrawElements(mode, count, type, (void*)(uintptr_t)offset);
    return 0;
}

int 
cwgl_backend_clear(cwgl_ctx_t* ctx, uint32_t mask){
    cwgl_ctx_global_state_t* s;
    s = &ctx->state.glo;
    configure_drawstate(ctx, 0);
    /* Configure clear */
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

