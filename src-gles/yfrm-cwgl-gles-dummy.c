#include <stdlib.h>
#include <stdint.h>

#ifndef CWGL_EXPERIMENTAL_TRACKER
extern void glGenVertexArraysOES(size_t n, unsigned int *arrays);
extern void glBindVertexArrayOES(unsigned int array);

void (*ptr_glGenVertexArrays)(size_t n, unsigned int *arrays);
void (*ptr_glBindVertexArray)(unsigned int array);
 
void
cwgl_ctx_glGenVertexArray(uintptr_t* id){
    unsigned int i;
    glGenVertexArraysOES(1, &i);
    *id = i;
}
 
void
cwgl_ctx_glBindVertexArray(uintptr_t id){
    glBindVertexArrayOES(id);
}
#endif
