#include "cwgl-backend-gles2-priv.h"
#include <stdlib.h>
#include <stdio.h>

// FIXME: Consider where to decl APIs
cwgl_string_t* cwgl_priv_alloc_string(cwgl_ctx_t* ctx, const char* str, size_t buflen);

int 
cwgl_backend_compileShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    GLuint name;
    char* buf;
    size_t srcsize;
    GLsizei infosize;
    GLint p = GL_FALSE;
    name = shader->backend->name;
    // FIXME: Move this to tracker
    cwgl_string_release(ctx, shader->state.infolog);
    shader->state.infolog = NULL;
    if(! shader->state.source){
        shader->state.COMPILE_STATUS = CWGL_FALSE;
        return 1;
    }
    srcsize = cwgl_string_size(ctx, shader->state.source);
    buf = malloc(srcsize);
    cwgl_string_read(ctx, shader->state.source,
                     buf, srcsize);
    glShaderSource(name,
                   1,
                   &buf,&srcsize);
    free(buf);
    glCompileShader(name);
    glGetShaderiv(name, GL_COMPILE_STATUS, &p);
    shader->state.COMPILE_STATUS = (p == GL_TRUE) ? CWGL_TRUE : CWGL_FALSE;
    glGetShaderiv(name, GL_INFO_LOG_LENGTH, &p);
    if(p >= 0){
        buf = malloc(p);
        glGetShaderInfoLog(name, p, &infosize, buf);
        shader->state.infolog = cwgl_priv_alloc_string(ctx, buf, infosize);
        free(buf);
    }
    return 0;
}

int 
cwgl_backend_linkProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    GLuint name;
    GLuint name_vs;
    GLuint name_fs;
    GLuint uniform_count;
    GLuint attribute_count;
    GLint p = GL_FALSE;
    GLsizei namelen;
    GLint size;
    GLenum type;
    int i;
    int r;
    char* buf;
    GLsizei infosize;
    cwgl_activeinfo_t* a;
    name = program->backend->name;
    // FIXME: Move this to tracker
    cwgl_string_release(ctx, program->state.infolog);
    program->state.infolog = NULL;
    /* Detach previous shaders */
    if(program->backend->attached_vs){
        glDetachShader(name, program->backend->attached_vs);
    }
    program->backend->attached_vs = 0;
    if(program->backend->attached_fs){
        glDetachShader(name, program->backend->attached_fs);
    }
    program->backend->attached_fs = 0;
    /* Attach shaders */
    name_vs = program->state.vertex_shader->backend->name;
    name_fs = program->state.fragment_shader->backend->name;
    program->backend->attached_vs = name_vs;
    program->backend->attached_fs = name_fs;
    glAttachShader(name, name_vs);
    glAttachShader(name, name_fs);
    /* Perform link */
    glLinkProgram(name);
    glGetProgramiv(name, GL_LINK_STATUS, &p);
    program->state.LINK_STATUS = (p == GL_TRUE) ? CWGL_TRUE : CWGL_FALSE;
    glGetProgramiv(name, GL_INFO_LOG_LENGTH, &p);
    if(p >= 0){
        buf = malloc(p);
        glGetProgramInfoLog(name, p, &infosize, buf);
        program->state.infolog = cwgl_priv_alloc_string(ctx, buf, infosize);
        free(buf);
    }
    /* Populate Uniform/Attributes */
    uniform_count = 0;
    attribute_count = 0;
    buf = malloc(1024);
    r = cwgl_integ_program_setup(ctx, program, 
                                 uniform_count, attribute_count);
    if(!r){
        glGetProgramiv(name, GL_ACTIVE_ATTRIBUTES, &attribute_count);
        glGetProgramiv(name, GL_ACTIVE_UNIFORMS, &uniform_count);
        a = program->state.attributes;
        for(i=0;i!=attribute_count;i++){
            glGetActiveAttrib(name, i, 1024, &namelen, &size, &type,
                              buf);
            a[i].type = type;
            a[i].size = size;
            a[i].name = cwgl_priv_alloc_string(ctx, buf, namelen);
        }
        a = program->state.uniforms;
        for(i=0;i!=uniform_count;i++){
            glGetActiveUniform(name, i, 1024, &namelen, &size, &type,
                               buf);
            a[i].type = type;
            a[i].size = size;
            a[i].name = cwgl_priv_alloc_string(ctx, buf, namelen);
        }
    }
    free(buf);
    return 0;
}

int 
cwgl_backend_validateProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    // FIXME: Implement this
    program->state.VALIDATE_STATUS = CWGL_TRUE;
    return 0;
}
