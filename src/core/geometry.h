#pragma once

#include <stdbool.h>

struct vector2i
{
    int x;
    int y;
};

struct rect
{
    struct vector2i pos;
    struct vector2i size;
};

struct vector2i vector_add(struct vector2i a, struct vector2i b);
int distance(struct vector2i from, struct vector2i to);

bool is_point_in_rect(struct vector2i point, struct rect rect);