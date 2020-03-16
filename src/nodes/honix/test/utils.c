#include "utils.h"

#include <stdio.h>

#include "../../../core/loop.h"
#include "../../../core/link.h"
#include "../../../core/node.h"

// (int, int) -> (int)
void sum (struct node *node) 
{
	int a = * (int*) node->in_pins[0]->data;
	int b = * (int*) node->in_pins[1]->data;
	* (int*) node->out_pins[0]->data = a + b;
}

// (int) -> ()
void print_int (struct node *node)
{
	int number = * (int*) node->in_pins[0]->data;
	printf("%d\n", number);
}

// () -> (trigger, int)
void do_times (struct node *node)
{
	int count = * (int*) node->in_pins[0]->data;
	for (int i = 0; i < count; i++) {
		* (int*) node->out_pins[1]->data = i; 
		direct_call_node(node->out_pins[0]->receiver);
		// inderect_call_node(node->out_pins[0]->receiver, 0);
	}
}

// () -> (int/trigger)
void do_times_inderect (struct node *node)
{
	int count = * (int*) node->in_pins[0]->data;
	int do_count;
	
	if (node->out_pins[0]->data == NULL) {
		do_count = 0;
	} else {
		do_count = * (int*) node->out_pins[0]->data;
	}

	printf("count = %d, do_count = %d\n", count, do_count);
	if (count > do_count) {
		direct_call_node(node->out_pins[0]->receiver);
		* (int*) node->out_pins[0]->data = do_count + 1;
		inderect_call_node(node, 0); // self-loop
	}
}

void register_library (void (*reg) (char*, void (*) (struct node*)))
{
	reg("sum", sum);
	reg("print_int", print_int);
	reg("do_times", do_times);
	reg("do_times_inderect", do_times_inderect);
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
