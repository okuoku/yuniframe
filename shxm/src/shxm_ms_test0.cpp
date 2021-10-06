#include "shxm_private.h"


#include "spirv-tools/optimizer.hpp"

#include <cstring>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "spirv_cross_c.h"

static void
dummy_consumer(spv_message_level_t level, const char*,
               const spv_position_t& position, const char* message) {
    switch (level) {
        case SPV_MSG_FATAL:
        case SPV_MSG_INTERNAL_ERROR:
        case SPV_MSG_ERROR:
            printf("ERROR: %d: %s\n", position.index, message);
            break;
        case SPV_MSG_WARNING:
            printf("WARN: %d: %s\n", position.index, message);
            break;
        case SPV_MSG_INFO:
            printf("INFO: %d: %s\n", position.index, message);
            break;
        default:
            break;
    }
}

static int
preprocess(uint32_t* ir, int len,
           uint32_t** out_ir, int* out_len){
    std::vector<uint32_t> out;
    spvtools::ValidatorOptions vopts;
    spvtools::Optimizer optimizer(SPV_ENV_VULKAN_1_1);
    optimizer.SetMessageConsumer(dummy_consumer);

    optimizer.RegisterSizePasses(); // InlineExhaustive + LoopUnroll
    if(optimizer.Run(ir, len, &out, vopts, false)){
        size_t siz;
        uint32_t* r;
        siz = out.size();
        r = (uint32_t*)malloc(sizeof(uint32_t)*siz);
        if(r){
            memcpy(r, out.data(), sizeof(uint32_t)*siz);
            *out_ir = r;
            *out_len = siz;
            return 0;
        }else{
            return 1;
        }
    }else{
        return 1;
    }
}

extern "C" int shxm_ms_asm0(uint32_t * ops, int len);

extern "C" void
shxm_ms_test0(int phase, uint32_t* ir, int len){
    spvc_context scctx = NULL;
    spvc_parsed_ir scir = NULL;
    spvc_compiler sccomp = NULL;
    spvc_compiler_options scopts = NULL;
    const char* src = NULL;
    spvc_context_create(&scctx);

    spvc_context_parse_spirv(scctx, ir, len, &scir);
    spvc_context_create_compiler(scctx, SPVC_BACKEND_HLSL,
                                 scir, SPVC_CAPTURE_MODE_TAKE_OWNERSHIP,
                                 &sccomp);

    spvc_compiler_create_compiler_options(sccomp, &scopts);
    spvc_compiler_options_set_uint(scopts,
                                   SPVC_COMPILER_OPTION_HLSL_SHADER_MODEL,
                                   50);
    spvc_compiler_install_compiler_options(sccomp, scopts);
    spvc_compiler_compile(sccomp, &src);
    printf("HLSL:\n%s\n", src);
    spvc_context_destroy(scctx);



#if 0
    uint32_t* xir;
    int xir_len;

    xir = NULL;
    preprocess(ir, len, &xir, &xir_len);
    if(xir){
        if(shxm_ms_asm0(xir, xir_len)){
            shxm_private_spirv_dis(reinterpret_cast<int*>(xir), xir_len);
        }
        free(xir);
    }
#endif
}
