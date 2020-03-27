#pragma once

struct node;
struct NVGcontext;

typedef void (*reg_function_t)(
	char *name,
	void (*init_func)(struct node *),
	void (*main_func)(struct node *),
	void (*deinit_func)(struct node *),
	void (*draw_func)(struct NVGcontext *vg, struct node *),
	void (*input_func)(struct node *));

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
	void (*input_func)(struct node *node);
};

void init_dl_loader_subsystem();
void register_function(
	char *name,
	void (*init_func)(struct node *),
	void (*main_func)(struct node *),
	void (*deinit_func)(struct node *),
	void (*draw_func)(struct NVGcontext *vg, struct node *),
	void (*input_func)(struct node *));
void load_library(char *path);

struct function_note *get_function_note(char *name);