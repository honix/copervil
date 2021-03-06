#include <stdio.h>
#include <math.h>

#include "sx/timer.h"

#include "core/node_api.h"

#define DEFINE_MATH_OP(name, op)                         \
	void name##_op_init(struct node *node)               \
	{                                                    \
		init_pins(node, true, 2, 1);                     \
		REG_PIN(node, PIN_INPUT, 1, "number a", double); \
		REG_PIN(node, PIN_INPUT, 2, "number b", double); \
		REG_PIN(node, PIN_OUTPUT, 1, "result", double);  \
	}                                                    \
	void name##_op(struct node *node)                    \
	{                                                    \
		GET_PIN(node, PIN_OUTPUT, 1, double) =           \
			GET_PIN(node, PIN_INPUT, 1, double) op       \
				GET_PIN(node, PIN_INPUT, 2, double);     \
	}

DEFINE_MATH_OP(add, +);
DEFINE_MATH_OP(sub, -);
DEFINE_MATH_OP(mul, *);
DEFINE_MATH_OP(div, /);

void print_int_init(struct node *node)
{
	init_pins(node, true, 1, 0);
	REG_PIN(node, PIN_INPUT, 1, "number", int);
}

void print_int(struct node *node)
{
	int number = GET_PIN(node, PIN_INPUT, 1, int);
	printf("%d\n", number);
}

void print_number_init(struct node *node)
{
	init_pins(node, true, 1, 0);
	REG_PIN(node, PIN_INPUT, 1, "number", double);
}

void print_number(struct node *node)
{
	double number = GET_PIN(node, PIN_INPUT, 1, double);
	printf("%f\n", number);
}

void do_times_init(struct node *node)
{
	init_pins(node, true, 1, 2);
	REG_PIN(node, PIN_INPUT, 1, "count", double);
	REG_PIN(node, PIN_OUTPUT, 1, "cycle", double);
	REG_PIN(node, PIN_OUTPUT, 2, "cycle/count", double);

	node->auto_call_next = false;
}

void do_times(struct node *node)
{
	int count = (int)GET_PIN(node, PIN_INPUT, 1, double);
	for (int i = 0; i < count; i++)
	{
		GET_PIN(node, PIN_OUTPUT, 1, double) = i;
		GET_PIN(node, PIN_OUTPUT, 2, double) = (double)i / count;
		direct_call_node_on_pin(node, 0);
	}
}

void on_open_init(struct node *node)
{
	init_pins(node, true, 0, 0);

	delayed_call_node_self(node, 0);
}

void on_open(struct node *node)
{
}

// // (int, double) -> (int/trigger)
// void do_times_inderect_init(struct node *node)
// {
// 	// node->in_pins_mask = 1 << 0 | 1 << 1;
// 	// node->out_pins_mask = 1 << 0;

// 	// TODO: oh we cant initialize pins before link comes in
// 	// *(int *)node->out_pins[0]->data = 0;

// 	init_pins(node, true, 2, 1);
// 	REG_PIN(node, PIN_INPUT, 0, "times", int);
// 	REG_PIN(node, PIN_INPUT, 1, "delay", double);

// 	REG_PIN(node, PIN_OUTPUT, 0, "int/trigger", int);

// 	// node->auto_call_next = false;
// }

// void do_times_inderect(struct node *node)
// {
// 	int count = *(int *)get_link_on_pin(node, PIN_INPUT, 0)->data;
// 	double time_step = *(double *)get_link_on_pin(node, PIN_INPUT, 1)->data;
// 	int do_count;

// 	// use out pin as cycle count state
// 	if (get_link_on_pin(node, PIN_OUTPUT, 0)->data == NULL)
// 	{
// 		do_count = 0;
// 	}
// 	else
// 	{
// 		do_count = *(int *)get_link_on_pin(node, PIN_OUTPUT, 0)->data;
// 	}

// 	// printf("count = %d, do_count = %d\n", count, do_count);
// 	if (count > do_count)
// 	{
// 		direct_call_node_on_pin(node, 0);
// 		*(int *)get_link_on_pin(node, PIN_OUTPUT, 0)->data = do_count + 1;
// 		delayed_call_node_self(node, time_step);
// 	}
// }

void loop_init(struct node *node)
{
	init_pins(node, true, 1, 0);
	REG_PIN(node, PIN_INPUT, 1, "delay", double);

	node->auto_call_next = false;

	GET_PIN(node, PIN_INPUT, 1, double) = 1.0 / 60;
}

void loop(struct node *node)
{
	double time_step = GET_PIN(node, PIN_INPUT, 1, double);

	direct_call_node_on_pin(node, 0);

	delayed_call_node_self(node, time_step);
}

struct lfo_state
{
	uint64_t previous_time;
};

struct lfo_state *get_lfo_state(struct node *node)
{
	return (struct lfo_state *)node->inner_state;
}

void lfo_init(struct node *node)
{
	init_pins(node, true, 1, 1);
	REG_PIN(node, PIN_INPUT, 1, "freq", double);
	REG_PIN(node, PIN_OUTPUT, 1, "value", double);

	node->inner_state = malloc(sizeof(struct lfo_state));
	get_lfo_state(node)->previous_time = sx_tm_now();
}

void lfo(struct node *node)
{
	*(double *)get_link_on_pin(node, PIN_OUTPUT, 1)->data +=
		*(double *)get_link_on_pin(node, PIN_INPUT, 1)->data *
		sx_tm_sec(sx_tm_since(get_lfo_state(node)->previous_time));

	*(double *)get_link_on_pin(node, PIN_OUTPUT, 1)->data =
		fmod(*(double *)get_link_on_pin(node, PIN_OUTPUT, 1)->data, 1.0);

	get_lfo_state(node)->previous_time = sx_tm_now();
}

void double_to_int_init(struct node *node)
{
	init_pins(node, true, 1, 1);
	REG_PIN(node, PIN_INPUT, 1, "number", double);
	REG_PIN(node, PIN_OUTPUT, 1, "int", int);
}

void double_to_int(struct node *node)
{
	GET_PIN(node, PIN_OUTPUT, 1, int) =
		GET_PIN(node, PIN_INPUT, 1, double);
}

void int_to_double_init(struct node *node)
{
	init_pins(node, true, 1, 1);
	REG_PIN(node, PIN_INPUT, 1, "int", int);
	REG_PIN(node, PIN_OUTPUT, 1, "number", double);
}

void int_to_double(struct node *node)
{
	GET_PIN(node, PIN_OUTPUT, 1, double) =
		GET_PIN(node, PIN_INPUT, 1, int);
}

void register_library()
{
	register_function((struct function_note){
		"add", add_op_init, add_op});
	register_function((struct function_note){
		"sub", sub_op_init, sub_op});
	register_function((struct function_note){
		"mul", mul_op_init, mul_op});
	register_function((struct function_note){
		"div", div_op_init, div_op});

	register_function((struct function_note){
		"print_int", print_int_init, print_int});
	register_function((struct function_note){
		"print_number", print_number_init, print_number});
	register_function((struct function_note){
		"do_times", do_times_init, do_times});
	register_function((struct function_note){
		"on_open", on_open_init, on_open});
	// register_function((struct function_note){
	// 	"do_times_inderect", do_times_inderect_init, do_times_inderect});
	register_function((struct function_note){
		"loop", loop_init, loop});
	register_function((struct function_note){
		"lfo", lfo_init, lfo});
	register_function((struct function_note){
		"double_to_int", double_to_int_init, double_to_int});
	register_function((struct function_note){
		"int_to_double", int_to_double_init, int_to_double});
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
