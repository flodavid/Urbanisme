#pragma once

#include <vector>

#include "stdafx.h"
#include "Engine/field.h"
#include "Engine/parameters.h"
#include "Engine/Coordinates.h"

/**
 * @brief Classe chargée de travailler sur les données en entrée du problème pour trouver des
 * solution réalisables, opitmisant les objectifs suivants :
 * 1. Maximimiser les nombres de parcelles exploitables
 * 2. Minimiser la moyenne, la plus grande ou la somme des ratios entre distance
 * des cellules par les routes et distance directe (distance Manhattan ou Euclidienne)
 *
 * On pourra éventuellement avoir plusieurs instances de Field dans une Resolution.
 */
class Evaluation
{
protected:
    // Données calculées
    /// @brief Nombre de parcelles utilisables
    unsigned nbUsables;
    /// @brief Moyenne du ratio entre distance directe et distance par les routes,
    /// représente l'accessibilité de la surface
    float avgAccess;

    /// @see Voir si on rajoute le ratio maximum et la moyenne quadratique

public:
    // FLAGS
    /**
     * Flag indiquant si la matrice de distance des routes a été calculée
     */
    bool road_distances_are_initiated= false;

public:
    /**
     * Constructeur de la casse résolution
     */
    Evaluation ();
    ~Evaluation();

    /* Getters */
    /**
     * Accesseur sur l'attribut de nombre de parcelle exploitables
     * @return nbUsables, un entier non signé
     */
    unsigned get_nbUsables() const
    { return nbUsables; }
    /**
     * Accesseur sur l'attribut de moyenne de ratio, entre la distance directe et
     * la distance par les routes
     * @return avgAccess, un flotant
     */
    float get_avgAccess() const
    { return avgAccess; }

    /* Fonctions utiles à l'évaluation */

    /**
     * Vérifie si l'évaluation est dominée par une autre
     * @param other Autre instance
     * @return vrai si les deux objectifs de l'évaluation sont inférieurs à ceux
     * de l'autre instance
     */
    bool is_dominated(const Evaluation& other) const
    {
        return ( other.nbUsables >= nbUsables && other.avgAccess <= avgAccess );
    }

};
