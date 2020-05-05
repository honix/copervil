#include "type_bank.h"

#include <string.h>

#include "sx/hash.h"

#include "list.h"

static struct list type_note_list;

static struct type_note *make_type_note(uint32_t id, char *name, size_t size)
{
    struct type_note *type_note = malloc(sizeof(struct type_note));
    type_note->id = id;
    type_note->name = name;
    type_note->size = size;
    return type_note;
}

static uint32_t hash(char *cp)
{
    return sx_hash_fnv32_str(cp);
}

struct get_type_note_by_id_pred_args
{
    unsigned int id;
};

static bool get_type_note_by_id_pred(struct list_cell *cell, void *pred_args)
{
    unsigned long cell_id =
        ((struct type_note *)cell->data)->id;
    unsigned long pred_id =
        ((struct get_type_note_by_id_pred_args *)pred_args)->id;
    return cell_id == pred_id;
}

struct type_note *get_type_note_by_id(unsigned long id)
{
    struct list_cell *cell = list_find(
        &type_note_list,
        get_type_note_by_id_pred,
        &(struct get_type_note_by_id_pred_args){id});

    if (cell == NULL)
        return NULL;

    return (struct type_note *)cell->data;
}

struct type_note *reg_type(char *name, size_t size)
{
    uint32_t id = hash(name);

    struct type_note *type_note = get_type_note_by_id(id);
    if (type_note != NULL)
    {
        return type_note;
    }
    else
    {
        type_note = make_type_note(id, name, size);
        insert_list_cell_at_top(
            &type_note_list,
            make_list_cell(type_note));
        return type_note;
    }
}

bool is_same_types(char *type_name_a, char *type_name_b)
{
    return hash(type_name_a) == hash(type_name_b);
}

struct type_note *get_type_note_by_name(char *type_name)
{
    return get_type_note_by_id(hash(type_name));
}