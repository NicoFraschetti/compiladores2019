#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
#include "types.h"

int symTblLevel();

void incSymTblLevel();

void decSymTblLevel();

void clearSymTblLevel(int lvl);

void add(char *name, int initialized, int offset, char *type, int lvl, TreeNode *paramList, char *label);

Info *findNode(char *name);

//int findValue(char *name);

//void updateTable(char *name, int value, int lvl);

ListNode *findListNode(char *name);

ListNode *checkListNodeInLevel(char *name , int lvl);

void insertInTable(char *name, int initialized, int offset, char *type, int lvl, TreeNode *paramList, char *label);

void printSymbolTable();

int paramListSize(TypesList *h);

int paramTreeSize(TreeNode *tree);

#endif