#!/bin/bash

FILE=".jpg"
FILE2=".txt"
DASH="/"
DASH2="-"
THRESHOLD=0
COUNT=1
COUNT2=2
TOTAL=0
while [ $COUNT -le 16 ]
do
	THRESHOLD=0
	while [ $THRESHOLD -le 24 ]
	do
		../../../Assignment1.exe $1 $2$COUNT$FILE $3 $4$COUNT2$FILE $5$COUNT$FILE2 $THRESHOLD $6 $7$TOTAL$FILE  #$7$COUNT$DASH2$COUNT2$DASH$COUNT$THRESHOLD$FILE
		echo $1 
		echo $2 
		echo $3 
		echo $4 
		echo $5 
		echo $THRESHOLD 
		echo $6 
		echo $7$THRESHOLD$FILE
		echo $COUNT 
		THRESHOLD=$((THRESHOLD + 1))
		TOTAL=$((TOTAL + 1))
	done
	COUNT=$((COUNT + 1))
	COUNT2=$((COUNT2 + 1))
done

echo All done
$SHELL
