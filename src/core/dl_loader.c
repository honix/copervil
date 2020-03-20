#include "dl_loader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

struct function_note
{
    char *name;
    void (*func)(struct node *);
};

struct function_note **loaded_functions;
unsigned int loaded_functions_pointer;

void init_dl_loader()
{
    loaded_functions = malloc(sizeof(struct function_note *) * 16);
    loaded_functions_pointer = 0;
}

void register_function(char *name, void (*func)(struct node *node))
{
    struct function_note *note = malloc(sizeof(struct function_note));
    note->name = name;
    note->func = func;
    loaded_functions[loaded_functions_pointer] = note;
    loaded_functions_pointer++;
}

void load_library(char *path)
{
    void *handle;
    handle = dlopen(path, RTLD_LAZY);
    void (*register_library)(void (*)(char *, void (*)(struct node *)));
    register_library = dlsym(handle, "register_library");
    if (register_library == NULL)
    {
        printf("Error: No register function in %s\n", path);
        return;
    }
    register_library(register_function);
}

void (*get_function(char *name))(struct node *)
{
    for (int i = 0; i < loaded_functions_pointer; i++)
    {
        struct function_note *note = loaded_functions[i];
        if (strcmp(note->name, name) == 0)
            return note->func;
    }

    printf("Error: No such function %s\n", name);
    return NULL;
}