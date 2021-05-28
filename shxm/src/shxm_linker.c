#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "shxm.h"
#include "shxm_private.h"

static int /* zero on success */
resolve_type(shxm_spirv_ent_t* ent, int32_t* ir, int id){
    int r;
    int op;
    int len;
    int offs;
    int tgt;
    int arrlen;
    int dim;
    if(ent[id].type == CWGL_VAR_UNKNOWN){
        op = ent[id].op;
        offs = ent[id].offs;
        switch(op){
            case 19: /* OpTypeVoid */
                ent[id].type = CWGL_VAR_DUMMY;
                break;
            case 20: /* OpTypeBool */
                ent[id].type = CWGL_VAR_BOOL;
                ent[id].array_length = 0;
                ent[id].width = 0;
                ent[id].is_signed = 0;
                break;
            case 21: /* OpTypeInt */
                ent[id].type = CWGL_VAR_INT;
                ent[id].array_length = 0;
                ent[id].width = ir[offs+2];
                ent[id].is_signed = ir[offs+3];
                break;
            case 22: /* OpTypeFloat */
                ent[id].type = CWGL_VAR_FLOAT;
                ent[id].array_length = 0;
                ent[id].width = ir[offs+2];
                ent[id].is_signed = 0;
                break;
            case 23: /* OpTypeVector */
                tgt = ir[offs+2];
                len = ir[offs+3];
                ent[id].array_length = 0;
                ent[id].width = 0;
                ent[id].is_signed = 0;
                r = resolve_type(ent, ir, tgt);
                if(r){
                    return r;
                }
                switch(ent[tgt].type){
                    // FIXME: How to handle BOOL??
                    case CWGL_VAR_FLOAT:
                        switch(len){
                            case 2:
                                ent[id].type = CWGL_VAR_FLOAT_VEC2;
                                break;
                            case 3:
                                ent[id].type = CWGL_VAR_FLOAT_VEC3;
                                break;
                            case 4:
                                ent[id].type = CWGL_VAR_FLOAT_VEC4;
                                break;
                            default:
                                printf("ERROR: Invalid vector width(%d)",
                                       len);
                                return 1;
                        }
                        break;
                    case CWGL_VAR_INT:
                        switch(len){
                            case 2:
                                ent[id].type = CWGL_VAR_INT_VEC2;
                                break;
                            case 3:
                                ent[id].type = CWGL_VAR_INT_VEC3;
                                break;
                            case 4:
                                ent[id].type = CWGL_VAR_INT_VEC4;
                                break;
                            default:
                                printf("ERROR: Invalid vector width(%d)",
                                       len);
                                return 1;
                        }
                        break;
                    default:
                        printf("ERROR: Invalid vector CWGL type(%d)\n", 
                               (int)ent[tgt].type);
                        return 1;
                }
                break;
            case 24: /* OpTypeMatrix */
                tgt = ir[offs+2];
                len = ir[offs+3];
                ent[id].array_length = 0;
                ent[id].width = 0;
                ent[id].is_signed = 0;
                r = resolve_type(ent, ir, tgt);
                if(r){
                    return r;
                }
                switch(len){
                    case 2:
                        if(ent[tgt].type != CWGL_VAR_FLOAT_VEC2){
                            printf("ERROR: Invalid matrix col type(%d)\n",
                                   (int)ent[tgt].type);
                        }
                        ent[id].type = CWGL_VAR_FLOAT_MAT2;
                        break;
                    case 3:
                        if(ent[tgt].type != CWGL_VAR_FLOAT_VEC3){
                            printf("ERROR: Invalid matrix col type(%d)\n",
                                   (int)ent[tgt].type);
                        }
                        ent[id].type = CWGL_VAR_FLOAT_MAT3;
                        break;
                    case 4:
                        if(ent[tgt].type != CWGL_VAR_FLOAT_VEC4){
                            printf("ERROR: Invalid matrix col type(%d)\n",
                                   (int)ent[tgt].type);
                        }
                        ent[id].type = CWGL_VAR_FLOAT_MAT4;
                        break;
                    default:
                        printf("ERROR: Invalid matrix col count(%d)\n",
                               len);
                        return 1;
                }
                break;
            case 25: /* OpTypeImage */
                dim = ir[offs+3];
                ent[id].array_length = 0;
                ent[id].width = 0;
                ent[id].is_signed = 0;
                switch(dim){
                    case 1: /* 2D */
                        ent[id].type = CWGL_VAR_SAMPLER_2D;
                        break;
                    case 3: /* Cube */
                        ent[id].type = CWGL_VAR_SAMPLER_CUBE;
                        break;
                    case 0: /* 1D */
                    case 2: /* 3D */
                    case 4: /* Rect */
                    case 5: /* Buffer */
                    case 6: /* SubpassData */
                    default:
                        printf("ERROR: Unrecognised image dimension(%d)\n",
                               dim);
                        return 1;
                }
                break;
            case 26: /* OpTypeSampler */
                printf("ERROR: OpTypeSampler(FIXME)\n");
                return 1;
            case 27: /* OpTypeSampledImage */
                tgt = ir[offs+2];
                ent[id].array_length = 0;
                ent[id].width = 0;
                ent[id].is_signed = 0;
                r = resolve_type(ent, ir, tgt);
                if(r){
                    return r;
                }
                ent[id].type = ent[tgt].type;
                break;
            case 28: /* OpTypeArray */
                /* 2: element type */
                tgt = ir[offs+2];
                r = resolve_type(ent, ir, tgt);
                if(r){
                    return r;
                }
                ent[id].width = ent[tgt].width;
                ent[id].is_signed = ent[tgt].is_signed;
                ent[id].type = ent[tgt].type;
                ent[id].element_type_id = tgt;

                /* 3: length */
                arrlen = ir[offs+3];
                if(ent[arrlen].op != 43 /* OpConstant */){
                    printf("ERROR: Array length is not constant(%d)\n",
                           ent[arrlen].op);
                    return 1;
                }
                tgt = ir[ent[arrlen].offs+1];
                r = resolve_type(ent, ir, tgt);
                if(r){
                    return r;
                }
                if(ent[tgt].type != CWGL_VAR_INT){
                    printf("ERROR: Integer scalar type required for array length(%d)\n", (int)ent[tgt].type);
                    return 1;
                }
                printf("FIXME: Array Index width = %d\n", ent[tgt].width);
                ent[id].array_length = *(uint32_t*)(&ir[ent[arrlen].offs+3]);
                printf("FIXME: Array Length = %d\n", ent[id].array_length);
                break;
            case 32: /* OpTypePointer */
                tgt = ir[offs+3];
                r = resolve_type(ent, ir, tgt);
                if(r){
                    return r;
                }
                ent[id].array_length = ent[tgt].array_length;
                ent[id].width = ent[tgt].width;
                ent[id].is_signed = ent[tgt].is_signed;
                ent[id].type = ent[tgt].type;
                break;
            case 33: /* OpTypeFunction */
                printf("ERROR: Tried to resolve function type op(%d)\n", op);
                return 1;
            default:
                printf("ERROR: Tried to resolve unknown type op(%d)\n", op);
                return 1;
        }
    }
    return 0;
}

