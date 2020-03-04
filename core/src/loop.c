#include "loop.h"

#include <time.h>
#include <stdio.h>

#include "node.h"

struct node_t *next_inderect_node = NULL;

void inderect_call_node (struct node_t *node, long usec)
{
    // TODO: apply usec arg
    next_inderect_node = node;
}

void run_loop ()
{
	struct timespec time_req;
	time_req.tv_sec = 1;
	time_req.tv_nsec = 0;

	struct timespec time_rem;

	while (1)
	{
		printf("// loop\n");

		nanosleep(&time_req, &time_rem);
		
		if (next_inderect_node != NULL) {
			direct_call_node(next_inderect_node);
			next_inderect_node = NULL;
		}
	}
}
