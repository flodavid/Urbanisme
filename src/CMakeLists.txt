cmake_minimum_required(VERSION 3.1.0)

project(sim-city)

### Definitions ###
# Use the compile definitions defined in the Qt 5 Widgets module
add_definitions(-Wall ${Qt5Widgets_DEFINITIONS} -fopenmp)

### Qt ###
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the Qt libraries
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

# Find and set the Open MP Librairie
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

### Sources ###
add_subdirectory(Engine) # must set ${Engine_SRCS}
add_subdirectory(Display) # must set ${Display_SRCS}

set(sim-city_SRCS ${Engine_SRCS} ${Display_SRCS} resolution.cpp localsearch.cpp evaluation.cpp fieldevaluation.cpp gnuplot-cpp/gnuplot_i.hpp Sim-City.cpp)

# Pour que les headers soient présents dans Qt Creator
set(HEADERS debug.h)

## Tests ##
if (UNIX)
# Si on veut pouvoir tester seulement "Engine"
    #add_subdirectory(Engine/Tests)
    add_subdirectory(Tests)
endif (UNIX)

### Compilation ###

## Flags ##
# Profilage
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_RELEASE} -fprofile-arcs -ftest-coverage")
# Add compiler flags for building executables (-fPIE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
# Enable C++11
set(CMAKE_CXX_STANDARD 11)

add_executable(sim-city WIN32 stdafx.cpp ${sim-city_SRCS} ${HEADERS})

# Use the Widgets module from Qt 5.
target_link_libraries(sim-city Qt5::Widgets Qt5::Core Qt5::Gui) # Warning dans cmake 3.X, "double-colons"
#target_link_libraries(sim-city ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES})

install(TARGETS sim-city RUNTIME DESTINATION bin)

##Version statique
#set(mylib ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Gui_LIBRARIES})

# UNUSED

#include_directories(${QT_INCLUDES})

## Static ?
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
