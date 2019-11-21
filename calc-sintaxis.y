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

int cantParams = 0;

%}
 
%union { struct TreeNode *node;}
 
%token<node> INT ID TRUE FALSE
%token VAR AND OR INTEGER BOOL WHILE IF ELSE MAIN EXTERN RETURN

%type<node> program decls decl statements statement expr block init func funcList paramList exprList globalDecls globalDecl type

%left AND OR
%left '!'
%left EQ '>' '<' 
%left '+' '-' 
%left '*' '/' '%'

 
%%

prog:
    init                    {   
                                //generateDot($1,"dot_output.dot");
                                //printSymbolTable();
                                generateAssembly($1,getName());
                                //generateCod3DList($1);
                                //printCod3DList(); 
                            }  

init:
    globalDecls funcList    {   $$ = createNode($1,$2,NULL,"next"); }
    | funcList              {   $$ = createNode(NULL,$1,NULL,"next"); }
    ; 

funcList:
    func funcList           {   $$ = createNode($1,$2,NULL,"next"); }
    | INTEGER MAIN '(' ')' { resetOffSet(0); } '{' program '}' { 
                                                Info *info = createNodeInfo("main",-1,-1,"int");
                                                info->lastOffSet = offSet(); 
                                                $$ = createNode(NULL,$7,info,"function"); 
                                           }
    ;

func:
    type ID '(' paramList ')' { cantParams = 0; 
                                resetOffSet(-(paramTreeSize($4)*8));
                                ListNode *aux = findListNode($2->info->name);
                                if (aux!=NULL){ //Undeclared variable
                                    printf("function %s already declared\n",$2->info->name);
                                    exit(1);
                                }
                                $2->info->type = $1->label;
                                $2->info->level = symTblLevel()-1;
                                insertInTable($2->info->name,1,-1,$1->label,symTblLevel()-1,$4,"function");
                               } block  {
                                        decSymTblLevel();         
                                        $2->info->lastOffSet = offSet();
                                        $$ = createNode($4,$7,$2->info,"function"); 
                                     }
    | type ID '(' ')' { resetOffSet(0);
                        ListNode *aux = findListNode($2->info->name);
                        if (aux!=NULL){ //Undeclared variable
                            printf("function %s already declared\n",$2->info->name);
                            exit(1);
                        }
                        $2->info->level = symTblLevel();
                        $2->info->type = $1->label;
                        insertInTable($2->info->name,1,-1,$2->info->type,symTblLevel(),NULL,"function"); } block        {  
                                        $2->info->lastOffSet = offSet();
                                        $$ = createNode(NULL,$6,$2->info,"function");
                                     }
    | EXTERN type ID '(' paramList ')' ';' { 
                                        cantParams = 0; 
                                        resetOffSet(-(paramTreeSize($5)*8));
                                        decSymTblLevel();
                                        ListNode *aux = findListNode($3->info->name);
                                        if (aux!=NULL){ //Undeclared variable
                                            printf("function %s already declared\n",$3->info->name);
                                            exit(1);
                                        }   
                                        $3->info->type = $2->label;
                                        $3->info->level = symTblLevel();
                                        insertInTable($3->info->name,1,-1,$2->label,symTblLevel(),$5,"function");
                                        $3->info->lastOffSet = offSet();
                                        $$ = createNode($5,NULL,$3->info,"extern");   
                                      }
    | EXTERN type ID '(' ')' ';'     {  
                                        ListNode *aux = findListNode($3->info->name);
                                        if (aux!=NULL){ //Undeclared variable
                                            printf("function %s already declared\n",$3->info->name);
                                            exit(1);
                                        }   
                                        $3->info->type = $2->label;
                                        $3->info->level = symTblLevel();
                                        insertInTable($3->info->name,1,-1,$3->info->type,symTblLevel(),NULL,"function");
                                        $3->info->lastOffSet = offSet();
                                        $$ = createNode(NULL,NULL,$3->info,"extern");  
                                     }

