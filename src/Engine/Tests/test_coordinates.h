// g++ -o test_my_class_full.exe test_my_class_full.cpp -I/usr/local/include -L/usr/local/lib -lcppunit
#ifndef TEST_COORDINATES_H_
#define TEST_COORDINATES_H_

#include "cppunit/TestCase.h"
#include "cppunit/TestFixture.h"
#include "cppunit/TestCaller.h"
#include "cppunit/TestResult.h"
#include "cppunit/TestSuite.h"
#include "cppunit/CompilerOutputter.h"
#include "cppunit/XmlOutputter.h"
#include "cppunit/ui/text/TestRunner.h"

#include "../coordinates.h"

/**
 * Tests sur les méthodes de la classe Coordinates
 */
class CoordinatesTestFixture : public CppUnit::TestFixture {
 private:
    Coordinates* coord1;
    Coordinates* coord2;
    
    // Manhattant dist
    int expected_manhattan;
    // Euclidean dist
    float expected_euclid;

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
     * @test le calcul de la distance Manhattan entre deux coordonnées
     */
    void test_calcManhattan();
    /**
     * @test le calcul de la distance Euclidienne entre deux coordonnées
     */
    void test_calcEuclidean();
 
};


#endif /* TEST_COORDINATES_H_ */

