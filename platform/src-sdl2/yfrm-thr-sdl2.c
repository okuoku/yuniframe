#include <SDL2/SDL.h>
#include <stdlib.h> /* malloc, free */
#include "yuniframe/yfrm.h"

struct sdl2_thread_param {
    const void* in;
    void* out;
    yfrm_cb_thr_entry entry;
};

static SDLCALL int
thr_run(void* data){
    struct sdl2_thread_param myparam;
    myparam = *(struct sdl2_thread_param*)data;
    free(data);
    myparam.entry(myparam.in, myparam.out);
    return 0;
}

YFRM_API yfrm_thr*
yfrm_thr_create(const void* param,yfrm_cb_thr_entry entry, const char* name,
                const void* in, void* out){
    SDL_Thread* thr;
    struct sdl2_thread_param* p;
    p = (struct sdl2_thread_param*)malloc(sizeof(struct sdl2_thread_param));
    if(! p){
        return 0;
    }

    p->in = in;
    p->out = out;
    p->entry = entry;

    thr = SDL_CreateThread(thr_run, name, p);
    if(! thr){
        free(p);
        return 0;
    }

    return (yfrm_thr*)thr;
}

YFRM_API void
yfrm_thr_join(yfrm_thr* thr){
    SDL_WaitThread((SDL_Thread*)thr, 0);
}

YFRM_API void
yfrm_thr_detach(yfrm_thr* thr){
    SDL_DetachThread((SDL_Thread*)thr);
}

