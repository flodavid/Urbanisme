// g++ -o test_my_class_full.exe test_my_class_full.cpp -I/usr/local/include -L/usr/local/lib -lcppunit
#ifndef TEST_MY_CLASS_H_
#define TEST_MY_CLASS_H_

#include "cppunit/TestCase.h"
#include "cppunit/TestFixture.h"
#include "cppunit/TestCaller.h"
#include "cppunit/TestResult.h"
#include "cppunit/TestSuite.h"
#include "cppunit/CompilerOutputter.h"
#include "cppunit/XmlOutputter.h"
#include "cppunit/ui/text/TestRunner.h"

#include "my_class.h"

/**
 * @brief Classe 'coquille' : ne pas utiliser
 * Tests sur les méthodes de la classe Dummy
 * @test
 */
class MyClassTestFixture : public CppUnit::TestFixture {
 private:

 public:
  void setUp();
  void tearDown();

  void test_set();
 
};


#endif /* TEST_MY_CLASS_H_ */

