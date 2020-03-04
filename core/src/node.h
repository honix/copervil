#pragma once

#define NODE_PINS_COUNT 16

enum node_flags {
	CALL_NEXT = 0b00000001
};

struct link_t;

struct node_t {
	char *name;
	void (*func) (struct node_t*);
	struct link_t *in_pins[NODE_PINS_COUNT];
	struct link_t *out_pins[NODE_PINS_COUNT];
	unsigned char flags;
};

struct node_t *make_node (char *name, void (*func) (struct node_t*));

void direct_call_node (struct node_t *node);

void connect_nodes (
	struct link_t *link, 
	struct node_t *sender,
	unsigned char sender_pin,
	struct node_t *receiver,
	unsigned char reciever_pin
);
