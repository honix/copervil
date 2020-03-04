#pragma once

struct sized_array_t {
	int size;
	int *data;
};

struct sized_array_t *allocate_array (int size);

struct sized_array_t *make_array (int size, int from, int inc);

void print_array (struct sized_array_t *array) ;

void apply_arrays (
	const int (*func) (int, int),
	const struct sized_array_t *array_a, 
	const struct sized_array_t *array_b,
	struct sized_array_t *array_out
);
