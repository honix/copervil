#include "link.h"

#include <stdlib.h>
#include <stdio.h>

#include "node.h"

#define LINKS_MAX_COUNT 128 // TODO: this is bad temp code. rewrite

void init_links_subsystem()
{
	links = malloc(sizeof(struct link *) * LINKS_MAX_COUNT);
	links_pointer = 0;
}

struct link *make_link(void *data)
{
	struct link *link = malloc(sizeof(struct link));
	link->data = data;
	// link->sender = NULL;
	// link->receiver = NULL;
	link->receivers_addresses =
		calloc(MAX_RECEIVERS_COUNT, sizeof(struct link_address));
	link->receivers_count = 0;

	links[links_pointer] = link;
	links_pointer++;

	if (links_pointer >= LINKS_MAX_COUNT)
		printf("Oops! LINKS_MAX_COUNT exceeded!\n");

	return link;
}

void free_link(struct link *link, enum pin_type owner)
{
	if (owner == PIN_INPUT && link->sender_address.node != NULL)
		drop_link(link->sender_address.node,
				  PIN_OUTPUT,
				  link->sender_address.pin_number);
	if (owner == PIN_OUTPUT)
	{
		for (uint8_t i = 0; i < link->receivers_count; i++)
		{
			if (link->receivers_addresses[i].node != NULL)
				drop_link(link->receivers_addresses[i].node,
						  PIN_INPUT,
						  link->receivers_addresses[i].pin_number);
		}
	}
	
	free(link->data);
	free(link->receivers_addresses);
	free(link);

	// TODO: remove from links collection
}