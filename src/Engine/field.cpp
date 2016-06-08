#include "../stdafx.h"
#include "field.h"

using namespace std;
///################################
/// Constructeurs et destructeurs
///################################
//@{

Field::Field(unsigned width, unsigned height/*, std::list<Coordinates>& inputs_and_ouputs*/) :
    nb_cols(width), nb_rows(height), parcels(NULL)       /*, ins_outs(inputs_and_ouputs)*/
{
    resizeWithDimensions();
}

Field::Field(const Field& other) :
    nb_cols(other.nb_cols), nb_rows(other.nb_rows), ins_outs(other.ins_outs)
{
    parcels= (State**)malloc(nb_rows * sizeof(State*));
    
    for (unsigned i= 0; i < nb_rows; ++i) {
	parcels[i]= (State*)malloc(nb_cols * sizeof(State));
	for (unsigned j= 0; j < nb_cols; ++j) {
	    parcels[i][j]= other.parcels[i][j];
	}
    }
}

Field::~Field()
{
    if (parcels != NULL) {
	for (unsigned i= 0; i < nb_rows; ++i) {
	    free(parcels[i]);
	}
	free(parcels);
    }
}

//@}
///#######################
///      Setters
///#######################
//@{

void Field::deleteOldMatrix()
{
    if (parcels != NULL) {
	for (unsigned i= 0; i < nb_rows; ++i) {
	    free(parcels[i]);
	}
	free(parcels);
    }
}


void Field::resizeWithDimensions()
{    
    parcels= (State**)malloc(nb_rows * sizeof(State*));

    for (unsigned i= 0; i < nb_rows; ++i) {
	parcels[i]= (State*)malloc(nb_cols * sizeof(State));
	for (unsigned j= 0; j < nb_cols; ++j) {
	    parcels[i][j]= is_undefined;
	}
    }
}

void Field::addRoads(std::list<Coordinates> *roads, unsigned serveDistance)
{
    for (const Coordinates& coord_road : *roads) {
        add_road(coord_road);
    }

    // On redéfinit les parcelles qui sont utilisables et celles qui ne le sont pas
    /// @see on pourrait améliorer, en mettant à jour seulement les parcelles proches des routes ajoutées
    resetUsables(serveDistance);
}

void Field::removeRoads(std::list<Coordinates> *roads, unsigned serveDistance)
{
    for (const Coordinates& coord_road : *roads) {
        add_undefined(coord_road);
    }

    // Mise à jour des cases affectées par la suppression des routes
    updateUsables(serveDistance);
}

bool Field::tryAdd_in_out(const Coordinates &coords)
{
    if (( coords.col == (int)get_width() -1 || coords.row == (int)get_height() -1 )
            || ( coords.col == 0 || coords.row == 0 )) {
        ins_outs.push_back(coords);
        parcels[coords.row][coords.col]= is_in_out;

        return true;
    } else return false;
}

void Field::add_undefined(const Coordinates& coords)
{
    if (is_in_out == parcels[coords.row][coords.col]){
        auto list_it= find(ins_outs.begin(), ins_outs.end(), coords);
        if ( list_it != ins_outs.end()) {
            ins_outs.erase(list_it);
        }
    }
    parcels[coords.row][coords.col]= is_undefined;
}

//@}
///#####################
///      Affichage
///#####################
//@{

void Field::show_ins_and_outs() const
{
    for (const Coordinates & in_or_out : ins_outs) {
        cout << in_or_out << " ; ";
    }
    cout << endl;
}

std::ostream &Field::print(std::ostream &out) const
{
    Coordinates& coord = Field::first();
    do {
        if ((coord.col) == 0) {
            out << endl << coord << "= " << at(coord);
        } else {
            out <<  "; " << coord << "= " << at(coord);
        }
    } while (nextCoordinates(&coord));
    delete &coord;

    return out;
}

//@}
///########################################
///=== Fonctions sur les coordonnées ===///
///########################################
//@{

bool Field::contains(int x, int y) const
{
    return (x >= 0 && ((unsigned) x) < nb_cols)       // abscisse correcte
           && (y >= 0  && ((unsigned) y) < nb_rows);       // ordonnée correcte
}

bool Field::contains(const Coordinates &coord) const
{
    return contains(coord.col, coord.row);
}


bool Field::nextCoordinates(Coordinates *coord) const
{
    // On vérifie que la coordonnée actuelle est dans la surface
    //  (vérification sur les colonnes puis sur les lignes)
    assert(coord->col >= 0 && coord->row >= 0 );
    assert( coord->col < (int)(nb_cols) && coord->row < (int)(nb_rows) );

    // Si on est à la fin de la ligne, on passe à la ligne suivante
    if ((unsigned)(coord->col) == nb_cols -1) {
        // seulement si on n'est pas à la fin de la surface
	if ((unsigned)(coord->row) + 1 < nb_cols) {
            coord->col= 0;
            coord->row+= 1;
#if DEBUG_PARCOURS_COORDS
            cout << "Passage à la ligne suivante" << endl;
#endif
            return true;
        } else {
            // dernière case de la surface
#if DEBUG_PARCOURS_COORDS
            cout << "Dernière case de la matrice" << endl;
#endif
            return false;
        }
    }// Sinon, on se décale sur la ligne
    else {
        coord->col += 1;
#if DEBUG_PARCOURS_COORDS
        cout << "Passage à la case suivante (à droite)" << endl;
#endif
        return true;
    }
}

