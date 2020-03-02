#pragma once

#include "link.h"

typedef struct node_t {
	void (*func) (struct node_t*);
	link_t *in_pins[16];
	link_t *out_pins[16];
} node_t;

node_t *make_node (void (*func) (node_t*));

void execute_node (node_t *node);

void connect_nodes (
	link_t *link, 
	node_t *sender,
	int sender_pin,
	node_t *receiver,
	int reciever_pin
);