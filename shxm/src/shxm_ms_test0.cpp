#include "shxm_private.h"


#include "spirv-tools/optimizer.hpp"

#include <cstring>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

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

extern "C" void
shxm_ms_test0(int phase, uint32_t* ir, int len){
    uint32_t* xir;
    int xir_len;

    xir = NULL;
    preprocess(ir, len, &xir, &xir_len);
    if(xir){
        shxm_private_spirv_dis(reinterpret_cast<int*>(xir), xir_len);
        free(xir);
    }
}
