# g++ -o test_engine.exe test_coordinates.cpp ../coordinates.cpp -I/usr/local/include -L/usr/local/lib -lcppunit

project(tests_engine)
cmake_minimum_required(VERSION 2.6)

add_definitions(-Wall -std=c++11)

# Tests
# Tests
add_definitions(-lcppunit)
include_directories(/usr/local/include/)
#add_definitions(-I/usr/local/include)
link_directories(/usr/local/lib/)
#add_definitions(-L/usr/local/lib)
# add_library( -L/usr/local/lib)

set(OTHERS_SOURCES  ../coordinates.cpp ../Field.cpp ../Field_others.cpp)

# Executable
add_executable(tests_engine ${OTHERS_SOURCES} test_next_coordinates.cpp test_coordinates.cpp fieldtestfixture.cpp all_tests.cpp)

install(TARGETS tests_engine RUNTIME DESTINATION bin)

# Tests
target_link_libraries(tests_engine cppunit)
