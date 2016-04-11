# Urbanisme
TER visant à l'optimisation du placement de routes sur une surface, en maximisant les zones exploitables et leur accessibilité.

## Organisation :

### Tâches : 

1. Etudes préliminaires :
+ Réflexions sur les applications et enjeux du problème 
+ Recherche de travaux similaires
+ Réflexion sur les langages, outils et approches du problème

2. Organisation du travail et du développement
+ Utilisation d'une classe Coordonnées, d'une classe représentant le terrain
et d'une classe représentant les parmètres du problème
+ Ajout des entrées et sorties et d'une matrice d'états de la surface
+ Fonction d'évaluation complète du nombre de parcelles exploitables d'une solution
+ Calcul de la distance "directe" entre deux points, Manhattant et Euclidienne, avec tests correspondants
+ Calcul de la distance par les routes entre deux points
+ Fonction d'évaluation complète du ratio entre les distances "directes" et les distance par les routes


3. Ajouts futurs
+ Affichage d'une solution (Qt envisagé)
+ Transformer la matrice en un graphe
+ Générer des solutions et créer le front Pareto correspondant
+ Créer une fonction de voisinage sur une solution afin d'effectuer une recherche locale
+ Ajouter des fonctions de mise à jour des valeurs d'évaluation d'une solution
+ Transformer la matrice en graphe
+ Implémenter des algorithmes de type Dijkstra (boost-graph ?)

4. Fonctionnalitées envisagées
+ Changer la forme des cellules : hexagonales ...
+ Ajouter des coefficients/couts sur les cellules et routes
+ Ajouter des types et contraintes de placement des parcelles
