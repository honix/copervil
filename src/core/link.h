#pragma once

#include <stdint.h>

#define MAX_RECEIVERS_COUNT 8 // TODO: bad code

struct node;
enum pin_type;

struct link_address
{
	struct node *node;
	uint8_t pin_number;
};

struct link
{
	void *data;
	// struct node *sender;
	// unsigned char sender_pin;
	struct link_address sender_address;
	// struct node *receiver;
	// unsigned char receiver_pin;
	uint8_t receivers_count;
	struct link_address *receivers_addresses;
};

struct link **links; // TODO: consider using dynamic collection
unsigned int links_pointer;

void init_links_subsystem();

struct link *make_link(void *data);
void free_link(struct link *link, enum pin_type owner, struct node *owner_node);

void set_sender(
	struct link *link,
	struct node *sender,
	uint8_t sender_pin_number);

void add_receiver(
	struct link *link,
	struct node *receiver,
	uint8_t reciever_pin_number);