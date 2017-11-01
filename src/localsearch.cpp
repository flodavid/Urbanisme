#include "localsearch.h"

#include <list>
#include <map>
#include <omp.h>

typedef std::list<Coordinates> Path;

using namespace std;

/// #########################
///      Constructeurs
/// #########################
//@{

LocalSearch::LocalSearch(Field* _field, const Parameters* _params):
    params(*_params)
{
    fieldeval= new FieldEvaluation(*_field, params);
}

LocalSearch::LocalSearch(const LocalSearch& other):
    params(other.params )
{
    fieldeval= new FieldEvaluation(*(other.fieldeval));
}

LocalSearch::~LocalSearch()
{
}

//@}
/// ################
///     Setters
/// ################
//@{

void LocalSearch::setField(Field *_field)
{
    delete fieldeval;
    fieldeval= new FieldEvaluation(*_field, params);
}

//@}
/// ###########################
///     Placement de routes
/// ###########################
//@{

/// Lignes droites de routes

void LocalSearch::vertical_roads(Coordinates &InOut1, const Coordinates &InOut2)
{
    InOut1.row= oneStep(InOut1.row, InOut2.row);
    if(InOut1.row != InOut2.row) {
        fieldeval->add_road(InOut1);
        vertical_roads(InOut1, InOut2);
    }
}

void LocalSearch::horizontal_roads(Coordinates &InOut1, const Coordinates &InOut2)
{
    InOut1.col= oneStep(InOut1.col, InOut2.col);
    if(InOut1.col != InOut2.col) {
        fieldeval->add_road((unsigned)(InOut1.col), (unsigned)(InOut1.row));
        horizontal_roads(InOut1, InOut2);
    }
}

/// Relie deux E/S sur des bords parallèles, aves deux coudes

void LocalSearch::verticalElbows(Coordinates &InOut1, const Coordinates &InOut2)
{
    int elbow_height= rand()%((int)(fieldeval->get_height()) -2) +1;
    cout << "Coude placé à hauteur "<< elbow_height<< endl;
    Coordinates inter_coord(InOut1);
    inter_coord.row= elbow_height;
    vertical_roads(InOut1, inter_coord);
    fieldeval->add_road(InOut1);
    horizontal_roads(inter_coord, InOut2);
    fieldeval->add_road(inter_coord);
    vertical_roads(inter_coord, InOut2);
}

void LocalSearch::horizontalElbows(Coordinates &InOut1, const Coordinates &InOut2)
{
    int elbow_width= rand()%((int)(fieldeval->get_width()) -2) +1;
    cout << "Coude placé à largeur "<< elbow_width<< endl;
    Coordinates inter_coord(InOut1);
    inter_coord.col= elbow_width;
    horizontal_roads(InOut1, inter_coord);
    fieldeval->add_road(InOut1);
    vertical_roads(inter_coord, InOut2);
    fieldeval->add_road(inter_coord);
    horizontal_roads(inter_coord, InOut2);
}

/// Création de chemins

void LocalSearch::addPath(Path *path)
{
    fieldeval->addRoads(path, params.get_serve_distance());
}

bool LocalSearch::tryPaveRoad(Path* path)
{
    if ( !path->empty()) {
        addPath(path);
        delete path;

        // Mise à jour des valeurs d'évaluation des objectifs
        fieldeval->evaluateTotalUsable();
        fieldeval->evaluateRatio();

        return true;
    } else {
        delete path;
        clog << "Aucun chemin viable pour maximiser l'accessibilité"<< endl;

        return false;
    }

}

//@}
/// ################################################
///     Création et améliorations d'une solution
/// ################################################
//@{

void LocalSearch::createRoadsBetween(Coordinates &inOut1, const Coordinates &inOut2)
{
    // On vérifie si la première E/S est sur un bord vertical ou non
    if (inOut1.col == 0 || inOut1.col == (int)fieldeval->get_width() -1) {
        // On vérifie si l'autre point est sur le bord en face
        if (inOut1.col + inOut2.col +1 == (int)fieldeval->get_width()){
            horizontalElbows(inOut1, inOut2);
        } else {
            // On vérifie que l'autre E/S n'est pas sur le même bord
            if (inOut1.col != inOut2.col) {
                horizontal_roads(inOut1, inOut2);
                if (inOut1.row != inOut2.row) {
                    fieldeval->add_road(inOut1);
                }
            }
            vertical_roads(inOut1, inOut2);
        }
    } else {
        // On vérifie si l'autre point est sur le bord en face
        if (inOut1.row + inOut2.row +1 == (int)fieldeval->get_height()){
            verticalElbows(inOut1, inOut2);
        } else {
            // On vérifie que l'autre E/S n'est pas sur le même bord
            if (inOut1.row != inOut2.row) {
                vertical_roads(inOut1, inOut2);
                if (inOut1.col != inOut2.col) {
                    fieldeval->add_road(inOut1);
                }
            }
            horizontal_roads(inOut1, inOut2);
        }
    }
}

