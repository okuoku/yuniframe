#include "shxm_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATCH_MAX_LEN 16000

struct patchctx_s {
    int phase;
    uint32_t* ir;
    shxm_program_t* prog;
    shxm_spirv_intr_t* intr;
};

static int
patch_binding_numbers(struct patchctx_s* cur,
                     shxm_util_buf_t* decorations){
    int i;
    int id;
    shxm_opaque_t* o;
    uint32_t op[4];
    // FIXME: Bind UBO to 0 here
    for(i=0;i!=cur->prog->opaque_count;i++){
        o = &cur->prog->opaque[i];
        if(o->phase == cur->phase){
            // FIXME: Check existing decoration here
            id = o->slot->id[cur->phase];
            op[0] = 71; /* OpDecorate */
            op[1] = id;
            op[2] = 33; /* Binding */
            op[3] = o->binding;
            if(shxm_private_util_buf_write_op(decorations, op, 4)){
                return 1;
            }
            op[0] = 71; /* OpDecorate */
            op[1] = id;
            op[2] = 34; /* DescriptorSet */
            op[3] = 0;
            if(shxm_private_util_buf_write_op(decorations, op, 4)){
                return 1;
            }
        }
    }
    return 0;
}



int
shxm_private_patch_spirv(shxm_ctx_t* ctx,
                         shxm_program_t* prog,
                         shxm_spirv_intr_t* intr,
                         int phase){
    int failed = 1;
    int defs_start;
    int defs_end;
    int entrypoint_start;
    int curid;
    int total_len;

    struct patchctx_s cur;
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

    cur.phase = phase;
    cur.prog = prog;
    cur.ir = temp_ir;
    cur.intr = intr;

    patch_decoration = shxm_private_util_buf_new(PATCH_MAX_LEN);
    if(!patch_decoration){
        goto fail_decoration;
    }
    patch_defs = shxm_private_util_buf_new(PATCH_MAX_LEN);
    if(!patch_defs){
        goto fail_defs;
    }
    patch_main = shxm_private_util_buf_new(PATCH_MAX_LEN);
    if(!patch_main){
        goto fail_main;
    }

    /* Generate patches */
    if(patch_binding_numbers(&cur, patch_decoration)){
        goto done;
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
        goto fail_final;
    }

    /* Merge into final output */
    if(shxm_private_util_buf_write_raw(final_output, temp_ir,
                                       defs_start)){
        goto done;
    }
    if(shxm_private_util_buf_merge(final_output, patch_decoration)){
        goto done;
    }
    if(shxm_private_util_buf_write_raw(final_output, &temp_ir[defs_start],
                                       defs_end - defs_start)){
        goto done;
    }
    if(shxm_private_util_buf_merge(final_output, patch_defs)){
        goto done;
    }
    if(shxm_private_util_buf_write_raw(final_output, &temp_ir[defs_end],
                                       entrypoint_start - defs_end)){
        goto done;
    }
    if(shxm_private_util_buf_merge(final_output, patch_main)){
        goto done;
    }
    if(shxm_private_util_buf_write_raw(final_output, &temp_ir[entrypoint_start],
                                       temp_ir_len - entrypoint_start)){
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

