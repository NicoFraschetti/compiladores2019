#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "types.h"

void add(char *name, int value, int initialized, int offset, char *label);

Info *findNode(char *name);

int findValue(char *name);

void insertValue(char *name, int value);

void updateTable(char *name, int value);

ListNode *findListNode(char *name);

void insertInTable(char *name, int value, int initialized, int offset, char *label);

void printSymbolTable();

#endif