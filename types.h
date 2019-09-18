#ifndef TYPES_H
#define TYPES_H

typedef struct Info {
	char *name;
	int value;
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
} Op;

typedef struct Cod3D {
	Op opCod;
	Info *arg1;
	Info *arg2;
	Info *result;
	struct Cod3D *next;
} Cod3D;

#endif