#include "dl_loader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

struct function_note **loaded_functions;
unsigned int loaded_functions_pointer;

void init_dl_loader_subsystem()
{
	loaded_functions = malloc(sizeof(struct function_note *) * 16);
	loaded_functions_pointer = 0;
}

void register_function(
	char *name,
	void (*init_func)(struct node *),
	void (*main_func)(struct node *),
	void (*deinit_func)(struct node *),
	void (*draw_func)(struct NVGcontext *vg, struct node *),
	void (*input_func)(struct node *))
{
	struct function_note *note = malloc(sizeof(struct function_note));
	note->name = name;
	note->init_func = init_func;
	note->main_func = main_func;
	note->deinit_func = deinit_func;
	note->draw_func = draw_func;
	note->input_func = input_func;
	loaded_functions[loaded_functions_pointer] = note;
	loaded_functions_pointer++;
}

void load_library(char *path)
{
	void *handle;
	handle = dlopen(path, RTLD_LAZY);
	void (*register_library)(reg_function_t);
	register_library = dlsym(handle, "register_library");
	if (register_library == NULL)
	{
		printf("Error: No register function in %s\n", path);
		return;
	}
	register_library(register_function);
}

struct function_note *get_function_note(char *name)
{
	for (int i = 0; i < loaded_functions_pointer; i++)
	{
		struct function_note *note = loaded_functions[i];
		if (strcmp(note->name, name) == 0)
			return note;
	}

	printf("Error: No such function note %s\n", name);
	return NULL;
}