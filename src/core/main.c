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

	int *h1 = malloc(sizeof(int));
	*h1 = 3;

	double *h2 = malloc(sizeof(double));
	*h2 = 1.0;

	// struct node *node1 = make_node("node1", do_times);
	struct node *node1 = make_node("do_times_inderect", 300, 300, get_function("do_times_inderect"));
	connect_nodes(make_link(h1), NULL, 0, node1, 0);
	connect_nodes(make_link(h2), NULL, 0, node1, 1);

	struct node *node2 = make_node("print_int", 400, 400, get_function("print_int"));
	connect_nodes(make_link(malloc(sizeof(int))), node1, 0, node2, 0);

	direct_call_node(node1);
}

void test_patch_editor()
{
	load_library("./src/nodes/honix/patch_editor/patch_editor.so");

	double *h1 = malloc(sizeof(double));
	*h1 = 1.0 / 60;

	// struct node *node1 = make_node("node1", do_times);
	struct node *node1 = make_node("loop", 100, 100, get_function("loop"));
	connect_nodes(make_link(h1), NULL, 0, node1, 0);

	struct node *node2 = make_node("patch_editor", 200, 200, get_function("patch_editor"));
	connect_nodes(make_link(NULL), node1, 0, node2, 0);

	direct_call_node(node1);
}

int main(int acount, char **args)
{
	printf("=== start ===\n");

	init_nodes();
	init_dl_loader();
	//test_arrays();
	//test_nodes();
	test_do_times();
	test_patch_editor();

	loop_run();

	printf("=== end ===\n");
	
	return EXIT_SUCCESS;
}
