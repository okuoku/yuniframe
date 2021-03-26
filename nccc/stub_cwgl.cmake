nccc_stub_begin(cwgl)

# Heap Objects
nccc_api(cwgl_string_size
    IN ptr ptr
    # size_t
    OUT u64)
nccc_api(cwgl_string_read
    IN ptr ptr ptr u64
    OUT u64)
nccc_api(cwgl_string_release
    IN ptr ptr
    OUT)

nccc_api(cwgl_Buffer_release
    IN ptr ptr
    OUT)
nccc_api(cwgl_Shader_release
    IN ptr ptr
    OUT)
nccc_api(cwgl_Program_release
    IN ptr ptr
    OUT)
nccc_api(cwgl_Texture_release
    IN ptr ptr
    OUT)
nccc_api(cwgl_Framebuffer_release
    IN ptr ptr
    OUT)
nccc_api(cwgl_Renderbuffer_release
    IN ptr ptr
    OUT)
nccc_api(cwgl_UniformLocation_release
    IN ptr ptr
    OUT)

# Context, Platform (none)
# OpenGL State
nccc_api(cwgl_disable
    IN ptr u32
    OUT)

nccc_api(cwgl_enable
    IN ptr u32
    OUT)

# 2.5 GL Errors
nccc_api(cwgl_getError
    IN ptr
    OUT u32)

# 2.7 Current Vertex State
nccc_api(cwgl_vertexAttrib1f
    IN ptr u32 f32
    OUT)

nccc_api(cwgl_vertexAttrib2f
    IN ptr u32 f32 f32
    OUT)

nccc_api(cwgl_vertexAttrib3f
    IN ptr u32 f32 f32 f32
    OUT)

nccc_api(cwgl_vertexAttrib4f
    IN ptr u32 f32 f32 f32 f32
    OUT)

# 2.8 Vertex Arrays
nccc_api(cwgl_vertexAttribPointer
    IN ptr u32 u32 u32 u32 u32 u32
    OUT)

nccc_api(cwgl_enableVertexAttribArray
    IN ptr u32
    OUT)

nccc_api(cwgl_disableVertexAttribArray
    IN ptr u32
    OUT)

nccc_api(cwgl_drawArrays
    IN ptr u32 u32 u32
    OUT)

nccc_api(cwgl_drawElements
    IN ptr u32 u32 u32 u32
    OUT)

# 2.9 Buffer Objects
nccc_api(cwgl_bindBuffer
    IN ptr u32 ptr
    OUT)

nccc_api(cwgl_deleteBuffer
    IN ptr ptr
    OUT)

nccc_api(cwgl_createBuffer
    IN ptr
    OUT ptr)

nccc_api(cwgl_bufferData
    IN ptr u32 u32 ptr u32
    OUT)

nccc_api(cwgl_bufferSubData
    IN ptr u32 u32 ptr u32
    OUT)

# 2.10.1 Loading and Creating Shader Source
nccc_api(cwgl_createShader
    IN ptr u32
    OUT ptr)
nccc_api(cwgl_shaderSource
    IN ptr ptr ptr u32
    OUT)
nccc_api(cwgl_compileShader
    IN ptr ptr
    OUT)
nccc_api(cwgl_deleteShader
    IN ptr ptr
    OUT)

# 2.10.3 Program Objects
nccc_api(cwgl_createProgram
    IN ptr
    OUT ptr)

nccc_api(cwgl_attachShader
    IN ptr ptr ptr
    OUT)

nccc_api(cwgl_detachShader
    IN ptr ptr ptr
    OUT)

nccc_api(cwgl_linkProgram
    IN ptr ptr
    OUT)

nccc_api(cwgl_useProgram
    IN ptr ptr
    OUT)

nccc_api(cwgl_deleteProgram
    IN ptr ptr
    OUT)

# 2.10.4 Shader Variables
nccc_api(cwgl_getAttribLocation
    IN ptr ptr ptr
    OUT u32)

nccc_api(cwgl_bindAttribLocation
    IN ptr ptr u32 ptr
    OUT)

nccc_api(cwgl_getUniformLocation
    IN ptr ptr ptr
    OUT ptr)

