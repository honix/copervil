#pragma once

struct node;
struct NVGcontext;

struct function_note
{
	char *name;

	// Moderate func, can be NULL
	void (*init_func)(struct node *node);
	// Required func, can't be NULL
	void (*main_func)(struct node *node);
	// Moderate func, can be NULL
	void (*deinit_func)(struct node *node);

	// Custom nodes funcs, can be NULL
	void (*draw_func)(struct NVGcontext *vg, struct node *node);

	void (*input_key_func)(struct node *, 
		int key, int scancode, int action, int mods);
	void (*input_mouse_button_func)(struct node *,
		int button, int action, int mods);
};

// typedef void (*reg_function_t)(
// 	struct function_note function_note
// );

void init_dl_loader_subsystem();
void register_function(struct function_note function_note);
void load_library(char *path);

struct function_note *get_function_note(char *name);