#pragma once

struct sized_array
{
	int size;
	int *data;
};

struct sized_array *allocate_array(int size);
struct sized_array *make_array(int size, int from, int inc);
void print_array(struct sized_array *array);
void apply_arrays(
	const int (*func)(int, int),
	const struct sized_array *array_a,
	const struct sized_array *array_b,
	struct sized_array *array_out);
