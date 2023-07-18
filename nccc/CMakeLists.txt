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

add_library(nccc_yfrm MODULE
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
        SPIRV-Tools
        )
endif()

target_link_libraries(nccc_yfrm
    PRIVATE
    ${backendlibs}
    cwgl yfrm)

