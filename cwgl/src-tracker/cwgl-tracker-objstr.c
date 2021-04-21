#include "cwgl-tracker-priv.h"
#include <string.h>

cwgl_string_t* 
cwgl_priv_alloc_string(cwgl_ctx_t* ctx, const char* str, size_t buflen){
    cwgl_string_t* r;
    void* buf;
    buf = malloc(buflen);
    r = malloc(sizeof(cwgl_string_t));
    memcpy(buf, str, buflen);
    r->size = buflen;
    r->str = buf;
    return r;
}

CWGL_API size_t
cwgl_string_size(cwgl_ctx_t* ctx, cwgl_string_t* str){
    if(str){
        return str->size;
    }else{
        return 0;
    }
}

CWGL_API cwgl_query_result_t
cwgl_string_read(cwgl_ctx_t* ctx, cwgl_string_t* str, char* buf, size_t buflen){
    const size_t siz = cwgl_string_size(ctx, str);
    if(str){
        if(buflen >= siz){
            memcpy(buf, str->str, siz);
            return CWGL_QR_SUCCESS;
        }else{
            return CWGL_QR_INSUFFICIENT_BUFFER;
        }
    }else{
        if(buflen > 0){
            buf[0] = 0;
            return CWGL_QR_SUCCESS;
        }else{
            return CWGL_QR_INSUFFICIENT_BUFFER;
        }
    }
}

CWGL_API void
cwgl_string_release(cwgl_ctx_t* ctx, cwgl_string_t* str){
    if(str){
        free(str->str);
        free(str);
    }
}
