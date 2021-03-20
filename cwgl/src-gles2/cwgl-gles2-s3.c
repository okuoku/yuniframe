#include "cwgl-gles2-priv.h"

// 3.4 Line Segments
CWGL_API void 
cwgl_lineWidth(cwgl_ctx_t* ctx, float width){
    CTX_ENTER(ctx);
    glLineWidth(width);
    CTX_LEAVE(ctx);
}

// 3.5 Polygons
CWGL_API void 
cwgl_frontFace(cwgl_ctx_t* ctx, cwgl_enum_t mode){
    CTX_ENTER(ctx);
    glFrontFace(mode);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_cullFace(cwgl_ctx_t* ctx, cwgl_enum_t mode){
    CTX_ENTER(ctx);
    glCullFace(mode);
    CTX_LEAVE(ctx);
}

// 3.5.2 Depth offset
CWGL_API void 
cwgl_polygonOffset(cwgl_ctx_t* ctx, float factor, float units){
    CTX_ENTER(ctx);
    glPolygonOffset(factor, units);
    CTX_LEAVE(ctx);
}

// 3.6.1 Pixel Storage Modes
CWGL_API void 
cwgl_pixelStorei(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t param){
    CTX_ENTER(ctx);
    // FIXME: Implement Y-flip ??
    glPixelStorei(pname, param);
    CTX_LEAVE(ctx);
}

// 3.7 Texturing
CWGL_API void 
cwgl_activeTexture(cwgl_ctx_t* ctx, cwgl_enum_t texture){
    CTX_ENTER(ctx);
    glActiveTexture(texture);
    CTX_LEAVE(ctx);
}

// 3.7.1 Texture Image Specification
CWGL_API void 
cwgl_texImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                int32_t level, cwgl_enum_t internalformat, 
                uint32_t width, uint32_t height, int32_t border, 
                cwgl_enum_t format, cwgl_enum_t type, 
                const void* buf, size_t buflen){
    CTX_ENTER(ctx);
    // FIXME: Check bufsize??
    glTexImage2D(target, level, internalformat, width, height, border,
                 format, type, buf);
    CTX_LEAVE(ctx);
}

// 3.7.2 Alternate Texture Image Specification Commands
CWGL_API void 
cwgl_copyTexImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, cwgl_enum_t internalformat, int32_t x, int32_t y, uint32_t width, uint32_t height, int32_t border){
    CTX_ENTER(ctx);
    glCopyTexImage2D(target, level, internalformat,
                     x, y, width, height, border);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_texSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height, cwgl_enum_t format, cwgl_enum_t type, const void* buf, size_t buflen){
    CTX_ENTER(ctx);
    // FIXME: Check bufsize??
    glTexSubImage2D(target, level, 
                    xoffset, yoffset, width, height,
                    format, type, buf);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_copyTexSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, int32_t x, int32_t y, uint32_t width, uint32_t height){
    CTX_ENTER(ctx);
    glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
    CTX_LEAVE(ctx);
}

// 3.7.3 Compressed Texture Images
CWGL_API void 
cwgl_compressedTexImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, cwgl_enum_t internalformat, uint32_t width, uint32_t height, int32_t border, const void* buf, size_t buflen){
    CTX_ENTER(ctx);
    // NB: Buflen for imagesize
    glCompressedTexImage2D(target, level, internalformat,
                           width, height, border, buflen, buf);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_compressedTexSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height, cwgl_enum_t format, const void* buf, size_t buflen){
    CTX_ENTER(ctx);
    // NB: Buflen for imagesize
    glCompressedTexSubImage2D(target, level,
                              xoffset, yoffset, width, height,
                              format, buflen, buf);
    CTX_LEAVE(ctx);
}

// 3.7.4 Texture Parameters
CWGL_API void 
cwgl_texParameterf(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t pname, float param){
    CTX_ENTER(ctx);
    glTexParameterf(target, pname, param);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_texParameteri(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t pname, int32_t param){
    CTX_ENTER(ctx);
    glTexParameteri(target, pname, param);
    CTX_LEAVE(ctx);
}

// 3.7.11 Mipmap Generation
CWGL_API void 
cwgl_generateMipmap(cwgl_ctx_t* ctx, cwgl_enum_t target){
    CTX_ENTER(ctx);
    glGenerateMipmap(target);
    CTX_LEAVE(ctx);
}

// 3.7.13 Texture Objects
CWGL_API void 
cwgl_bindTexture(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_Texture_t* texture){
    GLuint name;
    CTX_ENTER(ctx);
    if(texture){
        name = CTX_GETNAME(ctx, texture);
    }else{
        name = 0;
    }
    glBindTexture(target, name);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_deleteTexture(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
    GLuint name;
    CTX_ENTER(ctx);
    if(texture){
        name = CTX_GETNAME(ctx, texture);
        glDeleteTextures(1, &name);
    }
    CTX_LEAVE(ctx);
}

CWGL_API cwgl_Texture_t* 
cwgl_createTexture(cwgl_ctx_t* ctx){
    GLuint name = 0;
    cwgl_Texture_t* t;
    CTX_ENTER(ctx);
    glGenTextures(1, &name);
    t = CTX_ALLOC(ctx, Texture);
    CTX_SETNAME(ctx, t, name);
    CTX_LEAVE(ctx);
    return t;
}

CWGL_API void
cwgl_Texture_release(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
    //CTX_ENTER(ctx);
    if(texture){
        CTX_FREE(ctx, Texture, texture);
    }
    //CTX_LEAVE(ctx);
}


