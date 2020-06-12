#include "dl_loader.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sx/os.h"

struct function_note **loaded_functions;
unsigned int loaded_functions_pointer;

void init_dl_loader_subsystem()
{
	loaded_functions = malloc(sizeof(struct function_note *) * 128); // TODO: rewrite with sx_arrays
	loaded_functions_pointer = 0;
}

void register_function(struct function_note function_note)
{
	printf("Registering function %s\n", function_note.name);
	struct function_note *copy = malloc(sizeof(struct function_note));
	memcpy(copy, &function_note, sizeof(struct function_note));
	loaded_functions[loaded_functions_pointer] = copy;
	loaded_functions_pointer++;
}

void load_library(char *path)
{
	printf("Loading library %s\n", path);
	void *handle = sx_os_dlopen(path);
	void (*register_library)();
	register_library = sx_os_dlsym(handle, "register_library");
	if (register_library == NULL)
	{
		printf("Error: No register_library function in %s\n", path);
		return;
	}
	register_library();
}

struct function_note *get_function_note(char *name)
{
	for (int i = 0; i < loaded_functions_pointer; i++)
	{
		struct function_note *note = loaded_functions[i];
		if (strcmp(note->name, name) == 0)
			return note;
	}

	printf("Error: No such function note: %s\n", name);
	return NULL;
}