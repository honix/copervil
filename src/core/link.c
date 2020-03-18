#include "link.h"

#include <stdlib.h>

struct link *make_link(void *data)
{
	struct link *link = malloc(sizeof(struct link));
	link->data = data;
	link->sender = NULL;
	link->receiver = NULL;

	return link;
}