#ifdef CWGL_EXPERIMENTAL_TRACKER
#include "yuniframe/cwgl.h"
#include "cwgl-tracker-priv.h"
#endif
#include "yuniframe/yfrm.h"

#include <string.h>
#include <stdio.h>
#include "SDL2/SDL.h"

#ifdef YFRM_CWGL_USE_ANGLE
#include "SDL_syswm.h"
#endif

#ifdef CWGL_EXPERIMENTAL_TRACKER
#include "SDL_vulkan.h"
#endif

int cwgl_backend_beginframe(cwgl_ctx_t* ctx); // FIXME: Define it elsewhere
int cwgl_backend_endframe(cwgl_ctx_t* ctx); // FIXME: Define it elsewhere
void* yfrm_cwgl_pfctx_create_angle(void* pfdev, void* pfwnd);
void yfrm_cwgl_pfctx_reset_angle(void* ctx);
void yfrm_cwgl_pfctx_flip_angle(void* pf);
void* yfrm_gpu_initpfdev_d3d11(void);

/* Globals */
static SDL_Window* wnd;
#ifdef CWGL_EXPERIMENTAL_TRACKER
struct cwgl_platform_ctx_s;
typedef struct cwgl_platform_ctx_s cwgl_platform_ctx_t;
static cwgl_platform_ctx_t* cur;
#else
static cwgl_ctx_t* cur;
#endif

#ifdef _WIN32
#define STDCALL __stdcall
#else
#define STDCALL
#endif
void* STDCALL eglGetProcAddress(char const * procname);
void (*ptr_glGenVertexArrays)(size_t n, unsigned int *arrays);
void (*ptr_glBindVertexArray)(unsigned int array);

YFRM_API int
yfrm_init(void){
#ifndef CWGL_EXPERIMENTAL_TRACKER
#ifdef __APPLE__
extern void glGenVertexArraysOES(size_t n, unsigned int *arrays);
extern void glBindVertexArrayOES(unsigned int array);
    ptr_glGenVertexArrays = glGenVertexArraysOES;
    ptr_glBindVertexArray = glBindVertexArrayOES;
#else
    ptr_glGenVertexArrays = eglGetProcAddress("glGenVertexArraysOES");
    ptr_glBindVertexArray = eglGetProcAddress("glBindVertexArrayOES");
#endif
#endif
    wnd = NULL;
    cur = NULL;
    return 0;
}

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


YFRM_API void
yfrm_terminate(void){
    if(wnd){
        SDL_Quit();
        wnd = NULL;
    }
    cur = NULL;
}

#ifdef CWGL_EXPERIMENTAL_TRACKER
struct cwgl_platform_ctx_s {
    SDL_Window* wnd;
    SDL_GLContext glc;
    void* pf;
};
#else
struct cwgl_ctx_s {
    SDL_Window* wnd;
    SDL_GLContext glc;
    void* pf;
};
#endif

#ifdef CWGL_EXPERIMENTAL_TRACKER
int
cwgl_integ_vkpriv_getextensions(cwgl_ctx_t* ctx,
                                int* out_count,
                                const char** out_extensions){
    cwgl_platform_ctx_t* p;
    p = (cwgl_platform_ctx_t*)ctx->platform;
    if(SDL_Vulkan_GetInstanceExtensions(p->wnd, out_count, out_extensions)){
        return 0;
    }
    return -1;
}

int
cwgl_integ_vkpriv_createsurface(cwgl_ctx_t* ctx,
                                VkInstance instance, VkSurfaceKHR* surface){
    cwgl_platform_ctx_t* p;
    p = (cwgl_platform_ctx_t*)ctx->platform;
    if(SDL_Vulkan_CreateSurface(p->wnd, instance, surface)){
        return 0;
    }
    return -1;
}

