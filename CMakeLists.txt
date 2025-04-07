
cmake_minimum_required(VERSION 3.13)
project(hsp-test)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(hsp)

set(SOURCE_FILES src/main.cpp)

add_executable(hsp-test ${SOURCE_FILES})

target_include_directories(hsp-test
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(hsp-test PRIVATE hsp)

