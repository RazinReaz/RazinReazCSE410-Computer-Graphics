#pragma once

typedef struct color {
    float r, g, b;
    color(float r = 0, float g = 0, float b = 0) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
} color;

// enum to define colors
enum colors {
    RED, GREEN, BLUE, YELLOW, PURPLE, CYAN
};

// array of colors
color color_array[] = {
    color(1, 0, 0), // red
    color(0, 1, 0), // green
    color(0, 0, 1), // blue
    color(1, 1, 0), // yellow
    color(1, 0, 1),  // purple
    color(0, 1, 1)  // cyan

};