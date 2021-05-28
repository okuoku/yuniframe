#include "shxm_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _MSC_VER
#include <malloc.h>
#else
#include <alloca.h>
#endif

#define PATCH_MAX_LEN 16000

struct idpatchparam_s {
    int ubo_index;
    int ubo_type; /* Took from source */
    int ubo_uniform_constant_pointer_type; /* Took from source */
    int ubo_private_pointer_type; /* Generate */
    int ubo_uniform_pointer_type; /* Generate */
    int access_chain; /* Generate (Base) */
    int access_load; /* Generate (Base) */
    int legalized; /* has Stride decorations */
};

struct patchctx_s {
    int curid;
    int phase;
    int integers;
    int int32_type_id;
    int ubo_variable_id;
    int ubo_pointer_id;
    int ubo_structure_id;

    int integers_id_base;
    uint32_t* ir;
    shxm_program_t* prog;
    shxm_spirv_intr_t* intr;
    struct idpatchparam_s* idpatch; /* Indexed by ID */
};

static int
patch_main_load(struct patchctx_s* cur, shxm_util_buf_t* target){
    int u;
    int id;
    int curid;
    int i;
    uint32_t op[6];
    struct idpatchparam_s* param;
    shxm_uniform_t* uniform;
    for(u=0;u!=cur->prog->uniform_count;u++){
        uniform = &cur->prog->uniform[u];
        id = uniform->slot->id[cur->phase];
        if(id){
            param = &cur->idpatch[id];
            if(uniform->slot->array_length){
                for(i=0;i!=uniform->slot->array_length;i++){
                    /* Generate 2 OpAccessChains for load and store */
                    op[0] = 65; /* OpAccessChain */
                    op[1] = param->ubo_uniform_pointer_type;
                    op[2] = param->access_chain + (i*2);
                    op[3] = cur->ubo_variable_id;
                    op[4] = cur->integers_id_base + param->ubo_index;
                    op[5] = cur->integers_id_base + i;
                    if(shxm_private_util_buf_write_op(target, op, 6)){
                        return 1;
                    }
                    op[0] = 65; /* OpAccessChain */
                    op[1] = param->ubo_private_pointer_type;
                    op[2] = param->access_chain + (i*2) + 1;
                    op[3] = id;
                    op[4] = cur->integers_id_base + i;
                    if(shxm_private_util_buf_write_op(target, op, 5)){
                        return 1;
                    }
                    op[0] = 61; /* OpLoad */
                    op[1] = param->ubo_type;
                    op[2] = param->access_load + i;
                    op[3] = param->access_chain + (i*2);
                    if(shxm_private_util_buf_write_op(target, op, 4)){
                        return 1;
                    }
                    op[0] = 62; /* OpStore */
                    op[1] = param->access_chain + (i*2) + 1;
                    op[2] = param->access_load + i;
                    if(shxm_private_util_buf_write_op(target, op, 3)){
                        return 1;
                    }
                }
            }else{
                op[0] = 65; /* OpAccessChain */
                op[1] = param->ubo_uniform_pointer_type;
                op[2] = param->access_chain;
                op[3] = cur->ubo_variable_id;
                op[4] = cur->integers_id_base + param->ubo_index;
                if(shxm_private_util_buf_write_op(target, op, 5)){
                    return 1;
                }
                op[0] = 61; /* OpLoad */
                op[1] = param->ubo_type;
                op[2] = param->access_load;
                op[3] = param->access_chain;
                if(shxm_private_util_buf_write_op(target, op, 4)){
                    return 1;
                }
                op[0] = 62; /* OpStore */
                op[1] = id;
                op[2] = param->access_load;
                if(shxm_private_util_buf_write_op(target, op, 3)){
                    return 1;
                }
            }
        }
    }

    return 0;
}

static int
inject_integers(struct patchctx_s* cur, shxm_util_buf_t* defs){
    uint32_t op[4];
    int i;

    op[0] = 21; /* OpTypeInt */
    op[1] = cur->int32_type_id;
    op[2] = 32; /* 32bits */
    op[3] = 0; /* unsigned */
    if(shxm_private_util_buf_write_op(defs, op, 4)){
        return 1;
    }

    for(i=0;i<=cur->integers;i++){
        op[0] = 43; /* OpConstant */
        op[1] = cur->int32_type_id;
        op[2] = cur->integers_id_base + i;
        op[3] = i;
        if(shxm_private_util_buf_write_op(defs, op, 4)){
            return 1;
        }
    }
    return 0;
}

