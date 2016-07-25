#include "minpathroadtestfixture.h"
#include "../stdafx.h"

#include "../Engine/field.h"

using namespace std;

void MinPathRoadTestFixture::setUp()
{
// Surface de l'exemple :
    example= new Field(20, 20);
    example->createExample();
    
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
    clog << "Test chemin de coordonnées à d'autres"<< endl;
    Parameters* params= new Parameters(2, 1);
    eval= new FieldEvaluation(*example, *params);
    // On définit les parcelles qui sont utilisables et celles qui ne le sont pas
    eval->setUsables(params->get_serve_distance());
    
    if (!eval->road_distances_are_initiated){
        eval->initRoadDistances();
    }

    cout << "Routes desservant "<< (*coord1)<< " : "<< *(eval->getServingRoads(*coord1, params->get_serve_distance()));
    cout << "Routes desservant "<< (*coord2)<< " : "<< *(eval->getServingRoads(*coord2, params->get_serve_distance()));
    
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
    Parameters* params= new Parameters(2, 1);
    eval= new FieldEvaluation(*example, *params);
    // On définit les parcelles qui sont utilisables et celles qui ne le sont pas
    eval->setUsables(params->get_serve_distance());
    
    unsigned dist= eval->parcelsRoadDistance(*coord1, *coord1);
    // Si on test la distance entre une case et elle-même, on doit obtenir 0
    CPPUNIT_ASSERT_EQUAL(dist, (unsigned)0);
}

void MinPathRoadTestFixture::test_invalidsPath()
{
    // Le paramètre de distance de desserte n'est pas à 2, mais à 1
    Parameters* params= new Parameters(1, 1);
    eval= new FieldEvaluation(*example, *params);
    // On définit les parcelles qui sont utilisables et celles qui ne le sont pas
    eval->setUsables(params->get_serve_distance());
    
    unsigned dist_out= eval->getRoadDistance(*coord1, *coord_out);
    // Si une des deux cases est en dehors, on doit obtenir l'infini
    CPPUNIT_ASSERT_EQUAL(UNSIGNED_INFINITY, dist_out);
   
    Coordinates coord_unusable(1, 4);
    unsigned dist_unusable= eval->parcelsRoadDistance(*coord1, coord_unusable);
    // Si la case n'est pas exploitable, on doit obtenir l'infini
    CPPUNIT_ASSERT_EQUAL(UNSIGNED_INFINITY, dist_unusable);
}

#include <list>
void MinPathRoadTestFixture::test_findVisisted()
{
    list<const Coordinates*> visited;
    visited.push_back(coord1);
    
    CPPUNIT_ASSERT(find(visited.begin(), visited.end(), coord1) != visited.end());
}



