#!/bin/bash

FILE= $1
echo File : $FILE

gnuplot <<- EOF
    set xlabel "Nombre d'exploitables"
    set ylabel "Moyenne des ratios d'accessibilité"
    set term jpeg
    set output "${FILE.jpg"
    plot "${FILE}" using 1:2:3:4 with errorbars title "Graph title"
EOF

# Suppression de l'extension .txt
#echo File : $1

#EXTENSION=jpg
#FILE= $1

#FILENAME= ${FILE:(-5)}
#echo Filename : $FILENAME

#JPEG_FILE= $FILENAME
#JPEG_FILE+= $EXTENSION

#echo Output : $JPEG_FILE

#gnuplot
#set term jpeg

#set output $JPEG_FILENAME
#plot $1
