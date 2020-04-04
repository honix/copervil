#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

struct type_note
{
    unsigned long id; // hash of name

    char *name;
    size_t size;
};

// #define REG_TYPE(type)
//     reg_type(#type, sizeof(type))

// Add new type to type bank, if type is already registered does nothing
// TODO: if old reg size not match new one, throw error
struct type_note *reg_type(char *type_name, size_t size);

// Returns true if names are the same. Does string comprasion
bool is_same_types(char *type_name_a, char *type_name_b);

// Returns type_note by given type id (hash), otherwice returns NULL
struct type_note *get_type_note_by_id(unsigned long id);

// Returns type_note by given type name, otherwice returns NULL
struct type_note *get_type_note_by_name(char *type_name);