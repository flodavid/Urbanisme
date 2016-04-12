#include "stdafx.h"
#include "Resolution.h"

#include <cassert>

using namespace std;

Resolution::Resolution(const Field& _field, const Parameters& _params):
	field(_field), params(_params)
{
}

Resolution::~Resolution()
{
}

// #########################
// 	Getters
// #########################

// #########################
// 	Setters
// #########################
void Resolution::set_params(const Parameters& _params)
{
    params= _params;
}

void Resolution::increment_road_distance(const Coordinates& coord, unsigned int value)
{
    road_distances[coord.row][coord.col]+= value;
}


// #########################
//	Calculs de données
// #########################

// TODO
void Resolution::initManhattanDistancesMatrice()
{
    // Initialisation de la matrice selon la taille
    road_distances.resize(field.get_height());
    for (unsigned i= 0; i < field.get_height(); ++i) {
	vector<unsigned> row= road_distances[i];
	road_distances[i].resize(field.get_width(), 0); // Initialisation à 0
    }
    
    // Calculs des distances
    Coordinates coord1(0,0);
    while(nextCoordinates(&coord1)){
	// On calcule et additionne la distance pour aller vers chacun des successeurs
	Coordinates coord2(coord1);
	while(nextCoordinates(&coord2)){
// 	    unsigned dist= coord1.manhattanDistance(coord2);
	}
    }
    
    road_distances_are_initiated= true;
}


// #########################
// 	Evaluations
// #########################

unsigned int Resolution::evaluateTotalUsable() const
{
    unsigned nb_usables= 0;
    for (vector<State> row_parcel_state : field) {
	for (State parcel_state : row_parcel_state) {
	    cout << "parcel_state = "<< parcel_state<<endl;
	    assert(parcel_state >= -1 &&  parcel_state<= unusable);
	    if (parcel_state == usable) {
		++nb_usables;
	    }
	}
    }
    
#if DEBUG_EVALUATION
    cout << "Total number of usables parcels : "<< nb_usables<< endl;
#endif
    assert(nb_usables >=0 && nb_usables < field.getNbParcels() && "nombre d'exploitables incohérent");
    
    return nb_usables;
}

// TODO
float Resolution::manhattanRatioBetween2Parcels(const Coordinates& p1, const Coordinates& p2) const
{
    float ratio=-1;
//     ratio= field.get_distance(p1, p2) / p1.manhattanDistance(p2);
    
    return ratio;
}

float Resolution::evaluateRatio() const
{
    float total_ratio= 0.0;
    
    
    
    
    return total_ratio;
}


// #########################
/* Autres méthodes utiles */
// #########################

bool Resolution::nextCoordinates(Coordinates* coord)
{
    // On vérifie que la coordonnée actuelle est dans la surface
    //  (vérification sur les colonnes puis sur les lignes)
    if ( ( coord->col < 0 || coord->col >= (int)(field.get_width()) ) ||
	( coord->row < 0 || coord->row >= (int)(field.get_height()) ) ) {
	    #if DEBUG_PARCOURS_COORDS
		cout << "Passage à la ligne suivante" <<endl;
	    #endif
	return false;
    } else {
	// Si on est à la fin de la ligne, on passe à la ligne suivante 
	if ( (unsigned)(coord->col) +1 == field.get_width()) {
	    // seulement si on n'est pas à la fin de la surface
	    if ( (unsigned)(coord->row) +1 < field.get_width()) {
		coord->col= 0;
		coord->row+= 1;
	    #if DEBUG_PARCOURS_COORDS
		cout << "Passage à la ligne suivante" <<endl;
	    #endif
		return true;
	    }
	    else {
		// dernière case de la surface
	    #if DEBUG_PARCOURS_COORDS
		cout << "Dernière case de la matrice" <<endl;
	    #endif
		return false;
	    }
	}// Sinon, on se décale sur la ligne
	else {
	    coord->col+= 1;
	    #if DEBUG_PARCOURS_COORDS
		cout << "Passage à la case suivante (à droite)" <<endl;
	    #endif
	    return true;
	}
    }
}
