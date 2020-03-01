#pragma once

#include "link.h"

typedef struct node_t {
	int (*func) (int, int);
	link_t *in1;
	link_t *in2;
	link_t *out1;
} node_t;

node_t *make_node (const int (*func) (int, int));
void execute_node (node_t *node);
