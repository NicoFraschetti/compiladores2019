#!/bin/bash

flex calc-lexico.l 
bison -d calc-sintaxis.y
gcc lex.yy.c calc-sintaxis.tab.c symbol_table_utilities.c ast_utilities.c