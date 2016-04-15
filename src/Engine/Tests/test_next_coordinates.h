// g++ -o test_my_class_full.exe test_my_class_full.cpp -I/usr/local/include -L/usr/local/lib -lcppunit
#ifndef TEST_NEXT_COORDINATES_H_
#define TEST_NEXT_COORDINATES_H_

#include "cppunit/TestCase.h"
#include "cppunit/TestFixture.h"
#include "cppunit/TestCaller.h"
#include "cppunit/TestResult.h"
#include "cppunit/TestSuite.h"
#include "cppunit/CompilerOutputter.h"
#include "cppunit/XmlOutputter.h"
#include "cppunit/ui/text/TestRunner.h"

#include "../coordinates.h"
#include "../Field.h"

class NextCoordinatesTestFixture : public CppUnit::TestFixture {
 private:
    Field* resol;
     
    Coordinates* coord_in;
    Coordinates* coord_last_col;
    Coordinates* out_1;
    Coordinates* out_2;
    
// Expected results 
    // Next of coord_in
    Coordinates* expected_next_in;
    // Next of coord_last_col
    Coordinates* expected_next_last_col;

public:
    void setUp();
    void tearDown();

    void test_validsCoords();
    void test_invalidsCoords();
 
};


#endif /* TEST_NEXT_COORDINATES_H_ */


