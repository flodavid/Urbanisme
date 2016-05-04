#include "localsearch.h"

#include <list>
#include <map>

using namespace std;

/// #########################
///      Constructeurs
/// #########################
//@{

LocalSearch::LocalSearch(Field* _field, const Parameters* _params):
    field(_field), params(*_params)
{

}

//LocalSearch::LocalSearch(unsigned int nbCols, unsigned int nbRows, const Parameters* _params):
//    params(*_params)
//{
//    field= new Field(nbCols, nbRows);
//}

LocalSearch::LocalSearch(const LocalSearch& other):
    field(other.field), params(other.params ), eval(other.eval) // TODO Attention, il n'y a pas de constructeur par recopie de eval
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
            field->add_road(in_out_1);
        }
        vertical_roads(in_out_1, in_out_2);
    } else {
        if (in_out_1.row != in_out_2.row) {
            vertical_roads(in_out_1, in_out_2);
            field->add_road(in_out_1);
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

void LocalSearch::addRoad()
{
    Coordinates coord_min(-1,-1);

    Coordinates& coord= Field::first();

    int gain_max= -20;

    do {
        if (field->getNeighbourRoads(coord)->size() > 0) {
            cout << "\t Test de "<< coord;

            unsigned nb_roads_neighbours= field->getServingRoads(coord, params.get_serve_distance())->size();
            unsigned nb_parcels_neighbours= field->getNeighbourUnusableParcels(coord, params.get_serve_distance())->size();

            cout << " NB R : "<< nb_roads_neighbours<<" ; Nb P : "<< nb_parcels_neighbours;

            /// TODO est-ce utile de soustraire le nombre de voisin, mettre un coef, ... ?
            int ratio=  (nb_parcels_neighbours) /*- (nb_roads_neighbours)*/;

            if (ratio > gain_max) {
                cout <<endl<< coord << " a "<< nb_roads_neighbours<< " routes desservant"
                     " et "<< nb_parcels_neighbours<< " parcelles voisines"<< endl;
                gain_max=  ratio;
                coord_min= coord;
            }
            cout << endl;
        }
    } while(field->nextCoordinates(&coord));
    delete &coord;

    if ( !(coord_min == Coordinates(-1,-1))) {
        cout << "La première parcelle avec le plus de parcelles voisines et le moins de routes voisines est "
             << coord_min<< " avec un ratio "<< gain_max<< endl;
        field->add_road(coord_min);
    } else  cerr << "Aucune route viable"<< endl;

    // On définit les parcelles qui sont utilisables et celles qui ne le sont pas
    field->updateUsables(params.get_serve_distance());
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