static int
fill_slots(shxm_program_t* prog, shxm_spirv_intr_t* intr, int phase){
    /* Look for OpVariable Input/Output/Uniform/UniformConstant */
    int id;
    int failed = 0;
    shxm_varusage_t varusage;
    char* varname;
    int varclass;
    int varwidth;
    int r;
    int v;
    int typeid;
    shxm_slot_t* varslot;
    cwgl_var_type_t vartype;
    int32_t* ir = (phase == 0) ? prog->vertex_shader->ir : 
        prog->fragment_shader->ir;
    if(phase == 0){
        /* Reset current slot data */
        prog->slot_count = 0;
        prog->uniform_count = 0;
        prog->input_count = 0;
        prog->output_count = 0;
        prog->varying_count = 0;
        prog->unused_count = 0;
        prog->opaque_count = 0;
    }
    for(id=0;id!=intr->ent_count;id++){
        if(intr->ent[id].op == 59 /* OpVariable */){
            if(intr->ent[id].name){
                varname = (char*)&ir[intr->ent[id].name+2];
            }else{
                /* Unnamed variable can be safely ignored */
                continue;
            }
            varusage = UNKNOWN;
            varclass = ir[intr->ent[id].offs+3];
            switch(varclass){
                case 0: /* UniformConstant */
                    varusage = UNIFORM_CONSTANT;
                    break;
                case 1: /* Input */
                    varusage = INPUT;
                    break;
                case 2: /* Uniform */
                    printf("ERROR: Uniform??\n");
                    varusage = UNIFORM;
                    break;
                case 3: /* Output */
                    varusage = OUTPUT;
                    break;
                default:
                    /* Unknown, ignore */
                    continue;
            }

            if(phase == 0){
                varslot = &prog->slot[prog->slot_count];
                memset(varslot, 0, sizeof(shxm_slot_t));
                prog->slot_count++;
            }else{
                varslot = NULL;
                /* First, search existing variable with matching name */
                for(v=0;v!=prog->slot_count;v++){
                    if(!strncmp(varname, prog->slot[v].name, 255)){
                        varslot = &prog->slot[v];
                        break;
                    }
                }
                if(! varslot){
                    varslot = &prog->slot[prog->slot_count];
                    memset(varslot, 0, sizeof(shxm_slot_t));
                    prog->slot_count++;
                }
            }
            if(varslot){
                typeid = ir[intr->ent[id].offs+1];
                varslot->id[phase] = id;
                intr->ent[id].varusage = varusage;
                varslot->name = varname;
                r = resolve_type(intr->ent, ir, typeid);
                if(r){
                    return r;
                }
                varslot->array_length = intr->ent[typeid].array_length;
                varslot->type = intr->ent[typeid].type;
                if(varslot->array_length){
                    printf("var:%d:%s:%d (type %d[%d])\n",phase, varname, (int)varusage,
                           varslot->type,varslot->array_length);
                }else{
                    printf("var:%d:%s:%d (type %d)\n",phase, varname, (int)varusage,
                           varslot->type);
                }
            }else{
                printf("(ignored) var:%d:%s:%d\n",phase, varname, (int)varusage);
            }
        }
    }
    return failed;
}

