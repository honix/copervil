#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#include "node_api.h"

void test_do_times()
{
	// void *handle;
	// handle = dlopen("./src/nodes/honix/test/utils.so", RTLD_LAZY);
	// void (*do_times_inderect) (struct node *node);
	// void (*print_int) (struct node *node);
	// do_times_inderect = dlsym(handle, "do_times_inderect");
	// print_int = dlsym(handle, "print_int");

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
	// struct node *node1 = make_node("node1", do_times);
	make_node(200, 200, get_function_note("patch_editor"));

	// direct_call_node(node1);
}

void test_user_window()
{
	struct node *node_window = make_node(400, 125, get_function_note("window"));
	// connect_nodes(make_link(malloc(sizeof(int))), node1, 0, NULL, 0);

	// double *h1 = malloc(sizeof(double));
	// *h1 = 1.0 / 60;

	// struct node *node2 = make_node(400, 10, get_function_note("loop"));
	// connect_nodes(make_link(NULL), node2, 0, node1, 0);
	// connect_nodes(make_link(h1), NULL, 0, node2, 0);

	struct node *node_loop = make_node(400, 10, get_function_note("loop"));
	struct node *node_lfo = make_node(400, 75, get_function_note("lfo"));
	GET_PIN(node_lfo, PIN_INPUT, 1, double) = 0.25;

	struct node *node_draw_triangle = make_node(450, 200, get_function_note("draw_triangle"));

	connect_nodes(node_loop, 0, node_lfo, 0);
	connect_nodes(node_lfo, 0, node_window, 0);
	connect_nodes(node_lfo, 1, node_draw_triangle, 1);
	connect_nodes(node_window, 0, node_draw_triangle, 0);

	// direct_call_node(node2);
}

void test_number_io()
{
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
	struct node *in1 = make_node(10, 10, get_function_note("number_io"));
	struct node *in2 = make_node(200, 10, get_function_note("number_io"));

	struct node *add = make_node(10, 100, get_function_note("add"));

	struct node *out = make_node(10, 200, get_function_note("number_io"));

	struct node *print = make_node(10, 300, get_function_note("print_number"));

	connect_nodes(in1, 1, add, 1);
	connect_nodes(in2, 1, add, 2);
	connect_nodes(add, 1, out, 1);
	connect_nodes(out, 1, print, 1);

	connect_nodes(in1, 0, in2, 0);
	connect_nodes(in2, 0, add, 0);
	connect_nodes(add, 0, out, 0);
	connect_nodes(out, 0, print, 0);
}

void init_subsystems()
{
	init_loop_subsystem();
	init_nodes_subsystem();
	init_links_subsystem();
	init_dl_loader_subsystem();
}

int main(int acount, char **args)
{
	printf("=== start ===\n");

	init_subsystems();

	load_library("./src/nodes/honix/patch_editor/patch_editor.so");
	load_library("./src/nodes/honix/patch_editor/number_io.so");
	load_library("./src/nodes/honix/test/utils.so");
	load_library("./src/nodes/honix/window/window.so");

	//test_arrays();
	//test_nodes();
	// test_do_times();
	test_sum_node();

	test_user_window();
	// test_user_window(); // make another one!
	// test_user_window(); // make another one!
	// test_user_window(); // make another one!

	// test_number_io();

	test_patch_editor();
	
	loop_run();

	printf("=== end ===\n");

	// sx_dump_leaks();

	return EXIT_SUCCESS;
}
