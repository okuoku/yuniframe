#include "ShaderLang.h"
#include "SPIRV/GlslangToSpv.h"
#include "SPIRV/disassemble.h"
#include <iostream>

#include <stdlib.h>

namespace glslang {
    extern const TBuiltInResource DefaultTBuiltInResource;
};

extern "C" void
shxm_glslang_init(void){
    (void)glslang::InitializeProcess();
}

extern "C" void
shxm_glslang_deinit(void){
    (void)glslang::FinalizeProcess();
}

extern "C" void
shxm_private_spirv_dis(int* spv, int len){
#if 0
    int i;
    for(i=0;i!=len;i++){
        printf("%04d: %08x\n",i,spv[i]);
    }
#endif
    std::vector<unsigned int> spirv(spv, &spv[len-1]);
    spv::Disassemble(std::cout, spirv);
}

extern "C" int
shxm_glslang_build(int mode, const char* source, int len,
                   int** out_spv, int* out_spvlen){
    int ret;
    glslang::TShader::ForbidIncluder includer;
    glslang::TShader* ts;
    switch(mode){
        case 0:
            ts = new glslang::TShader(EShLangFragment);
            break;
        case 1:
            ts = new glslang::TShader(EShLangVertex);
            break;
        default:
            return 1;
    }

    ts->setStringsWithLengths(&source, &len, 1);
    //ts->setAutoMapBindings(true);
    ts->setAutoMapLocations(true);
    // ts->setInvertY(true);
    ts->setEnvInput(glslang::EShSourceGlsl,
                    (mode == 0)?EShLangFragment : EShLangVertex,
                    glslang::EShClientOpenGL,
                    100);
    /*
    ts->setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
    ts->setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);
    */

    if(ts->parse(&glslang::DefaultTBuiltInResource,
                 100,
                 false,
                 EShMsgDefault,
                 includer)){
        std::vector<unsigned int> spirv;
        spv::SpvBuildLogger logger;
        glslang::SpvOptions opt;

        printf("Success.\n%s\n%s\n",
               ts->getInfoLog(),
               ts->getInfoDebugLog());
        ts->getIntermediate()->setOriginUpperLeft();
        glslang::GlslangToSpv(*ts->getIntermediate(),
                              spirv,
                              &logger,
                              &opt);
        printf("%s\n", logger.getAllMessages().c_str());
        spv::Disassemble(std::cout, spirv);
        // Override version
        spirv[1] = 0x10300; // SPIR-V 1.3
        {
            // Output
            int* out;
            size_t siz;
            ret = 0;
            siz = spirv.size();
            out = (int*)malloc(sizeof(int)*siz);
            if(! out){
                ret = -1;
            }else{
                for(int i=0;i!=siz;i++){
                    out[i] = spirv[i];
                }
                if(out_spv){
                    *out_spv = out;
                }
                if(out_spvlen){
                    *out_spvlen = (int)siz;
                }
            }
        }
#if 0
        if(mode == 0){
            glslang::OutputSpvBin(spirv, "out.frag.spv");
        }else{
            glslang::OutputSpvBin(spirv, "out.vert.spv");
        }
#endif
    }else{
        ret = 1;
        printf("Fail.\n%s\n%s\n",
               ts->getInfoLog(),
               ts->getInfoDebugLog());
    }

    delete ts;
    return ret;
}

