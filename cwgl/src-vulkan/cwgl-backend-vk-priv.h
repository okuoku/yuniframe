#ifndef __YUNI_CWGL_BACKEND_VK_PRIV_H
#define __YUNI_CWGL_BACKEND_VK_PRIV_H

#include "cwgl-tracker-priv.h"
#include "cwgl-backend-states.h"

#include "shxm.h"

struct cwgl_backend_ctx_s {
    shxm_ctx_t* shxm_ctx;
};

struct cwgl_backend_Buffer_s {
    int dummy;
};

struct cwgl_backend_Shader_s {
    shxm_shader_t* shader;
};

struct cwgl_backend_Program_s {
    shxm_program_t* program;
};

struct cwgl_backend_Texture_s {
    int dummy;
};

struct cwgl_backend_Renderbuffer_s {
    int dummy;
};

struct cwgl_backend_Framebuffer_s {
    int dummy;
};


#endif
