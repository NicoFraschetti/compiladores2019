%{

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "symbol_table_utilities.h"
#include "ast_utilities.h"
#include "cod_3D_list_utilities.h"
#include "offset_generator.h"
void yyerror(char *);
int yylex(void);

%}
 
%union { struct TreeNode *node;}
 
%token<node> INT ID PRINTI
%token VAR

%type<node> program decls decl statements statement expr
 
%left '+' '-' 
%left '*' '/' '%'

 
%%

program:
    decls statements   { $$ = createNode($1,$2,NULL,"next");
                         generateCod3DList($$);
                         printCod3DList(); 
                       }
    ;

decls: 
    decls decl             {    $$ = createNode($1,$2,NULL,"next"); }
    |                      {    $$ = NULL; }
    ;
decl:
    VAR ID ';'             {

                                insertInTable($2->info->name,-1,0,getOffSet());


                           }
    | VAR ID '=' expr ';'  { 
                                insertInTable($2->info->name,evalTree($4),1,getOffSet());
                                $$ = createNode($2,$4,NULL,"asig"); 
                           }
    ;

statements:
    statements statement        {   $$ = createNode($1,$2,NULL,"next"); }
    | statement                 
    ;

statement: 
    ID '=' expr ';'             {   
                                    updateTable($1->info->name,evalTree($3));

                                    $$ = createNode($1,$3,NULL,"asig"); 
                                }
    | PRINTI '(' expr ')' ';'   {   $$ = createNode($3,NULL,NULL,"printi");
                                    printf("resultado es %d\n",evalTree($$->leftChild)); 
                                } 
    ; 

expr:
    INT         {   $$ = createNode(NULL,NULL,createNodeInfo(NULL,$1->info->value),"int"); }
    | ID        { 
                    ListNode *aux = findListNode($1->info->name);
                    if (aux==NULL){
                        printf("Undeclared Variable %s\n", $1->info->name);
                        exit(1);
                    }
                    if (aux->initialized==0){
                        printf("Unitialized Variable %s\n", $1->info->name);
                        exit(1);   
                    }
                    $$ = createNode(NULL,NULL,aux->info,"var");
                  
                }
    | expr '+' expr     {   $$ = createNode($1,$3,NULL,"add"); }
    | expr '-' expr     {   $$ = createNode($1,$3,NULL,"sub"); }
    | expr '*' expr     {   $$ = createNode($1,$3,NULL,"mul"); }
    | expr '/' expr     {   $$ = createNode($1,$3,NULL,"div"); }
    | expr '%' expr     {   $$ = createNode($1,$3,NULL,"mod"); }
    | '-' expr          {   Info *info = createNodeInfo(NULL, -1);
                            $$ = createNode($2,createNode(NULL,NULL,info,"int"),NULL,"mul");
                        }

    | '(' expr ')'      { $$ = $2; }
    ;

%%


