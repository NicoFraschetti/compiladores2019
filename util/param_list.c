#ifndef PARAM_C
#define PARAM_C
#include <stdio.h>
#include <stdlib.h>
#include "param_list.h"
#include "ast_utilities.h"

ListNode *head2 = NULL, *tail2 = NULL;

int isEmpty(){
	return head2 == NULL;
}

void insertParam(Info *info){
	ListNode *aux2 = (ListNode *) malloc(sizeof(ListNode));
	aux2->info = info;
	aux2->next = NULL;
	if (head2 == NULL)
		head2 = tail2 = aux2;
	else{
		tail2->next = aux2;
		tail2 = aux2;
	}
}

Info *getParam(){
	return head2->info;
}

void removeParam(){
	ListNode *aux2 = head2;
	head2 = head2->next;
	free(aux2);
}

void printParamTypeList(){
	ListNode *aux2 = head2;
	while (aux2!=NULL){
		printf("type: %s\n", aux2->label);
		aux2 = aux2->next;
	}
}

char *getTypeParam(){
	return head2->label;
}

void insertParamType(char *type){
	ListNode *aux2 = (ListNode *) malloc(sizeof(ListNode));
	aux2->label = type;
	aux2->next = NULL;
	if (head2 == NULL)
		head2 = tail2 = aux2;
	else{
		tail2->next = aux2;
		tail2 = aux2;
	}
}

#endif