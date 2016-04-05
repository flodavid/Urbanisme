#pragma once
#ifndef FIELD_H
#define FIELD_H

#include <list>
#include <vector>

#include "coordinates.h"

enum State{undefined= -1, road, usable, unusable};

class Field
{

private:
	unsigned nb_cols, nb_rows;
	std::list<Coordinates> ins_outs;

	std::vector<std::vector<State>> parcels;

public:
	Field(unsigned width, unsigned height,
	      std::list<Coordinates>& inputs_and_ouputs = *(new std::list<Coordinates>()));
	~Field();

/*=== Getters ===*/
    
private:
/*=== Setters ===*/
	void resizeWithDimensions();
	
/**=== Operators ===**/
public:
	std::vector<State>::const_iterator begin() const { return parcels[0].cbegin(); }
	std::vector<State>::const_iterator end() const { return parcels[nb_rows-1].cend(); }
	
	std::vector<State>::iterator begin() { return parcels[0].begin(); }
	std::vector<State>::iterator end() { return parcels[nb_rows-1].end(); }
	
	friend std::vector<State>::const_iterator begin(const Field& f) { return f.begin();}
	
public:
	/* Affichage */
	void show_ins_and_outs() const;

	// Fonctions générales
	void generateInsAndOuts(unsigned nb);

};

#endif //FIELD_H