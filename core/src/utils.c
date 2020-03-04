#include "utils.h"

#include <stdio.h>

#include "loop.h"
#include "link.h"
#include "node.h"

// (int, int) -> (int)
void sum (struct node_t *node) 
{
	int a = * (int*) node->in_pins[0]->data;
	int b = * (int*) node->in_pins[1]->data;
	* (int*) node->out_pins[0]->data = a + b;
}

// (int) -> ()
void print_int (struct node_t *node)
{
	int number = * (int*) node->in_pins[0]->data;
	printf("%d\n", number);
}

// () -> (trigger, int)
void do_times (struct node_t *node)
{
	int count = * (int*) node->in_pins[0]->data;
	for (int i = 0; i < count; i++) {
		* (int*) node->out_pins[1]->data = i; 
		direct_call_node(node->out_pins[0]->receiver);
		// inderect_call_node(node->out_pins[0]->receiver, 0);
	}
}

// () -> (int/trigger)
void do_times_inderect (struct node_t *node)
{
	int count = * (int*) node->in_pins[0]->data;
	int do_count;
	
	if (node->out_pins[0]->data == NULL) {
		do_count = 0;
	} else {
		do_count = * (int*) node->out_pins[0]->data;
	}

	// printf("count = %d, do_count = %d\n", count, do_count);
	if (count > do_count) {
		direct_call_node(node->out_pins[0]->receiver);
		* (int*) node->out_pins[0]->data = do_count + 1;
		inderect_call_node(node, 0); // self-loop
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
