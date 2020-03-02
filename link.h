#pragma once

// #include "node.h"

struct node_t;

typedef struct {
	void *data;
	struct node_t *sender;
	struct node_t *receiver;
} link_t;

link_t *make_link (void *data);
// void set_data (link_t *this, const sized_array_t *data);

