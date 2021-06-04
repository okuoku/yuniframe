#include "cwgl-tracker-states.h"
#include <stdint.h>
#include <stdio.h>

#include <GLES2/gl2.h>
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2ext.h>

static int
checkerr(void){
    GLenum c,e;
    c = GL_NO_ERROR;
retry:
    e = glGetError();
    if(e != GL_NO_ERROR){
        if(c == GL_NO_ERROR){
            c = e;
        }
        fprintf(stderr, "GLerr %x\n", e);
        goto retry;
    }
    if(c != GL_NO_ERROR){
        return 1;
    }else{
        return 0;
    }
}

static int
get_int32(GLenum pname, int32_t* i32){
    GLint i[2];
    i[0] = 0;
    glGetIntegerv(pname, &i);
    *i32 = i[0];
    return checkerr();
}

static int
get_int32_2(GLenum pname, int32_t* i32){
    GLint i[2];
    i[0] = 0;
    i[1] = 0;
    glGetIntegerv(pname, &i);
    i32[0] = i[0];
    i32[1] = i[1];
    return checkerr();
}

static int
get_float_2(GLenum pname, float* f32){
    GLfloat f[2];
    f[0] = 0;
    f[1] = 0;
    glGetFloatv(pname, &f);
    f32[0] = f[0];
    f32[1] = f[1];
    return checkerr();
}

static int
fill_shader_prec(GLenum type, GLenum v, cwgl_ctx_config_shaderprecisionformat_t* fmt) {
    int r;
    GLint range[2];
    GLint precision;
    glGetShaderPrecisionFormat(type, v, range, &precision);
    r = checkerr();
    if(r)return r;
    fmt->rangeMin = range[0];
    fmt->rangeMax = range[1];
    fmt->precision = precision;
    return 0;
}

int
cwgl_backend_cfg_init(cwgl_ctx_config_t* cfg){
    int r;
    (void)checkerr(); /* Flush current error */
    r = get_int32(GL_SUBPIXEL_BITS, &cfg->SUBPIXEL_BITS);
    if(r) return r;
    r = get_int32(GL_MAX_TEXTURE_SIZE, &cfg->MAX_TEXTURE_SIZE);
    if(r) return r;
    r = get_int32(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &cfg->MAX_CUBE_MAP_TEXTURE_SIZE);
    if(r) return r;
    r = get_int32_2(GL_MAX_VIEWPORT_DIMS, &cfg->MAX_VIEWPORT_DIMS);
    if(r) return r;
    r = get_float_2(GL_ALIASED_POINT_SIZE_RANGE, &cfg->ALIASED_POINT_SIZE_RANGE);
    if(r) return r;
    r = get_int32(GL_SAMPLE_BUFFERS, &cfg->SAMPLE_BUFFERS);
    if(r) return r;
    r = get_int32(GL_SAMPLES, &cfg->SAMPLES);
    if(r) return r;
    r = get_int32(GL_MAX_VERTEX_ATTRIBS, &cfg->MAX_VERTEX_ATTRIBS);
    if(r) return r;
    r = get_int32(GL_MAX_VERTEX_UNIFORM_VECTORS, &cfg->MAX_VERTEX_UNIFORM_VECTORS);
    if(r) return r;
    r = get_int32(GL_MAX_VARYING_VECTORS, &cfg->MAX_VARYING_VECTORS);
    if(r) return r;
    r = get_int32(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &cfg->MAX_COMBINED_TEXTURE_IMAGE_UNITS);
    if(r) return r;
    r = get_int32(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &cfg->MAX_VERTEX_TEXTURE_IMAGE_UNITS);
    if(r) return r;
    r = get_int32(GL_MAX_TEXTURE_IMAGE_UNITS, &cfg->MAX_TEXTURE_IMAGE_UNITS);
    if(r) return r;
    r = get_int32(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &cfg->MAX_FRAGMENT_UNIFORM_VECTORS);
    if(r) return r;
    r = get_int32(GL_MAX_RENDERBUFFER_SIZE, &cfg->MAX_RENDERBUFFER_SIZE);
    if(r) return r;

    r = fill_shader_prec(GL_VERTEX_SHADER, GL_LOW_FLOAT, &cfg->shader_precision_format.VERTEX_SHADER.LOW_FLOAT);
    if(r) return r;
    r = fill_shader_prec(GL_VERTEX_SHADER, GL_MEDIUM_FLOAT, &cfg->shader_precision_format.VERTEX_SHADER.MEDIUM_FLOAT);
    if(r) return r;
    r = fill_shader_prec(GL_VERTEX_SHADER, GL_HIGH_FLOAT, &cfg->shader_precision_format.VERTEX_SHADER.HIGH_FLOAT);
    if(r) return r;
    r = fill_shader_prec(GL_VERTEX_SHADER, GL_LOW_INT, &cfg->shader_precision_format.VERTEX_SHADER.LOW_INT);
    if(r) return r;
    r = fill_shader_prec(GL_VERTEX_SHADER, GL_MEDIUM_INT, &cfg->shader_precision_format.VERTEX_SHADER.MEDIUM_INT);
    if(r) return r;
    r = fill_shader_prec(GL_VERTEX_SHADER, GL_HIGH_INT, &cfg->shader_precision_format.VERTEX_SHADER.HIGH_INT);
    if(r) return r;
    r = fill_shader_prec(GL_FRAGMENT_SHADER, GL_LOW_FLOAT, &cfg->shader_precision_format.FRAGMENT_SHADER.LOW_FLOAT);
    if(r) return r;
    r = fill_shader_prec(GL_FRAGMENT_SHADER, GL_MEDIUM_FLOAT, &cfg->shader_precision_format.FRAGMENT_SHADER.MEDIUM_FLOAT);
    if(r) return r;
    r = fill_shader_prec(GL_FRAGMENT_SHADER, GL_HIGH_FLOAT, &cfg->shader_precision_format.FRAGMENT_SHADER.HIGH_FLOAT);
    if(r) return r;
    r = fill_shader_prec(GL_FRAGMENT_SHADER, GL_LOW_INT, &cfg->shader_precision_format.FRAGMENT_SHADER.LOW_INT);
    if(r) return r;
    r = fill_shader_prec(GL_FRAGMENT_SHADER, GL_MEDIUM_INT, &cfg->shader_precision_format.FRAGMENT_SHADER.MEDIUM_INT);
    if(r) return r;
    r = fill_shader_prec(GL_FRAGMENT_SHADER, GL_HIGH_INT, &cfg->shader_precision_format.FRAGMENT_SHADER.HIGH_INT);
    if(r) return r;

    cfg->RENDERER = glGetString(GL_RENDERER);
    r = checkerr();
    if(r) return r;
    cfg->SHADING_LANGUAGE_VERSION = glGetString(GL_SHADING_LANGUAGE_VERSION);
    r = checkerr();
    if(r) return r;
    cfg->VENDOR = glGetString(GL_VENDOR);
    r = checkerr();
    if(r) return r;
    cfg->VERSION = glGetString(GL_VERSION);
    r = checkerr();
    if(r) return r;

    return 0;
}
