#ifndef RESOLUTION_H
#define RESOLUTION_H

#include <list>
#include <fstream>
#include <sstream>

#include "Engine/parameters.h"
#include "evaluation.h"
#include "fieldevaluation.h"
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

    // Calulated data
    unsigned nbCells;
    std::list<FieldEvaluation> pareto_evals;

    // Others attributes
    std::ostringstream evaluations_stream;

public:
    /* Constructeurs */
    /**
     * Constructeur d'une résolution à partir de tous les paramètres du problème,
     * avec une liste d'entrées et sorties
     * @param nbCols Largeur de la surface
     * @param nbRows Hauteur de la surface
     * @param serveDistance Distance de desserte des parcelles par les routes
     * @param roadsWidth Largeur des routes
     * @param ins_outs Liste d'entrées/sorties de la surface
     */
    Resolution(unsigned nbCols, unsigned nbRows, unsigned serveDistance, unsigned roadsWidth, std::list<Coordinates>& ins_outs);

    /**
     * Construit l'instance enà partir d'une surface et de paramètres existants
     * @param field Surface à recopier
     * @param _params Paramètres du problème
     */
    Resolution(const Field& field, const Parameters& _params);

    ~Resolution();

    /* Getters */
    /**
     * Compte le nombre de solutions non dominées trouvées jusqu'à présent
     * @return le nombre de solutions non dominées
     */
    inline size_t get_nb_not_dominated() const { return pareto_evals.size(); }

    /* Setters */
    void changeWorkField(Field* _field, bool newField);

private:
    /**
     * Ouvre le fichier des évaluations dominées, sans enregistrer l'ancien
     */
    std::ofstream *openEvaluationsFile(std::string filename_end = "") const;
    /**
     * Vide le buffer d'écriture du fichier d'évaluations
     */
    void emptyEvaluationsFile();

public:
    /* Evaluations */
    /**
     * Evalue les deux objectifs
     */
    void evaluateBothObjectives();

    /**
     * Vérifie si l'évaluation est dominée par l'évaluation d'au moins une
     * des solutions précédemment calculées
     * @param eval Evaluation à traiter
     * @return faux si la solution est dominée par une des solutions présents
     */
    bool isNotDominated(const Evaluation &eval);

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
    FieldEvaluation *localSearchUsableObjective(unsigned maxRoadsToAdd = 10);
    /**
     * Exécute la recherche locale permettant de maximiser l'accessibilité
     * @see Utiliser un signal pour mettre à jour l'affichage pendant les recherche ?
     */
    FieldEvaluation* localSearchAccessObjective(unsigned maxPathsToAdd = 1);

    /* Résolution */
    /**
     * Création de la route qui relie deux entrées/sorties
     * @return
     */
    FieldEvaluation *initResolution();

    /* Export */
    /**
     * Ecrit l'évaluation dans le fichier des évaluations dominées
     */
    void writeDominatedEvaluation(const Evaluation& eval);
    /**
     * @brief trySaveParetoToTxt
     * @param fileName
     * @return vrai si le fichier s'est correctement ouvert
     */
    bool trySaveParetoToTxt(std::string fileName) const;
    /**
     * Exporte les solutions non dominées dans une image jpeg
     * @param dataName
     * @return le nom du fichier image crée
     */
    std::string drawParetoJpeg(std::string dataName) const;

    /**
     * @brief selectSavedField
     * @param ind
     * @return
     */
    FieldEvaluation* trySelectSavedField(unsigned index);
};

#endif // RESOLUTION_H
