#include "link.h"

#include <stdlib.h>

struct link_t *make_link (void *data) 
{
	struct link_t *link = malloc(sizeof(struct link_t));
	link->data = data;
	link->sender = NULL;
	link->receiver = NULL;
	
	return link;
}