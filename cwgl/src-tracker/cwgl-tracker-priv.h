#ifndef __YUNI_CWGL_TRACKER_PRIV_H
#define __YUNI_CWGL_TRACKER_PRIV_H

#define CWGL_DECL_ENUMS
#include "cwgl.h"
#include "cwgl-tracker-states.h"

/* Context */
void cwgl_priv_check_current(cwgl_ctx_t* ctx);
cwgl_string_t* cwgl_priv_alloc_string(cwgl_ctx_t* ctx, const char* str,
                                      size_t buflen);
cwgl_string_t* cwgl_priv_string_dup(cwgl_ctx_t* ctx, cwgl_string_t* src);
struct cwgl_string_s {
    size_t size; /* Includes nul */
    char* str;
};

/* Context */
enum cwgl_objtype_e {
    CWGL_OBJ_CTX = 0x299857,
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
    cwgl_buffer_state_t state;
};
struct cwgl_Shader_s {
    cwgl_objhdr_t hdr;
    cwgl_shader_state_t state;
};
struct cwgl_Program_s {
    cwgl_objhdr_t hdr;
    cwgl_program_state_t state;
};
struct cwgl_UniformLocation_s {
    cwgl_objhdr_t hdr;
};

struct cwgl_Texture_s {
    cwgl_objhdr_t hdr;
    cwgl_texture_state_t state;
};

struct cwgl_Renderbuffer_s {
    cwgl_objhdr_t hdr;
    cwgl_renderbuffer_state_t state;
};

struct cwgl_Framebuffer_s {
    cwgl_objhdr_t hdr;
    cwgl_framebuffer_state_t state;
};

#define CWGL_MAX_VAO_SIZE 32
struct cwgl_VertexArrayObject_s {
    cwgl_objhdr_t hdr;
    cwgl_vao_state_t state;
    cwgl_vao_attrib_state_t attrib[CWGL_MAX_VAO_SIZE];
};

/* Initializers */
void cwgl_priv_buffer_init(cwgl_buffer_state_t* state);
void cwgl_priv_framebuffer_attachment_init(cwgl_framebuffer_attachment_state_t* state);
void cwgl_priv_renderbuffer_init(cwgl_renderbuffer_state_t* state);
void cwgl_priv_texture_init(cwgl_texture_state_t* state);
void cwgl_priv_vao_init(cwgl_vao_state_t* state, cwgl_vao_attrib_state_t attrib[CWGL_MAX_VAO_SIZE]);

void cwgl_priv_objhdr_init(cwgl_ctx_t* ctx, cwgl_objhdr_t* hdr, cwgl_objtype_t objtype);
uintptr_t cwgl_priv_objhdr_release(cwgl_objhdr_t* hdr);
void cwgl_priv_objhdr_retain(cwgl_objhdr_t* hdr);

/* xref */
void cwgl_priv_buffer_release(cwgl_Buffer_t* buffer);
void cwgl_priv_texture_release(cwgl_Texture_t* texture);

#define CTX_SET_ERROR(ctx, num) ctx->state.err = num

#endif
