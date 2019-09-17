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

#endif