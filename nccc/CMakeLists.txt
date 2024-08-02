if(NOT NCCC_ROOT)
    # FIXME: Migrate to CMake module later.
    message(FATAL_ERROR "Set NCCC_ROOT for nccc directory")
endif()
if(NOT CWGL_ROOT)
    # FIXME: Make it optional
    message(FATAL_ERROR "CWGL Required.")
endif()

# => backendlibs
include(${CMAKE_CURRENT_LIST_DIR}/../calclinkdeps.cmake)

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

