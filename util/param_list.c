#ifndef PARAM_C
#define PARAM_C
#include <stdio.h>
#include <stdlib.h>
#include "param_list.h"
#include "ast_utilities.h"

ParamsList *head2 = NULL, *tail2 = NULL;

int isEmpty(){
	return head2 == NULL;
}

void insertParam(Info *info){
	ParamsList *aux2 = (ParamsList *) malloc(sizeof(ParamsList));
	aux2->info = info;
	//aux2->info = createNodeInfo(info->name,info->value,info->offSet,info->type);
	if (head2 == NULL)
		head2 = tail2 = aux2;
	else{
		tail2->next = aux2;
		tail2 = aux2;
	}
}

Info *getParam(){
	if (head2 == NULL)
		return NULL;
	return head2->info;
}

void removeParam(){
	head2 = head2->next;
}

void printParamsList(){
	ParamsList *aux2 = head2;
	while (aux2!=NULL){
		printf("aca hay un puto info\n");
		aux2 = aux2->next;
	}
}

#endif