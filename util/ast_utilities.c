#ifndef AST_C
#define AST_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table_utilities.h"
#include "ast_utilities.h"

TreeNode *root;

TreeNode *createNode(TreeNode *lft, TreeNode *rgt, Info *info, char * lbl){
	root = (TreeNode *) malloc(sizeof(TreeNode));
	root->info = info;
	root->label = (char *) malloc(sizeof(char *)*strlen(lbl));
	strcpy(root->label,lbl);
	root->leftChild = lft;
	root->rightChild = rgt;
	return root; 
}

char *checkTypesCorrectnes(TreeNode *t){
	if (t == NULL)
		return NULL;
	if (strcmp(t->label,"next")==0)
		checkTypesCorrectnes(t->leftChild);
	else if (strcmp(t->label,"int")==0 || strcmp(t->label,"bool")==0){
		return t->label;
	}
	else if (strcmp(t->label,"var")==0){
		return findNode(t->info->name,t->info->level)->type;
	}
	else if (strcmp(t->label,"asig")==0){
		char *leftChildType = checkTypesCorrectnes(t->leftChild);
		char *rightChildType = checkTypesCorrectnes(t->rightChild);
		if (strcmp(leftChildType,rightChildType)!=0){
			printf("Assignment error\n");
			exit(1);
		}
		return leftChildType;
	}
	else if (strcmp(t->label,"printi")==0){
		if (strcmp(checkTypesCorrectnes(t->leftChild),"int")!=0){
			printf("printi must only be used for integers\n");
			exit(1);	
		}
		return "int";
	}
	else if (strcmp(t->label,"printb")==0){
		if (strcmp(checkTypesCorrectnes(t->leftChild),"bool")!=0){
			printf("printb must only be used for booleans\n");
			exit(1);	
		}
		return "bool";
	}
	else if (strcmp(t->label,"add")==0){
		char *leftChildType = checkTypesCorrectnes(t->leftChild);
		char *rightChildType = checkTypesCorrectnes(t->rightChild);
		if (strcmp(leftChildType,"int")!=0 || strcmp(leftChildType,rightChildType)!=0){
			printf("Type error in addition\n");
			exit(1);
		}
		return leftChildType;
	}
	else if (strcmp(t->label,"sub")==0){
		char *leftChildType = checkTypesCorrectnes(t->leftChild);
		char *rightChildType = checkTypesCorrectnes(t->rightChild);
		if (strcmp(leftChildType,"int")!=0 || strcmp(leftChildType,rightChildType)!=0){
			printf("Type error in substraction\n");
			exit(1);
		}
		return leftChildType;
	}
	else if (strcmp(t->label,"mul")==0){
		char *leftChildType = checkTypesCorrectnes(t->leftChild);
		char *rightChildType = checkTypesCorrectnes(t->rightChild);
		if (strcmp(leftChildType,"int")!=0 || strcmp(leftChildType,rightChildType)!=0){
			printf("Type error in multiplication\n");
			exit(1);
		}
		return leftChildType;
	}
	else if (strcmp(t->label,"div")==0){
		char *leftChildType = checkTypesCorrectnes(t->leftChild);
		char *rightChildType = checkTypesCorrectnes(t->rightChild);
		if (strcmp(leftChildType,"int")!=0 || strcmp(leftChildType,rightChildType)!=0){
			printf("Type error in division\n");
			exit(1);
		}
		return leftChildType;
	}
	else if (strcmp(t->label,"mod")==0){
		char *leftChildType = checkTypesCorrectnes(t->leftChild);
		char *rightChildType = checkTypesCorrectnes(t->rightChild);
		if (strcmp(leftChildType,"int")!=0 || strcmp(leftChildType,rightChildType)!=0){
			printf("Type error in mod\n");
			exit(1);
		}
		return leftChildType;
	}
	else if (strcmp(t->label,"less")==0){
		char *leftChildType = checkTypesCorrectnes(t->leftChild);
		char *rightChildType = checkTypesCorrectnes(t->rightChild);
		if (strcmp(leftChildType,"int")!=0 || strcmp(leftChildType,rightChildType)!=0){
			printf("Type error in comparison\n");
			exit(1);
		}
		return "bool";
	}
	else if (strcmp(t->label,"greater")==0){
		char *leftChildType = checkTypesCorrectnes(t->leftChild);
		char *rightChildType = checkTypesCorrectnes(t->rightChild);
		if (strcmp(leftChildType,"int")!=0 || strcmp(leftChildType,rightChildType)!=0){
			printf("Type error in comparison\n");
			exit(1);
		}
		return "bool";
	}
	else if (strcmp(t->label,"equal")==0){
		char *leftChildType = checkTypesCorrectnes(t->leftChild);
		char *rightChildType = checkTypesCorrectnes(t->rightChild);
		if (strcmp(leftChildType,rightChildType)!=0){
			printf("Type error in comparison\n");
			exit(1);
		}
		return "bool";
	}
	else if (strcmp(t->label,"and")==0){
		char *leftChildType = checkTypesCorrectnes(t->leftChild);
		char *rightChildType = checkTypesCorrectnes(t->rightChild);
		if (strcmp(leftChildType,"bool")!=0 || strcmp(leftChildType,rightChildType)!=0){
			printf("Type error in and operation\n");
			exit(1);
		}
		return "bool";
	}
	else if (strcmp(t->label,"or")==0){
		char *leftChildType = checkTypesCorrectnes(t->leftChild);
		char *rightChildType = checkTypesCorrectnes(t->rightChild);
		if (strcmp(leftChildType,"bool")!=0 || strcmp(leftChildType,rightChildType)!=0){
			printf("Type error in or operation\n");
			exit(1);
		}
		return "bool";
	}
	else if (strcmp(t->label,"not")==0){
		char *leftChildType = checkTypesCorrectnes(t->leftChild);
		if (strcmp(leftChildType,"bool")!=0){
			printf("Type error in not operation\n");
			exit(1);
		}
		return "bool";
	}
	else if (strcmp(t->label,"if")==0){
		if (strcmp(checkTypesCorrectnes(t->leftChild),"bool")!=0){
			printf("if condition does not evaluate to boolean\n");
			exit(1);	
		}
		checkTypesCorrectnes(t->rightChild);
		return "bool";
	}
	else if (strcmp(t->label,"if_else")==0){
		checkTypesCorrectnes(t->leftChild);
		checkTypesCorrectnes(t->rightChild);
	}
	else if (strcmp(t->label,"while")==0){
		if (strcmp(checkTypesCorrectnes(t->leftChild),"bool")!=0){
			printf("while condition does not evaluate to boolean\n");
			exit(1);	
		}
		checkTypesCorrectnes(t->rightChild);
		return "bool";
	}
	if (strcmp(t->label,"next")==0)
		checkTypesCorrectnes(t->rightChild);

}
int evalTree(TreeNode *t){
	
	if (strcmp(t->label,"int")==0 || strcmp(t->label,"var")==0)
		return t->info->value;
	else if (strcmp(t->label,"add")==0)
		return evalTree(t->leftChild)+evalTree(t->rightChild);
	else if (strcmp(t->label,"sub")==0)
		return evalTree(t->leftChild)-evalTree(t->rightChild);
	else if (strcmp(t->label,"mul")==0)
		return evalTree(t->leftChild)*evalTree(t->rightChild);
	else if (strcmp(t->label,"div")==0)
		return evalTree(t->leftChild)/evalTree(t->rightChild);
	else if (strcmp(t->label,"mod")==0)
		return evalTree(t->leftChild)%evalTree(t->rightChild);
}

