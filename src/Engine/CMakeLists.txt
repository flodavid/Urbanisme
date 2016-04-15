cmake_minimum_required(VERSION 2.6)

add_definitions(-Wall)

include_directories( ${CMAKE_CURRENT_BINARY_DIR})

set(CURRENT_DIR Engine)

set(Engine_SRCS ${CURRENT_DIR}/coordinates.cpp ${CURRENT_DIR}/Field.cpp ${CURRENT_DIR}/Parameters.cpp)

set(Engine_SRCS ${Engine_SRCS} PARENT_SCOPE)

if (UNIX)
    add_subdirectory(Tests)
endif (UNIX)
