#pragma once

struct node;

struct link
{
	void *data;
	struct node *sender;
	unsigned char sender_pin;
	struct node *receiver;
	unsigned char receiver_pin;
};

struct link **links; // TODO: dynamic
unsigned int links_pointer;

void init_links_subsystem();

struct link *make_link(void *data);
void free_link(struct link *link);