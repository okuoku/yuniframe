if(NOT NCCC_ROOT)
    # FIXME: Migrate to CMake module later.
    message(FATAL_ERROR "Set NCCC_ROOT for nccc directory")
endif()
if(NOT CWGL_ROOT)
    # FIXME: Make it optional
    message(FATAL_ERROR "CWGL Required.")
endif()

cmake_minimum_required(VERSION 3.12)
project(yuniframe-nccc C)

include(${NCCC_ROOT}/stubsupport/cmake/NcccStubMacros.cmake)

include(${CMAKE_CURRENT_LIST_DIR}/stub_yfrm.cmake)
include(${CWGL_ROOT}/nccc/stub_cwgl.cmake)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${NCCC_ROOT}/stubsupport)

if(YFRM_BUILD_MODULE)
    set(libtype MODULE)
else()
    set(libtype STATIC)
endif()

add_library(nccc_yfrm ${libtype}
    nccc_cwgl.c
    nccc_yfrm.c)

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

target_link_libraries(nccc_yfrm
    PRIVATE
    ${backendlibs}
    cwgl yfrm)

