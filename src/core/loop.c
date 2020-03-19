#include "loop.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "list.h"
#include "utils.h"

// this list will be always sorted by > call_time
struct list delayed_node_list;

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
	// TODO: this seems to work only on linux family
	// need test this function on windows
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return time.tv_sec + (double) time.tv_nsec / 1000000000;
}

int ord(struct list_cell *a, struct list_cell *b)
{
	struct delayed_node *delayed_node_a = a->data;
	struct delayed_node *delayed_node_b = b->data;
	return delayed_node_a->call_time < delayed_node_b->call_time ? 1 : 0;
}

void delayed_call_node(struct node *node, double secs)
{
	struct list_cell *cell = make_list_cell(
		make_delayed_node(node, current_time_secs() + secs));

	// if (delayed_node_list.first_cell == NULL)
	// {
	// 	delayed_node_list = cell;
	// }
	// else
	{
		// TODO: insert cell in order (see delayed_node_list comment)
		// insert_list_cell(delayed_node_list, cell);
		insert_list_cell_ordered(&delayed_node_list, cell, ord);
	}
}

struct timespec time_req;
struct timespec time_rem;

void loop_init()
{
	time_req.tv_sec = 0;
	time_req.tv_nsec = 0;
}

void loop_step()
{
	// printf("// loop step %f\n", current_time_secs());

	double delay;

	if (delayed_node_list.first_cell != NULL)
	{
		struct delayed_node *delayed_node = delayed_node_list.first_cell->data;
		delay = delayed_node->call_time - current_time_secs();
	}
	else
	{
		delay = 1.0 / 30; 
	}
	
	delay = MAX(0, delay);
	// printf("// sleep for %f\n", delay);

	if (delay != 0) {
		time_req.tv_sec = (long)delay;
		time_req.tv_nsec = (delay - time_req.tv_sec) * 1000000000;
		nanosleep(&time_req, &time_rem);
	}

	if (delayed_node_list.first_cell != NULL)
	{
		struct delayed_node *delayed_node = delayed_node_list.first_cell->data;
		direct_call_node(delayed_node->node);
		// TODO: drop and free first cell
		// drop first cell
		delayed_node_list.first_cell = delayed_node_list.first_cell->next;
	}
}

void loop_run()
{
	loop_init();

	while (1)
	{
		loop_step();
	}
}
