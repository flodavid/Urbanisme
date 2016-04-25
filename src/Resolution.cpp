#include "stdafx.h"
#include "Resolution.h"

#include <cassert>
// ##include <pthread.h>
#include <thread>
#include <map>

using namespace std;

Resolution::Resolution(const Field &_field, const Parameters &_params) :
    field(_field), params(_params)
{
}

Resolution::~Resolution()
{
}

/// #########################
///      Getters
/// #########################
//@{

//@}
/// #########################
///      Setters
/// #########################
//@{

void Resolution::set_params(const Parameters &_params)
{
    params = _params;
}

//@}
/// #########################
///     Calculs de données
/// #########################
//@{

unsigned Resolution::calcRoadDistance(const Coordinates &coord1, const Coordinates &coord2)
{
    if (coord1 == coord2) {
#if DEBUG_ROADS_DIST
        clog << "les cases de départ et d'arrivée sont identiques" << endl;
#endif
        return 0;
    }

    unsigned min_dist = UNSIGNED_INFINITY;

    list<Coordinates> *serving_roads_c1 = field.getServingRoads(coord1, params.get_serve_distance());
    list<Coordinates> *serving_roads_c2 = field.getServingRoads(coord2, params.get_serve_distance());

    for (Coordinates road_c1 : *serving_roads_c1) {
        for (Coordinates road_c2 : *serving_roads_c2) {
            list<Coordinates> *empty_visited = new list<Coordinates>;
            unsigned int dist = recCalcRoadDistance(road_c1, road_c2, empty_visited, UNSIGNED_INFINITY);
            delete empty_visited;

            // On a trouvé un chemin, meilleur que le précédent si il en existait un
            if (dist < min_dist) {
                min_dist= dist;
            }
        }
    }
    delete serving_roads_c1;
    delete serving_roads_c2;


    if (min_dist != UNSIGNED_INFINITY) {
#if DEBUG_ROADS_DIST
        cout << "Les plus court chemin de " << coord1 << " à " << coord2
             << " est de longueur " << min_dist << endl;
#endif
    } else {
        cerr << "Impossible de relier les deux parcelle par les routes"
                "(au moins une des 2 parcelle n'a pas de route à proximité (distance < "
             << params.get_serve_distance() << "))" << endl;
    }

    return min_dist;
}

unsigned Resolution::recCalcRoadDistance(const Coordinates &coord1, const Coordinates &coord2,
                                            list< Coordinates > *visited, unsigned int dist_max)
{
#if DEBUG_ROADS_DIST
    cout << "Calcul de la distance entre " << coord1 << " et " << coord2 << endl;
#endif
    assert(field[coord1] == is_road);

    if (coord1 == coord2) {
        return 0;
    } else {
        if (visited->size() >= dist_max) {
            return UNSIGNED_INFINITY;
        } else {
            unsigned min_dist = UNSIGNED_INFINITY;
            //         Coordinates& min_dist_neighbour;

            list<Coordinates> *neighbour_roads = field.getNeighbourRoads(coord1);
            for (Coordinates new_coord : *neighbour_roads) {
#if DEBUG_ROADS_DIST
                cout << "Taille de la liste des visitées : " << visited->size() << endl;
#endif
                //  recherche de cet élément dans la liste des visités
                if (find(visited->begin(), visited->end(), new_coord) == visited->end()) {

                    // On applique la recursivité avec la coordonnée envisagée courante
                    unsigned dist;
                    float current_ratio = getRoadDistance(coord1, coord2);
                    if (current_ratio != 0) {
                        dist = current_ratio;
                    } else {
                        visited->push_back(coord1);
                        dist= recCalcRoadDistance(new_coord,  coord2, visited, min_dist);
                        visited->pop_back();
                        road_distances[coord1.row][coord1.col][coord2.row][coord2.col] = dist;
                    }

                    ++dist; // distance + déplacement de la case courant à son voisin
                    // On compare le résultat obtenu avec le minimum courant
                    if (dist < min_dist) {
                        min_dist= dist;
                    }
                } else {
#if DEBUG_ROADS_DIST
                    cout << "La parcelle a déjà été visitée" << endl;
#endif
                }
            }

            delete neighbour_roads;

#if DEBUG_ROADS_DIST
            cout << "Pour aller de " << coord1 << " à " << coord2
                 << ", il y a un trajet de longueur " << min_dist << endl;
#endif
            return min_dist +1;
        }
    }
}

void Resolution::initCoordNeighbourhoodManhattan(const Coordinates &coord)
{
    if (field[coord] == is_usable) {
        cout << "Premier point : " << coord << endl;
        // On calcule et additionne le ratio pour aller vers chacun des successeurs
        Coordinates coord2(coord);
        // On commence à la coordonnée suivante de celle courante
        while (field.nextCoordinates(&coord2)) {
            // On calcule la distance que si elle n'a pas déjà été calculée
            if (field[coord2] == is_usable) {
                road_distances[coord.row][coord.col][coord2.row][coord2.col] =
                        calcRoadDistance(coord, coord2);
            }
            if (field[coord] == is_road && field[coord2] == is_road) {
                list<Coordinates> *empty_visited = new list<Coordinates>;
                road_distances[coord.row][coord.col][coord2.row][coord2.col] =
                        recCalcRoadDistance(coord, coord2, empty_visited, UNSIGNED_INFINITY);
                delete empty_visited;
            }
        }
    }
}

