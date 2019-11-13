#ifndef COD3D_C
#define COD3D_C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cod_3D_list_utilities.h"
#include "ast_utilities.h"
#include "offset_generator.h"
#include "symbol_table_utilities.h"
#include "label_stack.h"
#include "param_list.h"

Cod3D *head, *tail;

/*ListNode *paramHead, *paramTail;

void insertParam(Info *info){
	ListNode *ptr = (ListNode *) malloc(sizeof(ListNode));
	ptr->info = info;
	ptr->next = NULL;
	if (paramHead == NULL)
		paramHead = paramTail = ptr;
	else{
		paramTail->next = ptr;
		paramTail = ptr;
	}
}*/

void insertCod3D(Op opCod, Info *arg1, Info *arg2, Info *res) {
	Cod3D *aux = (Cod3D *) malloc(sizeof(Cod3D));
	aux->opCod = opCod;
	aux->arg1 = arg1;
	aux->arg2 = arg2;
	aux->result = res;
	aux->next = NULL;
	if (head == NULL)
		head = tail = aux;
	else{
		tail->next = aux;
		tail = aux;
	}
}

int tmpCount = 0;

Info *generateNextTmp(char *type){
	char *name = (char *) malloc(sizeof(char *)*5);
	sprintf(name,"t%d",tmpCount++);
	Info *info = createNodeInfo(name,-1,getOffSet(),type);
	return info;
}

int lblCount = 0;

char *generateNextLabel(){
	char *name = (char *) malloc(sizeof(char *)*5);
	sprintf(name,".L%d",lblCount++);
	return name;
}

int countParams = 0;

int debug_pasada = 1;

