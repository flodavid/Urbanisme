#ifndef FIELDTESTFIXTURE_H
#define FIELDTESTFIXTURE_H

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

class FieldTestFixture : public CppUnit::TestFixture {
private:
    Field* field;
    
    Coordinates* coord_in;
    Coordinates* coord_border_bot;
    Coordinates* coord_border_top;
    Coordinates* coord_border_left;
    Coordinates* coord_border_right;
    
    Coordinates* coord_out;

public:
  void setUp();
  void tearDown();

  void test_containsValids();
  void test_containsInvalids();
};

#endif // FIELDTESTFIXTURE_H