static cwgl_ctx_t*
ctx_create_VK(int32_t width, int32_t height, int32_t reserved,
                     int32_t flags){
    SDL_GLContext glc;
    cwgl_ctx_t* r;
    cwgl_platform_ctx_t* p;

    if(! wnd){
        SDL_Window* window;
        /* Init SDL and Create a window */
        if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER|SDL_INIT_AUDIO|SDL_INIT_TIMER)){
            printf("SDL Init failed.\n");
            return NULL;
        }

        if(!(window = SDL_CreateWindow("cwgl",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       width, height,
                                       SDL_WINDOW_VULKAN))){
            SDL_Quit();
            printf("SDL CreateWindow failed.\n");
            return NULL;
        }
        wnd = window;
    }

    r = malloc(sizeof(cwgl_ctx_t));
    p = malloc(sizeof(cwgl_platform_ctx_t));
    p->wnd = wnd;
    //p->glc = glc;
    r->platform = p;

    // FIXME: Move this to tracker
    cwgl_integ_ctx_init(r);
    return r;
}
#endif

static cwgl_ctx_t*
ctx_create_EGL(int32_t width, int32_t height, int32_t reserved,
                     int32_t flags){
    SDL_GLContext glc;
    cwgl_ctx_t* r;
#ifdef CWGL_EXPERIMENTAL_TRACKER
    cwgl_platform_ctx_t* p;
#endif

    if(! wnd){
        SDL_Window* window;
        /* Init SDL and Create a window */
        if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER|SDL_INIT_AUDIO|SDL_INIT_TIMER)){
            printf("SDL Init failed.\n");
            return NULL;
        }

        SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 
                            SDL_GL_CONTEXT_PROFILE_ES);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

        if(!(window = SDL_CreateWindow("cwgl",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       width, height,
                                       SDL_WINDOW_OPENGL))){
            SDL_Quit();
            printf("SDL CreateWindow failed.\n");
            return NULL;
        }
        wnd = window;
    }

    glc = SDL_GL_CreateContext(wnd);
    SDL_GL_MakeCurrent(wnd, glc);

    SDL_GL_SetSwapInterval(0);

    r = malloc(sizeof(cwgl_ctx_t));
#ifdef CWGL_EXPERIMENTAL_TRACKER
    p = malloc(sizeof(cwgl_platform_ctx_t));
    p->wnd = wnd;
    p->glc = glc;
    r->platform = p;
    // FIXME: Move this to tracker
    cwgl_integ_ctx_init(r);
#else
    r->wnd = wnd;
    r->glc = glc;
#endif

    return r;
}

#ifdef YFRM_CWGL_USE_ANGLE
static cwgl_ctx_t*
ctx_create_ANGLE(int32_t width, int32_t height, int32_t reserved,
                 int32_t flags){
    void* dev;
    cwgl_ctx_t* r;
    SDL_SysWMinfo info;
    void* pfwnd;
    void* pf;
    uint32_t wndflags = 0;
#ifdef CWGL_EXPERIMENTAL_TRACKER
    cwgl_platform_ctx_t* p;
#endif
#if defined(SDL_VIDEO_DRIVER_COCOA) || defined(SDL_VIDEO_DRIVER_UIKIT)
    wndflags |= SDL_WINDOW_METAL|SDL_WINDOW_ALLOW_HIGHDPI;
#endif

    if(! wnd){
        SDL_Window* window;
        /* Init SDL and Create a window */
        if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER|SDL_INIT_TIMER)){
            printf("SDL Init failed.\n");
            return NULL;
        }
        if(SDL_InitSubSystem(SDL_INIT_AUDIO)){
            printf("WARNING: SDL Audio Init failed.\n");
        }

        if(!(window = SDL_CreateWindow("cwgl",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       width, height, wndflags))){
            SDL_Quit();
            printf("SDL CreateWindow failed.\n");
            return NULL;
        }
        wnd = window;
    }

#ifdef YFRM_CWGL_USE_DX11
    dev = yfrm_gpu_initpfdev_d3d11();
#else
    dev = NULL;
#endif

    SDL_VERSION(&info.version);
    SDL_GetWindowWMInfo(wnd, &info);
