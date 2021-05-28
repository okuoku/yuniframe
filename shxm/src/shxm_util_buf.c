#include "shxm_private.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct shxm_util_buf_s {
    uint32_t* buf;
    int len;
    int ptr;
};

shxm_util_buf_t* 
shxm_private_util_buf_new(int siz){
    uint32_t* buf;
    shxm_util_buf_t* r;
    buf = malloc(sizeof(uint32_t)*siz);
    if(buf){
        r = malloc(sizeof(shxm_util_buf_t));
        if(!r){
            free(buf);
            return NULL;
        }
        r->buf = buf;
        r->len = siz;
        r->ptr = 0;
        return r;
    }else{
        return NULL;
    }
}

void
shxm_private_util_buf_release(shxm_util_buf_t* buf){
    if(buf){
        free(buf->buf);
        free(buf);
    }
}

int
shxm_private_util_buf_dup(shxm_util_buf_t* buf, uint32_t** out_buf,
                          int* out_count){
    uint32_t* r;
    int len;
    len = shxm_private_util_buf_size(buf);
    r = malloc(sizeof(uint32_t)*len);
    if(r){
        *out_buf = r;
        *out_count = len;
        memcpy(r, buf->buf, sizeof(uint32_t)*len);
        return 0;
    }else{
        return 1;
    }
}

static int
checksize(shxm_util_buf_t* buf, int cnt){
    if(cnt < 0){
        printf("ERROR: Negative cnt %d\n",cnt);
        return 1;
    }
    if(buf->len < (buf->ptr + cnt)){
        printf("ERROR: Insufficient buffer size %d+%d > %d\n", cnt,
               buf->ptr, buf->len);
        return 1;
    }
    return 0;
}

int 
shxm_private_util_buf_write_raw(shxm_util_buf_t* buf, uint32_t* obj,
                                    int len){
    int i;
    int s;
    if(checksize(buf, len)){
        return 1;
    }
    s = buf->ptr;
    for(i=0;i!=len;i++){
        buf->buf[s+i] = obj[i];
    }
    buf->ptr += len;
    return 0;
}

int 
shxm_private_util_buf_write_op(shxm_util_buf_t* buf, uint32_t* obj,
                                   int len){
    int i;
    int s;
    uint32_t head;
    if(checksize(buf, len)){
        return 1;
    }
    head = obj[0] + (len << 16);
    s = buf->ptr;
    for(i=0;i!=len;i++){
        if(i==0){
            buf->buf[s+i] = head;
        }else{
            buf->buf[s+i] = obj[i];
        }
    }
    buf->ptr += len;
    return 0;
}

int 
shxm_private_util_buf_size(shxm_util_buf_t* buf){
    return buf->ptr;
}

uint32_t* 
shxm_private_util_buf_ptr(shxm_util_buf_t* buf){
    return buf->buf;
}

int 
shxm_private_util_buf_merge(shxm_util_buf_t* dest, shxm_util_buf_t* src){
    int i;
    int len;
    int ptr;
    ptr = shxm_private_util_buf_size(dest);
    len = shxm_private_util_buf_size(src);
    if(checksize(dest, len)){
        return 1;
    }
    for(i=0;i!=len;i++){
        //printf("Merge: %04d: %08x <= %04d\n",ptr+i,src->buf[i],i);
        dest->buf[ptr+i] = src->buf[i];
    }
    dest->ptr += len;
    return 0;
}

void
shxm_private_util_buf_clearnop(shxm_util_buf_t* buf){
    int i;
    int w;
    int len;
    int oplen;
    int dif;
    dif = 0;
    len = shxm_private_util_buf_size(buf);
    i = 5; /* SPIR-V has 5 words header */
    w = 5;
    while(i!=len){
        if(i>len){
            printf("WARNING: Something wrong!\n");
            break;
        }
        if(buf->buf[i] == 0x10000 /* OpNop */){
            i++;
        }else{
            oplen = (buf->buf[i]) >> 16;
            if(w != i){
                /* Copy this instruction to w */
                memmove(&buf->buf[w], &buf->buf[i], sizeof(uint32_t)*oplen);
            }
            w+=oplen;
            i+=oplen;
        }
    }
    buf->ptr = w;
}
