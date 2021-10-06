#include "shxm_private.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


struct iddata_s {
    enum {
        TYPE_LABEL,
        TYPE_REGISTER,
        TYPE_VARIABLE,
        TYPE_TYPE,
        TYPE_POINTER,
        TYPE_CONSTANT_VALUE,
        TYPE_CONSTANT_UNDEF,
        TYPE_CONSTANT_COMPOSITE
    } type;
    cwgl_var_type_t cls; /* GL type for type object */
    int bitwidth;
    int elms;
    union {
        int32_t asInt;
        uint32_t asId;
        float asFloat;
    } value[16];
    int typeid;
    int basetype;
    /* Register assignment work area */
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
    uint32_t typeid = 0;
    int i;
    iddata_t* idd = ctx->idd;

    (void) oplen;
    switch(op){
        /** Ext **/
        case 12: /* OpExtInst */
            typeid = ops[1];
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
            typeid = ops[1];
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
            typeid = ops[1];
            out = ops[2];
            in0 = ops[3];
            in1 = ops[4];
            break;
        case 169: /* OpSelect */
            typeid = ops[1];
            out = ops[2];
            in0 = ops[3];
            in1 = ops[4];
            in2 = ops[5];
            break;
        /** Memory ops **/
        case 61: /* OpLoad */
            typeid = ops[1];
            out = ops[2];
            break;
        case 62: /* OpStore */
            typeid = ops[1];
            in0 = ops[2];
            break;
        case 65: /* OpAccessChain */
            // FIXME:
            break;
        case 79: /* OpVectorShuffle */
            typeid = ops[1];
            out = ops[2];
            in0 = ops[3];
            in1 = ops[4];
            break;
        case 80: /* OpCompositeConstruct */
            typeid = ops[1];
            out = ops[2];
            for(i=3;i!=oplen;i++){
                // NB: Special handling
                idd[ops[i]].loc_lastref = idx;
            }
            break;
        case 81: /* OpCompositeExtract */
            typeid = ops[1];
            out = ops[2];
            in0 = ops[3];
            break;
        case 82: /* OpCompositeInsert */
            typeid = ops[1];
            out = ops[2];
            in0 = ops[3];
            in1 = ops[4];
            break;
        /** Shader ops **/
        case 87: /* OpImageSampleImplicitLod */
        case 88: /* OpImageSampleExplicitLod */
            typeid = ops[1];
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
            typeid = ops[1];
            out = ops[2];
            for(i=3;i!=oplen;i++){
                // NB: Special handling
                idd[ops[i]].loc_lastref = idx;
            }
            break;
        case 247: /* OpSelectionMerge */
            break;
        case 248: /* OpLabel */
            out = ops[1];
            idd[out].type = TYPE_LABEL;
            break;
        case 249: /* OpBranch */
            break;
        case 250: /* OpBranchConditional */
            break;
        case 253: /* OpReturn */
            break;
        /** Decl **/
        case 1: /* OpUndef */
            out = ops[1];
            idd[out].type = TYPE_CONSTANT_UNDEF;
            break;
        case 19: /* OpTypeVoid */
            out = ops[1];
            idd[out].type = TYPE_TYPE;
            idd[out].cls = CWGL_VAR_VOID;
            break;
        case 20: /* OpTypeBool */
            out = ops[1];
            idd[out].type = TYPE_TYPE;
            idd[out].cls = CWGL_VAR_BOOL;
            break;
        case 21: /* OpTypeInt */
            out = ops[1];
            idd[out].type = TYPE_TYPE;
            idd[out].cls = CWGL_VAR_INT;
            idd[out].bitwidth = ops[2];
            // FIXME: Implement signedness ??
            break;
        case 22: /* OpTypeFloat */
            out = ops[1];
            idd[out].type = TYPE_TYPE;
            idd[out].cls = CWGL_VAR_FLOAT;
            idd[out].bitwidth = ops[2];
            break;
        case 23: /* OpTypeVector */
            // FIXME:
            break;
        case 25: /* OpTypeImage */
            // FIXME:
            break;
        case 27: /* OpTypeSampledImage */
            out = ops[1];
            typeid = ops[2];
            idd[out].type = CWGL_VAR_SAMPLER_2D; // FIXME: Resolve this
            break;
        case 28: /* OpTypeArray */
            out = ops[1];
            typeid = ops[2];
            idd[out].elms = ops[3];
            idd[out].type = CWGL_VAR_ARRAY;
            break;
        case 30: /* OpTypeStruct */
            // FIXME:
            break;
        case 32: /* OpTypePointer */
            /* Storage Class is checked through SPIR-V validation */
            out = ops[1];
            idd[out].type = TYPE_POINTER;
            typeid = ops[3];
            break;
        case 33: /* OpTypeFunction */
            break;
        case 43: /* OpConstant */
            if(oplen != 4){
                printf("Constant too large\n");
                abort();
            }
            typeid = ops[1];
            out = ops[2];
            idd[out].value->asInt = ops[3];
            idd[out].type = TYPE_CONSTANT_VALUE;
            break;
        case 44: /* OpConstantComposite */
            // FIXME:
            break;
        case 46: /* OpConstantNull */
            typeid = ops[1];
            out = ops[2];
            idd[out].value->asInt = 0;
            idd[out].type = TYPE_CONSTANT_VALUE;
            break;
        case 54: /* OpFunction */
        case 56: /* OpFunctionEnd */
            // FIXME:
            break;
        case 59: /* OpVariable */
            if(ops[3] == 6 /* Private */ ||
               ops[3] == 7 /* Function */){
                printf("Unrecognized variable storage class %d\n", ops[3]);
                abort();
            }
            typeid = ops[1];
            out = ops[2];
            idd[out].type = TYPE_VARIABLE;
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
        idd[out].typeid = typeid;
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


