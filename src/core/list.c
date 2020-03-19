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

void insert_list_cell_ordered(
    struct list *list, 
    struct list_cell *insert_cell,
    int (*ord) (struct list_cell *, struct list_cell *))
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
        insert_list_cell(insert_cell, cell);
        list->first_cell = insert_cell;
        return;        
    }
    prev = cell;
    while(1)
    {
        cell = cell->next;
        if (cell == NULL || ord(cell, insert_cell) <= 0)
        {
            insert_list_cell(prev, insert_cell);
            return;
        }
        prev = cell;
    }
}

void remove_next_list_cell(struct list_cell *cell)
{
    // TODO: free removed cell
    struct list_cell *temp = cell->next->next;
    link_list_cell(cell, temp);
}