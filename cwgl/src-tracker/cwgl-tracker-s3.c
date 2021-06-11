#include "cwgl-tracker-priv.h"
#include <stdlib.h>

// 3.4 Line Segments
CWGL_API void 
cwgl_lineWidth(cwgl_ctx_t* ctx, float width){
    float cwidth;
    if(width <= 0){
        CTX_SET_ERROR(ctx, INVALID_VALUE);
    }
    if(!(width < ctx->state.cfg.ALIASED_LINE_WIDTH_RANGE[1])){
        cwidth = ctx->state.cfg.ALIASED_LINE_WIDTH_RANGE[1];
    }else{
        cwidth = width;
    }
    /* WebGL: Needs to reject NaN as INVALID_VALUE (FIXME) */
    ctx->state.glo.LINE_WIDTH = cwidth;
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
    ctx->state.glo.POLYGON_OFFSET_FACTOR = factor;
    ctx->state.glo.POLYGON_OFFSET_UNITS = units;
}

// 3.6.1 Pixel Storage Modes
CWGL_API void 
cwgl_pixelStorei(cwgl_ctx_t* ctx, cwgl_enum_t pname, int32_t param){
    switch(pname){
        case PACK_ALIGNMENT:
        case UNPACK_ALIGNMENT:
            switch(param){
                case 1:
                case 2:
                case 4:
                case 8:
                    if(pname == PACK_ALIGNMENT){
                        ctx->state.glo.PACK_ALIGNMENT = param;
                    }else{
                        ctx->state.glo.UNPACK_ALIGNMENT = param;
                    }
                    break;
                default:
                    CTX_SET_ERROR(ctx, INVALID_VALUE);
                    break;
            }
            break;
        case UNPACK_FLIP_Y_WEBGL:
            ctx->state.glo.UNPACK_FLIP_Y_WEBGL = param ? CWGL_TRUE : CWGL_FALSE;
            break;
        case UNPACK_PREMULTIPLY_ALPHA_WEBGL:
            ctx->state.glo.UNPACK_PREMULTIPLY_ALPHA_WEBGL = param ? CWGL_TRUE : CWGL_FALSE;
            break;
        case UNPACK_COLORSPACE_CONVERSION_WEBGL:
            switch((cwgl_enum_t)param){
                case BROWSER_DEFAULT_WEBGL:
                case NONE:
                    ctx->state.glo.UNPACK_COLORSPACE_CONVERSION_WEBGL = param;
                    break;
                default:
                    CTX_SET_ERROR(ctx, INVALID_VALUE);
                    break;
            }
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            break;
    }
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

static cwgl_texture_unit_state_t*
current_texture_unit(cwgl_ctx_t* ctx){
    int id = (ctx->state.glo.ACTIVE_TEXTURE - TEXTURE0);
    if(id < 0){
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return NULL;
    }
    if(id >= CWGL_MAX_TEXTURE_UNITS){
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return NULL;
    }
    return &ctx->state.bin.texture_unit[id];
}

static int /* bool */
validtexture_p(cwgl_ctx_t* ctx, cwgl_enum_t target){
    cwgl_texture_unit_state_t* s;
    int id = (ctx->state.glo.ACTIVE_TEXTURE - TEXTURE0);

    s = current_texture_unit(ctx);
    if(!s){
        return 0;
    }

    switch(target){
        case TEXTURE_2D:
            if(s[id].TEXTURE_BINDING_2D){
                return 1;
            }else{
                CTX_SET_ERROR(ctx, INVALID_OPERATION);
                return 0;
            }
            break;
        case TEXTURE_CUBE_MAP_POSITIVE_X:
        case TEXTURE_CUBE_MAP_NEGATIVE_X:
        case TEXTURE_CUBE_MAP_POSITIVE_Y:
        case TEXTURE_CUBE_MAP_NEGATIVE_Y:
        case TEXTURE_CUBE_MAP_POSITIVE_Z:
        case TEXTURE_CUBE_MAP_NEGATIVE_Z:
            if(s[id].TEXTURE_BINDING_CUBE_MAP){
                return 1;
            }else{
                CTX_SET_ERROR(ctx, INVALID_OPERATION);
                return 0;
            }
            break;

        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return 0;
    }
}

// 3.7.1 Texture Image Specification
CWGL_API void 
cwgl_texImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                int32_t level, cwgl_enum_t internalformat, 
                uint32_t width, uint32_t height, int32_t border, 
                cwgl_enum_t format, cwgl_enum_t type, 
                const void* buf, size_t buflen){
    if(validtexture_p(ctx, target)){
        cwgl_backend_texImage2D(ctx, target, level, internalformat, width, height, border, format, type, buf, buflen);
    }
}

