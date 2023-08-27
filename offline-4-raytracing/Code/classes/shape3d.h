#pragma once
#include "vector3f.h"
#include "ray.h"
#include "color.h"

class ray;
class shape3d {
public:

    double ambient, diffuse, reflection;
    shape3d(double ambient, double diffuse, double reflection) {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->reflection = reflection;
    }
    // virtual bool is_ray_intersecting(ray& r) = 0;

    //this function is for one iteration. if it hits, it will set the hit_info of the ray
    virtual void calculate_hit_distance(ray& r) = 0;
    
    virtual vector3f normal_at(vector3f& point) = 0; //assumes that the point is always on the shape
    virtual color get_color_at(vector3f point)  = 0; //assumes that the point is always on the shape
    // virtual color get_illuminated_color(vector3f point){
    //     // assumes that the point is always on the shape
    //     vector3f N = normal_at(point);
        

    // } 
    virtual void show() = 0;

    // debug
    virtual void print() = 0;
};