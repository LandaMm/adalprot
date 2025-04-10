
cmake_minimum_required(VERSION 3.13)

set(HSP_VERSION 0.0.1)
set(HSP_DESCRIPTION "home server protocol description")
project(hsp VERSION ${HSP_VERSION} DESCRIPTION ${HSP_DESCRIPTION})
set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES 
    src/address.cpp
    src/connection.cpp
    src/server.cpp
    src/client.cpp
    src/reader.cpp
    src/packet.cpp
    src/request.cpp
    src/router.cpp
)

add_library(hsp SHARED ${SOURCE_FILES})
target_include_directories(hsp
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/api
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)
set_target_properties(hsp PROPERTIES
    VERSION ${HSP_VERSION}
    SOVERSION 1
)

