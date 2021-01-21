cmake_minimum_required(VERSION 3.0.0)
project(server_client)

add_compile_options(-std=c++17)

file(GLOB_RECURSE SERVER_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/*.cpp"
)

file(GLOB_RECURSE SERVER_HDRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/server/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/defines.h"
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/server"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_executable(server ${SERVER_SRC} ${SERVER_HDRS})

####################### CLIENT ######################

file(GLOB_RECURSE CLIENT_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/client/*.cpp"
)

file(GLOB_RECURSE CLIENT_HDRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/client/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/defines.h"
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/client"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_executable(client ${CLIENT_SRC} ${CLIENT_HDRS})
