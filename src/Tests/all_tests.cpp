#include "minpathroadtestfixture.h"

using namespace std;

CppUnit::TestSuite *make_suite() {    
    CppUnit::TestSuite *suite = new CppUnit::TestSuite("Operations");
    cout << "==============================================" << endl;
    cout << "TEST " << suite->getName() << " (" << __FILE__ << ")" << endl;
    cout << "==============================================" << endl;

    // Tests calcRoadDistance
    suite->addTest( new CppUnit::TestCaller<MinPathRoadTestFixture>("test_validsPath",
			&MinPathRoadTestFixture::test_validsPath) );
    suite->addTest( new CppUnit::TestCaller<MinPathRoadTestFixture>("test_sameParcel",
			&MinPathRoadTestFixture::test_sameParcel) );
    suite->addTest( new CppUnit::TestCaller<MinPathRoadTestFixture>("test_invalidsPath",
			&MinPathRoadTestFixture::test_invalidsPath) );
    suite->addTest( new CppUnit::TestCaller<MinPathRoadTestFixture>("test_findVisisted",
			&MinPathRoadTestFixture::test_findVisisted) );
    
    return suite;
}


int main() {
    
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(make_suite());
    runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), cout));
    runner.run();
    return 0;
}