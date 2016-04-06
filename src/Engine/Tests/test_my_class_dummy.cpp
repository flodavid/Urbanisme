#include "test_my_class_full.h"

void MyClassTestFixture::setUp() {

}

void MyClassTestFixture::tearDown() {

}

void MyClassTestFixture::test_set() {
	MyClass a;
	int expected_value = 234;
	
	a.set(expected_value);
	CPPUNIT_ASSERT(a.get() == expected_value);

}