bool LocalSearch::tryInitSolution()
{
    list<Coordinates>& ins_outs= fieldeval->get_insOuts();

    if (ins_outs.size() >=2) {
        Coordinates in_out_1= ins_outs.front();
        ins_outs.pop_front();
        ins_outs.push_back(in_out_1);
        Coordinates in_out_2= ins_outs.front();

        cout << "E/S 1 : "<< in_out_1<< "; E/S 2 : "<< in_out_2<< endl;
        createRoadsBetween(in_out_1, in_out_2);

        fieldeval->resetUsables(params.get_serve_distance());

        return true;
    }
    return false;
}

list<Path*>* LocalSearch::getPaths(const Coordinates &coord1, const Coordinates &coord2)
{
    list<Path*> * paths= new list<Path*>;

    if (coord1 == coord2) {
        Path* path= new Path();
        paths->push_back(path);
    } else {
// 	if (fieldeval->getCloseRoads(coord1, 1)->size() < 2) {
    if (coord1.col != coord2.col) {
        Coordinates coord1_col_modif= coord1;
        coord1_col_modif.col= oneStep(coord1.col, coord2.col);

        if ( fieldeval->at(coord1_col_modif) < is_road) {
        list<Path*> * paths_col= getPaths(coord1_col_modif, coord2);
        for (Path* path : *paths_col){
            path->push_front(coord1_col_modif);
            paths->push_back(path);
        }
        delete paths_col;
        } else if (coord1_col_modif == coord2) {
        paths->push_back(new Path);
        }
    }

    if (coord1.row != coord2.row) {
        Coordinates coord1_row_modif= coord1;
        coord1_row_modif.row= oneStep(coord1.row, coord2.row);

        if ( fieldeval->at(coord1_row_modif) < is_road
        && fieldeval->getCloseRoads(coord1_row_modif, 1)->size() < 2) {
        list<Path*> * paths_row= getPaths(coord1_row_modif, coord2);
        for (Path* path : *paths_row){
            path->push_front(coord1_row_modif);
            paths->push_back(path);
        }
        delete paths_row;
        } else if (coord1_row_modif == coord2) {
        paths->push_back(new Path);
        }
    }
// 	}
    }

    return paths;
}

int LocalSearch::addRoadUsable() const
{
    Coordinates coord_min(-1,-1);

    Coordinates& coord= Field::first();

    int gain_max= 0;

    do {
        list<Coordinates>* neighours= fieldeval->getNeighbourRoads(coord);
        if ( neighours->size()> 0) {
            list<Coordinates>* serving_roads= fieldeval->getServingRoads(coord, params.get_serve_distance());
            list<Coordinates>* close_unusable_parcels= fieldeval->getCloseUnusableParcels(coord, params.get_serve_distance());
            unsigned nb_roads_neighbours= (unsigned)(serving_roads->size());
            unsigned nb_parcels_neighbours= (unsigned)(close_unusable_parcels->size());

            delete serving_roads;
            delete close_unusable_parcels;

#if DEBUG_ADD_USABLE_ROAD
            cout << "\t Test de "<< coord;
            cout << " NB R : "<< nb_roads_neighbours<<" ; Nb P : "<< nb_parcels_neighbours;
#endif

            /// TODO est-ce utile de soustraire le nombre de voisin, mettre un coef, ... ? OUI, ça a une utilité, pour éviter de coller 2 routes
            int ratio= (int)(nb_parcels_neighbours) - (int)(nb_roads_neighbours>>1);

            if (ratio > gain_max) {
#if DEBUG_ADD_USABLE_ROAD
                cout << endl<< coord << " a "<< nb_roads_neighbours<< " routes desservant"
                                                                      " et "<< nb_parcels_neighbours<< " parcelles voisines"<< endl;
#endif
                gain_max=  ratio;
                coord_min= coord;
            }
        }
        delete neighours;

    } while(fieldeval->nextCoordinates(&coord));
    delete &coord;

    if ( !(coord_min == Coordinates(-1,-1)) ) {
#if DEBUG_ADD_USABLE_ROAD
        cout << "La première parcelle avec le plus de parcelles voisines et le moins de routes voisines est "
             << coord_min<< " avec un ratio "<< gain_max<< endl;
#endif
        fieldeval->add_road(coord_min);

        // On définit les parcelles qui sont utilisables et celles qui ne le sont pas
        fieldeval->resetUsables(params.get_serve_distance());

        return gain_max;
    } else {
        clog << "Plus aucune route viable pour maximiser le nombre d'exploitables"<< endl;
        return -1;
    }
}

