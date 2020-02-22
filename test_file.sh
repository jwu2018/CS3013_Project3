#!/bin/sh
make clean
make 
for i in 1 2 3 4 5 6 7 8 9 10
do
	echo Running study \# $i
	./study >> study_output.txt
done