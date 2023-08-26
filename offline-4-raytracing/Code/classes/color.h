#pragma once

typedef struct color {
    double r, g, b;
    color(double r = 0, double g = 0, double b = 0) {
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