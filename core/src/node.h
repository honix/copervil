#pragma once

#define NODE_PINS_COUNT 16

enum node_flags {
	CALL_NEXT = 0b00000001
};

struct link;

struct node {
	char *name;
	void (*func) (struct node*);
	struct link *in_pins[NODE_PINS_COUNT];
	struct link *out_pins[NODE_PINS_COUNT];
	unsigned char flags;
};

struct node *make_node (char *name, void (*func) (struct node*));

void direct_call_node (struct node *node);

void connect_nodes (
	struct link *link, 
	struct node *sender,
	unsigned char sender_pin,
	struct node *receiver,
	unsigned char reciever_pin
);
