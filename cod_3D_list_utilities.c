#ifndef COD3D_C
#define COD3D_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cod_3D_list_utilities.h"
#include "ast_utilities.h"
#include "offset_generator.h"

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
	info->offSet = getOffSet();

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

char* OpCodName(int opCod){
	switch(opCod){
		case 0:
			return "ASIG";
			break;
		case 1:
			return "ADD";
			break;
		case 2:
			return "MUL";
			break;
		case 3:
			return "SUB";
			break;
		case 4:
			return "DIV";
			break;
		case 5:
			return "MOD";
			break;
		case 6:
			return "PRINT";
			break;
		default:
			return NULL;
	}
}

void printCod3DList(){
	Cod3D *aux = head;
	while (aux!=NULL){
		printf("[%s,",OpCodName(aux->opCod));
		if (aux->arg1!=NULL)
			printf("(%s,%d,offset=%d),",aux->arg1->name,aux->arg1->value,aux->arg1->offSet);
		else
			printf("null,");
		if (aux->arg2!=NULL)
			printf("(%s,%d,offset=%d),",aux->arg2->name,aux->arg2->value,aux->arg2->offSet);
		else
			printf("null,");
		if (aux->result!=NULL)
			printf("(%s,%d,offset=%d)]\n",aux->result->name,aux->result->value,aux->result->offSet);
		else
			printf("null]\n");
		aux = aux->next;		
	}
}

void generateAssembly(TreeNode *t, char *fileName){
	generateCod3DList(t);
	Cod3D *aux = head; 
	char *subStr = malloc(strlen(fileName)-3);
	strncpy(subStr,fileName,strlen(fileName)-4);
	sprintf(fileName,"%s%s",subStr,".s");
	FILE *f = fopen(fileName,"w");
	fprintf(f,"%s\n","	.globl main");
	fprintf(f,"%s\n","main:");
	fprintf(f, "	enter	$%d. $0\n", -offSet()+16);
	while (aux != NULL){
		switch(aux->opCod){
			case 0:
				fprintf(f, "	movq	$%d, %d(%rbp)\n", );
				break;
			case 1:
				//Do something
				break;
			case 2:
				//Do something
				break;
			case 3:
				//Do something
				break;
			case 4:
				//Do something
				break;
			case 5:
				//Do something
				break;
			case 6:
				//Do something
				break;
		}	
		aux = aux->next;
	}
	fprintf(f, "	%s\n","leave");
	fprintf(f, "	%s\n","ret");
	fclose(f);	
}

#endif