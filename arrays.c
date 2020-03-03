#include <stdio.h>
#include <stdlib.h>

#include "sized_array.h"
#include "node.h"
#include "link.h"
#include "utils.h"

// void test_arrays () {
// 	sized_array_t 
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

void test_nodes () 
{
	int h1 = 100;
	int h2 = 25;
	node_t *node1 = make_node("node1", sum);
	connect_nodes(make_link(&h1), NULL, 0, node1, 0);
	connect_nodes(make_link(&h2), NULL, 0, node1, 1);

	int h3 = 25;
	node_t *node2 = make_node("node2", sum);
	connect_nodes(make_link(malloc(sizeof(int))), node1, 0, node2, 0);
	connect_nodes(make_link(&h3), NULL, 0, node2, 1);

	node_t *node3 = make_node("node3", print_int);
	connect_nodes(make_link(malloc(sizeof(int))), node2, 0, node3, 0);

	execute_node(node1); // connected node will be executed
}

void test_do_times ()
{
	int h1 = 10;
	node_t *node1 = make_node("node1", do_times);
	connect_nodes(make_link(&h1), NULL, 0, node1, 0);

	node_t *node2 = make_node("node2", print_int);
	connect_nodes(make_link(NULL), node1, 0, node2, 15);
	connect_nodes(make_link(malloc(sizeof(int))), node1, 1, node2, 0);

	execute_node(node1);
}

int main (int acount, char **args) 
{
	printf("=== start ===\n");

	//test_arrays();
	//test_nodes();
	test_do_times();
	
	printf("=== end ===\n");
	return 0;
}

