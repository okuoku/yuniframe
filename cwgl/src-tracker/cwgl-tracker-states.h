#ifndef __YUNI_CWGL_TRACKER_STATES_H
#define __YUNI_CWGL_TRACKER_STATES_H

#include <stdint.h>
#include "cwgl.h"

#ifdef __cplusplus
extern "C" {
#endif
// }

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
    /* ShaderPrecisionFormat: n/a */
    /* EXTENSIONS: n/a */
    /* RENDERER: n/a */
    /* SHADING_LANGUAGE_VERSION: n/a */
    /* VENDOR: n/a */
    /* VERSION: n/a */
    /* GLES2 Table 6.20 */
    int32_t MAX_VERTEX_ATTRIBS;
    int32_t MAX_VERTEX_UNIFORM_VECTORS;
    int32_t MAX_VARYING_VECTORS;
    int32_t MAX_COMBINED_TEXTURE_IMAGE_UNITS;
    int32_t MAX_VERTEX_TEXTURE_IMAGE_UNITS;
    int32_t MAX_TEXTURE_IMAGE_UNITS;
    int32_t MAX_FRAGMENT_UNIFORM_VECTORS;
    int32_t MAX_RENDERBUFFER_SIZE;
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

typedef struct cwgl_ctx_config_s cwgl_ctx_config_t;

struct cwgl_ctx_global_state_s {
    /* GLES2 Table 6.4 */
    int32_t VIEWPORT[4];
    float DEPTH_RANGE[2];
    /* GLES2 Table 6.5 */
    float LINE_WIDTH;
    cwgl_bool_t CULL_FACE;
    cwgl_enum_t CULL_FACE_MODE;
    cwgl_enum_t FRONT_FACE;
    float POLYGON_OFFSET_FACTOR;
    float POLYGON_OFFSET_UNITS;
    cwgl_bool_t POLYGON_OFFSET_FILL;
    /* GLES2 Table 6.6 */
    cwgl_bool_t SAMPLE_ALPHA_TO_COVERAGE;
    cwgl_bool_t SAMPLE_COVERAGE;
    float SAMPLE_COVERAGE_VALUE;
    cwgl_bool_t SAMPLE_COVERAGE_INVERT;
    /* GLES2 Table 6.9 */
    cwgl_enum_t ACTIVE_TEXTURE;
    /* GLES2 Table 6.10 */
    cwgl_bool_t SCISSOR_TEST;
    int32_t SCISSOR_BOX[4];
    cwgl_bool_t STENCIL_TEST;
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
    cwgl_bool_t DEPTH_TEST;
    cwgl_enum_t DEPTH_FUNC;
    /* GLES2 Table 6.11 */
    cwgl_bool_t BLEND;
    cwgl_enum_t BLEND_SRC_RGB; /* 1.1 BLEND_SRC */
    cwgl_enum_t BLEND_SRC_ALPHA;
    cwgl_enum_t BLEND_DST_RGB; /* 1.1 BLEND_DST */
    cwgl_enum_t BLEND_DST_ALPHA;
    cwgl_enum_t BLEND_EQUATION_RGB; /* 1.1 BLEND_EQUATION */
    cwgl_enum_t BLEND_EQUATION_ALPHA;
    float BLEND_COLOR[4]; /* Not In the table (WebGL) */
    cwgl_bool_t DITHER;
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

struct cwgl_ctx_bindings_s {
    /* GLES2 Table 6.2 */
    cwgl_Buffer_t* ARRAY_BUFFER_BINDING;
    cwgl_Buffer_t* ELEMENT_ARRAY_BUFFER_BINDING;
    /* GLES2 Table 6.15 */
    cwgl_Program_t* CURRENT_PROGRAM; /* FIXME: ??? */
    /* GLES2 Table 6.7 */
    cwgl_Texture_t* TEXTURE_BINDING_2D;
    cwgl_Texture_t* TEXTURE_BINDING_CUBE_MAP;
    /* GLES2 Table 6.24 */
    cwgl_Framebuffer_t* FRAMEBUFFER_BINDING;
    /* GLES2 Table 6.23 */
    cwgl_Renderbuffer_t* RENDERBUFFER_BINDING;
};

typedef struct cwgl_ctx_bindings_s cwgl_ctx_bindings_t;

struct cwgl_ctx_tracker_state_s {
    /* FIXME: VENDOR, VERSION strings */
    /* FIXME: Shader prec. */
    cwgl_ctx_config_t cfg;
    cwgl_ctx_global_state_t glo;
    cwgl_ctx_bindings_t bin;
};

typedef struct cwgl_ctx_tracker_state_s cwgl_ctx_tracker_state_t;

// {
#ifdef __cplusplus
};
#endif

#endif
