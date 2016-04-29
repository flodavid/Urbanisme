#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include "Engine/Field.h"
#include "Engine/Parameters.h"
#include "evaluation.h"

/**
 * @brief The LocalSearch class
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
     * @param params Paramètres des routes
     */
    LocalSearch( Field* _field, const Parameters* _params );
    /**
     * Constructeur d'une recherche locale à partir de la taille de la surface et
     * des paramètres des routes
     * Une surface est crée
     * @param nbCols Largeur de la surface
     * @param nbRows Hauteur de la surface
     * @param params Paramètres des routes
     */
    LocalSearch ( unsigned int nbCols, unsigned int nbRows, const Parameters* _params );
    /**
     * Constructeur d'une recherche locale par recopie
     * La surface de l'autre instance est recopiée
     * @param other Instance à recopier
     */
    LocalSearch(const LocalSearch& other);

    /**
     * Relie les entrées et sorties entre elles, de manière directe
     */
    void initSolution();

private:
    void vertical_roads(Coordinates& in_out_1, Coordinates& in_out_2);
    void horizontal_roads(Coordinates& in_out_1, Coordinates& in_out_2);

};

/**
 * Rapproche la valeur de coordinate1 d'une unité de coordinate2
 * @param coordinate1
 * @param coordinate2
 * @return coordinate1 incrémenté ou décrémenté de 1
 */
int oneStep(int coordinate1, int coordinate2);

#endif // LOCALSEARCH_H