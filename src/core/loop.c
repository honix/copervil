#include "loop.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "sx/timer.h"
#include "sx/os.h"

#include "node.h"
#include "link.h"
#include "list.h"
#include "utils.h"

// this list will be always sorted by > call_time
struct list delayed_node_list;

uint64_t start_time;

struct delayed_node
{
	struct node *node;
	double call_time;
};

struct delayed_node *make_delayed_node(struct node *node, double call_time)
{
	struct delayed_node *delayed_node = malloc(sizeof(struct delayed_node));
	delayed_node->node = node;
	delayed_node->call_time = call_time;
	return delayed_node;
}

double current_time_secs()
{
	return sx_tm_sec(sx_tm_since(start_time));
}

int ord(struct list_cell *a, struct list_cell *b)
{
	struct delayed_node *delayed_node_a = a->data;
	struct delayed_node *delayed_node_b = b->data;
	return delayed_node_a->call_time < delayed_node_b->call_time ? 1 : 0;
}

void delayed_call_node_self(struct node *node, double secs)
{
	struct list_cell *cell = make_list_cell(
		make_delayed_node(node, current_time_secs() + secs));

	insert_list_cell_ordered(&delayed_node_list, cell, ord);
}

void delayed_call_node_on_pin(struct node *node, uint8_t pin, double secs)
{
	struct link *link = get_link_on_pin(node, PIN_OUTPUT, pin);
	for (uint8_t i = 0; i < link->receivers_count; i++)
	{
		struct node *target = link->receivers_addresses[i].node;

		// if (target == NULL || target->only_self_trigger)
		if (target == NULL)
			return;

		delayed_call_node_self(target, secs);
	}
}

void loop_step()
{
	// printf("// loop step %f\n", current_time_secs());

	double delay;

	if (delayed_node_list.first_cell != NULL)
	{
		struct delayed_node *delayed_node =
			delayed_node_list.first_cell->data;
		delay = delayed_node->call_time - current_time_secs();
	}
	else
	{
		delay = 1.0 / 30;
	}

	delay = MAX(0, delay);
	// printf("// sleep for %f\n", delay);

	if (delay != 0)
	{
		sx_os_sleep(delay / 1000);
	}

	if (delayed_node_list.first_cell != NULL)
	{
		struct delayed_node *delayed_node =
			delayed_node_list.first_cell->data;
		direct_call_node_self(delayed_node->node);
		// TODO: drop and free first cell
		// drop first cell
		delayed_node_list.first_cell =
			delayed_node_list.first_cell->next;
	}
}

void init_loop_subsystem()
{
	sx_tm_init();
	start_time = sx_tm_now();
}

void loop_run()
{
	while (1)
	{
		loop_step();
	}
}
