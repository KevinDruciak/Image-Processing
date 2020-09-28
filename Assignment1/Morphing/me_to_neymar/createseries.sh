#!/bin/bash

FILE=".jpg"
THRESHOLD=0
while [ $THRESHOLD -le 9 ]
do
	../../../Assignment1.exe $1 $2 $3 $4 $5 $THRESHOLD $6 $7$THRESHOLD$FILE
	echo $1 
	echo $2 
	echo $3 
	echo $4 
	echo $5 
	echo $THRESHOLD 
	echo $6 
	echo $7$THRESHOLD$FILE
	THRESHOLD=$((THRESHOLD + 1))
done
echo All done
$SHELL
