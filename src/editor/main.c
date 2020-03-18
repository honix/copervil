#include <GLFW/glfw3.h>
#include <stdio.h>

#include "../../thirdparty/nanovg/src/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "../../thirdparty/nanovg/src/nanovg_gl.h"

#include "../core/node.h"
#include "../core/link.h"
#include "../core/loop.h"
#include "../core/utils.h"

extern struct node **nodes;
extern unsigned int nodes_pointer;

void key_callback(
    GLFWwindow *window, int key,
    int scancode, int action, int mods)
{
    // printf("key_callback: %s %d %d %d %d\n",
    //     glfwGetKeyName(key, scancode),
    //     key, scancode, action, mods);
    switch (key)
    {
    case GLFW_KEY_Q:
    case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void cursor_pos_callback(GLFWwindow *window, double x, double y)
{
    // printf("cursor_pos_callback: %d %d\n", (int)x, (int)y);
    nodes[1]->x = x; // TEST
    nodes[1]->y = y;
}

const int pin_size = 10;
const int pin_half_size = pin_size / 2;
const int pin_padding = 5;

const float width = pin_padding + (pin_size + pin_padding) * 16;
const float height = 45;

struct vector2i
{
    int x;
    int y;
};

struct vector2i calc_in_pin_pos(struct node *node, unsigned char pin)
{
    struct vector2i vec;
    vec.x = node->x + pin_padding + (pin_size + pin_padding) * pin;
    vec.y = node->y;
    return vec;
}

struct vector2i calc_out_pin_pos(struct node *node, unsigned char pin)
{
    struct vector2i vec;
    vec.x = node->x + pin_padding + (pin_size + pin_padding) * pin;
    vec.y = node->y + height - pin_size;
    return vec;
}

void draw_node(struct NVGcontext *vg, struct node *node)
{

    int x = node->x;
    int y = node->y;

    nvgBeginPath(vg);
    nvgRect(vg, x, y, width, height);
    nvgFillColor(vg, nvgRGBA(255, 192, 0, 255));
    nvgFill(vg);
    nvgLineJoin(vg, NVG_BUTT);
    nvgStrokeWidth(vg, 1);
    nvgStrokeColor(vg, nvgHSLA(0, 0, 0, 100));
    nvgStroke(vg);

    nvgFillColor(vg, nvgRGBA(0, 0, 0, 128));
    for (int i = 0; i < 16; i++)
    {
        struct vector2i pin_pos = calc_in_pin_pos(node, i);
        nvgBeginPath(vg);
        nvgRect(vg,
                pin_pos.x, pin_pos.y,
                pin_size, pin_size);
        nvgFill(vg);
    }

    for (int i = 0; i < 16; i++)
    {
        struct vector2i pin_pos = calc_out_pin_pos(node, i);
        nvgBeginPath(vg);
        nvgRect(vg,
                pin_pos.x, pin_pos.y,
                pin_size, pin_size);
        nvgFill(vg);

        struct link *out_link = node->out_pins[i];
        if (out_link == NULL)
            continue;

        struct vector2i other_pin_pos = calc_in_pin_pos(
            out_link->receiver,
            out_link->receiver_pin);
        nvgBeginPath(vg);
        nvgMoveTo(vg, pin_pos.x + pin_half_size, pin_pos.y + pin_size);
        nvgLineTo(vg, pin_pos.x + pin_half_size, pin_pos.y + pin_size * 2);
        nvgLineTo(vg, other_pin_pos.x + pin_half_size, other_pin_pos.y - pin_size);
        nvgLineTo(vg, other_pin_pos.x + pin_half_size, other_pin_pos.y);

        nvgLineJoin(vg, NVG_ROUND);
        nvgStrokeWidth(vg, 3);
        nvgStrokeColor(vg, nvgHSLA(0, 0, 0, 170));
        nvgStroke(vg);
    }

    nvgFontSize(vg, 15.0f);
    nvgFontFace(vg, "sans");
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
    nvgText(vg, x + 10, y + height / 2, node->name, NULL);
}

void test_node_setup()
{
    init_nodes();

    int *h1 = malloc(sizeof(int));
    int *h2 = malloc(sizeof(int));
    *h1 = 3;
    *h2 = 0;

    // struct node *node1 = make_node("node1", do_times);
    struct node *node1 = make_node("do_times_inderect", 100, 100, do_times_inderect);
    connect_nodes(make_link(h1), NULL, 0, node1, 0);

    struct node *node2 = make_node("print_int", 200, 200, print_int);
    connect_nodes(make_link(h2), node1, 0, node2, 0);

    direct_call_node(node1);
}

int main(void)
{
    GLFWwindow *window;
    struct NVGcontext *vg;

    struct node *node;

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

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);

    vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
    if (vg == NULL)
    {
        printf("Could not init nanovg.\n");
        return -1;
    }

    nvgCreateFont(vg, "sans", "./assets/Roboto-Regular.ttf");

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_CULL_FACE);
    // glDisable(GL_DEPTH_TEST);
    // end initialization

    test_node_setup();
    loop_init();

    int frame = 0;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        frame++;
        loop_step(); // TODO: maybe loop will push rendering frame too?

        /* Render here */
        glViewport(0, 0, 512, 512);
        glClearColor(0.5f, 0.5f, 0.5f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        nvgBeginFrame(vg, 512, 512, 1);

        for (int i = 0; i < nodes_pointer; i++)
        {
            draw_node(vg, nodes[i]);
        }

        nvgEndFrame(vg);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}