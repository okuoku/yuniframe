#ifndef __YUNI_CWGL_BACKEND_GLES2_PRIV_H

#include "cwgl-tracker-priv.h"
#include "cwgl-backend-states.h"

#include <GLES2/gl2.h>
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2ext.h>

struct cwgl_backend_ctx_s {
    int dummy;
};

struct cwgl_backend_Buffer_s {
    GLuint name;
};

struct cwgl_backend_Shader_s {
    GLuint name;
};

struct cwgl_backend_Program_s {
    GLuint name;
    GLuint attached_vs;
    GLuint attached_fs;
};

struct cwgl_backend_Texture_s {
    GLuint name;
};

struct cwgl_backend_Renderbuffer_s {
    GLuint name;
};

struct cwgl_backend_Framebuffer_s {
    GLuint name;
};

#endif