//@}
///#############################
///===  Méthodes générales	===/
///#############################
//@{

void Field::generateInsAndOuts(unsigned nb)
{

    for (unsigned num_in_out = 0; num_in_out < nb; ++num_in_out) {
        int row, col;

        // On choisit une case en haut ou en bas, avec une colonne aléatoire
        bool on_top_or_down= (bool)(rand() % 2);
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


bool Field::isRoad(const Coordinates &neighbour) const
{
    return contains(neighbour)
           && at(neighbour) >= is_road; // route ou entrée/sortie
}

bool Field::isParcel(const Coordinates &neighbour) const
{
    return contains(neighbour)
           && ( at(neighbour) == is_usable || at(neighbour) == is_unusable);
}


bool Field::isUnusableParcel(const Coordinates &neighbour) const
{
    return contains(neighbour)
           && at(neighbour) == is_unusable;
}


bool Field::isRoadAndNeighbourOf(const Coordinates &neighbour, const Coordinates &coord, unsigned servingDistance) const
{
    return (isRoad(neighbour) && coord.manhattanDistance(neighbour) <= servingDistance && !(neighbour == coord));
}

std::list<Coordinates> *Field::getNeighbourRoads(const Coordinates &coord) const
{
#if DEBUG_ROADS_DIST
    cout << "Recherche des voisins de la parcelle en " << coord.col << " ; " << coord.row << endl;
#endif
    list<Coordinates> *neighbour_roads = new list<Coordinates>;

    Coordinates west(coord.col - 1, coord.row);
    Coordinates east(coord.col + 1, coord.row);
    Coordinates north(coord.col, coord.row - 1);
    Coordinates south(coord.col, coord.row + 1);
    // On vérifie que chaque voisin n'est pas en dehors de la matrice

    if (contains(west) && at(west) >= is_road) {
        // Ajout dans les routes voisines de la parcelle
        neighbour_roads->push_back(west);
#if DEBUG_ROADS_DIST
        cout << "\tparcelle " << west << endl;
#endif
    }
    if (contains(east) && at(east) >= is_road) {
        // Ajout dans les routes voisines de la parcelle
        neighbour_roads->push_back(east);
#if DEBUG_ROADS_DIST
        cout << "\tparcelle " << east << endl;
#endif
    }
    if (contains(north) && at(north) >= is_road) {
        // Ajout dans les routes voisines de la parcelle
        neighbour_roads->push_back(north);
#if DEBUG_ROADS_DIST
        cout << "\tparcelle " << north << endl;
#endif
    }
    if (contains(south) && at(south) >= is_road) {
        // Ajout dans les routes voisines de la parcelle
        neighbour_roads->push_back(south);
#if DEBUG_ROADS_DIST
        cout << "\tparcelle " << south << endl;
#endif
    }

    return neighbour_roads;
}

std::list<Coordinates> *Field::getClose(const Coordinates &coord, unsigned maxDist, bool (Field::*neighbourVerif)(const Coordinates&) const) const
{
    list<Coordinates> *close_cells= new list<Coordinates>;

    int serve_dist = (int)maxDist;

    // On vérifie si les routes entre (x +dist;y +dist) et (x -dist;y -dist)
    /// On vérifie ((2.serve_dist)+1)² parcelles,  alors qu'on pourrait en vérifier moins
    for (int i = coord.row + serve_dist; i >= coord.row - serve_dist; --i) {
        for (int j = coord.col + serve_dist; j >= coord.col - serve_dist; --j) {
            // On vérifie que la parcelle n'est pas en dehors de la matrice et qu'elle n'est pas la coordonnée courante
            Coordinates neighbour(j,  i);
            if ( (this->*neighbourVerif)(neighbour) && !(neighbour == coord)
                    && coord.manhattanDistance(neighbour) <= maxDist) {
                // Ajout dans les routes voisines de la parcelle
                close_cells->push_back(neighbour);    /// @see copie faite :'(
            }
        }// fin_for
    }// fin_for

    return close_cells;
}

std::list<Coordinates> * Field::getCloseUnusableParcels(const Coordinates &coord, unsigned servingDistance) const
{
    bool (Field::*monPointeur)(const Coordinates&) const;   //On déclare un pointeur sur fonction
    monPointeur = &Field::isUnusableParcel;

    return getClose(coord, servingDistance, monPointeur);
}

std::list<Coordinates> *Field::getCloseParcels(const Coordinates &coord, unsigned maxDist) const
{
    bool (Field::*monPointeur)(const Coordinates&) const;   //On déclare un pointeur sur fonction
    monPointeur = &Field::isParcel;

    return getClose(coord, maxDist, monPointeur);
}

std::list<Coordinates> *Field::getCloseRoads(const Coordinates &coord, unsigned maxDist) const
{
    bool (Field::*monPointeur)(const Coordinates&) const;   //On déclare un pointeur sur fonction
    monPointeur = &Field::isRoad;

    return getClose(coord, maxDist, monPointeur);
}

std::list<Coordinates> *Field::getServingRoads(const Coordinates &coord , unsigned servingDistance) const
{
    list<Coordinates> *serving_neighbours = new list<Coordinates>;
    // on ne récupère pas les routes qui desservent d'autres routes,
    // seulement celles qui desservent des parcelles ou sont collées

    if (at(coord) >= is_road) {
        servingDistance = 1;
    }

    bool neighbour_found =  false;
    for (int s_dist = 1; s_dist <= (int) servingDistance && !neighbour_found; ++s_dist) {

        // On vérifie si les routes entre (x +dist;y +dist) et (x -dist;y -dist)
        /// On vérifie ((2.servingDistance)+1)² parcelles,  alors qu'on pourrait en vérifier moins
        for (int i = coord.row + s_dist; i >= coord.row - s_dist; --i) {
            for (int j = coord.col + s_dist; j >= coord.col - s_dist; --j) {

                // On vérifie que la parcelle n'est pas en dehors de la matrice et qu'elle n'est pas la coordonnée courante
                Coordinates neighbour(j,  i);
                if (isRoad(neighbour)
                        && coord.manhattanDistance(neighbour) == (unsigned)s_dist) {
                    // Ajout dans les routes voisines de la parcelle
                    serving_neighbours->push_back(neighbour);    // @see copie faite :'(
                    neighbour_found = true;
#if DEBUG_ROADS_DIST
                    cout << "parcelle en " << j << " ; " << i << " est une route voisine de la parcelle en "
                         << coord.col << " ; " << coord.row << endl;
#endif
                }
            }// fin_for
        }// fin_for
    }// fin_for distance

    return serving_neighbours;
}

bool Field::hasServingRoad(const Coordinates &coord , unsigned servingDistance) const
{
    int serve_dist = (int) servingDistance;  // il est plus simple de convertir en entier

    // On vérifie si les routes entre (x +dist;y +dist) et (x -dist;y -dist)
    /// On vérifie ((2.serve_dist)+1)² parcelles,  alors qu'on pourrait en vérifier moins
    for (int i = coord.row + serve_dist; i >= coord.row - serve_dist; --i) {
        for (int j = coord.col + serve_dist; j >= coord.col - serve_dist; --j) {

            // On vérifie que la parcelle n'est pas en dehors de la matrice et qu'elle n'est pas la coordonnée courante
            Coordinates neighbour(j,  i);
            if (isRoadAndNeighbourOf(neighbour,  coord,  serve_dist)) {
#if DEBUG_ROADS_DIST
                cout << "parcelle en " << j << " ; " << i << " est une route voisine de la parcelle en "
                     << coord.col << " ; " << coord.row << endl;
#endif
                // SORT DE LA DOUBLE BOUCLE, on a trouvé un voisin
                return true;
            }
        }
    }

    // Si on arrive à ce point, c'est que l'on n'a pas trouvé de voisin
    return false;
}


void Field::setUsables(unsigned int servingDistance)
{
    Coordinates &coords = first();
    do {
#if DEBUG
        if ((coord.col) == 0) {
            cout << endl << coord;
        } else {
            cout <<  "; " << coord;
        }
#endif
        if (at(coords) == is_undefined) {
            if (hasServingRoad(coords, servingDistance)) {
                parcels[coords.row][coords.col] = is_usable;
            } else {
                parcels[coords.row][coords.col] = is_unusable;
            }
        }
    } while (nextCoordinates(&coords));
    delete &coords;
}

void Field::resetUsables(unsigned int servingDistance)
{
    Coordinates &coords = first();
    do {
#if DEBUG
        if ((coord.col) == 0) {
            cout << endl << coord;
        } else {
            cout <<  "; " << coord;
        }
#endif
        if (at(coords) < is_road) {
            if (hasServingRoad(coords, servingDistance)) {
                parcels[coords.row][coords.col] = is_usable;
            } else {
                parcels[coords.row][coords.col] = is_unusable;
            }
        }
    } while (nextCoordinates(&coords));
    delete &coords;
}

void Field::updateUsables(unsigned int servingDistance)
{
    Coordinates &coords = first();
    do {
        if (at(coords) == is_undefined) {
            list<Coordinates>* parcels_to_update= getCloseParcels(coords, servingDistance);
            parcels_to_update->push_back(coords);

            for (const Coordinates& coords_parcel : *parcels_to_update) {
                if (hasServingRoad(coords_parcel, servingDistance)) {
                    parcels[coords_parcel.row][coords_parcel.col] = is_usable;
                } else {
                    parcels[coords_parcel.row][coords_parcel.col] = is_unusable;
                }
            }
            delete parcels_to_update;
        }
    } while (nextCoordinates(&coords));
    delete &coords;
}

//@}
