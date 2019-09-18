#ifndef COD3D_C
#define COD3D_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cod_3D_list_utilities.h"
#include "ast_utilities.h"

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