static int
fill_ubo_info(struct patchctx_s* cur){
    int u;
    int id;
    int curid;
    int uboindex;
    uint32_t* op;
    shxm_uniform_t* uniform;
    curid = cur->curid;
    /* at least, we need intergers for uniform entries */
    cur->integers = cur->prog->uniform_count;

    uboindex = 0;
    /* Pass1: Fetch/Generate IDs for types and variables */
    for(u=0;u!=cur->prog->uniform_count;u++){
        uniform = &cur->prog->uniform[u];
        id = uniform->slot->id[cur->phase];
        if(id){
            op = &cur->ir[cur->intr->ent[id].offs];
            cur->idpatch[id].ubo_uniform_constant_pointer_type = op[1];
            cur->idpatch[id].ubo_private_pointer_type = curid;
            curid++;
            cur->idpatch[id].ubo_uniform_pointer_type = curid;
            curid++;
            cur->idpatch[id].ubo_index = uboindex;
            uboindex++;
            /* Fetch original type information */
            op = &cur->ir[cur->intr->ent[cur->idpatch[id].ubo_uniform_constant_pointer_type].offs];
            cur->idpatch[id].ubo_type = op[3];
        }
    }

    /* Pass2: Generate access chain */
    for(u=0;u!=cur->prog->uniform_count;u++){
        uniform = &cur->prog->uniform[u];
        id = uniform->slot->id[cur->phase];
        if(id){
            if(uniform->slot->array_length){
                if(uniform->slot->array_length > cur->integers){
                    cur->integers = uniform->slot->array_length;
                }
                cur->idpatch[id].access_chain = curid;
                curid += (uniform->slot->array_length * 2);
                cur->idpatch[id].access_load = curid;
                curid += uniform->slot->array_length;
            }else{
                cur->idpatch[id].access_chain = curid;
                curid++;
                cur->idpatch[id].access_load = curid;
                curid++;
            }
        }
    }
    cur->integers_id_base = curid;
    curid += (cur->integers + 1);
    cur->ubo_variable_id = curid;
    curid++;
    cur->ubo_pointer_id = curid;
    curid++;
    cur->ubo_structure_id = curid;
    curid++;
    cur->int32_type_id = curid;
    curid++;
    cur->curid = curid;

    return 0;
}

static void
nopout(uint32_t* ir){
    int i;
    int len;
    len = ir[0] >> 16;
    for(i=0;i!=len;i++){
        ir[i] = (1 << 16); /* OpNop */
    }
}

static int
patch_uniform_to_private(struct patchctx_s* cur, shxm_util_buf_t* defs){
    int u;
    int id;
    int curid;
    uint32_t op[4];
    struct idpatchparam_s* param;
    shxm_uniform_t* uniform;
    for(u=0;u!=cur->prog->uniform_count;u++){
        uniform = &cur->prog->uniform[u];
        id = uniform->slot->id[cur->phase];
        if(id){
            param = &cur->idpatch[id];
            nopout(&cur->ir[cur->intr->ent[id].offs]);
            op[0] = 32; /* OpTypePointer */
            op[1] = param->ubo_uniform_pointer_type;
            op[2] = 2; /* Uniform */
            op[3] = param->ubo_type;
            if(shxm_private_util_buf_write_op(defs, op, 4)){
                return 1;
            }
            op[0] = 32; /* OpTypePointer */
            op[1] = param->ubo_private_pointer_type;
            op[2] = 6; /* Private */
            op[3] = param->ubo_type;
            if(shxm_private_util_buf_write_op(defs, op, 4)){
                return 1;
            }
            op[0] = 59; /* OpVariable */
            op[1] = param->ubo_private_pointer_type;
            op[2] = id;
            op[3] = 6; /* Private */
            if(shxm_private_util_buf_write_op(defs, op, 4)){
                return 1;
            }
        }
    }
    return 0;
}

static int
inject_opname(shxm_util_buf_t* out, int id, const char* name){
    size_t namelen;
    int namewords;
    uint32_t* op;
    if(!name){
        /* Ignore empty name */
        return 0;
    }
    namelen = strnlen(name, 255);
    namelen++; /* NUL */
    namewords = ((namelen + 3) & (0xffff - 3)) >> 2;
    op = alloca(sizeof(uint32_t)*(namewords+2));
    memset(op, 0, sizeof(uint32_t)*(namewords+2));
    op[0] = 5; /* OpName */
    op[1] = id;
    memcpy(&op[2], name, namelen);
    return shxm_private_util_buf_write_op(out, op, namewords + 2);
}

static int
inject_opmembername(shxm_util_buf_t* out, int id, int idx, const char* name){
    size_t namelen;
    int namewords;
    uint32_t* op;
    if(!name){
        /* Ignore empty name */
        return 0;
    }
    namelen = strnlen(name, 255);
    namelen++; /* NUL */
    namewords = ((namelen + 3) & (0xffff - 3)) >> 2;
    op = alloca(sizeof(uint32_t)*(namewords+3));
    memset(op, 0, sizeof(uint32_t)*(namewords+3));
    op[0] = 6; /* OpName */
    op[1] = id;
    op[2] = idx;
    memcpy(&op[3], name, namelen);
    return shxm_private_util_buf_write_op(out, op, namewords + 3);
}

