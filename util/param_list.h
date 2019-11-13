#ifndef PARAM_H
#define PARAM_H
#include "types.h"

int isEmpty();

void insertParam(Info *info);

void insertParamType(char *type);

Info *getParam();

char *getTypeParam();

void removeParam();

void printParamTypeList();

#endif