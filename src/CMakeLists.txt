file(GLOB_RECURSE sources
    *.cpp
    *.hpp
    *.c
    *.h
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/deps/rakhook/include
    ${CMAKE_SOURCE_DIR}/deps/minhook/include
    ${CMAKE_SOURCE_DIR}/deps/discord/include
)

add_library(${PROJECT_NAME} SHARED ${sources})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/deps/sampapi/include
    ${CMAKE_SOURCE_DIR}/deps/rakhook/include
    ${CMAKE_SOURCE_DIR}/deps/minhook/include
    ${CMAKE_SOURCE_DIR}/deps/discord/include
)

set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".asi")

find_library(DISCORD_RPC_LIB NAMES discord-rpc.lib PATHS ${CMAKE_SOURCE_DIR}/deps/discord/lib)

if (NOT DISCORD_RPC_LIB)
    message(FATAL_ERROR "discord-rpc.lib was not found.")
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE minhook rakhook ${DISCORD_RPC_LIB})