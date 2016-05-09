#ifndef COORDINATES_H
#define COORDINATES_H

#include <iostream>
#include <cassert>
#include <cmath>
#include <list>
#include <algorithm> /// @see Normalement inutile, permet de ne pas utilise -lm lors compilation

/**
 * @brief Représentation et opérations simple sur des coordonnées dans un plan.
 */
class Coordinates
{

public:
    /**
     * Colonne, abscisse des coordonnées
     */
    int col;
    /**
     * Ligne, ordonnée des coordonnées
     */
	int row;

public:
    /**
     * Constructeur de la classe Coordinates, crée les coordonnées à partir de la position
     * sur les axes
     * @param _x Abscisse des coordonnées, axe horizontal
     * @param _y Ordonnée des coordonnées, axe vertical
     */
	Coordinates(int _x, int _y);
    /**
     * Constructeur par recopie de la classe Coordinates, crée les coordonnées à partir
     * d'autres coordonnées
     * @param other Les autres coordonnées, que l'on va recopier pour créer la nouvelle instance
     */
	Coordinates(const Coordinates& other);
	virtual~ Coordinates();
	
    /**
     * Surcharge de l'opérateur = d'affectation, pour la classe
     * @param other Les autres coordonnées, que l'on va recopier dans l'instance
     * @return Les nouvelles coordonnnées
     */
	Coordinates& operator=(const Coordinates& other);

    /**
     * Surcharge de l'opérateur == de comparaison, pour la classe
     * @param other Les autres coordonnées, que l'on va comparer avec l'instance
     * @return vrai si les deux coordonnées sont identiques (même position)
     */
	virtual bool operator==(const Coordinates& other) const;

    // Affichage
    /**
     * Imprime dans un flux les informations de l'instance
     * @param out Flux dans lequel on imprime les informations
     * @return Le flux en entrée sur lequel on a imprimé les informations
     */
	std::ostream& print(std::ostream& out) const;
    /**
     * Surcharge de l'opérateur << de sortie sur flux
     * @param out Flux dans lequel on imprime les informations
     * @param coord Coordonnées que l'on souhaite imprimer
     * @return  Le flux en entrée sur lequel on a imprimé les informations des
     * coordonnées passées en entrée
     */
	friend std::ostream& operator<<(std::ostream& out, const Coordinates& coord)
	    { return coord.print(out); }
	
    // Calculs
    /**
	* Calcul de la distance Manhattan entre deux points
	* @return distance manhattan : entier non signé
	*/
    unsigned manhattanDistance(const Coordinates& other) const;
    /**
	* Calcul de la distance euclidienne entre deux points
	* @return distance euclidienne : flottant non signé
	*/
    float euclideanDistance(const Coordinates& other) const;
    
    
    /**
	* Surcharge de l'opérateur << de sortie sur flux
	* @param out Flux dans lequel on imprime les informations
	* @param coords Liste de coordonnées que l'on souhaite imprimer
	* @return  Le flux en entrée sur lequel on a imprimé les informations des
	* coordonnées passées en entrée
	*/
    friend std::ostream& operator<<( std::ostream &out, const std::list< Coordinates > &coords )
    {
        for (const Coordinates& coord : coords) {
            out << coord<< "; ";
        }
        return out;
    }

    /**
     * Méthode utile seulement pour placer les vecteur dans un conteneur map
     * @param other Autres coordonnées
     * @return vrai si la ligne de la seconde coordonnée est supérieur à la ligne de l'instance,
     * ou la colonne les coordonnées sont sur la "même ligne". Faux sinon
     */
    virtual bool operator<(const Coordinates& other) const;
};

#endif // COORDINATES_H
