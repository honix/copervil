#include "node.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "link.h"
#include "dl_loader.h" // funtion_note
#include "type_bank.h"

#define NODES_MAX_COUNT 128 // TODO: this is bad temp code. rewrite

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

static void init_pin_link(struct pin *pin)
{
	// TODO: consider default pin values
	// TODO: alloc link for this type from type->byte table
	pin->connected_link = make_link(
		calloc(1, get_type_note_by_id(pin->type_id)->size));
}

void reg_pin(
	struct node *node,
	enum pin_type pin_type, uint8_t pin,
	char *name, char *type_name, size_t type_size)
{
	struct pin *p = get_pin(node, pin_type, pin);
	p->name = name;

	struct type_note *type_note = reg_type(type_name, type_size);
	p->type_id = type_note->id;

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
		if (node->in_pins.array_size > pin)
			return node->in_pins.pins + pin;
		else
			goto error;

	case PIN_OUTPUT:
		if (node->out_pins.array_size > pin)
			return node->out_pins.pins + pin;
		else
			goto error;

	default:
		goto error;
	}

error:
	printf("error: bad pin\n");
	return NULL;
}

struct link *get_link_on_pin(
	struct node *node, enum pin_type pin_type, uint8_t pin)
{
	return (*get_pin(node, pin_type, pin)).connected_link;
}

void direct_call_node_self(struct node *node)
{
	// printf("// direct_call_node %s\n", node->name);

	node->function_note.main_func(node);

	// Do we need this CALL_NEXT feature?
	// if (node->flags & CALL_NEXT)
	// {
	// 	call_next(node);
	// }
}

void direct_call_node_on_pin(struct node *node, uint8_t pin)
{
	struct node *next_node = get_link_on_pin(node, PIN_OUTPUT, pin)->receiver;

	if (next_node == NULL || next_node->only_self_trigger)
		return;

	direct_call_node_self(next_node);
}

void deinit_node(struct node *node)
{
	if (node->function_note.deinit_func == NULL)
		return;
	node->function_note.deinit_func(node);
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
	node->only_self_trigger = false;

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
	deinit_node(node);

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
	struct node *sender,
	uint8_t sender_pin_number,
	struct node *receiver,
	uint8_t reciever_pin_number)
{
	struct pin *sender_pin;
	struct pin *receiver_pin;
	size_t size = -1;

	if (sender != NULL)
	{
		sender_pin = get_pin(sender, PIN_OUTPUT, sender_pin_number);
		size = get_type_note_by_id(sender_pin->type_id)->size;
	}
	if (receiver != NULL)
	{
		receiver_pin = get_pin(receiver, PIN_INPUT, reciever_pin_number);
		if (size == -1)
			size = get_type_note_by_id(receiver_pin->type_id)->size;
	}

	if (sender_pin != NULL && receiver_pin != NULL &&
		sender_pin->type_id != receiver_pin->type_id)
	{
		printf("Error: types are not same\n");
		return;
	}

	// TODO: free old links, and maybe save datas they hold
	struct link *link = make_link(calloc(1, size));
	link->sender = sender;
	link->sender_pin = sender_pin_number;
	link->receiver = receiver;
	link->receiver_pin = reciever_pin_number;

	if (sender_pin != NULL)
		sender_pin->connected_link = link;
	if (receiver_pin != NULL)
		receiver_pin->connected_link = link;
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