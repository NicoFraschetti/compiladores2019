%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "types.h"
#include "symbol_table_utilities.h"
#include "ast_utilities.h"
#include "calc-sintaxis.tab.h"

%}

%option noyywrap
%option yylineno

letra [a-zA-z]
digito [0-9]

%%

var						  	  {	return VAR; }

printi						  { return PRINTI; }

{digito}+                     { 
								Info *info = createNodeInfo(NULL, atoi(yytext));
								yylval.node = createNode(NULL,NULL,info,"int");
                                //printf("INT : %d\n",atoi(yytext)); 
                                return INT;}

{letra}({letra}|{digito})*    { char *name = (char *) malloc(sizeof(yytext)*yyleng);
                                strcpy(name,yytext);
                                Info *info = createNodeInfo(name,-1);
                                yylval.node = createNode(NULL,NULL,info,"var");
                                //printf("ID : %s\n",yytext);
                                return ID;}

[-+*/%;()=]                   	  { return *yytext;}

"//".*\n                      ;

.|\n                          ; /* ignore all the rest */

%%

void yyerror(){
	printf("%s%d\n","-> ERROR Sintactico en la linea: ",yylineno);
}

int main(int argc,char *argv[]){
	++argv,--argc;
	if (argc > 0)
		yyin = fopen(argv[0],"r");
	else
		yyin = stdin;

	yyparse();

}