#include "cwgl-gles2-priv.h"

// 2.5 GL Errors
CWGL_API cwgl_enum_t 
cwgl_getError(cwgl_ctx_t* ctx){
    cwgl_enum_t r;
    CTX_ENTER(ctx);
    r = (cwgl_enum_t)glGetError();
    CTX_LEAVE(ctx);
    return r;
}

// 2.7 Current Vertex State
CWGL_API void 
cwgl_vertexAttrib1f(cwgl_ctx_t* ctx, uint32_t indx, float x){
    CTX_ENTER(ctx);
    glVertexAttrib1f(indx, x);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_vertexAttrib2f(cwgl_ctx_t* ctx, uint32_t indx, float x, float y){
    CTX_ENTER(ctx);
    glVertexAttrib2f(indx, x, y);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_vertexAttrib3f(cwgl_ctx_t* ctx, uint32_t indx, float x, float y, float z){
    CTX_ENTER(ctx);
    glVertexAttrib3f(indx, x, y, z);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_vertexAttrib4f(cwgl_ctx_t* ctx, uint32_t indx, 
                    float x, float y, float z, float w){
    CTX_ENTER(ctx);
    glVertexAttrib4f(indx, x, y, z, w);
    CTX_LEAVE(ctx);
}

// 2.8 Vertex Arrays
CWGL_API void 
cwgl_vertexAttribPointer(cwgl_ctx_t* ctx, uint32_t indx, 
                         int32_t size, cwgl_enum_t type, int normalized, 
                         uint32_t stride, uint32_t offset){
    CTX_ENTER(ctx);
    // FIXME: Check we're binding some array buffer now
    //        https://www.khronos.org/registry/webgl/specs/latest/1.0/#6.2
    glVertexAttribPointer(indx, size, type, normalized,
                          stride, (void*)(uintptr_t)offset);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_enableVertexAttribArray(cwgl_ctx_t* ctx, uint32_t index){
    CTX_ENTER(ctx);
    glEnableVertexAttribArray(index);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_disableVertexAttribArray(cwgl_ctx_t* ctx, uint32_t index){
    CTX_ENTER(ctx);
    glDisableVertexAttribArray(index);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_drawArrays(cwgl_ctx_t* ctx, cwgl_enum_t mode, 
                int32_t first, uint32_t count){
    CTX_ENTER(ctx);
    glDrawArrays(mode, first, count);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_drawElements(cwgl_ctx_t* ctx, cwgl_enum_t mode, 
                  uint32_t count, cwgl_enum_t type, uint32_t offset){
    CTX_ENTER(ctx);
    // FIXME: Check we're binding some array buffer now
    //        https://www.khronos.org/registry/webgl/specs/latest/1.0/#6.2
    glDrawElements(mode, count, type, (void*)(uintptr_t)offset);
    CTX_LEAVE(ctx);
}


// 2.9 Buffer Objects
CWGL_API void 
cwgl_bindBuffer(cwgl_ctx_t* ctx, cwgl_enum_t target, cwgl_Buffer_t* buffer){
    GLuint name;
    CTX_ENTER(ctx);
    if(buffer){
        name = CTX_GETNAME(ctx, buffer);
    }else{
        name = 0;
    }
    glBindBuffer(target, name);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_deleteBuffer(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer){
    GLuint name;
    CTX_ENTER(ctx);
    if(buffer){
        name = CTX_GETNAME(ctx, buffer);
        glDeleteBuffers(1, &name);
    }
    CTX_LEAVE(ctx);
}

CWGL_API cwgl_Buffer_t* 
cwgl_createBuffer(cwgl_ctx_t* ctx){
    GLuint name = 0;
    cwgl_Buffer_t* b;
    CTX_ENTER(ctx);
    glGenBuffers(1, &name);
    b = CTX_ALLOC(ctx, Buffer);
    CTX_SETNAME(ctx, b, name);
    CTX_LEAVE(ctx);
    return b;
}

CWGL_API void
cwgl_Buffer_release(cwgl_ctx_t* ctx, cwgl_Buffer_t* buffer){
    //CTX_ENTER(ctx);
    if(buffer){
        CTX_FREE(ctx, Buffer, buffer);
    }
    //CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_bufferData(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                uint32_t size, void* data, cwgl_enum_t usage){
    CTX_ENTER(ctx);
    glBufferData(target, size, data, usage);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_bufferSubData(cwgl_ctx_t* ctx, cwgl_enum_t target, 
                   uint32_t offset, void* data, size_t buflen){
    CTX_ENTER(ctx);
    glBufferSubData(target, offset, buflen, data);
    CTX_LEAVE(ctx);
}

// 2.10.1 Loading and Creating Shader Source
CWGL_API cwgl_Shader_t* 
cwgl_createShader(cwgl_ctx_t* ctx, cwgl_enum_t type){
    GLuint name;
    cwgl_Shader_t* r;
    CTX_ENTER(ctx);
    name = glCreateShader(type);
    if(name){
        r = CTX_ALLOC(ctx, Shader);
        CTX_SETNAME(ctx, r, name);
    }else{
        r = NULL;
    }
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API void
cwgl_Shader_release(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    //CTX_ENTER(ctx);
    if(shader){
        CTX_FREE(ctx, Shader, shader);
    }
    //CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_shaderSource(cwgl_ctx_t* ctx, cwgl_Shader_t* shader, const char* source,
                  size_t sourcelen){
    GLuint name;
    const GLint len = sourcelen;
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, shader);
    glShaderSource(name, 1, &source, &len);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_compileShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    GLuint name;
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, shader);
    glCompileShader(name);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_deleteShader(cwgl_ctx_t* ctx, cwgl_Shader_t* shader){
    GLuint name;
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, shader);
    glDeleteShader(name);
    CTX_LEAVE(ctx);
}

// 2.10.3 Program Objects
CWGL_API cwgl_Program_t* 
cwgl_createProgram(cwgl_ctx_t* ctx){
    GLuint name;
    cwgl_Program_t* r;
    CTX_ENTER(ctx);
    name = glCreateProgram();
    if(name){
        r = CTX_ALLOC(ctx, Program);
        CTX_SETNAME(ctx, r, name);
    }else{
        r = NULL;
    }
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API void
cwgl_Program_release(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    //CTX_ENTER(ctx);
    if(program){
        CTX_FREE(ctx, Program, program);
    }
    //CTX_LEAVE(ctx);
}


CWGL_API void 
cwgl_attachShader(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                  cwgl_Shader_t* shader){
    GLuint namep;
    GLuint names;
    CTX_ENTER(ctx);
    namep = CTX_GETNAME(ctx, program);
    names = CTX_GETNAME(ctx, shader);
    glAttachShader(namep,names);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_detachShader(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                  cwgl_Shader_t* shader){
    GLuint namep;
    GLuint names;
    CTX_ENTER(ctx);
    namep = CTX_GETNAME(ctx, program);
    names = CTX_GETNAME(ctx, shader);
    glDetachShader(namep,names);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_linkProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    GLuint name;
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, program);
    glLinkProgram(name);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_useProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    GLuint name;
    CTX_ENTER(ctx);
    if(program){
        name = CTX_GETNAME(ctx, program);
        glUseProgram(name);
    }else{
        glUseProgram(0);
    }
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_deleteProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    GLuint name;
    CTX_ENTER(ctx);
    name = CTX_GETNAME(ctx, program);
    glDeleteProgram(name);
    CTX_LEAVE(ctx);
}

// 2.10.4 Shader Variables
CWGL_API cwgl_query_result_t 
cwgl_getActiveAttrib(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                     int32_t index, int32_t* out_size, 
                     int32_t* type, cwgl_string_t** name){
    // FIXME: Implement this
    return CWGL_QR_UNIMPLEMENTED;
}
CWGL_API int32_t 
cwgl_getAttribLocation(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                       const char* name){
    GLuint namep;
    int32_t r;
    CTX_ENTER(ctx);
    namep = CTX_GETNAME(ctx, program);
    r = glGetAttribLocation(namep, name);
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API void 
cwgl_bindAttribLocation(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                        uint32_t index, const char* name){
    GLuint namep;
    CTX_ENTER(ctx);
    namep = CTX_GETNAME(ctx, program);
    glBindAttribLocation(namep, index, name);
    CTX_LEAVE(ctx);
}

CWGL_API cwgl_UniformLocation_t* 
cwgl_getUniformLocation(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                        const char* name){
    GLint nameu;
    GLuint namep;
    cwgl_UniformLocation_t* r;
    CTX_ENTER(ctx);
    namep = CTX_GETNAME(ctx, program);
    nameu = glGetUniformLocation(namep, name);
    if(nameu == -1){
        r = NULL;
    }else{
        r = CTX_ALLOC(ctx, UniformLocation);
        CTX_SETNAME(ctx, r, nameu);
    }
    CTX_LEAVE(ctx);
    return r;
}

CWGL_API void
cwgl_UniformLocation_release(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* u){
    //CTX_ENTER(ctx);
    if(u){
        CTX_FREE(ctx, UniformLocation, u);
    }
    //CTX_LEAVE(ctx);
}

CWGL_API cwgl_query_result_t 
cwgl_getActiveUniform(cwgl_ctx_t* ctx, cwgl_Program_t* program, 
                      int32_t index, int32_t* out_size, int32_t* out_type, 
                      cwgl_string_t** name){
    cwgl_string_t* r;
    char namebuf[320]; // In WebGL, it should be <256
    GLuint namep;
    GLint size = 0;
    GLenum type = 0;
    GLsizei namelength = 0;
    CTX_ENTER(ctx);
    namep = CTX_GETNAME(ctx, program);
    glGetActiveUniform(namep, index, sizeof(namebuf), &namelength,
                       &size, &type, namebuf);
    r = cwgl_priv_alloc_string(ctx, namebuf, namelength + 1);
    *out_size = size;
    *out_type = type;
    *name = r;
    CTX_LEAVE(ctx);
    return CWGL_QR_SUCCESS;
}

CWGL_API void 
cwgl_uniform1f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, float x){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform1f(loc, x);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform1i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, int32_t x){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform1i(loc, x);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform2f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               float x, float y){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform2f(loc, x, y);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform2i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               int32_t x, int32_t y){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform2i(loc, x, y);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform3f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               float x, float y, float z){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform3f(loc, x, y, z);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform3i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               int32_t x, int32_t y, int32_t z){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform3i(loc, x, y, z);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform4f(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               float x, float y, float z, float w){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform4f(loc, x, y, z, w);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform4i(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
               int32_t x, int32_t y, int32_t z, int32_t w){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform4i(loc, x, y, z, w);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform1fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                float* values, size_t count){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform1fv(loc, count, values);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform1iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                int32_t* values, size_t count){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform1iv(loc, count, values);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform2fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                float* values, size_t count){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform2fv(loc, count, values);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform2iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                int32_t* values, size_t count){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform2iv(loc, count, values);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform3fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                float* values, size_t count){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform3fv(loc, count, values);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform3iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                int32_t* values, size_t count){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform3iv(loc, count, values);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform4fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                float* values, size_t count){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform4fv(loc, count, values);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniform4iv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                int32_t* values, size_t count){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniform4iv(loc, count, values);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniformMatrix2fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                      int transpose, float* value, size_t count){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniformMatrix2fv(loc, count, transpose, value);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniformMatrix3fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                      int transpose, float* value, size_t count){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniformMatrix3fv(loc, count, transpose, value);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_uniformMatrix4fv(cwgl_ctx_t* ctx, cwgl_UniformLocation_t* location, 
                      int transpose, float* value, size_t count){
    GLint loc;
    CTX_ENTER(ctx);
    loc = CTX_GETNAME(ctx, location);
    glUniformMatrix4fv(loc, count, transpose, value);
    CTX_LEAVE(ctx);
}


// 2.10.5 Shader Execution
CWGL_API void 
cwgl_validateProgram(cwgl_ctx_t* ctx, cwgl_Program_t* program){
    GLuint namep;
    CTX_ENTER(ctx);
    namep = CTX_GETNAME(ctx, program);
    glValidateProgram(namep);
    CTX_LEAVE(ctx);
}

// 2.12.1 Controlling the Viewport
CWGL_API void 
cwgl_depthRange(cwgl_ctx_t* ctx, float zNear, float zFar){
    CTX_ENTER(ctx);
    glDepthRangef(zNear, zFar);
    CTX_LEAVE(ctx);
}

CWGL_API void 
cwgl_viewport(cwgl_ctx_t* ctx, 
              int32_t x, int32_t y, int32_t width, int32_t height){
    CTX_ENTER(ctx);
    glViewport(x, y, width, height);
    CTX_LEAVE(ctx);
}

