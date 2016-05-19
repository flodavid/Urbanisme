#include "localsearch.h"

#include <list>
#include <map>

typedef std::list<Coordinates> Path;

using namespace std;

/// #########################
///      Constructeurs
/// #########################
//@{

LocalSearch::LocalSearch(Field* _field, const Parameters* _params):
    field(_field), params(*_params), eval(*field, params)
{
//    eval= new Evaluation(*field, params);
}

LocalSearch::LocalSearch(const LocalSearch& other):
    field(other.field), params(other.params ), eval(other.eval)
{
}

LocalSearch::~LocalSearch()
{
//    delete eval;
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
        field->add_road(InOut1);
        vertical_roads(InOut1, InOut2);
    }
}

void LocalSearch::horizontal_roads(Coordinates &InOut1, const Coordinates &InOut2)
{
    InOut1.col= oneStep(InOut1.col, InOut2.col);
    if(InOut1.col != InOut2.col) {
        field->add_road(InOut1.col, InOut1.row);
        horizontal_roads(InOut1, InOut2);
    }
}

/// Relie deux E/S sur des bords parallèles, aves deux coudes

void LocalSearch::verticalElbows(Coordinates &InOut1, const Coordinates &InOut2)
{
    int elbow_height= rand()%((int)(field->get_height()) -2) +1;
    cout << "Coude placé à hauteur "<< elbow_height<< endl;
    Coordinates inter_coord(InOut1);
    inter_coord.row= elbow_height;
    vertical_roads(InOut1, inter_coord);
    field->add_road(InOut1);
    horizontal_roads(inter_coord, InOut2);
    field->add_road(inter_coord);
    vertical_roads(inter_coord, InOut2);
}

void LocalSearch::horizontalElbows(Coordinates &InOut1, const Coordinates &InOut2)
{
    int elbow_width= rand()%((int)(field->get_width()) -2) +1;
    cout << "Coude placé à largeur "<< elbow_width<< endl;
    Coordinates inter_coord(InOut1);
    inter_coord.col= elbow_width;
    horizontal_roads(InOut1, inter_coord);
    field->add_road(InOut1);
    vertical_roads(inter_coord, InOut2);
    field->add_road(inter_coord);
    horizontal_roads(inter_coord, InOut2);
}

/// Création de chemins

void LocalSearch::add_path(Path *path)
{
    for (const Coordinates& coord_road : *path) {
#if LOGS_ADD_ACCESS_ROAD
        clog << " Ajout de la route "<< coord_road<< " pour augmenter l'accessibilité"<< endl;
#endif
        field->add_road(coord_road);
    }

    // On définit les parcelles qui sont utilisables et celles qui ne le sont pas
    field->updateUsables(params.get_serve_distance());
}

float LocalSearch::paveRoad(Path* path, float gainPath)
{
    if ( !path->empty()) {
#if LOGS_ADD_ACCESS_ROAD
        clog << "Chemin viable, pour maximiser l'accessibilité de "<< gain_max
             << " par route, trouvé"<< endl;
#endif
        add_path(path);

        delete path;

        eval.evaluateTotalUsable();
        eval.evaluateRatio();

        return gainPath;
    } else {
        delete path;
        clog << "Aucun chemin viable pour maximiser l'accessibilité"<< endl;
        return 0.0;
    }

}

//@}
/// ################################################
///     Création et améliorations d'une solution
/// ################################################
//@{

void LocalSearch::initSolution()
{
    list<Coordinates>& ins_outs= field->get_insOuts();

    Coordinates in_out_1= ins_outs.front();
    ins_outs.pop_front();
    ins_outs.push_back(in_out_1);
    Coordinates in_out_2= ins_outs.front();

    cout << "E/S 1 : "<< in_out_1<< "; E/S 2 : "<< in_out_2<< endl;

    if (in_out_1.col == 0 || in_out_1.col == (int)field->get_width() -1) {
        if (in_out_1.col + in_out_2.col +1 == (int)field->get_width()){
            horizontalElbows(in_out_1, in_out_2);
        } else {
            if (in_out_1.col != in_out_2.col) {
                horizontal_roads(in_out_1, in_out_2);
                if (in_out_1.row != in_out_2.row) {
                    field->add_road(in_out_1);
                }
            }
            vertical_roads(in_out_1, in_out_2);
        }
    } else {
        if (in_out_1.row + in_out_2.row +1 == (int)field->get_height()){
            verticalElbows(in_out_1, in_out_2);
        } else {
            if (in_out_1.row != in_out_2.row) {
                vertical_roads(in_out_1, in_out_2);
                if (in_out_1.col != in_out_2.col) {
                    field->add_road(in_out_1);
                }
            }
            horizontal_roads(in_out_1, in_out_2);
        }
    }

//    field->defineUsables(params.get_serve_distance());
    field->updateUsables(params.get_serve_distance());
}

