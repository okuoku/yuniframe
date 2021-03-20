#include "imgui.h"
#include "yfrm.h"

int mouse_x_abs = 0;
int mouse_y_abs = 0;
int mouse_buttons = 0;


static int
wheeldelta(int32_t v){
    if(v > 0){
        return 1;
    }else if(v < 0){
        return -1;
    }else{
        return 0;
    }
}

static void
procevent_one(ImGuiIO& io, const int32_t* events, size_t loc){
    const int32_t type = events[loc];
    switch(type){
        case 3: /* mousemotion */
            mouse_x_abs = events[loc+1];
            mouse_y_abs = events[loc+2];
            mouse_buttons |= events[loc+5];
            break;
        case 2: /* mousewheel */
            io.MouseWheelH += wheeldelta(events[loc+1]);
            io.MouseWheel += wheeldelta(events[loc+2]);
            break;
        case 0: /* mousedown */
        case 1: /* mouseup */
            mouse_buttons |= events[loc+4];
            break;
        default:
            /* Do nothing */
            break;
    }
}

bool
ImGui_ImplYfrm_ProcessEvent0(const int32_t* events, size_t start, size_t end){
    ImGuiIO& io = ImGui::GetIO();
    size_t idx;
    mouse_buttons = 0;
    idx = start;
    while(idx < end){
        int len = events[idx];
        procevent_one(io, events, idx + 1);
        idx = idx + len;
    }
    io.MousePos = ImVec2(mouse_x_abs, mouse_y_abs);
    io.MouseDown[0] = (mouse_buttons & 1);
    io.MouseDown[1] = (mouse_buttons & 2);
    io.MouseDown[2] = (mouse_buttons & 4);
    return true;
}

bool
ImGui_ImplYfrm_Init(){
    ImGuiIO& io = ImGui::GetIO();

    io.BackendPlatformName = "imgui_impl_yfrm";

    // FIXME: KeyMap

    // FIXME: Mouse cursors

    return true;
}

