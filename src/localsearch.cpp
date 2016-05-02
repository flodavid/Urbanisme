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

LocalSearch::LocalSearch(unsigned int nbCols, unsigned int nbRows, const Parameters* _params):
    params(*_params)
{
    field= new Field(nbCols, nbRows);
}

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
    
    
    // On définit les parcelles qui sont utilisables et celles qui ne le sont pas
    field->defineUsables(params.get_serve_distance());
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
    map<Coordinates, int> neighbour_occur;
    map<Coordinates, int>::iterator it;

    Coordinates& coord= Field::first();

    do {
        if (field->at(coord) == is_road) {
            for (Coordinates& neighbour : *(field->getNeighbourParcels(coord)) ){
                it= neighbour_occur.find(neighbour);
                if (it == neighbour_occur.end()) {
                    neighbour_occur[neighbour] = 0;
                }
                ++(neighbour_occur[neighbour]);
            }
        }
    } while(field->nextCoordinates(&coord));

    delete &coord;
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
