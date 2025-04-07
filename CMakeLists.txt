
cmake_minimum_required(VERSION 3.13)
project(ada-test)

add_subdirectory(adal-prot)

set(SOURCE_FILES src/main.cpp)

add_executable(ada-test ${SOURCE_FILES})

target_include_directories(ada-test
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(ada-test PRIVATE adal-prot)