Info *generateCod3DList(TreeNode *t) {
	if (t==NULL)
		return NULL;
	//printf("pasada : %d \n\n", debug_pasada++);
	//printCod3DList();
	if (strcmp(t->label,"next")==0)
		generateCod3DList(t->leftChild);
	if (strcmp(t->label,"int")==0 || strcmp(t->label,"bool")==0 || strcmp(t->label,"var")==0 || strcmp(t->label,"global")==0)
		return t->info;
	else if (strcmp(t->label,"asig")==0){
		Op opCod = ASIGI;
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),NULL);
		return NULL;
	}
	else if (strcmp(t->label,"add")==0){
		Op opCod = ADDI;
		Info *res = generateNextTmp("int");
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;	
	}
	else if (strcmp(t->label,"sub")==0){
		Op opCod = SUBI;
		Info *res = generateNextTmp("int");
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;	
	}
	else if (strcmp(t->label,"mul")==0){
		Op opCod = MULI;
		Info *res = generateNextTmp("int");
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;
	}
	else if (strcmp(t->label,"div")==0){
		Op opCod = DIVI;
		Info *res = generateNextTmp("int");
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;
	}
	else if (strcmp(t->label,"mod")==0){
		Op opCod = MODI;
		Info *res = generateNextTmp("int");
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;
	}
	else if (strcmp(t->label,"less")==0){
		Op opCod = LESS;
		Info *res = generateNextTmp("bool");
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;
	}
	else if (strcmp(t->label,"greater")==0){
		Op opCod = GREATER;
		Info *res = generateNextTmp("bool");
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;
	}
	else if (strcmp(t->label,"equal")==0){
		Op opCod = EQUAL;
		Info *res = generateNextTmp("bool");
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;
	}
	else if (strcmp(t->label,"and")==0){
		Op opCod = ANDB;
		Info *res = generateNextTmp("bool");
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;
	}
	else if (strcmp(t->label,"or")==0){
		Op opCod = ORB;
		Info *res = generateNextTmp("bool");
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),res);
		return res;
	}	
	else if (strcmp(t->label,"not")==0){
		Op opCod = NOTB;
		Info *res = generateNextTmp("bool");
		insertCod3D(opCod,generateCod3DList(t->leftChild),NULL,res);
		return res;
	}
	else if (strcmp(t->label,"if")==0){
		Op opCod = IF_SENTENCE;
		insertCod3D(opCod,generateCod3DList(t->leftChild),NULL,NULL);
		if (strcmp(t->rightChild->label,"if_else")!=0){
			Op opCod = THEN_SENTENCE;
			insertCod3D(opCod,NULL,NULL,NULL);	
		}
		generateCod3DList(t->rightChild);
		opCod = ENDIF;
		insertCod3D(opCod,NULL,NULL,NULL);
		//insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),NULL);
		return NULL;
	}
	else if (strcmp(t->label,"if_else")==0){
		Op opCod = THEN_SENTENCE;
		insertCod3D(opCod,NULL,NULL,NULL);
		generateCod3DList(t->leftChild);
		opCod = ELSE_SENTENCE;
		insertCod3D(opCod,NULL,NULL,NULL);
		generateCod3DList(t->rightChild);		
		//insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),NULL);
		return NULL;
	}
	else if (strcmp(t->label,"while")==0){
		Op opCod = WHILE_SENTENCE;
		insertCod3D(opCod,NULL,NULL,NULL);
		generateCod3DList(t->rightChild);
		opCod = MIDWHILE;
		insertCod3D(opCod,NULL,NULL,NULL);
		opCod = ENDWHILE;
		insertCod3D(opCod,generateCod3DList(t->leftChild),NULL,NULL);
		return NULL;
	}
	else if (strcmp(t->label,"function")==0){
		Op opCod = FUNCTION;
		resetOffSet(t->info->lastOffSet);
		insertCod3D(opCod,t->info,NULL,NULL);
		generateCod3DList(t->rightChild);
		t->info->lastOffSet = offSet();
		opCod = ENDFUNCTION;
		insertCod3D(opCod,NULL,NULL,NULL);
		return NULL;
	}
	else if (strcmp(t->label,"extern")==0)
		return NULL;
	else if (strcmp(t->label,"call")==0){
		countParams = 0;
		generateCod3DList(t->leftChild);
		//printParamList()
		int i = 0;
		Op opCod = LOAD;
		while (!isEmpty()){
			insertCod3D(opCod,getParam(),NULL,NULL);
			removeParam();
		}
		opCod = CALL;
		Info *res = generateNextTmp(t->info->type);
		insertCod3D(opCod,t->info,NULL,res);
		return res;
	}
	else if (strcmp(t->label,"actual_arg")==0){
		if (countParams++<6){
			insertParam(generateCod3DList(t->leftChild));
			if (t->rightChild != NULL)
				generateCod3DList(t->rightChild);
	  	}
	  	else{
	  		if (t->rightChild != NULL){
	  			generateCod3DList(t->rightChild);
	  		}
	  		insertParam(generateCod3DList(t->leftChild));
	  	}
		return NULL;
	}
	else if (strcmp(t->label,"return")==0){
		Op opCod = FUNC_RETURN;
		insertCod3D(opCod,generateCod3DList(t->leftChild),NULL,NULL);
		return NULL;
	}
	else if (strcmp(t->label,"global_decl")==0){
		Op opCod = GLOBAL_DECL;
		insertCod3D(opCod,generateCod3DList(t->leftChild),NULL,NULL);
		return NULL;
	}
	else if (strcmp(t->label,"global_asig")==0){
		Op opCod = GLOBAL_ASIG;
		insertCod3D(opCod,generateCod3DList(t->leftChild),generateCod3DList(t->rightChild),NULL);
		return NULL;
	}
	if (strcmp(t->label,"next")==0)
		generateCod3DList(t->rightChild);
}

char* OpCodName(int opCod){
	switch(opCod){
		case 0:
			return "ASIG";
			break;
		case 1:
			return "ADD";
			break;
		case 2:
			return "MUL";
			break;
		case 3:
			return "SUB";
			break;
		case 4:
			return "DIV";
			break;
		case 5:
			return "MOD";
			break;
		case 6:
			return "GLOBAL_DECL";
			break;
		case 7:
			return "LESSER";
			break;
		case 8:
			return "GREATER";
			break;
		case 9:
			return "EQUAL";
			break;
		case 10:
			return "AND";
			break;
		case 11:
			return "OR";
			break;
		case 12:
			return "NOT";
			break;
		case 13:
			return "IF";
			break;
		case 14:
			return "THEN";
			break;
		case 15:
			return "ELSE";
			break;
		case 16:
			return "WHILE";
			break;
		case 17:
			return "ENDIF";
			break;
		case 18:
			return "MIDWHILE";
			break;
		case 19:
			return "ENDWHILE";
			break;
		case 20:
			return "GLOBAL_ASIG";
			break;
		case 21:
			return "FUNC";
			break;
		case 22:
			return "EXTERN";
			break;
		case 23:
			return "ENDFUNC";
			break;
		case 24:
			return "CALL";
			break;
		case 25:
			return "RETURN";
			break;
		case 26:
			return "LOAD";
		default:
			return NULL;
	}
}

