#include "link.h"

#include <stdlib.h>

link_t *make_link (void *data) {
	link_t *link = malloc(sizeof(link_t));
	link->data = data;
	
	return link;
}
