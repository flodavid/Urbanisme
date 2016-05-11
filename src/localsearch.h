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

    Evaluation* eval;

public:
    /**
     * Constructeur d'une recherche locale à partir d'une surface existante et
     * des paramètres des routes.
     * La surface sera modifiée
     * @param _field Surface déjà instanciée
     * @param _params Paramètres des routes
     */
    LocalSearch( Field* _field, const Parameters* _params );
////     /**
////      * Constructeur d'une recherche locale à partir de la taille de la surface et
////      * des paramètres des routes
////      * Une surface est crée
////      * @param nbCols Largeur de la surface
////      * @param nbRows Hauteur de la surface
////      * @param _params Paramètres des routes
////      */
////     LocalSearch( unsigned int nbCols, unsigned int nbRows, const Parameters* _params );

    /**
     * Constructeur d'une recherche locale par recopie
     * La surface de l'autre instance est recopiée
     * @param other Instance à recopier
     */
    LocalSearch(const LocalSearch& other);

    /*  Recherche locale */
    /**
     * Relie les entrées et sorties entre elles, de manière directe
     */
    void initSolution();

private:
    void vertical_roads(Coordinates& in_out_1, Coordinates& in_out_2);
    void horizontal_roads(Coordinates& in_out_1, Coordinates& in_out_2);

public:
    typedef std::list<Coordinates> Path;
//    typedef Path std::list<Coordinates>;

    std::list<Path*>* getPaths(const Coordinates& coord1, const Coordinates& coord2);

    float gainPath(Path* path) const;
    /**
     * Tente l'ajout d'une route avec pour objectif d'augmenter le nombre de parcelles exploitables.
     * @return faux si aucune route n'a pu être ajoutée car il n'y a aucun placement de route
     * permettant d'obtenir une solution réalisable avec un nombre de parcelles exploitables
     * supérieur ou égal au nombre courant.
     */
    bool addRoadUsable() const;
    /**
     * Tente l'ajout d'un chemin (série de routes) avec pour objectif d'améliorer l'accessibilité.
     * @return faux si aucune route n'a pu être ajoutée car il n'y a aucun placement de chemin
     * permettant d'obtenir une solution réalisable avec une accessibilité meilleures ou équivalente.
     */
    bool addRoadsAccess(unsigned nbToAdd);

};

/**
 * Rapproche la valeur de coordinate1 d'une unité de coordinate2
 * @param coordinate1 Coordonnée de départ
 * @param coordinate2 Coordonnée vers laquelle on tend
 * @return coordinate1 incrémenté ou décrémenté de 1
 */
int oneStep(int coordinate1, int coordinate2);

#endif // LOCALSEARCH_H
