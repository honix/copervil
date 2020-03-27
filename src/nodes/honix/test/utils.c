#include "utils.h"

#include <stdio.h>

#include "core/link.h"
#include "core/node.h"
#include "core/loop.h"
#include "core/utils.h"
#include "core/dl_loader.h"

// (int, int) -> (int)
void sum_init(struct node *node)
{
	node->in_pins_mask = 1 << 0 | 1 << 1;
	node->out_pins_mask = 1 << 0;
}

void sum(struct node *node)
{
	int a = *(int *)node->in_pins[0]->data;
	int b = *(int *)node->in_pins[1]->data;
	*(int *)node->out_pins[0]->data = a + b;
}

// (int) -> ()
void print_int_init(struct node *node)
{
	node->in_pins_mask = 1 << 0;
}

void print_int(struct node *node)
{
	int number = *(int *)node->in_pins[0]->data;
	printf("%d\n", number);
}

// (int) -> (trigger, int)
void do_times_init(struct node *node)
{
	node->in_pins_mask = 1 << 0;
	node->out_pins_mask = 1 << 0 | 1 << 1;
}

void do_times(struct node *node)
{
	int count = *(int *)node->in_pins[0]->data;
	for (int i = 0; i < count; i++)
	{
		*(int *)node->out_pins[1]->data = i;
		direct_call_node(node->out_pins[0]->receiver);
		// inderect_call_node(node->out_pins[0]->receiver, 0);
	}
}

// (int, double) -> (int/trigger)
void do_times_inderect_init(struct node *node)
{
	node->in_pins_mask = 1 << 0 | 1 << 1;
	node->out_pins_mask = 1 << 0;

	// TODO: oh we cant initialize pins before link comes in
	// *(int *)node->out_pins[0]->data = 0;
}

void do_times_inderect(struct node *node)
{
	int count = *(int *)node->in_pins[0]->data;
	double time_step = *(double *)node->in_pins[1]->data;
	int do_count;

	// use out pin as cycle count state
	if (node->out_pins[0]->data == NULL)
	{
		do_count = 0;
	}
	else
	{
		do_count = *(int *)node->out_pins[0]->data;
	}

	// printf("count = %d, do_count = %d\n", count, do_count);
	if (count > do_count)
	{
		direct_call_node(node->out_pins[0]->receiver);
		*(int *)node->out_pins[0]->data = do_count + 1;
		delayed_call_node(node, time_step);
	}
}

// (double) -> (trigger)
void loop_init(struct node *node)
{
	node->in_pins_mask = 1 << 0;
	node->out_pins_mask = 1 << 0;
}

void loop(struct node *node)
{
	double time_step = *(double *)node->in_pins[0]->data;

	direct_call_node(node->out_pins[0]->receiver);
	delayed_call_node(node, time_step);
}

void register_library(reg_function_t reg)
{
	reg("sum", sum_init, sum, NULL, NULL, NULL);
	reg("print_int", print_int_init, print_int, NULL, NULL, NULL);
	reg("do_times", do_times_init, do_times, NULL, NULL, NULL);
	reg("do_times_inderect", 
		do_times_inderect_init, do_times_inderect, NULL, NULL, NULL);
	reg("loop", loop_init, loop, NULL, NULL, NULL);
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
