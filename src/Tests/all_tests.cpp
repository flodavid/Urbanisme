#include "minpathroadtestfixture.h"

using namespace std;

/**
 * Crée la suite de tests de la partie résolution de l'application
 * @return Une TestSuite, contenant les tests à effectuer sur la résolution
 */
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

/**
 * Exécute des tests sur la partie résolution de l'application
 * @return l'état de retour de la fonction
 */
int main() {
    
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(make_suite());
    runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), cout));
    runner.run();
    return 0;
}
