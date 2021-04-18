#define CWGL_DECL_ENUMS
#include "cwgl.h"
#include "yfrm.h"

#include "imgui.h"

bool ImGui_ImplCwgl_Init(cwgl_ctx_t* ctx);
void ImGui_ImplCwgl_RenderDrawData(ImDrawData* draw_data);

bool ImGui_ImplYfrm_ProcessEvent0(const int32_t* events, size_t start, size_t end);
bool ImGui_ImplYfrm_Init();


extern "C" int
YFRM_ENTRYPOINT(int ac, char** av){
    int w,h;
    int buf[128];
    int init = 0;
    int events;
    cwgl_ctx_t* ctx;
    yfrm_init();

    w = 1280;
    h = 720;
    ctx = yfrm_cwgl_ctx_create(w,h,0,0);
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 

    ImGui::StyleColorsDark();

    /* Loop */
    bool showdemo = true;
    int frame;
    frame = 0;
    for(;;){
        yfrm_frame_begin0(ctx);
        if(!init){
            ImGui_ImplYfrm_Init();
            ImGui_ImplCwgl_Init(ctx);
            init = 1;
        }else{
            /* Clear */
            cwgl_viewport(ctx, 0, 0, w, h);
            cwgl_disable(ctx, SCISSOR_TEST);
            cwgl_clearColor(ctx, 0, 0, 0, 1.0f);
            cwgl_clear(ctx, COLOR_BUFFER_BIT);

            /* Draw something */
            // FIXME: Should go to backend
            io.DisplaySize = ImVec2((float)w, (float)h);
            io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

            io.DeltaTime = 1.0f/60.0f; /* 60Hz */

            ImGui::NewFrame();
            ImGui::ShowDemoWindow(&showdemo);
            ImGui::Render();
            ImGui_ImplCwgl_RenderDrawData(ImGui::GetDrawData());

            for(;;){
                events = yfrm_query0(0, buf, 128);
                if(events > 0){
                    ImGui_ImplYfrm_ProcessEvent0(buf, 0, events);
                }else{
                    break;
                }
            }
        }
        yfrm_frame_end0(ctx);
        frame ++;
    }

    return 0;
}
