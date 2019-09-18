#ifndef COD3D_H
#define COD3D_H
#include "types.h"

void insertCod3D(Op opCod, Info *arg1, Info *arg2, Info *res);

Info *generateCod3DList(TreeNode *t);

void printCod3DList();

#endif