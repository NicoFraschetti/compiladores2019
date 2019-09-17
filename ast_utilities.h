#ifndef AST_H
#define AST_H
#include "types.h"

TreeNode *createNode(TreeNode *lft, TreeNode *rgt, Info *info, char * lbl);

int evalTree(TreeNode *t);

int eval();

Info *createNodeInfo(char *name, int value);

#endif