#include <stdio.h>

int printi(int x){
	printf("%d\n", x);
	return 0;
}

int printb(int x){
	if (x)
		printf("true\n");
	else
		printf("false\n");
	return 0;
}