#include "utils.h"

// (int, int) -> int
void sum (node_t *node) {
	int a = * (int*) node->in1->data;
	int b = * (int*) node->in2->data;
	* (int*) node->out1->data = a + b;
}

/*

// https://www.geeksforgeeks.org/function-pointer-in-c/

void *sum (const void *a, const void *b) {
	int a = * (int*) a;
	int b = * (int*) b;
	return &(a + b);
}

void *mul (const void *a, const void *b) {
	int a = * (int*) a;
	int b = * (int*) b;
	return &(a * b);
}

*/
