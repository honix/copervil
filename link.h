#pragma once

// #include "sized_array.h"

typedef struct {
	void *data;
} link_t;

link_t *make_link (void *data);
// void set_data (link_t *this, const sized_array_t *data);

