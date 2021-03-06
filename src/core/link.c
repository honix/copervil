#include "link.h"

#include <stdlib.h>
#include <stdio.h>

#include "sx/allocator.h"
#include "sx/array.h"

#include "node.h"

void init_links_subsystem()
{
	links = NULL;
}

struct link *make_link(void *data)
{
	struct link *link = malloc(sizeof(struct link));
	link->data = data;
	// link->sender = NULL;
	// link->receiver = NULL;
	link->sender_address.node = NULL;
	link->sender_address.pin_number = -1;
	link->receivers_addresses = // TODO: replace with sx_array?
		calloc(MAX_RECEIVERS_COUNT, sizeof(struct link_address));
	link->receivers_count = 0;

	sx_array_push(sx_alloc_malloc_leak_detect(), links, link);

	return link;
}

void free_link(struct link *link, enum pin_type owner, struct node *owner_node)
{
	if (owner == PIN_INPUT && link->sender_address.node != NULL)
		drop_link(link->sender_address.node,
				  PIN_OUTPUT,
				  link->sender_address.pin_number);

	for (uint8_t i = 0; i < link->receivers_count; i++)
	{
		struct link_address link_address = link->receivers_addresses[i];
		if (link_address.node != NULL && link_address.node != owner_node)
			drop_link(link_address.node,
					  PIN_INPUT,
					  link_address.pin_number);
	}

	free(link->data);
	free(link->receivers_addresses);

	unsigned int shift = 0;
	for (int i = 0; i < sx_array_count(links); i++)
	{
		if (links[i] == link)
		{
			shift = i;
			break;
		}
	}

	links[shift] = sx_array_last(links);
	sx_array_pop_last(links);

	free(link);
}

void set_sender(
	struct link *link,
	struct node *sender,
	uint8_t sender_pin_number)
{
	link->sender_address.node = sender;
	link->sender_address.pin_number = sender_pin_number;
}

void add_receiver(
	struct link *link,
	struct node *receiver,
	uint8_t reciever_pin_number)
{
	link->receivers_addresses[link->receivers_count].node = receiver;
	link->receivers_addresses[link->receivers_count].pin_number = reciever_pin_number;
	link->receivers_count++;

	// printf("link->receivers_count = %d\n", link->receivers_count); // TODO: bad code

	if (link->receivers_count > MAX_RECEIVERS_COUNT)
		printf("Error: MAX_RECEIVERS_COUNT exceeded\n"); // TODO: bad code
}