%{

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "util/symbol_table_utilities.h"
#include "util/ast_utilities.h"
#include "util/cod_3D_list_utilities.h"
#include "util/offset_generator.h"
#include "util/label_stack.h"

void yyerror(char *);
int yylex(void);

char *getName();

char *currentType;

%}
 
%union { struct TreeNode *node;}
 
%token<node> INT ID PRINTI PRINTB
%token VAR AND OR INTEGER BOOL TRUE FALSE WHILE IF ELSE MAIN EXTERN RETURN

%type<node> program decls decl statements statement expr block init func funcList paramList exprList  

%left AND OR
%left '!'
%left EQ '>' '<' 
%left '+' '-' 
%left '*' '/' '%'

 
%%

prog:
    init                    {   
                                generateDot($1,"dot_output.dot");
                                printSymbolTable();
                                //generateCod3DList($1);
                                //printCod3DList(); 
                            }  

init:
    decls funcList          {   $$ = createNode($1,$2,NULL,"next"); }
    | funcList              {   $$ = createNode(NULL,$1,NULL,"next"); }
    ; 

funcList:
    func funcList           {   $$ = createNode($1,$2,NULL,"next"); }
    | INTEGER MAIN '(' ')' '{' program '}' {  $$ = createNode($6,NULL,NULL,"main"); }
    ;

func:
    type ID '(' paramList ')' block  {
                                        decSymTblLevel();   
                                        Info *info = createNodeInfo($2->info->name,-1,-1,currentType);
                                        info->level = symTblLevel();
                                        insertInTable(info->name,1,getOffSet(),info->type,info->level,$4,"function");
                                        $$ = createNode($4,$6,info,"function"); 
                                     }
    | type ID '(' ')' block          {  
                                        Info *info = createNodeInfo($2->info->name,-1,-1,currentType);
                                        info->level = symTblLevel();
                                        insertInTable(info->name,1,getOffSet(),info->type,info->level,NULL,"function");
                                        $$ = createNode(NULL,$5,info,"function"); 
                                     }
    | EXTERN ID '(' paramList ')' ';' { 
                                        Info *info = createNodeInfo($2->info->name,-1,-1,"extern");  
                                        $$ = createNode($4,NULL,info,"function"); 
                                      }
    | EXTERN ID '(' ')' ';'          {  
                                        Info *info = createNodeInfo($2->info->name,-1,-1,"extern");
                                        $$ = createNode(NULL,NULL,info,"function"); 
                                     }

paramList:
    type ID {   LabelPair *p = (LabelPair *) malloc(sizeof(LabelPair)); 
                p->else_tag = (char *) malloc(sizeof(char)*strlen(currentType));
                strcpy(p->else_tag, currentType);
                push(p);
            } ',' paramList   
                            {   
                                Info *info = createNodeInfo($2->info->name,-1,-1,top()->else_tag);
                                insertInTable(info->name,1,getOffSet(),info->type,symTblLevel(),NULL,"param");
                                pop();
                                $$ = createNode(createNode(NULL,NULL,info,"formal_arg"),$5,NULL,"next_arg"); 
                            }
    | type ID               {  
                                incSymTblLevel();
                                Info *info = createNodeInfo($2->info->name,-1,-1,currentType);
                                insertInTable(info->name,1,getOffSet(),info->type,symTblLevel(),NULL,"param"); 
                                $$ = createNode(createNode(NULL,NULL,info,"formal_arg"),NULL,NULL,"next_arg"); 
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
                                    $3->info->type = currentType;
                                    insertInTable($3->info->name,1,$3->info->offSet,currentType,symTblLevel(),NULL,"var");
                                 }
                                 else{
                                    Info *info = createNodeInfo($3->info->name,-1,getOffSet(),currentType);
                                    info->level = symTblLevel();
                                    $3->info = info;
                                    insertInTable($3->info->name,1,$3->info->offSet,currentType,symTblLevel(),NULL,"var");
                                 }
                                 $$ = NULL;
                            }
    | VAR type ID '=' expr ';'  
                            {    
                                 if ($3->info->offSet == -1){
                                    $3->info->offSet = getOffSet();
                                    $3->info->level = symTblLevel();
                                    $3->info->type = currentType;
                                    insertInTable($3->info->name,1,$3->info->offSet,currentType,symTblLevel(),NULL,"var");
                                 }
                                 else{
                                    Info *info = createNodeInfo($3->info->name,-1,getOffSet(),currentType);
                                    info->level = symTblLevel();
                                    $3->info = info;
                                    insertInTable($3->info->name,1,$3->info->offSet,currentType,symTblLevel(),NULL,"var");
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
                                 //updateTable($1->info->name,evalTree($3),symTblLevel());
                                 ListNode *aux = findListNode($1->info->name);
                                 if (aux==NULL){ //Undeclared variable
                                    printf("undeclared variable %s\n",$1->info->name);
                                    exit(1);
                                 }
                                 aux->initialized = 1;
                                 $$ = createNode($1,$3,NULL,"asig"); 
                            }
    | PRINTI '(' expr ')' ';'   
                            {    $$ = createNode($3,NULL,NULL,"printi"); }
    | PRINTB '(' expr ')' ';'
                            {    $$ = createNode($3,NULL,NULL,"printb"); }
    | IF '(' expr ')' block ELSE block 
                            {    $$ = createNode($3,createNode($5,$7,NULL,"if_else"),NULL,"if"); }
    | IF '(' expr ')' block {    $$ = createNode($3,$5,NULL,"if"); }
    | WHILE '(' expr ')' block         
                            {    $$ = createNode($3,$5,NULL,"while"); }
    | RETURN expr ';'       {    $$ = createNode($2,NULL,NULL,"return"); }
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
    | ID '(' exprList ')' 
                        { 
                            Info *info = findNode($1->info->name);
                            if (info == NULL){
                                printf("Undeclared function %s\n", $1->info->name);
                                exit(1);
                            }       
                            $$ = createNode(NULL,$3,info,"call"); }
    ;

exprList:
    expr ',' exprList   {   $$ = createNode($1,$3,NULL,"actual_arg"); }
    | expr              {   $$ = createNode($1,NULL,NULL,"actual_arg"); }
    ;

type:
    INTEGER             {   currentType = "int"; }
    | BOOL              {   currentType = "bool"; }
    ;

%%