float LocalSearch::gainPath(Path *path)
{
    FieldEvaluation tmp_eval= *fieldeval;
    Field& tmp_field= tmp_eval;

    ///@see voir comment ne pas utiliser initRoadDistances et si c'est utile de recalculer le ratio
    fieldeval->initRoadDistances();
    float access_before= fieldeval->evaluateRatio();

    // Ajout des routes puis évaluation
    tmp_field.addRoads(path, params.get_serve_distance());
    ///@see voir comment ne pas utiliser initRoadDistances et si c'est utile de recalculer le ratio
    tmp_eval.initRoadDistances();
    float access_after= tmp_eval.evaluateRatio();

    // Restauration de la surface
    tmp_field.removeRoads(path, params.get_serve_distance());

    return access_before - access_after;
}

float LocalSearch::addRoadsAccess(unsigned nbToAdd)
{
    // Evaluation
    if (!fieldeval->road_distances_are_initiated) {
        fieldeval->initRoadDistances();
    }

//    Coordinates& coord= Field::first();

    float gain_max= 0.0;
    Path* best_path= new Path;

    #pragma omp parallel for
    for (int i= 0; i < (int)(fieldeval->getNbParcels()); ++i) {
       Coordinates coord(i%(int)fieldeval->get_width(), i/(int)fieldeval->get_height());
//    do {
#if LOGS_ACCESS_ROAD
    if (coord.col == 0) {
        cout << "Ligne "<< coord.row<< ", gain courant : "<< gain_max<< endl;
    }
#endif

        if (fieldeval->at(coord) == is_road) {
            list<Coordinates>* accessible_roads= fieldeval->getCloseRoads(coord, nbToAdd +1);

#if DEBUG_ADD_ACCESS_ROAD
            clog << "NOMBRE DE ROUTES ACCESSIBLES "<< coord<<" : "<< accessible_roads->size();
#endif
            list<Coordinates>* neighbour_roads= fieldeval->getNeighbourRoads(coord);
            list<Coordinates> accessible_roads_clean;

            for(const Coordinates& accessible_road : *accessible_roads) {
                if ( fieldeval->getRoadDistance(coord, accessible_road) > coord.manhattanDistance(accessible_road)
                     && find(neighbour_roads->begin(), neighbour_roads->end(), accessible_road) == neighbour_roads->end())
                {
                    accessible_roads_clean.push_back(accessible_road);
                }
            }

            delete neighbour_roads;
            delete accessible_roads;
#if DEBUG_ADD_ACCESS_ROAD
            clog << ", APRES NETTOYAGE : "<< accessible_roads_clean.size()<< " : "
                 << accessible_roads_clean<<endl;
#endif
            // On tente de relier chaque route accessible depuis la coordonnée courante,
            // puis on calcul le gain d'accessibilité offert
            for(const Coordinates& accessible_road : accessible_roads_clean) {
                list<Path*>* possible_paths= getPaths(coord, accessible_road);
#if DEBUG_ADD_ACCESS_ROAD
                clog << "\tNombre de chemins pour aller à "<< accessible_road<< " : "
                     << possible_paths->size()<< endl;
#endif
                for (Path* path: *possible_paths) {
                    float gain= gainPath(path) / (float)(path->size());

#if DEBUG_ADD_ACCESS_GAIN
                    clog << "Gain potentiel "<< gain<< " (chemin de longueur "<< path->size()
                         << ") (max : "<< gain_max<< ")"<< endl;
#endif
                    if (gain > gain_max){
                        delete best_path;

                        best_path= path;
                        gain_max= gain;
                    } else {
                        delete path;
                    }
                }
                delete possible_paths;
            }
        }

    }
//    while(fieldeval->nextCoordinates(&coord));
//    delete &coord;

    tryPaveRoad(best_path);
#if LOGS_ADD_ACCESS_ROAD
    if (gain_max > 0.0) {
        clog << "Chemin viable, pour maximiser l'accessibilité de "<< gain_max
             << " par route, trouvé"<< endl;
    }
#endif
    return gain_max;
}

//@}
/// #########################
///    Hors de la classe
/// #########################
//@{

int oneStep(int coordinate1, int coordinate2)
{
    if (coordinate1 - coordinate2 < 0) {
        return coordinate1 +1;
    } else if (coordinate1 != coordinate2) {
        return coordinate1 -1;
    } else return coordinate1;
}

//@}
