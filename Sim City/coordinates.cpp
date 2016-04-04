#include "stdafx.h"
#include "coordinates.h"

Coordinates::Coordinates(int _x, int _y)
	:col(_x), row(_y)
{}

Coordinates::Coordinates(const Coordinates& other)
	:col(other.col), row(other.row)
{}

bool Coordinates::operator==(const Coordinates& other) const
{
	return (other.col==col && other.row==row);
}