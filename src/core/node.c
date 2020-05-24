#include "node.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sx/allocator.h"
#include "sx/array.h"
#include "sx/threads.h"
#include "sx/os.h"

#include "link.h"
#include "dl_loader.h" // function_note
#include "type_bank.h"
#include "threads.h"
#include "loop.h"

void init_nodes_subsystem()
{
	// nodes = NULL;
}

void init_pins(
	struct node *node,
	bool in_out_trigger,
	uint8_t in_pins,
	uint8_t out_pins)
{
	node->in_out_trigger = in_out_trigger;
	if (in_out_trigger)
	{
		in_pins += 1;
		out_pins += 1;
	}

	node->in_pins = (struct pin_array){
		.array_size = in_pins,
		.pins =
			node->in_pins.pins == NULL
				? calloc(in_pins, sizeof(struct pin))
				: realloc(node->in_pins.pins, in_pins * sizeof(struct pin))};
	node->out_pins = (struct pin_array){
		.array_size = out_pins,
		.pins =
			node->out_pins.pins == NULL
				? calloc(out_pins, sizeof(struct pin))
				: realloc(node->out_pins.pins, out_pins * sizeof(struct pin))};

	if (in_out_trigger)
	{
		REG_PIN(node, PIN_INPUT, 0, "trigger", trigger);
		REG_PIN(node, PIN_OUTPUT, 0, "trigger", trigger);
	}
}

static void init_pin_link(
	struct node *node,
	enum pin_type pin_type,
	struct pin *pin,
	uint8_t pin_number)
{
	// TODO: consider default pin values
	pin->connected_link = make_link(
		calloc(1, get_type_note_by_id(pin->type_id)->size));

	// switch (pin_type)
	// {
	// case PIN_OUTPUT:
	// 	set_sender(pin->connected_link, node, pin_number);
	// 	break;

	// case PIN_INPUT:
	// 	add_receiver(pin->connected_link, node, pin_number);
	// 	break;
	// }

	if (pin_type == PIN_OUTPUT)
		set_sender(pin->connected_link, node, pin_number);
}

void reg_pin(
	struct node *node,
	enum pin_type pin_type,
	uint8_t pin_number,
	char *name,
	char *type_name,
	size_t type_size)
{
	struct pin *pin = get_pin(node, pin_type, pin_number);
	pin->name = name;

	struct type_note *type_note = reg_type(type_name, type_size);
	pin->type_id = type_note->id;

	init_pin_link(node, pin_type, pin, pin_number);
}

void drop_link(
	struct node *node,
	enum pin_type pin_type,
	uint8_t pin_number)
{
	struct pin *pin = get_pin(node, pin_type, pin_number);
	// pin->connected_link = NULL;
	init_pin_link(node, pin_type, pin, pin_number);
}

struct pin *get_pin(
	struct node *node,
	enum pin_type pin_type,
	uint8_t pin_number)
{
	switch (pin_type)
	{
	case PIN_INPUT:
		if (node->in_pins.array_size > pin_number)
			return node->in_pins.pins + pin_number;
		else
			goto error;

	case PIN_OUTPUT:
		if (node->out_pins.array_size > pin_number)
			return node->out_pins.pins + pin_number;
		else
			goto error;

	default:
		goto error;
	}

error:
	printf("error: bad pin!\n");
	return NULL;
}

struct link *get_link_on_pin(
	struct node *node,
	enum pin_type pin_type,
	uint8_t pin_number)
{
	struct pin *pin = get_pin(node, pin_type, pin_number);
	if (pin == NULL)
		return NULL;
	return pin->connected_link;
}

void direct_call_node_self(struct node *node)
{
	// if (node->function_note.name[0] != 'p')
	// 	printf("direct_call_node %s\n", node->function_note.name);
	// if (node->thread_note->node_to_run == NULL)
	// {
	if (node->thread_note->node_to_run == NULL)
	{
		send_func_to_thread(direct_call_node_self, node);
	}
	else
	{
		node->function_note.main_func(node);

		// if (node->function_note.name[0] != 'p')
		// 	printf("send_func_to_thread(node->function_note.main_func, node)\n");
		// sx_signal_wait(node->thread_note->signal_done, -1);
		// if (node->function_note.name[0] != 'p')
		// 	printf("sx_signal_wait(node->thread_note->signal_done, -1) done\n");
		// return;
		// }

		if (node->in_out_trigger && node->auto_call_next)
			direct_call_node_on_pin(node, 0);
	}
}

