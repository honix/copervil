#include <stdio.h>

#include <GLFW/glfw3.h>

#include "nanovg/src/nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg/src/nanovg_gl.h"

#include "core/node.h"
#include "core/link.h"
#include "core/loop.h"
#include "core/dl_loader.h"
// #include "../core/utils.h"

extern struct node **nodes;
extern unsigned int nodes_pointer;

const int pin_size = 10;
const int pin_half_size = pin_size / 2;
const int pin_padding = 5;

const float width = pin_padding + (pin_size + pin_padding) * 16;
const float height = 30;

GLFWwindow *window;
struct NVGcontext *vg;

struct vector2i
{
    int x;
    int y;
};

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
    // nodes[1]->x = x; // TEST
    // nodes[1]->y = y;
}

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

    // Draw body
    nvgBeginPath(vg);
    nvgRect(vg, x, y, width, height);
    nvgFillColor(vg, nvgRGBA(255, 192, 0, 255));
    nvgFill(vg);
    nvgLineJoin(vg, NVG_BUTT);
    nvgStrokeWidth(vg, 2);
    nvgStrokeColor(vg, nvgHSLA(0, 0, 0, 100));
    nvgStroke(vg);

    // Draw in pins
    for (int i = 0; i < 16; i++)
    {
        if (! in_pin_is_active(node, i)) continue;

        struct vector2i pin_pos = calc_in_pin_pos(node, i);
        nvgBeginPath(vg);
        nvgRect(vg,
                pin_pos.x, pin_pos.y,
                pin_size, pin_half_size);
        nvgFillColor(vg, nvgHSLA(0, 0, 0, 128));
        nvgFill(vg);

        struct link *in_link = node->in_pins[i];
        if (in_link == NULL)
            continue;

        nvgBeginPath(vg);
        nvgRect(vg,
            pin_pos.x, pin_pos.y - pin_half_size,
            pin_size, pin_half_size);
        nvgFillColor(vg, nvgHSLA(0, 0, 128, 128));
        nvgFill(vg);
    }

    // Draw out pins and out links
    for (int i = 0; i < 16; i++)
    {
        if (! out_pin_is_active(node, i)) continue;

        struct vector2i pin_pos = calc_out_pin_pos(node, i);
        nvgBeginPath(vg);
        nvgRect(vg,
                pin_pos.x, pin_pos.y + pin_half_size,
                pin_size, pin_half_size);
        nvgFillColor(vg, nvgHSLA(0, 0, 0, 128));
        nvgFill(vg);

        // Draw link from this out pin
        struct link *out_link = node->out_pins[i];
        if (out_link == NULL)
            continue;

        if (out_link->receiver == NULL) 
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
        nvgStrokeColor(vg, nvgHSLA(0, 0, 128, 170));
        nvgStroke(vg);
    }

    // Draw node name
    nvgFontSize(vg, 15.0f);
    nvgFontFace(vg, "sans");
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
    nvgText(vg, x + 10, y + height / 2, node->name, NULL);
}

void init()
{
    if (!glfwInit())
        return;

    #ifndef _WIN32 // don't require this on win32, and works with more cards
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

    window = glfwCreateWindow(512, 512, "World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);

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
}

void deinit()
{
    glfwTerminate();
}

void patch_editor_init(struct node *node)
{
    node->in_pins_mask = 1 << 0;

    init();
}

void patch_editor(struct node *node)
{
    glfwMakeContextCurrent(window);

    /* Render here */
    glViewport(0, 0, 512, 512);
    glClearColor(0.25f, 0.25f, 0.25f, 1);
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

    if (glfwWindowShouldClose(window))
    {
        glfwTerminate(); // TEMP
        exit(0);
    }
}

void register_library(reg_function_t reg)
{
    reg("patch_editor", patch_editor_init, patch_editor, NULL, NULL, NULL);
}

// TEST API
// void register_library_custom(reg_custom_t reg)
// {
    // reg("number_io", 
    //     numner_io_init, number_io, number_io_deinit, 
    //     number_io_draw, number_io_input);
// }