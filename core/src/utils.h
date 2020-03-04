#pragma once

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))

struct node_t;

// (int, int) -> (int)
void sum (struct node_t *node);
// (int) -> ()
void print_int (struct node_t *node);
// () -> (trigger, int)
void do_times (struct node_t *node);
// () -> (int/trigger)
void do_times_inderect (struct node_t *node);
