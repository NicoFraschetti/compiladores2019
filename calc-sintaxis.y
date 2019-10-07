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

char *getName();

char *currentType;

%}
 
%union { struct TreeNode *node;}
 
%token<node> INT ID PRINTI
%token VAR AND OR INTEGER BOOL TRUE FALSE

%type<node> program decls decl statements statement expr

%left AND OR
%left '!'
%left EQ '>' '<' 
%left '+' '-' 
%left '*' '/' '%'

 
%%

program:
    decls statements   { $$ = createNode($1,$2,NULL,"next");
                         generateDot($$, getName());
                         //generateAssembly($$, getName());
                       }
    ;

decls: 
    decls decl             {    $$ = createNode($1,$2,NULL,"next"); }
    |                      {    $$ = NULL; }
    ;
decl:
    VAR type ID ';'        {

                                insertInTable($3->info->name,-1,0,$3->info->offSet,currentType);
                                $$ = NULL;

                           }
    | VAR type ID '=' expr ';'  { 
                                insertInTable($3->info->name,evalTree($5),1,$3->info->offSet,currentType);
                                $$ = createNode($3,$5,NULL,"asig"); 
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
                                    //printf("resultado es %d\n",evalTree($$->leftChild)); 
                                } 
    ; 

expr:
    INT         {   $$ = createNode(NULL,NULL,createNodeInfo(NULL,$1->info->value,-1),"int"); }
    | ID        {   
                    //printf("$1->info->offSet=%d, $1->info->name=%s\n", $1->info->offSet, $1->info->name);
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
    | TRUE              {   $$ = createNode(NULL,NULL,createNodeInfo(NULL,1,-1),"bool"); }
    | FALSE             {   $$ = createNode(NULL,NULL,createNodeInfo(NULL,0,-1),"bool"); }
    | expr '+' expr     {   $$ = createNode($1,$3,NULL,"add"); }
    | expr '-' expr     {   $$ = createNode($1,$3,NULL,"sub"); }
    | expr '*' expr     {   $$ = createNode($1,$3,NULL,"mul"); }
    | expr '/' expr     {   $$ = createNode($1,$3,NULL,"div"); }
    | expr '%' expr     {   $$ = createNode($1,$3,NULL,"mod"); }
    | '-' expr          {   Info *info = createNodeInfo(NULL, -1,-1);
                            $$ = createNode($2,createNode(NULL,NULL,info,"int"),NULL,"mul");
                        }

    | '(' expr ')'      {  $$ = $2; }
    | expr '<' expr     {  $$ = createNode($1,$3,NULL,"less"); }
    | expr '>' expr     {  $$ = createNode($1,$3,NULL,"greater"); }
    | expr EQ expr      {  $$ = createNode($1,$3,NULL,"equal"); }
    | expr AND expr     {  $$ = createNode($1,$3,NULL,"and"); }
    | expr OR expr      {  $$ = createNode($1,$3,NULL,"or"); }
    | '!' expr          {  $$ = createNode($2,NULL,NULL,"not"); }

    ;
type:
    INTEGER             {   currentType = "integer";    }
    | BOOL              {   currentType = "bool";       }
    ;

%%


