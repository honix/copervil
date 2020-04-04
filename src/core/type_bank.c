#include "type_bank.h"

#include <string.h>

#include "list.h"

struct list type_note_list;

struct type_note
{
    unsigned long id; // hash of name

    char *name;
    size_t size;
};

struct type_note *make_type_note(unsigned int id, char *name, size_t size)
{
    struct type_note *type_note = malloc(sizeof(struct type_note));
    type_note->id = id;
    type_note->name = name;
    type_note->size = size;
    return type_note;
}

struct is_type_registed_pred_args
{
    unsigned int id;
};

static bool is_type_registed_pred(struct list_cell *cell, void *pred_args)
{
    unsigned long cell_id =
        ((struct type_note *)cell->data)->id;
    unsigned long pred_id =
        ((struct is_type_registed_pred_args *)pred_args)->id;
    return cell_id == pred_id;
}

static bool is_type_id_registred(unsigned long id)
{
    return is_list_any(
        &type_note_list,
        is_type_registed_pred,
        (struct is_type_registed_pred_args *){id});
}

static unsigned long hash(char *cp)
{
    // D. J. Bernstein hash function
    unsigned long hash = 5381;
    while (*cp)
        hash = 33 * hash ^ (unsigned char)*cp++;
    return hash;
}

void reg_type(char *name, size_t size)
{
    unsigned long id = hash(name);
    if (!is_type_id_registred(id))
    {
        insert_list_cell_at_top(
            &type_note_list,
            make_type_note(id, name, size));
    }
}

bool is_same_types(char *type_name_a, char *type_name_b)
{
    return strcmp(type_name_a, type_name_b) == 0 ? true : false;
}

struct get_size_of_type_pred_args
{
    char *name;
};

bool get_size_of_type_pred(struct list_cell *cell, void *pred_args)
{
    char *cell_type_name =
        ((struct type_note *)cell->data)->name;
    char *pred_type_name =
        ((struct get_size_of_type_pred_args *)pred_args)->name;
    return is_same_types(cell_type_name, pred_type_name);
}

size_t get_size_of_type(char *type_name)
{
    struct list_cell *cell = list_find(
        &type_note_list,
        get_size_of_type_pred,
        (struct get_size_of_type_pred_args *){type_name});

    if (cell == NULL)
        return -1;

    return ((struct type_note *)cell->data)->size;
}