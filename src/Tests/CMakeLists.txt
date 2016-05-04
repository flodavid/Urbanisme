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

set(ENGINE_DIR ../Engine)
set(TEST_ENGINE_DIR ${ENGINE_DIR}/Tests)

set(ENGINE_SOURCES ${ENGINE_DIR}/coordinates.cpp ${ENGINE_DIR}/Field.cpp ${ENGINE_DIR}/Field_others.cpp  ${ENGINE_DIR}/Parameters.cpp)
set(EVAL_SOURCES  ../evaluation.cpp)
set(TEST_ENGINE_SOURCES ${TEST_ENGINE_DIR}/test_next_coordinates.cpp ${TEST_ENGINE_DIR}/test_coordinates.cpp ${TEST_ENGINE_DIR}/fieldtestfixture.cpp)
set(TEST_EVAL_SOURCES minpathroadtestfixture.cpp)

# Executable evaluation
add_executable(tests ${ENGINE_SOURCES} ${EVAL_SOURCES} ${TEST_ENGINE_SOURCES} ${TEST_EVAL_SOURCES} all_tests.cpp)

install(TARGETS tests RUNTIME DESTINATION bin)

# Tests
target_link_libraries(tests cppunit)
