#include "yfrm.h"

#include <string.h>
#include <stdio.h>
#include "SDL.h"

#ifdef YFRM_CWGL_USE_DX11
#include "SDL_syswm.h"
#endif

void* yfrm_cwgl_pfctx_create_angle(void* pfdev, void* pfwnd);
void yfrm_cwgl_pfctx_flip_angle(void* pf);

/* Globals */
static SDL_Window* wnd;
static cwgl_ctx_t* cur;

YFRM_API int
yfrm_init(void){
    wnd = NULL;
    cur = NULL;
}

YFRM_API void
yfrm_terminate(void){
    if(wnd){
        SDL_Quit();
        wnd = NULL;
    }
    cur = NULL;
}

struct cwgl_ctx_s {
    SDL_Window* wnd;
    SDL_GLContext glc;
    void* pf;
};

static cwgl_ctx_t*
ctx_create_EGL(int32_t width, int32_t height, int32_t reserved,
                     int32_t flags){
    SDL_GLContext glc;
    cwgl_ctx_t* r;

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
    r->wnd = wnd;
    r->glc = glc;

    return r;
}

#ifdef YFRM_CWGL_USE_DX11
static cwgl_ctx_t*
ctx_create_DX11(int32_t width, int32_t height, int32_t reserved,
                     int32_t flags){
    ID3D11Device* dev;
    SDL_Renderer* rnd;
    cwgl_ctx_t* r;
    SDL_SysWMinfo info;
    HWND hWnd;
    void* pf;

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
                                       width, height, 0))){
            SDL_Quit();
            printf("SDL CreateWindow failed.\n");
            return NULL;
        }
        wnd = window;
    }

    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "direct3d11");
    SDL_SetHint(SDL_HINT_RENDER_DIRECT3D11_DEBUG, "1");
    rnd = SDL_CreateRenderer(wnd, -1, SDL_RENDERER_ACCELERATED);
    dev = SDL_RenderGetD3D11Device(rnd);

    SDL_VERSION(&info.version);
    SDL_GetWindowWMInfo(wnd, &info);
    hWnd = (HWND)(info.info.win.window);

    pf = yfrm_cwgl_pfctx_create_angle(dev, hWnd);

    r = malloc(sizeof(cwgl_ctx_t));
    r->wnd = wnd;
    r->glc = NULL;
    r->pf = pf;

    return r;
}
#endif

YFRM_API cwgl_ctx_t*
yfrm_cwgl_ctx_create(int32_t width, int32_t height, int32_t reserved,
                     int32_t flags){
#ifndef YFRM_CWGL_USE_DX11
    return ctx_create_EGL(width, height, reserved, flags);
#else
    return ctx_create_DX11(width, height, reserved, flags);
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

    cur = (cwgl_ctx_t*)c;

}

YFRM_API void
yfrm_frame_end0(void* c){
    cwgl_ctx_t* ctx = (cwgl_ctx_t*)c;
    cur = NULL;
#ifndef YFRM_CWGL_USE_DX11
    SDL_GL_SwapWindow(ctx->wnd);
#else
    yfrm_cwgl_pfctx_flip_angle(ctx->pf);
#endif
}

void
cwgl_priv_check_current(cwgl_ctx_t* ctx){
    if(ctx != cur){
        printf("WARNING: Submitting cross context command !\n");
    }
}

