#include "node.h"

#include <stdlib.h>
#include <stdio.h>

#include "link.h"
 
void init_nodes()
{
	nodes = malloc(sizeof(struct node*) * 16);
	nodes_pointer = 0;
}

struct node *make_node (char *name, int x, int y, void (*func) (struct node*)) 
{
	struct node *node = malloc(sizeof(struct node));

	node->name = name;
	node->x = x;
	node->y = y;
	node->func = func;

	for (int i = 0; i < NODE_PINS_COUNT; i++) {
		node->in_pins[i] = NULL;
		node->out_pins[i] = NULL;
	}
	
	node->flags = 0;
	//node->flags = 0 | CALL_NEXT;

	nodes[nodes_pointer] = node;
	nodes_pointer++;

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
	link->sender_pin = sender_pin;
	link->receiver = receiver;
	link->receiver_pin = reciever_pin;
	if (sender != NULL)
		sender->out_pins[sender_pin] = link;
	if (receiver != NULL)
		receiver->in_pins[reciever_pin] = link;
}
