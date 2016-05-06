#include "../stdafx.h"
#include "Parameters.h"


Parameters::Parameters(unsigned _serve_distance, unsigned _road_width):
    serve_distance(_serve_distance), road_width(_road_width)
{
}

Parameters::Parameters(const Parameters& other):
    serve_distance(other.serve_distance), road_width(other.road_width)
{}

Parameters::~Parameters()
{
}
