#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "geometry.h"
#include "dl_loader.h"

#define NODE_PINS_COUNT 8

struct link;
struct function_note;

// Do this field will be there? Node is abstract thing
#define PIN_SIZE 10
#define PIN_HALF_SIZE (PIN_SIZE / 2)
#define PIN_PADDING 10

#define NODE_WIDTH (PIN_PADDING + (PIN_SIZE + PIN_PADDING) * NODE_PINS_COUNT)
#define NODE_HEIGHT 30

// enum node_flags
// {
// 	CALL_NEXT = 0b00000001
// };

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

	struct function_note function_note;

	struct pin_array in_pins;
	struct pin_array out_pins;

	// uint8_t flags;
};

struct node **nodes; // TODO: dynamic
unsigned int nodes_pointer;

void init_nodes_subsystem();

void init_pins(struct node *node, uint8_t in_pins, uint8_t out_pins);
void reg_pin(
	struct node *node, 
	enum pin_type pin_type, uint8_t pin, 
	char *name, char *type);

struct pin *get_pin(struct node *node, enum pin_type pin_type, uint8_t pin);
struct link *get_link_on_pin(
	struct node *node, enum pin_type pin_type, uint8_t pin);

struct node *make_node(
	int x, int y,
	struct function_note *function_note);
void direct_call_node(struct node *node);
void try_direct_call_next(struct node *node);
void connect_nodes(
	struct link *link,
	struct node *sender,
	uint8_t sender_pin,
	struct node *receiver,
	uint8_t reciever_pin);

bool in_pin_is_active(struct node *node, uint8_t pin);
bool out_pin_is_active(struct node *node, uint8_t pin);