nccc_api(cwgl_getActiveUniform
    IN ptr ptr u32 ptr ptr ptr
    OUT u32)

nccc_api(cwgl_uniform1f
    IN ptr ptr f32
    OUT)

nccc_api(cwgl_uniform1i
    IN ptr ptr u32
    OUT)

nccc_api(cwgl_uniform2f
    IN ptr ptr f32 f32
    OUT)

nccc_api(cwgl_uniform2i
    IN ptr ptr u32 u32
    OUT)

nccc_api(cwgl_uniform3f
    IN ptr ptr f32 f32 f32
    OUT)

nccc_api(cwgl_uniform3i
    IN ptr ptr u32 u32 u32
    OUT)

nccc_api(cwgl_uniform4f
    IN ptr ptr f32 f32 f32 f32
    OUT)

nccc_api(cwgl_uniform4i
    IN ptr ptr u32 u32 u32 u32
    OUT)

nccc_api(cwgl_uniform1fv
    IN ptr ptr ptr u32
    OUT)
nccc_api(cwgl_uniform1iv
    IN ptr ptr ptr u32
    OUT)
nccc_api(cwgl_uniform2fv
    IN ptr ptr ptr u32
    OUT)
nccc_api(cwgl_uniform2iv
    IN ptr ptr ptr u32
    OUT)
nccc_api(cwgl_uniform3fv
    IN ptr ptr ptr u32
    OUT)
nccc_api(cwgl_uniform3iv
    IN ptr ptr ptr u32
    OUT)
nccc_api(cwgl_uniform4fv
    IN ptr ptr ptr u32
    OUT)
nccc_api(cwgl_uniform4iv
    IN ptr ptr ptr u32
    OUT)
nccc_api(cwgl_uniformMatrix2fv
    IN ptr ptr u32 ptr u32
    OUT)
nccc_api(cwgl_uniformMatrix3fv
    IN ptr ptr u32 ptr u32
    OUT)
nccc_api(cwgl_uniformMatrix4fv
    IN ptr ptr u32 ptr u32
    OUT)

# 2.10.5 Shader Execution
nccc_api(cwgl_validateProgram
    IN ptr ptr
    OUT)

# 2.12.1 Controlling the Viewport
nccc_api(cwgl_depthRange
    IN ptr f32 f32
    OUT)

nccc_api(cwgl_viewport
    IN ptr u32 u32 u32 u32
    OUT)

# 3.4 Line Segments
nccc_api(cwgl_lineWidth
    IN ptr f32
    OUT)

# 3.5 Polygons
nccc_api(cwgl_frontFace
    IN ptr u32 
    OUT)
    
nccc_api(cwgl_cullFace
    IN ptr u32
    OUT)

# 3.5.2 Depth offset
nccc_api(cwgl_polygonOffset
    IN ptr f32 f32
    OUT)

# 3.6.1 Pixel Storage Modes
nccc_api(cwgl_pixelStorei
    IN ptr u32 u32
    OUT)

# 3.7 Texturing
nccc_api(cwgl_activeTexture
    IN ptr u32
    OUT)

# 3.7.1 Texture Image Specification
nccc_api(cwgl_texImage2D
    IN ptr u32 u32 u32 u32 u32 u32 u32 u32 ptr u32
    OUT)

# 3.7.2 Alternate Texture Image Specification Commands
nccc_api(cwgl_copyTexImage2D
    IN ptr u32 u32 u32 u32 u32 u32 u32 u32
    OUT)

nccc_api(cwgl_texSubImage2D
    IN ptr u32 u32 u32 u32 u32 u32 u32 u32 ptr u32
    OUT)
nccc_api(cwgl_copyTexSubImage2D
    IN ptr u32 u32 u32 u32 u32 u32 u32 u32
    OUT)

# 3.7.3 Compressed Texture Images
nccc_api(cwgl_compressedTexImage2D
    IN ptr u32 u32 u32 u32 u32 u32 ptr u32
    OUT)
nccc_api(cwgl_compressedTexSubImage2D
    IN ptr u32 u32 u32 u32 u32 u32 u32 ptr u32
    OUT)

