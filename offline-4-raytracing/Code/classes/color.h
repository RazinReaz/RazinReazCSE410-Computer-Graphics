#pragma once

typedef struct color {
    double r, g, b;
    color(double r = 0, double g = 0, double b = 0) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    color operator+(const color &c);
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

color color::operator+(const color &c) {
    double r = this->r + c.r > 1 ? 1 : this->r + c.r;
    double g = this->g + c.g > 1 ? 1 : this->g + c.g;
    double b = this->b + c.b > 1 ? 1 : this->b + c.b;
    return color(r, g, b);
}