void direct_call_node_on_pin(struct node *node, uint8_t pin_number)
{
	struct pin *pin = get_pin(node, PIN_OUTPUT, pin_number);
	if (pin == NULL)
		return;

	struct link *link = pin->connected_link;
	for (uint8_t i = 0; i < link->receivers_count; i++)
	{
		struct node *next_node = link->receivers_addresses[i].node;
		// if (next_node == NULL || next_node->only_self_trigger)
		if (next_node == NULL)
			return;

		direct_call_node_self(next_node);
	}
}

void deinit_node(struct node *node)
{
	if (node->function_note.deinit_func == NULL)
		return;
	node->function_note.deinit_func(node);
}

struct node *make_node(
	int x, int y,
	struct function_note *function_note,
	struct thread_note *thread_note)
{
	printf("Make node %s\n", function_note->name);

	struct node *node = malloc(sizeof(struct node));

	node->rect.pos.x = x;
	node->rect.pos.y = y;
	node->rect.size.x = NODE_WIDTH;
	node->rect.size.y = NODE_HEIGHT;
	node->function_note = *function_note;
	node->in_out_trigger = true;
	// node->only_self_trigger = false;
	node->auto_call_next = true;
	node->thread_note =
		thread_note == NULL ? default_thread_note : thread_note;

	node->in_pins = (struct pin_array){.array_size = 0, .pins = NULL};
	node->out_pins = (struct pin_array){.array_size = 0, .pins = NULL};

	sx_array_push(sx_alloc_malloc(), nodes, node);

	if (node->function_note.init_func != NULL)
	{
		send_func_to_thread(node->function_note.init_func, node);
		// printf("send_func_to_thread(node->function_note.init_func, node)\n");
		// sx_signal_wait(node->thread_note->signal_done, -1);
		// printf("sx_signal_wait(node->thread_note->signal_done, -1) done\n");
	}
	else
	{
		init_pins(node, true, 0, 0);
	}

	return node;
}

void free_node(struct node *node)
{
	printf("Remove node %s @ 0x%lx\n",
		   node->function_note.name,
		   (long unsigned int)node);

	deinit_node(node);

	remove_node_from_delayed_list(node);

	for (int i = 0; i < node->in_pins.array_size; i++)
		free_link(
			get_pin(node, PIN_INPUT, i)->connected_link, PIN_INPUT, node);
	for (int i = 0; i < node->out_pins.array_size; i++)
		free_link(
			get_pin(node, PIN_OUTPUT, i)->connected_link, PIN_OUTPUT, node);

	free(node->in_pins.pins);
	free(node->out_pins.pins);

	unsigned int shift = 0;
	for (int i = 0; i < sx_array_count(nodes); i++)
	{
		if (nodes[i] == node)
		{
			shift = i;
			break;
		}
	}

	nodes[shift] = sx_array_last(nodes);
	sx_array_pop_last(nodes);

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
		printf("Error: connecting %s and %s nodes: types are not same: %s != %s\n",
			   sender->function_note.name, receiver->function_note.name, get_type_note_by_id(sender_pin->type_id)->name,
			   get_type_note_by_id(receiver_pin->type_id)->name);
		return;
	}

	// TODO: free old links, and maybe save datas they hold
	// do not allow multiple in's ? (btw this can be usefull whith triggers)

	struct link *receiver_link = receiver_pin->connected_link;
	if (receiver_link != NULL)
		free_link(receiver_link, PIN_INPUT, receiver);

	// struct link *link = make_link(calloc(1, size));
	struct link *sender_link = sender_pin->connected_link;

	// link->sender_address.node = sender;
	// link->sender_address.pin = sender_pin_number;
	add_receiver(sender_link, receiver, reciever_pin_number);

	// if (sender_pin != NULL)
	// sender_pin->connected_link = link;
	// if (receiver_pin != NULL)
	receiver_pin->connected_link = sender_link;
}

void set_thread_note(struct node *node, uint8_t thread_num)
{
	if (thread_num >= sx_array_count(thread_notes)) return;
	node->thread_note = thread_notes[thread_num];
}