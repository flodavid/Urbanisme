#ifndef FIELDTESTFIXTURE_H
#define FIELDTESTFIXTURE_H

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
 * Tests sur la classe Field
 * @test
 */
class FieldTestFixture : public CppUnit::TestFixture {
private:
    Field* field;
    
    Coordinates* coord_in;
    Coordinates* coord_border_bot;
    Coordinates* coord_border_top;
    Coordinates* coord_border_left;
    Coordinates* coord_border_right;
    
    Coordinates* coord_out;

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
     * @test de la méthode contains() sur des coordonnées dans la surface
     * Contains doit retourner vrai
     */
    void test_containsValids();
    /**
     * @test de la méthode contains() sur des coordonnées hors de la surface
     * Contains doit retourner faux
     */
    void test_containsInvalids();
    
    /**
     * @test de la méthode getServingRoads() sur des coordonnées dans la surface
     */
    void test_servingValids();
};

#endif // FIELDTESTFIXTURE_H
