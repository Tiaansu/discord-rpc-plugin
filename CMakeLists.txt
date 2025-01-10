cmake_minimum_required(VERSION 3.19)
project(discord-rpc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

message(STATUS ${BUILD_VERSION})

if (DEFINED BUILD_VERSION)
    message(STATUS "Building for version ${BUILD_VERSION}")
    if (BUILD_VERSION STREQUAL "R1")
        add_compile_definitions("SAMP_R1")
    elseif (BUILD_VERSION STREQUAL "R3")
        add_compile_definitions("SAMP_R3")
    elseif (BUILD_VERSION STREQUAL "R5")
        add_compile_definitions("SAMP_R5")
    elseif (BUILD_VERSION STREQUAL "DL")
        add_compile_definitions("SAMP_DL")
    else ()
        message(WARNING "Unknown version ${BUILD_VERSION}, setting SAMP_R1 instead")
    endif ()
else ()
    message(STATUS "No build version set, setting SAMP_R1")
    add_compile_definitions("SAMP_R1")
endif ()

set(POLYHOOK_BUILD_STATIC_RUNTIME OFF CACHE BOOL "")

add_subdirectory(deps/sampapi)
add_subdirectory(deps/minhook)
add_subdirectory(deps/rakhook)

add_subdirectory(src)