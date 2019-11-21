#ifndef AST_C
#define AST_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table_utilities.h"
#include "ast_utilities.h"
#include "param_list.h"

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
		return "";
	if (strcmp(t->label,"next")==0)
		checkTypesCorrectnes(t->leftChild);
	else if (strcmp(t->label,"int")==0 || strcmp(t->label,"bool")==0){
		return t->label;
	}
	else if (strcmp(t->label,"var")==0 || strcmp(t->label,"global")==0){
		return t->info->type;
	}
	else if (strcmp(t->label,"asig")==0){
		char *leftChildType = checkTypesCorrectnes(t->leftChild);
		char *rightChildType = checkTypesCorrectnes(t->rightChild);
		if (strcmp(leftChildType,rightChildType)!=0){
			printf("Assignment error\n");
			exit(1);
		}
	}
	else if (strcmp(t->label,"global_asig")==0){
		char *leftChildType = checkTypesCorrectnes(t->leftChild);
		char *rightChildType = checkTypesCorrectnes(t->rightChild);
		if (strcmp(leftChildType,rightChildType)!=0){
			printf("Assignment error\n");
			exit(1);
		}
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
	}
	else if (strcmp(t->label,"function")==0){
		checkTypesCorrectnes(t->rightChild);
		return t->info->type;
	}
	else if (strcmp(t->label,"extern")==0){
		return t->info->type;
	}
	else if (strcmp(t->label,"return")==0){
		checkTypesCorrectnes(t->leftChild);
		return "";
	}
	else if (strcmp(t->label,"call")==0){
		TreeNode *currentArg = t->leftChild;
		while (currentArg!=NULL){
			insertParamType(checkTypesCorrectnes(currentArg->leftChild));
			currentArg = currentArg->rightChild;
		}
		ListNode *aux = findListNode(t->info->name);
		TypesList *paramType = aux->head;
		while(!isEmpty()){
			if (paramType==NULL){
				printf("Function %s called with too many arguments \n",t->info->name);
				exit(1);
			}
			else if(strcmp(getTypeParam(),paramType->type)!=0){
				printf("Incorrect argument type in function %s \n",t->info->name);
				exit(1);
			}
			removeParam();
			paramType = paramType->next;
		}
		if (paramType!=NULL){
			printf("Function %s called with too few arguments \n",t->info->name);
			exit(1);
		}
		return t->info->type;
	}
	if (strcmp(t->label,"next")==0)
		checkTypesCorrectnes(t->rightChild);

}

Info *createNodeInfo(char *name, int value, int offset, char *type){
	Info *aux = (Info *) malloc(sizeof(Info));
	aux->name = name;
	aux->value = value;
	aux->offSet = offset;
	aux->type = type;
	return aux;
}

int functionCount = 0;
int formalArgCount = 0;
int nextArgCount = 0;
int externCount = 0;
int returnCount = 0;
int callCount = 0;
int nextCount = 0;
int actualArgCount = 0;
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
int globalCount = 0;
int globalAsigCount = 0;
int globalDeclCount = 0;

char *generateNextName(TreeNode *t){
	char *name = (char *) malloc(sizeof(char *)*10);
	if (strcmp(t->label,"int")==0)
		sprintf(name,"%d%d",t->info->value,intCount++);
	else if (strcmp(t->label,"var")==0)
		sprintf(name,"%s%d%d%d%s",t->info->name,varCount++,-t->info->offSet,t->info->level,t->info->type);
	else if (strcmp(t->label,"global")==0)
		sprintf(name,"glbl%s%d%d",t->info->name,t->info->level,globalCount++);
	else if (strcmp(t->label,"next")==0)
		sprintf(name,"next%d",nextCount++);
	else if (strcmp(t->label,"asig")==0)
		sprintf(name,"asig%d",asigCount++);
	else if (strcmp(t->label,"global_decl")==0)
		sprintf(name,"gldecl%d",globalDeclCount++);
	else if (strcmp(t->label,"global_asig")==0)
		sprintf(name,"glasig%d",globalAsigCount++);
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
	else if (strcmp(t->label,"function")==0)
		sprintf(name,"%s%s%d",t->info->type,t->info->name,functionCount++);
	else if (strcmp(t->label,"extern")==0)
		sprintf(name,"extern%s%s%d",t->info->type,t->info->name,externCount++);
	else if (strcmp(t->label,"main")==0)
		sprintf(name,"main");
	else if (strcmp(t->label,"formal_arg")==0)
		sprintf(name,"%sformal_arg%d",t->info->type,formalArgCount++);
	else if (strcmp(t->label,"next_arg")==0)
		sprintf(name,"next_arg%d",nextArgCount++);
	else if (strcmp(t->label,"return")==0)
		sprintf(name,"return%d",returnCount++);
	else if (strcmp(t->label,"call")==0)
		sprintf(name,"call%s%d",t->info->name,callCount++);
	else if (strcmp(t->label,"actual_arg")==0)
		sprintf(name,"actual_arg%d",actualArgCount++);
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