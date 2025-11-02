#include <stdlib.h>

#include "EGL/egl.h"
#define EGL_EGLEXT_PROTOTYPES
#include "EGL/eglext.h"

#ifdef YFRM_CWGL_USE_DX11
#include "EGL/eglext_angle.h"
#endif

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
yfrm_cwgl_pfctx_create_egl(int pftype, void* pfdev, void* pfwnd){
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
#elif defined(YFRM_CWGL_USE_WAYLAND)
    if(pftype == 0){
        egl_disp = eglGetPlatformDisplayEXT(EGL_PLATFORM_WAYLAND_EXT, 
                                            pfdev /* (struct wl_display) */,
                                            NULL);
    }else{
        /* X11: ANGLE do not support EGL_PLATFORM_X11_KHR for display
         *      specification */
        egl_disp = eglGetDisplay((EGLNativeDisplayType)EGL_DEFAULT_DISPLAY);
    }
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
#ifndef YFRM_CWGL_USE_WAYLAND
    egl_surf = eglCreateWindowSurface(egl_disp, r->egl_cfg, 
                                      (EGLNativeWindowType)pfwnd, NULL);
#else
    /* FIXME: https://registry.khronos.org/EGL/extensions/EXT/EGL_EXT_platform_wayland.txt */
    /*        says wl_egl_surface */
    if(pftype == 0){
        egl_surf = eglCreatePlatformWindowSurfaceEXT(egl_disp, r->egl_cfg, 
                                                     /* (struct wl_egl_window) */
                                                     pfwnd, NULL);
    }else{
        void* nwnd;
        nwnd = (void*)(uintptr_t)pfwnd;
        egl_surf = eglCreatePlatformWindowSurfaceEXT(egl_disp, r->egl_cfg,
                                                     /* Window */
                                                     &nwnd, NULL);
    }
#endif
    eglBindAPI(EGL_OPENGL_ES_API);
    r->egl_ctx = eglCreateContext(egl_disp, r->egl_cfg, NULL, glesattrs);
    eglMakeCurrent(egl_disp, egl_surf, egl_surf, r->egl_ctx);

    r->egl_surf = egl_surf;
    r->egl_disp = egl_disp;

    return r;
}

void (*ptr_glGenVertexArrays)(size_t n, unsigned int *arrays);
void (*ptr_glBindVertexArray)(unsigned int array);

void
cwgl_ctx_glGenVertexArray(uintptr_t* id){
    unsigned int i;
    ptr_glGenVertexArrays(1, &i);
    *id = i;
}

void
cwgl_ctx_glBindVertexArray(uintptr_t id){
    ptr_glBindVertexArray(id);
}

void
yfrm_cwgl_pfctx_loadext_egl(void){
#ifndef CWGL_EXPERIMENTAL_TRACKER
    ptr_glGenVertexArrays = (void*)eglGetProcAddress("glGenVertexArraysOES");
    ptr_glBindVertexArray = (void*)eglGetProcAddress("glBindVertexArrayOES");
#endif
}

void
yfrm_cwgl_pfctx_flip_egl(void* pf){
    pfctx_egl* pfctx = (pfctx_egl*)pf;
    eglSwapBuffers(pfctx->egl_disp, pfctx->egl_surf);
}
