#include "test_coordinates.h"
#include "test_next_coordinates.h"
#include "fieldtestfixture.h"

using namespace std;

/**
 * Crée la suite de tests du "moteur" de l'application
 * @return Une TestSuite, contenant les tests à effectuer sur le "moteur"
 */
CppUnit::TestSuite *make_suite() {    
	CppUnit::TestSuite *suite = new CppUnit::TestSuite("Coordinates");
	cout << "==============================================" << endl;
	cout << "TEST " << suite->getName() << " (" << __FILE__ << ")" << endl;
	cout << "==============================================" << endl;

	suite->addTest( new CppUnit::TestCaller<CoordinatesTestFixture>("test_calcManhattan",
			    &CoordinatesTestFixture::test_calcManhattan) );
	suite->addTest( new CppUnit::TestCaller<CoordinatesTestFixture>("test_calcEuclidean",
			    &CoordinatesTestFixture::test_calcEuclidean) );
	
	// Tests test next_coordinates()
	suite->addTest( new CppUnit::TestCaller<NextCoordinatesTestFixture>("test_validsCoords",
				&NextCoordinatesTestFixture::test_validsCoords) );
	suite->addTest( new CppUnit::TestCaller<NextCoordinatesTestFixture>("test_invalidsCoords",
				&NextCoordinatesTestFixture::test_invalidsCoords) );
	
	suite->addTest( new CppUnit::TestCaller<FieldTestFixture>("test_containsValids",
			    &FieldTestFixture::test_containsValids) );
	suite->addTest( new CppUnit::TestCaller<FieldTestFixture>("test_containsInvalids",
			    &FieldTestFixture::test_containsInvalids) );

	return suite;
}

/**
 * Exécute des tests sur le "moteur" de l'application
 * @return l'état de retour de la fonction
 */
int main() {
    
	CppUnit::TextUi::TestRunner runner;

	runner.addTest(make_suite());
	runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), cout));
	runner.run();
    return EXIT_SUCCESS;
}
