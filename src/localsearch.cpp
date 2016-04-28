#include "localsearch.h"

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
