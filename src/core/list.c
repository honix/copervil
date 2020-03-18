#include "list.h"

#include <stdlib.h>

struct list_cell *make_list_cell(void *data)
{
    struct list_cell *list_cell = malloc(sizeof(struct list_cell));

    list_cell->data = data;
    list_cell->next = NULL;

    return list_cell;
}

void link_list_cell(struct list_cell *cell, struct list_cell *next)
{
    cell->next = next;
}

void insert_list_cell(struct list_cell *cell, struct list_cell *next)
{
    struct list_cell *temp = cell->next;
    link_list_cell(cell, next);
    link_list_cell(next, temp);
}

void remove_next_list_cell(struct list_cell *cell)
{
    struct list_cell *temp = cell->next->next;
    link_list_cell(cell, temp);
}