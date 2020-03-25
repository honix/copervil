#include "geometry.h"

#include <math.h>

struct vector2i vector_add(struct vector2i a, struct vector2i b)
{
    struct vector2i result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

int distance(struct vector2i from, struct vector2i to)
{
    int dx = from.x - to.x;
    int dy = from.y - to.y;
    return sqrtf(dx*dx + dy*dy);
}

bool is_point_in_rect(struct vector2i point, struct rect rect)
{
    struct vector2i top_left = rect.pos;
    struct vector2i bottom_right = vector_add(rect.pos, rect.size);
    return point.x > top_left.x && point.y > top_left.y
        && point.x < bottom_right.x && point.y < bottom_right.y;
}