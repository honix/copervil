#include "node.h"

#include <stdlib.h>
#include <stdio.h>

#include "link.h"
 
struct node_t *make_node (char *name, void (*func) (struct node_t*)) 
{
	struct node_t *node = malloc(sizeof(struct node_t));

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

void direct_call_node (struct node_t *node) 
{
	printf("// direct_call_node %s\n", node->name);

	node->func(node);

	if (node->flags & CALL_NEXT && node->out_pins[0] != NULL) {
		struct node_t *next_node = node->out_pins[0]->receiver;
		if (next_node != NULL)
			direct_call_node(next_node);
	}
}

void connect_nodes (
	struct link_t *link, 
	struct node_t *sender,
	unsigned char sender_pin,
	struct node_t *receiver,
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
