#ifndef AST_H
#define AST_H
#include "types.h"

TreeNode *createNode(TreeNode *lft, TreeNode *rgt, Info *info, char * lbl);

char *checkTypesCorrectnes(TreeNode *t);

Info *createNodeInfo(char *name, int value, int offset, char* type);

void generateDot(TreeNode *t,char *fileName);

#endif