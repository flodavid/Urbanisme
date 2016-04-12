// g++ -o test_my_class_full.exe test_my_class_full.cpp -I/usr/local/include -L/usr/local/lib -lcppunit
#ifndef TEST_COORDINATES_H_
#define TEST_COORDINATES_H_

#include "cppunit/TestCase.h"
#include "cppunit/TestFixture.h"
#include "cppunit/TestCaller.h"
#include "cppunit/TestResult.h"
#include "cppunit/TestSuite.h"
#include "cppunit/CompilerOutputter.h"
#include "cppunit/XmlOutputter.h"
#include "cppunit/ui/text/TestRunner.h"

using namespace std;

#include "../Engine/coordinates.h"
#include "../Engine/Parameters.h"
#include "../Engine/Field.h"
#include "../Resolution.h"


class CoordinatesTestFixture : public CppUnit::TestFixture {
 private:
    Resolution* resol;
     
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


#endif /* TEST_COORDINATES_H_ */


