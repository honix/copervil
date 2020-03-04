#include "node.h"

#include <stdlib.h>
#include <stdio.h>

#include "link.h"
 
struct node *make_node (char *name, void (*func) (struct node*)) 
{
	struct node *node = malloc(sizeof(struct node));

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

void direct_call_node (struct node *node) 
{
	printf("// direct_call_node %s\n", node->name);

	node->func(node);

	if (node->flags & CALL_NEXT && node->out_pins[0] != NULL) {
		struct node *next_node = node->out_pins[0]->receiver;
		if (next_node != NULL)
			direct_call_node(next_node);
	}
}

void connect_nodes (
	struct link *link, 
	struct node *sender,
	unsigned char sender_pin,
	struct node *receiver,
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
