#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include "Engine/Field.h"
#include "Engine/Parameters.h"
#include "evaluation.h"

/**
 * @brief La classe chargée d'effectuer des recherches locales sur les différentes solutions
 * TODO ajouter les entrées et sorties
 */
class LocalSearch
{
private:
    Field* field;
    const Parameters& params;

    Evaluation eval;

public:
    /**
     * Un chemin est une suite de coordonnées qu'on suppose adjacentes
     */
    typedef std::list<Coordinates> Path;

    /**
     * Constructeur d'une recherche locale à partir d'une surface existante et
     * des paramètres des routes.
     * La surface sera modifiée
     * @param _field Surface déjà instanciée
     * @param _params Paramètres des routes
     */
    LocalSearch( Field* _field, const Parameters* _params );

    /**
     * Constructeur d'une recherche locale par recopie
     * La surface de l'autre instance est recopiée
     * @param other Instance à recopier
     */
    LocalSearch(const LocalSearch& other);

    ~LocalSearch();

    /* Getters */
    /**
     * Accesseur sur la surface de l'Evaluation
     * @return Une référence sur la surface, modifiable
     */
    Field& get_field() { return eval.get_field(); }
    /**
     * Accesseur sur l'Evaluation
     * @return  Une référence sur l'évaluation, modifiable
     */
    Evaluation& get_evaluation() { return eval; }


    /* Placement de routes */
private:
    void vertical_roads(Coordinates& InOut1, const Coordinates &InOut2);
    void horizontal_roads(Coordinates& InOut1, const Coordinates& InOut2);

    // Création de coudes avec les routes
    void verticalElbows(Coordinates& InOut1, const Coordinates& InOut2);
    void horizontalElbows(Coordinates& InOut1, const Coordinates& InOut2);

    /**
     * Ajoute un ensemble de routes, sensées former un chemin
     * Une mise à jour des parcelles exploitables est effectuée
     * @param path
     */
    void addPath(Path* path);

    /**
     * Transforme les cellules du chemin en routes et recalcule les évaluations
     * après l'opération
     * @return Le gain en accessibilité
     */
    bool tryPaveRoad(Path* path);

    /*  Algorithm */
    /* = Initialisation = */

    /* Création et améliorations d'une solution */
    /**
     * Relie deux entrées et sorties entre elles
     */
    void createRoadsBetween(Coordinates &inOut1, const Coordinates &inOut2);

public:
    /**
     * Relie les entrées et sorties entre elles, de manière directe
     */
    void initSolution();

    /* = Improvments = */
    /**
     * Cherche tous les chemins "directs" possibles entre deux routes.
     * Ces chemins ne doivent pas contenir de routes aller de manière directe d'un point à l'autre,
     * pas de "détours" pour éviter une route.
     * @param coord1 Coordonnées de la première route
     * @param coord2 Coordonnées de la seconde route
     * @return Un pointeur sur une liste de pointeurs sur les différents chemin possibles
     */
    std::list<Path*>* getPaths(const Coordinates& coord1, const Coordinates& coord2);
    /**
     * Calcule le gain hypotéthique en accessibilité lors de l'ajout d'un chemin donné
     * @param path Chemin testé (liste de routes)
     * @return Gain en accessibilité lors de l'ajout du chemin, nul si le chemin et vide
     */
    float gainPath(Path* path);
    /**
     * Tente l'ajout d'une route avec pour objectif d'augmenter le nombre de parcelles exploitables.
     * @return faux si aucune route n'a pu être ajoutée car il n'y a aucun placement de route
     * permettant d'obtenir une solution réalisable avec un nombre de parcelles exploitables
     * supérieur ou égal au nombre courant.
     */
    int addRoadUsable() const;
    /**
     * Tente l'ajout d'un chemin (série de routes) avec pour objectif d'améliorer l'accessibilité.
     * @return faux si aucune route n'a pu être ajoutée car il n'y a aucun placement de chemin
     * permettant d'obtenir une solution réalisable avec une accessibilité meilleures ou équivalente.
     */
    float addRoadsAccess(unsigned nbToAdd);
};

/**
 * Rapproche la valeur de coordinate1 d'une unité de coordinate2
 * @param coordinate1 Coordonnée de départ
 * @param coordinate2 Coordonnée vers laquelle on tend
 * @return coordinate1 incrémenté ou décrémenté de 1
 */
int oneStep(int coordinate1, int coordinate2);

#endif // LOCALSEARCH_H
