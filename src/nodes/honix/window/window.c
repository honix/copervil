#include <stdlib.h>
#include <stdio.h>

#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include "core/node.h"
#include "core/link.h"
#include "core/dl_loader.h"

// WARN // WARN // WARN // WARN
// THIS CODE IS SUPER EXPERIMTAL!

uint8_t windows_count = 0;

struct make_window_state
{
	GLFWwindow *window;
	// int random_number;
};

static struct make_window_state *get_state(struct node *node)
{
	return (struct make_window_state *)node->inner_state;
}

void make_window_init(struct node *node)
{
	init_pins(node, 2, 1);
	reg_pin(node, PIN_INPUT, 0, "trigger", "trigger");
	reg_pin(node, PIN_INPUT, 1, "rotate", "double");
	reg_pin(node, PIN_OUTPUT, 0, "window", "GLFWWindow *");

	node->inner_state = malloc(sizeof(struct make_window_state));
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

	direct_call_node(node); // draw one frame
}

void make_window(struct node *node)
{
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
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(0.5, 0.5, 0.5);
	glRotatef(*(double *)get_link_on_pin(node, PIN_INPUT, 1)->data * 360, 0, 0, 1);

	// printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());

	glBegin(GL_TRIANGLES); // glBegin/End depricated in gl >= 3.0
	glColor3f(1, 1, 1);
	glVertex3f(0, 1, 0);
	glVertex3f(1, -1, 0);
	glVertex3f(-1, -1, 0);
	glEnd();

	// printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());

	glfwSwapBuffers(get_state(node)->window);

	*(GLFWwindow **)get_link_on_pin(node, PIN_OUTPUT, 0)->data = get_state(node)->window;
}

void make_window_deinit(struct node *node)
{
	printf("make_window_deinit\n");
	glfwDestroyWindow(get_state(node)->window);
	free(node->inner_state);
}

void register_library(reg_function_t reg)
{
	reg((struct function_note){
		"make_window",
		make_window_init,
		make_window,
		make_window_deinit});
}