#include "yuniframe/yfrm.h"

#include <string.h>
#include <stdio.h>
#include "SDL2/SDL.h"

#ifdef YFRM_CWGL_USE_ANGLE
#include "SDL2/SDL_syswm.h"
#endif

#ifdef CWGL_EXPERIMENTAL_TRACKER
#include "SDL2/SDL_vulkan.h"
#endif

void* cwgl_ctx_platform_get(void* ctx);
void* cwgl_ctx_alloc(void* platform);
void cwgl_ctx_destroy(void* ctx);

int cwgl_backend_beginframe(cwgl_ctx_t* ctx); // FIXME: Define it elsewhere
int cwgl_backend_endframe(cwgl_ctx_t* ctx); // FIXME: Define it elsewhere
void* yfrm_cwgl_pfctx_create_egl(void* pfdev, void* pfwnd);
void yfrm_cwgl_pfctx_reset_egl(void* ctx);
void yfrm_cwgl_pfctx_flip_egl(void* pf);
void yfrm_cwgl_pfctx_loadext_egl(void);
void* yfrm_gpu_initpfdev_d3d11(void);

/* Globals */
static SDL_Window* wnd;
struct cwgl_platform_ctx_s;
typedef struct platform_ctx_s platform_ctx;
static cwgl_ctx_t* cur;

YFRM_API int
yfrm_init(void){
#if !defined(CWGL_EXPERIMENTAL_TRACKER) && !defined(__APPLE__)
    yfrm_cwgl_pfctx_loadext_egl();
#endif
    wnd = NULL;
    cur = NULL;
    return 0;
}

YFRM_API void
yfrm_terminate(void){
    if(wnd){
        SDL_Quit();
        wnd = NULL;
    }
    cur = NULL;
}

struct platform_ctx_s {
    SDL_Window* wnd;
    SDL_GLContext glc;
    void* pf;
};

#ifdef CWGL_EXPERIMENTAL_TRACKER
int
cwgl_integ_vkpriv_getextensions(cwgl_ctx_t* ctx,
                                int* out_count,
                                const char** out_extensions){
    platform_ctx* p;
    p = (platform_ctx*)cwgl_ctx_platform_get(ctx);
    if(SDL_Vulkan_GetInstanceExtensions(p->wnd, out_count, out_extensions)){
        return 0;
    }
    return -1;
}

int
cwgl_integ_vkpriv_createsurface(cwgl_ctx_t* ctx,
                                VkInstance instance, VkSurfaceKHR* surface){
    platform_ctx* p;
    p = (platform_ctx*)cwgl_ctx_platform_get(ctx);
    if(SDL_Vulkan_CreateSurface(p->wnd, instance, surface)){
        return 0;
    }
    return -1;
}

static cwgl_ctx_t*
ctx_create_VK(int32_t width, int32_t height, int32_t reserved,
                     int32_t flags){
    uint32_t extraflags = 0;
    SDL_GLContext glc;
    cwgl_ctx_t* r;
    platform_ctx* p;

    if(! wnd){
        SDL_Window* window;
        /* Init SDL and Create a window */
        if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER|SDL_INIT_AUDIO|SDL_INIT_TIMER)){
            printf("SDL Init failed.\n");
            return NULL;
        }

        if(flags & YFRM_SCREEN_RESIZABLE){
            extraflags |= SDL_WINDOW_RESIZABLE;
        }

        if(!(window = SDL_CreateWindow("cwgl",
                                       SDL_WINDOWPOS_UNDEFINED,
                                       SDL_WINDOWPOS_UNDEFINED,
                                       width, height,
                                       extraflags|SDL_WINDOW_VULKAN))){
            SDL_Quit();
            printf("SDL CreateWindow failed.\n");
            return NULL;
        }
        wnd = window;
    }

    p = malloc(sizeof(platform_ctx));
    r = cwgl_ctx_alloc(p);
    p->wnd = wnd;
    return r;
}
#endif

