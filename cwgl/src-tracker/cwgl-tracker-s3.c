#include "cwgl-tracker-priv.h"

// 3.4 Line Segments
CWGL_API void 
cwgl_lineWidth(cwgl_ctx_t* ctx, float width){
}

// 3.5 Polygons
CWGL_API void 
cwgl_frontFace(cwgl_ctx_t* ctx, cwgl_enum_t mode){
    switch(mode){
        case CW:
        case CCW:
            ctx->state.glo.FRONT_FACE = mode;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
    }
}

CWGL_API void 
cwgl_cullFace(cwgl_ctx_t* ctx, cwgl_enum_t mode){
    switch(mode){
        case FRONT:
        case BACK:
        case FRONT_AND_BACK:
            ctx->state.glo.CULL_FACE_MODE = mode;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            break;
    }
}

// 3.5.2 Depth offset
CWGL_API void 
cwgl_polygonOffset(cwgl_ctx_t* ctx, float factor, float units){
}

// 3.6.1 Pixel Storage Modes
CWGL_API void 
cwgl_pixelStorei(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t param){
}

// 3.7 Texturing
CWGL_API void 
cwgl_activeTexture(cwgl_ctx_t* ctx, cwgl_enum_t texture){
    const int maxtexture = ctx->state.cfg.MAX_COMBINED_TEXTURE_IMAGE_UNITS;
    const int number = (int)texture;
    const int base = (int)TEXTURE0;
    const int diff = number - base;

    if(diff < 0){
        CTX_SET_ERROR(ctx, INVALID_ENUM);
    }else if(diff > maxtexture){
        CTX_SET_ERROR(ctx, INVALID_ENUM);
    }else{
        ctx->state.glo.ACTIVE_TEXTURE = texture;
    }
}

// 3.7.1 Texture Image Specification
CWGL_API void 
cwgl_texImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                int32_t level, cwgl_enum_t internalformat, 
                uint32_t width, uint32_t height, int32_t border, 
                cwgl_enum_t format, cwgl_enum_t type, 
                const void* buf, size_t buflen){
}

// 3.7.2 Alternate Texture Image Specification Commands
CWGL_API void 
cwgl_copyTexImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, cwgl_enum_t internalformat, int32_t x, int32_t y, uint32_t width, uint32_t height, int32_t border){
}

CWGL_API void 
cwgl_texSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height, cwgl_enum_t format, cwgl_enum_t type, const void* buf, size_t buflen){
}

CWGL_API void 
cwgl_copyTexSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, int32_t x, int32_t y, uint32_t width, uint32_t height){
}

// 3.7.3 Compressed Texture Images
CWGL_API void 
cwgl_compressedTexImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, cwgl_enum_t internalformat, uint32_t width, uint32_t height, int32_t border, const void* buf, size_t buflen){
}

CWGL_API void 
cwgl_compressedTexSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height, cwgl_enum_t format, const void* buf, size_t buflen){
}

// 3.7.4 Texture Parameters
CWGL_API void 
cwgl_texParameterf(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t pname, float param){
}

CWGL_API void 
cwgl_texParameteri(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t pname, int32_t param){
}

// 3.7.11 Mipmap Generation
CWGL_API void 
cwgl_generateMipmap(cwgl_ctx_t* ctx, cwgl_enum_t target){
}

// 3.7.13 Texture Objects
CWGL_API void 
cwgl_bindTexture(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_Texture_t* texture){
}

CWGL_API void 
cwgl_deleteTexture(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
}

CWGL_API cwgl_Texture_t* 
cwgl_createTexture(cwgl_ctx_t* ctx){
}

CWGL_API void
cwgl_Texture_release(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
}