void printCod3DList(){
	Cod3D *aux = head;
	while (aux!=NULL){
		printf("[%s,",OpCodName(aux->opCod));
		if (aux->arg1!=NULL)
			printf("(%s,%d,offset=%d),",aux->arg1->name,aux->arg1->value,aux->arg1->offSet);
		else
			printf("null,");
		if (aux->arg2!=NULL)
			printf("(%s,%d,offset=%d),",aux->arg2->name,aux->arg2->value,aux->arg2->offSet);
		else
			printf("null,");
		if (aux->result!=NULL)
			printf("(%s,%d,offset=%d)]\n",aux->result->name,aux->result->value,aux->result->offSet);
		else
			printf("null]\n");
		aux = aux->next;		
	}
}

int loadCount = 0;

void generateAssembly(TreeNode *t, char *fileName){
	checkTypesCorrectnes(t);
	generateCod3DList(t);
	Cod3D *aux = head; 
	char *subStr = malloc(sizeof(char)*31);
	strncpy(subStr,fileName,strlen(fileName)-3);
	strcat(subStr,"s");
	FILE *f = fopen(subStr,"w");
	//fprintf(f,"%s\n","	.globl main");
	//fprintf(f,"%s\n","main:");
	char *else_lbl, *endif_lbl;
	while (aux != NULL){
		switch(aux->opCod){
			case 0:	//asig
				if (aux->arg1->offSet == -1){ 
					if (aux->arg2->name == NULL)
						fprintf(f, "	movq	$%d, %s(%%rip)\n", aux->arg2->value, aux->arg1->name);
					else if (aux->arg2->offSet == -1){
						fprintf(f, "	movq	%s(%%rip), %%rax\n", aux->arg2->name);
						fprintf(f, "	movq	%%rax, %s(%%rip)\n", aux->arg1->name);
					}
					else{
						fprintf(f, "	movq	%d(%%rbp), %%rax\n", aux->arg2->offSet);
						fprintf(f, "	movq	%%rax, %s(%%rip)\n", aux->arg1->name);	
					}
				}
				else{
					if (aux->arg2->name == NULL)
						fprintf(f, "	movq	$%d, %d(%%rbp)\n", aux->arg2->value, aux->arg1->offSet);
					else if (aux->arg2->offSet == -1){
						fprintf(f, "	movq	%s(%%rip), %%rax\n", aux->arg2->name);
						fprintf(f, "	movq	%%rax, %d(%%rbp)\n", aux->arg1->offSet);
					}
					else {
						fprintf(f, "	movq	%d(%%rbp), %%rax\n", aux->arg2->offSet);
						fprintf(f, "	movq	%%rax, %d(%%rbp)\n", aux->arg1->offSet);
					}
				}
				break;
			case 1:	//add
				if (aux->arg1->name == NULL)
					fprintf(f, "	movq	$%d, %%rax\n", aux->arg1->value);
				else if (aux->arg1->offSet == -1)
					fprintf(f, "	movq	%s(%%rip), %%rax\n", aux->arg1->name);
				else
					fprintf(f, "	movq	%d(%%rbp), %%rax\n",aux->arg1->offSet);
				if (aux->arg2->name == NULL)
					fprintf(f, "	addq	$%d, %%rax\n", aux->arg2->value);
				else if (aux->arg2->offSet == -1)
					fprintf(f, "	addq	%s(%%rip), %%rax\n", aux->arg2->name);
				else 
					fprintf(f, "	addq	%d(%%rbp), %%rax\n",aux->arg2->offSet);
				fprintf(f, "	movq	%%rax, %d(%%rbp)\n",aux->result->offSet);
				break;
			case 2:	//mul
				if (aux->arg1->name == NULL)
					fprintf(f, "	movq	$%d, %%rax\n", aux->arg1->value);
				else if (aux->arg1->offSet == -1)
					fprintf(f, "	movq	%s(%%rip), %%rax\n", aux->arg1->name);
				else
					fprintf(f, "	movq	%d(%%rbp), %%rax\n",aux->arg1->offSet);
				if (aux->arg2->name == NULL)
					fprintf(f, "	imulq	$%d, %%rax\n", aux->arg2->value);
				else if (aux->arg2->offSet == -1)
					fprintf(f, "	imulq	%s(%%rip), %%rax\n", aux->arg2->name);
				else 
					fprintf(f, "	imulq	%d(%%rbp), %%rax\n",aux->arg2->offSet);
				fprintf(f, "	movq	%%rax, %d(%%rbp)\n",aux->result->offSet);
				break;
			case 3:	//sub
				if (aux->arg1->name == NULL)
					fprintf(f, "	movq	$%d, %%rax\n", aux->arg1->value);
				else if (aux->arg1->offSet == -1)
					fprintf(f, "	movq	%s(%%rip), %%rax\n", aux->arg1->name);
				else
					fprintf(f, "	movq	%d(%%rbp), %%rax\n",aux->arg1->offSet);
				if (aux->arg2->name == NULL)
					fprintf(f, "	subq	$%d, %%rax\n", aux->arg2->value);
				else if (aux->arg2->offSet == -1)
					fprintf(f, "	subq	%s(%%rip), %%rax\n", aux->arg2->name);
				else 
					fprintf(f, "	subq	%d(%%rbp), %%rax\n",aux->arg2->offSet);
				fprintf(f, "	movq	%%rax, %d(%%rbp)\n",aux->result->offSet);
				break;
			case 4:	//div
				if (aux->arg1->name == NULL)
					fprintf(f, "	movq	$%d, %%rax\n", aux->arg1->value);
				else if (aux->arg1->offSet == -1)
					fprintf(f, "	movq	%s(%%rip), %%rax\n", aux->arg1->name);
				else
					fprintf(f, "	movq	%d(%%rbp), %%rax\n",aux->arg1->offSet);
				fprintf(f, "	cltd\n");
				if (aux->arg2->name == NULL){
					fprintf(f, "	movq	$%d, %%rbx\n", aux->arg2->value);
					fprintf(f, "	idivq	%%rbx\n");
				}
				else if (aux->arg2->offSet == -1){
					fprintf(f, "	movq	%s(%%rip), %%rbx\n", aux->arg2->name);
					fprintf(f, "	idivq	%%rbx\n");
				}
				else 
					fprintf(f, "	idivq	%d(%%rbp)\n",aux->arg2->offSet);
				fprintf(f, "	movq	%%rax, %d(%%rbp)\n",aux->result->offSet);
				break;
			case 5:	//mod
				if (aux->arg1->name == NULL)
					fprintf(f, "	movq	$%d, %%rax\n", aux->arg1->value);
				else if (aux->arg1->offSet == -1)
					fprintf(f, "	movq	%s(%%rip), %%rax\n", aux->arg1->name);
				else
					fprintf(f, "	movq	%d(%%rbp), %%rax\n",aux->arg1->offSet);
				fprintf(f, "	cltd\n");
				if (aux->arg2->name == NULL){
					fprintf(f, "	movq	$%d, %%rbx\n", aux->arg2->value);
					fprintf(f, "	idivq	%%rbx\n");
				}
				else if (aux->arg2->offSet == -1){
					fprintf(f, "	movq	%s(%%rip), %%rbx\n", aux->arg2->name);
					fprintf(f, "	idivq	%%rbx\n");
				}
				else 
					fprintf(f, "	idivq	%d(%%rbp)\n",aux->arg2->offSet);
				fprintf(f, "	movq	%%rdx, %d(%%rbp)\n",aux->result->offSet);
				break;
			case 6:	//global_decl
				fprintf(f, "	.comm	%s,8,8\n", aux->arg1->name);
				break;
			case 7: //lesser
				if (aux->arg1->name == NULL)
					fprintf(f, "	movq	$%d, %%r10\n", aux->arg1->value);
				else if (aux->arg1->offSet == -1)
					fprintf(f, "	movq	%s(%%rip), %%r10\n", aux->arg1->name);
				else
					fprintf(f, "	movq	%d(%%rbp), %%r10\n", aux->arg1->offSet);
				if (aux->arg2->name == NULL)
					fprintf(f, "	movq	$%d, %%r11\n", aux->arg2->value);
				else if (aux->arg2->offSet == -1)
					fprintf(f, "	movq	%s(%%rip), %%r11\n", aux->arg2->name);
				else
					fprintf(f, "	movq	%d(%%rbp), %%r11\n", aux->arg2->offSet);
				fprintf(f, "	cmpq	%%r11, %%r10\n");
				fprintf(f, "	setl	%%al\n");
				fprintf(f, "	movzbl	%%al, %%eax\n");
				fprintf(f, "	movq	%%rax, %d(%%rbp)\n", aux->result->offSet);		
				break;
			case 8:	//greater
				if (aux->arg1->name == NULL)
					fprintf(f, "	movq	$%d, %%r10\n", aux->arg1->value);
				else if (aux->arg1->offSet == -1)
					fprintf(f, "	movq	%s(%%rip), %%r10\n", aux->arg1->name);
				else
					fprintf(f, "	movq	%d(%%rbp), %%r10\n", aux->arg1->offSet);
				if (aux->arg2->name == NULL)
					fprintf(f, "	movq	$%d, %%r11\n", aux->arg2->value);
				else if (aux->arg2->offSet == -1)
					fprintf(f, "	movq	%s(%%rip), %%r11\n", aux->arg2->name);
				else
					fprintf(f, "	movq	%d(%%rbp), %%r11\n", aux->arg2->offSet);
				fprintf(f, "	cmpq	%%r11, %%r10\n");
				fprintf(f, "	setg	%%al\n");
				fprintf(f, "	movzbl	%%al, %%eax\n");
				fprintf(f, "	movq	%%rax, %d(%%rbp)\n", aux->result->offSet);
				break;
			case 9:	//equal
				if (aux->arg1->name == NULL)
					fprintf(f, "	movq	$%d, %%r10\n", aux->arg1->value);
				else if (aux->arg1->offSet == -1)
					fprintf(f, "	movq	%s(%%rip), %%r10\n", aux->arg1->name);
				else
					fprintf(f, "	movq	%d(%%rbp), %%r10\n", aux->arg1->offSet);
				if (aux->arg2->name == NULL)
					fprintf(f, "	movq	$%d, %%r11\n", aux->arg2->value);
				else if (aux->arg2->offSet == -1)
					fprintf(f, "	movq	%s(%%rip), %%r11\n", aux->arg2->name);
				else
					fprintf(f, "	movq	%d(%%rbp), %%r11\n", aux->arg2->offSet);
				fprintf(f, "	cmpq	%%r11, %%r10\n");
				fprintf(f, "	sete	%%al\n");
				fprintf(f, "	movzbl	%%al, %%eax\n");
				fprintf(f, "	movq	%%rax, %d(%%rbp)\n", aux->result->offSet);
				break;
			case 10: //and
				fprintf(f, "	movq	$1, %%rax\n");
				if (aux->arg1->name == NULL)
					fprintf(f, "	andq	$%d, %%rax\n", aux->arg1->value);
				else if (aux->arg1->offSet == -1)
					fprintf(f, "	andq	%s(%%rip), %%rax\n", aux->arg1->name);
				else
					fprintf(f, "	andq	%d(%%rbp), %%rax\n", aux->arg1->offSet);
				if (aux->arg2->name == NULL)
					fprintf(f, "	andq	$%d, %%rax\n", aux->arg2->value);
				else if (aux->arg2->offSet == -1)
					fprintf(f, "	andq	%s(%%rip), %%rax\n", aux->arg2->name);
				else
					fprintf(f, "	andq	%d(%%rbp), %%rax\n", aux->arg2->offSet);
				fprintf(f, "	movq	%%rax, %d(%%rbp)\n", aux->result->offSet);
				break;
			case 11: //or 
				fprintf(f, "	movq	$0, %%rax\n");
				if (aux->arg1->name == NULL)
					fprintf(f, "	orq		$%d, %%rax\n", aux->arg1->value);
				else if (aux->arg1->offSet == -1)
					fprintf(f, "	orq		%s(%%rip), %%rax\n", aux->arg1->name);
				else
					fprintf(f, "	orq		%d(%%rbp), %%rax\n", aux->arg1->offSet);
				if (aux->arg2->name == NULL)
					fprintf(f, "	orq		$%d, %%rax\n", aux->arg2->value);
				else if (aux->arg2->offSet == -1)
					fprintf(f, "	orq		%s(%%rip), %%rax\n", aux->arg2->name);
				else
					fprintf(f, "	orq		%d(%%rbp), %%rax\n", aux->arg2->offSet);
				fprintf(f, "	movq	%%rax, %d(%%rbp)\n", aux->result->offSet);
				break;
			case 12: //not
				if (aux->arg1->name == NULL)
					fprintf(f, "	movq	$%d, %%r10\n", aux->arg1->value);
				else if (aux->arg1->offSet == -1)
					fprintf(f, "	movq	%s(%%rip), %%r10\n", aux->arg1->name);
				else
					fprintf(f, "	movq	%d(%%rbp), %%r10\n", aux->arg1->offSet);
				fprintf(f, "	cmpq	$1, %%r10\n");
				fprintf(f, "	setne	%%al\n");
				fprintf(f, "	movzbl	%%al, %%eax\n");
				fprintf(f, "	movq	%%rax, %d(%%rbp)\n", aux->result->offSet);
				break;
			case 13: //if
				if (aux->arg1->name == NULL)
					fprintf(f, "	movq	$%d, %%r10\n", aux->arg1->value);
				if (aux->arg1->offSet == -1)
					fprintf(f, "	movq 	%s(%%rip), %%r10\n", aux->arg1->name);
				else
					fprintf(f, "	movq	%d(%%rbp), %%r10\n", aux->arg1->offSet);
				fprintf(f, "	cmpq	$0, %%r10\n");
				else_lbl = generateNextLabel();
				fprintf(f, "	je	    %s\n", else_lbl);
				LabelPair *p = (LabelPair *) malloc(sizeof(LabelPair));
				p->else_tag = (char *) malloc(sizeof(else_lbl));
				strcpy(p->else_tag,else_lbl);
				p->endif_tag = NULL;
				push(p);
				break;
			case 14: //then
				break;
			case 15: //else
				endif_lbl = generateNextLabel();
				fprintf(f, "	jmp	%s\n", endif_lbl);
				p = top();
				p->endif_tag = (char *) malloc(sizeof(endif_lbl));
				strcpy(p->endif_tag,endif_lbl);
				fprintf(f,"%s:\n",p->else_tag);
				break;
			case 16: //while
				else_lbl = generateNextLabel();
				endif_lbl = generateNextLabel();
				p = (LabelPair *) malloc(sizeof(LabelPair));
				p->else_tag = (char *) malloc(sizeof(else_lbl));
				strcpy(p->else_tag,else_lbl);
				p->endif_tag = (char *) malloc(sizeof(endif_lbl));
				strcpy(p->endif_tag,endif_lbl);
				push(p);
				fprintf(f, "	jmp		%s\n", p->else_tag);
				fprintf(f, "%s:\n", p->endif_tag);
				break;
			case 17: //endif
				p = top();
				pop();
				if (p->endif_tag == NULL)
					fprintf(f, "%s:\n", p->else_tag);
				else
					fprintf(f, "%s:\n", p->endif_tag);
				break;
			case 18: //midwhile
				p = top();
				fprintf(f, "%s:\n", p->else_tag);
				break;
			case 19: //endwhile
				if (aux->arg1->name == NULL)
					fprintf(f, "	movq	$%d, %%r10\n", aux->arg1->value);
				if (aux->arg1->offSet == -1)
					fprintf(f, "	movq 	%s(%%rip), %%r10\n", aux->arg1->name);
				else
					fprintf(f, "	movq	%d(%%rbp), %%r10\n", aux->arg1->offSet);
				fprintf(f, "	cmpq	$1, %%r10\n");
				p = top();
				pop();
				fprintf(f, "	je 		%s\n", p->endif_tag);
				break;
			case 20: //global_asig
				fprintf(f, "	.globl	%s\n", aux->arg1->name);
				fprintf(f, "	.data\n");
				fprintf(f, "	.align 8\n");
				fprintf(f, "	.type	%s, @object\n", aux->arg1->name);
				fprintf(f, "	.size	%s, 8\n", aux->arg1->name);
				fprintf(f, "%s:\n", aux->arg1->name);
				fprintf(f, "	.quad	%d\n", aux->arg2->value);
				break;
			case 21: //function
				fprintf(f,"    .globl %s\n", aux->arg1->name);
				fprintf(f,"%s:\n",aux->arg1->name);
				if (strcmp(aux->arg1->name,"main")==0)
					fprintf(f, "	enter	$%d, $0\n", -offSet()+16);
				else{
					ListNode *functionNode = findListNode(aux->arg1->name);
					int paramNumber = paramListSize(functionNode->head);
					fprintf(f, "	enter	$%d, $0\n", -aux->arg1->lastOffSet);
					if (paramNumber >= 1)
						fprintf(f, "	movq	%%rdi, -8(%%rbp)\n");
					if (paramNumber >= 2)
						fprintf(f, "	movq	%%rsi, -16(%%rbp)\n");
					if (paramNumber >= 3)
						fprintf(f, "	movq	%%rdx, -24(%%rbp)\n");
					if (paramNumber >= 4)
						fprintf(f, "	movq	%%rcx, -32(%%rbp)\n");
					if (paramNumber >= 5)
						fprintf(f, "	movq	%%r8, -40(%%rbp)\n");
					if (paramNumber >= 6)
						fprintf(f, "	movq	%%r9, -48(%%rbp)\n");
				}
				break;
			case 22:
				break;
			case 23://endfunction
				fprintf(f, "   	leave\n");
				fprintf(f, "	ret\n");
				break;
			case 24: //call
				fprintf(f, "	call 	%s\n", aux->arg1->name);
				fprintf(f, "	movq	%%rax, %d(%%rbp)\n", aux->result->offSet);
				if (loadCount > 6)
					fprintf(f, "	addq 	$%d, %%rsp\n", (loadCount - 6) * 8);
				loadCount = 0;			
				break;
			case 25: //return
				if (aux->arg1->name == NULL)
					fprintf(f, "	movq	$%d, %%rax\n", aux->arg1->value);
				else if (aux->arg1->offSet == -1)
					fprintf(f, "	movq 	%s(%%rip), %%rax\n", aux->arg1->name);
				else
                	fprintf(f, "	movq	%d(%%rbp), %%rax\n", aux->arg1->offSet);
				break;
			case 26: //load
				loadCount++;
				if (aux->arg1->name == NULL){
					if (loadCount == 1)
						fprintf(f, "	movq	$%d, %%rdi\n", aux->arg1->value);
					else if (loadCount == 2)
						fprintf(f, "	movq	$%d, %%rsi\n", aux->arg1->value);
					else if (loadCount == 3)
						fprintf(f, "	movq	$%d, %%rdx\n", aux->arg1->value);
					else if (loadCount == 4)
						fprintf(f, "	movq	$%d, %%rcx\n", aux->arg1->value);
					else if (loadCount == 5)
						fprintf(f, "	movq	$%d, %%r8\n", aux->arg1->value);
					else if (loadCount == 6)
						fprintf(f, "	movq	$%d, %%r9\n", aux->arg1->value);
					else
						fprintf(f, "	pushq	$%d\n", aux->arg1->value);
				}
				else if (aux->arg1->offSet == -1){
					if (loadCount == 1)
						fprintf(f, "	movq	%s(%%rip), %%rdi\n", aux->arg1->name);
					else if (loadCount == 2)
						fprintf(f, "	movq	%s(%%rip), %%rsi\n", aux->arg1->name);
					else if (loadCount == 3)
						fprintf(f, "	movq	%s(%%rip), %%rdx\n", aux->arg1->name);
					else if (loadCount == 4)
						fprintf(f, "	movq	%s(%%rip), %%rcx\n", aux->arg1->name);
					else if (loadCount == 5)
						fprintf(f, "	movq	%s(%%rip), %%r8\n", aux->arg1->name);
					else if (loadCount == 6)
						fprintf(f, "	movq	%s(%%rip), %%r9\n", aux->arg1->name);
					else
						fprintf(f, "	pushq	%s(%%rip)\n", aux->arg1->name);
				}
				else{
					if (loadCount == 1)
						fprintf(f, "	movq	%d(%%rbp), %%rdi\n", aux->arg1->offSet);
					else if (loadCount == 2)
						fprintf(f, "	movq	%d(%%rbp), %%rsi\n", aux->arg1->offSet);
					else if (loadCount == 3)
						fprintf(f, "	movq	%d(%%rbp), %%rdx\n", aux->arg1->offSet);
					else if (loadCount == 4)
						fprintf(f, "	movq	%d(%%rbp), %%rcx\n", aux->arg1->offSet);
					else if (loadCount == 5)
						fprintf(f, "	movq	%d(%%rbp), %%r8\n", aux->arg1->offSet);
					else if (loadCount == 6)
						fprintf(f, "	movq	%d(%%rbp), %%r9\n", aux->arg1->offSet);
					else
						fprintf(f, "	pushq	%d(%%rbp)\n", aux->arg1->offSet);	
				}
				break;

		}	
		aux = aux->next;
	}
	fclose(f);	
}

#endif