#!/bin/bash

./a.out input.txt
gcc -c input.s
gcc input.o printi.o -o input
./input