cmake_minimum_required(VERSION 3.0.0)
project(server_client)

add_compile_options(-std=c++17)

file(GLOB_RECURSE SERVER_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/socket/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/main.cpp"
)

file(GLOB_RECURSE SERVER_HDRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/socket/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Defines.hpp"
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/socket"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_executable(server ${SERVER_SRC} ${SERVER_HDRS})

####################### CLIENT ######################

file(GLOB_RECURSE CLIENT_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/src/socket/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/client/main.cpp"
)

file(GLOB_RECURSE CLIENT_HDRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/socket/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/Defines.hpp"
)

include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/socket"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_executable(client ${CLIENT_SRC} ${CLIENT_HDRS})
