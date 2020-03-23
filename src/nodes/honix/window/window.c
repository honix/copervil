#include <stdio.h>

#include <GLFW/glfw3.h>
#include <GL/gl.h>


#include "core/node.h"
#include "core/link.h"
#include "core/dl_loader.h"

// TEMP
// #include <GL/glut.h>
//

GLFWwindow *window;

void make_window_init(struct node *node)
{
    node->out_pins_mask = 0b0000000000000001;
}

void make_window(struct node *node)
{
    /* Initialize the library */
    if (!glfwInit())
        return;

    #ifndef _WIN32 // don't require this on win32, and works with more cards
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    #endif
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(256, 256, "Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());

    glViewport(0, 0, 256, 256);

    // glfwSetKeyCallback(window, key_callback);
    // glfwSetCursorPosCallback(window, cursor_pos_callback);

    printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());
    
    // Fill window with black
    glClearColor(0.5, 0.5, 0.5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());

    // TEMP
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());

    glBegin(GL_TRIANGLES); // glBegin/End depricated in gl >= 3.0
        glColor3f(1, 1, 1);
		glVertex3f(0,1,0);
		glVertex3f(1,-1,0);
		glVertex3f(-1,-1,0);
	glEnd();
    //

    printf("GL error at %s:%d: %x\n", __FILE__, __LINE__, glGetError());

    glfwSwapBuffers(window);

    node->out_pins[0]->data = window;
}

void register_library(reg_function_t reg)
{
	reg("make_window", make_window_init, make_window, NULL);
}