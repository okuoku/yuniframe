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

    printf("debug:\n");
    shxm_private_discomp_spirv(ir, len);

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
                    ment = calc_ent(intr, ir[i+1]);
                    if(! ment){
                        printf("ERROR: Invalid offset op %d\n", op);
                        failed = 1;
                    }else{
                        ment->name = i;
                    }
                    break;
                case 6: /* OpMemberName */
                    printf("!!! FIXME !!! OpMemberName.!\n");
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

