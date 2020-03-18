#include "loop.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "list.h"
#include "utils.h"

// this list will be sorted by > call_time
struct list_cell *delayed_node_list = NULL;

struct delayed_node
{
	struct node *node;
	double call_time;
};

struct delayed_node *make_delayed_node(struct node *node, double call_time)
{
	struct delayed_node *i_node = malloc(sizeof(struct delayed_node));
	i_node->node = node;
	i_node->call_time = call_time;
	return i_node;
}

double current_time()
{
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return time.tv_sec + (double) time.tv_nsec / 1000000000;
}

void delayed_call_node(struct node *node, double secs)
{
	struct list_cell *cell = make_list_cell(
		make_delayed_node(node, current_time() + secs));

	if (delayed_node_list == NULL)
	{
		delayed_node_list = cell;
	}
	else
	{
		// TODO: insert cell in order
		insert_list_cell(delayed_node_list, cell);
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
	printf("// loop step %f\n", current_time());

	double delay;

	if (delayed_node_list != NULL)
	{
		struct delayed_node *i_node = delayed_node_list->data;
		delay = current_time() - i_node->call_time;
	}
	else
	{
		delay = 1.0 / 30; 
	}
	
	delay = MAX(0, delay);
	printf("// sleep for %f\n", delay);

	time_req.tv_sec = (long)delay;
	time_req.tv_nsec = (delay - time_req.tv_sec) * 1000000000;
	nanosleep(&time_req, &time_rem);

	if (delayed_node_list != NULL)
	{
		struct delayed_node *i_node = delayed_node_list->data;
		direct_call_node(i_node->node);
		// TODO: drop and free first cell
		// drop first cell
		delayed_node_list = delayed_node_list->next;
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
