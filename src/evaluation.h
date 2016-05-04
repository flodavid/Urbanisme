#pragma once

#include <list>

#include "stdafx.h"
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
class Evaluation
{
private:
    // Données sur lesquelles on travaille
//public: // TODO temporaire pour tests, TODELETE
    const Field& field;
    Parameters params;

    // Données calculées
    std::vector< std::vector< std::vector< std::vector< unsigned > > > > road_distances;
    unsigned nbUsables;
    float avgAccess;

    // FLAGS
    bool road_distances_are_initiated= false;

public:
    /**
     * Constructeur de la casse résolution
     * @param _field Surface sur laquelle on travaille
     * @param _params Paramètres du problème
     */
    Evaluation ( const Field & _field, const Parameters & _params );
    ~Evaluation();

    /* Getters */
    /**
     * Accesseur sur l'attribut de nombre de parcelle exploitables
     * @return nbUsables, un entier non signé
     */
    float get_nbUsables() const
    { return nbUsables; }
    /**
     * Accesseur sur l'attribut de moyenne de ratio, entre la distance directe et
     * la distance par les routes
     * @return avgAccess, un flotant
     */
    float get_avgAccess() const
    { return avgAccess; }

    /**
     * Donne la distance par les routes stockée
     * @param coord1 Première coordonnée
     * @param coord2 Seconde coordonnée
     * @return La distance par le route, un entier non signé
     */
    inline unsigned getRoadDistance(const Coordinates& coord1, const Coordinates& coord2) const
    { if (!field.contains(coord1) || !field.contains(coord2)) return UNSIGNED_INFINITY;
      return road_distances[coord1.row][coord1.col][coord2.row][coord2.col]; }

    /*** Setters */
    /**
     * Mutateur sur les paramètres du problème
     */
    void set_params ( const Parameters& _params );
    
    /* Calculs de données */
    /**
     * Initialise la matrice de voisinage selon la taille de la surface
     */
    void initSizeNeighbourhood();
    /**
     * Définit la matrice de voisinage avec les routes avec la distance Manhattan
     */
    void initCoordNeighbourhoodManhattan(const Coordinates& coord);
    /**
     * Définit la matrice de voisinage avec les routes avec la distance Manhattan
     */
    void initNeighbourhoodManhattan();

    /* Fonctions utiles à l'évaluation */

private:
    /**
     * Calcule la distance entre deux routes, en passant de route en route, stocke
     * la valeur calculée dans ma matrice de distance si un chemin est trouvé<-+
     * @param testCoord Position de la route de départ
     * @param dest  Position de la route d'arrivé
     * @param visited Liste des routes déjà visitées, on ne repasse plus par elles
     * @param minDist Distance minimale courante // @see est pris en compte dans la distance max de visite
     *  (on se'arrête si le nombre de visités est supérieur à la distance minimale courante)
     * @return la longueur entre les deux routes
     */
    unsigned int lengthBy ( const Coordinates& testCoord, const Coordinates& dest, std::list<Coordinates>* visited, unsigned int minDist );

    /**
     * Vérifie que l'on n'est pas déjà passé par la coordonnées testée, effectue
     * le calcul de distance le cas échéant et modifie la distance minimale trouvée si besoin
     * @param testCoord Coordonnées testées
     * @param dest Coordonées d'arrivé
     * @param visited Cases déjà visitées
     * @param minDist Distance minimale courante
     * @return La distance minimale calculée (inchangée si on n'a pas trouvé de plus court chemin)
     */
    unsigned int testPathBy ( const Coordinates& testCoord, const Coordinates& dest, std::list<Coordinates>* visited, unsigned int minDist );
    /**
     * Calcule la distance entre deux routes, en passant de route en route, en testant tous les voisins.
     * On suppose ques les coordonées courantes sont des routes
     */
    unsigned calcRoadDistance (const Coordinates& coord1, const Coordinates& coord2, std::list<Coordinates> *visited, unsigned dist_max );

public:
    /**
     * Calcule la distance par les routes entre deux cellules quelconques, en testant
     * toutes les routs qui desservent la cellue et en respectant les paramètres
        * @param coord1 Coordonnée "de départ"
        * @param coord2 Coordonnée "d'arrivée"
     * @return la valeur, entier non signé, de distance la plus courte,
     *   entre les deux coordonnées, en passant par les routes
     */
    unsigned parcelsRoadDistance (const Coordinates& coord1, const Coordinates& coord2 );

    /* Evaluations */
    /**
     * Compte le nombre total de parcelles exploitables sur la surface
     * @return nombre de parcelles exploitables
     */
    unsigned evaluateTotalUsable();

    /**
     * Calcule le ratio entre distance par la route et distance Manhattan entre deux positions
         * @param p1 premières coordonnées
         * @param p2 secondes coordonnées
     * @return le ratio, sous forme de flottant
     */
    float manhattanRatioBetween2Parcels ( const Coordinates& p1, const Coordinates& p2 ) const;
    /**
     * Evalue le ratio de toutes les solutions
     * @return la moyenne des ratios
     */
    float evaluateRatio();
    /**
     * Evalue le ratio de toutes les solutions avec des threads
     * @return l'évaluation en flottant
     */
    float threadsEvaluateRatio() const;
     
     /* Threads */

};

void TParcelRatios(Coordinates coord, float* ratio, const Evaluation* res);
/* Threads */
//      void *TParcelRatios(void *threadid);
