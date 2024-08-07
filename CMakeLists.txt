cmake_minimum_required(VERSION 3.12)
project(yfrm C CXX) # FIXME: Enable CXX only if we need C++fs

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

if(NOT YFRM_CWGL_USE_PLATFORMGLES)
    add_definitions(
        # Use static-link
        -DANGLE_EXPORT=
        -DKHRONOS_STATIC
        #-DEGLAPI=
        #-DGL_API=
        #-DGL_APIENTRY=
        #-DGL_APICALL=
        )
endif()

if(YFRM_BUILD_MODULE)
    add_definitions(
        -DCWGL_DLL
        -DCWGL_SHARED_BUILD
        -DYFRM_DLL
        -DYFRM_SHARED_BUILD
        )
endif()

set(yfrmsrc
    platform/src-sdl2/yfrm-audio-sdl2.c
    platform/src-sdl2/yfrm-ctx-sdl2.c
    platform/src-sdl2/yfrm-events-sdl2.c
    )

set(yfrmmainsrc
    platform/src-sdl2/yfrm-main-sdl2.c
    )

if(YFRM_FS_USE_SDL)
    list(APPEND yfrmsrc
        platform/src-sdl2/yfrm-fs-sdl2.c
        )
elseif(YFRM_FS_USE_CXX17)
    list(APPEND yfrmsrc
        platform/src-cxx17/yfrm-fs-cxx17.cpp
        )
endif()

if(YFRM_CWGL_USE_VULKAN)
    add_definitions(-DYFRM_CWGL_USE_VULKAN)
elseif(WIN32 OR WINDOWS_STORE)
    add_definitions(-DYFRM_CWGL_USE_DX11)
    list(APPEND yfrmsrc
        platform/src-win/yfrm-gpu-d3d11.c
        )
endif()


if(YFRM_CWGL_USE_ANGLE)
    add_definitions(-DYFRM_CWGL_USE_ANGLE)
    list(APPEND yfrmsrc
        platform/src-angle/yfrm-cwgl-angle.c
        )
endif()

if(YFRM_CWGL_EXPERIMENTAL_TRACKER)
    add_definitions(-DCWGL_EXPERIMENTAL_TRACKER)
endif()

if(WINDOWS_STORE)
    add_definitions(-DYFRM_USE_UWP)
elseif(WIN32)
    list(APPEND yfrmsrc
        platform/src-win/yfrm-execpath-win32.c
        )
endif()

add_library(yfrm OBJECT 
    ${yfrmsrc})

add_library(yfrm_main OBJECT
    ${yfrmmainsrc})

if(YFRM_CWGL_PREFIX)
    # FIXME: Refactor yfrm not to require specify cwgl location
    target_include_directories(yfrm PRIVATE ${YFRM_CWGL_PREFIX}/include)
    if(YFRM_CWGL_EXPERIMENTAL_TRACKER)
        target_include_directories(yfrm PRIVATE ${YFRM_CWGL_PREFIX}/src-tracker)
    endif()
endif()
if(CWGL_KHRONOS_HEADER_DIR)
    # FIXME: ditto
    target_include_directories(yfrm PRIVATE ${CWGL_KHRONOS_HEADER_DIR})
endif()

set(sdl2libprefix) # FIXME: Use "SDL2::" for Platform SDL2
target_link_libraries(yfrm PUBLIC ${sdl2libprefix}SDL2-static)
if("${CMAKE_SYSTEM_NAME}" STREQUAL WindowsStore)
    target_link_libraries(yfrm_main PUBLIC ${sdl2libprefix}SDL2-static)
else()
    target_link_libraries(yfrm_main PUBLIC ${sdl2libprefix}SDL2-static 
        ${sdl2libprefix}SDL2main)
endif()

if(YFRM_SDL2_PREFIX)
    # Unfortunately, we have to generate SDL2 prefix in some way
    # Because the SDL2 repository uses `include/SDL.h` but standard
    # include procedure is `include/SDL2/SDL.h`. 
    target_include_directories(yfrm PRIVATE ${YFRM_SDL2_PREFIX})
    target_include_directories(yfrm_main PRIVATE ${YFRM_SDL2_PREFIX})
endif()
if(TARGET sdl2headers)
    # `sdl2headers` target will be declared in proj/sdl2
    add_dependencies(yfrm sdl2headers)
    add_dependencies(yfrm_main sdl2headers)
endif()


target_include_directories(yfrm PUBLIC include)
target_include_directories(yfrm_main PUBLIC include)

# NCCC Module (with CWGL)
add_subdirectory(nccc)
