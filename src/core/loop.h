#pragma once

#include <stdint.h>

struct node;

void delayed_call_node_self(struct node *node, double secs);
void delayed_call_node_on_pin(struct node *node, uint8_t pin, double secs);
void loop_run();
