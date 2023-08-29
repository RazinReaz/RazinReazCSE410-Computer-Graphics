#pragma once

typedef struct color {
    double r, g, b;
    color(double r = 0, double g = 0, double b = 0) {
        this->r = r;
        this->g = g;
        this->b = b;
    }
    color operator+(const color &c) const;
    color operator+=(const color &c);
    color operator*(const double &d) const;
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

color color::operator+(const color &c) const {
    double r = this->r + c.r > 1 ? 1 : this->r + c.r;
    double g = this->g + c.g > 1 ? 1 : this->g + c.g;
    double b = this->b + c.b > 1 ? 1 : this->b + c.b;
    return color(r, g, b);
}

color color::operator+=(const color &c) {
    this->r = this->r + c.r > 1 ? 1 : this->r + c.r;
    this->g = this->g + c.g > 1 ? 1 : this->g + c.g;
    this->b = this->b + c.b > 1 ? 1 : this->b + c.b;
    return *this;
}

color color::operator*(const double &d) const {
    double r = this->r * d > 1 ? 1 : this->r * d;
    double g = this->g * d > 1 ? 1 : this->g * d;
    double b = this->b * d > 1 ? 1 : this->b * d;
    return color(r, g, b);
}