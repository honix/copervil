#pragma once

#include "node.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

// (int, int) -> int
void sum (node_t *node);
