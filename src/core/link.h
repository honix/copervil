#pragma once

struct node;
enum pin_type;

struct link
{
	void *data;
	struct node *sender;
	unsigned char sender_pin;
	struct node *receiver;
	unsigned char receiver_pin;
};

struct link **links; // TODO: consider using dynamic collection
unsigned int links_pointer;

void init_links_subsystem();

struct link *make_link(void *data);
void free_link(struct link *link, enum pin_type owner);