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
	Field(unsigned width, unsigned height, std::list<Coordinates>& inputs_and_ouputs = std::list<Coordinates>());
	~Field();

	// Affichage
	void show_ins_and_outs() const;

	// Fonctions générales
	void generateInsAndOuts(unsigned nb);

};

