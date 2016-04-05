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

std::ostream& Coordinates::print(std::ostream& out) const
{
	out << "(" << col << "," << row << ")";
	return out;
}

std::ostream& operator<<(std::ostream& out, const Coordinates & coord)
{
	return coord.print(out);
}
