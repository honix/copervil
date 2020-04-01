#include "node.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "link.h"
#include "dl_loader.h" // funtion_note

#define NODES_MAX_COUNT 16 // TODO: this is bad temp code. rewrite

void init_nodes_subsystem()
{
	nodes = malloc(sizeof(struct node *) * NODES_MAX_COUNT);
	nodes_pointer = 0;
}

void init_pins(struct node *node, uint8_t in_pins, uint8_t out_pins)
{
	node->in_pins = (struct pin_array){
		.array_size = in_pins,
		.pins = calloc(in_pins, sizeof(struct pin))};
	node->out_pins = (struct pin_array){
		.array_size = out_pins,
		.pins = calloc(out_pins, sizeof(struct pin))};
}

void init_pin_link(struct pin *pin)
{
	// TODO: consider default pin values
	// TODO: alloc link for this type from type->byte table
	pin->connected_link = make_link(calloc(1, 128));
}

void reg_pin(
	struct node *node,
	enum pin_type pin_type, uint8_t pin,
	char *name, char *type)
{
	struct pin *p = get_pin(node, pin_type, pin);
	p->name = name;
	// TODO: match type table and give unique type number
	p->type_id = 0;

	init_pin_link(p);
}

void drop_link(
	struct node *node,
	enum pin_type pin_type, uint8_t pin)
{
	struct pin *p = get_pin(node, pin_type, pin);
	p->connected_link = NULL;
	init_pin_link(p);
}

static void init_node(struct node *node)
{
	if (node->function_note.init_func == NULL)
		return;
	node->function_note.init_func(node);
}

struct pin *get_pin(struct node *node, enum pin_type pin_type, uint8_t pin)
{
	switch (pin_type)
	{
	case PIN_INPUT:
		return node->in_pins.pins + pin;
		break;

	case PIN_OUTPUT:
		return node->out_pins.pins + pin;
		break;

	default:
		printf("bad pin_type\n");
		break;
	}

	return NULL;
}

struct link *get_link_on_pin(
	struct node *node, enum pin_type pin_type, uint8_t pin)
{
	return (*get_pin(node, pin_type, pin)).connected_link;
}

void direct_call_node(struct node *node)
{
	// printf("// direct_call_node %s\n", node->name);

	node->function_note.main_func(node);

	// Do we need this CALL_NEXT feature?
	// if (node->flags & CALL_NEXT)
	// {
	// 	try_call_next(node);
	// }
}

void try_direct_call_next(struct node *node)
{
	if (node->out_pins.array_size == 0)
		return;
	struct node *next_node = get_link_on_pin(node, PIN_OUTPUT, 0)->receiver;
	if (next_node == NULL)
		return;
	direct_call_node(next_node);
}

void deinit_node(struct node *node)
{
	if (node->function_note.deinit_func == NULL)
		return;
	node->function_note.deinit_func(node);
	// TODO: free memory
}

struct node *make_node(
	int x, int y,
	struct function_note *function_note)
{
	struct node *node = malloc(sizeof(struct node));

	node->rect.pos.x = x;
	node->rect.pos.y = y;
	node->rect.size.x = NODE_WIDTH;
	node->rect.size.y = NODE_HEIGHT;
	node->function_note = *function_note;

	// for (int i = 0; i < NODE_PINS_COUNT; i++)
	// {
	// 	node->in_pins[i] = NULL;
	// 	node->out_pins[i] = NULL;
	// }

	// node->in_pins_mask = 0b0000000000000000;
	// node->out_pins_mask = 0b0000000000000000;

	// node->flags = 0b00000000;
	//node->flags = 0 | CALL_NEXT;

	nodes[nodes_pointer] = node;
	nodes_pointer++;

	if (nodes_pointer >= NODES_MAX_COUNT)
		printf("Oops! NODES_MAX_COUNT exceeded!\n");

	init_node(node);

	return node;
}

void free_node(struct node *node)
{
	for (int i = 0; i < node->in_pins.array_size; i++)
		free_link(get_pin(node, PIN_INPUT, i)->connected_link, PIN_INPUT);
	for (int i = 0; i < node->out_pins.array_size; i++)
		free_link(get_pin(node, PIN_OUTPUT, i)->connected_link, PIN_OUTPUT);
	free(node->in_pins.pins);
	free(node->out_pins.pins);

	unsigned int shift = 0;
	for (int i = 0; i < nodes_pointer; i++)
	{
		if (nodes[i] == node)
		{
			shift = i;
			break;
		}
	}
	memmove(
		nodes + shift,
		nodes + shift + 1,
		(nodes_pointer - shift - 1) * sizeof(struct node *));

	nodes_pointer--;

	free(node);
}

void connect_nodes(
	struct link *link,
	struct node *sender,
	uint8_t sender_pin,
	struct node *receiver,
	uint8_t reciever_pin)
{
	// TODO: free old links
	link->sender = sender;
	link->sender_pin = sender_pin;
	link->receiver = receiver;
	link->receiver_pin = reciever_pin;
	// TODO: check types of pins
	// TODO: auto make link using sizeof type
	if (sender != NULL)
		(*get_pin(sender, PIN_OUTPUT, sender_pin)).connected_link = link;
	if (receiver != NULL)
		(*get_pin(receiver, PIN_INPUT, reciever_pin)).connected_link = link;
}

bool in_pin_is_active(struct node *node, uint8_t pin)
{
	// return node->in_pins_mask & 1 << pin;
	return true;
}

bool out_pin_is_active(struct node *node, uint8_t pin)
{
	// return node->out_pins_mask & 1 << pin;
	return true;
}