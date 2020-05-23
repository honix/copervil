#pragma once

#include <stdint.h>

struct node;

void delayed_call_node_self(struct node *node, double secs);
void delayed_call_node_on_pin(struct node *node, uint8_t pin, double secs);
void remove_node_from_delayed_list(struct node *node);
void init_loop_subsystem();
void loop_run();
double current_time_secs();