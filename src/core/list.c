#include "list.h"

#include <stdlib.h>

struct list_cell *make_list_cell(void *data)
{
	struct list_cell *list_cell = malloc(sizeof(struct list_cell));

	list_cell->data = data;
	list_cell->next = NULL;

	return list_cell;
}

static void link_list_cell(struct list_cell *cell, struct list_cell *next)
{
	cell->next = next;
}

static void link_list_cell_saving_tail(
	struct list_cell *cell,
	struct list_cell *next)
{
	struct list_cell *temp = cell->next;
	link_list_cell(cell, next);
	link_list_cell(next, temp);
}

void insert_list_cell_at_top(
	struct list *list,
	struct list_cell *insert_cell)
{
	struct list_cell *cell = list->first_cell;
	if (cell == NULL)
	{
		list->first_cell = insert_cell;
		return;
	}
	else
	{
		link_list_cell(insert_cell, cell);
		list->first_cell = insert_cell;
		return;
	}
}

void insert_list_cell_ordered(
	struct list *list,
	struct list_cell *insert_cell,
	int (*ord)(struct list_cell *, struct list_cell *))
{
	struct list_cell *cell = list->first_cell;
	struct list_cell *prev;
	if (cell == NULL)
	{
		list->first_cell = insert_cell;
		return;
	}
	if (ord(cell, insert_cell) <= 0)
	{
		link_list_cell(insert_cell, cell);
		list->first_cell = insert_cell;
		return;
	}
	prev = cell;
	while (1)
	{
		cell = cell->next;
		if (cell == NULL || ord(cell, insert_cell) <= 0)
		{
			link_list_cell_saving_tail(prev, insert_cell);
			return;
		}
		prev = cell;
	}
}

struct list_cell *list_find(
	struct list *list,
	bool(pred)(struct list_cell *, void *),
	void *pred_args)
{
	struct list_cell *cell = list->first_cell;
	if (cell == NULL)
		return NULL;
	do
		if (pred(cell, pred_args))
			return cell;
	while ((cell = cell->next) != NULL);

	return NULL;
}

bool is_list_any(
	struct list *list,
	bool(pred)(struct list_cell *, void *),
	void *pred_args)
{
	return list_find(list, pred, pred_args) == NULL ? false : true;
}

void remove_next_list_cell(struct list_cell *cell)
{
	// TODO: free removed cell
	struct list_cell *temp = cell->next->next;
	link_list_cell(cell, temp);
}