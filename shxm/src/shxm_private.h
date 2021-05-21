#ifndef __YUNI_SHXM_PRIVATE_H
#define __YUNI_SHXM_PRIVATE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
// }

struct shxm_ctx_s {
    int dummy;
};

#define SHXM_MAX_PHASE 2
struct shxm_slot_s {
    cwgl_var_type_t type;
    const char* name;
    int id[SHXM_MAX_PHASE];
    // FIXME: Signedness for WebGL2
    int array_length; /* zero for non arrays */
};

typedef struct shxm_slot_s shxm_slot_t;

struct shxm_uniform_s {
    shxm_slot_t* slot;
    unsigned int offset;
    unsigned int size;
};

typedef struct shxm_uniform_s shxm_uniform_t;

#define SHXM_LOCATION_BUILTIN -1
struct shxm_attribute_s {
    shxm_slot_t* slot;
    int location;
};

typedef struct shxm_attribute_s shxm_attribute_t;

struct shxm_shader_s {
    int refcnt;
    shxm_shader_stage_t type;
    char* source; /* GLSL Source */
    unsigned int source_len;
    uint32_t* ir; /* SPIR-V IR (Unpatched) */
    unsigned int ir_len;
};
typedef struct shxm_shader_s shxm_shader_t;

#define SHXM_MAX_SLOTS 512
#define SHXM_MAX_INPUTS 256
#define SHXM_MAX_OUTPUTS 256
#define SHXM_MAX_VARYINGS 256
#define SHXM_MAX_UNIFORMS 256
#define SHXM_MAX_UNUSED 256
struct shxm_program_s {
    /* Pre-link fields */
    shxm_shader_t* vertex_shader;
    shxm_shader_t* fragment_shader;

    /* Post-link fields */
    shxm_slot_t slot[SHXM_MAX_SLOTS];
    unsigned int slot_count;
    shxm_uniform_t uniform[SHXM_MAX_UNIFORMS];
    unsigned int uniform_count;
    shxm_attribute_t input[SHXM_MAX_INPUTS];
    unsigned int input_count;
    shxm_attribute_t output[SHXM_MAX_OUTPUTS];
    unsigned int output_count;
    shxm_attribute_t varying[SHXM_MAX_VARYINGS];
    unsigned int varying_count;
    shxm_attribute_t unused[SHXM_MAX_UNUSED];
    unsigned int unused_count;

    uint32_t* vertex_ir;
    unsigned int vertex_ir_len;
    uint32_t* fragment_ir;
    unsigned int fragment_ir_len;
};

enum shxm_varusage_e {
    UNKNOWN,
    INPUT,
    OUTPUT,
    UNIFORM,
    UNIFORM_CONSTANT
};

typedef enum shxm_varusage_e shxm_varusage_t;

struct shxm_spirv_ent_s {
    int offs; /* SPIR-V Location */
    int op;
    int name; /* OpName location */
    int chain; /* Decoration chain start location */
    int width;
    int is_signed;
    int array_length; /* zero for non arrays */
    shxm_varusage_t varusage;
    cwgl_var_type_t type;
};

typedef struct shxm_spirv_ent_s shxm_spirv_ent_t;

struct shxm_spirv_intr_s {
    unsigned int* chain;
    shxm_spirv_ent_t* ent;
    unsigned int ent_count;
    unsigned int chain_count;
};

typedef struct shxm_spirv_intr_s shxm_spirv_intr_t;

shxm_spirv_intr_t* shxm_private_read_spirv(uint32_t* ir, int len);

void shxm_private_discomp_spirv(uint32_t* spirv, int len);

// {
#ifdef __cplusplus
};
#endif

#endif

