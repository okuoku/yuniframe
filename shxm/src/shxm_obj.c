#include "shxm.h"
#include "shxm_private.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Globals */
static int glslang_init = 0;

shxm_ctx_t global_ctx;

/* Glslang interface */
void shxm_glslang_init(void);
void shxm_glslang_deinit(void);
int shxm_glslang_build(int mode, const char* source,
                   int** out_spv, int* out_spvlen);

SHXM_API shxm_ctx_t* 
shxm_init(void){
    if(! glslang_init){
        shxm_glslang_init();
        glslang_init = 1;
    }
    return &global_ctx;
}

SHXM_API shxm_shader_t* 
shxm_shader_create(shxm_ctx_t* ctx, shxm_shader_stage_t type){
    shxm_shader_t* shader;
    shader = malloc(sizeof(shxm_shader_t));
    if(shader){
        // Assume NULL is zero
        memset(shader, 0, sizeof(shxm_shader_t));
        shader->refcnt = 1;
        shader->type = type;
    }
    return shader;
}

SHXM_API void 
shxm_shader_release(shxm_ctx_t* ctx, shxm_shader_t* sh){
    if(sh){
        if(sh->refcnt){
            sh->refcnt--;
            if(! sh->refcnt){
                free(sh->source);
                free(sh->ir);
                free(sh);
            }
        }else{
            printf("ERROR: minus refcnt..?\n");
        }
    }
}

SHXM_API shxm_program_t* 
shxm_program_create(shxm_ctx_t* ctx){
    shxm_program_t* program;
    program = malloc(sizeof(shxm_program_t));
    if(program){
        // Assume NULL is zero
        memset(program, 0, sizeof(shxm_program_t));
    }
    return program;
}

SHXM_API void 
shxm_program_release(shxm_ctx_t* ctx, shxm_program_t* prog){
    if(prog){
        shxm_shader_release(ctx, prog->vertex_shader);
        shxm_shader_release(ctx, prog->fragment_shader);
        free(prog->vertex_ir);
        free(prog->fragment_ir);
        free(prog);
    }
}

SHXM_API void 
shxm_shader_source(shxm_ctx_t* ctx, shxm_shader_t* sh,
                                 const char* src /* Copied */,
                                 unsigned int len){
    char* cpsrc;
    cpsrc = malloc(len);
    memcpy(cpsrc, src, len);
    free(sh->source);
    sh->source = cpsrc;
    sh->source_len = len;
}


SHXM_API int 
shxm_shader_compile(shxm_ctx_t* ctx, shxm_shader_t* sh){
    int irlen;
    int* irptr;
    int r;
    int mode;
    mode = (sh->type == SHXM_SHADER_STAGE_FRAGMENT) ? 0 : 1;
    r = shxm_glslang_build(mode, sh->source, &irptr, &irlen);
    if(!r){
        // FIXME: Assume int == int32_t
        sh->ir = irptr;
        sh->ir_len = irlen;
        return 0;
    }else{
        return r;
    }
}

SHXM_API void 
shxm_program_attach(shxm_ctx_t* ctx, shxm_program_t* prog, shxm_shader_t* sh){
    if(sh->type == SHXM_SHADER_STAGE_FRAGMENT){
        if(prog->fragment_shader){
            shxm_program_detach(ctx, prog, prog->fragment_shader);
            // FIXME: Unlink here
        }
        prog->fragment_shader = sh;
    }else if(sh->type == SHXM_SHADER_STAGE_VERTEX){
        if(prog->vertex_shader){
            shxm_program_detach(ctx, prog, prog->vertex_shader);
            // FIXME: Unlink here
        }
        prog->vertex_shader = sh;
    }else{
        printf("ERROR: ???\n");
    }
    sh->refcnt++;
}

SHXM_API int 
shxm_program_detach(shxm_ctx_t* ctx, shxm_program_t* prog, shxm_shader_t* sh){
    // FIXME: Check if program is valid
    shxm_shader_release(ctx, sh);
    return 0;
}
