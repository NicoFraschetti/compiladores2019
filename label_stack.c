#ifndef STACK_C
#define STACK_C
#include <stdlib.h>
#include "label_stack.h"

StackNode *stack_top = NULL;

void push(LabelPair *p){
	StackNode *aux = (StackNode *) malloc(sizeof(StackNode));
	aux->pair = p;
	aux->next = stack_top;
	stack_top = aux;
}

void pop(){
	stack_top = stack_top->next;
}

LabelPair *top(){
	if (stack_top == NULL)
		return NULL;
	return stack_top->pair;
}



#endif