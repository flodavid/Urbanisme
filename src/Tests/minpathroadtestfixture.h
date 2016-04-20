#ifndef MINPATHROADTESTFIXTURE_H
#define MINPATHROADTESTFIXTURE_H


#include "cppunit/TestCase.h"
#include "cppunit/TestFixture.h"
#include "cppunit/TestCaller.h"
#include "cppunit/TestResult.h"
#include "cppunit/TestSuite.h"
#include "cppunit/CompilerOutputter.h"
#include "cppunit/XmlOutputter.h"
#include "cppunit/ui/text/TestRunner.h"

#include "../Resolution.h"
#include "../Engine/coordinates.h"

/**
 * Tests sur la méthode calcRoadDistance() de la classe Resolution
 * @test
 */
class MinPathRoadTestFixture : public CppUnit::TestFixture
{
private:
    Resolution* resol;
    
    Coordinates* coord1;
    Coordinates* coord2;
    
    Coordinates* coord_out;
    Coordinates* coord_unusable; // TODO aucun test ne vérifie le chemin avec une coordonnée de parcelle non exploitable
    
    // Results expected
    unsigned expected_dist;
    
public:
    /**
     * @brief Opérations avant les tests
     */
    void setUp();
    /**
     * @brief Opérations après les tests
     */
    void tearDown();

    void test_validsPath();
    void test_sameParcel();
    void test_invalidsPath();
    
    void test_findVisisted();
};

#endif // MINPATHROADTESTFIXTURE_H
