%{

#include <stdlib.h>
#include <stdio.h>
#include "util/symbol_table_utilities.h"
#include "util/ast_utilities.h"
#include "util/cod_3D_list_utilities.h"
#include "util/offset_generator.h"

void yyerror(char *);
int yylex(void);

char *getName();

char *currentType;

%}
 
%union { struct TreeNode *node;}
 
%token<node> INT ID PRINTI
%token VAR AND OR INTEGER BOOL TRUE FALSE WHILE IF ELSE

%type<node> program decls decl statements statement expr block  

%left AND OR
%left '!'
%left EQ '>' '<' 
%left '+' '-' 
%left '*' '/' '%'

 
%%     

prog:
    program                 {
                                //printf("Synstax tree type = %s \n", checkTypesCorrectnes($1));
                                //generateDot($1,"dot_output.dot");
                                //printSymbolTable();
                                generateAssembly($1,getName());
                                //generateCod3DList($1);
                                //printCod3DList();
                            }
    ;
program:
    decls statements        {    $$ = createNode($1,$2,NULL,"next"); }
    ;

decls: 
    decls decl              {    $$ = createNode($1,$2,NULL,"next"); }
    |                       {    $$ = NULL; }
    ;
decl:
    VAR type ID ';'         {    
                                 if ($3->info->offSet == -1){
                                    $3->info->offSet = getOffSet();
                                    $3->info->level = symTblLevel();
                                    insertInTable($3->info->name,-1,1,$3->info->offSet,currentType,symTblLevel());
                                 }
                                 else{
                                    Info *info = createNodeInfo($3->info->name,-1,getOffSet(),"var");
                                    info->level = symTblLevel();
                                    $3->info = info;
                                    insertInTable($3->info->name,-1,1,$3->info->offSet,currentType,symTblLevel());
                                 }
                                 $$ = NULL;
                            }
    | VAR type ID '=' expr ';'  
                            {    
                                 if ($3->info->offSet == -1){
                                    $3->info->offSet = getOffSet();
                                    $3->info->level = symTblLevel();
                                    insertInTable($3->info->name,evalTree($5),1,$3->info->offSet,currentType,symTblLevel());
                                 }
                                 else{
                                    Info *info = createNodeInfo($3->info->name,-1,getOffSet(),"var");
                                    info->level = symTblLevel();
                                    $3->info = info;
                                    insertInTable($3->info->name,evalTree($5),1,$3->info->offSet,currentType,symTblLevel());
                                 }
                                 $$ = createNode($3,$5,NULL,"asig"); 
                            }
    ;

statements:
    statements statement    {    $$ = createNode($1,$2,NULL,"next"); }
    |                       {    $$ = NULL; }                 
    ;

statement: 
    ID '=' expr ';'         {   
                                 updateTable($1->info->name,evalTree($3));
                                 $$ = createNode($1,$3,NULL,"asig"); 
                            }
    | PRINTI '(' expr ')' ';'   
                            {    $$ = createNode($3,NULL,NULL,"printi"); }
    | IF '(' expr ')' block ELSE block 
                            {    $$ = createNode($3,createNode($5,$7,NULL,"if_else"),NULL,"if"); }
    | IF '(' expr ')' block {    $$ = createNode($3,$5,NULL,"if"); }
    | WHILE '(' expr ')' block         
                            {    $$ = createNode($3,$5,NULL,"while"); }
    ; 

block: 
    {   incSymTblLevel();   }
    '{' program '}'         
    {   decSymTblLevel();   }   {    $$ = $3; }
    | ';'                       {    $$ = createNode(NULL,NULL,NULL,"next"); }
    ;

expr:
    INT         {   $$ = createNode(NULL,NULL,createNodeInfo(NULL,$1->info->value,-1,"int"),"int"); }
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
    | TRUE              {   $$ = createNode(NULL,NULL,createNodeInfo(NULL,1,-1,"bool"),"bool"); }
    | FALSE             {   $$ = createNode(NULL,NULL,createNodeInfo(NULL,0,-1,"bool"),"bool"); }
    | expr '+' expr     {   $$ = createNode($1,$3,NULL,"add"); }
    | expr '-' expr     {   $$ = createNode($1,$3,NULL,"sub"); }
    | expr '*' expr     {   $$ = createNode($1,$3,NULL,"mul"); }
    | expr '/' expr     {   $$ = createNode($1,$3,NULL,"div"); }
    | expr '%' expr     {   $$ = createNode($1,$3,NULL,"mod"); }
    | '-' expr          {   
                            Info *info = createNodeInfo(NULL, -1,-1,"int");
                            $$ = createNode($2,createNode(NULL,NULL,info,"int"),NULL,"mul");
                        }
    | '(' expr ')'      {   $$ = $2; }
    | expr '<' expr     {   $$ = createNode($1,$3,NULL,"less"); }
    | expr '>' expr     {   $$ = createNode($1,$3,NULL,"greater"); }
    | expr EQ expr      {   $$ = createNode($1,$3,NULL,"equal"); }
    | expr AND expr     {   $$ = createNode($1,$3,NULL,"and"); }
    | expr OR expr      {   $$ = createNode($1,$3,NULL,"or"); }
    | '!' expr          {   $$ = createNode($2,NULL,NULL,"not"); }
    ;
type:
    INTEGER             {   currentType = "int"; }
    | BOOL              {   currentType = "bool"; }
    ;

%%


