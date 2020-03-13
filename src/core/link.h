#pragma once

struct node;

struct link {
	void *data;
	struct node *sender;
	struct node *receiver;
};

struct link *make_link (void *data);
// void set_data (link *this, const struct sized_array *data);
