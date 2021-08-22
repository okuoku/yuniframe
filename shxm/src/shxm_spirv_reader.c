#include "shxm.h"
#include "shxm_private.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static shxm_spirv_ent_t*
calc_ent(shxm_spirv_intr_t* intr, int id){
    if(id < intr->ent_count){
        return &intr->ent[id];
    }else{
        printf("!!! Invalid id offset!(%d)\n", id);
        return NULL;
    }
}

shxm_spirv_intr_t* 
shxm_private_read_spirv(uint32_t* ir, int len){
    shxm_spirv_intr_t* intr;
    shxm_spirv_ent_t* ent;
    shxm_spirv_ent_t* ment;
    int failed = 0;
    int prev;
    uint32_t magic;
    uint32_t version;
    uint32_t bound;
    uint32_t zero;
    uint32_t oplen;
    uint32_t op;
    int i;
    if(len < 5){
        printf("ERROR: Too short(%d)\n", len);
        return NULL;
    }

    magic = ir[0];
    version = ir[1];
    bound = ir[3];
    zero = ir[4];

    printf("Reading: magic = %x version = %x bound = %d zero = %d\n",
           magic, version, bound, zero);

    intr = malloc(sizeof(shxm_spirv_intr_t));
    if(! intr){
        return NULL;
    }
    intr->chain = malloc(sizeof(unsigned int)*len);
    if(! intr->chain){
        free(intr);
        return NULL;
    }
    ent = malloc(sizeof(shxm_spirv_ent_t)*bound);
    intr->entrypoint = 0;
    intr->defs_start = 0;
    intr->defs_end = 0;
    intr->preamble_end = 0;
    intr->int32_type_id = 0;
    intr->float32_type_id = 0;
    intr->voidfunc_type_id = 0;
    intr->void_type_id = 0;
    if(ent){
        intr->ent = ent;
        intr->ent_count = bound;
        memset(ent, 0, sizeof(shxm_spirv_ent_t)*bound);
        memset(intr->chain, 0, sizeof(unsigned int)*len);
        i = 5;
        while(i<len){
            oplen = (ir[i]) >> 16;
            op = (ir[i]) & 0xffff;
            //printf("[%d] op = %d len = %d(%d)\n", i, op, oplen,len);
            if(oplen == 0){
                break;
            }
            switch(op){
                /* Names */
                case 5: /* OpName */
                    if(! intr->preamble_end){
                        intr->preamble_end = i;
                    }
                    ment = calc_ent(intr, ir[i+1]);
                    if(! ment){
                        printf("ERROR: Invalid offset op %d\n", op);
                        failed = 1;
                    }else{
                        ment->name = i;
                    }
                    break;
                case 6: /* OpMemberName */
                    if(! intr->preamble_end){
                        intr->preamble_end = i;
                    }
                    break;

                /* Types */
                case 19: /* OpTypeVoid */
                case 20: /* OpTypeBool */
                case 21: /* OpTypeInt */
                case 22: /* OpTypeFloat */
                case 23: /* OpTypeVector */
                case 24: /* OpTypeMatrix */
                case 25: /* OpTypeImage */
                case 26: /* OpTypeSampler */
                case 27: /* OpTypeSampledImage */
                case 28: /* OpTypeArray */
                case 32: /* OpTypePointer */
                case 33: /* OpTypeFunction */
                    ment = calc_ent(intr, ir[i+1]);
                    if(! ment){
                        printf("ERROR: Invalid offset op %d\n", op);
                        failed = 1;
                    }else{
                        ment->offs = i;
                        ment->op = op;
                    }
                    if(! intr->defs_start){
                        intr->defs_start = i;
                    }
                    if(op == 19 /* OpTypeVoid */){
                        intr->void_type_id = ir[i+1];
                    }
                    if(op == 21 /* OpTypeInt */){
                        if(ir[i+2] == 32 && ir[i+3] == 0){
                            intr->int32_type_id = ir[i+1];
                        }
                    }
                    if(op == 22 /* OpTypeFloat */){
                        if(ir[i+2] == 32){
                            intr->float32_type_id = ir[i+1];
                        }
                    }
                    if(op == 33 /* OpTypeFunction */){
                        if(ir[i+2] == intr->void_type_id &&
                           oplen == 3){
                            intr->voidfunc_type_id = ir[i+1];
                        }
                    }
                    break;
                case 43: /* OpConstant */
                case 54: /* OpFunction */
                case 59: /* OpVariable */
                    ment = calc_ent(intr, ir[i+2]);
                    if(! ment){
                        printf("ERROR: Invalid offset op %d\n", op);
                        failed = 1;
                    }else{
                        ment->offs = i;
                        ment->op = op;
                    }
                    if(op == 54 /* OpFunction */ && (intr->defs_end == 0)){
                        intr->defs_end = i;
                    }
                    break;

                /* Decorations */
                case 71: /* OpDecorate */
                    ment = calc_ent(intr, ir[i+1]);
                    if(! ment){
                        printf("ERROR: Invalid offset op %d\n", op);
                        failed = 1;
                    }else{
                        prev = ment->chain;
                        intr->chain[i] = prev;
                        ment->chain = i;
                    }
                    break;
                case 72: /* OpMemberDecorate */
                    printf("!!! FIXME !!! OpMemberDecorate.!\n");
                    break;
                /* Misc */
                case 15: /* OpEntryPoint */
                    if(intr->entrypoint){
                        printf("ERROR: Too many entry points.!\n");
                        failed = 1;
                    }
                    intr->entrypoint = ir[i+2];
                    intr->entrypoint_loc = i;
                    break;
                default:
                    /* Do nothing, skip insn */
                    break;
            }
            i+=oplen;
        }
    }else{
        printf("ERROR: Insufficient memory?? (%d)\n", bound);
        free(intr->chain);
        free(intr);
    }
    if(intr->entrypoint == 0 && !failed){
        printf("ERROR: Entrypoint not found!\n");
        failed = 1;
    }
    if(failed){
        // FIXME: Free intr here
        intr = NULL;
    }
    return intr;
}

