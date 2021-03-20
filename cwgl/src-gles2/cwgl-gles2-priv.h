#ifndef __YUNI_CWGL_GLES2_PRIV_H
#define __YUNI_CWGL_GLES2_PRIV_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <cwgl.h>

/* Context */
void cwgl_priv_check_current(cwgl_ctx_t* ctx);
cwgl_string_t* cwgl_priv_alloc_string(cwgl_ctx_t* ctx, const char* str,
                                      size_t buflen);
struct cwgl_string_s {
    size_t size; /* Includes nul */
    char* str;
};

#define CTX_ENTER(ctx) cwgl_priv_check_current(ctx)
#define CTX_LEAVE(ctx)


/* Buffer structures */
struct cwgl_Buffer_s {
    int name;
};
struct cwgl_Shader_s {
    int name;
};
struct cwgl_Program_s {
    int name;
};
struct cwgl_UniformLocation_s {
    int name;
};

struct cwgl_Texture_s {
    int name;
};

struct cwgl_Renderbuffer_s {
    int name;
};

struct cwgl_Framebuffer_s {
    int name;
};

/* Buffer allocation */
#include <stdlib.h>

#define CTX_ALLOC(ctx, type) \
    malloc(sizeof(cwgl_ ## type ## _t))

#define CTX_FREE(ctx, type, p) \
    free(p)

#define CTX_SETNAME(ctx, p, id) \
    p->name = id

#define CTX_GETNAME(ctx, p) \
    p->name

#endif
