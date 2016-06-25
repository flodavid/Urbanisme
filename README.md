Projet TER : Urbanisme
======================

            Compilation Travis                   |                  License                     |
------------------------------------------------ | -------------------------------------------- |
[![Build Status](https://travis-ci.org/flodavid/Urbanisme.svg?branch=master)](https://travis-ci.org/flodavid/Urbanisme) | [![GPLv3 License](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://github.com/flodavid/Urbanisme/blob/master/COPYING.txt)

[![SIM-CITY](https://github.com/flodavid/Urbanisme/blob/gh-pages/images/SimCity_shadows-logo.png)](https://flodavid.github.io/Urbanisme)

## Organisation du travail et du développement

### I. Etudes préliminaires :
1. [x] Réflexions sur les applications et enjeux du problème 
2. [x] Recherche de travaux similaires
3. [x] Réflexion sur les langages, outils et approches du problème

### II. Fonctionnalités essentielles
1. [x] Utilisation d'une classe Coordonnées, et création d'une classe représentant les paramètres du problème
2. [x] Représentation du terrain : des entrées et sorties et d'une matrice d'états de la surface
3. [x] Fonction d'évaluation complète du nombre de parcelles exploitables d'une solution
4. [x] Calcul de la distance "directe" entre deux points, Manhattant et Euclidienne, avec tests correspondants
5. [x] Calcul de la distance par les routes entre deux points : pathfinding
6. [x] Fonction d'évaluation complète du ratio entre les distances "directes" et les distances
par les routes

### III. Ajouts futurs
1. [x] Affichage et interaction avec une surface : Framework Qt
2. [x] Créer une fonction de voisinage sur une solution afin d'effectuer une recherche locale
3. [x] Ajouter des fonctions de mise à jour des valeurs et de l'évaluation d'une solution
4. [x] Générer et évaluer des solutions et créer le front Pareto, puis l'afficher
    - [x] Garder les valeurs d'évaluations des solutions dominées pour pouvoir les afficher en plus du front Pareto

### IV. Améliorations et optimisations possibles
+ [ ] Stocker les voisinages dans un vecteur de listes (avec couples : valeur-voisin ?)
+ [x] Stocker les valeurs de distance par routes entre chaque parcelle et chaque autre parcelle ?
Ou entre chaque parcelles et ses suivantes ? J'utilise une matrice à 4 dimensions
+ [ ] Transformer la matrice en un graphe ? pour mplémenter des algorithmes de type Dijkstra (boost-graph ?)
+ [ ] Corriger les bugs 
    + [x] Des boutons reset et flush (revoir qui reset, ce qui est reset et sauvegardé et comment on le fait)
    + [ ] Vérifier que l'évaluation est correcte
    + [ ] Pourquoi ce n'est pas toujours la meilleure route qui est choisie (apparition de "patés")

### V. Fonctionnalitées envisagées
+ [ ] Prendre en compte la largeur des routes
+ [ ] Changer la forme des cellules : hexagonales ...
+ [ ] Ajouter des coefficients/couts sur les cellules et routes
+ [ ] L'utilisateur demande un nombre de routes à ajouter 
    - [x] Pour maximiser le nombre d'utilisables
    - [x] Pour augmenter l'accessibilité
    - [ ] Pour améliorer les deux critères (avec une valuation des critères ? avec un alpha et un beta ?)
    - [ ] Avec une planification sur le nombre de routes demandé, plutôt que des algos gloutons
+ [x] Ajouter des couleurs à l'affichage d'une surface, selon les valeurs attribuées ou selon, pour répérer les "points chauds" : points qui on un rapport élevé distance "directe"/distance_Manhattan, avec la plus grande mauvaise accessibilité
    + [x] Couleurs selon l'écart avec la moyenne
    + [ ] Valeurs d'écart avec la moyenne en sur-impression ?
+ [ ] Essayer différentes techniques : Vider la matrice au lieu de remplir (pb d'évaluation), algos génétique, ...
+ [ ] Ajouter des types et contraintes de placement des parcelles
+ [ ] Sauvegarde et chargement d'une solution ?


## Algorithme et détailes techniques :
### Algorithme
- Pour ratio entre distance directe et par les routes, faire une moyenne ? un maximum ? garder total à coté ? Moyenne pour l'instant  (OK : Moyenne quadratique et ratio max possibles)

### Questions :
- Dans listes (`list<Coordinates>*`) utiliser pointeurs ? si oui, comment utiliser,
ou remplacer, `find(begin, end, val)` ?

## Autres :
+ Seulement 2 entrées/sorties : 3 Cas pour relier : ligne droite, ligne avec 2 coudes, Equerre (ligne avec 1 coude (ou 2n +1 ?)).
+ Recherche locale : incrémentale
+ On peut utiliser la distance euclidienne au lieu de la distance Manhattan.
+ Distance par les routes : on passe par les routes les plus proches de la parcelle
+ Pour définir exploitable, je vérifie si chaque parcelle a une route voisine et l'inverse lors des mises à jour. Définir toutes les voisins de chaque route supprimée ou ajoutée comme étant exploitable.
+ S'est posée la question de comment trouver les routes à X parcelles de distance d'une parcelle donnée ?
    - Actuellement, une recherche dans un de 2*X parcelles de coté est effectuée, 
    on garde les routes qui sont à moins de 2 de distance.
    - Faudrait-il stocker les routes voisines d'une parcelle dans un vecteur de listes de paires d'une coordonnée et d'une distance ? ou juste une coordonnée ? OK : plutôt oui

## Authors and Contributors
Projet développé par @flodavid. Utilise gnuplot-cpp, développé par Jeremy Conlin (jeremit0@gmail.com)

## Support or Contact
Vous pouvez me contacter à l'adresse [fl.david.53@gmail.com](mailto:fl.david.53@gmail.com)

_Urbanisme_ est un logiciel libre, licencié sous la [GPLv3](https://www.gnu.org/licenses/gpl.html).
_Urbanisme_ is free software, licensed under the [GPLv3](https://www.gnu.org/licenses/gpl.html).
