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
	node_t *node1 = make_node(sum);
	int h1 = 100;
	int h2 = 25;
	connect_nodes(make_link(&h1), NULL, 0, node1, 0);
	connect_nodes(make_link(&h2), NULL, 0, node1, 1);

	node_t *node2 = make_node(sum);
	int h3 = 25;
	connect_nodes(make_link(malloc(sizeof(int))), node1, 0, node2, 0);
	connect_nodes(make_link(&h3), NULL, 0, node2, 1);

	node2->out_pins[0] = make_link(malloc(sizeof(int)));
	connect_nodes(make_link(malloc(sizeof(int))), node2, 0, NULL, 0);

	execute_node(node1); // connected node will be executed

	printf("%d\n", * (int*) node2->out_pins[0]->data);
	
	// node->in2 = link_out1;
	// execute_node(node);
	// print_array(node->out1->data);
}

int main (int acount, char **args) 
{
	//test_arrays();
	test_nodes();
	
	printf("hello\n");
	return 0;
}

