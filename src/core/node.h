#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "geometry.h"
#include "dl_loader.h"

typedef struct
{
} trigger;

struct link;
struct function_note;
struct thread_note;

// Do this field will be there? Node is abstract thing
#define PIN_SIZE 10
#define PIN_HALF_SIZE (PIN_SIZE / 2)
#define PIN_PADDING 10

#define NODE_WIDTH (PIN_PADDING + (PIN_SIZE + PIN_PADDING) * 8)
#define NODE_HEIGHT 30

enum pin_type
{
	PIN_NONE,
	PIN_INPUT,
	PIN_OUTPUT
};

struct pin
{
	char *name;
	unsigned int type_id;
	// bool hidden;

	struct link *connected_link;
};

struct pin_array
{
	uint8_t array_size;
	struct pin *pins;
};

struct node
{
	struct rect rect; // Is node abstract, or rect is ok?

	struct function_note function_note; // TODO: maybe pointer?

	struct pin_array in_pins;
	struct pin_array out_pins;

	void *inner_state;

	bool in_out_trigger;
	// bool only_self_trigger;
	bool auto_call_next;
	// uint8_t flags;

	struct thread_note *thread_note;
};

struct node **nodes;

void init_nodes_subsystem();

void init_pins(
	struct node *node, 
	bool in_out_trigger, 
	uint8_t in_pins, 
	uint8_t out_pins);

#define REG_PIN(node, pin_type, pin, name, type) \
	(reg_pin(node, pin_type, pin, name, #type, sizeof(type)))

#define GET_PIN(node, pin_type, pin, type) \
	(*(type *)get_link_on_pin(node, pin_type, pin)->data)

void reg_pin(
	struct node *node,
	enum pin_type pin_type,
	uint8_t pin_number,
	char *name,
	char *type,
	size_t type_size);

void drop_link(
	struct node *node,
	enum pin_type pin_type, uint8_t pin_number);

struct pin *get_pin(
	struct node *node,
	enum pin_type pin_type,
	uint8_t pin_number);
struct link *get_link_on_pin(
	struct node *node,
	enum pin_type pin_type,
	uint8_t pin_number);

struct node *make_node(
	int x, int y,
	struct function_note *function_note,
	struct thread_note *thread_note);
void free_node(struct node *node);
void direct_call_node_self(struct node *node);
void direct_call_node_on_pin(struct node *node, uint8_t pin_number);
void connect_nodes(
	struct node *sender,
	uint8_t sender_pin_number,
	struct node *receiver,
	uint8_t reciever_pin_number);