static int
is_builtin(shxm_slot_t* slot){
    // FIXME: Tentative. Use var decoration instead
#define CHECK(nam) \
    if(slot->name && (! strncmp(slot->name, nam, sizeof(nam)+1))) return 1
    CHECK("gl_Position");
    CHECK("gl_PointSize");
    CHECK("gl_VertexID");
    CHECK("gl_InstanceID");
    CHECK("gl_FragCood");
    CHECK("gl_PointCood");
    CHECK("gl_FrontFacing");
    CHECK("gl_FragDepth");
    // FIXME: Assign legal location to fragment outputs
    //CHECK("gl_FragColor");
    //CHECK("gl_FragData");
    return 0;
#undef CHECK
}

static int
is_opaque_type(cwgl_var_type_t type){
    switch(type){
        case CWGL_VAR_SAMPLER_2D:
        case CWGL_VAR_SAMPLER_CUBE:
            return 1;
        default:
            break;
    }
    return 0;
}

static void
add_input(shxm_program_t* prog, shxm_slot_t* slot){
    prog->input[prog->input_count].slot = slot;
    prog->input_count++;
}
static void
add_output(shxm_program_t* prog, shxm_slot_t* slot){
    prog->output[prog->output_count].slot = slot;
    prog->output_count++;
}
static void
add_varying(shxm_program_t* prog, shxm_slot_t* slot){
    prog->varying[prog->varying_count].slot = slot;
    prog->varying_count++;
}
static void
add_uniform(shxm_program_t* prog, shxm_slot_t* slot){
    prog->uniform[prog->uniform_count].slot = slot;
    prog->uniform_count++;
}
static void
add_unused(shxm_program_t* prog, shxm_slot_t* slot){
    prog->unused[prog->unused_count].slot = slot;
    prog->unused_count++;
}
static void
add_opaque(shxm_program_t* prog, shxm_slot_t* slot, int phase){
    prog->opaque[prog->opaque_count].slot = slot;
    prog->opaque[prog->opaque_count].phase = phase;
    prog->opaque_count++;
}

