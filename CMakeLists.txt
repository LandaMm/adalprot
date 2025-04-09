
cmake_minimum_required(VERSION 3.13)
project(hsp-server)
project(hsp-client)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(hsp)

add_executable(hsp-server main.cpp)
add_executable(hsp-client client.cpp)

target_include_directories(hsp-server
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(hsp-server PRIVATE hsp)

target_include_directories(hsp-client
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(hsp-client PRIVATE hsp)

