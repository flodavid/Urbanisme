Urbanisme
=========
# TER visant à l'optimisation du placement de routes sur une surface, en maximisant
les zones exploitables et leur accessibilité.


            Compilation Travis                   |                  License                     |
------------------------------------------------ | -------------------------------------------- |
[![Build Status](https://travis-ci.org/flodavid/Urbanisme.svg?branch=master)](https://travis-ci.org/flodavid/Urbanisme) | [![GPLv3 License](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://github.com/flodavid/Urbanisme/blob/master/COPYING.txt)

[![SIM-CITY](http://eaassets-a.akamaihd.net/prod.simcity.com/sites/all/themes/prod-build/img/logo-simcity.png)](https://flodavid.github.io/Urbanisme)

## Organisation du travail et du développement
---------------

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
4. [ ] Générer des solutions et créer le front Pareto correspondant : En cours

### IV. Améliorations possibles
+ [ ] Stocker les voisinages dans un vecteur de listes (avec couples : valeur-voisin ?)
+ [x] Stocker les valeurs de distance par routes entre chaque parcelle et chaque autre parcelle ?
Ou entre chaque parcelles et ses suivantes ? J'utilise une matrice à 4 dimensions
+ [ ] Transformer la matrice en un graphe ?
    + Implémenter des algorithmes de type Dijkstra (boost-graph ?)

### V. Fonctionnalitées envisagées
+ [ ] Essayer différentes techniques : Vider la matrice au lieu de remplir (pb d'évaluation), algos génétique, ...
+ [ ] Ajouter une largeur pour les routes
+ [ ] Changer la forme des cellules : hexagonales ...
+ [ ] Ajouter des coefficients/couts sur les cellules et routes
+ [ ] Ajouter des couleurs à l'affichage, selon les valeurs attribuées ou selon
les "points chauds", qui on un rapport élevé distance "directe"/distance_Manhattan 
+ [ ] Ajouter des types et contraintes de placement des parcelles

## A faire :
+ [ ] L'utilisateur demande un nombre de routes à ajouter 
    - [x] Pour maximiser le nombre d'utilisables
    - [x] Pour augmenter l'accessibilité
    - [ ] Pour améliorer les deux critères (avec une valuation des critères ? avec un alpha et un beta ?)
    - [ ] Avec une planification sur le nombre de routes demandé, plutôt que des algos gloutons
+ [ ] Corriger les bugs 
    + [x] Des boutons reset et flush (revoir qui reset, ce qui est reset et sauvegardé et comment on le fait)
    + [ ] Vérifier que l'évaluation est correcte
    + [ ] Pourquoi ce n'est pas toujours la meilleure route qui est choisie (apparition de "patés")
+ [ ] Front pareto et collection solution
    - [x] Ajouter des solutions dans le front Pareto lors de la recherche de meilleures solutions
    - [ ] Garder les valeurs d'évaluations des solutions dominées pour pouvoir les afficher en plus du front Pareto ?
+ [x] Afficher les points chauds : points avec la plus grande moyenne d'accessibilité avec les autres
    + [x] Couleurs selon l'écart avec la moyenne 
    + Valeurs de la moyenne avec les autres cellules ?
    
+ [ ] Sauvegarde et chargement d'une solution ?

## Algorithme/Objectifs
- Pour ratio entre distance directe et par les routes, faire une moyenne ? un maximum ? garder total à coté ? Moyenne pour l'instant  (OK : Moyenne quadratique et ratio max possibles)

## Code/Technique :
- Dans listes (`list<Coordinates>*`) utiliser pointeurs ? si oui, comment utiliser,
ou remplacer, `find(begin, end, val)` ?
- Faire une listes de routes ?
- Comment trouver les routes à X parcelles de distance d'une parcelle donnée ?
    - Actuellement, une recherche dans un de 2*X parcelles de coté est effectuée, 
    on garde les routes qui sont à moins de 2 de distance.
    - Faudrait-il stocker les routes voisines d'une parcelle dans un vecteur de listes de paires d'une coordonnée et d'une distance ? ou juste une coordonnée ? OK : plutôt oui

### Profiler :
+ Gprof (gnu profiler)
+ Kcachegrind

## Autres :
+ Seulement 2 entrées/sorties : 3 Cas pour relier : ligne droite, ligne avec 2 coudes, Equerre (ligne avec 1 coude (ou 2n +1 ?)).
+ Recherche locale : incrémentale
+ On peut utiliser la distance euclidienne au lieu de la distance Manhattan.
+ Distance par les routes : on passe par les routes les plus proches de la parcelle
+ Largeur routes ?? La largeur des routes n'est prise en compte dans aucun calcul, cela entraine une lenteur importante lors du pathfinding entre parcelles, par les routes, si on utilise une largeur supérieure à 1.
+ Pour définir exploitable, je vérifie si chaque parcelle a une route voisine et l'inverse lors des mises à jour. Définir toutes les voisins de chaque route supprimée ou ajoutée comme étant exploitable.

_Urbanisme_ is free software, licensed under the [GPLv3](https://www.gnu.org/licenses/gpl.html).
