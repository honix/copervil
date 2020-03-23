#pragma once

struct node;

typedef void (*reg_function_t)(
            char *, 
            void (*)(struct node *),
            void (*)(struct node *),
            void (*)(struct node *));

struct function_note
{
    char *name;
    void (*init_func)(struct node *node);
    void (*main_func)(struct node *node);
    void (*deinit_func)(struct node *node);
};

void init_dl_loader_subsystem();
void register_function(
    char *name, 
    void (*init_func)(struct node *node),
    void (*main_func)(struct node *node),
    void (*deinit_func)(struct node *node));
void load_library(char *path);

struct function_note *get_function_note(char *name);