#ifdef YFRM_USE_UWP
    pfwnd = info.info.winrt.window;
#elif defined(_WIN32)
    pfwnd = (HWND)(info.info.win.window);
#elif defined(SDL_VIDEO_DRIVER_COCOA) || defined(SDL_VIDEO_DRIVER_UIKIT)
    // pfwnd is CALayer 
    pfwnd = SDL_Metal_GetLayer(SDL_Metal_CreateView(wnd));
#else
    pfwnd = NULL; //FIXME
#endif

    pf = yfrm_cwgl_pfctx_create_angle(dev, pfwnd);

    r = malloc(sizeof(cwgl_ctx_t));
#ifdef CWGL_EXPERIMENTAL_TRACKER
    cwgl_integ_ctx_init(r);
    p = malloc(sizeof(cwgl_platform_ctx_t));
    p->wnd = wnd;
    p->glc = NULL;
    p->pf = pf;
    r->platform = p;
#else
    r->wnd = wnd;
    r->glc = NULL;
    r->pf = pf;
#endif

    return r;
}

static cwgl_ctx_t*
ctx_reset_ANGLE(cwgl_ctx_t* ctx){
#ifdef CWGL_EXPERIMENTAL_TRACKER
    abort(); // Need to reset tracker
#else
    printf("=== reset ctx ===\n");
    yfrm_cwgl_pfctx_reset_angle(ctx->pf);
#endif
    return ctx;
}

#endif

YFRM_API cwgl_ctx_t*
yfrm_cwgl_ctx_create(int32_t width, int32_t height, int32_t reserved,
                     int32_t flags){
#if defined(CWGL_EXPERIMENTAL_TRACKER) && defined(YFRM_CWGL_USE_VULKAN)
    return ctx_create_VK(width, height, reserved, flags);
#elif !defined(YFRM_CWGL_USE_ANGLE)
    return ctx_create_EGL(width, height, reserved, flags);
#else
    return ctx_create_ANGLE(width, height, reserved, flags);
#endif
}

YFRM_API cwgl_ctx_t*
yfrm_cwgl_ctx_reset0(cwgl_ctx_t* ctx){
#if defined(YFRM_CWGL_USE_ANGLE)
    return ctx_reset_ANGLE(ctx);
#else
    abort();
#endif
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

#ifdef CWGL_EXPERIMENTAL_TRACKER
    cur = (cwgl_platform_ctx_t*)((cwgl_ctx_t*)c)->platform;
#else
    cur = (cwgl_ctx_t*)c;
#endif
#if defined(CWGL_EXPERIMENTAL_TRACKER) && defined(YFRM_CWGL_USE_VULKAN)
    cwgl_backend_beginframe((cwgl_ctx_t*)c);
#endif

}

YFRM_API void
yfrm_frame_end0(void* c){
#ifdef CWGL_EXPERIMENTAL_TRACKER
    cwgl_platform_ctx_t* ctx = (cwgl_platform_ctx_t*)((cwgl_ctx_t*)c)->platform;
#else
    cwgl_ctx_t* ctx = (cwgl_ctx_t*)c;
#endif
    cur = NULL;
#if defined(CWGL_EXPERIMENTAL_TRACKER) && defined(YFRM_CWGL_USE_VULKAN)
    cwgl_backend_endframe((cwgl_ctx_t*)c);
#elif defined(YFRM_CWGL_USE_ANGLE)
    yfrm_cwgl_pfctx_flip_angle(ctx->pf);
#else
    SDL_GL_SwapWindow(ctx->wnd);
#endif
}

void
cwgl_priv_check_current(cwgl_ctx_t* ctx){
#ifdef CWGL_EXPERIMENTAL_TRACKER
    if(ctx->platform != (void*)cur){
        printf("WARNING: Submitting cross context command !\n");
    }
#else
    if(ctx != cur){
        printf("WARNING: Submitting cross context command !\n");
    }
#endif
}

