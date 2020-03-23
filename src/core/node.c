#include "node.h"

#include <stdlib.h>
#include <stdio.h>

#include "link.h"
#include "dl_loader.h" // funtion_note

void init_nodes_subsystem()
{
	nodes = malloc(sizeof(struct node *) * 16);
	nodes_pointer = 0;
}

void init_node(struct node *node)
{
	if (node->init_func == NULL) return; 
	node->init_func(node);
}

void direct_call_node(struct node *node)
{
	// printf("// direct_call_node %s\n", node->name);

	node->main_func(node);

	// Do we need this CALL_NEXT feature?
	// if (node->flags & CALL_NEXT && node->out_pins[0] != NULL)
	// {
	// 	struct node *next_node = node->out_pins[0]->receiver;
	// 	if (next_node != NULL)
	// 		direct_call_node(next_node);
	// }
}

void deinit_node(struct node *node)
{
	if (node->deinit_func == NULL) return; 
	node->deinit_func(node);
	// TODO: free memory
}

struct node *make_node(
	char *name, 
	int x, int y, 
	struct function_note *function_note)
{
	struct node *node = malloc(sizeof(struct node));

	node->name = name;
	node->x = x;
	node->y = y;
	node->init_func = function_note->init_func;
	node->main_func = function_note->main_func;
	node->deinit_func = function_note->deinit_func;

	for (int i = 0; i < NODE_PINS_COUNT; i++)
	{
		node->in_pins[i] = NULL;
		node->out_pins[i] = NULL;
	}

	node->in_pins_mask  = 0b0000000000000000;
	node->out_pins_mask = 0b0000000000000000;

	node->flags = 0b00000000;
	//node->flags = 0 | CALL_NEXT;

	nodes[nodes_pointer] = node;
	nodes_pointer++;

	init_node(node);

	return node;
}

void connect_nodes(
	struct link *link,
	struct node *sender,
	uint8_t sender_pin,
	struct node *receiver,
	uint8_t reciever_pin)
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

bool in_pin_is_active(struct node *node, uint8_t pin) 
{
    return node->in_pins_mask & 1 << pin;
}

bool out_pin_is_active(struct node *node, uint8_t pin) 
{
    return node->out_pins_mask & 1 << pin;
}