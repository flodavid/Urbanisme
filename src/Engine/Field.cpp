#include "../stdafx.h"
#include "Field.h"

using namespace std;

Field::Field(unsigned width, unsigned height/*, std::list<Coordinates>& inputs_and_ouputs*/) :
	nb_cols(width), nb_rows(height)/*, ins_outs(inputs_and_ouputs)*/
{
    resizeWithDimensions();
}

Field::~Field()
{
}

///#######################
///      Setters
///#######################

void Field::resizeWithDimensions()
{
    parcels.resize(nb_cols);
    
    for (vector<State>& parcel_row : parcels){
	parcel_row.resize(nb_cols, is_undefined);
    }
}

///###################
///      Affichage
///###################

void Field::show_ins_and_outs() const
{
    for (const Coordinates& in_or_out : ins_outs) {
	cout << in_or_out << " ; ";
    }
    cout << endl;
}

///########################################
///=== Fonctions sur les coordonnées ===///
///########################################
bool Field::contains(int x, int y) const
{
    return ( x >= 0 &&  ((unsigned)x) < nb_cols )     // abscisse correcte
        && ( y >= 0  &&  ((unsigned)y) < nb_rows );    // ordonnée correcte
}


bool Field::contains(const Coordinates& coord) const
{
    return contains(coord.col, coord.row);
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

        ins_outs.push_back(Coordinates(col, row));
    }
}
