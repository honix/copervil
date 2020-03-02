#include "node.h"
// #include "sized_array.h"

#include <stdlib.h>

node_t *make_node (void (*func) (node_t*)) {
	node_t *node = malloc(sizeof(node_t));
	node->func = func;
	
	return node;
}

void execute_node (node_t *node) {
	// apply_arrays(
	// 	node->func,
	// 	node->in1->data,
	// 	node->in2->data,
	// 	node->out1->data
	// );	
	node->func(node);
}
