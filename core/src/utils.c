#include "utils.h"

#include <stdio.h>

// (int, int) -> (int)
void sum (node_t *node) 
{
	int a = * (int*) node->in_pins[0]->data;
	int b = * (int*) node->in_pins[1]->data;
	* (int*) node->out_pins[0]->data = a + b;
}

// (int) -> ()
void print_int (node_t *node)
{
	int number = * (int*) node->in_pins[0]->data;
	printf("%d\n", number);
}

// () -> (trigger, int)
void do_times (node_t *node)
{
	int count = * (int*) node->in_pins[0]->data;
	for (int i = 0; i < count; i++) {
		* (int*) node->out_pins[1]->data = i; 
		execute_node(node->out_pins[0]->receiver);
	}
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
