#ifndef RESOLUTION_H
#define RESOLUTION_H

#include <vector>
#include <list>

#include "Engine/Parameters.h"
#include "evaluation.h"

class Resolution
{
private:
    unsigned nb_cols, nb_rows;
    Parameters params;

    std::vector<Evaluation*> pareto_evals;

public:
    /**
     * Constructeur d'une résolution à partir de tous les paramètres du problème
     * @param nbCols
     * @param nbRows
     * @param serveDistance
     * @param roadsWidth
     */
    Resolution(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth);
    /**
     * Constructeur d'une résolution à partir de tous les paramètres du problème, avec une liste d'entrées et sorties
     * @param nbCols
     * @param nbRows
     * @param serveDistance
     * @param roadsWidth
     */
    Resolution(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, std::list<Coordinates>& ins_outs);
};

#endif // RESOLUTION_H
