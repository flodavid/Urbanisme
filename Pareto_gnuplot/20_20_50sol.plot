set term jpeg

set output 'resolutionPareto20_20_50sol.jpeg'
plot '20_20_50sol.pareto.txt' lc rgb "red", '20_20_50sol.evaluations.txt' lc rgb "black"
