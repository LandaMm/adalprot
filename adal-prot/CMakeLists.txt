
cmake_minimum_required(VERSION 3.13)

set(ADAL_PROT_VERSION 0.0.1)
set(ADAL_PROT_DESCRIPTION "adal protocol description")
project(adal-prot VERSION ${ADAL_PROT_VERSION} DESCRIPTION ${ADAL_PROT_DESCRIPTION})
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES src/main.cpp)

add_library(adal-prot SHARED ${SOURCE_FILES})
target_include_directories(adal-prot
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/api
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
set_target_properties(adal-prot PROPERTIES
    VERSION ${ADAL_PROT_VERSION}
    SOVERSION 1
)

