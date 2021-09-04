#include "shxm_private.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


struct iddata_s {
    enum {
        TYPE_LABEL,
        TYPE_REGISTER,
        TYPE_VARIABLE
    } type;
    int loc_set;
    int loc_lastref;
    int regno;
    /* link list for registers */
    int prev;
    int next;
};

typedef struct iddata_s iddata_t;

struct asmctx_s {
    int last_reg;
    iddata_t* idd;
};

typedef struct asmctx_s asmctx_t;

static int max_regs = 0;

static int /* bool */
fill_opinfo(int idx, uint32_t op, int oplen, uint32_t* ops, asmctx_t* ctx){
    uint32_t in0 = 0;
    uint32_t in1 = 0;
    uint32_t in2 = 0;
    uint32_t out = 0;
    int i;
    iddata_t* idd = ctx->idd;

    (void) oplen;
    switch(op){
        /** Ext **/
        case 12: /* OpExtInst */
            out = ops[2];
            switch(ops[4]){ // Assume GLSL.std.450
                case 4: /* FAbs x */
                case 13: /* Sin x */
                case 14: /* Cos x */
                case 29: /* Exp2 x */
                case 30: /* Log2 x */
                case 31: /* Sqrt x */
                case 32: /* InverseSqrt x */
                    in0 = ops[5];
                    break;
                case 37: /* FMin x y */
                case 40: /* FMax x y */
                    in0 = ops[5];
                    in1 = ops[6];
                    break;
                case 43: /* FClamp x y z */
                    in0 = ops[5];
                    in1 = ops[6];
                    in2 = ops[7];
                    break;
                default:
                    printf("Unrecognized ext %d\n", ops[4]);
                    abort();
                    return 0;
            }
            break;
        /** Arithmetic **/
        case 127: /* OpFNegate */
            out = ops[2];
            in0 = ops[3];
            break;
        case 129: /* OpFAdd */
        case 131: /* OpFSub */
        case 133: /* OpFMul */
        case 136: /* OpFDiv */
        case 148: /* OpDot */
        case 180: /* OpFOrdEqual */
        case 183: /* OpFUnordNotEqual */
        case 184: /* OpFOrdLessThan */
        case 190: /* OpFOrdGreaterThanEqual */
            out = ops[2];
            in0 = ops[3];
            in1 = ops[4];
            break;
        case 169: /* OpSelect */
            out = ops[2];
            in0 = ops[3];
            in1 = ops[4];
            in2 = ops[5];
            break;
        /** Memory ops **/
        case 61: /* OpLoad */
            out = ops[2];
            break;
        case 62: /* OpStore */
            in0 = ops[2];
            break;
        case 65: /* OpAccessChain */
            break;
        case 79: /* OpVectorShuffle */
            out = ops[2];
            in0 = ops[3];
            in1 = ops[4];
            break;
        case 80: /* OpCompositeConstruct */
            out = ops[2];
            for(i=3;i!=oplen;i++){
                // NB: Special handling
                idd[ops[i]].loc_lastref = idx;
            }
            break;
        case 81: /* OpCompositeExtract */
            out = ops[2];
            in0 = ops[3];
            break;
        case 82: /* OpCompositeInsert */
            out = ops[2];
            in0 = ops[3];
            in1 = ops[4];
            break;
        /** Shader ops **/
        case 87: /* OpImageSampleImplicitLod */
        case 88: /* OpImageSampleExplicitLod */
            out = ops[2];
            in0 = ops[3];
            in1 = ops[4];
            if(oplen == 7){
                if(ops[5] != 2 /* Lod */){
                    printf("Non Lod argument!\n");
                    abort();
                }
                in2 = ops[6];
            }else if(oplen != 5){
                printf("Unrecognized fetch argument\n");
                abort();
            }
            break;
        case 252: /* OpKill */
            break;
        /** Flow **/
        case 245: /* OpPhi */
            out = ops[2];
            for(i=3;i!=oplen;i++){
                // NB: Special handling
                idd[ops[i]].loc_lastref = idx;
            }
            break;
        case 247: /* OpSelectionMerge */
            break;
        case 248: /* OpLabel */
            idd[ops[1]].type = TYPE_LABEL;
            break;
        case 249: /* OpBranch */
            break;
        case 250: /* OpBranchConditional */
            break;
        case 253: /* OpReturn */
            break;
        /** Decl **/
        case 1: /* OpUndef */
        case 19: /* OpTypeVoid */
        case 20: /* OpTypeBool */
        case 21: /* OpTypeInt */
        case 22: /* OpTypeFloat */
        case 23: /* OpTypeVector */
        case 25: /* OpTypeImage */
        case 27: /* OpTypeSampledImage */
        case 28: /* OpTypeArray */
        case 30: /* OpTypeStruct */
        case 32: /* OpTypePointer */
        case 33: /* OpTypeFunction */
        case 43: /* OpConstant */
        case 44: /* OpConstantComposite */
        case 46: /* OpConstantNull */
        case 54: /* OpFunction */
        case 56: /* OpFunctionEnd */
            break;
        case 59: /* OpVariable */
            if(ops[3] == 6 /* Private */ ||
               ops[3] == 7 /* Function */){
                printf("Unrecognized variable storage class %d\n", ops[3]);
                abort();
            }
            idd[ops[2]].type = TYPE_VARIABLE;
            break;
        /** Metadata **/
        case 3: /* OpSource */
        case 4: /* OpSourceExtension */
        case 5: /* OpName */
        case 6: /* OpMemberName */
        case 11: /* OpExtInstImport */
        case 14: /* OpMemoryModel */
        case 15: /* OpEntryPoint */
        case 16: /* OpExecutionMode */
        case 17: /* OpCapability */
        case 71: /* OpDecorate */
        case 72: /* OpMemberDecorate */
            break;

        default:
            printf("unrecognized op %d \n", op);
            abort();
            return 0;
    }
    if(out){
        if(idd[out].loc_set){
            printf("Doubly set id\n");
            abort();
        }
        idd[out].prev = ctx->last_reg;
        idd[ctx->last_reg].next = out;
        ctx->last_reg = out;
        idd[out].loc_set = idx;
    }
    if(in0){
        idd[in0].loc_lastref = idx;
    }
    if(in1){
        idd[in1].loc_lastref = idx;
    }
    if(in2){
        idd[in2].loc_lastref = idx;
    }
    return 1;
}

