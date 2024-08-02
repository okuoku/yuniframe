# As we don't rely CMake's dependency tracking for library targets,
# write dependencies here for now.

#
# INPUTs:
#   

if(YFRM_MOLTENVK_PREFIX) # Consume paramter
endif()

set(backendlibs)
if(YFRM_CWGL_USE_DX11)
    list(APPEND backendlibs
        # angle (Yuniframe)
        dxgi
        dxguid
        # yuniframe
        d3d11
        )
endif()
if(YFRM_CWGL_USE_VULKAN OR YFRM_CWGL_USE_METAL)
    list(APPEND backendlibs
        SPIRV-Tools-static
        )
endif()
if(YFRM_CWGL_USE_ANGLE)
    list(APPEND backendlibs
        angle_static)
    if(WIN32)
        list(APPEND backendlibs
            synchronization)
    endif()
endif()
if(YFRM_CWGL_EXPERIMENTAL_TRACKER)
    # FIXME: Move this to shxm dependency
    list(APPEND backendlibs
        glslang
        my-glslang-default-resource-limits
        SPIRV
        SPIRV-Tools-opt
        spirv-cross-c
        )
endif()

if(YFRM_CWGL_USE_PLATFORMGLES)
    if(APPLE)
        list(APPEND backendlibs
            "-framework OpenGLES")
    else()
        # FIXME: Use IMPORT library instead...
        if(WIN32)
            set(libprefix lib)
        else()
            set(libprefix)
        endif()
        list(APPEND backendlibs
            ${libprefix}EGL
            ${libprefix}GLESv2)
    endif()
endif()

if(APPLE)
    if(YFRM_CWGL_USE_VULKAN)
        list(APPEND backendlibs
            ${YFRM_MOLTENVK_PREFIX}/MoltenVK/dynamic/MoltenVK.xcframework
            )
    endif()
    # FIXME: Why do we need it actually?
    list(APPEND backendlibs
        "-framework IOSurface"
        )
endif()