static int
linkup_slots(shxm_program_t* prog, shxm_spirv_intr_t* vintr,
             shxm_spirv_intr_t* fintr){
    int s;
    int vid, fid;
    shxm_slot_t* slot;
    for(s=0;s!=prog->slot_count;s++){
        slot = &prog->slot[s];
        vid = slot->id[0];
        fid = slot->id[1];
        /* Generate field table */
        if(vid && fid){
            /* On both fragment shader and vertex shader */
            switch(vintr->ent[vid].varusage){
                case OUTPUT: /* varying */
                    if(fintr->ent[fid].varusage != INPUT){
                        printf("ERROR: fs %s expected INPUT(%d)\n",
                               slot->name, fintr->ent[fid].varusage);
                        return 1;
                    }
                    // FIXME: Check type here.
                    add_varying(prog, slot);
                    break;
                case UNIFORM_CONSTANT: /* uniform */
                    if(fintr->ent[fid].varusage != UNIFORM_CONSTANT){
                        printf("ERROR: fs %s expected UNIFORM_CONSTANT(%d)\n",
                               slot->name, fintr->ent[fid].varusage);
                        return 1;
                    }
                    if(is_opaque_type(slot->type)){
                        add_opaque(prog, slot, 0);
                        add_opaque(prog, slot, 1);
                    }else{
                        add_uniform(prog, slot);
                    }
                    break;
                default:
                case INPUT:
                case UNKNOWN:
                case UNIFORM:
                    printf("ERROR: vs %s Invalid varusage(%d)\n",
                           slot->name, vintr->ent[vid].varusage);
                    return 1;
            }
        }else if(vid){
            /* Vertex shader only */
            switch(vintr->ent[vid].varusage){
                case OUTPUT:
                    if(is_builtin(slot)){
                        add_varying(prog, slot);
                    }else{
                        add_unused(prog, slot);
                    }
                    break;
                case UNIFORM_CONSTANT:
                    if(is_opaque_type(slot->type)){
                        add_opaque(prog, slot, 0);
                    }else{
                        add_uniform(prog, slot);
                    }
                    break;
                case INPUT:
                    add_input(prog, slot);
                    break;
                default:
                case UNKNOWN:
                case UNIFORM:
                    printf("ERROR: vs %s Invalid varusage(%d)\n",
                           slot->name, vintr->ent[vid].varusage);
                    return 1;
            }
        }else if(fid){
            /* Fragment shader only */
            switch(fintr->ent[fid].varusage){
                case INPUT:
                    add_varying(prog,slot);
                    break;
                case OUTPUT:
                    add_output(prog,slot);
                    break;
                case UNIFORM_CONSTANT:
                    if(is_opaque_type(slot->type)){
                        add_opaque(prog, slot, 1);
                    }else{
                        add_uniform(prog, slot);
                    }
                    break;
                default:
                case UNKNOWN:
                case UNIFORM:
                    printf("ERROR: fs %s Invalid varusage(%d)\n",
                           slot->name, vintr->ent[vid].varusage);
                    return 1;
            }
        }else{
            printf("ERROR: Unknown field usage[%s]\n",
                   slot->name);
            return 1;
        }
    }
    return 0;
}

static int /* count */
calc_location_size(shxm_slot_t* slot){
    cwgl_var_type_t type;
    int locsize;
    switch(slot->type){
        case CWGL_VAR_FLOAT_MAT2:
            locsize = 2;
            break;
        case CWGL_VAR_FLOAT_MAT3:
            locsize = 3;
            break;
        case CWGL_VAR_FLOAT_MAT4:
            locsize = 4;
            break;
        default:
            locsize = 1;
            break;
    }
    if(slot->array_length){
        return locsize * slot->array_length;
    }else{
        return locsize;
    }
}

static int
assign_locations(shxm_program_t* prog){
    int i;
    int curloc;
    shxm_slot_t* slot;
    shxm_attribute_t* attr;

    curloc = 0;
    for(i=0;i!=prog->input_count;i++){
        attr = &prog->input[i];
        slot = attr->slot;
        if(is_builtin(slot)){
            attr->location = SHXM_LOCATION_BUILTIN;
        }else{
            attr->location = curloc;
            curloc += calc_location_size(slot);
        }
    }

    curloc = 0;
    for(i=0;i!=prog->varying_count;i++){
        attr = &prog->varying[i];
        slot = attr->slot;
        if(is_builtin(slot)){
            attr->location = SHXM_LOCATION_BUILTIN;
        }else{
            attr->location = curloc;
            curloc += calc_location_size(slot);
        }
    }

    curloc = 0;
    for(i=0;i!=prog->output_count;i++){
        attr = &prog->output[i];
        slot = attr->slot;
        if(is_builtin(slot)){
            attr->location = SHXM_LOCATION_BUILTIN;
        }else{
            attr->location = curloc;
            curloc += calc_location_size(slot);
        }
    }
    return 0;
}

