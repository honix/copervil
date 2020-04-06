#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "sized_array.h"
#include "link.h"
#include "node.h"
#include "loop.h"
#include "dl_loader.h"
// #include "utils.h"

// void test_arrays () {
// 	struct sized_array
// 		*array_a = make_array(10, 0 , 1),
// 		*array_b = make_array(2, 0, 1),
// 		*array_c = allocate_array(MAX(array_a->size, array_b->size));

// 	printf("array a\n");
// 	print_array(array_a);
// 	printf("array b\n");
// 	print_array(array_b);

// 	printf("sum\n");
// 	apply_arrays(sum, array_a, array_b, array_c);
// 	print_array(array_c);

// 	// printf("mul\n");
// 	// apply_arrays(mul, array_a, array_b, array_c);
// 	// print_array(array_c);
// }

// void test_nodes ()
// {
// 	int h1 = 100;
// 	int h2 = 25;
// 	struct node *node1 = make_node("node1", 0, 0, sum);
// 	connect_nodes(make_link(&h1), NULL, 0, node1, 0);
// 	connect_nodes(make_link(&h2), NULL, 0, node1, 1);

// 	int h3 = 25;
// 	struct node *node2 = make_node("node2", 0, 0, sum);
// 	connect_nodes(make_link(malloc(sizeof(int))), node1, 0, node2, 0);
// 	connect_nodes(make_link(&h3), NULL, 0, node2, 1);

// 	struct node *node3 = make_node("node3", 0, 0, print_int);
// 	connect_nodes(make_link(malloc(sizeof(int))), node2, 0, node3, 0);

// 	direct_call_node(node1); // connected node will be executed
// }

void test_do_times()
{
	// void *handle;
	// handle = dlopen("./src/nodes/honix/test/utils.so", RTLD_LAZY);
	// void (*do_times_inderect) (struct node *node);
	// void (*print_int) (struct node *node);
	// do_times_inderect = dlsym(handle, "do_times_inderect");
	// print_int = dlsym(handle, "print_int");

	load_library("./src/nodes/honix/test/utils.so");

	// struct node *node1 = make_node("node1", do_times);
	struct node *node1 = make_node(300, 300, get_function_note("do_times_inderect"));
	GET_PIN(node1, PIN_INPUT, 0, int) = 10;
	GET_PIN(node1, PIN_INPUT, 1, double) = 1.0;

	struct node *node2 = make_node(400, 400, get_function_note("print_int"));
	connect_nodes(node1, 0, node2, 0);

	direct_call_node_self(node1);
}

void test_patch_editor()
{
	load_library("./src/nodes/honix/patch_editor/patch_editor.so");

	// struct node *node1 = make_node("node1", do_times);
	struct node *node1 = make_node(200, 100, get_function_note("loop"));
	GET_PIN(node1, PIN_INPUT, 0, double) = 1.0 / 60;

	struct node *node2 = make_node(200, 200, get_function_note("patch_editor"));
	connect_nodes(node1, 0, node2, 0);

	// direct_call_node(node1);
}

void test_user_window()
{
	load_library("./src/nodes/honix/window/window.so");

	struct node *node_window = make_node(400, 125, get_function_note("make_window"));
	// connect_nodes(make_link(malloc(sizeof(int))), node1, 0, NULL, 0);

	// double *h1 = malloc(sizeof(double));
	// *h1 = 1.0 / 60;

	// struct node *node2 = make_node(400, 10, get_function_note("loop"));
	// connect_nodes(make_link(NULL), node2, 0, node1, 0);
	// connect_nodes(make_link(h1), NULL, 0, node2, 0);

	struct node *node_loop = make_node(400, 10, get_function_note("loop"));
	struct node *node_lfo = make_node(400, 75, get_function_note("lfo"));
	GET_PIN(node_lfo, PIN_INPUT, 1, double) = 0.25;

	connect_nodes(node_loop, 0, node_lfo, 0);
	connect_nodes(node_lfo, 0, node_window, 0);
	connect_nodes(node_lfo, 1, node_window, 1);

	// direct_call_node(node2);
}

void test_number_io()
{
	load_library("./src/nodes/honix/patch_editor/number_io.so");

	int *h1 = malloc(sizeof(int));
	*h1 = 100;

	double *h2 = malloc(sizeof(double));
	*h2 = 0.1;

	// struct node *node1 = make_node("node1", do_times);
	struct node *node1 = make_node(10, 300, get_function_note("do_times_inderect"));
	GET_PIN(node1, PIN_INPUT, 0, int) = 100;
	GET_PIN(node1, PIN_INPUT, 1, double) = 0.1;

	struct node *node2 = make_node(10, 450, get_function_note("number_io"));
	connect_nodes(node1, 0, node2, 0);

	direct_call_node_self(node1);
}

void test_sum_node()
{
	load_library("./src/nodes/honix/patch_editor/number_io.so");

	make_node(10, 10, get_function_note("number_io"));
	make_node(200, 10, get_function_note("number_io"));

	make_node(10, 100, get_function_note("sum"));

	make_node(10, 200, get_function_note("number_io"));
}

void init_subsystems()
{
	init_nodes_subsystem();
	init_links_subsystem();
	init_dl_loader_subsystem();
}

int main(int acount, char **args)
{
	printf("=== start ===\n");

	init_subsystems();

	//test_arrays();
	//test_nodes();
	test_do_times();
	test_patch_editor();
	test_sum_node();

	test_user_window();
	// test_user_window(); // make another one!
	// test_user_window(); // make another one!
	// test_user_window(); // make another one!

	test_number_io();

	loop_run();

	printf("=== end ===\n");

	return EXIT_SUCCESS;
}
