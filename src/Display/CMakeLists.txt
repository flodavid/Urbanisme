cmake_minimum_required(VERSION 2.6)

find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

# Add the include directories for the Qt 5 Widgets module to the compile lines.
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
#include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

# Use the compile definitions defined in the Qt 5 Widgets module
add_definitions(-Wall ${Qt5Widgets_DEFINITIONS})

include_directories( ${CMAKE_CURRENT_BINARY_DIR})

set(CURRENT_DIR Display)

set(Display_SRCS ${CURRENT_DIR}/fieldview.cpp ${CURRENT_DIR}/loadwindow.cpp)

set(Display_SRCS ${Display_SRCS} PARENT_SCOPE)
