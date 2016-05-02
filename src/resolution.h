#ifndef RESOLUTION_H
#define RESOLUTION_H

#include <vector>
#include <list>

#include "Engine/Parameters.h"
#include "evaluation.h"

/**
 * @brief The Resolution class
 * Elle contient l'intégralité des données du problème et est chargée de lancer les algos,
 * de stocker les solutions et de construire le front pareto résultant.
 */
class Resolution
{
private:
    unsigned nb_cols, nb_rows;
    Parameters params;

    std::vector<Evaluation*> pareto_evals;

public:
    /**
     * Constructeur d'une résolution à partir de tous les paramètres du problème
     * @param nbCols Largeur de la surface
     * @param nbRows Hauteur de la surface
     * @param serveDistance Distance de desserte des parcelles par les routes
     * @param roadsWidth Largeur des routes
     */
    Resolution(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth);
    /**
     * Constructeur d'une résolution à partir de tous les paramètres du problème, avec une liste d'entrées et sorties
     * @param nbCols Largeur de la surface
     * @param nbRows Hauteur de la surface
     * @param serveDistance Distance de desserte des parcelles par les routes
     * @param roadsWidth Largeur des routes
     * @param ins_outs Liste d'entrées/sorties de la surface
     */
    Resolution(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, std::list<Coordinates>& ins_outs);
};

#endif // RESOLUTION_H
