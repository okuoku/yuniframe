#include "SDL2/SDL_main.h"

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

int YFRM_ENTRYPOINT(int ac, char** av);
int YFRM_FRAME(void* bogus);

#ifdef __EMSCRIPTEN__
static void
loop(void){
    int r;
    r = YFRM_FRAME(0);
    if(r){
        emscripten_cancel_main_loop();
    }
}
#endif

int
main(int ac, char** av){
    int r;
    r = YFRM_ENTRYPOINT(ac, av);
    if(r){
        return r;
    }
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loop, 0, 1);
#else
    for(;;){
        r = YFRM_FRAME(0);
        if(r){
            return r;
        }
    }
#endif
}


