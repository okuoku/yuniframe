#include <stdlib.h>

#include "SDL2/SDL_syswm.h"
#include "EGL/egl.h"
#define EGL_EGLEXT_PROTOTYPES
#include "EGL/eglext.h"
#include "EGL/eglext_angle.h"

struct pfctx_egl_s {
    EGLDisplay egl_disp;
    EGLSurface egl_surf;
    EGLContext egl_ctx;
    EGLConfig egl_cfg;
};

typedef struct pfctx_egl_s pfctx_egl;

static const EGLint glesattrs[] = {
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
};

void
yfrm_cwgl_pfctx_reset_egl(void* ctx){
    pfctx_egl* pf = (pfctx_egl*)ctx;

    eglDestroyContext(pf->egl_disp, pf->egl_ctx);
    pf->egl_ctx = eglCreateContext(pf->egl_disp, pf->egl_cfg, NULL, glesattrs);
    if(pf->egl_ctx ==  EGL_NO_CONTEXT){
        abort();
    }
    eglMakeCurrent(pf->egl_disp, pf->egl_surf, pf->egl_surf, pf->egl_ctx);
}

void* /* pfctx */
yfrm_cwgl_pfctx_create_egl(void* pfdev, void* pfwnd){
    EGLDisplay egl_disp;
    EGLSurface egl_surf;
    EGLint egl_ncfg;
    pfctx_egl* r;

    /* ANGLE EGL+GLES context creation */
#ifdef YFRM_CWGL_USE_DX11
    EGLDeviceEXT egl_dev;
    egl_dev = eglCreateDeviceANGLE(EGL_D3D11_DEVICE_ANGLE, pfdev, NULL);
    egl_disp = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT,
                                        egl_dev, NULL);
#else
    (void)pfdev;
    egl_disp = eglGetDisplay((EGLNativeDisplayType)EGL_DEFAULT_DISPLAY);
#endif
    eglInitialize(egl_disp, NULL, NULL);
    const EGLint attrs[] = {
        EGL_DEPTH_SIZE, 16,
        EGL_STENCIL_SIZE, 8,
        EGL_ALPHA_SIZE, 0,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_NONE
    };
    r = malloc(sizeof(pfctx_egl));
    if(! r){
        abort();
    }
    eglChooseConfig(egl_disp, attrs, &r->egl_cfg, 1, &egl_ncfg);
    egl_surf = eglCreateWindowSurface(egl_disp, r->egl_cfg, pfwnd, NULL);
    eglBindAPI(EGL_OPENGL_ES_API);
    r->egl_ctx = eglCreateContext(egl_disp, r->egl_cfg, NULL, glesattrs);
    eglMakeCurrent(egl_disp, egl_surf, egl_surf, r->egl_ctx);

    r->egl_surf = egl_surf;
    r->egl_disp = egl_disp;

    return r;
}

void
yfrm_cwgl_pfctx_flip_egl(void* pf){
    pfctx_egl* pfctx = (pfctx_egl*)pf;
    eglSwapBuffers(pfctx->egl_disp, pfctx->egl_surf);
}
