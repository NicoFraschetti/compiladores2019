#ifndef AST_C
#define AST_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast_utilities.h"
#include "cod_3D_list_utilities.h"

TreeNode *root;

Cod3D *head, *tail;

void insertCod3D(Op opCod, Info *arg1, Info *arg2, Info *res) {
	Cod3D *aux = (Cod3D *) malloc(sizeof(Cod3D));
	aux->opCod = opCod;
	aux->arg1 = arg1;
	aux->arg2 = arg2;
	aux->result = res;
	if (head == NULL)
		head = tail = aux;
	else{
		tail->next = aux;
		tail = aux;
	}
}


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

Info *createNodeInfo(char *name, int value){
	Info *aux = (Info *) malloc(sizeof(Info));
	aux->name = name;
	aux->value = value;
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

int tmpCount = 0;

Info *generateNextTmp(){
	char *name = (char *) malloc(sizeof(char *)*5);
	sprintf(name,"t%d",tmpCount++);
	Info *info = createNodeInfo(name,-1);

}

Info *generateCod3DList(TreeNode *t) {
	if (t==NULL)
		return NULL;
	if (strcmp(t->label,"next")==0)
		generateCod3DList(t->leftChild);
	if (strcmp(t->label,"int")==0 || strcmp(t->label,"var")==0)
		return t->info;
	else if (strcmp(t->label,"asig")==0){
		Op opCod = ASIGI;
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),NULL);
		return NULL;
	}
	else if (strcmp(t->label,"add")==0){
		Op opCod = ADDI;
		Info *res = generateNextTmp();
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;	
	}
	else if (strcmp(t->label,"sub")==0){
		Op opCod = SUBI;
		Info *res = generateNextTmp();
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;	
	}
	else if (strcmp(t->label,"mul")==0){
		Op opCod = MULI;
		Info *res = generateNextTmp();
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;
	}
	else if (strcmp(t->label,"div")==0){
		Op opCod = DIVI;
		Info *res = generateNextTmp();
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;
	}
	else if (strcmp(t->label,"mod")==0){
		Op opCod = MODI;
		Info *res = generateNextTmp();
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;
	}
	else if (strcmp(t->label,"printi")==0){
		Op opCod = PRINT;
		insertCod3D(opCod,t->leftChild->info,NULL,NULL);
		return NULL;
	}
	if (strcmp(t->label,"next")==0)
		generateCod3DList(t->rightChild);
}

void printCod3DList(){
	Cod3D *aux = head;
	while (aux!=NULL){
		printf("[%d,",aux->opCod);
		if (aux->arg1!=NULL)
			printf("(%s,%d),",aux->arg1->name,aux->arg1->value);
		else
			printf("null,");
		if (aux->arg2!=NULL)
			printf("(%s,%d),",aux->arg2->name,aux->arg2->value);
		else
			printf("null,");
		if (aux->result!=NULL)
			printf("(%s,%d)]\n",aux->result->name,aux->result->value);
		else
			printf("null]\n");
		aux = aux->next;		
	}
}

#endif