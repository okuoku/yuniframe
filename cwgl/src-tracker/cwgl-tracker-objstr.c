#include "cwgl-tracker-priv.h"
#include <string.h>
#include <stdlib.h>

cwgl_string_t* 
cwgl_priv_alloc_string(cwgl_ctx_t* ctx, const char* str, size_t buflen){
    cwgl_string_t* r;
    char* buf;
    buf = malloc(buflen+1);
    if(buf){
        r = malloc(sizeof(cwgl_string_t));
        if(!r){
            free(buf);
            return NULL;
        }
        memcpy(buf, str, buflen);
        buf[buflen] = 0;
        r->size = buflen;
        r->str = buf;
    }else{
        return NULL;
    }

    return r;
}

static int /* -1 for non-digit */
fetchdigit(const char* c){
    int r;
    r = c - '0';
    if((r >= 0) && (r < 10)){
        return r;
    }else{
        return -1;
    }
}

static void
parsesym(const char* sym, int len, int* out_symlen, int* out_idx){
    /* Split symbol to basename and array index (-1 for non-array) */
    int i;
    int r;
    int acc;
    int array_idx_start = -1;
    int array_idx_end = -1;
    int sym_end = -1;
    /* Parse */
    i=0;
    while(1){
        if(i == len || sym[i] == 0){
            if(sym_end == -1){
                sym_end = i;
            }
            // FIXME: Report error ?? 
            break;
        }else if(sym[i] == '['){
            sym_end = i;
            array_idx_start = i+1;
        }else if(sym[i] == ']'){
            array_idx_end = i;
        }
        i++;
    }
    *out_symlen = sym_end;
    /* Generate array index */
    if(array_idx_start > 0){
        acc = 0;
        for(i=array_idx_start;i!=array_idx_end;i++){
            r = fetchdigit(sym[i]);
            if(r >= 0){
                acc *= 10;
                acc += r;
            }
            // FIXME: Report error ??
        }
        *out_idx = acc;
    }else{
        *out_idx = -1;
    }
}

int /* bool */
cwgl_priv_match_symobj(cwgl_ctx_t* ctx, cwgl_string_t* s, cwgl_string_t* t,
                       int* array_idx_l, int* array_idx_r){
    int idx_l;
    int idx_r;
    int symlen_l;
    int symlen_r;
    parsesym(s->str, s->size, &symlen_l, &idx_l);
    parsesym(t->str, t->size, &symlen_r, &idx_r);
    if(array_idx_l){
        *array_idx_l = idx_l;
    }
    if(array_idx_r){
        *array_idx_r = idx_r;
    }

    if(symlen_l != symlen_r){
        /* Early cut */
        return 0;
    }
    if(!strncmp(s->str, t->str, symlen_l)){
        return 1;
    }else{
        return 0;
    }
}

int /* bool */
cwgl_priv_match_sym(cwgl_ctx_t* ctx, cwgl_string_t* s, const char* name,
                    int* array_idx_l, int* array_idx_r){
    int idx_l;
    int idx_r;
    int symlen_l;
    int symlen_r;
    parsesym(s->str, s->size, &symlen_l, &idx_l);
    parsesym(name, -1, &symlen_r, &idx_r);
    if(array_idx_l){
        *array_idx_l = idx_l;
    }
    if(array_idx_r){
        *array_idx_r = idx_r;
    }

    if(symlen_l != symlen_r){
        /* Early cut */
        return 0;
    }
    if(!strncmp(s->str, name, symlen_l)){
        return 1;
    }else{
        return 0;
    }
}

cwgl_string_t*
cwgl_priv_string_dup(cwgl_ctx_t* ctx, cwgl_string_t* src){
    return cwgl_priv_alloc_string(ctx, src->str, src->size);
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
            if(buflen > siz){
                buf[siz] = 0;
            }
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
