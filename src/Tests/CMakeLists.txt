project(tests_operations)
cmake_minimum_required(VERSION 2.6)

add_definitions(-Wall -std=c++11)

# Tests
add_definitions(-lcppunit)
include_directories(/usr/local/include/) 
#add_definitions(-I/usr/local/include)
link_directories(/usr/local/lib/)
#add_definitions(-L/usr/local/lib)
# add_library( -L/usr/local/lib)

set(OTHERS_SOURCES  ../Engine/coordinates.cpp ../Engine/Field.cpp ../Engine/Parameters.cpp ../Resolution.cpp)
set(MIN_PATH_SOURCES minpathroadtestfixture.cpp)

# Executable
add_executable(tests_operations ${OTHERS_SOURCES} ${MIN_PATH_SOURCES} all_tests.cpp)

install(TARGETS tests_operations RUNTIME DESTINATION bin)

# Tests
target_link_libraries(tests_operations cppunit)