#define MAX_REGS 4096

int
shxm_ms_asm0(uint32_t* ops, int len){
    int cur;
    int op;
    int oplen;
    int idcount;
    int i,r;
    int* regs;
    int curreg;
    asmctx_t ctx;
    iddata_t* idd;
    idcount = ops[3];
    idd = malloc(sizeof(iddata_t)*idcount);

    for(i=0;i!=idcount;i++){
        idd[i].type = TYPE_REGISTER;
        idd[i].loc_set = 0;
        idd[i].loc_lastref = 0;
        idd[i].next = 0;
        idd[i].prev = 0;
    }
    ctx.idd = idd;
    ctx.last_reg = 0;

    /* Pass1: Check for types and liveness */
    cur = 5;
    for(;;){
        op = ops[cur] & 0xffff;
        oplen = ops[cur] >> 16;
        if(! fill_opinfo(cur, op, oplen, &ops[cur], &ctx)){
            return 1;
        }
        cur += oplen;
        if(cur >= len){
            break;
        }
    }

    regs = malloc(sizeof(int)*MAX_REGS);
    for(i=0;i!=MAX_REGS;i++){
        regs[i] = -1;
    }
    /* Pass2: Allocate registers */
    curreg = ctx.last_reg;
    if(curreg){
        for(;;){
            if(! idd[curreg].prev){
                break;
            }
            curreg = idd[curreg].prev;
        }
        while(curreg){
            if(idd[curreg].type == TYPE_REGISTER){
                for(r=0;r!=MAX_REGS;r++){
                    if(regs[r] < idd[curreg].loc_set){
                        idd[curreg].regno = r;
                        regs[r] = idd[curreg].loc_lastref;
                        break;
                    }
                }
                if(r == MAX_REGS){
                    printf("Insufficient registers...\n");
                    abort();
                }
            }
            curreg = idd[curreg].next;
        }
        r = 0;
        for(i=0;i!=MAX_REGS;i++){
            if(regs[i] != -1){
                r++;
            }
        }
        if(r > max_regs){
            max_regs = r;
        }
        printf("Register pressure = %d (max %d)\n",r,max_regs);
    }
    

    free(idd);
    free(regs);
    return 0;
}


