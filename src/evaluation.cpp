#include "stdafx.h"
#include "evaluation.h"

#include <cassert>
// ##include <pthread.h>
#include <thread>
#include <map>

using namespace std;

Evaluation::Evaluation(const Field &_field, const Parameters &_params) :
    field(_field), params(_params)
{
}

Evaluation::~Evaluation()
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

void Evaluation::set_params(const Parameters &_params)
{
    params = _params;
}

//@}
/// #########################
///     Calculs de données
/// #########################
//@{

unsigned int Evaluation::lengthBy(const Coordinates& testCoord, const Coordinates& dest, list<Coordinates> *visited, unsigned int minDist)
{
    // On applique la recursivité avec la coordonnée envisagée courante
    unsigned dist;
    float current_ratio = getRoadDistance(testCoord, dest);
    if (current_ratio != 0) {
        dist = current_ratio;
#if DEBUG_ROADS_DIST
        cout << "\tIl y a déjà une valeur pour ce trajet : "<< current_ratio<< endl;
#endif
    } else {
        visited->push_back(testCoord);
        dist= calcRoadDistance(testCoord,  dest, visited, minDist);
        visited->pop_back();
            if (dist != UNSIGNED_INFINITY) {
                road_distances[testCoord.row][testCoord.col][dest.row][dest.col] = dist;
#if DEBUG_ROADS_DIST
                cout<< "  Je stocke ";
#endif
            }
#if DEBUG_ROADS_DIST
        cout << "\tLa valeur calculée pour le trajet "<< testCoord<< "->"<< dest<< "est : "<< dist<< endl;
#endif
    }

    return dist;
}

unsigned int Evaluation::testPathBy(const Coordinates& testCoord, const Coordinates& dest, list<Coordinates> *visited, unsigned minDist)
{
    // Recherche de cet élément dans la liste des visités, si il ne s'y trouve pas,
    // c'est qu'on n'est pas déjà passé par lui
    if (find(visited->begin(), visited->end(), testCoord) == visited->end()) {
        unsigned dist= lengthBy(testCoord, dest, visited, minDist);

        // On compare le résultat obtenu avec le minimum courant,
        // on met à jour si nécessaire
        if (dist < minDist) {
            minDist= dist;
        }
    } else {
    #if DEBUG_ROADS_DIST
        cout << "La parcelle "<< testCoord<< " a déjà été visitée" << endl;
    #endif
    }
    
    return minDist;
}


unsigned Evaluation::calcRoadDistance(const Coordinates &coord1, const Coordinates &coord2,
                                            list< Coordinates > *visited, unsigned int dist_max)
{
    assert(field[coord1] >= is_road && field[coord2] >= is_road);

#if DEBUG_ROADS_DIST
    cout << "Calcul de la distance entre " << coord1 << " et " << coord2 << endl;
#endif

    if (coord1 == coord2) {
#if DEBUG_ROADS_DIST
        cout << "Je suis arrivé à " << coord2<< endl;
#endif
        return 0;
    } else {
#if DEBUG_ROADS_DIST
                cout << "***Taille de la liste des visitées : " << visited->size() << endl;
#endif
         if (visited->size() >= dist_max) {
#if DEBUG_ROADS_DIST
             clog<< "ON EST SORTI PLUS TOT"<< endl<< endl;
#endif
             return UNSIGNED_INFINITY;
         } else {
            unsigned min_dist = UNSIGNED_INFINITY;

            list<Coordinates> *neighbour_roads = field.getNeighbourRoads(coord1);
            for (const Coordinates& new_coord : (*neighbour_roads)) {
                min_dist = testPathBy(new_coord, coord2, visited,  min_dist);
            }

            delete neighbour_roads;

#if DEBUG_ROADS_DIST
            cout << "Pour aller de " << coord1 << " à " << coord2
                 << ", il y a un trajet de longueur " << min_dist << endl;
#endif
            if (min_dist != UNSIGNED_INFINITY) {
                // On incrément min_dist, car on se déplace vers le voisin
                return min_dist +1;
            } else {
#if DEBUG_ROADS_DIST
                cout << "\tPas de chemin pour aller de "<< coord1<< " à "<< coord2<< endl;
#endif
                return UNSIGNED_INFINITY;
            }
         }// fin_else(visited->size >= dist_max)
    }
}

