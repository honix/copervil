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
	link->sender = NULL;
	link->receiver = NULL;

	links[links_pointer] = link;
	links_pointer++;

	if (links_pointer >= LINKS_MAX_COUNT)
		printf("Oops! LINKS_MAX_COUNT exceeded!\n");

	return link;
}

void free_link(struct link *link, enum pin_type owner)
{
	if (owner == PIN_INPUT && link->sender != NULL)
		drop_link(link->sender, PIN_OUTPUT, link->sender_pin);
	if (owner == PIN_OUTPUT && link->receiver != NULL)
		drop_link(link->receiver, PIN_INPUT, link->receiver_pin);

	free(link->data);
	free(link);

	// TODO: remove from links collection
}