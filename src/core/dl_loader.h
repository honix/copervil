#pragma once

struct node;

void init_dl_loader();
void register_function(char *name, void (*func)(struct node *node));
void load_library(char *path);

void (*get_function(char *name))(struct node *);