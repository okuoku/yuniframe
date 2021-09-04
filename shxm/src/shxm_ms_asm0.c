#include "shxm_private.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

static int /* bool */
fill_opinfo(int idx, uint32_t op, int oplen, uint32_t* ops){
    (void) oplen;
    switch(op){
        /** Ext **/
        case 12: /* OpExtInst */
            switch(ops[4]){ // Assume GLSL.std.450
                case 4: /* FAbs */
                case 13: /* Sin */
                case 14: /* Cos */
                case 29: /* Exp2 */
                case 30: /* Log2 */
                case 31: /* Sqrt */
                case 32: /* InverseSqrt */
                case 37: /* FMin */
                case 40: /* FMax */
                case 43: /* FClamp */
                    return 1;
                default:
                    printf("Unrecognized ext %d\n", ops[4]);
                    return 0;
            }
            return 0;
        /** Arithmetic **/
        case 127: /* OpFNegate */
        case 129: /* OpFAdd */
        case 131: /* OpFSub */
        case 133: /* OpFMul */
        case 136: /* OpFDiv */
        case 148: /* OpDot */
        case 169: /* OpSelect */
        case 180: /* OpFOrdEqual */
        case 183: /* OpFUnordNotEqual */
        case 184: /* OpFOrdLessThan */
        case 190: /* OpFOrdGreaterThanEqual */
        /** Memory ops **/
        case 61: /* OpLoad */
        case 62: /* OpStore */
        case 65: /* OpAccessChain */
        case 79: /* OpVectorShuffle */
        case 80: /* OpCompositeConstruct */
        case 81: /* OpCompositeExtract */
        case 82: /* OpCompositeInsert */
        /** Shader ops **/
        case 87: /* OpImageSampleImplicitLod */
        case 88: /* OpImageSampleExplicitLod */
        case 252: /* OpKill */
        /** Flow **/
        case 245: /* OpPhi */
        case 247: /* OpSelectionMerge */
        case 248: /* OpLabel */
        case 249: /* OpBranch */
        case 250: /* OpBranchConditional */
        case 253: /* OpReturn */
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
            return 1;

        default:
            printf("unrecognized op %d \n", op);
            return 0;
    }
}

int
shxm_ms_asm0(uint32_t* ops, int len){
    int cur;
    int op;
    int oplen;
    cur = 5;
    for(;;){
        op = ops[cur] & 0xffff;
        oplen = ops[cur] >> 16;
        if(! fill_opinfo(cur, op, oplen, &ops[cur])){
            return 1;
        }
        cur += oplen;
        if(cur >= len){
            return 0;
        }
    }
}


