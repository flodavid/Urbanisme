#pragma once

#include <list>

#include "Engine/Field.h"
#include "Engine/Parameters.h"
#include "Engine/coordinates.h"

class Resolution
{
private:
    // Données sur lesquelles on travaille
    Field field;
    Parameters params;

    // Données calculées
    std::vector<std::vector<unsigned>> road_distances;

    // FLAGS
    bool road_distances_are_initiated= false;

public:
    Resolution ( const Field & _field, const Parameters & _params );
    ~Resolution();

    /* Getters */

    /*** Setters */
    /**
     * @brief set_params
     */
    void set_params ( const Parameters& _params );
    /**
     * @brief set_road_distance
     * @deprecated
     */
    void increment_road_distance ( const Coordinates& coord, unsigned value ) __attribute__ ( ( deprecated ) );

    /* Calculs de données */
    /**
     * Initialise la matrice de disance entre les points selon leur distance avec les routes
     *  avec la distance Manhattan
         * @see
     */
    void parcours_cellules();

    /* Fonctions auxiliaires */
    /**
    * Recherche des portions de routes qui peuvent désservir la parcelle
    * @param coord Coordonnées de la parcelle desservir
    * @return une liste de routes distance inférieure ou égale à la distance de desserte
    */
    std::list<Coordinates> *getServingRoads ( const Coordinates& coord ) const;

    /**
    * Recherche des portions de routes qui sont collées à la parcelle courante
    * @param coord Coordonnées de la parcelle
    * @return une liste de routes adjacentes à la parcelle
    */
    std::list<Coordinates> *getNeighbourRoads ( const Coordinates& coord ) const;

    /**
     * Calcule la distance par les routes entre deux cellules, en respectant les paramètres
        * @param coord1 Coordonnée "de départ"
        * @param coord2 Coordonnée "d'arrivée"
     * @return la valeur, entier non signé, de distance la plus courte,
     *   entre les deux coordonnées, en passant par les routes
     */
    unsigned calcRoadDistance ( const Coordinates& coord1, const Coordinates& coord2 ) const;

private:
    /**
     * Calcule la distance entre deux points, en passant de route en route
     * On suppose ques les coordonées courantes sont des routes
     */
    unsigned recCalcRoadDistance ( const Coordinates& coord1, const Coordinates& coord2, std::list<Coordinates> *visited ) const;

public:
    /* Evaluations */
    /**
     * Compte le nombre total de parcelles exploitables sur la surface
     * @return nombre de parcelles exploitables
     */
    unsigned evaluateTotalUsable() const;

    /**
     * Calcule le ratio entre distance par la route et distance Manhattan entre deux positions
         * @param p1 premières coordonnées
         * @param p2 secondes coordonnées
     * @return le ratio, sous forme de flottant
     */
    float manhattanRatioBetween2Parcels ( const Coordinates& p1, const Coordinates& p2 ) const;
    /**
     * Evalue le ratio de toutes les solutions
     * @return l'évaluation en flottant
     */
    float evaluateRatio() const;

    /* Autres méthodes utiles */
    /**
     * Modifie la coordonnée en entrée pour obtenir celle qu'elle précède
     * Agit comme une itération sur l'ensemble des coordonnées de la surface
         * @param coord Coordonnée qui va être modifiée, non modifiée si il n'y a pas de suivante
     * @return vrai si il y a une coordonnées, faux si on est à la fin ou en dehors de la surface
     */
    bool nextCoordinates ( Coordinates* coord );

};
