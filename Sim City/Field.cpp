#include "stdafx.h"
#include "Field.h"

using namespace std;

Field::Field(unsigned width, unsigned height, std::list<Coordinates>& inputs_and_ouputs) :
	nb_rows(width), nb_cols(height), ins_outs(inputs_and_ouputs)
{
}

Field::~Field()
{
}

void Field::show_ins_and_outs() const
{
	for (const Coordinates& in_or_out : ins_outs) {
		cout << in_or_out << " ; ";
	}
	cout << endl;
}

void Field::generateInsAndOuts(unsigned nb)
{
	bool on_top_or_down;
	for (unsigned num_in_out = 0; num_in_out < nb; ++num_in_out) {
		on_top_or_down = (bool) (rand() % 2);
		int row, col;

		// On choisit une case en haut ou en bas, avec une colonne aléatoire
		if (on_top_or_down) {
			row = (rand() % 2) * nb_rows;
			col = rand() % nb_cols;
		}
		// Même opération en inversant lignes et colonnes
		else {
			row = rand() % nb_rows;
			col = (rand() % 2) * nb_cols;
		}

		ins_outs.push_front(Coordinates(col, row));
		ins_outs.push_back(Coordinates(col, row));
	}



}
