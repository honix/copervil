#pragma once

typedef struct {
	int size;
	int *data;
} sized_array_t;

sized_array_t *allocate_array (int size);

sized_array_t *make_array (int size, int from, int inc);

void print_array (sized_array_t *array) ;

void apply_arrays (
	const int (*func) (int, int),
	const sized_array_t *array_a, 
	const sized_array_t *array_b,
	sized_array_t *array_out
);
