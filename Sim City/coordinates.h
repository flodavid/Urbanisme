#ifndef COORDINATES_H
#define COORDINATES_H

class Coordinates
{

public:
	int col;
	int row;

public:
	Coordinates(int _x, int _y);
	Coordinates(const Coordinates& other);
	virtual bool operator==(const Coordinates& other) const;
	
};

#endif // COORDINATES_H
