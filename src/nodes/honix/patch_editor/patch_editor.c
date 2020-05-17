#include <stdio.h>

#include <GLFW/glfw3.h>

#include "nanovg/src/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg/src/nanovg_gl.h"

#include "sx/allocator.h"
#include "sx/array.h"

#include "core/node_api.h"

#define HIT_RECT_EXPAND_AMOUNT 10

struct type_note *trigger_type_note;

// struct node *patch_editor_node; // needed to call it from glfw callbacks

// TODO: move this data to local node starage
int window_width = 700;
int window_height = 512;

bool need_to_redraw = false;

GLFWwindow *window;
struct NVGcontext *vg;

// TODO: able to select group of nodes
struct node *selected_node;
struct node *dragged_node;
struct vector2i dragged_node_offset;

struct pin_hold
{
	struct node *node;
	enum pin_type pin_type;
	uint8_t pin;
	unsigned long type_id;
};

struct pin_hold pin_hold;

char new_node_name[128];

struct vector2i calc_pin_pos(struct node *node, enum pin_type pin_type, uint8_t pin)
{
	struct vector2i vec;
	switch (pin_type)
	{
	case PIN_INPUT:
		vec.x = node->rect.pos.x + PIN_PADDING + (PIN_SIZE + PIN_PADDING) * pin;
		vec.y = node->rect.pos.y;
		break;

	case PIN_OUTPUT:
		vec.x = node->rect.pos.x + PIN_PADDING + (PIN_SIZE + PIN_PADDING) * pin;
		vec.y = node->rect.pos.y + NODE_HEIGHT - PIN_HALF_SIZE;
		break;

	default:
		break;
	}
	return vec;
}

struct rect calc_pin_rect(struct node *node, enum pin_type pin_type, uint8_t pin)
{
	return (struct rect){
		.pos = calc_pin_pos(node, pin_type, pin),
		.size = (struct vector2i){.x = PIN_SIZE, .y = PIN_HALF_SIZE}};
}

struct node *node_under_cursor(struct vector2i cursor_pos)
{
	struct node *node = NULL;
	for (int i = 0; i < sx_array_count(nodes); i++)
	{
		struct rect expanded_rect =
			expand_rect(nodes[i]->rect, HIT_RECT_EXPAND_AMOUNT);

		if (is_point_in_rect(cursor_pos, expanded_rect))
		{
			node = nodes[i];
			break;
		}
	}
	return node;
}

struct pin_hold pin_under_cursor(struct node *node, struct vector2i cursor_pos)
{
	int x = node->rect.pos.x;
	int y = node->rect.pos.y;
	int width = node->rect.size.y;
	int height = node->rect.size.y;

	struct pin_hold none = {.node = NULL};

	if (cursor_pos.x < x && cursor_pos.x > x + width)
		return none;

	enum pin_type pin_type = PIN_NONE;

	if (cursor_pos.y > y - HIT_RECT_EXPAND_AMOUNT &&
		cursor_pos.y < y + PIN_HALF_SIZE)
		pin_type = PIN_INPUT;
	else if (cursor_pos.y > y + height - PIN_HALF_SIZE &&
			 cursor_pos.y < y + height + HIT_RECT_EXPAND_AMOUNT)
		pin_type = PIN_OUTPUT;

	if (pin_type == PIN_NONE)
		return none;

	uint8_t pin_count = pin_type == PIN_INPUT
							? node->in_pins.array_size
							: node->out_pins.array_size;

	for (int i = 0; i < pin_count; i++)
	{
		struct rect expanded_rect =
			expand_rect(calc_pin_rect(node, pin_type, i),
						HIT_RECT_EXPAND_AMOUNT);

		if (is_point_in_rect(cursor_pos, expanded_rect))
		{
			return (struct pin_hold){
				.node = node,
				.pin_type = pin_type,
				.pin = i,
				.type_id = get_pin(node, pin_type, i)->type_id};
		}
	}

	return none;
}

struct vector2i get_cursor_pos()
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return (struct vector2i){.x = x, .y = y};
}

