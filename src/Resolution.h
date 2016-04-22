#pragma once

#include <list>

#include "Engine/Field.h"
#include "Engine/Parameters.h"
#include "Engine/coordinates.h"

/**
 * Classe chargée de travailler sur les données en entrée du problème pour trouver des
 * solution réalisables, opitmisant les objectifs suivants :
 * 1. Maximimiser les nombres de parcelles exploitables
 * 2. Minimiser la moyenne, la plus grande ou la somme des ratios entre distance
 * des cellules par les routes et distance directe (distance Manhattan ou Euclidienne)
 * 
 * On pourra éventuellement avoir plusieurs instances de Field dans une Resolution.
 */
class Resolution
{
private:
    // Données sur lesquelles on travaille
public: // TODO temporaire, TODELETE
    Field field;
    Parameters params;

    // Données calculées
    std::vector<std::vector<std::vector<std::vector<unsigned>>>> road_distances;

    // FLAGS
    bool road_distances_are_initiated= false;

public:
    /**
     * Constructeur de la casse résolution
     * @param _field Surface sur laquelle on travaille
     * @param _params Paramètres du problème
     */
    Resolution ( const Field & _field, const Parameters & _params );
    ~Resolution();

    /* Getters */
    unsigned getRoadDistance(const Coordinates& coord1, const Coordinates& coord2) const
    { return road_distances[coord1.row][coord1.col][coord2.row][coord2.col]; }

    /*** Setters */
    /**
     * @brief set_params
     */
    void set_params ( const Parameters& _params );
    
    /* Calculs de données */
    
    /**
     * Initialise la matrice de voisinage avec les routes avec la distance Manhattan
     * TODO
     */
    void initCoordNeighbourhoodManhattan(const Coordinates& coord);
    /**
     * Initialise la matrice de voisinage avec les routes avec la distance Manhattan
     * TODO
     */
    void initNeighbourhoodManhattan();

    /* Fonctions auxiliaires */

    /**
     * Calcule la distance par les routes entre deux cellules, en respectant les paramètres
        * @param coord1 Coordonnée "de départ"
        * @param coord2 Coordonnée "d'arrivée"
     * @return la valeur, entier non signé, de distance la plus courte,
     *   entre les deux coordonnées, en passant par les routes
     */
    unsigned calcRoadDistance ( const Coordinates& coord1, const Coordinates& coord2 );

private:
    /**
     * Calcule la distance entre deux points, en passant de route en route
     * On suppose ques les coordonées courantes sont des routes
     */
    unsigned recCalcRoadDistance ( const Coordinates& coord1, const Coordinates& coord2, std::list<Coordinates> *visited, unsigned dist_max );

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
    /**
     * Evalue le ratio de toutes les solutions avec des threads
     * @return l'évaluation en flottant
     */
    float threadsEvaluateRatio() const;

    /* Autres méthodes utiles */
    /**
      * @brief createExample
      * @test Utile seulement pour les tests
      */
     void createExample();
     
     /* Threads */

};

void TParcelRatios(Coordinates coord, float* ratio, const Resolution* res);
/* Threads */
//      void *TParcelRatios(void *threadid);
