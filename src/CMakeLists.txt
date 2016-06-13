project(sim-city)

cmake_minimum_required(VERSION 2.8.11)

### Definitions ###
# Use the compile definitions defined in the Qt 5 Widgets module
add_definitions(-Wall -std=c++11 -O2 -pthread -lpthread ${Qt5Widgets_DEFINITIONS})

### Qt ###
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the Qt libraries
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

#include_directories(${QT_INCLUDES})

### FILES ###
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(Engine) # must set ${Engine_SRCS}
add_subdirectory(Display) # must set ${Display_SRCS}

set(sim-city_SRCS ${Engine_SRCS} ${Display_SRCS} resolution.cpp localsearch.cpp evaluation.cpp fieldevaluation.cpp gnuplot-cpp/gnuplot_i.hpp Sim-City.cpp)

# Pour que les headers soient présents dans Qt Creator
set(HEADERS debug.h)

# Tests
if (UNIX)
# Si on veut pouvoir tester seulement "Engine"
    #add_subdirectory(Engine/Tests)
    add_subdirectory(Tests)
endif (UNIX)

### Compilation ###
add_executable(sim-city stdafx.cpp ${sim-city_SRCS} ${HEADERS})

# Use the Widgets module from Qt 5.
target_link_libraries(sim-city Qt5::Widgets Qt5::Core Qt5::Gui)
#target_link_libraries(sim-city ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES})

install(TARGETS sim-city RUNTIME DESTINATION bin)

##Version statique
#set(mylib ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES})

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
