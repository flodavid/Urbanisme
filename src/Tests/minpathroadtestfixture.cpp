#include "minpathroadtestfixture.h"
#include "../stdafx.h"

#include "../Engine/Field.h"

using namespace std;

void MinPathRoadTestFixture::setUp()
{
// Surface de l'exemple :
    Field example(20, 20);
    example.add_in_out(9, 0);
    // Colonne Milieu
	example.add_road(9, 1);
	example.add_road(9, 2);
	example.add_road(9, 3);
	example.add_road(9, 4);
	example.add_road(9, 5);
	example.add_road(9, 6);
	example.add_road(9, 7);
	example.add_road(9, 8);
	example.add_road(9, 9);
	example.add_road(9, 10);
	example.add_road(9, 11);
	example.add_road(9, 12);
	example.add_road(9, 13);
	example.add_road(9, 14);
	example.add_road(9, 15);
	example.add_road(9, 16);
	example.add_road(9, 17);
	example.add_road(9, 18);
    // Première Ligne
	example.add_road(0, 2);
	example.add_road(1, 2);
	example.add_road(2, 2);
	example.add_road(3, 2);
	example.add_road(4, 2);
	example.add_road(5, 2);
	example.add_road(6, 2);
	example.add_road(7, 2);
	example.add_road(8, 2);
	// example.add_road(9, 2); déjà fait dans colonne milieu
	example.add_road(10, 2);
	example.add_road(11, 2);
	example.add_road(12, 2);
	example.add_road(13, 2);
	example.add_road(14, 2);
	example.add_road(15, 2);
	example.add_road(16, 2);
	example.add_road(17, 2);
	example.add_road(18, 2);
	example.add_road(19, 2);
    // Deuxième Ligne
	example.add_road(0, 7);
	example.add_road(1, 7);
	example.add_road(2, 7);
	example.add_road(3, 7);
	example.add_road(4, 7);
	example.add_road(5, 7);
	example.add_road(6, 7);
	example.add_road(7, 7);
	example.add_road(8, 7);
	// example.add_road(9, 7); déjà fait dans colonne milieu
	example.add_road(10, 7);
	example.add_road(11, 7);
	example.add_road(12, 7);
	example.add_road(13, 7);
	example.add_road(14, 7);
	example.add_road(15, 7);
	example.add_road(16, 7);
	example.add_road(17, 7);
	example.add_road(18, 7);
	example.add_road(19, 7);
    // Troisième Ligne
	example.add_road(0, 12);
	example.add_road(1, 12);
	example.add_road(2, 12);
	example.add_road(3, 12);
	example.add_road(4, 12);
	example.add_road(5, 12);
	example.add_road(6, 12);
	example.add_road(7, 12);
	example.add_road(8, 12);
	// example.add_road(9, 12); déjà fait dans colonne milieu
	example.add_road(10, 12);
	example.add_road(11, 12);
	example.add_road(12, 12);
	example.add_road(13, 12);
	example.add_road(14, 12);
	example.add_road(15, 12);
	example.add_road(16, 12);
	example.add_road(17, 12);
	example.add_road(18, 12);
	example.add_road(19, 12);
    // Quatrième Ligne
	example.add_road(0, 17);
	example.add_road(1, 17);
	example.add_road(2, 17);
	example.add_road(3, 17);
	example.add_road(4, 17);
	example.add_road(5, 17);
	example.add_road(6, 17);
	example.add_road(7, 17);
	example.add_road(8, 17);
	// example.add_road(9, 17); déjà fait dans colonne milieu
	example.add_road(10, 17);
	example.add_road(11, 17);
	example.add_road(12, 17);
	example.add_road(13, 17);
	example.add_road(14, 17);
	example.add_road(15, 17);
	example.add_road(16, 17);
	example.add_road(17, 17);
	example.add_road(18, 17);
	example.add_road(19, 17);
    example.add_in_out(9, 19);
    
    example.defineUsables(eval->params.get_serve_distance());
    
    eval= new Evaluation(example, Parameters(1, 2));
    
/// Initialisation
    // Coordonnées valides
    /* Coordonnées de l'exemple */
    coord1= new Coordinates(19,4); // A
    coord2= new Coordinates(19,5); // B
    
    expected_dist= 26;
    
    // Coordonnées invalides
    coord_out= new Coordinates(-1,-1); // En dehors du plateau
    coord_unusable= NULL; // TODO modifier le plateau pour vérifier les coordonnées non atteignables
    
    
    // Affichage
    cout << "Coordinates of test : " << (*coord1)
    << " and "<< (*coord2) << endl;
    
    // Définition des états des parcelles
//    eval->field.defineUsables(eval->params.get_serve_distance());
//    if (!eval->road_distances_are_initiated){
//        eval->initNeighbourhoodManhattan();
//    }
}

void MinPathRoadTestFixture::tearDown()
{
    delete eval;
    
    delete coord1;
    delete coord2;
    
    delete coord_out;
}

void MinPathRoadTestFixture::test_validsPath()
{
    if (!eval->road_distances_are_initiated){
        eval->initNeighbourhoodManhattan();
    }

    cout << "Routes desservant "<< (*coord1)<< " : "<< *(eval->field.getServingRoads(*coord1, eval->params.get_serve_distance()));
    cout << "Routes desservant "<< (*coord2)<< " : "<< *(eval->field.getServingRoads(*coord2, eval->params.get_serve_distance()));
    
    unsigned dist= eval->getRoadDistance(*coord1, *coord2);
    // Si on test la distance entre la première et la 2e est égale à celle attendue
    cout << "Valeur infinity : "<< UNSIGNED_INFINITY <<" OMFG !"<< endl
	<<endl;
	
    CPPUNIT_ASSERT_EQUAL(expected_dist, dist);

    
    Coordinates coord3(12, 6);
    Coordinates coord4(6, 18);
    unsigned expected_dist_local= 17;
    
    unsigned dist2= eval->getRoadDistance(coord3, coord4);
    // Si on test la distance entre la première et la 2e est égale à celle attendue
    cout << "Valeur infinity : "<< UNSIGNED_INFINITY <<" OMFG !"<< endl
    <<endl;
    
    CPPUNIT_ASSERT_EQUAL(expected_dist_local, dist2);
}

void MinPathRoadTestFixture::test_sameParcel()
{
    unsigned dist= eval->getRoadDistance(*coord1, *coord1);
    // Si on test la distance entre une case et elle-même, on doit obtenir 0
    CPPUNIT_ASSERT_EQUAL(dist, (unsigned)0);
}

void MinPathRoadTestFixture::test_invalidsPath()
{
    unsigned dist_out= eval->getRoadDistance(*coord1, *coord_out);
    // Si une des deux cases est en dehors, on doit obtenir l'infini
    CPPUNIT_ASSERT_EQUAL(UNSIGNED_INFINITY, dist_out);
   
    // TODO initialiser la coordonnée de parcelle non exploitable pour effectuer le test
//     unsigned dist_out= eval->calcRoadDistance(*coord1, *coord_unusable);
//     // Si une des deux cases est en dehors, on doit obtenir l'infini
//     CPPUNIT_ASSERT(dist_unusable == UNSIGNED_INFINITY);
}

#include <list>
void MinPathRoadTestFixture::test_findVisisted()
{
    list<const Coordinates*> visited;
    visited.push_back(coord1);
    
    CPPUNIT_ASSERT(find(visited.begin(), visited.end(), coord1) != visited.end());
}