list<Path*>* LocalSearch::getPaths(const Coordinates &coord1, const Coordinates &coord2)
{
    list<Path*> * paths= new list<Path*>;

    if (coord1 == coord2) {
        Path* path= new Path();
        paths->push_back(path);
    } else {
        if (coord1.col != coord2.col) {
            Coordinates coord1_col_modif= coord1;
            coord1_col_modif.col= oneStep(coord1.col, coord2.col);

            if ( field->at(coord1_col_modif) != is_road) {
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

            if ( field->at(coord1_row_modif) != is_road) {
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
    }

    return paths;
}

float LocalSearch::gainPath(Path *path)
{
    if (! eval.road_distances_are_initiated){
        eval.initRoadDistances();
        eval.evaluateRatio();
        cerr << "LES DISTANCES PAR ROUTE DEVRAIENT ETRE INITIALISEES"<< endl<< endl;
    }

    float eval_before= eval.get_avgAccess();

    Evaluation tmp_eval= eval;

    Field& tmp_field= tmp_eval.get_field();
    Field save_field= tmp_field;
    for (const Coordinates& coord_road : *path) {
        tmp_field.add_road(coord_road);
    }
    tmp_eval.initRoadDistances();
    tmp_eval.evaluateRatio();
    float eval_after= tmp_eval.get_avgAccess();

    // Restauration de la surface
//    delete &(eval.get_field());
    eval.set_field(save_field);

    return  eval_before - eval_after;
}

int LocalSearch::addRoadUsable() const
{
    Coordinates coord_min(-1,-1);

    Coordinates& coord= Field::first();

    int gain_max= 0;

    do {
        list<Coordinates>* neighours= field->getNeighbourRoads(coord);
        if ( neighours->size()> 0) {
            list<Coordinates>* serving_roads= field->getServingRoads(coord, params.get_serve_distance());
            list<Coordinates>* close_unusable_parcels= field->getCloseUnusableParcels(coord, params.get_serve_distance());
            unsigned nb_roads_neighbours= serving_roads->size();
            unsigned nb_parcels_neighbours= close_unusable_parcels->size();

            delete serving_roads;
            delete close_unusable_parcels;

#if DEBUG_ADD_USABLE_ROAD
            cout << "\t Test de "<< coord;
            cout << " NB R : "<< nb_roads_neighbours<<" ; Nb P : "<< nb_parcels_neighbours;
#endif

            /// TODO est-ce utile de soustraire le nombre de voisin, mettre un coef, ... ? OUI, ça a une utilité, pour éviter de coller 2 routes
            int ratio=  (nb_parcels_neighbours) - (nb_roads_neighbours/2);

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

    } while(field->nextCoordinates(&coord));
    delete &coord;

    if ( !(coord_min == Coordinates(-1,-1))) {
#if DEBUG_ADD_USABLE_ROAD
        cout << "La première parcelle avec le plus de parcelles voisines et le moins de routes voisines est "
             << coord_min<< " avec un ratio "<< gain_max<< endl;
#endif
        field->add_road(coord_min);

        // On définit les parcelles qui sont utilisables et celles qui ne le sont pas
        field->updateUsables(params.get_serve_distance());

        return gain_max;
    } else {
        clog << "Plus aucune route viable pour maximiser le nombre d'exploitables"<< endl;
        return -1;
    }
}

float LocalSearch::addRoadsAccess(unsigned nbToAdd)
{
    // Evaluation
    if (!eval.road_distances_are_initiated) {
        eval.initRoadDistances();
    }

    Coordinates& coord= Field::first();

    float gain_max= 0.0;
    Path* best_path= new Path;

    do {
        if (field->at(coord) == is_road) {
            list<Coordinates>* accessible_roads= field->getCloseRoads(coord, nbToAdd +1);

#if DEBUG_ADD_ACCESS_ROAD
            clog << "NOMBRE DE ROUTES ACCESSIBLES "<< coord<<" : "<< accessible_roads->size();
#endif

            list<Coordinates>* neighbour_roads= field->getNeighbourRoads(coord);
            list<Coordinates> accessible_roads_clean;
            for(const Coordinates& accessible_road : *accessible_roads) {
                if ( eval.parcelsRoadDistance(coord, accessible_road) > coord.manhattanDistance(accessible_road)
                     && find(neighbour_roads->begin(), neighbour_roads->end(), accessible_road) == neighbour_roads->end()) {
                    accessible_roads_clean.push_back(accessible_road);
                }
            }
            delete neighbour_roads;
            delete accessible_roads;
#if DEBUG_ADD_ACCESS_ROAD
            clog << ", APRES NETTOYAGE : "<< accessible_roads_clean.size()<< " : "
                 << accessible_roads_clean<<endl;
#endif

            for(const Coordinates& accessible_road : accessible_roads_clean) {
                list<Path*>* possible_paths= getPaths(coord, accessible_road);
#if DEBUG_ADD_ACCESS_ROAD
                clog << "\tNombre de chemins pour aller à "<< accessible_road<< " : "
                     << possible_paths->size()<< endl;
#endif
                for (Path* path: *possible_paths){
                    float gain= gainPath(path) / (float)(path->size());
#if DEBUG_ADD_ACCESS_ROAD_LIGHT
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

    } while(field->nextCoordinates(&coord));
    delete &coord;

    return paveRoad(best_path, gain_max);
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
