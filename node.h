#pragma once

#include "link.h"

#define PINS_NUM 16

typedef struct node_t {
	void (*func) (struct node_t*);
	link_t *in_pins[PINS_NUM];
	link_t *out_pins[PINS_NUM];
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