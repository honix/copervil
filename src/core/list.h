#pragma once

#include <stdbool.h>

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
// void link_list_cell(struct list_cell *cell, struct list_cell *next);
// void link_list_cell_saving_tail(struct list_cell *cell, struct list_cell *next);
void insert_list_cell_at_top(
	struct list *list,
	struct list_cell *insert_cell);
void insert_list_cell_ordered(
	struct list *list,
	struct list_cell *insert_cell,
	int (*ord)(struct list_cell *, struct list_cell *));

// Return first cell that turns pred to true, otherwice returns NULL
struct list_cell *list_find(
	struct list *list, 
	bool(pred)(struct list_cell *, void *), 
	void *pred_args);

// Return true if pred returns true on at least one element in the list
bool is_list_any(
	struct list *list, 
	bool(pred)(struct list_cell *, void *), 
	void *pred_args);

void remove_next_list_cell(struct list_cell *cell);
