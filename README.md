Urbanisme
=========
TER visant à l'optimisation du placement de routes sur une surface, en maximisant
les zones exploitables et leur accessibilité.


            Compilation Travis                   |                      License                 |
------------------------------------------------ | -------------------------------------------- |
[![Build Status](https://travis-ci.org/flodavid/Urbanisme.svg?branch=master)](https://travis-ci.org/flodavid/Urbanisme) | [![GPLv3 License](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://github.com/flodavid/Urbanisme/blob/master/COPYING.txt)
 
![SIM-CITY](http://eaassets-a.akamaihd.net/prod.simcity.com/sites/all/themes/prod-build/img/logo-simcity.png)

## Organisation
---------------

### I. Etudes préliminaires :
1. [x] Réflexions sur les applications et enjeux du problème 
2. [x] Recherche de travaux similaires
3. [x] Réflexion sur les langages, outils et approches du problème

### II. Organisation du travail et du développement
1. [x] Utilisation d'une classe Coordonnées, d'une classe représentant le terrain
et d'une classe représentant les paramètres du problème 
2. [x] Ajout des entrées et sorties et d'une matrice d'états de la surface
3. [x] Fonction d'évaluation complète du nombre de parcelles exploitables d'une solution
4. [x] Calcul de la distance "directe" entre deux points, Manhattant et Euclidienne,
avec tests correspondants
5. [ ] Calcul de la distance par les routes entre deux points
6. [ ] Fonction d'évaluation complète du ratio entre les distances "directes" et les distances
par les routes

### III. Ajouts futurs
1. [ ] Affichage d'une solution (Qt envisagé)
2. [ ] Transformer la matrice en un graphe
3. [ ] Générer des solutions et créer le front Pareto correspondant
4. [ ] Créer une fonction de voisinage sur une solution afin d'effectuer une recherche locale
5. [ ] Ajouter des fonctions de mise à jour des valeurs et de l'évaluation d'une solution
6. [ ] Transformer la matrice en graphe
7. [ ] Implémenter des algorithmes de type Dijkstra (boost-graph ?)

### IV. Améliorations possibles
+ [ ] Stocker les voisinages dans un vecteur de listes (avec couples : valeur-voisin ?)
+ [ ] Stocker les valeurs de distance par routes entre chaque parcelle et chaque autre parcelle ?
Ou entre chaque parcelles et ses suivantes ? (couple : valeur-voisin ? comment implémenter pour 
accéder à la valeur du voisin ?)

### V. Fonctionnalitées envisagées
+ [ ] Changer la forme des cellules : hexagonales ...
+ [ ] Ajouter des coefficients/couts sur les cellules et routes
+ [ ] Ajouter des couleurs à l'affichage, selon les valeurs attribuées ou selon
les "points chauds", qui on un rapport élevé distance "directe"/distance_Manhattan 
+ [ ] Ajouter des types et contraintes de placement des parcelles


## Détails :
+ On suppose que l'on est obligé de prendre la route la plus proche de la parcelle
pour se déplacer

## Questions :
### Algorithme/Objectifs
- Comment trouver les routes à X parcelles de distance d'une parcelle donnée ?
    - Actuellement, une recherche dans un de 2*X parcelles de coté est effectuée, 
    on garde les routes qui sont à moins de 2 de distance. OK : Parcourir les routes
    - Faudrait-il stocker les routes voisines d'une parcelle dans un vecteur de listes
     de paires d'une coordonnée et d'une distance ? ou juste une coordonnée ? OK : plutôt oui
- Les E/S sont-elles bien des routes ? OK : Oui
- Pour II.6, faire une moyenne ? un maximum ? garder total à coté ? OK : Moyenne pour l'instant
    - Noté : moyenne 80 % (OK : Moyenne quadratique)
    - Utiliser également distance euclidienne ? OK : plus tard
    - Distance par les routes : compte la distance entre la parcelle et la première route ? OK : faire optimisation : 
        - [ ] Prendre les routes les plus proches d'abord
        - [ ] 
- Transformer en graphe (voir Code 2. et 4. )
- Collection solutions, comment démarrer ? aléatoire ? voisinage ? Comment relier les E/S ?

### Code/Technique :
- Dans listes (`list<Coordinates>*`) utiliser pointeurs ? si oui, comment utiliser,
ou remplacer, `find(begin, end, val)` ? => Non demandé
- Utiliser boost-graph ? Bof
- Largeur routes ?? Non 
- Faire une listes de routes ? Pour définir exploitable, je vérifie que chaque route
a un voisin (en cherchant tous ses voisins, alors qu'on pourrait s'arrêter au premier),
l'inverse est possible : définir toutes les voisins de chaque route comme étant exploitable
- servingDistance est unsigned, comment l'utiliser dans les boucles avec des Coordinates
qui ont des entiers ? je crée une variable locale


## Profiler :
+ Gprof (gnu profiler)
+ Kcachegrind

#### Autres :
+ Seulement 2 entrées/sorties : 3 Cas pour relier : ligne droite, ligne avec 2 coudes, Equerre (ligne avec 1 coude (ou 2n +1 ?))
+ Recherche locale : incrémentale


_Urbanisme_ is free software, licensed under the [GPLv3](https://www.gnu.org/licenses/gpl.html).
