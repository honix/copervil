#pragma once

#include "link.h"

typedef struct node_t {
	void (*func) (struct node_t*);
	link_t *in1; // do i really need link abstraction?
	link_t *in2; // we can store void* here
	link_t *out1;
	link_t *out2;
} node_t;

node_t *make_node (void (*func) (node_t*));
void execute_node (node_t *node);