unsigned Evaluation::parcelsRoadDistance(const Coordinates &coord1, const Coordinates &coord2)
{
#if DEBUG_ROADS_DIST
    cout << "### Calcul de la distance entre "<< coord1<< " et "<< coord2<< endl
         << "=============================================="<< endl;
#endif
        if (coord1 == coord2) {
#if DEBUG_ROADS_DIST
        clog << "les cases de départ et d'arrivée sont identiques" << endl;
#endif
        return 0;
    }

    unsigned min_dist = UNSIGNED_INFINITY;

    list<Coordinates> *serving_roads_c1 = field.getServingRoads(coord1, params.get_serve_distance());
    list<Coordinates> *serving_roads_c2 = field.getServingRoads(coord2, params.get_serve_distance());

    // Pour chaque route qui dessert chaque parcelle, on calcule le plus court chemin
    // On garde le plus court de ces chemins
    for (const Coordinates& road_c1 : (*serving_roads_c1)) {
        for (const Coordinates& road_c2 : (*serving_roads_c2)) {
            list<Coordinates> empty_visited;
            unsigned int dist = calcRoadDistance(road_c1, road_c2, &empty_visited, UNSIGNED_INFINITY);

            // Si on a trouvé un chemin meilleur que le précédent, si il en existait un,
            // on stocke sa longueur
            if (dist < min_dist) {
                min_dist= dist;
            }
        }
    }

    delete serving_roads_c1;
    delete serving_roads_c2;

    // On devrait trouver un chemin entre les deux parcelles
    if (min_dist == UNSIGNED_INFINITY) {
        cerr << "Impossible de relier"<< coord1<< " et "<< coord2<< " par les routes"
                "(au moins une des 2 parcelle n'a pas de route à proximité (distance < "
             << params.get_serve_distance() << "))" << endl;
    }
#if DEBUG_EVALUATION || DEBUG_ROADS_DIST
    else {
        cout << "### Distance entre "<< coord1<< " et "<< coord2<< " : "<< min_dist +1<< endl
             << endl;
    }
#endif

    return min_dist+1;
}

void Evaluation::initSizeNeighbourhood()
{
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
}

void Evaluation::initCoordNeighbourhoodManhattan(const Coordinates &coord)
{
    if (field[coord] == is_usable) {
#if DEBUG_EVALUATION_LIGHT
        cout << "Premier point : " << coord << endl;
#endif
        // On calcule et additionne le ratio pour aller vers chacun des successeurs
        Coordinates coord2(coord);
        // On commence à la coordonnée suivante de celle courante
        while (field.nextCoordinates(&coord2)) {
            // On calcule la distance que si elle n'a pas déjà été calculée
            if (field[coord2] != is_unusable && road_distances[coord.row][coord.col][coord2.row][coord2.col] == 0) {
                road_distances[coord.row][coord.col][coord2.row][coord2.col] =
                        parcelsRoadDistance(coord, coord2);
            }
        }
    }
}

void Evaluation::initNeighbourhoodManhattan()
{
    initSizeNeighbourhood();

    // Calculs des ratios de distances
    Coordinates& coord1 = Field::first();
    do {
        initCoordNeighbourhoodManhattan(coord1);
    } while (field.nextCoordinates(&coord1));
    delete &coord1;

    road_distances_are_initiated =  true;
}


//@}
/// #########################
///      Evaluations
/// #########################
//@{

unsigned int Evaluation::evaluateTotalUsable()
{
    unsigned nb_usables = 0;
    for (vector<State> row_parcel_state : field) {
        for (State parcel_state : row_parcel_state) {
            assert(parcel_state >= -1 &&  parcel_state < max_state);
            if (parcel_state == is_usable) {
                ++nb_usables;
            }
        }
    }

    // Sauvegarde de la valeur
    nbUsables= nb_usables;

#if DEBUG_EVALUATION
    cout << "Total number of usables parcels : " << nb_usables << endl;
#endif
    assert(nb_usables >= 0 && nb_usables < field.getNbParcels() && "nombre d'exploitables incohérent");

    return nb_usables;
}

