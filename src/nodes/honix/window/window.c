#include <stdlib.h>
#include <stdio.h>

#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include "core/node_api.h"

// WARN // WARN // WARN // WARN
// THIS CODE IS SUPER EXPERIMTAL!

uint8_t windows_count = 0;

struct window_state
{
	GLFWwindow *window;
	// int random_number;
};

static struct window_state *get_state(struct node *node)
{
	return (struct window_state *)node->inner_state;
}

void window_init(struct node *node)
{
	init_pins(node, true, 0, 1);
	REG_PIN(node, PIN_OUTPUT, 1, "window", GLFWwindow *);

	node->inner_state = malloc(sizeof(struct window_state));
	// node->auto_call_next = false;
	// get_state(node)->random_number = rand() % 360;

	windows_count += 1;

	if (!glfwInit())
		return;

#ifndef _WIN32 // don't require this on win32, and works with more cards
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	glfwWindowHint(GLFW_SAMPLES, 4);

	get_state(node)->window = glfwCreateWindow(256, 256, "Window", NULL, NULL);
	glfwSetWindowPos(
		get_state(node)->window, 100 + (windows_count - 1) * 280, 200);
	if (!get_state(node)->window)
	{
		glfwTerminate();
		return;
	}

	// printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());

	glViewport(0, 0, 256, 256);

	// glfwSetKeyCallback(window, key_callback);
	// glfwSetCursorPosCallback(window, cursor_pos_callback);

	// printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());

	direct_call_node_self(node); // draw one frame
}

unsigned int samples = 60;
double prev_time = 0;
double acc = 0;
int t = 0;

void window(struct node *node)
{
	if (++t == samples)
	{
		printf("delta: %f\n", acc / samples);
		t = 0;
		acc = 0;
	}
	double curr_time = current_time_secs();
	acc += curr_time - prev_time;
	prev_time = curr_time;

	// get_state(node)->random_number += 1;

	glfwMakeContextCurrent(get_state(node)->window);

	// Fill window with black
	glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());

	// TEMP
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, -1, 1);

	// DRAW STUFF
	direct_call_node_on_pin(node, 0);

	glfwSwapBuffers(get_state(node)->window);

	// *(GLFWwindow **)get_link_on_pin(node, PIN_OUTPUT, 0)->data = get_state(node)->window;
	GET_PIN(node, PIN_OUTPUT, 1, GLFWwindow *) = get_state(node)->window;
}

void window_deinit(struct node *node)
{
	printf("window_deinit\n");
	glfwDestroyWindow(get_state(node)->window);
	free(node->inner_state);
}

void draw_triangle_init(struct node *node)
{
	init_pins(node, true, 1, 0);
	REG_PIN(node, PIN_INPUT, 1, "rotate", double);
}

void draw_triangle(struct node *node)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(0.5, 0.5, 0.5);
	glRotatef(GET_PIN(node, PIN_INPUT, 1, double) * 360, 0, 0, 1);
	// glRotatef(360, 0, 0, 1);

	// printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());

	glBegin(GL_LINE_LOOP); // glBegin/End depricated in gl >= 3.0
	glColor3f(1, 1, 1);
	glVertex3f(0, 1, 0);
	glVertex3f(1, -1, 0);
	glVertex3f(-1, -1, 0);
	glEnd();

	// direct_call_node_on_pin(node, 0);
	// printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());
}

void set_window_pos_init(struct node *node)
{
	init_pins(node, true, 3, 0);
	REG_PIN(node, PIN_INPUT, 1, "window", GLFWwindow *);
	REG_PIN(node, PIN_INPUT, 2, "x", double);
	REG_PIN(node, PIN_INPUT, 3, "y", double);
}

void set_window_pos(struct node *node)
{
	glfwSetWindowPos(
		GET_PIN(node, PIN_INPUT, 1, GLFWwindow *),
		(int)GET_PIN(node, PIN_INPUT, 2, double),
		(int)GET_PIN(node, PIN_INPUT, 3, double));
}

void register_library()
{
	register_function((struct function_note){
		"window",
		window_init,
		window,
		window_deinit});

	register_function((struct function_note){
		"set_window_pos",
		set_window_pos_init,
		set_window_pos});

	register_function((struct function_note){
		"draw_triangle",
		draw_triangle_init,
		draw_triangle});
}