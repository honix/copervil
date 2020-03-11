#include <GLFW/glfw3.h>

#include "../../nanovg/src/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "../../nanovg/src/nanovg_gl.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

#ifndef _WIN32 // don't require this on win32, and works with more cards
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

#ifdef DEMO_MSAA
	glfwWindowHint(GLFW_SAMPLES, 4);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(512, 512, "World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    struct NVGcontext* vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    if (vg == NULL) {
		printf("Could not init nanovg.\n");
		return -1;
	}

    int frame = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        frame++;

        /* Render here */
        glViewport(0, 0, 512, 512);
        glClearColor(0.1f, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

        nvgBeginFrame(vg, 512, 512, 1);

        nvgBeginPath(vg);
        nvgRect(vg, 100 + (sin(frame * 0.1f) * 10),100, 120,30);
        nvgFillColor(vg, nvgRGBA(255,192,0,255));
        nvgFill(vg);

        nvgEndFrame(vg);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}