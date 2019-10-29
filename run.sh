#!/bin/bash
FILE=input.s
if test -f "$FILE"; then
	rm input.s
fi
./a.out input.txt
if test -f "$FILE"; then
	gcc -c input.s printi.c
	gcc input.o printi.o -o input
	./input
fi