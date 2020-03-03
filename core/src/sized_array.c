#include "sized_array.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

sized_array_t *allocate_array (int size) 
{
	int *data = malloc(sizeof(int) * size); 
	sized_array_t *array = malloc(sizeof(sized_array_t));
	array->size = size;
	array->data = data;
	return array;
}

sized_array_t *make_array (int size, int from, int inc) 
{
	sized_array_t *array = allocate_array(size);
	for (int i = 0; i < size; i++, from += inc) array->data[i] = from;
	return array;
}

void print_array (sized_array_t *array) 
{
	for (int i = 0; i < array->size; i++) printf("%d ", array->data[i]);
	printf("\n");
}

void apply_arrays (
	const int (*func) (int, int),
	const sized_array_t *array_a, 
	const sized_array_t *array_b,
	sized_array_t *array_out
) 
{
	int max_size = MAX(array_a->size, array_b->size);
	// TODO: check that array_out matches this size
	for (int i = 0; i < max_size; i++) {
		array_out->data[i] = func(
			array_a->data[i % array_a->size],
			array_b->data[i % array_b->size]
		);
	}
}
