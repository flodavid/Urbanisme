#include "test_next_coordinates.h"
#include "minpathroadtestfixture.h"

using namespace std;

CppUnit::TestSuite *make_suite() {    
    CppUnit::TestSuite *suite = new CppUnit::TestSuite("Operations");
    cout << "==============================================" << endl;
    cout << "TEST " << suite->getName() << " (" << __FILE__ << ")" << endl;
    cout << "==============================================" << endl;

    // Tests test next_coordinates()
    suite->addTest( new CppUnit::TestCaller<CoordinatesTestFixture>("test_validsCoords", &CoordinatesTestFixture::test_validsCoords) );
    suite->addTest( new CppUnit::TestCaller<CoordinatesTestFixture>("test_invalidsCoords", &CoordinatesTestFixture::test_invalidsCoords) );

    // Tests calcRoadDistance
    suite->addTest( new CppUnit::TestCaller<MinPathRoadTestFixture>("test_validsPath", &MinPathRoadTestFixture::test_validsPath) );
    suite->addTest( new CppUnit::TestCaller<MinPathRoadTestFixture>("test_sameParcel", &MinPathRoadTestFixture::test_sameParcel) );
    suite->addTest( new CppUnit::TestCaller<MinPathRoadTestFixture>("test_invalidsPath", &MinPathRoadTestFixture::test_invalidsPath) );
    
    return suite;
}


int main() {
    
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(make_suite());
    runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), cout));
    runner.run();
    return 0;
}