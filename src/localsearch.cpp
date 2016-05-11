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
    field(_field), params(*_params)
{
    eval= new Evaluation(*field, params);
}

//LocalSearch::LocalSearch(unsigned int nbCols, unsigned int nbRows, const Parameters* _params):
//    params(*_params)
//{
//    field= new Field(nbCols, nbRows);
//}

LocalSearch::LocalSearch(const LocalSearch& other):
    field(other.field), params(other.params ), eval(other.eval)
{
}

//@}
/// #########################
///      Recherche locale
/// #########################
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
        if (in_out_1.col != in_out_2.col) {
            horizontal_roads(in_out_1, in_out_2);
            if (in_out_1.row != in_out_2.row) {
                field->add_road(in_out_1);
            }
        }
        vertical_roads(in_out_1, in_out_2);
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

void LocalSearch::vertical_roads(Coordinates &in_out_1, Coordinates &in_out_2)
{
    in_out_1.row= oneStep(in_out_1.row, in_out_2.row);
    if(in_out_1.row != in_out_2.row) {
        field->add_road(in_out_1);
        vertical_roads(in_out_1, in_out_2);
    }
}

void LocalSearch::horizontal_roads(Coordinates &in_out_1, Coordinates &in_out_2)
{
    in_out_1.col= oneStep(in_out_1.col, in_out_2.col);
    if(in_out_1.col != in_out_2.col) {
        field->add_road(in_out_1.col, in_out_1.row);
        horizontal_roads(in_out_1, in_out_2);
    }
}


list<Path*>* LocalSearch::getPaths(const Coordinates &coord1, const Coordinates &coord2)
{
    list<Path*> * paths= new list<Path*>;

    if (coord1 == coord2){
        clog << "STOP";
        Path* path= new Path();
        paths->push_back(path);
    } else {
        if (coord1.col != coord2.col){
            Coordinates coord1_col_modif= coord1;
            coord1_col_modif.col= oneStep(coord1.col, coord2.col);
            if ( coord1_col_modif == coord2 || field->at(coord1_col_modif) != is_road) {
                clog << "-";
                list<Path*> * paths_col= getPaths(coord1_col_modif, coord2);
                for (Path* path : *paths_col){
                    path->push_front(coord1_col_modif);
                    paths->push_back(path);
                }
                delete paths_col;
//                clog<< ";"<< endl;
            } else {
                clog << "+"<< endl;
            }
        }

        if (coord1.row != coord2.row){
            Coordinates coord1_row_modif= coord1;
            coord1_row_modif.row= oneStep(coord1.row, coord2.row);

            if (coord1_row_modif == coord2 || field->at(coord1_row_modif) != is_road) {
                clog << "|";
                list<Path*> * paths_row= getPaths(coord1_row_modif, coord2);
                for (Path* path : *paths_row){
                    path->push_front(coord1_row_modif);
                    paths->push_back(path);
                }
                delete paths_row;
//                clog<< ";"<< endl;
            } else {
                clog << "+"<< endl;
            }
        }
    }

    return paths;
}

float LocalSearch::gainPath(Path *path) const
{
    eval->initRoadDistances();
    eval->evaluateRatio();
    float eval_before= eval->get_avgAccess();

    Field& tmp_field= eval->get_field();
    Field save_field= tmp_field;
    for (const Coordinates& coord_road : *path) {
        tmp_field.add_road(coord_road);
    }
    eval->initRoadDistances();
    float eval_after= eval->evaluateRatio();

    // Restauration de la surface
    eval->set_field(&save_field);

    return  eval_before - eval_after;
}

bool LocalSearch::addRoadUsable() const
{
    Coordinates coord_min(-1,-1);

    Coordinates& coord= Field::first();

    int gain_max= 0;

    do {
        if (field->getNeighbourRoads(coord)->size() > 0) {

            unsigned nb_roads_neighbours= field->getServingRoads(coord, params.get_serve_distance())->size();
            unsigned nb_parcels_neighbours= field->getCloseUnusableParcels(coord, params.get_serve_distance())->size();

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

        return true;
    } else {
        clog << "Plus aucune route viable pour maximiser le nombre d'exploitables"<< endl;
        return false;
    }
}

bool LocalSearch::addRoadsAccess(unsigned nbToAdd)
{

//    cout << "Valeur de road_distances_are_initiated : "<< eval->road_distances_are_initiated<< endl;
    // Evaluation
    if (!eval->road_distances_are_initiated) {
        eval->initRoadDistances();
    }

    Coordinates& coord= Field::first();

    float gain_max= 0.0;
    Path* best_path= new Path;

    do {
        if (field->at(coord) == is_road) {
            list<Coordinates>* neighbour_roads= field->getNeighbourRoads(coord);

            list<Coordinates>* accessible_roads= field->getCloseRoads(coord, nbToAdd +1);
            clog << "NOMBRE DE ROUTES ACCESSIBLES "<< coord<<" : "<< accessible_roads->size();

            list<Coordinates> accessible_roads_clean;
            for(const Coordinates& accessible_road : *accessible_roads) {
                if ( eval->parcelsRoadDistance(coord, accessible_road) > coord.manhattanDistance(accessible_road)
                     && find(neighbour_roads->begin(), neighbour_roads->end(), accessible_road) == neighbour_roads->end()) {
                    accessible_roads_clean.push_back(accessible_road);
                }
            }
            delete accessible_roads;
            clog << ", APRES NETTOYAGE : "<< accessible_roads_clean.size()<< " : "<< accessible_roads_clean<<endl;

            for(const Coordinates& accessible_road : accessible_roads_clean) {
                list<Path*>* possible_paths= getPaths(coord, accessible_road);
                clog << "\tNombre de chemins pour aller à "<< accessible_road<< " : "<< possible_paths->size()<< endl;
                for (Path* path: *possible_paths){
                    float gain= gainPath(path);
                    cout << "Gain potentiel "<< gain<< endl;
                    if (gain > gain_max){
                        cout << " !!! Un chemin viable pour maximiser l'accessibilité de "<< gain<< " trouvé"<< endl;
                        delete best_path;
                        best_path= path;
                        gain_max= gain;
                    }
                }
            }
        }

    } while(field->nextCoordinates(&coord));
    delete &coord;

    if ( !best_path->empty()) {
        clog << "Chemin viable pour maximiser l'accessibilité de "<< gain_max<< " trouvé"<< endl;
        for (const Coordinates& coord_road : *best_path) {
#if DEBUG_ADD_ACCESS_ROAD
            cout << " Ajout de la route "<< coord_road<< " pour augmenter l'accessibilité"<< endl;
#endif
            field->add_road(coord_road);
        }

        // On définit les parcelles qui sont utilisables et celles qui ne le sont pas
        field->updateUsables(params.get_serve_distance());

        delete best_path;

        return true;
    } else {
        delete best_path;
        cerr << "Aucun chemin viable pour maximiser l'accessibilité"<< endl;
        return false;
    }

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
