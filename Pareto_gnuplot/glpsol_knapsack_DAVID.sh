#!/bin/sh

#to change student to edit STUDENT_NAME
STUDENT_NAME="DAVID_Florian"

FILES_PATH="/resources"

echo "Apply glpsol on ILP formulation on -knapsack_problem.ilp- file write in  MathProlog language"
echo ""

file="./$STUDENT_NAME.data.ilp"
if [ -e $file ]; then
	echo "Data file exists, launching glpsol ..."
else 
	echo "Data file does not exists, creating it with perl script ..."
	echo ""
	perl .$FILES_PATH/txt_to_data-ilp.pl .$FILES_PATH/$STUDENT_NAME.txt .$FILES_PATH/$STUDENT_NAME.data.ilp
	echo ""
	echo ""
	echo "Launching glpsol ..."
fi 

glpsol --math .$FILES_PATH/knapsack_problem.ilp --data .$FILES_PATH/$STUDENT_NAME.data.ilp --wlp .$FILES_PATH/out.cplex

#echo ""
#echo "---Display file output : ---"
#echo ""

#cat out.cplex