# 3.7.4 Texture Parameters
nccc_api(cwgl_texParameterf
    IN ptr u32 u32 f32
    OUT)
nccc_api(cwgl_texParameteri
    IN ptr u32 u32 u32
    OUT)

# 3.7.11 Mipmap Generation
nccc_api(cwgl_generateMipmap
    IN ptr u32
    OUT)

# 3.7.13 Texture Objects
nccc_api(cwgl_bindTexture
    IN ptr u32 ptr
    OUT)
nccc_api(cwgl_deleteTexture
    IN ptr ptr
    OUT)
nccc_api(cwgl_createTexture
    IN ptr
    OUT ptr)

# 4.1.2 Scissor Test
nccc_api(cwgl_scissor
    IN ptr u32 u32 u32 u32
    OUT)

# 4.1.3 Multisample Fragment Operations
nccc_api(cwgl_sampleCoverage
    IN ptr f32 u32
    OUT)

# 4.1.4 Stencil Test
nccc_api(cwgl_stencilFunc
    IN ptr u32 u32 u32
    OUT)
nccc_api(cwgl_stencilFuncSeparate
    IN ptr u32 u32 u32 u32
    OUT)
nccc_api(cwgl_stencilOp
    IN ptr u32 u32 u32
    OUT)
nccc_api(cwgl_stencilOpSeparate
    IN ptr u32 u32 u32 u32
    OUT)

# 4.1.5 Depth Buffer Test
nccc_api(cwgl_depthFunc
    IN ptr u32
    OUT)

# 4.1.6 Blending
nccc_api(cwgl_blendEquation
    IN ptr u32
    OUT)
nccc_api(cwgl_blendEquationSeparate
    IN ptr u32 u32
    OUT)
nccc_api(cwgl_blendFuncSeparate
    IN ptr u32 u32 u32 u32
    OUT)
nccc_api(cwgl_blendFunc
    IN ptr u32 u32
    OUT)
nccc_api(cwgl_blendColor
    IN ptr f32 f32 f32 f32
    OUT)

# 4.2.2 Fine Control of Buffer Updates
nccc_api(cwgl_colorMask
    IN ptr u32 u32 u32 u32
    OUT)
nccc_api(cwgl_depthMask
    IN ptr u32
    OUT)
nccc_api(cwgl_stencilMask
    IN ptr u32
    OUT)
nccc_api(cwgl_stencilMaskSeparate
    IN ptr u32 u32
    OUT)

# 4.2.3 Clearing the Buffers
nccc_api(cwgl_clear
    IN ptr u32
    OUT)
nccc_api(cwgl_clearColor
    IN ptr f32 f32 f32 f32
    OUT)
nccc_api(cwgl_clearDepth
    IN ptr f32
    OUT)
nccc_api(cwgl_clearStencil
    IN ptr u32
    OUT)

# 4.3.1 Reading Pixels
nccc_api(cwgl_readPixels
    IN ptr u32 u32 u32 u32 u32 u32 ptr u32
    OUT)

# 4.4.1 Binding and Managing Framebuffer Objects
nccc_api(cwgl_bindFramebuffer
    IN ptr u32 ptr
    OUT)
nccc_api(cwgl_deleteFramebuffer
    IN ptr ptr
    OUT)
nccc_api(cwgl_createFramebuffer
    IN ptr
    OUT ptr)

# 4.4.3 Renderbuffer Objects
nccc_api(cwgl_bindRenderbuffer
    IN ptr u32 ptr
    OUT)
nccc_api(cwgl_deleteRenderbuffer
    IN ptr ptr
    OUT)
nccc_api(cwgl_createRenderbuffer
    IN ptr
    OUT ptr)
nccc_api(cwgl_renderbufferStorage
    IN ptr u32 u32 u32 u32
    OUT)
nccc_api(cwgl_framebufferRenderbuffer
    IN ptr u32 u32 u32 ptr
    OUT)
nccc_api(cwgl_framebufferTexture2D
    IN ptr u32 u32 u32 ptr u32
    OUT)

# 4.4.5 Framebuffer Completeness
nccc_api(cwgl_checkFramebufferStatus
    IN ptr u32
    OUT u32)

