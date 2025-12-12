#!/bin/bash

w=$1
h=$2
iter=$3
count=0
while (( count < iter )) 
do
	echo ----------------------
	echo "$4" | ./life $w $h $count
	(( count++ ))
done
echo
