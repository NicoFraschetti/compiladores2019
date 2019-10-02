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
		else 
			printf("── %s %s \n",t->label,s);
		printTree2(t->leftChild,n+1,"leftChild");
		printTree2(t->rightChild,n+1,"rightChild");
	}	
}

#endif