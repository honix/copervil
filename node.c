#include "node.h"

#include <stdlib.h>

node_t *make_node (void (*func) (node_t*)) 
{
	node_t *node = malloc(sizeof(node_t));
	node->func = func;
	
	return node;
}

void execute_node (node_t *node) 
{
	node->func(node);

	node_t *next_node = node->out_pins[0]->receiver;
	if (next_node != NULL)
		execute_node(next_node);
}

void connect_nodes (
	link_t *link, 
	node_t *sender,
	int sender_pin,
	node_t *receiver,
	int reciever_pin
) 
{
	link->sender = sender;
	link->receiver = receiver;
	if (sender != NULL)
		sender->out_pins[sender_pin] = link;
	if (receiver != NULL)
		receiver->in_pins[reciever_pin] = link;
}