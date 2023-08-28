#pragma once

#include <cassert>
#include <cmath>
#include <vector>
#include "vector3f.h"
#include "utils.h"
#include "color.h"

class triangle {
    int screen_height;
    int screen_width;
    double dx;
    double dy;

    double nudge_towards_centroid_x(double x) {
        return x;

        assert (dx != 0);
        double floor_x = floor(x/dx)*dx;
        double ceil_x = ceil(x/dx)*dx;
        double mid_x = (floor_x + ceil_x) / 2;

        if (abs(centroid.x - mid_x) < abs(centroid.x - x)) //mid_x inside the triangle
            return mid_x;
        
        // if mid_x is not inside the triangle, then either mid_x + dx or mid_x - dx is inside the triangle
        if (abs(centroid.x - (mid_x + dx)) < abs(centroid.x - (mid_x - dx)))
            return mid_x + dx;
        else
            return mid_x - dx;
    }

    double nudge_towards_centroid_y(double y) {
        // return y;

        assert (dy != 0);
        double floor_y = floor(y/dy)*dy ;
        double ceil_y = ceil(y/dy)*dy;
        if (abs(centroid.y - floor_y) < abs(centroid.y - ceil_y)) {
            return floor_y;
        } else {
            return ceil_y;
        }
    }
public:
    vector3f vertices[3];
    vector3f centroid, normal;
    color c;
    triangle(vector3f v1, vector3f v2, vector3f v3, color c) {
        vertices[0] = v1;
        vertices[1] = v2;
        vertices[2] = v3;
        centroid = (v1 + v2 + v3) / 3;
        this->c = c;

        normal = (v2 - v1).cross(v3 - v1);
        normal.normalize();
    }

    void set_screen_height(int height){
        this->screen_height = height;
        this->dy = 2.0 / height;
    }
    void set_screen_width(int width){
        this->screen_width = width;
        this->dx = 2.0 / width;
    }

    void print() {
        std::cout << "Triangle: " << std::endl;
        std::cout << "v1: " << vertices[0] << std::endl;
        std::cout << "v2: " << vertices[1] << std::endl;
        std::cout << "v3: " << vertices[2] << std::endl;
        // std::cout << "color: " << this->c.r << " " <<this->c.g << " " << this->c.b << std::endl;
        std::cout << "centroid: " << this->centroid << std::endl;
        std::cout << std::endl;
    }

    color get_color() {
        return this->c;
    }

    double get_top_scanline() {
        assert ( dy != 0 );
        double top_y = 1.0 - dy/2.0;
        double t_max_y = max(vertices[0].y, vertices[1].y, vertices[2].y);
        t_max_y = nudge_towards_centroid_y(t_max_y);
        double max_y = std::min(top_y, t_max_y);

        return max_y;
    }

    double get_bottom_scanline() {
        assert ( dy != 0 );
        double bottom_y = -1.0 + dy/2.0;
        double t_min_y = min(vertices[0].y, vertices[1].y, vertices[2].y);
        t_min_y = nudge_towards_centroid_y(t_min_y);
        double min_y = std::max(bottom_y, t_min_y);

        return min_y;
    }


    std::vector<double> get_x_range(double y) {
        std::vector<double> x_range;
        const double left_x = -1.0 + dx / 2.0;
        const double right_x = 1.0 - dx / 2.0;

        // assuming that the slope code is correct
        if(is_between(y, vertices[0].y, vertices[1].y)) {
            if(vertices[0].y == vertices[1].y) {
                x_range.push_back(vertices[0].x);
                x_range.push_back(vertices[1].x);
                return x_range;
            }
            double x = vertices[0].x + (y - vertices[0].y) * (vertices[1].x - vertices[0].x) / (vertices[1].y - vertices[0].y);
            x_range.push_back(x);
        }
        if(is_between(y, vertices[1].y, vertices[2].y)) {
            if(vertices[1].y == vertices[2].y) {
                x_range.push_back(vertices[1].x);
                x_range.push_back(vertices[2].x);
                return x_range;
            }
            double x = vertices[1].x + (y - vertices[1].y) * (vertices[2].x - vertices[1].x) / (vertices[2].y - vertices[1].y);
            x_range.push_back(x);
        }
        if(is_between(y, vertices[2].y, vertices[0].y)) {
            if(vertices[2].y == vertices[0].y) {
                x_range.push_back(vertices[2].x);
                x_range.push_back(vertices[0].x);
                return x_range;
            }
            double x = vertices[2].x + (y - vertices[2].y) * (vertices[0].x - vertices[2].x) / (vertices[0].y - vertices[2].y);
            x_range.push_back(x);
        }
        assert (x_range.size() == 2 );
        double a = nudge_towards_centroid_x(x_range[0]);
        double b = nudge_towards_centroid_x(x_range[1]);
        x_range[0] = std::max(std::min(a, b), left_x);
        x_range[1] = std::min(std::max(a, b), right_x);
        return x_range;
    }

    double get_z(double x, double y) {
        double A = normal.x;
        double B = normal.y;
        double C = normal.z;
        double D = -(A*vertices[0].x + B*vertices[0].y + C*vertices[0].z);
        double z = -(A*x + B*y + D) / C;
        return z;
    }
};