#pragma once

struct node_t;

struct link_t {
	void *data;
	struct node_t *sender;
	struct node_t *receiver;
};

struct link_t *make_link (void *data);
// void set_data (link_t *this, const struct sized_array_t *data);
