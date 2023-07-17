#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "yuniframe/yfrm.h"

static char*
execpath(void){
    const size_t buf0_len = 32768;
    int buf1_len;
    char* buf0;
    char* buf1;
    size_t buf0_siz;
    size_t buf1_siz;

    buf0 = malloc(buf0_len);
    if(buf0 == NULL){
        goto bailout_none;
    }
    memset(buf0, 0, buf0_len);
    buf0_siz = GetModuleFileNameW(NULL, buf0, buf0_len);
    if(buf0_siz <= 0){
        goto bailout_buf0;
    }
    buf0[buf0_siz * 2] = 0;
    buf0[buf0_siz * 2 + 1] = 0;

    /* Pass 1 */
    buf1_siz = WideCharToMultiByte(CP_UTF8,
                                   0,
                                   buf0,
                                   -1 /* Null terminated */,
                                   NULL, NULL, NULL, NULL);

    if(buf1_siz > 0){
        buf1 = malloc(buf1_siz);
        if(buf1 == NULL){
            goto bailout_buf0;
        }
        /* Pass 2 */
        if(!WideCharToMultiByte(CP_UTF8,
                                0,
                                buf0,
                                -1 /* Null terminated */,
                                buf1, buf1_siz, NULL, NULL)){
            goto bailout_buf1;
        }
        free(buf0);
        return buf1;
    }else{
        goto bailout_buf0;
    }
bailout_buf1:
    free(buf1);
bailout_buf0:
    free(buf0);
bailout_none:
    return NULL;
}

YFRM_API void
yfrm_file_configure0(const char* slotname){
    const size_t maxutf8path = 64*1024 /* FIXME */;
    char* execname = execpath();
    char* buf;
    size_t execname_len = strnlen(execname, maxutf8path);
    size_t e;

    if(execname_len == maxutf8path){
        abort();
    }
    for(e = execname_len; e!= 0; e--){
        if(execname[e] == '\\'){
            execname[e] = 0;
            break;
        }
    }
    buf = malloc(maxutf8path);
    snprintf(buf, maxutf8path, "%s\\%s\\",
             execname, slotname);

    yfrm_file_set_prefix0(buf);
    free(buf);
    free(execname);
}
