#include "link.h"

#include <stdlib.h>

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
		printf("Oops! LINKS_MAX_COUNT exceeded!");

	return link;
}

void free_link(struct link *link)
{
	free(link->data);
	free(link);

	// TODO: remove from links collection
}