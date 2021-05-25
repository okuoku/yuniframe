#include "shxm.h"
#include "shxm_private.h"

#include <stdlib.h>
#include <stdio.h>

#include "spirv_cross_c.h"

void
shxm_private_decomp_spirv(uint32_t* spirv, int len){
    spvc_context ctx = NULL;
    spvc_compiler compiler = NULL;
    spvc_compiler_options options = NULL;
    spvc_parsed_ir ir = NULL;
    const char* result = NULL;

    spvc_context_create(&ctx);
    spvc_context_parse_spirv(ctx, spirv, len, &ir);
    spvc_context_create_compiler(ctx, SPVC_BACKEND_GLSL, ir, 
                                 SPVC_CAPTURE_MODE_TAKE_OWNERSHIP,
                                 &compiler);
    spvc_compiler_create_compiler_options(compiler, &options);
    spvc_compiler_options_set_uint(options, SPVC_COMPILER_OPTION_GLSL_VERSION, 
                                   330);
    spvc_compiler_options_set_bool(options, SPVC_COMPILER_OPTION_GLSL_ES, 
                                   SPVC_FALSE);
    spvc_compiler_install_compiler_options(compiler, options);
    spvc_compiler_compile(compiler, &result);
    printf("%s\n", result);
    spvc_context_destroy(ctx);
}