Info *createNodeInfo(char *name, int value, int offset, char *type){
	Info *aux = (Info *) malloc(sizeof(Info));
	aux->name = name;
	aux->value = value;
	aux->offSet = offset;
	aux->type = type;
	return aux;
}

int nextCount = 0;
int asigCount = 0;
int intCount = 0;
int trueCount = 0;
int falseCount = 0;
int varCount = 0;
int addCount = 0;
int subCount = 0;
int mulCount = 0;
int divCount = 0;
int modCount = 0;
int ltCount = 0;
int gtCount = 0;
int eqCount = 0;
int andCount = 0;
int orCount = 0;
int notCount = 0;
int printiCount = 0;
int printbCount = 0;
int ifCount = 0;
int ifElseCount = 0;
int whileCount = 0;

char *generateNextName(TreeNode *t){
	char *name = (char *) malloc(sizeof(char *)*10);
	if (strcmp(t->label,"int")==0)
		sprintf(name,"%d%d",t->info->value,intCount++);
	else if (strcmp(t->label,"var")==0)
		sprintf(name,"%s%d%d%d",t->info->name,varCount++,-t->info->offSet,t->info->level);
	else if (strcmp(t->label,"next")==0)
		sprintf(name,"next%d",nextCount++);
	else if (strcmp(t->label,"asig")==0)
		sprintf(name,"asig%d",asigCount++);
	else if (strcmp(t->label,"add")==0)
		sprintf(name,"add%d",addCount++);
	else if (strcmp(t->label,"sub")==0)
		sprintf(name,"sub%d",subCount++);
	else if (strcmp(t->label,"mul")==0)
		sprintf(name,"mul%d",mulCount++);
	else if (strcmp(t->label,"div")==0)
		sprintf(name,"div%d",divCount++);
	else if (strcmp(t->label,"mod")==0)
		sprintf(name,"mod%d",modCount++);
	else if (strcmp(t->label,"less")==0)
		sprintf(name,"less%d",ltCount++);
	else if (strcmp(t->label,"greater")==0)
		sprintf(name,"greater%d",gtCount++);
	else if (strcmp(t->label,"equal")==0)
		sprintf(name,"equal%d",eqCount++);
	else if (strcmp(t->label,"and")==0)
		sprintf(name,"and%d",andCount++);
	else if (strcmp(t->label,"or")==0)
		sprintf(name,"or%d",orCount++);
	else if (strcmp(t->label,"not")==0)
		sprintf(name,"not%d",notCount++);
	else if (strcmp(t->label,"printi")==0)
		sprintf(name,"printi%d",printiCount++);
	else if (strcmp(t->label,"printb")==0)
		sprintf(name,"printb%d",printbCount++);
	else if (strcmp(t->label,"bool")==0){
		if (t->info->value == 0)
			sprintf(name,"false%d",falseCount++);
		else
			sprintf(name,"true%d",trueCount++);
	}
	else if (strcmp(t->label,"if")==0)
		sprintf(name,"if%d",ifCount++);
	else if (strcmp(t->label,"if_else")==0)
		sprintf(name,"if_else%d",ifElseCount++);
	else if (strcmp(t->label,"while")==0)
		sprintf(name,"while%d",whileCount++);
	return name;
}

void generateDot2(TreeNode *t, FILE *f, char *parentName){
	if (t!=NULL){
		char *nodeName = generateNextName(t);
		fprintf(f,"%s->%s\n", parentName,nodeName);
		generateDot2(t->leftChild,f,nodeName);
		generateDot2(t->rightChild,f,nodeName);
	}
}

void generateDot(TreeNode *t, char *fileName){
	FILE *f = fopen(fileName,"w");
	char *rootName = generateNextName(t);
	fprintf(f, "digraph{\n");
	fprintf(f, "inic[shape=point];\n");
	fprintf(f, "inic->%s\n",rootName);
	generateDot2(t->leftChild,f,rootName);
	generateDot2(t->rightChild,f,rootName);
	fprintf(f, "}\n");
	fclose(f);	
}

#endif