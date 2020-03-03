#pragma once

#include "link.h"

#define NODE_PINS_COUNT 16

enum node_flags {
	CALL_NEXT = 0b00000001
};

typedef struct node_t {
	char *name;
	void (*func) (struct node_t*);
	link_t *in_pins[NODE_PINS_COUNT];
	link_t *out_pins[NODE_PINS_COUNT];
	unsigned char flags;
} node_t;

node_t *make_node (char *name, void (*func) (node_t*));

void execute_node (node_t *node);

void connect_nodes (
	link_t *link, 
	node_t *sender,
	unsigned char sender_pin,
	node_t *receiver,
	unsigned char reciever_pin
);