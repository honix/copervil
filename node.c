#include "node.h"

#include <stdlib.h>
#include <stdio.h>

node_t *make_node (char *name, void (*func) (node_t*)) 
{
	node_t *node = malloc(sizeof(node_t));

	node->name = name;
	node->func = func;

	for (int i = 0; i < NODE_PINS_COUNT; i++) {
		node->in_pins[i] = NULL;
		node->out_pins[i] = NULL;
	}
	
	node->flags = 0;
	//node->flags = 0 | CALL_NEXT;

	return node;
}

void execute_node (node_t *node) 
{
	printf("// execute_node %s\n", node->name);

	node->func(node);

	if (node->flags & CALL_NEXT && node->out_pins[0] != NULL) {
		node_t *next_node = node->out_pins[0]->receiver;
		if (next_node != NULL)
			execute_node(next_node);
	}
}

void connect_nodes (
	link_t *link, 
	node_t *sender,
	unsigned char sender_pin,
	node_t *receiver,
	unsigned char reciever_pin
) 
{
	link->sender = sender;
	link->receiver = receiver;
	if (sender != NULL)
		sender->out_pins[sender_pin] = link;
	if (receiver != NULL)
		receiver->in_pins[reciever_pin] = link;
}