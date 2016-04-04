#pragma once
#include <list>
#include <vector>
#include "coordinates.h"

enum State{undefined= -1, road, usable, unusable};

class Field
{

private:
	std::list<Coordinates> ins_outs;
	unsigned nb_cols, nb_rows;

	std::vector<std::vector<State>> parcels;

public:
	Field(std::list<Coordinates>& inputs_and_ouputs);
	~Field();
};

