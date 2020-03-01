#pragma once

#include "sized_array.h"

typedef struct {
	sized_array_t *data;
} link_t;

link_t *make_link (sized_array_t *data);
// void set_data (link_t *this, const sized_array_t *data);

