#include <stdio.h>

#include <GLFW/glfw3.h>

#include "nanovg/src/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg/src/nanovg_gl.h"

#include "core/node.h"
#include "core/link.h"
#include "core/loop.h"
#include "core/dl_loader.h"

enum number_io_inputs
{
	in_number
};

enum number_io_outputs
{
	out_number
};

void number_io_init(struct node *node)
{
	// define in and out pins count
	init_pins(node, 1, 1);
	// reg pin
	REG_PIN(node, PIN_INPUT, in_number, "number", int);
	REG_PIN(node, PIN_OUTPUT, out_number, "number", int);
}

void number_io(struct node *node)
{
}

void number_io_draw(struct NVGcontext *vg, struct node *node)
{
	int x = node->rect.pos.x;
	int y = node->rect.pos.y;
	int width = node->rect.size.x;
	int height = node->rect.size.y;

	int number = GET_PIN(node, PIN_INPUT, in_number, int);

	// Draw node name
	nvgBeginPath(vg);
	nvgFillColor(vg, nvgRGBA(0, 0, 250, 75));
	nvgRect(vg, x, y, width * ((float)number / 100), height);
	nvgFill(vg);

	nvgFontSize(vg, 15.0f);
	nvgFontFace(vg, "sans");
	nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
	nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
	char s[32];
	sprintf(s, "%d", number);
	nvgText(vg, x + 15, y + 30 / 2, s, NULL);
}

void number_io_input_key_func(
	struct node *node,
	int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS && action != GLFW_REPEAT)
		return;

#define change_number(op, value)                  \
	GET_PIN(node, PIN_INPUT, in_number, int)      \
	op value;                                     \
	GET_PIN(node, PIN_OUTPUT, out_number, int) =  \
		GET_PIN(node, PIN_INPUT, in_number, int); \
	direct_call_node_on_pin(node, 0);

	switch (key)
	{
	case GLFW_KEY_UP:
	case GLFW_KEY_KP_8:
		change_number(+=, 1);
		break;

	case GLFW_KEY_DOWN:
	case GLFW_KEY_KP_2:
		change_number(-=, 1);
		break;

	case GLFW_KEY_R:
		change_number(=, 0);
		break;

	default:
		break;
	}
}

void register_library()
{
	//REG_TYPE(int);
	// -> req_type("int", sizeof(int));

	register_function((struct function_note){
		.name = "number_io",
		.init_func = number_io_init,
		.main_func = number_io,
		.draw_func = number_io_draw,
		.input_key_func = number_io_input_key_func});
}