void draw_node_link(struct NVGcontext *vg, struct node *node, uint8_t pin)
{
	struct vector2i pin_pos = calc_pin_pos(node, PIN_OUTPUT, pin);
	struct link *out_link = get_link_on_pin(node, PIN_OUTPUT, pin);
	if (out_link == NULL)
		return;

	struct link *link = get_link_on_pin(node, PIN_OUTPUT, pin);
	for (uint8_t i = 0; i < link->receivers_count; i++)
	{
		if (out_link->receivers_addresses[i].node == NULL)
			return;

		struct vector2i other_pin_pos = calc_pin_pos(
			out_link->receivers_addresses[i].node,
			PIN_INPUT,
			out_link->receivers_addresses[i].pin_number);
		nvgBeginPath(vg);
		nvgMoveTo(vg, pin_pos.x + PIN_HALF_SIZE, pin_pos.y + PIN_HALF_SIZE);
		nvgLineTo(vg, pin_pos.x + PIN_HALF_SIZE, pin_pos.y + PIN_HALF_SIZE + PIN_SIZE);
		nvgLineTo(vg, other_pin_pos.x + PIN_HALF_SIZE, other_pin_pos.y - PIN_SIZE);
		nvgLineTo(vg, other_pin_pos.x + PIN_HALF_SIZE, other_pin_pos.y);

		nvgLineJoin(vg, NVG_ROUND);
		nvgStrokeWidth(vg, 3);
		unsigned long type_id = get_pin(node, PIN_OUTPUT, pin)->type_id;
		if (type_id == trigger_type_note->id)
			nvgStrokeColor(vg, nvgHSLA(0.6f, 1.0f, 0.5f, 220));
		else
			nvgStrokeColor(vg, nvgHSLA(0, 0, 1.0f, 170));
		nvgStroke(vg);
	}
}

void draw_pin_hold(struct NVGcontext *vg)
{
	struct vector2i cursor_pos = get_cursor_pos();
	struct vector2i pin_pos =
		calc_pin_pos(pin_hold.node, pin_hold.pin_type, pin_hold.pin);

	nvgBeginPath(vg);
	if (pin_hold.pin_type == PIN_OUTPUT)
	{
		nvgMoveTo(vg, pin_pos.x + PIN_HALF_SIZE, pin_pos.y + PIN_HALF_SIZE);
		nvgLineTo(vg, pin_pos.x + PIN_HALF_SIZE, pin_pos.y + PIN_HALF_SIZE + PIN_SIZE);
		nvgLineTo(vg, cursor_pos.x, cursor_pos.y - PIN_SIZE);
		nvgLineTo(vg, cursor_pos.x, cursor_pos.y);
	}
	else
	{
		nvgMoveTo(vg, pin_pos.x + PIN_HALF_SIZE, pin_pos.y);
		nvgLineTo(vg, pin_pos.x + PIN_HALF_SIZE, pin_pos.y - PIN_SIZE);
		nvgLineTo(vg, cursor_pos.x, cursor_pos.y + PIN_SIZE);
		nvgLineTo(vg, cursor_pos.x, cursor_pos.y);
	}

	nvgLineJoin(vg, NVG_ROUND);
	nvgStrokeWidth(vg, 3);
	nvgStrokeColor(vg, nvgHSLA(0, 0, 128, 170));
	nvgStroke(vg);
}

