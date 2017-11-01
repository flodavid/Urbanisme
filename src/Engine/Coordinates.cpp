#include "stdafx.h"
#include "Coordinates.h"

Coordinates::Coordinates(int _x, int _y)
	:col(_x), row(_y)
{}

Coordinates::Coordinates(const Coordinates& other)
	:col(other.col), row(other.row)
{}

Coordinates::~Coordinates()
{

}

Coordinates& Coordinates::operator=(const Coordinates& other)
{
    col= other.col;
    row= other.row;
    
    return *this;
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
    return static_cast<unsigned>(abs(col- other.col) + abs(row - other.row)); 
}

double Coordinates::euclideanDistance(const Coordinates& other) const
{
    assert(sqrt( pow((col - other.col), 2)  + pow((row - other.row), 2)) >= 0.0);
    // square-root(square(x1 - x2) + square(y1 - y2))
    return sqrt( pow((col - other.col), 2)  + pow((row - other.row), 2));
}

bool Coordinates::operator <(const Coordinates& other) const
{
    if (row < other.row) return true;
    else if (other.row == row) return col < other.col;
    else return false;
}