paramList:
    type ID {   cantParams ++;
                $2->info->offSet = cantParams;
            } ',' paramList   
                            {  
                                 int prm_offset;
                                 if ($2->info->offSet <= 6 && cantParams <= 6)
                                     prm_offset = -((cantParams+1)*8+getParamOffSet());
                                 else if ($2->info->offSet <= 6 && cantParams > 6)
                                     prm_offset = -(7*8+getParamOffSet());
                                 else
                                     prm_offset = ($2->info->offSet-5)*8;
                                 $2->info->offSet = prm_offset;
                                 $2->info->type = $1->label;  
                                 insertInTable($2->info->name,1,$2->info->offSet,$2->info->type,symTblLevel(),NULL,"param");
                                 $$ = createNode(createNode(NULL,NULL,$2->info,"formal_arg"),$5,NULL,"next_arg"); 
                            }
    | type ID               {   
                                 cantParams++;
                                 resetParamOffSet();
                                 incSymTblLevel();
                                 int prm_offset;
                                 if (cantParams <= 6)
                                     prm_offset = -((cantParams+1)*8+getParamOffSet());
                                 else
                                     prm_offset = (cantParams-5)*8;
                                 $2->info->offSet = prm_offset;
                                 $2->info->type = $1->label;
                                 insertInTable($2->info->name,1,$2->info->offSet,$2->info->type,symTblLevel(),NULL,"param"); 
                                 $$ = createNode(createNode(NULL,NULL,$2->info,"formal_arg"),NULL,NULL,"next_arg"); 
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
                                 $3->info->offSet = getOffSet();
                                 $3->info->level = symTblLevel();
                                 $3->info->type = $2->label;
                                 insertInTable($3->info->name,0,$3->info->offSet,$3->info->type,symTblLevel(),NULL,"var");
                                 $$ = NULL;
                            }
    | VAR type ID '=' expr ';'  
                            {    
                                 $3->info->offSet = getOffSet();
                                 $3->info->level = symTblLevel();
                                 $3->info->type = $2->label;
                                 insertInTable($3->info->name,1,$3->info->offSet,$3->info->type,symTblLevel(),NULL,"var");
                                 $$ = createNode($3,$5,NULL,"asig"); 
                            }
    ;
globalDecls:
    globalDecls globalDecl  {    $$ = createNode($1,$2,NULL,"next"); }
    |                       {    $$ = NULL; }
    ;
globalDecl:
    VAR type ID ';'         {    
                                 $3->info->level = symTblLevel();
                                 $3->info->type = $2->label;
                                 $3->label = "global";    
                                 insertInTable($3->info->name,1,-1,$3->info->type,symTblLevel(),NULL,"global");
                                 $$ = createNode($3,NULL,NULL,"global_decl");
                            }
    | VAR type ID '=' INT ';'  
                            {    
                                 $3->info->level = symTblLevel();
                                 $3->info->type = $2->label;
                                 $3->label = "global";
                                 insertInTable($3->info->name,1,-1,$3->info->type,symTblLevel(),NULL,"global");
                                 $$ = createNode($3,$5,NULL,"global_asig"); 
                            }
    | VAR type ID '=' TRUE ';'
                            {    
                                 $3->info->level = symTblLevel();
                                 $3->info->type = $2->label;
                                 $3->label = "global";
                                 insertInTable($3->info->name,1,-1,$3->info->type,symTblLevel(),NULL,"global");
                                 $$ = createNode($3,$5,NULL,"global_asig"); 
                            }
    | VAR type ID '=' FALSE ';'
                            {    
                                 $3->info->level = symTblLevel();
                                 $3->info->type = $2->label;
                                 $3->label = "global";
                                 insertInTable($3->info->name,1,-1,$3->info->type,symTblLevel(),NULL,"global");
                                 $$ = createNode($3,$5,NULL,"global_asig"); 
                            }
    ;

statements:
    statements statement    {    $$ = createNode($1,$2,NULL,"next"); }
    |                       {    $$ = NULL; }                 
    ;

statement: 
    ID '=' expr ';'         {   
                                 ListNode *aux = findListNode($1->info->name);
                                 if (aux==NULL){ //Undeclared variable
                                    printf("undeclared variable %s\n",$1->info->name);
                                    exit(1);
                                 }
                                 if (strcmp(aux->label,"global")==0)
                                    $1->label = "global";
                                 aux->initialized = 1;
                                 $$ = createNode($1,$3,NULL,"asig"); 
                            }
    | IF '(' expr ')' block ELSE block 
                            {    $$ = createNode($3,createNode($5,$7,NULL,"if_else"),NULL,"if"); }
    | IF '(' expr ')' block {    $$ = createNode($3,$5,NULL,"if"); }
    | WHILE '(' expr ')' block         
                            {    $$ = createNode($3,$5,NULL,"while"); }
    | RETURN expr ';'       {    $$ = createNode($2,NULL,NULL,"return"); }
    ; 

block: 
    {   incSymTblLevel();  }
    '{' program '}'         
    {   decSymTblLevel();  }    
                            {    $$ = $3; }
    | ';'                   {    $$ = createNode(NULL,NULL,NULL,"next"); }
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
                    if (strcmp(aux->label,"global")==0)
                        $$ = createNode(NULL,NULL,aux->info,"global");
                    else
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
                            $$ = createNode($3,NULL,info,"call"); 
                        }
    | ID '(' ')'        {
                            Info *info = findNode($1->info->name);
                            if (info == NULL){
                                printf("Undeclared function %s\n", $1->info->name);
                                exit(1);
                            }       
                            $$ = createNode(NULL,NULL,info,"call");   
                        }
    ;

exprList:
    expr ',' exprList   {   $$ = createNode($1,$3,NULL,"actual_arg"); }
    | expr              {   $$ = createNode($1,NULL,NULL,"actual_arg"); }
    ;

type:
    INTEGER             {   $$ = createNode(NULL,NULL,NULL,"int"); }
    | BOOL              {   $$ = createNode(NULL,NULL,NULL,"bool"); }
    ;

%%


