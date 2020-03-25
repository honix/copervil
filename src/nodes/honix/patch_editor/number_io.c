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

void number_io(struct node *node)
{

}

void number_io_draw(struct NVGcontext *vg, struct node *node)
{
    int x = node->rect.pos.x;
    int y = node->rect.pos.y;

    int number = *(int *)node->in_pins[0]->data;

    // Draw node name
    nvgFontSize(vg, 15.0f);
    nvgFontFace(vg, "sans");
    nvgTextAlign(vg, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
    nvgFillColor(vg, nvgRGBA(0, 0, 0, 255));
    char s[32];
    sprintf(s, "%d", number);
    nvgText(vg, x + 15, y + 30 / 2, s, NULL);
}

void register_library(reg_function_t reg)
{
    reg("number_io", NULL, number_io, NULL, number_io_draw, NULL);
}