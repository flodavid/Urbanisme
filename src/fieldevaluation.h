#ifndef FIELDEVALUATION_H
#define FIELDEVALUATION_H

#include <list>

#include "Engine/field.h"
#include "evaluation.h"

/**
 * @brief La classe permettant de stocker une surface et de calculer et mettre à jour son évaluation
 */
class FieldEvaluation : public Field, public Evaluation
{
private:
    const Parameters& params;

    // Données calculées
    std::vector< std::vector< std::vector< std::vector< unsigned > > > > road_distances;

public:
    /**
     * Constructeur à partir d'une surface existante recopiée et de paramètres recopiés
     * @param field
     * @param params
     */
    FieldEvaluation(Field &field, const Parameters& params);

    /* Getters */
    /**
     * Donne la distance par les routes stockée
     * @param coord1 Première coordonnée
     * @param coord2 Seconde coordonnée
     * @return La distance par le route, un entier non signé
     */
    inline unsigned getRoadDistance(const Coordinates& coord1, const Coordinates& coord2) const
    {
        if (!contains(coord1) || !contains(coord2)) {
            return UNSIGNED_INFINITY;
        }
    //        assert(road_distances_are_initiated);
        return road_distances.at(coord1.row)[coord1.col][coord2.row][coord2.col];
    }

    /* Calculs de données */
private:
    /**
     * Initialise la matrice de voisinage selon la taille de la surface
     */
    void initSizeNeighbourhood();
    /**
     * Calcule la distance entre une parcelle et les parcelles "suivantes"
     */
    void initRoadDistance(const Coordinates& coord);

public:
    /**
     * Définit la matrice de distance entre chaque parcelle et chaque autre parcelle
     */
    void initRoadDistances();

    /**
     * Change de surface et remet à zéro les valeurs
     */
    void reset();

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
     * toutes les routes qui desservent les cellules et en respectant les paramètres
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

    /* Threads */

    /**
     * Evalue le ratio de toutes les solutions avec des threads
     * @return l'évaluation en flottant
     */
    float threadsEvaluateRatio() const;
};

void TParcelRatios(Coordinates coord, float* ratio, const Evaluation* res);
/* Threads */
//      void *TParcelRatios(void *threadid);

#endif // FIELDEVALUATION_H
