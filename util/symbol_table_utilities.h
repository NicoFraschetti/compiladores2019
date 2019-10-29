#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "types.h"

int symTblLevel();

void incSymTblLevel();

void decSymTblLevel();

void add(char *name, int value, int initialized, int offset, char *type, int lvl);

Info *findNode(char *name, int lvl);

//int findValue(char *name);

void updateTable(char *name, int value, int lvl);

ListNode *findListNode(char *name, int lvl);

ListNode *checkListNodeInLevel(char *name , int lvl);

void insertInTable(char *name, int value, int initialized, int offset, char *type, int lvl);

void printSymbolTable();

#endif