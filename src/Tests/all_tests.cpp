#include "test_next_coordinates.h"

CppUnit::TestSuite *make_suite() {    
	CppUnit::TestSuite *suite = new CppUnit::TestSuite("Operations");
	cout << "==============================================" << endl;
	cout << "TEST " << suite->getName() << " (" << __FILE__ << ")" << endl;
	cout << "==============================================" << endl;

	suite->addTest( new CppUnit::TestCaller<CoordinatesTestFixture>("test_validsCoords", &CoordinatesTestFixture::test_validsCoords) );
	suite->addTest( new CppUnit::TestCaller<CoordinatesTestFixture>("test_invalidsCoords", &CoordinatesTestFixture::test_invalidsCoords) );

	return suite;
}


int main() {
    
	CppUnit::TextUi::TestRunner runner;

	runner.addTest(make_suite());
	runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), cout));
	runner.run();
	return 0;
}