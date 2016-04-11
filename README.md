Urbanisme
=========
TER visant à l'optimisation du placement de routes sur une surface, en maximisant les zones exploitables et leur accessibilité.

![SIM-CITY](http://eaassets-a.akamaihd.net/prod.simcity.com/sites/all/themes/prod-build/img/logo-simcity.png)

# Organisation :

[![Build Status](https://travis-ci.org/atom/atom.svg?branch=master)](https://travis-ci.org/atom/atom)
[![Build status](https://ci.appveyor.com/api/projects/status/1tkktwh654w07eim?svg=true)](https://ci.appveyor.com/project/Atom/atom)
[![Dependency Status](https://david-dm.org/atom/atom.svg)](https://david-dm.org/atom/atom)


## Tâches : 

### I. Etudes préliminaires :
1. Réflexions sur les applications et enjeux du problème 
2. Recherche de travaux similaires
3. Réflexion sur les langages, outils et approches du problème

### II. Organisation du travail et du développement
1. Utilisation d'une classe Coordonnées, d'une classe représentant le terrain et d'une classe représentant les parmètres du problème 
2. Ajout des entrées et sorties et d'une matrice d'états de la surface
3. Fonction d'évaluation complète du nombre de parcelles exploitables d'une solution
4. Calcul de la distance "directe" entre deux points, Manhattant et Euclidienne, avec tests correspondants
5. Calcul de la distance par les routes entre deux points
6. Fonction d'évaluation complète du ratio entre les distances "directes" et les distance par les routes

### III. Ajouts futurs
1. Affichage d'une solution (Qt envisagé)
2. Transformer la matrice en un graphe
3. Générer des solutions et créer le front Pareto correspondant
4. Créer une fonction de voisinage sur une solution afin d'effectuer une recherche localeAjouter des fonctions de mise à jour
5. des valeurs d'évaluation d'une solution
6. Transformer la matrice en graphe
7. Implémenter des algorithmes de type Dijkstra (boost-graph ?)

### IV. Fonctionnalitées envisagées
+ Changer la forme des cellules : hexagonales ...
+ Ajouter des coefficients/couts sur les cellules et routes
+ Ajouter des types et contraintes de placement des parcelles
