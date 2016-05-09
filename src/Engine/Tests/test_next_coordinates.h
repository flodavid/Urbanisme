// g++ -o test_my_class_full.exe test_my_class_full.cpp -I/usr/local/include -L/usr/local/lib -lcppunit
#ifndef TEST_NEXT_COORDINATES_H_
#define TEST_NEXT_COORDINATES_H_

#include "cppunit/TestCase.h"
#include "cppunit/TestFixture.h"
#include "cppunit/TestCaller.h"
#include "cppunit/TestResult.h"
#include "cppunit/TestSuite.h"
#include "cppunit/CompilerOutputter.h"
#include "cppunit/XmlOutputter.h"
#include "cppunit/ui/text/TestRunner.h"

#include "../coordinates.h"
#include "../Field.h"

/**
 * @brief Tests sur la méthode nextCoordinates() de la classe Field
 * @test
 */
class NextCoordinatesTestFixture : public CppUnit::TestFixture {
 private:
    Field* resol;
     
    Coordinates* coord_in;
    Coordinates* coord_last_col;
    Coordinates* out_1;
    Coordinates* out_2;
    
// Expected results 
    // Next of coord_in
    Coordinates* expected_next_in;
    // Next of coord_last_col
    Coordinates* expected_next_last_col;

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
     * @test de la méthode nextCoordinates() sur des coordonnées "valides"
     * Doit retourner vrai et modifier la coordonnée en entrée
     */
    void test_validsCoords();
    /**
     * @test de la méthode nextCoordinates() sur des coordonnées non "valides"
     * Doit retourner faux et ne pas modifier la coordonnée en entrée
     */
    void test_invalidsCoords();
 
};


#endif /* TEST_NEXT_COORDINATES_H_ */


