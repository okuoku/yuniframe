#ifdef CWGL_EXPERIMENTAL_TRACKER
#error Not implemented.
#endif

#include "yuniframe/yfrm.h"

#include "EGL/egl.h"
 
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int cwgl_backend_beginframe(cwgl_ctx_t* ctx); // FIXME: Define it elsewhere
int cwgl_backend_endframe(cwgl_ctx_t* ctx); // FIXME: Define it elsewhere
void* yfrm_cwgl_pfctx_create_egl(int pftype, void* pfdev, void* pfwnd);
void yfrm_cwgl_pfctx_reset_egl(void* ctx);
void yfrm_cwgl_pfctx_flip_egl(void* pf);
void yfrm_cwgl_pfctx_loadext_egl(void);
void* yfrm_gpu_initpfdev_d3d11(void);

/* Globals */

static void* cur;

YFRM_API int
yfrm_init(void){
    yfrm_cwgl_pfctx_loadext_egl();
    cur = 0;
    return 0;
}

YFRM_API void
yfrm_terminate(void){
    /* Do nothing */
}

YFRM_API cwgl_ctx_t*
yfrm_cwgl_ctx_create(int32_t width, int32_t height, int32_t reserved,
                     int32_t flags){
    void* pf;
    pf = yfrm_cwgl_pfctx_create_egl(0, 0, 0);
    return (cwgl_ctx_t*)pf;
}

YFRM_API cwgl_ctx_t*
yfrm_cwgl_ctx_reset0(cwgl_ctx_t* ctx){
    yfrm_cwgl_pfctx_reset_egl((void*)ctx);
    return ctx;
}

YFRM_API void
yfrm_cwgl_ctx_release(cwgl_ctx_t* ctx){
    free(ctx);
}

YFRM_API void
yfrm_frame_begin0(void* c){
    if(cur){
        printf("WARNING: Overriding frame !\n");
    }
    cur = c;
}

YFRM_API void
yfrm_frame_end0(void* c){
    yfrm_cwgl_pfctx_flip_egl(cur);
    cur = 0;
}

void
cwgl_priv_check_current(cwgl_ctx_t* ctx){
    if(ctx != cur){
        printf("WARNING: Submitting cross context command!\n");
    }
}
