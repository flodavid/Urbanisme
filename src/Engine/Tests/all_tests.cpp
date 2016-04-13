#include "test_coordinates.h"

CppUnit::TestSuite *make_suite() {    
	CppUnit::TestSuite *suite = new CppUnit::TestSuite("Coordinates");
	cout << "==============================================" << endl;
	cout << "TEST " << suite->getName() << " (" << __FILE__ << ")" << endl;
	cout << "==============================================" << endl;

	suite->addTest( new CppUnit::TestCaller<CoordinatesTestFixture>("test_calcManhattan", &CoordinatesTestFixture::test_calcManhattan) );
	suite->addTest( new CppUnit::TestCaller<CoordinatesTestFixture>("test_calcEuclidean", &CoordinatesTestFixture::test_calcEuclidean) );

	return suite;
}


int main() {
    
	CppUnit::TextUi::TestRunner runner;

	runner.addTest(make_suite());
	runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), cout));
	runner.run();
	return 0;
}