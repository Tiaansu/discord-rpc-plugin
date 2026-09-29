cmake_minimum_required(VERSION 3.19)
project(discord-rpc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

message(STATUS ${BUILD_VERSION})

set(POLYHOOK_BUILD_STATIC_RUNTIME OFF CACHE BOOL "")

add_subdirectory(deps/minhook)
add_subdirectory(deps/rakhook)

add_subdirectory(src)