project(sim-city)
cmake_minimum_required(VERSION 2.6)

# Compilers
#set (CMAKE_C_COMPILER D:/Apps/Qt/Tools/mingw492_32/bin/gcc.exe)
#set (CMAKE_CXX_COMPILER D:/Apps/Qt/Tools/mingw492_32/bin/g++.exe)

# FILES
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(Engine) # must set ${Engine_SRCS}
# add_subdirectory(Display) # must set ${Display_SRCS}

add_definitions(-Wall -std=c++11)

set(sim-city_SRCS ${Engine_SRCS} ${Display_SRCS} Sim-City.cpp)

set(HEADERS debug.h)

add_executable(sim-city stdafx.cpp ${Engine_SRCS} Resolution.cpp Sim-City.cpp ${HEADERS})

install(TARGETS sim-city RUNTIME DESTINATION bin)


add_subdirectory(Tests)


# UNUSED

#   if(BUILD_STATIC_EXECUTABLES)
#     set(CMAKE_EXE_LINKER_FLAGS -static)
#     set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
#     set(CMAKE_EXE_LINK_DYNAMIC_C_FLAGS)       # remove -Wl,-Bdynamic
#     set(CMAKE_EXE_LINK_DYNAMIC_CXX_FLAGS)
#     set(CMAKE_SHARED_LIBRARY_C_FLAGS)         # remove -fPIC
#     set(CMAKE_SHARED_LIBRARY_CXX_FLAGS)
#     set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS)    # remove -rdynamic
#     set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
#     # Maybe this works as well, haven't tried yet.
#     # set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)
#   else(BUILD_STATIC_EXECUTABLES)
#     # Set RPATH to use for installed targets; append linker search path
#     set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LOFAR_LIBDIR}")
#     set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#   endif(BUILD_STATIC_EXECUTABLES)
