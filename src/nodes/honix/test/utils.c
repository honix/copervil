#include "utils.h"

#include <stdio.h>
#include <math.h>

#include "core/link.h"
#include "core/node.h"
#include "core/loop.h"
#include "core/utils.h"
#include "core/dl_loader.h"

// (int, int) -> (int)
void sum_init(struct node *node)
{
	// node->in_pins_mask = 1 << 0 | 1 << 1;
	// node->out_pins_mask = 1 << 0;

	// connect_nodes(make_link(calloc(1, sizeof(int))), NULL, 0, node, 0);
	// connect_nodes(make_link(calloc(1, sizeof(int))), NULL, 0, node, 1);

	init_pins(node, 2, 1);

	reg_pin(node, PIN_INPUT, 0, "number a", "int");
	reg_pin(node, PIN_INPUT, 1, "number b", "int");

	reg_pin(node, PIN_OUTPUT, 0, "result", "int");
}

void sum(struct node *node)
{
	int a = *(int *)get_link_on_pin(node, PIN_INPUT, 0)->data;
	int b = *(int *)get_link_on_pin(node, PIN_INPUT, 1)->data;
	*(int *)get_link_on_pin(node, PIN_OUTPUT, 0)->data = a + b;
}

// (int) -> ()
void print_int_init(struct node *node)
{
	// node->in_pins_mask = 1 << 0;
	init_pins(node, 1, 0);
	reg_pin(node, PIN_INPUT, 0, "number", "int");
}

void print_int(struct node *node)
{
	int number = *(int *)get_link_on_pin(node, PIN_INPUT, 0)->data;
	printf("%d\n", number);
}

// (int) -> (trigger, int)
void do_times_init(struct node *node)
{
	// node->in_pins_mask = 1 << 0;
	// node->out_pins_mask = 1 << 0 | 1 << 1;
	init_pins(node, 1, 2);
	reg_pin(node, PIN_INPUT, 0, "times", "int");

	reg_pin(node, PIN_OUTPUT, 0, "trigger", "trigger");
	reg_pin(node, PIN_OUTPUT, 1, "times", "int");
}

void do_times(struct node *node)
{
	int count = *(int *)get_link_on_pin(node, PIN_INPUT, 0)->data;
	for (int i = 0; i < count; i++)
	{
		*(int *)get_link_on_pin(node, PIN_OUTPUT, 0)->data = i;
		direct_call_node_on_pin(node, 0);
		// inderect_call_node(node->out_pins[0]->receiver, 0);
	}
}

// (int, double) -> (int/trigger)
void do_times_inderect_init(struct node *node)
{
	// node->in_pins_mask = 1 << 0 | 1 << 1;
	// node->out_pins_mask = 1 << 0;

	// TODO: oh we cant initialize pins before link comes in
	// *(int *)node->out_pins[0]->data = 0;

	init_pins(node, 2, 1);
	reg_pin(node, PIN_INPUT, 0, "times", "int");
	reg_pin(node, PIN_INPUT, 1, "delay", "double");

	reg_pin(node, PIN_OUTPUT, 0, "int/trigger", "int");
}

void do_times_inderect(struct node *node)
{
	int count = *(int *)get_link_on_pin(node, PIN_INPUT, 0)->data;
	double time_step = *(double *)get_link_on_pin(node, PIN_INPUT, 1)->data;
	int do_count;

	// use out pin as cycle count state
	if (get_link_on_pin(node, PIN_OUTPUT, 0)->data == NULL)
	{
		do_count = 0;
	}
	else
	{
		do_count = *(int *)get_link_on_pin(node, PIN_OUTPUT, 0)->data;
	}

	// printf("count = %d, do_count = %d\n", count, do_count);
	if (count > do_count)
	{
		direct_call_node_on_pin(node, 0);
		*(int *)get_link_on_pin(node, PIN_OUTPUT, 0)->data = do_count + 1;
		delayed_call_node_self(node, time_step);
	}
}

// (double) -> (trigger)
void loop_init(struct node *node)
{
	// node->in_pins_mask = 1 << 0;
	// node->out_pins_mask = 1 << 0;

	init_pins(node, 1, 1);
	reg_pin(node, PIN_INPUT, 0, "delay", "double");
	*(double *)get_link_on_pin(node, PIN_INPUT, 0)->data = 1.0/60;
	reg_pin(node, PIN_OUTPUT, 0, "trigger", "trigger");

	node->only_self_trigger = true;

	delayed_call_node_self(node, 0);
}

void loop(struct node *node)
{
	double time_step = *(double *)get_link_on_pin(node, PIN_INPUT, 0)->data;

	// direct_call_node(get_link_on_pin(node, PIN_OUTPUT, 0)->receiver);
	direct_call_node_on_pin(node, 0);

	delayed_call_node_self(node, time_step);
}

void lfo_init(struct node *node)
{
	init_pins(node, 2, 2);
	reg_pin(node, PIN_INPUT, 0, "trigger", "trigger");
	reg_pin(node, PIN_INPUT, 1, "freq", "int"); // TODO: double
	reg_pin(node, PIN_OUTPUT, 0, "trigger", "trigger");
	reg_pin(node, PIN_OUTPUT, 1, "value", "double");

	// delayed_call_node_on_pin(node, 0);
}

void lfo(struct node *node)
{
	*(double *)get_link_on_pin(node, PIN_OUTPUT, 1)->data +=
		*(int *)get_link_on_pin(node, PIN_INPUT, 1)->data * 1.0/60; // ignore relatime for now

	*(double *)get_link_on_pin(node, PIN_OUTPUT, 1)->data =
		fmod(*(double *)get_link_on_pin(node, PIN_OUTPUT, 1)->data, 1.0);

	direct_call_node_on_pin(node, 0);
	// delayed_call_node_on_pin(node, 1/60);
}

void register_library()
{
	register_function((struct function_note){
		"sum", sum_init, sum});
	register_function((struct function_note){
		"print_int", print_int_init, print_int});
	register_function((struct function_note){
		"do_times", do_times_init, do_times});
	register_function((struct function_note){
		"do_times_inderect", do_times_inderect_init, do_times_inderect});
	register_function((struct function_note){
		"loop", loop_init, loop});
	register_function((struct function_note){
		"lfo", lfo_init, lfo});
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
