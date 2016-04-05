#ifndef COORDINATES_H
#define COORDINATES_H

#include <iostream>

class Coordinates
{

public:
	int col;
	int row;

public:
	Coordinates(int _x, int _y);
	Coordinates(const Coordinates& other);
	virtual bool operator==(const Coordinates& other) const;

	// Affichage
	std::ostream& print(std::ostream& out) const;
	friend std::ostream& operator<<(std::ostream& out, const Coordinates& coord);
};

#endif // COORDINATES_H
