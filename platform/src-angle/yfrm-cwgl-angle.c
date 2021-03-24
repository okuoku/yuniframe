#include <stdlib.h>

#include "SDL_syswm.h"
#include "EGL/egl.h"
#define EGL_EGLEXT_PROTOTYPES
#include "EGL/eglext.h"
#include "EGL/eglext_angle.h"

typedef struct {
    EGLDisplay egl_disp;
    EGLSurface egl_surf;
} pfctx_angle_t;

void* /* pfctx */
yfrm_cwgl_pfctx_create_angle(void* pfdev, void* pfwnd){
    EGLDeviceEXT egl_dev;
    EGLDisplay egl_disp;
    EGLSurface egl_surf;
    EGLConfig egl_cfg;
    EGLint egl_ncfg;
    EGLContext egl_ctx;
    SDL_SysWMinfo info;
    HWND hWnd;
    pfctx_angle_t* r;

    /* ANGLE EGL+GLES context creation */
    egl_dev = eglCreateDeviceANGLE(EGL_D3D11_DEVICE_ANGLE, pfdev, NULL);
    egl_disp = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT,
                                        egl_dev, NULL);
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
    eglChooseConfig(egl_disp, attrs, &egl_cfg, 1, &egl_ncfg);
    egl_surf = eglCreateWindowSurface(egl_disp, egl_cfg, pfwnd, NULL);
    const EGLint glesattrs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    eglBindAPI(EGL_OPENGL_ES_API);
    egl_ctx = eglCreateContext(egl_disp, egl_cfg, NULL, glesattrs);
    eglMakeCurrent(egl_disp, egl_surf, egl_surf, egl_ctx);

    r = malloc(sizeof(pfctx_angle_t));
    r->egl_surf = egl_surf;
    r->egl_disp = egl_disp;

    return r;
}

void
yfrm_cwgl_pfctx_flip_angle(void* pf){
    pfctx_angle_t* pfctx = (pfctx_angle_t*)pf;
    eglSwapBuffers(pfctx->egl_disp, pfctx->egl_surf);
}
