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

void test_nodes () {
	// sized_array_t *array_a = make_array(10, 0 , 1);
	// sized_array_t *array_b = make_array(2, 0 , 1);
	
	node_t *node1 = make_node(sum);
	int h1 = 100;
	node1->in1 = make_link(&h1);
	int h2 = 25;
	node1->in2 = make_link(&h2);
	node1->out1 = make_link(malloc(sizeof(int)));

	node_t *node2 = make_node(sum);
	node2->in1 = node1->out1;
	int h3 = 25;
	node2->in2 = make_link(&h3);
	node2->out1 = make_link(malloc(sizeof(int)));

	execute_node(node1);
	execute_node(node2);

	printf("%d\n", * (int*) node2->out1->data);
	
	// node->in2 = link_out1;
	// execute_node(node);
	// print_array(node->out1->data);
}

int main (int acount, char **args) {
	//test_arrays();
	test_nodes();
	
	printf("hello\n");
	return 0;
}

