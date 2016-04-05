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

CppUnit::TestSuite *make_suite() {
	CppUnit::TestSuite *suite = new CppUnit::TestSuite("MyClass");
	cout << "==============================================" << endl;
	cout << "TEST " << suite->getName() << " (" << __FILE__ << ")" << endl;
	cout << "==============================================" << endl;

	suite->addTest( new CppUnit::TestCaller<MyClassTestFixture>("test_set", &MyClassTestFixture::test_set) );

	return suite;
}


int main() {
	CppUnit::TextUi::TestRunner runner;

	runner.addTest(make_suite());
	runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), cout));
	runner.run();
	return 0;
}