void Resolution::initNeighbourhoodManhattan()
{
    Coordinates coord1 = Field::first();

    // Initialisation, selon la taille, de toutes les cellules à 0
    road_distances.resize(field.get_height());
    // Initialisation de première colonne
    for (unsigned x1 = 0; x1 < field.get_height(); ++x1) {
        road_distances[x1].resize(field.get_width());

        // Initialisation de la deuxieme ligne
        for (unsigned y1 = 0; y1 < field.get_width(); ++y1) {
            road_distances[x1][y1].resize(field.get_height());

            // Initialisation de la deuxième colonne, à 0
            for (unsigned x2 = 0; x2 < field.get_height(); ++x2) {
                road_distances[x1][y1][x2].resize(field.get_width(), 0.0);
            }
        }
    }

    // Calculs des ratios de distances
    coord1 = Field::first();
    do {
        initCoordNeighbourhoodManhattan(coord1);
    } while (field.nextCoordinates(&coord1));
    
    road_distances_are_initiated =  true;
}


//@}
/// #########################
///      Evaluations
/// #########################
//@{

unsigned int Resolution::evaluateTotalUsable() const
{
    unsigned nb_usables = 0;
#if DEBUG_EVALUATION
    int x = 0, y = 0;
#endif
    for (vector<State> row_parcel_state : field) {
        for (State parcel_state : row_parcel_state) {
#if DEBUG_EVALUATION
            cout << "state(" << x << "," << y << ")= " << parcel_state << "; ";
            if (x == 0) cout << endl;
            ++x;
#endif
            assert(parcel_state >= -1 &&  parcel_state <= is_unusable);
            if (parcel_state == is_usable) {
                ++nb_usables;
            }
        }
#if DEBUG_EVALUATION
        ++y;
        x = 0;
#endif
    }

#if DEBUG_EVALUATION
    cout << "Total number of usables parcels : " << nb_usables << endl;
#endif
    assert(nb_usables >= 0 && nb_usables < field.getNbParcels() && "nombre d'exploitables incohérent");

    return nb_usables;
}

float Resolution::manhattanRatioBetween2Parcels(const Coordinates &p1, const Coordinates &p2) const
{
    unsigned road_distance = getRoadDistance(p1, p2);
    unsigned manhattan_distance = p1.manhattanDistance(p2);
    float ratio = ((float)road_distance) / ((float) manhattan_distance);

    cout << "\tDistance route "<< p1<< "->"<< p2<< " = "<< road_distance<< endl;
    cout << "\tDistance directe "<< p1<< "->"<< p2<< " = "<< manhattan_distance<< endl;
    cout << "\t\tRatio : "<< ratio<< endl;

    return ratio;
}

float Resolution::evaluateRatio(unsigned nbUsables) const
{
    float total_ratio = 0.0;
    unsigned nb_ratio = 0;

    // Calculs des distances
    Coordinates coord1 = Field::first();
    do {
        if (field[coord1] == is_usable) {
            cout << "Premier point : " << coord1 << endl;
            // On calcule et additionne le ratio pour aller vers chacun des successeurs
            Coordinates coord2(coord1);
            // On commence à la coordonnée suivante de celle courante
            while (field.nextCoordinates(&coord2)) {
                if (field[coord2] == is_usable) {
                    float ratio_c1_goto_c2 = manhattanRatioBetween2Parcels(coord1, coord2);
                    total_ratio += 2.0 * ratio_c1_goto_c2; // @see on pourrait faire un décalage de bit
                    nb_ratio+= 2;
                }
            }
        }
    } while (field.nextCoordinates(&coord1));
    cout << "Ratio total : "<< total_ratio<< ", nb ratios : "<< nb_ratio<< endl;
    
    float average =  total_ratio / ((float) nb_ratio);
    
    return average;
}

float Resolution::threadsEvaluateRatio() const
{
    // Initialisation des threads
    vector<pair<thread *, float *>> threads;

    // Calculs des distances
    Coordinates coord1 = Field::first();
    do {
        if (field[coord1] == is_usable) {
            // On calcule et additionne le ratio pour aller vers chacun des successeurs
            float *ratio = new float;
            thread *my_thread = new thread(TParcelRatios, coord1, ratio, this);
            threads.push_back(make_pair(my_thread, ratio));
        }
    } while (field.nextCoordinates(&coord1));

    float total_ratio = 0.0;
    for (pair<thread *, float *> thread_result : threads) {
        thread_result.first->join();
        total_ratio += (*thread_result.second);
#if DEBUG_EVALUATION
        cout << "On ajoute " << (*thread_result.second) << ", le ratio total est " << total_ratio << endl;
#endif
        delete thread_result.second;
    }

    //    for(vector<pair<thread*, float*>>::iterator it(threads.end()); it != threads.begin(); --it ){
    //        pair<thread*, float*>  thread_result;
    //        thread_result.first->join();
    //        total_ratio += (*thread_result.second);
    //#if DEBUG_EVALUATION
    //        cout << "On ajoute "<< (*thread_result.second)<< ", le ratio total est "<< total_ratio<< endl;
    //#endif
    //        delete thread_result.second;
    //    }

    return total_ratio;
}

//@}
/// #########################
/// 	Threads
/// #########################
//@{

void TParcelRatios(Coordinates coord, float *ratio, const Resolution *res)
{
    (*ratio) = 0.0;
    cout << "Le ratio de " << coord << " au début est " << (*ratio) << endl;
    Coordinates coord2(coord);
    // On commence à la coordonnée suivante de celle courante
    while (res->field.nextCoordinates(&coord2)) {
        if (res->field[coord2] == is_usable) {
            float ratio_c1_goto_c2 = res->manhattanRatioBetween2Parcels(coord, coord2);
            (*ratio) += 2.0 * ratio_c1_goto_c2; // @see on pourrait faire un décalage de bit
        }
    }
    cout << "Le ratio de " << coord << " à la fin est " << (*ratio) << endl;
}

//@}