static int
is_matrix(cwgl_var_type_t type){
    switch(type){
        case CWGL_VAR_FLOAT_MAT2:
        case CWGL_VAR_FLOAT_MAT3:
        case CWGL_VAR_FLOAT_MAT4:
            return 1;
        default:
            return 0;
    }
}

static int
calc_matrix_stride(cwgl_var_type_t type){
    switch(type){
        case CWGL_VAR_FLOAT_MAT2:
            return 4*2;
        case CWGL_VAR_FLOAT_MAT3:
        case CWGL_VAR_FLOAT_MAT4:
            return 4*4;
        default:
            return 0;
    }
}

static int
inject_ubo_def(struct patchctx_s* cur, 
               shxm_util_buf_t* names,
               shxm_util_buf_t* decorations,
               shxm_util_buf_t* defs){
    int u;
    int id;
    int membercount;
    uint32_t* opa;
    uint32_t op[5];
    struct idpatchparam_s* param;
    shxm_uniform_t* uniform;
    if(inject_opname(names, cur->ubo_structure_id, "cwgl_ubo_s")){
        return 1;
    }
    if(inject_opname(names, cur->ubo_variable_id, "cwgl_ubo")){
        return 1;
    }
    op[0] = 71; /* OpDecorate */
    op[1] = cur->ubo_structure_id;
    op[2] = 2; /* Block */
    if(shxm_private_util_buf_write_op(decorations, op, 3)){
        return 1;
    }
    op[0] = 71; /* OpDecorate */
    op[1] = cur->ubo_variable_id;
    op[2] = 33; /* Binding */
    op[3] = 0; /* UBO always bound at zero */
    if(shxm_private_util_buf_write_op(decorations, op, 4)){
        return 1;
    }
    op[0] = 71; /* OpDecorate */
    op[1] = cur->ubo_variable_id;
    op[2] = 34; /* DescriptorSet */
    op[3] = 0;
    if(shxm_private_util_buf_write_op(decorations, op, 4)){
        return 1;
    }

    membercount = 0;
    for(u=0;u!=cur->prog->uniform_count;u++){
        uniform = &cur->prog->uniform[u];
        id = uniform->slot->id[cur->phase];
        if(id){
            membercount++;
            param = &cur->idpatch[id];
            nopout(&cur->ir[cur->intr->ent[id].offs]);

            if(inject_opmembername(names,
                                   cur->ubo_structure_id,
                                   param->ubo_index,
                                   uniform->slot->name)){
                return 1;
            }

            op[0] = 72; /* OpMemberDecorate */
            op[1] = cur->ubo_structure_id;
            op[2] = param->ubo_index;
            op[3] = 35; /* Offset */
            op[4] = uniform->offset;
            if(shxm_private_util_buf_write_op(decorations, op, 5)){
                return 1;
            }

            if(is_matrix(uniform->slot->type)){
                op[0] = 72; /* OpMemberDecorate */
                op[1] = cur->ubo_structure_id;
                op[2] = param->ubo_index;
                op[3] = 5; /* ColMajor */
                if(shxm_private_util_buf_write_op(decorations, op, 4)){
                    return 1;
                }
                op[0] = 72; /* OpMemberDecorate */
                op[1] = cur->ubo_structure_id;
                op[2] = param->ubo_index;
                op[3] = 7; /* MatrixStride */
                op[4] = calc_matrix_stride(uniform->slot->type);
                if(shxm_private_util_buf_write_op(decorations, op, 5)){
                    return 1;
                }
            }
        }
    }

    /* Define structure */
    opa = alloca(sizeof(uint32_t)*(membercount+2));
    opa[0] = 30; /* OpTypeStruct */
    opa[1] = cur->ubo_structure_id;
    membercount = 0;
    for(u=0;u!=cur->prog->uniform_count;u++){
        uniform = &cur->prog->uniform[u];
        id = uniform->slot->id[cur->phase];
        if(id){
            param = &cur->idpatch[id];
            if(uniform->slot->type == CWGL_VAR_BOOL){
                /* Promote boolean type to int32 */
                opa[2+membercount] = cur->int32_type_id;
            }else{
                opa[2+membercount] = param->ubo_type;
            }
            membercount++;

            if(uniform->slot->array_length){
                if(! cur->idpatch[param->ubo_type].legalized){
                    op[0] = 71; /* OpDecorate */
                    op[1] = param->ubo_type;
                    op[2] = 6; /* ArrayStride */
                    op[3] = uniform->size / uniform->slot->array_length;
                    if(shxm_private_util_buf_write_op(decorations, op, 4)){
                        return 1;
                    }
                    cur->idpatch[param->ubo_type].legalized = 1;
                }
            }
        }
    }
    if(shxm_private_util_buf_write_op(defs, opa, 2+membercount)){
        return 1;
    }
    op[0] = 32; /* OpTypePointer */
    op[1] = cur->ubo_pointer_id;
    op[2] = 2; /* Uniform */
    op[3] = cur->ubo_structure_id;
    if(shxm_private_util_buf_write_op(defs, op, 4)){
        return 1;
    }
    op[0] = 59; /* OpVariable */
    op[1] = cur->ubo_pointer_id;
    op[2] = cur->ubo_variable_id;
    op[3] = 2; /* Uniform */
    if(shxm_private_util_buf_write_op(defs, op, 4)){
        return 1;
    }

    return 0;
}