static int
calc_slot_size(shxm_slot_t* slot){
    int locsize;
    switch(slot->type){
        case CWGL_VAR_FLOAT_VEC2:
        case CWGL_VAR_INT_VEC2:
        case CWGL_VAR_BOOL_VEC2:
            locsize = 2;
            break;
        case CWGL_VAR_FLOAT_VEC3:
        case CWGL_VAR_INT_VEC3:
        case CWGL_VAR_BOOL_VEC3:
            locsize = 3;
            break;
        case CWGL_VAR_FLOAT_VEC4:
        case CWGL_VAR_INT_VEC4:
        case CWGL_VAR_BOOL_VEC4:
            locsize = 4;
            break;
        case CWGL_VAR_FLOAT_MAT2:
            locsize = 2*2;
            break;
        case CWGL_VAR_FLOAT_MAT3:
            locsize = 3*3;
            break;
        case CWGL_VAR_FLOAT_MAT4:
            locsize = 4*4;
            break;
        case CWGL_VAR_FLOAT:
        case CWGL_VAR_INT:
        case CWGL_VAR_BOOL:
        default:
            locsize = 1;
            break;
    }
    if(slot->array_length){
        if(locsize == 3){
            // Do not pack 3 elements field
            return 4 * slot->array_length * 4;
        }else{
            return locsize * slot->array_length * 4;
        }
    }else{
        return locsize * 4;
    }
}

static int
calc_slot_alignment(shxm_slot_t* slot){
    int locsize;
    if(slot->array_length){
        /* Arrays will have 4 component alignment in std140
         * ref: GLES3 2.12.6.4 Standard Uniform Block Layout */
        locsize = 4;
    }else{
        switch(slot->type){
            case CWGL_VAR_FLOAT_VEC2:
            case CWGL_VAR_INT_VEC2:
            case CWGL_VAR_BOOL_VEC2:
                locsize = 2;
                break;
            case CWGL_VAR_FLOAT_VEC3:
            case CWGL_VAR_INT_VEC3:
            case CWGL_VAR_BOOL_VEC3:
            case CWGL_VAR_FLOAT_VEC4:
            case CWGL_VAR_INT_VEC4:
            case CWGL_VAR_BOOL_VEC4:
                /* VEC3 will have 4 component alignment in std140
                 * ref: GLES3 2.12.6.4 Standard Uniform Block Layout */
                locsize = 4;
                break;
            case CWGL_VAR_FLOAT_MAT2:
            case CWGL_VAR_FLOAT_MAT4:
            case CWGL_VAR_FLOAT_MAT3:
                /* Matrix have 4 component alignment in std140
                 * ref: GLES3 2.12.6.4 Standard Uniform Block Layout */
                locsize = 4;
                break;
            case CWGL_VAR_FLOAT:
            case CWGL_VAR_INT:
            case CWGL_VAR_BOOL:
            default:
                locsize = 1;
                break;
        }
    }
    /* Convert it into bytes */
    return locsize * 4;
}

static int
layout_uniforms(shxm_program_t* prog){
    int i;
    int curoff;
    int alignment;
    int size;
    int d,nd;
    shxm_slot_t* slot;
    curoff = 0;
    for(i=0;i!=prog->uniform_count;i++){
        slot = prog->uniform[i].slot;
        alignment = calc_slot_alignment(slot);
        size = calc_slot_size(slot);
        d = curoff / alignment;
        nd = alignment * d;
        if(nd != curoff){
            curoff = alignment * (d+1);
        }
        prog->uniform[i].offset = curoff;
        prog->uniform[i].size = size;
        curoff += size;
    }
    return 0;
}

static int
bind_uniforms(shxm_program_t* prog){
    int i;
    int idx[2];
    int phase;
    idx[0] = idx[1] = 1;
    for(i=0;i!=prog->opaque_count;i++){
        phase = prog->opaque[i].phase;
        switch(phase){
            case 0:
            case 1:
                prog->opaque[i].binding = idx[phase];
                idx[phase]++;
                break;
            default:
                printf("ERROR: Invalid phase %d\n",phase);
                return 1;
        }
    }
    return 0;
}

