#ifndef TYPES_H
#define TYPES_H

typedef struct Info {
	char *name;
	int value;
	int offSet;
	char *type;
} Info;

typedef struct ListNode {
	Info *info;
	struct ListNode *next;
	int initialized;
} ListNode;

typedef struct TreeNode{
	Info *info;
	struct TreeNode *leftChild;
	struct TreeNode *rightChild;
	char *label;
}TreeNode;

//enum Op{ASIGI,ADDI,MULI,SUBI,DIVI,MODI,PRINT};

typedef enum {
	ASIGI,
	ADDI,
	MULI,
	SUBI,
	DIVI,
	MODI,
	PRINT,
	LESS,
	GREATER,
	EQUAL,
	ANDB,
	ORB,
	NOTB,
	IF_SENTENCE,
	THEN_SENTENCE,
	ELSE_SENTENCE,
	WHILE_SENTENCE,
	ENDIF,
	MIDWHILE,
	ENDWHILE	
} Op;

typedef struct Cod3D {
	Op opCod;
	Info *arg1;
	Info *arg2;
	Info *result;
	struct Cod3D *next;
} Cod3D;

typedef struct LabelPair {
	char *else_tag;
	char *endif_tag;
} LabelPair;

typedef struct StackNode {
	LabelPair *pair;
	struct StackNode *next; 
} StackNode;

#endif