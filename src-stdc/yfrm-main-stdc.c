#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#endif
    
int YFRM_ENTRYPOINT(int ac, const char** av);
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
main(int ac, const char** av){
    int r;
    r = YFRM_ENTRYPOINT(ac, av);
    if(r){
        return r;
    }
#ifdef __EMSCRIPTEN__
    emscripten_set_canvas_element_size("#canvas", 1280, 720);
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