// 3.7.2 Alternate Texture Image Specification Commands
CWGL_API void 
cwgl_copyTexImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, cwgl_enum_t internalformat, int32_t x, int32_t y, uint32_t width, uint32_t height, int32_t border){
    if(validtexture_p(ctx, target)){
        cwgl_backend_copyTexImage2D(ctx, target, level, internalformat, x, y, width, height, border);
    }
}

CWGL_API void 
cwgl_texSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height, cwgl_enum_t format, cwgl_enum_t type, const void* buf, size_t buflen){
    if(validtexture_p(ctx, target)){
        cwgl_backend_texSubImage2D(ctx, target, level, xoffset, yoffset, width, height, format, type, buf, buflen);
    }
}

CWGL_API void 
cwgl_copyTexSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, int32_t x, int32_t y, uint32_t width, uint32_t height){
    if(validtexture_p(ctx, target)){
        cwgl_backend_copyTexSubImage2D(ctx, target, level, xoffset, yoffset, x, y, width, height);
    }
}

// 3.7.3 Compressed Texture Images
CWGL_API void 
cwgl_compressedTexImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, cwgl_enum_t internalformat, uint32_t width, uint32_t height, int32_t border, const void* buf, size_t buflen){
    if(validtexture_p(ctx, target)){
        cwgl_backend_compressedTexImage2D(ctx, target, level, internalformat, width, height, border, buf, buflen);
    }
}

CWGL_API void 
cwgl_compressedTexSubImage2D(cwgl_ctx_t* ctx, cwgl_enum_t target, int32_t level, int32_t xoffset, int32_t yoffset, uint32_t width, uint32_t height, cwgl_enum_t format, const void* buf, size_t buflen){
    if(validtexture_p(ctx, target)){
        cwgl_backend_compressedTexSubImage2D(ctx, target, level, xoffset, yoffset, width, height, format, buf, buflen);
    }
}



// 3.7.4 Texture Parameters
CWGL_API void 
cwgl_texParameterf(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t pname, float param){
    // No floating point param for textures
    CTX_SET_ERROR(ctx, INVALID_ENUM);
}


