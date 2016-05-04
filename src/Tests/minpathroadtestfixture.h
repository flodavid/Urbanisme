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

#include "../evaluation.h"
#include "../Engine/coordinates.h"

/**
 * Tests sur la méthode calcRoadDistance() de la classe Evaluation
 * @test
 */
class MinPathRoadTestFixture : public CppUnit::TestFixture
{
private:
    Evaluation* eval;
    Field* example;
    
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

    /**
     * @test de la méthode calcRoadDistance() entre des parcelles "reliées" par des routes
     * La méthode doit alors retourner une valeur supérieure à 0
     */
    void test_validsPath();
    /**
     * @test de la méthode calcRoadDistance() entre une parcelle et elle-même
     * La méthode doit alors retourner 0
     */
    void test_sameParcel();
    /**
     * @test de la méthode calcRoadDistance() entre des parcelles non "reliées" par des routes
     * La méthode doit alors retourner l'infini
     */
    void test_invalidsPath();
    
    /**
     * @test d'un find() sur un vecteur de coordonnée, utilisé dans recCalcRoadDistance,
     * lors de l'ajout d'un voisin d'une route
     */
    void test_findVisisted();
};

#endif // MINPATHROADTESTFIXTURE_H
