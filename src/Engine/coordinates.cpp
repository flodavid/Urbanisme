#include "../stdafx.h"
#include "coordinates.h"

Coordinates::Coordinates(int _x, int _y)
	:col(_x), row(_y)
{}

Coordinates::Coordinates(const Coordinates& other)
	:col(other.col), row(other.row)
{}

Coordinates::~Coordinates()
{

}


bool Coordinates::operator==(const Coordinates& other) const
{
	return (other.col==col && other.row==row);
}

std::ostream& Coordinates::print(std::ostream& out) const
{
	out << "(" << col << "," << row << ")";
	return out;
}

unsigned Coordinates::manhattanDistance(const Coordinates& other) const
{
    assert(abs(col- other.col) + abs(row - other.row) >= 0);
    return abs(col- other.col) + abs(row - other.row); 
}

float Coordinates::euclideanDistance(const Coordinates& other) const
{
    assert(sqrt( pow((col - other.col), 2)  + pow((row - other.row), 2)) >= 0.0);
    // square-root(square(x1 - x2) + square(y1 - y2))
    return sqrt( pow((col - other.col), 2)  + pow((row - other.row), 2));
}