CWGL_API void 
cwgl_texParameteri(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t pname, int32_t param){
    cwgl_texture_unit_state_t* current;
    cwgl_Texture_t* tex;
    cwgl_enum_t in = (cwgl_enum_t)param;

    current = current_texture_unit(ctx);
    if(! current){
        CTX_SET_ERROR(ctx, INVALID_ENUM);
        return;
    }

    switch(target){
        case TEXTURE_2D:
            tex = current->TEXTURE_BINDING_2D;
            break;
        case TEXTURE_CUBE_MAP:
            tex = current->TEXTURE_BINDING_CUBE_MAP;
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    if(! tex){
        // WebGL specific: No default texture
        CTX_SET_ERROR(ctx, INVALID_OPERATION);
        return;
    }

    switch(pname){
        case TEXTURE_MIN_FILTER:
            switch(in){
                case NEAREST:
                case LINEAR:
                case NEAREST_MIPMAP_NEAREST:
                case LINEAR_MIPMAP_NEAREST:
                case NEAREST_MIPMAP_LINEAR:
                case LINEAR_MIPMAP_LINEAR:
                    tex->state.TEXTURE_MIN_FILTER = in;
                    break;
                default:
                    CTX_SET_ERROR(ctx, INVALID_ENUM);
                    break;
            }
            break;
        case TEXTURE_MAG_FILTER:
            switch(in){
                case NEAREST:
                case LINEAR:
                    tex->state.TEXTURE_MAG_FILTER = in;
                    break;
                default:
                    CTX_SET_ERROR(ctx, INVALID_ENUM);
                    break;
            }
            break;
        case TEXTURE_WRAP_S:
            switch(in){
                case CLAMP_TO_EDGE:
                case MIRRORED_REPEAT:
                case REPEAT:
                    tex->state.TEXTURE_WRAP_S = in;
                    break;
                default:
                    CTX_SET_ERROR(ctx, INVALID_ENUM);
                    break;
            }
            break;
        case TEXTURE_WRAP_T:
            switch(in){
                case CLAMP_TO_EDGE:
                case MIRRORED_REPEAT:
                case REPEAT:
                    tex->state.TEXTURE_WRAP_T = in;
                    break;
                default:
                    CTX_SET_ERROR(ctx, INVALID_ENUM);
                    break;
            }
            break;
    }
}

// 3.7.11 Mipmap Generation
CWGL_API void 
cwgl_generateMipmap(cwgl_ctx_t* ctx, cwgl_enum_t target){
    cwgl_backend_generateMipmap(ctx, target);
}

// 3.7.13 Texture Objects
void
cwgl_priv_texture_release(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
    uintptr_t v;
    if(texture){
        v = cwgl_priv_objhdr_release(&texture->hdr);
        if(! v){
            cwgl_backend_Texture_release(ctx, texture);
            free(texture);
        }
    }
}

CWGL_API void 
cwgl_bindTexture(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_Texture_t* texture){
    cwgl_texture_unit_state_t* state;
    cwgl_Texture_t** point;
    state = current_texture_unit(ctx);
    if(! state){
        return;
    }
    switch(target){
        case TEXTURE_2D:
            point = &state->TEXTURE_BINDING_2D;
            if(texture && texture->state.texture_type == TEXTURE_CUBE_MAP){
                CTX_SET_ERROR(ctx, INVALID_OPERATION);
                return;
            }
            break;
        case TEXTURE_CUBE_MAP:
            point = &state->TEXTURE_BINDING_CUBE_MAP;
            if(texture && texture->state.texture_type == TEXTURE_2D){
                CTX_SET_ERROR(ctx, INVALID_OPERATION);
                return;
            }
            break;
        default:
            CTX_SET_ERROR(ctx, INVALID_ENUM);
            return;
    }
    if(*point){
        cwgl_priv_texture_release(ctx, *point);
    }
    if(texture){
        cwgl_priv_objhdr_retain(&texture->hdr);
        texture->state.texture_type = target;
    }
    *point = texture;
}

static void
unbind_texture(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
    int i;
    if(texture){
        for(i=0;i!=CWGL_MAX_TEXTURE_UNITS;i++){
            if(ctx->state.bin.texture_unit[i].TEXTURE_BINDING_2D == texture){
                cwgl_priv_texture_release(ctx, texture);
                ctx->state.bin.texture_unit[i].TEXTURE_BINDING_2D = NULL;
            }
            if(ctx->state.bin.texture_unit[i].TEXTURE_BINDING_CUBE_MAP == texture){
                cwgl_priv_texture_release(ctx, texture);
                ctx->state.bin.texture_unit[i].TEXTURE_BINDING_CUBE_MAP = NULL;
            }
        }
    }
}

CWGL_API void 
cwgl_deleteTexture(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
    unbind_texture(ctx, texture);
}

CWGL_API cwgl_Texture_t* 
cwgl_createTexture(cwgl_ctx_t* ctx){
    cwgl_Texture_t* t;
    t = malloc(sizeof(cwgl_Texture_t));
    if(t){
        cwgl_priv_objhdr_init(ctx, &t->hdr, CWGL_OBJ_TEXTURE);
        cwgl_priv_texture_init(&t->state);
        cwgl_backend_Texture_init(ctx, t);
    }
    return t;
}

CWGL_API void
cwgl_Texture_release(cwgl_ctx_t* ctx, cwgl_Texture_t* texture){
    cwgl_priv_texture_release(ctx, texture);
}


