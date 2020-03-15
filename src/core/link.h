#pragma once

struct node;

struct link {
	void *data;
	struct node *sender;
	unsigned char sender_pin; 
	struct node *receiver;
	unsigned char receiver_pin;
};

struct link *make_link (void *data);
// void set_data (link *this, const struct sized_array *data);
