#include "sized_array.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct sized_array *allocate_array(int size)
{
	int *data = malloc(sizeof(int) * size);
	struct sized_array *array = malloc(sizeof(struct sized_array));
	array->size = size;
	array->data = data;
	return array;
}

struct sized_array *make_array(int size, int from, int inc)
{
	struct sized_array *array = allocate_array(size);
	for (int i = 0; i < size; i++, from += inc)
		array->data[i] = from;
	return array;
}

void print_array(struct sized_array *array)
{
	for (int i = 0; i < array->size; i++)
		printf("%d ", array->data[i]);
	printf("\n");
}

void apply_arrays(
	const int (*func)(int, int),
	const struct sized_array *array_a,
	const struct sized_array *array_b,
	struct sized_array *array_out)
{
	int max_size = MAX(array_a->size, array_b->size);
	// TODO: check that array_out matches this size
	for (int i = 0; i < max_size; i++)
	{
		array_out->data[i] = func(
			array_a->data[i % array_a->size],
			array_b->data[i % array_b->size]);
	}
}
