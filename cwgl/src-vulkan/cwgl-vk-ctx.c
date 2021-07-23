#include "cwgl-backend-vk-priv.h"

int
cwgl_backend_cfg_init(cwgl_ctx_config_t* cfg){
    cfg->SUBPIXEL_BITS = 4; // GL2 minimum
    cfg->MAX_TEXTURE_SIZE = 16384; // iPod
    cfg->MAX_CUBE_MAP_TEXTURE_SIZE = 16384; // iPod

    cfg->MAX_VIEWPORT_DIMS[0] = 16384; // iPod
    cfg->MAX_VIEWPORT_DIMS[1] = 16384; // iPod
    cfg->ALIASED_POINT_SIZE_RANGE[0] = 1; // GL2 minimum
    cfg->ALIASED_POINT_SIZE_RANGE[1] = 1; // GL2 minimum
    cfg->ALIASED_LINE_WIDTH_RANGE[0] = 1; // GL2 minimum
    cfg->ALIASED_LINE_WIDTH_RANGE[1] = 1; // GL2 minimum
    cfg->SAMPLE_BUFFERS = 0; // GL2 minimum
    cfg->SAMPLES = 0; // GL2 minimum
    
    cfg->MAX_VERTEX_ATTRIBS = 16; // iPod
    cfg->MAX_VERTEX_UNIFORM_VECTORS = 512; // iPod
    cfg->MAX_VARYING_VECTORS = 15; // iPod
    cfg->MAX_COMBINED_TEXTURE_IMAGE_UNITS = 32; // iPod
    cfg->MAX_VERTEX_TEXTURE_IMAGE_UNITS = 16; // iPod
    cfg->MAX_TEXTURE_IMAGE_UNITS = 16; // iPod
    cfg->MAX_FRAGMENT_UNIFORM_VECTORS = 224; // iPod
    cfg->MAX_RENDERBUFFER_SIZE = 16384; // iPod
    
    // FIXME: serious?
    cfg->shader_precision_format.VERTEX_SHADER.LOW_FLOAT.rangeMin = 126;
    cfg->shader_precision_format.VERTEX_SHADER.LOW_FLOAT.rangeMax = 126;
    cfg->shader_precision_format.VERTEX_SHADER.LOW_FLOAT.precision = 23;
    cfg->shader_precision_format.VERTEX_SHADER.MEDIUM_FLOAT.rangeMin = 126;
    cfg->shader_precision_format.VERTEX_SHADER.MEDIUM_FLOAT.rangeMax = 126;
    cfg->shader_precision_format.VERTEX_SHADER.MEDIUM_FLOAT.precision = 23;
    cfg->shader_precision_format.VERTEX_SHADER.HIGH_FLOAT.rangeMin = 126;
    cfg->shader_precision_format.VERTEX_SHADER.HIGH_FLOAT.rangeMax = 126;
    cfg->shader_precision_format.VERTEX_SHADER.HIGH_FLOAT.precision = 23;
    cfg->shader_precision_format.VERTEX_SHADER.LOW_INT.rangeMin = 24;
    cfg->shader_precision_format.VERTEX_SHADER.LOW_INT.rangeMax = 24;
    cfg->shader_precision_format.VERTEX_SHADER.LOW_INT.precision = 0;
    cfg->shader_precision_format.VERTEX_SHADER.MEDIUM_INT.rangeMin = 24;
    cfg->shader_precision_format.VERTEX_SHADER.MEDIUM_INT.rangeMax = 24;
    cfg->shader_precision_format.VERTEX_SHADER.MEDIUM_INT.precision = 0;
    cfg->shader_precision_format.VERTEX_SHADER.HIGH_INT.rangeMin = 24;
    cfg->shader_precision_format.VERTEX_SHADER.HIGH_INT.rangeMax = 24;
    cfg->shader_precision_format.VERTEX_SHADER.HIGH_INT.precision = 0;

    cfg->shader_precision_format.FRAGMENT_SHADER.LOW_FLOAT.rangeMin = 126;
    cfg->shader_precision_format.FRAGMENT_SHADER.LOW_FLOAT.rangeMax = 126;
    cfg->shader_precision_format.FRAGMENT_SHADER.LOW_FLOAT.precision = 23;
    cfg->shader_precision_format.FRAGMENT_SHADER.MEDIUM_FLOAT.rangeMin = 126;
    cfg->shader_precision_format.FRAGMENT_SHADER.MEDIUM_FLOAT.rangeMax = 126;
    cfg->shader_precision_format.FRAGMENT_SHADER.MEDIUM_FLOAT.precision = 23;
    cfg->shader_precision_format.FRAGMENT_SHADER.HIGH_FLOAT.rangeMin = 126;
    cfg->shader_precision_format.FRAGMENT_SHADER.HIGH_FLOAT.rangeMax = 126;
    cfg->shader_precision_format.FRAGMENT_SHADER.HIGH_FLOAT.precision = 23;
    cfg->shader_precision_format.FRAGMENT_SHADER.LOW_INT.rangeMin = 24;
    cfg->shader_precision_format.FRAGMENT_SHADER.LOW_INT.rangeMax = 24;
    cfg->shader_precision_format.FRAGMENT_SHADER.LOW_INT.precision = 0;
    cfg->shader_precision_format.FRAGMENT_SHADER.MEDIUM_INT.rangeMin = 24;
    cfg->shader_precision_format.FRAGMENT_SHADER.MEDIUM_INT.rangeMax = 24;
    cfg->shader_precision_format.FRAGMENT_SHADER.MEDIUM_INT.precision = 0;
    cfg->shader_precision_format.FRAGMENT_SHADER.HIGH_INT.rangeMin = 24;
    cfg->shader_precision_format.FRAGMENT_SHADER.HIGH_INT.rangeMax = 24;
    cfg->shader_precision_format.FRAGMENT_SHADER.HIGH_INT.precision = 0;

    cfg->RENDERER = "temp";
    cfg->SHADING_LANGUAGE_VERSION = "WebGL GLSL ES 1.0";
    cfg->VENDOR = "temp";
    cfg->VERSION = "WebGL 1.0";

    return 0;
}
