project(tests_operations)
cmake_minimum_required(VERSION 2.6)

add_definitions(-Wall -std=c++11)

# Tests
add_definitions(-lcppunit)
include_directories(/usr/local/include/)
link_directories(/usr/local/lib/)

# Executable
add_executable(tests_operations ../Engine/coordinates.cpp ../Engine/Field.cpp ../Engine/Parameters.cpp ../Resolution.cpp test_next_coordinates.cpp all_tests.cpp)

install(TARGETS tests_operations RUNTIME DESTINATION bin)

# Tests
target_link_libraries(tests_operations cppunit)
