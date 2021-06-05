#ifndef __YUNI_CWGL_TRACKER_STATES_H
#define __YUNI_CWGL_TRACKER_STATES_H

#include <stdint.h>
#include "cwgl.h"

#ifdef __cplusplus
extern "C" {
#endif
// }

struct cwgl_ctx_config_shaderprecisionformat_s {
    int32_t rangeMin;
    int32_t rangeMax;
    int32_t precision;
};
typedef struct cwgl_ctx_config_shaderprecisionformat_s cwgl_ctx_config_shaderprecisionformat_t;

struct cwgl_ctx_config_shaderprecisiontype_s {
    cwgl_ctx_config_shaderprecisionformat_t LOW_FLOAT;
    cwgl_ctx_config_shaderprecisionformat_t MEDIUM_FLOAT;
    cwgl_ctx_config_shaderprecisionformat_t HIGH_FLOAT;
    cwgl_ctx_config_shaderprecisionformat_t LOW_INT;
    cwgl_ctx_config_shaderprecisionformat_t MEDIUM_INT;
    cwgl_ctx_config_shaderprecisionformat_t HIGH_INT;
};
typedef struct cwgl_ctx_config_shaderprecisiontype_s cwgl_ctx_config_shaderprecisiontype_t;

struct cwgl_ctx_config_s {
    /* GLES2 Table 6.18 */
    int32_t SUBPIXEL_BITS;
    int32_t MAX_TEXTURE_SIZE;
    int32_t MAX_CUBE_MAP_TEXTURE_SIZE;
    int32_t MAX_VIEWPORT_DIMS[2];
    float ALIASED_POINT_SIZE_RANGE[2];
    float ALIASED_LINE_WIDTH_RANGE[2];
    int32_t SAMPLE_BUFFERS;
    int32_t SAMPLES;
    /* COMPRESSED_TEXTURE_FORMAT: Handled specially */
    /* NUM_COMPRESSED_TEXTURE_FORMAT: n/a */
    /* GLES2 Table 6.19 */
    /* SHADER_BINARY_FORMATS: n/a */
    /* NUM_SHADER_BINARY_FORMATS: n/a */
    /* SHADER_COMPILER: n/a */
    /* GLES2 Table 6.20 */
    int32_t MAX_VERTEX_ATTRIBS;
    int32_t MAX_VERTEX_UNIFORM_VECTORS;
    int32_t MAX_VARYING_VECTORS;
    int32_t MAX_COMBINED_TEXTURE_IMAGE_UNITS;
    int32_t MAX_VERTEX_TEXTURE_IMAGE_UNITS;
    int32_t MAX_TEXTURE_IMAGE_UNITS;
    int32_t MAX_FRAGMENT_UNIFORM_VECTORS;
    int32_t MAX_RENDERBUFFER_SIZE;
    /* GLES2 6.1.8, not in table */
    struct {
        cwgl_ctx_config_shaderprecisiontype_t VERTEX_SHADER;
        cwgl_ctx_config_shaderprecisiontype_t FRAGMENT_SHADER;
    } shader_precision_format;
    /* Strings */
    const char* RENDERER;
    const char* SHADING_LANGUAGE_VERSION;
    const char* VENDOR;
    const char* VERSION;
};

typedef struct cwgl_ctx_config_s cwgl_ctx_config_t;

struct cwgl_ctx_global_state_s {
    /* GLES2 Table 6.4 */
    int32_t VIEWPORT[4];
    float DEPTH_RANGE[2];
    /* GLES2 Table 6.5 */
    float LINE_WIDTH;
    cwgl_bool_t CULL_FACE; /* enable */
    cwgl_enum_t CULL_FACE_MODE;
    cwgl_enum_t FRONT_FACE;
    float POLYGON_OFFSET_FACTOR;
    float POLYGON_OFFSET_UNITS;
    cwgl_bool_t POLYGON_OFFSET_FILL; /* enable */
    /* GLES2 Table 6.6 */
    cwgl_bool_t SAMPLE_ALPHA_TO_COVERAGE; /* enable */
    cwgl_bool_t SAMPLE_COVERAGE; /* enable */
    float SAMPLE_COVERAGE_VALUE;
    cwgl_bool_t SAMPLE_COVERAGE_INVERT;
    /* GLES2 Table 6.9 */
    cwgl_enum_t ACTIVE_TEXTURE;
    /* GLES2 Table 6.10 */
    cwgl_bool_t SCISSOR_TEST; /* enable */
    int32_t SCISSOR_BOX[4];
    cwgl_bool_t STENCIL_TEST; /* enable */
    cwgl_enum_t STENCIL_FUNC;
    uint32_t STENCIL_VALUE_MASK;
    int32_t STENCIL_REF;
    cwgl_enum_t STENCIL_FAIL;
    cwgl_enum_t STENCIL_PASS_DEPTH_FAIL;
    cwgl_enum_t STENCIL_PASS_DEPTH_PASS;
    cwgl_enum_t STENCIL_BACK_FUNC;
    uint32_t STENCIL_BACK_VALUE_MASK;
    int32_t STENCIL_BACK_REF;
    cwgl_enum_t STENCIL_BACK_FAIL;
    cwgl_enum_t STENCIL_BACK_PASS_DEPTH_FAIL;
    cwgl_enum_t STENCIL_BACK_PASS_DEPTH_PASS;
    cwgl_bool_t DEPTH_TEST; /* enable */
    cwgl_enum_t DEPTH_FUNC;
    /* GLES2 Table 6.11 */
    cwgl_bool_t BLEND; /* enable */
    cwgl_enum_t BLEND_SRC_RGB; /* 1.1 BLEND_SRC */
    cwgl_enum_t BLEND_SRC_ALPHA;
    cwgl_enum_t BLEND_DST_RGB; /* 1.1 BLEND_DST */
    cwgl_enum_t BLEND_DST_ALPHA;
    cwgl_enum_t BLEND_EQUATION_RGB; /* 1.1 BLEND_EQUATION */
    cwgl_enum_t BLEND_EQUATION_ALPHA;
    float BLEND_COLOR[4]; /* Not In the table (WebGL) */
    cwgl_bool_t DITHER; /* enable */
    /* GLES2 Table 6.12 */
    cwgl_bool_t COLOR_WRITEMASK[4];
    cwgl_bool_t DEPTH_WRITEMASK;
    uint32_t STENCIL_WRITEMASK;
    uint32_t STENCIL_BACK_WRITEMASK;
    float COLOR_CLEAR_VALUE[4];
    float DEPTH_CLEAR_VALUE;
    int32_t STENCIL_CLEAR_VALUE;
    /* GLES2 Table 6.13 */
    int32_t UNPACK_ALIGNMENT;
    int32_t PACK_ALIGNMENT;
    /* GLES2 Table 6.17 */
    cwgl_enum_t GENERATE_MIPMAP_HINT;
    /* WebGL1 */
    cwgl_enum_t UNPACK_COLORSPACE_CONVERSION_WEBGL;
    cwgl_bool_t UNPACK_FLIP_Y_WEBGL;
    cwgl_bool_t UNPACK_PREMULTIPLY_ALPHA_WEBGL;
};

typedef struct cwgl_ctx_global_state_s cwgl_ctx_global_state_t;

struct cwgl_texture_unit_state_s {
    cwgl_Texture_t* TEXTURE_BINDING_2D;
    cwgl_Texture_t* TEXTURE_BINDING_CUBE_MAP;
};

typedef struct cwgl_texture_unit_state_s cwgl_texture_unit_state_t;

#define CWGL_MAX_TEXTURE_UNITS 32
struct cwgl_ctx_bindings_s {
    /* GLES2 Table 6.2 */
    cwgl_Buffer_t* ARRAY_BUFFER_BINDING;
    /* ELEMENT_ARRAY_BUFFER_BINDING: gone to VAO */
    /* GLES2 Table 6.15 */
    cwgl_Program_t* CURRENT_PROGRAM;
    /* GLES2 Table 6.7 */
    cwgl_texture_unit_state_t texture_unit[CWGL_MAX_TEXTURE_UNITS];
    /* GLES2 Table 6.24 */
    cwgl_Framebuffer_t* FRAMEBUFFER_BINDING;
    /* GLES2 Table 6.23 */
    cwgl_Renderbuffer_t* RENDERBUFFER_BINDING;
    /* OES_vertex_array_object */
    cwgl_VertexArrayObject_t* VERTEX_ARRAY_BINDING;
};

typedef struct cwgl_ctx_bindings_s cwgl_ctx_bindings_t;

struct cwgl_ctx_tracker_state_s {
    /* GLES2 Table 6.22 */
    cwgl_enum_t err;
    cwgl_ctx_config_t cfg;
    cwgl_ctx_global_state_t glo;
    cwgl_ctx_bindings_t bin;

    cwgl_VertexArrayObject_t* default_vao;
};

typedef struct cwgl_ctx_tracker_state_s cwgl_ctx_tracker_state_t;

struct cwgl_buffer_state_s {
    uint32_t BUFFER_SIZE;
    cwgl_enum_t BUFFER_USAGE;
};

typedef struct cwgl_buffer_state_s cwgl_buffer_state_t;

struct cwgl_framebuffer_attachment_state_s {
    cwgl_enum_t FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE;
    union {
        void* asVoid;
        cwgl_Texture_t* asTexture;
        cwgl_Renderbuffer_t* asRenderbuffer;
    } FRAMEBUFFER_ATTACHMENT_OBJECT_NAME;
    uint32_t FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL;
    cwgl_enum_t FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE;
};

typedef struct cwgl_framebuffer_attachment_state_s 
cwgl_framebuffer_attachment_state_t;

struct cwgl_framebuffer_state_s {
    cwgl_framebuffer_attachment_state_t COLOR_ATTACHMENT0;
    cwgl_framebuffer_attachment_state_t DEPTH_ATTACHMENT;
    cwgl_framebuffer_attachment_state_t STENCIL_ATTACHMENT;
    /* GLES2 Table 6.21 */
    int32_t RED_BITS;
    int32_t GREEN_BITS;
    int32_t BLUE_BITS;
    int32_t ALPHA_BITS;
    int32_t DEPTH_BITS;
    int32_t STENCIL_BITS;
    cwgl_enum_t IMPLEMENTATION_COLOR_READ_TYPE;
    cwgl_enum_t IMPLEMENTATION_COLOR_READ_FORMAT;
};

typedef struct cwgl_framebuffer_state_s cwgl_framebuffer_state_t;

struct cwgl_renderbuffer_state_s {
    int32_t RENDERBUFFER_WIDTH;
    int32_t RENDERBUFFER_HEIGHT;
    cwgl_enum_t RENDERBUFFER_INTERNAL_FORMAT;
    int32_t RENDERBUFFER_RED_SIZE;
    int32_t RENDERBUFFER_GREEN_SIZE;
    int32_t RENDERBUFFER_BLUE_SIZE;
    int32_t RENDERBUFFER_ALPHA_SIZE;
    int32_t RENDERBUFFER_DEPTH_SIZE;
    int32_t RENDERBUFFER_STENCIL_SIZE;
};

typedef struct cwgl_renderbuffer_state_s cwgl_renderbuffer_state_t;

struct cwgl_texture_state_s {
    cwgl_enum_t TEXTURE_MAG_FILTER;
    cwgl_enum_t TEXTURE_MIN_FILTER;
    cwgl_enum_t TEXTURE_WRAP_S;
    cwgl_enum_t TEXTURE_WRAP_T;
    /* Maintained by tracker, not in the spec */
    cwgl_enum_t texture_type;
};

typedef struct cwgl_texture_state_s cwgl_texture_state_t;

struct cwgl_activeinfo_s {
    cwgl_enum_t type;
    uint32_t size; /* 1 for non-array */
    cwgl_string_t* name; /* allocated by backend */
};

typedef struct cwgl_activeinfo_s cwgl_activeinfo_t;

union cwgl_uniformcontent_u {
    struct {
        float v0;
        float v1;
        float v2;
        float v3;
    }asFloat;
    struct {
        int32_t v0;
        int32_t v1;
        int32_t v2;
        int32_t v3;
    }asInt;
    struct {
        float m[2*2];
    }asMat2;
    struct {
        float m[3*3];
    }asMat3;
    struct {
        float m[4*4];
    }asMat4;
};

typedef union cwgl_uniformcontent_u cwgl_uniformcontent_t;

struct cwgl_program_state_s {
    cwgl_bool_t DELETE_STATUS;
    cwgl_bool_t LINK_STATUS;
    cwgl_bool_t VALIDATE_STATUS;
    uint32_t ATTACHED_SHADERS;
    uint32_t ACTIVE_ATTRIBUTES;
    uint32_t ACTIVE_UNIFORMS;
    /* Maintained by tracker, not in the spec */
    cwgl_activeinfo_t* uniforms;
    cwgl_uniformcontent_t* uniformcontents;
    cwgl_activeinfo_t* attributes;
    cwgl_Shader_t* vertex_shader;
    cwgl_Shader_t* fragment_shader;
    cwgl_string_t* infolog; /* allocated by backend */
};

typedef struct cwgl_program_state_s cwgl_program_state_t;

struct cwgl_shader_state_s {
    cwgl_enum_t SHADER_TYPE;
    cwgl_bool_t DELETE_STATUS;
    cwgl_bool_t COMPILE_STATUS;
    /* Maintained by tracker, not in the spec */
    cwgl_string_t* source;
    cwgl_string_t* infolog; /* allocated by backend */
};

typedef struct cwgl_shader_state_s cwgl_shader_state_t;

struct cwgl_vao_attrib_state_s {
    /* getVertexAttrib */
    cwgl_Buffer_t* VERTEX_ATTRIB_ARRAY_BUFFER_BINDING;
    cwgl_bool_t VERTEX_ATTRIB_ARRAY_ENABLED;
    uint32_t VERTEX_ATTRIB_ARRAY_SIZE;
    uint32_t VERTEX_ATTRIB_ARRAY_STRIDE;
    cwgl_enum_t VERTEX_ATTRIB_ARRAY_TYPE;
    cwgl_bool_t VERTEX_ATTRIB_ARRAY_NORMALIZED;
    float CURRENT_VERTEX_ATTRIB[4];
    /* getVertexAttribOffset */
    uint32_t VERTEX_ATTRIB_ARRAY_POINTER;
};

struct cwgl_vao_state_s {
    /* getParameter */
    cwgl_Buffer_t* ELEMENT_ARRAY_BUFFER_BINDING;
};

typedef struct cwgl_vao_state_s cwgl_vao_state_t;
typedef struct cwgl_vao_attrib_state_s cwgl_vao_attrib_state_t;

// {
#ifdef __cplusplus
};
#endif

#endif
