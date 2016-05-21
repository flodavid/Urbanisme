cmake_minimum_required(VERSION 2.6)

add_definitions(-Wall)

set(CURRENT_DIR Engine)

set(Engine_SRCS ${CURRENT_DIR}/Coordinates.cpp ${CURRENT_DIR}/field.cpp ${CURRENT_DIR}/field_others.cpp  ${CURRENT_DIR}/parameters.cpp ${CURRENT_DIR}/loadprogress.cpp )

set(Engine_SRCS ${Engine_SRCS} PARENT_SCOPE)
