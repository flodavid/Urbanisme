#include "minpathroadtestfixture.h"
#include "../Engine/Tests/fieldtestfixture.h"
#include "../Engine/Tests/test_coordinates.h"
#include "../Engine/Tests/test_next_coordinates.h"

using namespace std;

/**
 * Crée la suite de tests du "moteur" de l'application
 * @return Une TestSuite, contenant les tests à effectuer sur le "moteur"
 */
CppUnit::TestSuite *make_suite_engine() {    
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
    
    
    suite->addTest( new CppUnit::TestCaller<FieldTestFixture>("test_servingValids",
							      &FieldTestFixture::test_servingValids) );
    
    return suite;
}

/**
 * Crée la suite de tests sur la recherche de plus court chemin entre deux parcelles
 * @return Une TestSuite, contenant les tests à effectuer sur l'évaluation
 */
CppUnit::TestSuite *make_suite_pathfinding() {    
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
 * Exécute des tests sur toute l'application
 * @return l'état de retour de la fonction
 */
int main() {
    
    CppUnit::TextUi::TestRunner runner;

    runner.addTest(make_suite_engine());
    runner.addTest(make_suite_pathfinding());
    runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), cout));
    runner.run();
    return 0;
}
