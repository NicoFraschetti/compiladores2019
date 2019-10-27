#ifndef SYMBOL_TABLE_C
#define SYMBOL_TABLE_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table_utilities.h"

int level = 0;

int symTblLevel(){
	return level;
}

void incSymTblLevel(){
	level++;
}

void decSymTblLevel(){
	level--;
}

ListNode *p;

void add(char *name, int value, int initialized, int offset, char *type, int lvl) {
	ListNode *aux = (ListNode *) malloc(sizeof(ListNode));
	aux->info = (Info *) malloc(sizeof(Info));
	aux->info->name = (char *) malloc(sizeof(char *)*strlen(name));
	strcpy(aux->info->name,name);
	aux->info->value = value;
	aux->info->offSet = offset;
	aux->info->type = type;
	aux->info->level = lvl;
	aux->initialized = initialized;
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

int findValue(char *name){
	Info *info = findNode(name);
	return info->value;
}

void updateTable(char *name, int value) {
	ListNode *aux = findListNode(name);
	if (aux==NULL){ //Undeclared variable
		printf("undeclared variable %s\n",name);
		exit(1);
	} 
	aux->info->value = value;
	aux->initialized = 1;
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

void insertInTable(char *name, int value, int initialized, int offset, char *type, int lvl){
	if (findListNode(name)!=NULL){
		printf("Variable %s already declared \n",name);
		exit(1);
	}
	add(name,value,initialized,offset,type,lvl);
}

void printSymbolTable(){
	ListNode *aux = p;
	printf("[ ");
	while (aux != NULL){
		Info *info = aux->info;
		printf("(%s,%d, %d), ", info->name, info->offSet, info->level);
		aux = aux->next;
	}
	printf("]\n");
}

#endif