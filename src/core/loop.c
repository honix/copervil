#include "loop.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "node.h"
#include "list.h"

// this list will be sorted by > call_time
struct list_cell *inderect_node_list = NULL;

struct inderect_node
{
	struct node *node;
	long call_time;
};

struct inderect_node *make_inderect_node(struct node *node, long call_time)
{
	struct inderect_node *i_node = malloc(sizeof(struct inderect_node));
	i_node->node = node;
	i_node->call_time = call_time;
	return i_node;
}

// placeholder
long current_time()
{
	return 0;
}

void inderect_call_node(struct node *node, long usec)
{
	// TODO: apply usec arg
	struct list_cell *cell = make_list_cell(
		make_inderect_node(node, current_time() + usec));

	if (inderect_node_list == NULL)
	{
		inderect_node_list = cell;
	}
	else
	{
		insert_list_cell(inderect_node_list, cell);
	}
}

struct timespec time_req;
struct timespec time_rem;

void loop_init()
{
	time_req.tv_sec = 0;
	time_req.tv_nsec = 500000000;
}

void loop_step()
{
	printf("// loop step\n");

	// TODO: smart sleep cycle to call nearest inderect nodes
	nanosleep(&time_req, &time_rem);

	if (inderect_node_list != NULL)
	{
		struct inderect_node *i_node = inderect_node_list->data;
		direct_call_node(i_node->node);
		inderect_node_list = inderect_node_list->next;
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