#ifndef __YUNI_CWGL_TRACKER_PRIV_H
#define __YUNI_CWGL_TRACKER_PRIV_H

#define CWGL_DECL_ENUMS
#include "cwgl.h"
#include "cwgl-tracker-states.h"

/* Context */
void cwgl_priv_check_current(cwgl_ctx_t* ctx);
cwgl_string_t* cwgl_priv_alloc_string(cwgl_ctx_t* ctx, const char* str,
                                      size_t buflen);
struct cwgl_string_s {
    size_t size; /* Includes nul */
    char* str;
};

/* Context */
enum cwgl_objtype_e {
    CWGL_OBJ_CTX = 0x9857,
    CWGL_OBJ_BUFFER,
    CWGL_OBJ_SHADER,
    CWGL_OBJ_PROGRAM,
    CWGL_OBJ_UNIFORMLOCATION,
    CWGL_OBJ_TEXTURE,
    CWGL_OBJ_RENDERBUFFER,
    CWGL_OBJ_FRAMEBUFFER,
    CWGL_OBJ_VAO
};
typedef enum cwgl_objtype_e cwgl_objtype_t;

struct cwgl_objhdr_s {
    cwgl_objtype_t type;
    uintptr_t refcnt;
    cwgl_ctx_t* ctx;
};

typedef struct cwgl_objhdr_s cwgl_objhdr_t;

struct cwgl_ctx_s {
    cwgl_ctx_tracker_state_t state;
};

/* Buffer structures */
struct cwgl_Buffer_s {
    cwgl_objhdr_t hdr;
};
struct cwgl_Shader_s {
    cwgl_objhdr_t hdr;
};
struct cwgl_Program_s {
    cwgl_objhdr_t hdr;
};
struct cwgl_UniformLocation_s {
    cwgl_objhdr_t hdr;
};

struct cwgl_Texture_s {
    cwgl_objhdr_t hdr;
};

struct cwgl_Renderbuffer_s {
    cwgl_objhdr_t hdr;
};

struct cwgl_Framebuffer_s {
    cwgl_objhdr_t hdr;
};

struct cwgl_VertexArrayObject_s {
    cwgl_objhdr_t hdr;
};

#define CTX_SET_ERROR(ctx, num) ctx->state.err = num

#endif
