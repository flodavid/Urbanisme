#include "stdafx.h"
#include "evaluation.h"

/// #########################
/// Autres méthodes utiles
/// #########################
//@{

///*
// Grand
void Evaluation::createExample()
{
    // Surface de l'exemple :
    Field &example = field;
    example.set_width(20);
    example.set_height(20);
    example.resizeWithDimensions();

    example.add_in_out(9, 0);
    // Colonne Milieu
    example.add_road(9, 1);
    example.add_road(9, 2);
    example.add_road(9, 3);
    example.add_road(9, 4);
    example.add_road(9, 5);
    example.add_road(9, 6);
    example.add_road(9, 7);
    example.add_road(9, 8);
    example.add_road(9, 9);
    example.add_road(9, 10);
    example.add_road(9, 11);
    example.add_road(9, 12);
    example.add_road(9, 13);
    example.add_road(9, 14);
    example.add_road(9, 15);
    example.add_road(9, 16);
    example.add_road(9, 17);
    example.add_road(9, 18);
    // Première Ligne
    example.add_road(0, 2);
    example.add_road(1, 2);
    example.add_road(2, 2);
    example.add_road(3, 2);
    example.add_road(4, 2);
    example.add_road(5, 2);
    example.add_road(6, 2);
    example.add_road(7, 2);
    example.add_road(8, 2);
    // example.add_road(9, 2); déjà fait dans colonne milieu
    example.add_road(10, 2);
    example.add_road(11, 2);
    example.add_road(12, 2);
    example.add_road(13, 2);
    example.add_road(14, 2);
    example.add_road(15, 2);
    example.add_road(16, 2);
    example.add_road(17, 2);
    example.add_road(18, 2);
    example.add_road(19, 2);
    // Deuxième Ligne
    example.add_road(0, 7);
    example.add_road(1, 7);
    example.add_road(2, 7);
    example.add_road(3, 7);
    example.add_road(4, 7);
    example.add_road(5, 7);
    example.add_road(6, 7);
    example.add_road(7, 7);
    example.add_road(8, 7);
    // example.add_road(9, 7); déjà fait dans colonne milieu
    example.add_road(10, 7);
    example.add_road(11, 7);
    example.add_road(12, 7);
    example.add_road(13, 7);
    example.add_road(14, 7);
    example.add_road(15, 7);
    example.add_road(16, 7);
    example.add_road(17, 7);
    example.add_road(18, 7);
    example.add_road(19, 7);
    // Troisième Ligne
    example.add_road(0, 12);
    example.add_road(1, 12);
    example.add_road(2, 12);
    example.add_road(3, 12);
    example.add_road(4, 12);
    example.add_road(5, 12);
    example.add_road(6, 12);
    example.add_road(7, 12);
    example.add_road(8, 12);
    // example.add_road(9, 12); déjà fait dans colonne milieu
    example.add_road(10, 12);
    example.add_road(11, 12);
    example.add_road(12, 12);
    example.add_road(13, 12);
    example.add_road(14, 12);
    example.add_road(15, 12);
    example.add_road(16, 12);
    example.add_road(17, 12);
    example.add_road(18, 12);
    example.add_road(19, 12);
    // Quatrième Ligne
    example.add_road(0, 17);
    example.add_road(1, 17);
    example.add_road(2, 17);
    example.add_road(3, 17);
    example.add_road(4, 17);
    example.add_road(5, 17);
    example.add_road(6, 17);
    example.add_road(7, 17);
    example.add_road(8, 17);
    // example.add_road(9, 17); déjà fait dans colonne milieu
    example.add_road(10, 17);
    example.add_road(11, 17);
    example.add_road(12, 17);
    example.add_road(13, 17);
    example.add_road(14, 17);
    example.add_road(15, 17);
    example.add_road(16, 17);
    example.add_road(17, 17);
    example.add_road(18, 17);
    example.add_road(19, 17);

    example.add_in_out(9, 19);

    evaluateTotalUsable();

    // Définition des états des parcelles
    field.defineUsables(params.get_serve_distance());
}
// */
/*
// Petit
void Resolution::createExample()
{
    // Surface de l'exemple :
    Field &example = field;
    example.set_width(10);
    example.set_height(10);
    example.resizeWithDimensions();

    example.add_in_out(9, 0);
    // Colonne Milieu
    example.add_road(9, 1);
    example.add_road(9, 2);
    example.add_road(9, 3);
    example.add_road(9, 4);
    example.add_road(9, 5);
    example.add_road(9, 6);
    example.add_road(9, 7);
    example.add_road(9, 8);
    example.add_road(9, 9);
    // Première Ligne
    example.add_road(0, 2);
    example.add_road(1, 2);
    example.add_road(2, 2);
    example.add_road(3, 2);
    example.add_road(4, 2);
    example.add_road(5, 2);
    example.add_road(6, 2);
    example.add_road(7, 2);
    example.add_road(8, 2);
    // example.add_road(9, 2); déjà fait dans colonne milieu
    // Deuxième Ligne
    example.add_road(0, 7);
    example.add_road(1, 7);
    example.add_road(2, 7);
    example.add_road(3, 7);
    example.add_road(4, 7);
    example.add_road(5, 7);
    example.add_road(6, 7);
    example.add_road(7, 7);
    example.add_road(8, 7);
    // example.add_road(9, 7); déjà fait dans colonne milieu

    evaluateTotalUsable();

    // Définition des états des parcelles
    field.defineUsables(params.get_serve_distance());
}
// */

/*
// Tout Petit
void Resolution::createExample()
{
    // Surface de l'exemple :
    Field &example = field;
    example.set_width(3);
    example.set_height(3);
    example.resizeWithDimensions();

    // Première Ligne
    example.add_road(0, 2);
    example.add_road(1, 2);
    example.add_road(2, 2);

    evaluateTotalUsable();

    // Définition des états des parcelles
    field.defineUsables(params.get_serve_distance());
}
// */

//@}