static int
patch_binding_numbers(struct patchctx_s* cur, shxm_util_buf_t* decorations){
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

static int
patch_locations(struct patchctx_s* cur, shxm_util_buf_t* decorations){
    int i;
    int id;
    uint32_t op[4];
    shxm_attribute_t* a;
    for(i=0;i!=cur->prog->input_count;i++){
        // FIXME: Search existing location 
        a = &cur->prog->input[i];
        if(a->location >= 0){ /* Not Built-in */
            id = a->slot->id[cur->phase];
            if(id > 0){
                op[0] = 71; /* OpDecorate */
                op[1] = id;
                op[2] = 30; /* Location */
                op[3] = a->location;
                if(shxm_private_util_buf_write_op(decorations, op, 4)){
                    return 1;
                }
            }
        }
    }
    for(i=0;i!=cur->prog->output_count;i++){
        // FIXME: Search existing location 
        a = &cur->prog->output[i];
        if(a->location >= 0){ /* Not Built-in */
            id = a->slot->id[cur->phase];
            if(id > 0){
                op[0] = 71; /* OpDecorate */
                op[1] = id;
                op[2] = 30; /* Location */
                op[3] = a->location;
                if(shxm_private_util_buf_write_op(decorations, op, 4)){
                    return 1;
                }
            }
        }
    }
    for(i=0;i!=cur->prog->varying_count;i++){
        // FIXME: Search existing location 
        a = &cur->prog->varying[i];
        if(a->location >= 0){ /* Not Built-in */
            id = a->slot->id[cur->phase];
            if(id > 0){
                op[0] = 71; /* OpDecorate */
                op[1] = id;
                op[2] = 30; /* Location */
                op[3] = a->location;
                if(shxm_private_util_buf_write_op(decorations, op, 4)){
                    return 1;
                }
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
    int preamble_end;
    int entrypoint_start;
    int curid;
    int total_len;

    struct patchctx_s cur;
    shxm_util_buf_t* patch_names;
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
    cur.curid = intr->ent_count;

    cur.idpatch = malloc(sizeof(struct idpatchparam_s)*cur.curid);
    if(! cur.idpatch){
        goto fail_idpatch;
    }
    memset(cur.idpatch, 0, sizeof(struct idpatchparam_s)*cur.curid);

    patch_names = shxm_private_util_buf_new(PATCH_MAX_LEN);
    if(!patch_names){
        goto fail_names;
    }
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
    if(patch_locations(&cur, patch_decoration)){
        goto done;
    }
    if(fill_ubo_info(&cur)){
        goto done;
    }
    cur.ir[3] = cur.curid; /* Patch Bounds */
    if(patch_uniform_to_private(&cur, patch_defs)){
        goto done;
    }
    if(inject_integers(&cur, patch_defs)){
        goto done;
    }
    if(inject_ubo_def(&cur, patch_names, patch_decoration, patch_defs)){
        goto done;
    }
    if(patch_main_load(&cur, patch_main)){
        goto done;
    }

    preamble_end = intr->preamble_end;
    defs_start = intr->defs_start;
    defs_end = intr->defs_end;
    entrypoint_start = 
        intr->ent[intr->entrypoint].offs + 5 /* OpFunction len */
        + 2 /* OpLabel len */;

    printf("Patch points: %d - %d - %d - %d\n",
           preamble_end,
           defs_start, defs_end, entrypoint_start);

    total_len = temp_ir_len +
        shxm_private_util_buf_size(patch_names) +
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
                                       intr->preamble_end)){
        goto done;
    }
    if(shxm_private_util_buf_merge(final_output, patch_names)){
        goto done;
    }
    if(shxm_private_util_buf_write_raw(final_output, &temp_ir[preamble_end],
                                       defs_start - preamble_end)){
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

    shxm_private_util_buf_clearnop(final_output);

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
    shxm_private_util_buf_release(patch_names);
fail_names:
    free(cur.idpatch);
fail_idpatch:
    free(temp_ir);

    if(failed){
        return 1;
    }else{
        return 0;
    }
}

