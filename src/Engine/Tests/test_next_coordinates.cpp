#include "test_next_coordinates.h"

using namespace std;

void NextCoordinatesTestFixture::setUp() {
    
    resol= new Field(20,20);
    
    // Coordonnées valides
    coord_in= new Coordinates(10,11);		expected_next_in= new Coordinates(11,11); 
    coord_last_col= new Coordinates(19,8);	expected_next_last_col= new Coordinates(0,9);
    
    // Coordonnées invalides
    out_1= new Coordinates(20, 7);
    out_2= new Coordinates(5, -1);
    
    // Affichage
    cout << "Coordinates of test : " << (*coord_in)
    << " and "<< (*coord_last_col)
    << " and "<< (*out_1)
    << " and "<< (*out_2) << endl;
}

void NextCoordinatesTestFixture::tearDown() {
    delete resol;
    delete coord_in;
    delete coord_last_col;
    delete out_1;
    delete out_2;
    delete expected_next_in;
    delete expected_next_last_col;
}

void NextCoordinatesTestFixture::test_validsCoords()
{
//     Au milieu
    // La méthode doit trouver une coord suivante et retourner vrai
    CPPUNIT_ASSERT(resol->nextCoordinates(coord_in));
    
    // Elle doit être égale à celle attendue
    CPPUNIT_ASSERT((*coord_in) == (*expected_next_in));
    
//     Dernière colonne
    // La méthode doit trouver une coord suivante et retourner vrai
    CPPUNIT_ASSERT(resol->nextCoordinates(coord_last_col));
    
    // Elle doit être égale à celle attendue
    CPPUNIT_ASSERT((*coord_last_col) == (*expected_next_last_col));
}

void NextCoordinatesTestFixture::test_invalidsCoords()
{
    Coordinates save_out_1= *out_1;
    Coordinates save_out_2= *out_2;
    
//     Première
    // La méthode doit pas trouver de coord suivante et retourner faux
    CPPUNIT_ASSERT(! resol->nextCoordinates(out_1));
    
    // Elle ne doit pas avoir été modifiée
    CPPUNIT_ASSERT(save_out_1 == (*out_1));
    
//     Seconde
    // La méthode doit pas trouver de coord suivante et retourner faux
    CPPUNIT_ASSERT(! resol->nextCoordinates(out_2));
    
    // Elle ne doit pas avoir été modifiée
    CPPUNIT_ASSERT(save_out_2 == (*out_2));
}

