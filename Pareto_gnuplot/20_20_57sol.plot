set term jpeg

set output 'resolutionPareto20_20_57sol.jpeg'
plot '20_20_57sol.pareto.txt' lc rgb "red", '20_20_57sol.evaluations.txt' lc rgb "black"