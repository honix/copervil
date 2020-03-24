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

void make_window_init(struct node *node)
{
    windows_count += 1;

    node->in_pins_mask = 1 << 0; // trigger
    node->out_pins_mask = 0b0000000000000001;

    // TODO: ugh I want better node storage than unused in pins pointers
    node->in_pins[14] = make_link(malloc(sizeof(GLFWwindow *)));
    node->in_pins[15] = make_link(malloc(sizeof(int))); // cozy API :(
    * (int *) node->in_pins[15]->data = rand() % 360;

    /* Initialize the library */
    if (!glfwInit())
        return;

    #ifndef _WIN32 // don't require this on win32, and works with more cards
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    #endif
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

    glfwWindowHint(GLFW_SAMPLES, 4);

    /* Create a windowed mode window and its OpenGL context */
    node->in_pins[14]->data = glfwCreateWindow(256, 256, "Window", NULL, NULL);
    glfwSetWindowPos(node->in_pins[14]->data, 100 + (windows_count - 1) * 280, 200);
    if (!(GLFWwindow *) node->in_pins[14]->data)
    {
        glfwTerminate();
        return;
    }

    
    // printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());

    glViewport(0, 0, 256, 256);

    // glfwSetKeyCallback(window, key_callback);
    // glfwSetCursorPosCallback(window, cursor_pos_callback);

    // printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());
}

void make_window(struct node *node)
{
    * (int *) node->in_pins[15]->data += 1;
    
    /* Make the window's context current */
    glfwMakeContextCurrent((GLFWwindow *) node->in_pins[14]->data);

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
    glRotatef(* (int *) node->in_pins[15]->data * 2, 0, 0, 1);

    // printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());

    glBegin(GL_TRIANGLES); // glBegin/End depricated in gl >= 3.0
        glColor3f(1, 1, 1);
		glVertex3f(0,1,0);
		glVertex3f(1,-1,0);
		glVertex3f(-1,-1,0);
	glEnd();

    // printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());

    glfwSwapBuffers((GLFWwindow *) node->in_pins[14]->data);

    node->out_pins[0]->data = (GLFWwindow *) node->in_pins[14]->data;
}

void register_library(reg_function_t reg)
{
	reg("make_window", make_window_init, make_window, NULL, NULL, NULL);
}