# 5.1 Flush and Finish
nccc_api(cwgl_finish
    IN ptr
    OUT)
nccc_api(cwgl_flush
    IN ptr
    OUT)

# 5.2 Hints
nccc_api(cwgl_hint
    IN ptr u32 u32
    OUT)

# 6.1.1 Simple Queries
nccc_api(cwgl_getParameter_b1
    IN ptr u32 ptr
    OUT u32)
nccc_api(cwgl_getParameter_b4
    IN ptr u32 ptr ptr ptr ptr
    OUT u32)
nccc_api(cwgl_getParameter_i1
    IN ptr u32 ptr
    OUT u32)
nccc_api(cwgl_getParameter_i2
    IN ptr u32 ptr ptr
    OUT u32)
nccc_api(cwgl_getParameter_i4
    IN ptr u32 ptr ptr ptr ptr
    OUT u32)
nccc_api(cwgl_getParameter_f1
    IN ptr u32 ptr
    OUT u32)
nccc_api(cwgl_getParameter_f2
    IN ptr u32 ptr ptr
    OUT u32)
nccc_api(cwgl_getParameter_f4
    IN ptr u32 ptr ptr ptr ptr
    OUT u32)
nccc_api(cwgl_getParameter_str
    IN ptr u32 ptr
    OUT u32)
nccc_api(cwgl_getParameter_Buffer
    IN ptr u32 ptr
    OUT u32)
nccc_api(cwgl_getParameter_Program
    IN ptr u32 ptr
    OUT u32)
nccc_api(cwgl_getParameter_Framebuffer
    IN ptr u32 ptr
    OUT u32)
nccc_api(cwgl_getParameter_Renderbuffer
    IN ptr u32 ptr
    OUT u32)
nccc_api(cwgl_getParameter_Texture
    IN ptr u32 ptr
    OUT u32)
nccc_api(cwgl_isEnabled
    IN ptr u32
    OUT u32)
# 6.1.3 Enumerated Queries
nccc_api(cwgl_getTexParameter_i1
    IN ptr u32 u32 ptr
    OUT u32)
nccc_api(cwgl_getBufferParameter_i1
    IN ptr u32 u32 ptr
    OUT u32)
nccc_api(cwgl_getFramebufferAttachmentParameter_i1
    IN ptr u32 u32 u32 ptr
    OUT u32)
nccc_api(cwgl_getFramebufferAttachmentParameter_Renderbuffer
    IN ptr u32 u32 u32 ptr
    OUT u32)
nccc_api(cwgl_getFramebufferAttachmentParameter_Texture
    IN ptr u32 u32 u32 ptr
    OUT u32)
nccc_api(cwgl_getRenderbufferParameter_i1
    IN ptr u32 u32 ptr
    OUT u32)

# 6.1.4 Texture Queries
nccc_api(cwgl_isTexture
    IN ptr ptr
    OUT u32)
# 6.1.6 Buffer Object Queries
nccc_api(cwgl_isBuffer
    IN ptr ptr
    OUT u32)
# 6.1.7 Framebuffer Object and Renderbuffer Queries
nccc_api(cwgl_isFramebuffer
    IN ptr ptr
    OUT u32)
nccc_api(cwgl_isRenderbuffer
    IN ptr ptr
    OUT u32)

# 6.1.8 Shader and Program Queries
nccc_api(cwgl_isShader
    IN ptr ptr
    OUT u32)
nccc_api(cwgl_getShaderParameter_i1
    IN ptr ptr u32 ptr
    OUT u32)
nccc_api(cwgl_isProgram
    IN ptr ptr
    OUT u32)
nccc_api(cwgl_getProgramParameter_i1
    IN ptr ptr u32 ptr
    OUT u32)
nccc_api(cwgl_getProgramInfoLog
    IN ptr ptr
    OUT ptr)
nccc_api(cwgl_getShaderInfoLog
    IN ptr ptr
    OUT ptr)
nccc_api(cwgl_getShaderPrecisionFormat
    IN ptr u32 u32 ptr ptr ptr
    OUT u32)

nccc_stub_end(cwgl)
