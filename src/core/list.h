#pragma once

struct list
{
	struct list_cell *first_cell;
};

struct list_cell
{
	void *data;
	struct list_cell *next;
};

struct list_cell *make_list_cell(void *data);
void link_list_cell(struct list_cell *cell, struct list_cell *next);
void insert_list_cell(struct list_cell *cell, struct list_cell *next);
void insert_list_cell_ordered(
	struct list *list,
	struct list_cell *insert_cell,
	int (*ord)(struct list_cell *, struct list_cell *));
void remove_next_list_cell(struct list_cell *cell);
