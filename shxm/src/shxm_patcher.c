#include "shxm_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATCH_MAX_LEN 16000

int
shxm_private_patch_spirv(shxm_ctx_t* ctx,
                         shxm_program_t* prog,
                         shxm_spirv_intr_t* intr,
                         int phase){
    int failed = 0;
    int defs_start;
    int defs_end;
    int entrypoint_start;
    int curid;
    int total_len;

    shxm_util_buf_t* patch_decoration;
    shxm_util_buf_t* patch_defs;
    shxm_util_buf_t* patch_main;
    shxm_util_buf_t* final_output;

    uint32_t* source_ir;
    unsigned int source_ir_len;
    uint32_t* temp_ir;
    unsigned int temp_ir_len;

    if(phase == 0){
        source_ir = prog->vertex_shader->ir;
        source_ir_len = prog->vertex_shader->ir_len;
    }else{
        source_ir = prog->fragment_shader->ir;
        source_ir_len = prog->fragment_shader->ir_len;
    }

    temp_ir = malloc(sizeof(uint32_t)*source_ir_len);
    if(! temp_ir){
        return 1;
    }
    memcpy(temp_ir, source_ir, sizeof(uint32_t)*source_ir_len);
    temp_ir_len = source_ir_len;

    patch_decoration = shxm_private_util_buf_new(PATCH_MAX_LEN);
    if(!patch_decoration){
        failed = 1;
        goto fail_decoration;
    }
    patch_defs = shxm_private_util_buf_new(PATCH_MAX_LEN);
    if(!patch_defs){
        failed = 1;
        goto fail_defs;
    }
    patch_main = shxm_private_util_buf_new(PATCH_MAX_LEN);
    if(!patch_main){
        failed = 1;
        goto fail_main;
    }

    defs_start = intr->defs_start;
    defs_end = intr->defs_end;
    entrypoint_start = 
        intr->ent[intr->entrypoint].offs + 5 /* OpFunction len */;

    printf("Patch points: %d - %d - %d\n",
           defs_start, defs_end, entrypoint_start);

    total_len = temp_ir_len +
        shxm_private_util_buf_size(patch_decoration) +
        shxm_private_util_buf_size(patch_defs) +
        shxm_private_util_buf_size(patch_main);

    printf("Total len = %d\n",total_len);
    final_output = shxm_private_util_buf_new(total_len);

    if(! final_output){
        failed = 1;
        goto fail_final;
    }

    /* Merge into final output */
    if(shxm_private_util_buf_write_raw(final_output, temp_ir,
                                       defs_start)){
        failed = 1;
        goto done;
    }
    if(shxm_private_util_buf_merge(final_output, patch_decoration)){
        failed = 1;
        goto done;
    }
    if(shxm_private_util_buf_write_raw(final_output, &temp_ir[defs_start],
                                       defs_end - defs_start)){
        failed = 1;
        goto done;
    }
    if(shxm_private_util_buf_merge(final_output, patch_defs)){
        failed = 1;
        goto done;
    }
    if(shxm_private_util_buf_write_raw(final_output, &temp_ir[defs_end],
                                       entrypoint_start - defs_end)){
        failed = 1;
        goto done;
    }
    if(shxm_private_util_buf_merge(final_output, patch_main)){
        failed = 1;
        goto done;
    }
    if(shxm_private_util_buf_write_raw(final_output, &temp_ir[entrypoint_start],
                                       temp_ir_len - entrypoint_start)){
        failed = 1;
        goto done;
    }

    /* Allocate and copy final output */
    if(phase == 0){
        failed = shxm_private_util_buf_dup(final_output,
                                           &prog->vertex_ir,
                                           &prog->vertex_ir_len);
    }else{
        failed = shxm_private_util_buf_dup(final_output,
                                           &prog->fragment_ir,
                                           &prog->fragment_ir_len);
    }


done:
    shxm_private_util_buf_release(final_output);
fail_final:
    shxm_private_util_buf_release(patch_main);
fail_main:
    shxm_private_util_buf_release(patch_defs);
fail_defs:
    shxm_private_util_buf_release(patch_decoration);
fail_decoration:
    free(temp_ir);

    if(failed){
        return 1;
    }else{
        return 0;
    }
}

