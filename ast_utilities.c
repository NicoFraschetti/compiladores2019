#ifndef AST_C
#define AST_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast_utilities.h"
#include "offset_generator.h"

TreeNode *root;

TreeNode *createNode(TreeNode *lft, TreeNode *rgt, Info *info, char * lbl){
	root = (TreeNode *) malloc(sizeof(TreeNode));
	root->info = info;
	root->label = (char *) malloc(sizeof(char *)*strlen(lbl));
	strcpy(root->label,lbl);
	root->leftChild = lft;
	root->rightChild = rgt;
	return root; 
}

int evalTree(TreeNode *t){
	
	if (strcmp(t->label,"int")==0 || strcmp(t->label,"var")==0)
		return t->info->value;
	else if (strcmp(t->label,"add")==0)
		return evalTree(t->leftChild)+evalTree(t->rightChild);
	else if (strcmp(t->label,"sub")==0)
		return evalTree(t->leftChild)-evalTree(t->rightChild);
	else if (strcmp(t->label,"mul")==0)
		return evalTree(t->leftChild)*evalTree(t->rightChild);
	else if (strcmp(t->label,"div")==0)
		return evalTree(t->leftChild)/evalTree(t->rightChild);
	else if (strcmp(t->label,"mod")==0)
		return evalTree(t->leftChild)%evalTree(t->rightChild);
}

int eval(){
	return evalTree(root);
}

Info *createNodeInfo(char *name, int value, int offset){
	Info *aux = (Info *) malloc(sizeof(Info));
	aux->name = name;
	aux->value = value;
	aux->offSet = offset;
}

void printTree(TreeNode *t, int n){
	printTree2(t,n,"root");
}

void printTree2(TreeNode *t, int n, char *s){
	if (t!=NULL){
		for (int i = 0; i < n; ++i){
			printf("   │");
		}
		if (strcmp(t->label,"int")==0)
			printf("── %d %s \n",t->info->value,s);
		else if (strcmp(t->label,"var")==0)
			printf("── %s %s \n",t->info->name,s);
		else if (strcmp(t->label,"bool")==0){
			if (t->info->value == 0)
				printf("── false %s \n",s);
			else
				printf("── true %s \n",s);
		}
		else 
			printf("── %s %s \n",t->label,s);
		printTree2(t->leftChild,n+1,"leftChild");
		printTree2(t->rightChild,n+1,"rightChild");
	}	
}

int nextCount = 0;
int asigCount = 0;
int intCount = 0;
int trueCount = 0;
int falseCount = 0;
int varCount = 0;
int addCount = 0;
int subCount = 0;
int mulCount = 0;
int divCount = 0;
int modCount = 0;
int ltCount = 0;
int gtCount = 0;
int eqCount = 0;
int andCount = 0;
int orCount = 0;
int notCount = 0;
int printiCount = 0;
int printbCount = 0;

char *generateNextName(TreeNode *t){
	char *name = (char *) malloc(sizeof(char *)*10);
	if (strcmp(t->label,"int")==0)
		sprintf(name,"%d%d",t->info->value,intCount++);
	else if (strcmp(t->label,"var")==0)
		sprintf(name,"%s%d",t->info->name,varCount++);
	else if (strcmp(t->label,"next")==0)
		sprintf(name,"next%d",nextCount++);
	else if (strcmp(t->label,"asig")==0)
		sprintf(name,"asig%d",asigCount++);
	else if (strcmp(t->label,"add")==0)
		sprintf(name,"add%d",addCount++);
	else if (strcmp(t->label,"sub")==0)
		sprintf(name,"sub%d",subCount++);
	else if (strcmp(t->label,"mul")==0)
		sprintf(name,"mul%d",mulCount++);
	else if (strcmp(t->label,"div")==0)
		sprintf(name,"div%d",divCount++);
	else if (strcmp(t->label,"mod")==0)
		sprintf(name,"mod%d",modCount++);
	else if (strcmp(t->label,"less")==0)
		sprintf(name,"less%d",ltCount++);
	else if (strcmp(t->label,"greater")==0)
		sprintf(name,"greater%d",gtCount++);
	else if (strcmp(t->label,"equal")==0)
		sprintf(name,"equal%d",eqCount++);
	else if (strcmp(t->label,"and")==0)
		sprintf(name,"and%d",andCount++);
	else if (strcmp(t->label,"or")==0)
		sprintf(name,"or%d",orCount++);
	else if (strcmp(t->label,"not")==0)
		sprintf(name,"not%d",notCount++);
	else if (strcmp(t->label,"printi")==0)
		sprintf(name,"printi%d",printiCount++);
	else if (strcmp(t->label,"printb")==0)
		sprintf(name,"printb%d",printbCount++);
	else if (strcmp(t->label,"bool")==0){
		if (t->info->value == 0)
			sprintf(name,"false%d",falseCount++);
		else
			sprintf(name,"true%d",trueCount++);
	}
	return name;
}

void generateDot2(TreeNode *t, FILE *f, char *parentName){
	if (t!=NULL){
		char * nodeName = generateNextName(t);
		fprintf(f,"%s->%s\n", parentName,nodeName);
		generateDot2(t->leftChild,f,nodeName);
		generateDot2(t->rightChild,f,nodeName);
	}
}

void generateDot(TreeNode *t, char *fileName){
	char *subStr = malloc(strlen(fileName)+4);
	strcpy(subStr,fileName);
	sprintf(subStr,"%s%s",fileName,".dot");
	FILE *f = fopen(subStr,"w");
	char * rootName = generateNextName(t);
	fprintf(f, "digraph{\n");
	fprintf(f, "inic[shape=point];\n");
	fprintf(f, "inic->%s\n",rootName);
	generateDot2(t->leftChild,f,rootName);
	generateDot2(t->rightChild,f,rootName);
	fprintf(f, "}\n");
	fclose(f);	
}

#endif