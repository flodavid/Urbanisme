﻿#include "stdafx.h"
#include "Resolution.h"

#include <cassert>
#include <map>

using namespace std;

Resolution::Resolution(const Field& _field, const Parameters& _params):
    field(_field), params(_params)
{
}

Resolution::~Resolution()
{
}

/// #########################
///      Getters
/// #########################

/// #########################
///      Setters
/// #########################
void Resolution::set_params(const Parameters& _params)
{
    params= _params;
}

void Resolution::increment_road_distance(const Coordinates& coord, unsigned int value)
{
    road_distances[coord.row][coord.col]+= value;
}


/// #########################
///	Calculs de données
/// #########################

// TODO
// void Resolution::TODO_parcours_cellules()
// {
//     // Initialisation de la matrice selon la taille
//     road_distances.resize(field.get_height());
//     for (unsigned i= 0; i < field.get_height(); ++i) {
// 	vector<unsigned> row= road_distances[i];
// 	road_distances[i].resize(field.get_width(), 0); // Initialisation    0
//     }
//     // Calculs des distances
//     Coordinates coord1(0,0);
//     while(nextCoordinates(&coord1)){
// 	// On calcule et additionne la distance pour aller vers chacun des successeurs
// 	Coordinates coord2(coord1);
// 	while(nextCoordinates(&coord2)){
// // 	    unsigned dist= coord1.manhattanDistance(coord2);
// 	}
//     }
//
//     road_distances_are_initiated= true;
// }

//@{
/// ###############################
///	Fonctions auxiliaires
/// ###############################
std::list<const Coordinates *>* Resolution::getServingRoads(const Coordinates& coord) const
{
    list<const Coordinates*>* serving_roads= new list<const Coordinates*>;
    
    int serve_dist= params.get_serve_distance(); // on pourrait utiliser unsigned

    // On vérifie si les routes entre (x +dist;y +dist) et (x -dist;y -dist)
    // @SEE on vérifie serve_dist² parcelles,  alors qu'on pourrait en vérifier ??
    for(int i= coord.row + serve_dist; i >= coord.row -serve_dist; --i) {
        for(int j= coord.col + serve_dist; j >= coord.row -serve_dist; --j) {
            // On vérifie que la parcelle n'est pas en dehors de la matrice et qu'elle n'est pas la coordonnée courante
            if (field.contains(j, i) && j != coord.col && i != coord.row
                // TODO changer, ne pas utiliser manhattanDistance,  peu performant ? si on l'utilise,  crée Coordinate(j, i) avant !
                && coord.manhattanDistance(Coordinates(j,  i)) <= 2) { 
                #if DEBUG_ROADS
                cout << "parcelle en "<< j<< " ; "<< i<< "est voisine de celle en "<< coord.col<< " ; "<< coord.row<< endl;
                #endif
                // Ajout dans les routes voisines de la parcelle
                Coordinates* road_coord= new Coordinates(j, i);
                serving_roads->push_back(road_coord);
            }
        }
    }

    return serving_roads;
}
    
std::list<const Coordinates *>* Resolution::getNeighbourRoads(const Coordinates& coord) const
{
    list<const Coordinates*>* neighbour_roads= new list<const Coordinates*>;
    
    assert(false == true);//TODO faire 4 'if's
    if (false) {
         // On vérifie que la parcelle n'est pas en dehors de la matrice et qu'elle n'est pas la coordonnée courante
            if (field.contains(j, i) && j != coord.col && i != coord.row) {
                #if DEBUG_ROADS
                cout << "parcelle en "<< j<< " ; "<< i<< "est voisine de celle en "<< coord.col<< " ; "<< coord.row<< endl;
                #endif
                // Ajout dans les routes voisines de la parcelle
                Coordinates* road_coord= new Coordinates(j, i);
                neighbour_roads->push_back(road_coord);
            }
    }

    return neighbour_roads;
}



int Resolution::calcRoadDistance(const Coordinates& coord1, const Coordinates& coord2) const
{
    map<int,  Coordinates*> dist_by_neighbour;              //  @SEE utile ?
    if (coord1 ==  coord2) {
        return 0;
    } else {
        list<Coordinates*>* neighbour_roads = getServingRoads(coord1);
        for(Coordinates* new_coord : *neighbour_roads) {
            int dist = calcRoadDistance(*new_coord,  coord2);
            dist_by_neighbour[dist] =  new_coord;
        }
//         min();
    }
        
    return coord1.col;
}
//@}

/// #########################
///      Evaluations
/// #########################

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


/// #########################
/// Autres méthodes utiles
/// #########################

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
                cout << "Passage àla ligne suivante" <<endl;
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
