// Based on imgui_impl_opengl3.cpp

#include "imgui.h"
#include "cwgl.h"

/* Global Objects */
cwgl_ctx_t* g_ctx = 0;
cwgl_Texture_t* g_FontTexture;
cwgl_Program_t* g_ShaderHandle;
cwgl_Shader_t* g_VertHandle;
cwgl_Shader_t* g_FragHandle;
cwgl_UniformLocation_t* g_AttribLocationTex;
cwgl_UniformLocation_t* g_AttribLocationProjMtx;
int32_t g_AttribLocationVtxPos;
int32_t g_AttribLocationVtxUV;
int32_t g_AttribLocationVtxColor;
cwgl_Buffer_t* g_VboHandle;
cwgl_Buffer_t* g_ElementsHandle;

/* Shaders */
static const char shader_vert[] =
"#version 100\n"
"uniform mat4 ProjMtx;\n"
"attribute vec2 Position;\n"
"attribute vec2 UV;\n"
"attribute vec4 Color;\n"
"varying vec2 Frag_UV;\n"
"varying vec4 Frag_Color;\n"
"void main()\n"
"{\n"
"    Frag_UV = UV;\n"
"    Frag_Color = Color;\n"
"    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
"}\n";

static const char shader_frag[] =
"#version 100\n"
"precision mediump float;\n"
"uniform sampler2D Texture;\n"
"varying vec2 Frag_UV;\n"
"varying vec4 Frag_Color;\n"
"void main()\n"
"{\n"
"    gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);\n"
"}\n";

bool
ImGui_ImplCwgl_Init(cwgl_ctx_t* ctx){
    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererName = "cwgl";
    g_ctx = ctx;

    // FIXME: Backup states
    // Shaders
    g_FragHandle = cwgl_createShader(g_ctx, (cwgl_enum_t)0x8B30);
    g_VertHandle = cwgl_createShader(g_ctx, (cwgl_enum_t)0x8B31);
    cwgl_shaderSource(g_ctx, g_FragHandle, shader_frag,
                      sizeof(shader_frag));
    cwgl_compileShader(g_ctx,g_FragHandle);
    cwgl_shaderSource(g_ctx, g_VertHandle, shader_vert,
                      sizeof(shader_vert));
    cwgl_compileShader(g_ctx, g_VertHandle);
    g_ShaderHandle = cwgl_createProgram(g_ctx);
    cwgl_attachShader(g_ctx, g_ShaderHandle, g_FragHandle);
    cwgl_attachShader(g_ctx, g_ShaderHandle, g_VertHandle);
    cwgl_linkProgram(g_ctx, g_ShaderHandle);

    g_AttribLocationTex = cwgl_getUniformLocation(g_ctx, g_ShaderHandle,
                                                  "Texture");
    g_AttribLocationProjMtx = cwgl_getUniformLocation(g_ctx, g_ShaderHandle,
                                                      "ProjMtx");
    g_AttribLocationVtxPos = cwgl_getAttribLocation(g_ctx, g_ShaderHandle,
                                                    "Position");
    g_AttribLocationVtxUV = cwgl_getAttribLocation(g_ctx, g_ShaderHandle,
                                                   "UV");
    g_AttribLocationVtxColor = cwgl_getAttribLocation(g_ctx, g_ShaderHandle,
                                                      "Color");

    // Buffers
    g_VboHandle = cwgl_createBuffer(g_ctx);
    g_ElementsHandle = cwgl_createBuffer(g_ctx);

    // FontTextures
    int width, height;
    unsigned char* pixels;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    g_FontTexture = cwgl_createTexture(g_ctx);
    cwgl_bindTexture(g_ctx, (cwgl_enum_t)0x0DE1, g_FontTexture);
    cwgl_texParameteri(g_ctx, (cwgl_enum_t)0x0DE1, (cwgl_enum_t)0x2801, 0x2601);
    cwgl_texParameteri(g_ctx, (cwgl_enum_t)0x0DE1, (cwgl_enum_t)0x2800, 0x2601);
    cwgl_texImage2D(g_ctx, (cwgl_enum_t)0x0DE1, 0, (cwgl_enum_t)0x1908, width, height, 0, (cwgl_enum_t)0x1908, (cwgl_enum_t)0x1401,
                    pixels, width * height * 4);
    io.Fonts->SetTexID((ImTextureID)(intptr_t)g_FontTexture);

    // FIXME: Restore states
    return true;
}

void
ImGui_ImplCwgl_Shutdown(){
    // TODO
}

void
ImGui_ImplCwgl_NewFrame(){
    // Nothing to do
}

