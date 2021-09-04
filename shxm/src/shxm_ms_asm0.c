#include "shxm_private.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

struct iddata_s {
    int loc_set;
    int loc_lastref;
};

typedef struct iddata_s iddata_t;

static int /* bool */
fill_opinfo(int idx, uint32_t op, int oplen, uint32_t* ops, iddata_t* idd){
    uint32_t in0 = 0;
    uint32_t in1 = 0;
    uint32_t in2 = 0;
    uint32_t out = 0;
    int i;

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
            // FIXME: Set is_label
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
        case 59: /* OpVariable */
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

int
shxm_ms_asm0(uint32_t* ops, int len){
    int cur;
    int op;
    int oplen;
    int idcount;
    int i;
    iddata_t* idd;
    idcount = ops[3];
    cur = 5;
    idd = malloc(sizeof(iddata_t)*idcount);

    for(i=0;i!=idcount;i++){
        idd[i].loc_set = 0;
        idd[i].loc_lastref = 0;
    }

    for(;;){
        op = ops[cur] & 0xffff;
        oplen = ops[cur] >> 16;
        if(! fill_opinfo(cur, op, oplen, &ops[cur], idd)){
            return 1;
        }
        cur += oplen;
        if(cur >= len){
            return 0;
        }
    }
}


