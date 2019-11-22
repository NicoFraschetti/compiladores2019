#ifndef SYMBOL_TABLE_C
#define SYMBOL_TABLE_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table_utilities.h"
#include "ast_utilities.h"

ListNode *p;

int level = 0;

int symTblLevel(){
	return level;
}

void incSymTblLevel(){
	level++;
}

void clearLevel(){
	ListNode *aux = p;
	ListNode *aux2 = NULL;
	int first = 1;
	while (aux != NULL){
		if (aux->info->level == level){
			ListNode *aux3 = aux2; 
			if (aux2 != NULL)
				aux2->next = aux->next;
			aux3 = aux;
			aux = aux->next;
			free(aux3);
		}
		else{
			if (first++ == 1)
				p = aux;
			aux2 = aux;
			aux = aux->next;
		}
	}
	if (first == 1)
		p = NULL;
}

void decSymTblLevel(){
	clearLevel();
	level--;
}

void add(char *name, int initialized, int offset, char *type, int lvl, TreeNode *paramList, char *label) {
	ListNode *aux = (ListNode *) malloc(sizeof(ListNode));
	aux->info = (Info *) malloc(sizeof(Info));
	aux->info->name = (char *) malloc(sizeof(char *)*strlen(name));
	strcpy(aux->info->name,name);
	aux->info->offSet = offset;
	aux->info->type = type;
	aux->info->level = lvl;
	aux->initialized = initialized;
	aux->label = label;
	while (paramList != NULL){
		TypesList *aux2 = (TypesList *) malloc(sizeof(TypesList));
		aux2->type = (char *) malloc(sizeof(char)*strlen(type));
		strcpy(aux2->type,paramList->leftChild->info->type);
		aux2->next = NULL;
		if (aux->head == NULL)
			aux->head = aux->tail = aux2;
		else{
			aux->tail->next = aux2;
			aux->tail = aux2;
		}	
		paramList = paramList->rightChild;	
	}

	aux->next = p;
	p = aux;
}

Info *findNode(char *name){
	ListNode *aux = p;
	while (aux != NULL) {
		if (strcmp(aux->info->name,name)==0)
			return aux->info;
		aux = aux->next;
	}
	return NULL;
} 

ListNode *findListNode(char *name){
	ListNode *aux = p;
	while (aux != NULL) {
		if (strcmp(aux->info->name,name)==0)
			return aux;
		aux = aux->next;
	}
	return NULL;	
}

ListNode *checkListNodeInLevel(char *name , int lvl){
	ListNode *aux = p;
	while (aux != NULL) {
		if (strcmp(aux->info->name,name)==0 && aux->info->level == lvl)
			return aux;
		aux = aux->next;
	}
	return NULL;
}

void insertInTable(char *name, int initialized, int offset, char *type, int lvl, TreeNode *paramList, char *label){
	if (checkListNodeInLevel(name,lvl)!=NULL){
		printf("Variable %s already declared \n",name);
		exit(1);
	}
	add(name,initialized,offset,type,lvl,paramList,label);
}

void printSymbolTable(){
	ListNode *aux = p;
	printf("[ ");
	while (aux != NULL){
		Info *info = aux->info;
		printf("(%s,%d, %d ,", info->name, info->offSet, info->level);
		TypesList *aux2 = aux->head;
		while (aux2 != NULL){
			printf("%s ,", aux2->type);
			aux2 = aux2->next;
		}
		printf(" %s) ", aux->label);
		aux = aux->next;
	}
	printf("]\n");
}

int paramListSize(TypesList *h){
	int size = 0;
	while (h != NULL){
		size++;
		h = h->next;
	}
	return size;
}

int paramTreeSize(TreeNode *tree){
	int size = 0;
	while (tree != NULL){
		size++;
		tree = tree->rightChild;
	}
	return size;
}

#endif