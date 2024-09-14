#include <SDL2/SDL.h>
#include "yuniframe/yfrm.h"

YFRM_API void 
yfrm_sync_tagtype(int tagtype){
    (void)tagtype;
    return;
}

/* Objects */
YFRM_API yfrm_mtx*
yfrm_mtx_create(const void* param, const void* tag){
    return (yfrm_mtx*)SDL_CreateMutex();
}

YFRM_API void
yfrm_mtx_release(yfrm_mtx* mtx){
    SDL_DestroyMutex((SDL_mutex*)mtx);
}

YFRM_API yfrm_cond*
yfrm_cond_create(const void* param, const void* tag){
    return (yfrm_cond*)SDL_CreateCond();
}

YFRM_API void
yfrm_cond_release(yfrm_cond* cnd){
    SDL_DestroyCond((SDL_cond*)cnd);
}


/* OPs */
YFRM_API void
yfrm_mtx_lock(yfrm_mtx* mtx){
    SDL_LockMutex((SDL_mutex*)mtx);
}

YFRM_API void
yfrm_mtx_unlock(yfrm_mtx* mtx){
    SDL_UnlockMutex((SDL_mutex*)mtx);
}

YFRM_API void
yfrm_cond_wait(yfrm_cond* cnd, yfrm_mtx* mtx){
    SDL_CondWait((SDL_cond*)cnd, (SDL_mutex*)mtx);
}

YFRM_API void
yfrm_cond_broadcast(yfrm_cond* cnd){
    SDL_CondBroadcast((SDL_cond*)cnd);
}

YFRM_API void
yfrm_cond_signal(yfrm_cond* cnd){
    SDL_CondSignal((SDL_cond*)cnd);
}