void draw_node(struct NVGcontext *vg, struct node *node, bool only_body)
{

	int x = node->rect.pos.x;
	int y = node->rect.pos.y;
	// int width = node->rect.size.x;
	int height = node->rect.size.y;

	// Draw body
	nvgBeginPath(vg);
	nvgRect(vg, x, y, NODE_WIDTH, NODE_HEIGHT);
	nvgFillColor(vg, nvgRGBA(255, 192, 0, 255));
	nvgFill(vg);
	nvgLineJoin(vg, NVG_BUTT);
	nvgStrokeWidth(vg, 2);
	nvgStrokeColor(vg, node == selected_node
						   ? nvgHSLA(0.5f, 0.75f, 0.75f, 200)
						   : nvgHSLA(0, 0, 0, 100));
	nvgStroke(vg);

	if (node->function_note.draw_func != NULL)
	{
		node->function_note.draw_func(vg, node);
	}
	else
	{
		// Draw node name
		nvgFontSize(vg, 15.0f);
		nvgFontFace(vg, "sans");
		nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
		nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
		nvgText(vg, x + 10, y + height / 2, node->function_note.name, NULL);
	}

	// Draw in pins
	for (int i = 0; i < node->in_pins.array_size; i++)
	{
		struct vector2i pin_pos = calc_pin_pos(node, PIN_INPUT, i);
		nvgBeginPath(vg);
		nvgRect(vg,
				pin_pos.x, pin_pos.y,
				PIN_SIZE, PIN_HALF_SIZE);
		unsigned long type_id = get_pin(node, PIN_INPUT, i)->type_id;
		if (type_id == trigger_type_note->id)
			nvgFillColor(vg, nvgHSLA(0.6f, 1, 0.5f, 128));
		else if (pin_hold.node != NULL && pin_hold.type_id == type_id)
			nvgFillColor(vg, nvgHSLA(0.35f, 1, 0.5f, 128));
		else
			nvgFillColor(vg, nvgHSLA(0, 0, 0, 128));
		nvgFill(vg);

		if (!only_body)
		{
			if (node == selected_node)
			{
				// Draw node name
				nvgFontSize(vg, 15.0f);
				nvgFontFace(vg, "sans");
				nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
				nvgTranslate(vg, pin_pos.x + PIN_HALF_SIZE, pin_pos.y - 10);
				nvgRotate(vg, -45);
				nvgFillColor(vg, nvgHSLA(0, 0, 1, 255));
				nvgText(vg, 0, 0, get_pin(node, PIN_INPUT, i)->name, NULL);
				nvgReset(vg);
			}
		}
	}

	// Draw out pins and out links
	for (int i = 0; i < node->out_pins.array_size; i++)
	{
		// Draw out pin
		struct vector2i pin_pos = calc_pin_pos(node, PIN_OUTPUT, i);
		nvgBeginPath(vg);
		nvgRect(vg,
				pin_pos.x, pin_pos.y,
				PIN_SIZE, PIN_HALF_SIZE);
		unsigned long type_id = get_pin(node, PIN_OUTPUT, i)->type_id;
		if (type_id == trigger_type_note->id)
			nvgFillColor(vg, nvgHSLA(0.6f, 1, 0.5f, 128));
		else
			nvgFillColor(vg, nvgHSLA(0, 0, 0, 128));
		nvgFill(vg);

		if (!only_body)
		{
			// Draw link from this out pin
			draw_node_link(vg, node, i);

			if (node == selected_node)
			{
				// Draw node name
				nvgFontSize(vg, 15.0f);
				nvgFontFace(vg, "sans");
				nvgTextAlign(vg, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
				nvgTranslate(vg, pin_pos.x + PIN_HALF_SIZE, pin_pos.y + 10 + PIN_HALF_SIZE);
				nvgRotate(vg, -45);
				nvgFillColor(vg, nvgHSLA(0, 0, 1, 255));
				nvgText(vg, 0, 0, get_pin(node, PIN_OUTPUT, i)->name, NULL);
				nvgReset(vg);
			}
		}
	}
}

void draw_patch_editor()
{
	glfwMakeContextCurrent(window);

	glViewport(0, 0, window_width, window_height);
	glClearColor(0.25f, 0.25f, 0.25f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	nvgBeginFrame(vg, window_width, window_height, 1);

	for (int i = 0; i < sx_array_count(nodes); i++)
	{
		draw_node(vg, nodes[i], false);
	}

	if (pin_hold.node != NULL)
	{
		draw_pin_hold(vg);
	}

	nvgFontSize(vg, 15.0f);
	nvgFontFace(vg, "sans");
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_BOTTOM);
	nvgFillColor(vg, nvgRGBA(255, 120, 0, 255));
	nvgText(vg, 10, window_height - 10, new_node_name, NULL);

	nvgEndFrame(vg);

	glfwSwapBuffers(window);
}

void draw_patch_editor_custom_nodes()
{
	glfwMakeContextCurrent(window);

	glViewport(0, 0, window_width, window_height);

	nvgBeginFrame(vg, window_width, window_height, 1);

	for (int i = 0; i < sx_array_count(nodes); i++)
	{
		if (nodes[i]->function_note.draw_func != NULL)
			draw_node(vg, nodes[i], true);
	}

	nvgEndFrame(vg);

	glfwSwapBuffers(window);
}

void redraw_patch_editor()
{
	need_to_redraw = true;
}

void character_callback(GLFWwindow *window, unsigned int codepoint)
{
	uint8_t p = -1;
	while (new_node_name[++p] != '\0')
		;
	new_node_name[p] = (char)codepoint;
	new_node_name[p + 1] = '\0';
}

void key_callback(
	GLFWwindow *window, int key,
	int scancode, int action, int mods)
{
	// printf("key_callback: %s %d %d %d %d\n",
	//     glfwGetKeyName(key, scancode),
	//     key, scancode, action, mods);

	struct function_note *note;
	uint8_t p;
	struct vector2i pos;

	if (action & (GLFW_PRESS | GLFW_REPEAT))
	{

		switch (key)
		{
		case GLFW_KEY_DELETE:
			if (action == GLFW_PRESS && selected_node != NULL)
			{
				free_node(selected_node);
				selected_node = NULL;
			}
			break;
		case GLFW_KEY_Q:
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			break;
		case GLFW_KEY_ENTER:
			note = get_function_note(new_node_name);
			if (note != NULL)
			{
				pos = get_cursor_pos();
				make_node(pos.x, pos.y, note);
			}
			if (!(mods & GLFW_MOD_SHIFT))
				new_node_name[0] = '\0';
			break;
		case GLFW_KEY_BACKSPACE:
			p = -1;
			while (new_node_name[++p] != '\0')
				;
			if (p > 0)
				new_node_name[p - 1] = '\0';
			break;
		}
	}

	if (selected_node != NULL &&
		selected_node->function_note.input_key_func != NULL)
	{
		selected_node->function_note.input_key_func(
			selected_node, key, scancode, action, mods);
	}

	redraw_patch_editor();
}

void cursor_pos_callback(GLFWwindow *window, double x, double y)
{
	// printf("cursor_pos_callback: %d %d\n", (int)x, (int)y);
	// nodes[1]->x = x; // TEST
	// nodes[1]->y = y;

	if (dragged_node != NULL)
	{
		struct vector2i cursor_pos = {.x = x, .y = y};
		dragged_node->rect.pos = vector_add(cursor_pos, dragged_node_offset);

		redraw_patch_editor();
	}

	if (pin_hold.node != NULL)
		redraw_patch_editor();
}

void clear_pin_hold()
{
	pin_hold = (struct pin_hold){.node = NULL};
}

void mouse_button_callback(
	GLFWwindow *window,
	int button, int action, int mods)
{
	struct vector2i cursor_pos = get_cursor_pos();

	// printf("mouse_button_callback: %d %d %d %d\n",
	// 	   button, action, cursor_pos.x, cursor_pos.y);

	switch (action)
	{
	case GLFW_PRESS:
		dragged_node = selected_node = node_under_cursor(cursor_pos);
		if (dragged_node != NULL)
		{
			dragged_node_offset = vector_sub(
				dragged_node->rect.pos,
				cursor_pos);

			struct pin_hold new_pin_hold =
				pin_under_cursor(selected_node, cursor_pos);
			if (new_pin_hold.node != NULL && pin_hold.node != NULL &&
				new_pin_hold.pin_type != pin_hold.pin_type)
			{
				if (pin_hold.pin_type == PIN_OUTPUT)
					connect_nodes(
						pin_hold.node, pin_hold.pin,
						new_pin_hold.node, new_pin_hold.pin);
				else
					connect_nodes(
						new_pin_hold.node, new_pin_hold.pin,
						pin_hold.node, pin_hold.pin);
				clear_pin_hold();
			}
			else
			{
				pin_hold = new_pin_hold;
			}
		}
		else
		{
			clear_pin_hold();
		}
		redraw_patch_editor();
		break;
	case GLFW_RELEASE:
		dragged_node = NULL;
		break;
	}
}

void window_size_callback(GLFWwindow *window, int width, int height)
{
	window_width = width;
	window_height = height;

	printf("window_size_callback: %d %d\n", width, height);

	glfwMakeContextCurrent(window);
	glViewport(0, 0, width, height);

	redraw_patch_editor();
}

void init_window()
{
	trigger_type_note = reg_type("trigger", sizeof(trigger));

	if (!glfwInit())
		return;

#ifndef _WIN32 // don't require this on win32, and works with more cards
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	window = glfwCreateWindow(
		window_width, window_height, "World", NULL, NULL);

	int xpos, ypos, width, height;
	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(),
						   &xpos, &ypos, &width, &height);
	glfwSetWindowPos(window,
					 width / 2 - window_width / 2,
					 height / 2 - window_height / 2);

	if (!window)
	{
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetCharCallback(window, character_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
	if (vg == NULL)
	{
		printf("Could not init nanovg.\n");
		return;
	}

	nvgCreateFont(vg, "sans", "./assets/Roboto-Regular.ttf");

	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glEnable(GL_CULL_FACE);
	// glDisable(GL_DEPTH_TEST);
	// end initialization

	redraw_patch_editor();
}

void deinit()
{
	glfwTerminate();
}

void patch_editor_init(struct node *node)
{
	// node->in_pins_mask = 1 << 0;
	init_pins(node, true, 0, 0);
	// REG_PIN(node, PIN_INPUT, 0, "trigger", trigger);
	// node->only_self_trigger = true;

	init_window();

	direct_call_node_self(node);
}

int frame = 0;

void patch_editor(struct node *node)
{
	frame++;

	if (need_to_redraw)
	{
		need_to_redraw = false;
		draw_patch_editor();
	}
	else if (frame % 60 == 0)
	{
		draw_patch_editor_custom_nodes();
	}

	glfwPollEvents();

	if (glfwWindowShouldClose(window))
	{
		glfwTerminate(); // TEMP
		exit(0);
	}

	delayed_call_node_self(node, 1.0 / 60);
}

void register_library()
{
	register_function((struct function_note){
		.name = "patch_editor",
		.init_func = patch_editor_init,
		.main_func = patch_editor});
}