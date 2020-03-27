#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "geometry.h"
#include "dl_loader.h"

#define NODE_PINS_COUNT 16

struct link;
struct function_note;

// Do this field will be there? Node is abstract thing
#define PIN_SIZE 10
#define PIN_HALF_SIZE (PIN_SIZE / 2)
#define PIN_PADDING 5

#define NODE_WIDTH (PIN_PADDING + (PIN_SIZE + PIN_PADDING) * 16)
#define NODE_HEIGHT 30

enum node_flags
{
	CALL_NEXT = 0b00000001
};

struct node
{
	struct rect rect; // Is node abstract, or rect is ok?

	struct function_note function_note;

	struct link *in_pins[NODE_PINS_COUNT];
	struct link *out_pins[NODE_PINS_COUNT];
	uint16_t in_pins_mask;
	uint16_t out_pins_mask;

	uint8_t flags;
};

struct node **nodes; // TODO: dynamic
unsigned int nodes_pointer;

void init_nodes_subsystem();
struct node *make_node(
	int x, int y,
	struct function_note *function_note);
void direct_call_node(struct node *node);
void connect_nodes(
	struct link *link,
	struct node *sender,
	uint8_t sender_pin,
	struct node *receiver,
	uint8_t reciever_pin);

bool in_pin_is_active(struct node *node, uint8_t pin);
bool out_pin_is_active(struct node *node, uint8_t pin);