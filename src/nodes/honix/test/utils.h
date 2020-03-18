#pragma once

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct node;

// (int, int) -> (int)
void sum(struct node *node);
// (int) -> ()
void print_int(struct node *node);
// () -> (trigger, int)
void do_times(struct node *node);
// () -> (int/trigger)
void do_times_inderect(struct node *node);

void register_library(void (*reg)(char *, void (*)(struct node *)));