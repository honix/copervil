#pragma once

#include <stdbool.h>

struct vector2i
{
	int x;
	int y;
};

#define VECTOR_ONE \
	(struct vector2i) { .x = 1, .y = 1 }
	
struct vector2i vector_add(struct vector2i a, struct vector2i b);
struct vector2i vector_sub(struct vector2i a, struct vector2i b);
struct vector2i vector_mul(struct vector2i a, struct vector2i b);
struct vector2i vector_scale(struct vector2i a, int amount);
int distance(struct vector2i from, struct vector2i to);

struct rect
{
	struct vector2i pos;
	struct vector2i size;
};

bool is_point_in_rect(struct vector2i point, struct rect rect);
struct rect expand_rect(struct rect rect, int amount);