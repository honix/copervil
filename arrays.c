#include <stdio.h>
#include "sized_array.h"
#include "node.h"
#include "link.h"
#include "utils.h"

void test_arrays () {
	sized_array_t 
		*array_a = make_array(10, 0 , 1),
		*array_b = make_array(2, 0, 1),
		*array_c = allocate_array(MAX(array_a->size, array_b->size));
		
	printf("array a\n");
	print_array(array_a);
	printf("array b\n");
	print_array(array_b);
	
	printf("sum\n");
	apply_arrays(sum, array_a, array_b, array_c);
	print_array(array_c);
	
	printf("mul\n");
	apply_arrays(mul, array_a, array_b, array_c);
	print_array(array_c);
}

void test_nodes () {
	sized_array_t *array_a = make_array(10, 0 , 1);
	sized_array_t *array_b = make_array(2, 0 , 1);
	node_t *node = make_node(sum);
	link_t *link_in1 = make_link(array_a);
	link_t *link_in2 = make_link(array_b);
	link_t *link_out1 = make_link(allocate_array(10));
	
	node->in1 = link_in1;
	node->in2 = link_in2;
	node->out1 = link_out1;
	
	execute_node(node);
	print_array(node->out1->data);
	
	node->in2 = link_out1;
	execute_node(node);
	print_array(node->out1->data);
}

int main (int acount, char **args) {
	//test_arrays();
	test_nodes();
	
	printf("hello\n");
	return 0;
}

