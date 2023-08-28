#pragma once

#include <iostream>
#include "color.h"


static unsigned long int g_seed = 1;
inline int random()
{
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

color get_random_color() {
    int r = random() % 256;
    int g = random() % 256;
    int b = random() % 256;
    return color(r, g, b);
}

double min(double a, double b, double c) {
    return std::min(a, std::min(b, c));
}

double max(double a, double b, double c) {
    return std::max(a, std::max(b, c));
}

//!untested
inline int get_row_from_y(double y, int screen_height) {
    return screen_height - std::round((y + 1.0) * screen_height / 2.0) - 1;
}

inline int get_col_from_x(double x, int screen_width) {
    //! bhai ei part e integer e cast korar shomoy onek pera khaite hoise
    // std::cout << "x: " << x << std::endl;
    // std::cout << "x+1: " << x+1 << std::endl;
    // std::cout << "(x+1)*screen width: " << (x+1)*screen_width << std::endl;
    // std::cout << "(x+1)*screen width/2: " << int(std::round((x+1)*screen_width/2.0)) << std::endl;
    return std::round((x + 1) * screen_width / 2.0);
}

bool is_between(double query, double a, double b) {
    double mini = std::min(a, b);
    double maxi = std::max(a, b);

    return (query >= mini && query <= maxi);
}

