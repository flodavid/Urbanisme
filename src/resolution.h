#ifndef RESOLUTION_H
#define RESOLUTION_H

#include <list>

#include "Engine/parameters.h"
#include "evaluation.h"
#include "localsearch.h"

/**
 * @brief La classe effectuant l'ensemble de la résolution et le front Pareto
 * Elle contient l'intégralité des données du problème et est chargée de lancer les algos,
 * de stocker les solutions et de construire le front pareto résultant.
 */
class Resolution
{
private:
    Parameters params;

    LocalSearch localSearch;

    std::list<Evaluation> pareto_evals;

    // Calulated data
    unsigned nbCells;

public:
    /* Constructeurs */
    /**
     * Constructeur d'une résolution à partir de tous les paramètres du problème, avec une liste d'entrées et sorties
     * @param nbCols Largeur de la surface
     * @param nbRows Hauteur de la surface
     * @param serveDistance Distance de desserte des parcelles par les routes
     * @param roadsWidth Largeur des routes
     * @param ins_outs Liste d'entrées/sorties de la surface
     */
    Resolution(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, std::list<Coordinates>& ins_outs);

    /**
     * Construit l'instance en recopiant une surface et des paramètres existants
     * @param field
     * @param _params
     */
    Resolution(const Field& field, const Parameters& _params);

    ~Resolution();

    /* Evaluations */
    /**
     * Evalue les deux objectifs
     */
    void evaluateBothObjectives();

    /**
     * Effectue une propagation à partir de la solution courante :
     * toutes les solutions précédentes dominées par la solution sont supprimées
     * @param eval Evaluation courante
     * @return le nombre de solutions supprimées
     */
    int spread(const Evaluation &eval);

    /* Recherche locale */
    /**
     * Exécute la recherche locale permettant de maximiser le nombre de parcelles exploitables
     * @see Utiliser un signal pour mettre à jour l'affichage pendant les recherche ?
     */
    Field & localSearchUsableObjective(unsigned maxRoadsToAdd = 10);
    /**
     * Exécute la recherche locale permettant de maximiser l'accessibilité
     * @see Utiliser un signal pour mettre à jour l'affichage pendant les recherche ?
     */
    Field & localSearchAccessObjective(unsigned maxPathsToAdd = 1);

    /* Résolution */
    Field &initResolution();

    Field & launchResolution();

    /* Export */

    bool trySaveParetoToTxt(std::string fileName) const;
};

#endif // RESOLUTION_H