static cwgl_ctx_t*
ctx_create_EGL(int32_t width, int32_t height, int32_t reserved,
                     int32_t flags){
    uint32_t extraflags = 0;
    SDL_GLContext glc;
    cwgl_ctx_t* r;
    platform_ctx* p;

    if(! wnd){
        SDL_Window* window;
        /* Init SDL and Create a window */
        if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER|SDL_INIT_AUDIO|SDL_INIT_TIMER)){
            printf("SDL Init failed.\n");
            return NULL;
        }

        if(flags & YFRM_SCREEN_RESIZABLE){
            extraflags |= SDL_WINDOW_RESIZABLE;
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
                                       extraflags|SDL_WINDOW_OPENGL))){
            SDL_Quit();
            printf("SDL CreateWindow failed.\n");
            return NULL;
        }
        wnd = window;
    }

    glc = SDL_GL_CreateContext(wnd);
    SDL_GL_MakeCurrent(wnd, glc);

    SDL_GL_SetSwapInterval(0);

    p = malloc(sizeof(platform_ctx));
    r = (cwgl_ctx_t*)cwgl_ctx_alloc(p);
    p->wnd = wnd;
    p->glc = glc;
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
    platform_ctx* p;
#if defined(SDL_VIDEO_DRIVER_COCOA) || defined(SDL_VIDEO_DRIVER_UIKIT)
    wndflags |= SDL_WINDOW_METAL|SDL_WINDOW_ALLOW_HIGHDPI;
#endif

    if(flags & YFRM_SCREEN_RESIZABLE){
        wndflags |= SDL_WINDOW_RESIZABLE;
    }

    if(! wnd){
        SDL_Window* window;
#ifdef YFRM_CWGL_USE_WAYLAND
        SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland");
        wndflags |= SDL_WINDOW_OPENGL /* for egl surface */;
#endif
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

    SDL_VERSION(&info.version);
    SDL_GetWindowWMInfo(wnd, &info);

#ifdef YFRM_CWGL_USE_DX11
    dev = yfrm_gpu_initpfdev_d3d11();
#elif defined(YFRM_CWGL_USE_WAYLAND)
    dev = info.info.wl.display;
#else
    dev = NULL;
#endif

#ifdef YFRM_USE_UWP
    pfwnd = info.info.winrt.window;
#elif defined(_WIN32)
    pfwnd = (HWND)(info.info.win.window);
#elif defined(SDL_VIDEO_DRIVER_COCOA) || defined(SDL_VIDEO_DRIVER_UIKIT)
    // pfwnd is CALayer 
    pfwnd = SDL_Metal_GetLayer(SDL_Metal_CreateView(wnd));
#elif defined(YFRM_CWGL_USE_WAYLAND)
    pfwnd = info.info.wl.egl_window;
#else
    pfwnd = NULL; //FIXME
#endif

    pf = yfrm_cwgl_pfctx_create_egl(dev, pfwnd);

    p = malloc(sizeof(platform_ctx));
    r = (cwgl_ctx_t*)cwgl_ctx_alloc(p);
    p->wnd = wnd;
    p->glc = NULL;
    p->pf = pf;
    return r;
}

static cwgl_ctx_t*
ctx_reset_ANGLE(cwgl_ctx_t* ctx){
#ifdef CWGL_EXPERIMENTAL_TRACKER
    abort(); // Need to reset tracker
#else
    platform_ctx* p;
    p = (platform_ctx*)cwgl_ctx_platform_get(ctx);
    printf("=== reset ctx ===\n");
    yfrm_cwgl_pfctx_reset_egl(p->pf);
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
#ifdef CWGL_EXPERIMENTAL_TRACKER
    free(cwgl_ctx_platform_get(ctx));
#endif
    cwgl_ctx_destroy(ctx);
}

YFRM_API void
yfrm_frame_begin0(void* c){

    if(cur){
        printf("WARNING: Overriding frame !\n");
    }
    cur = c;

#if defined(CWGL_EXPERIMENTAL_TRACKER) && defined(YFRM_CWGL_USE_VULKAN)
    cwgl_backend_beginframe((cwgl_ctx_t*)c);
#endif

}

YFRM_API void
yfrm_frame_end0(void* c){
    platform_ctx* p;
    cur = NULL;
#if defined(CWGL_EXPERIMENTAL_TRACKER) && defined(YFRM_CWGL_USE_VULKAN)
    cwgl_backend_endframe((cwgl_ctx_t*)c);
#elif defined(YFRM_CWGL_USE_ANGLE)
    p = (platform_ctx*)cwgl_ctx_platform_get(c);
    yfrm_cwgl_pfctx_flip_egl(p->pf);
#else
    p = (platform_ctx*)cwgl_ctx_platform_get(c);
    SDL_GL_SwapWindow(p->wnd);
#endif
}

void
cwgl_priv_check_current(cwgl_ctx_t* ctx){
    if(ctx != cur){
        printf("WARNING: Submitting cross context command !\n");
    }
}
