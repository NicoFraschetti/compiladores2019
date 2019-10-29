%{

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util/ast_utilities.h"
#include "util/symbol_table_utilities.h"
#include "util/offset_generator.h"
#include "calc-sintaxis.tab.h"

%}

%option noyywrap
%option yylineno

letra [a-zA-z]
digito [0-9]

%%

var						  	  	{ 	return VAR; 	}

printi						  	{ 	return PRINTI; 	}

printb							{	return PRINTB;	}

true						  	{ 	return TRUE; 	}

false						  	{ 	return FALSE; 	}

int 						  	{ 	return INTEGER; }

bool						  	{ 	return BOOL; 	}

while						  	{ 	return WHILE; 	}

if 							  	{	return IF;		}

else  						  	{	return ELSE;	}

{digito}+                     	{ 
									Info *info = createNodeInfo(NULL, atoi(yytext),-1,"int");
									yylval.node = createNode(NULL,NULL,info,"int"); 
                                	return INT;
                            	}

{letra}({letra}|{digito})*    	{ 
									char *name = (char *) malloc(sizeof(yytext)*yyleng);
                                	strcpy(name,yytext);
                                	Info *info = findNode(name,symTblLevel());
                                	if (info == NULL)
                                		info = createNodeInfo(name,-1,-1,"var");
                                	yylval.node = createNode(NULL,NULL,info,"var");
                                	return ID;
                                }

[-+*/%;()=!><{}]              	{ 	return *yytext;	  }

"&&"							{ return AND;  	}

"||"							{ return OR;	}

"=="							{ return EQ;	}	

"//".*\n                      	;

.|\n                          	; /* ignore all the rest */

%%

char *fileName;

char *getName(){
	return fileName;
}

void yyerror(){
	printf("%s%d\n","-> ERROR Sintactico en la linea: ",yylineno);
}

int main(int argc,char *argv[]){
	++argv,--argc;
	if (argc > 0){
		fileName = (char *) malloc(sizeof(char)*20);
		strcpy(fileName,argv[0]);
		yyin = fopen(argv[0],"r");
	}
	else
		yyin = stdin;
	yyparse();

}