SHXM_API int
shxm_program_link(shxm_ctx_t* ctx, shxm_program_t* prog){
    int i;
    shxm_spirv_intr_t* vintr;
    shxm_spirv_intr_t* fintr;

    if(!prog->vertex_shader){
        printf("ERROR: No vertex shader.\n");
        return 1;
    }
    if(!prog->fragment_shader){
        printf("ERROR: No fragment shader.\n");
        return 1;
    }
    if(!prog->vertex_shader->ir){
        printf("ERROR: Vertex shader not compiled.\n");
        return 1;
    }
    if(!prog->fragment_shader->ir){
        printf("ERROR: Fragment shader not compiled.\n");
        return 1;
    }

    vintr = shxm_private_read_spirv(prog->vertex_shader->ir,
                                    prog->vertex_shader->ir_len);
    fintr = shxm_private_read_spirv(prog->fragment_shader->ir,
                                    prog->fragment_shader->ir_len);

    if(fill_slots(prog, vintr, 0)){
        printf("ERROR: Failed to fill vertex shader variable slots.\n");
        // FIXME: Release vintr, fintr
        return 1;
    }
    if(fill_slots(prog, fintr, 1)){
        printf("ERROR: Failed to fill fragment shader variable slots.\n");
        // FIXME: Release vintr, fintr
        return 1;
    }
    if(linkup_slots(prog, vintr, fintr)){
        // FIXME: Release vintr, fintr
        return 1;
    }

    if(assign_locations(prog)){
        // FIXME: Release vintr, fintr
        return 1;
    }

    if(layout_uniforms(prog)){
        // FIXME: Release vintr, fintr
        return 1;
    }

    if(bind_uniforms(prog)){
        // FIXME: Release vintr, fintr
        return 1;
    }

    printf("== PostLink ==\n");
    for(i=0;i!=prog->uniform_count;i++){
        printf("Uniform:%s:%d:%d (off: %d, size: %d)\n",
               prog->uniform[i].slot->name,
               prog->uniform[i].slot->id[0],
               prog->uniform[i].slot->id[1],
               prog->uniform[i].offset, prog->uniform[i].size);
    }
    for(i=0;i!=prog->opaque_count;i++){
        printf("Opaque_:%s:%d:%d (phase %d, bind %d)\n",
               prog->opaque[i].slot->name,
               prog->opaque[i].slot->id[0],
               prog->opaque[i].slot->id[1], prog->opaque[i].phase,
               prog->opaque[i].binding);
    }
    for(i=0;i!=prog->input_count;i++){
        printf("Input__:%s:%d:%d (loc %d)\n",prog->input[i].slot->name,
               prog->input[i].slot->id[0],
               prog->input[i].slot->id[1], prog->input[i].location);
    }
    for(i=0;i!=prog->varying_count;i++){
        printf("Varying:%s:%d:%d (loc %d)\n",prog->varying[i].slot->name,
               prog->varying[i].slot->id[0],
               prog->varying[i].slot->id[1], prog->varying[i].location);
    }
    for(i=0;i!=prog->output_count;i++){
        printf("Output_:%s:%d:%d (loc %d)\n",prog->output[i].slot->name,
               prog->output[i].slot->id[0],
               prog->output[i].slot->id[1], prog->output[i].location);
    }
    for(i=0;i!=prog->unused_count;i++){
        printf("Unused_:%s:%d:%d\n",prog->unused[i].slot->name,
               prog->unused[i].slot->id[0],
               prog->unused[i].slot->id[1]);
    }

    if(shxm_private_patch_spirv(ctx, prog, vintr, 0)){
        // FIXME: Release vintr, fintr
        return 1;
    }
    shxm_private_spirv_dis(prog->vertex_ir, prog->vertex_ir_len);
    shxm_private_decomp_spirv(prog->vertex_ir, prog->vertex_ir_len);

    if(shxm_private_patch_spirv(ctx, prog, fintr, 1)){
        // FIXME: Release vintr, fintr
        return 1;
    }
    shxm_private_spirv_dis(prog->fragment_ir, prog->fragment_ir_len);
    shxm_private_decomp_spirv(prog->fragment_ir, prog->fragment_ir_len);

    uint32_t* opt_ir;
    int opt_ir_len;
    printf("== Optimized(VTX) ==\n");
    if(shxm_private_spvopt_run(prog->vertex_ir, prog->vertex_ir_len,
                               &opt_ir, &opt_ir_len)){
        printf("Fail.\n");
    }else{
        shxm_private_spirv_dis(opt_ir, opt_ir_len);
        shxm_private_decomp_spirv(opt_ir, opt_ir_len);
    }
    printf("== Optimized(FLG) ==\n");
    if(shxm_private_spvopt_run(prog->fragment_ir, prog->fragment_ir_len,
                               &opt_ir, &opt_ir_len)){
        printf("Fail.\n");
    }else{
        shxm_private_spirv_dis(opt_ir, opt_ir_len);
        shxm_private_decomp_spirv(opt_ir, opt_ir_len);
    }

    // FIXME: Implement this.
    return 1;
}

