#pragma once

#include <stdlib.h>
#include <stdbool.h>

#define REG_TYPE(type) \
    req_type(#type, sizeof(type))

// Add new type to type bank, if type is already registered does nothing
// TODO: if old reg size not match new one, throw error
void reg_type(char *type_name, size_t size);

// Returns true if names are the same. Does string comprasion
bool is_same_types(char *type_name_a, char *type_name_b);

// Returns size of type in bytes, if type not registered returns -1
size_t get_size_of_type(char *type_name);