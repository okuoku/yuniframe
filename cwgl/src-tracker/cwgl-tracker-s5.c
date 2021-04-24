#include "cwgl-tracker-priv.h"

// 5.1 Flush and Finish
CWGL_API void 
cwgl_finish(cwgl_ctx_t* ctx){
}

CWGL_API void 
cwgl_flush(cwgl_ctx_t* ctx){
}

// 5.2 Hints
CWGL_API void 
cwgl_hint(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_enum_t mode){
    int accepted = 0;
    switch(target){
        case GENERATE_MIPMAP_HINT:
            switch(mode){
                case FASTEST:
                case NICEST:
                case DONT_CARE:
                    accepted = 1;
                    ctx->state.glo.GENERATE_MIPMAP_HINT = mode;
                    break;
                default:
                    break;
            }
        default:
            break;

    }
    if(! accepted){
        CTX_SET_ERROR(ctx, INVALID_ENUM);
    }
}

