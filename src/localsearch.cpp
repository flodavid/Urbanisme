#include "localsearch.h"

using namespace std;

LocalSearch::LocalSearch(Field* _field, const Parameters* _params):
    field(_field), params(*_params)
{

}

LocalSearch::LocalSearch(unsigned nbCols, unsigned nbRows, const Parameters* _params):
    params(*_params)
{
    field= new Field(nbCols, nbRows);
}

LocalSearch::LocalSearch(const LocalSearch& other):
    params(other.params ), eval(other.eval) // TODO Attention, il n'y a pas de constructeur par recopie de eval
{
    field= &eval->field;// TODO voir si on fait comme ça
}

#include <list>

void LocalSearch::initSolution()
{
    list<Coordinates>& ins_outs= field->get_insOuts();

    Coordinates in_out_1= ins_outs.front();
    ins_outs.pop_front();
    ins_outs.push_back(in_out_1);
    Coordinates in_out_2= ins_outs.front();

    cout << "E/S 1 : "<< in_out_1<< "; E/S 2 : "<< in_out_2<< endl;

    if (in_out_1.col == 0 || in_out_1.col == (int)field->get_width()) {
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
//    else {
//        // Si je ne suis pas arrivé, je pose une dernière route "verticale"
//        if (in_out_1.col != in_out_2.col) {
//            field->add_road(in_out_1.col, in_out_1.row);
//        }
//    }
}

void LocalSearch::horizontal_roads(Coordinates &in_out_1, Coordinates &in_out_2)
{
    in_out_1.col= oneStep(in_out_1.col, in_out_2.col);
    if(in_out_1.col != in_out_2.col) {
        field->add_road(in_out_1.col, in_out_1.row);
        horizontal_roads(in_out_1, in_out_2);
    }
//    else {
//        // Si je ne suis pas arrivé, je pose une dernière route "horizontale"
//        if (in_out_1.row != in_out_2.row) {
//            field->add_road(in_out_1.col, in_out_1.row);
//        }
//    }
}

int oneStep(int coordinate1, int coordinate2)
{
    if (coordinate1 - coordinate2 < 0) {
        return coordinate1 +1;
    } else if (coordinate1 != coordinate2) {
        return coordinate1 -1;
    } else return coordinate1;
}