static void
SetupRenderState(ImDrawData* draw_data, int fb_width, int fb_height){
    cwgl_enable(g_ctx, (cwgl_enum_t)0x0BE2);
    cwgl_blendEquation(g_ctx, (cwgl_enum_t)0x8006);
    cwgl_blendFuncSeparate(g_ctx, (cwgl_enum_t)0x0302, (cwgl_enum_t)0x0303, (cwgl_enum_t)1,
        (cwgl_enum_t)0x0303);
    cwgl_disable(g_ctx, (cwgl_enum_t)0x0B44);
    cwgl_disable(g_ctx, (cwgl_enum_t)0x0B71);
    cwgl_disable(g_ctx, (cwgl_enum_t)0x0B90);
    cwgl_enable(g_ctx, (cwgl_enum_t)0x0C11);

    cwgl_viewport(g_ctx, 0, 0, fb_width, fb_height);

    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;

    /* const */ float ortho_projection[4][4] = {
        { 2.0f/(R-L),   0.0f,         0.0f,   0.0f },
        { 0.0f,         2.0f/(T-B),   0.0f,   0.0f },
        { 0.0f,         0.0f,        -1.0f,   0.0f },
        { (R+L)/(L-R),  (T+B)/(B-T),  0.0f,   1.0f },
    };

    cwgl_useProgram(g_ctx, g_ShaderHandle);
    cwgl_uniform1i(g_ctx, g_AttribLocationTex, 0);
    cwgl_uniformMatrix4fv(g_ctx, g_AttribLocationProjMtx, 0, &ortho_projection[0][0], 1);

    // FIXME: No VAO
    
    cwgl_bindBuffer(g_ctx, (cwgl_enum_t)0x8892, g_VboHandle);
    cwgl_bindBuffer(g_ctx, (cwgl_enum_t)0x8893, g_ElementsHandle);
    cwgl_enableVertexAttribArray(g_ctx, g_AttribLocationVtxPos);
    cwgl_enableVertexAttribArray(g_ctx, g_AttribLocationVtxUV);
    cwgl_enableVertexAttribArray(g_ctx, g_AttribLocationVtxColor);
    cwgl_vertexAttribPointer(g_ctx, g_AttribLocationVtxPos,
                             2, (cwgl_enum_t)0x1406, 0, sizeof(ImDrawVert),
                             IM_OFFSETOF(ImDrawVert, pos));
    cwgl_vertexAttribPointer(g_ctx, g_AttribLocationVtxUV,
                             2, (cwgl_enum_t)0x1406, 0, sizeof(ImDrawVert),
                             IM_OFFSETOF(ImDrawVert, uv));
    cwgl_vertexAttribPointer(g_ctx, g_AttribLocationVtxColor,
                             4, (cwgl_enum_t)0x1401, 1, sizeof(ImDrawVert),
                             IM_OFFSETOF(ImDrawVert, col));
}

void
ImGui_ImplCwgl_RenderDrawData(ImDrawData* draw_data){
    int fb_width;
    int fb_height;

    fb_width = (int)(draw_data->DisplaySize.x);
    fb_height = (int)(draw_data->DisplaySize.y);
    if(fb_width <= 0 || fb_height <= 0){
        return;
    }
    // FIXME: Backup states

    SetupRenderState(draw_data, fb_width, fb_height);

    ImVec2 clip_off = draw_data->DisplayPos;
    ImVec2 clip_scale = draw_data->FramebufferScale;
    // Render command lists
    for(int n = 0; n < draw_data->CmdListsCount; n++){
        const ImDrawList* cmd_list = draw_data->CmdLists[n];

        cwgl_bufferData(g_ctx, (cwgl_enum_t)0x8892, cmd_list->VtxBuffer.Size *
                        sizeof(ImDrawVert), cmd_list->VtxBuffer.Data,
            (cwgl_enum_t)0x88E0);
        cwgl_bufferData(g_ctx, (cwgl_enum_t)0x8893, cmd_list->IdxBuffer.Size *
                        sizeof(ImDrawIdx), cmd_list->IdxBuffer.Data,
            (cwgl_enum_t)0x88E0);

        for(int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++){
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if(pcmd->UserCallback != NULL){
                if(pcmd->UserCallback == ImDrawCallback_ResetRenderState){
                    SetupRenderState(draw_data, fb_width, fb_height);
                }else{
                    pcmd->UserCallback(cmd_list, pcmd);
                }
            }else{
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if(clip_rect.x < fb_width && clip_rect.y < fb_height &&
                   clip_rect.z >= 0.0f && clip_rect.w >= 0.0f){
                    cwgl_scissor(g_ctx, clip_rect.x, fb_height - clip_rect.w,
                                 clip_rect.z - clip_rect.x, clip_rect.w - clip_rect.y);
                    cwgl_bindTexture(g_ctx, (cwgl_enum_t)0x0DE1,
                                     (cwgl_Texture_t*)(intptr_t)pcmd->TextureId);

                    cwgl_drawElements(g_ctx, (cwgl_enum_t)0x0004, pcmd->ElemCount,
                                      sizeof(ImDrawIdx) == 2 ? (cwgl_enum_t)0x1403 : (cwgl_enum_t)0x1405,
                                      pcmd->IdxOffset * sizeof(ImDrawIdx));
                }
            }
        }
    }


    // FIXME: Restore states
}