float Evaluation::manhattanRatioBetween2Parcels(const Coordinates &p1, const Coordinates &p2) const
{
    unsigned road_distance = getRoadDistance(p1, p2);
    unsigned manhattan_distance = p1.manhattanDistance(p2);
    float ratio = ((float)road_distance) / ((float) manhattan_distance);

#if DEBUG_EVALUATION
    cout << "\tDistance route "<< p1<< "->"<< p2<< " = "<< road_distance<< endl;
    cout << "\tDistance directe "<< p1<< "->"<< p2<< " = "<< manhattan_distance<< endl;
    cout << "\t\tRatio : "<< ratio<< endl;
#endif

    return ratio;
}

float Evaluation::evaluateRatio(unsigned nbUsables)
{
    float total_ratio = 0.0;
    unsigned nb_ratio = 0;

    // Calculs des distances
    Coordinates& coord1 = Field::first();
    do {
        if (field[coord1] == is_usable) {
#if DEBUG_EVALUATION_LIGHT
            cout << "Premier point : " << coord1 << endl;
#endif
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
    delete &coord1;


    cout << "Ratio total : "<< total_ratio<< ", nb ratios : "<< nb_ratio<< endl;

    float average =  total_ratio / ((float) nb_ratio);
    // Sauvegarde de la valeur
    avgAccess= average;

    return average;
}

//@}
/// #########################
/// 	Threads
/// #########################
//@{

//float Evaluation::threadsEvaluateRatio() const
//{
//    // Initialisation des threads
//    vector<pair<thread *, float *>> threads;

//    // Calculs des distances
//    Coordinates coord1 = Field::first();
//    do {
//        if (field[coord1] == is_usable) {
//            // On calcule et additionne le ratio pour aller vers chacun des successeurs
//            float *ratio = new float;
//            thread *my_thread = new thread(TParcelRatios, coord1, ratio, this);
//            threads.push_back(make_pair(my_thread, ratio));
//        }
//    } while (field.nextCoordinates(&coord1));

//    float total_ratio = 0.0;
//    for (pair<thread *, float *> thread_result : threads) {
//        thread_result.first->join();
//        total_ratio += (*thread_result.second);
//#if DEBUG_EVALUATION
//        cout << "On ajoute " << (*thread_result.second) << ", le ratio total est " << total_ratio << endl;
//#endif
//        delete thread_result.second;
//    }

//    //    for(vector<pair<thread*, float*>>::iterator it(threads.end()); it != threads.begin(); --it ){
//    //        pair<thread*, float*>  thread_result;
//    //        thread_result.first->join();
//    //        total_ratio += (*thread_result.second);
//    //#if DEBUG_EVALUATION
//    //        cout << "On ajoute "<< (*thread_result.second)<< ", le ratio total est "<< total_ratio<< endl;
//    //#endif
//    //        delete thread_result.second;
//    //    }

//    return total_ratio;
//}

// TODO A voir le multi-threading
//void TParcelRatios(Coordinates coord, float *ratio, const Evaluation *res)
//{
//    (*ratio) = 0.0;
//    cout << "Le ratio de " << coord << " au début est " << (*ratio) << endl;
//    Coordinates coord2(coord);
//    // On commence à la coordonnée suivante de celle courante
//    while (res->field.nextCoordinates(&coord2)) {
//        if (res->field[coord2] == is_usable) {
//            float ratio_c1_goto_c2 = res->manhattanRatioBetween2Parcels(coord, coord2);
//            (*ratio) += 2.0 * ratio_c1_goto_c2; // @see on pourrait faire un décalage de bit
//        }
//    }
//    cout << "Le ratio de " << coord << " à la fin est " << (*ratio) << endl;
//}

//@}
