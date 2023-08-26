#pragma once
#include "vector3f.h"
#include "ray.h"

class ray;

class shape3d {
public:

    double ambient, diffuse, reflection;
    shape3d(double ambient, double diffuse, double reflection) {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->reflection = reflection;
    }
    virtual bool is_ray_intersecting(ray& r) = 0;
    // virtual vector3f get_ray_intersection(ray r) = 0;
    virtual void calculate_hit_distance(ray& r) = 0;
    virtual vector3f normal_at(vector3f& point) = 0; //assumes that the point is always on the shape
    virtual void show() = 0;

    // debug
    virtual void print() = 0;
    virtual void change_color(color c) = 0;
    
};