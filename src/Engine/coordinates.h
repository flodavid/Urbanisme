#ifndef COORDINATES_H
#define COORDINATES_H

#include <iostream>
#include <cassert>
#include <cmath>
#include <algorithm> /// @SEE Normalement inutile, permet de ne pas utilise -lm lors compilation

/**
 * Représentation et opérations simple sur des coordonnées dans un plan.
 */
class Coordinates
{

public:
	int col;
	int row;

public:
	Coordinates(int _x, int _y);
	Coordinates(const Coordinates& other);
	virtual~ Coordinates();
	
	Coordinates& operator=(const Coordinates& other);
	
	virtual bool operator==(const Coordinates& other) const;

    // Affichage
	std::ostream& print(std::ostream& out) const;
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
	
};

#endif // COORDINATES_H