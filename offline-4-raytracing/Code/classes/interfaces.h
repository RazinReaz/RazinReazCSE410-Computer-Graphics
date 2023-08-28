#pragma once
#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include "vector3f.h"
#include "color.h"


class ray;
class shape3d;

class light
{
public:
    vector3f position;
    double falloff;

    light(vector3f position, double falloff)
    {
        this->position = position;
        this->falloff = falloff;
    }
    virtual bool is_visible_from(vector3f point, ray& r, std::vector<shape3d *> shape) = 0;
    virtual void show() = 0;
};

typedef struct Hit_info
{
    bool hit;
    shape3d *object;
    vector3f normal;
    double distance;
} Hit_info;

class ray {
public:
    vector3f origin;
    vector3f direction;
    Hit_info hit_info;

    ray(vector3f origin, vector3f direction)
    {
        this->origin = origin;
        this->direction = direction.normalize();
        this->hit_info.distance = 1000000.0;
        this->hit_info.hit = false;
        this->hit_info.normal = vector3f(0, 0, 0);
        this->hit_info.object = NULL;
    }

    ray reflect(const vector3f &reflection_point, vector3f &normal);
    void set_hit(double hit_distance, shape3d *object, vector3f normal);
    void show();
    // assignment operator
    ray &operator=(const ray &r);
};

ray ray::reflect(const vector3f &reflection_point, vector3f &normal)
{
    vector3f reflected_direction = this->direction - normal * 2.0 * this->direction.dot(normal);
    return ray(reflection_point, reflected_direction);
};

void ray::set_hit(double hit_distance, shape3d *object, vector3f normal)
{
    if (hit_distance < 0)
        return;
    if (this->hit_info.distance <= hit_distance)
        return;
    this->hit_info.distance = hit_distance;
    this->hit_info.hit = true;
    this->hit_info.object = object;
    this->hit_info.normal = normal;
    return;
};

void ray::show()
{
    glPushMatrix();
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    glLineWidth(10);
    glVertex3f(origin.x, origin.y, origin.z);
    glVertex3f(origin.x + direction.x, origin.y + direction.y, origin.z + direction.z);
    glEnd();
    glPopMatrix();
};

ray &ray::operator=(const ray &r)
{
    this->origin = r.origin;
    this->direction = r.direction;
    this->hit_info = r.hit_info;
    return *this;
};

class shape3d
{
public:
    double ambient, diffuse, specular, reflection, shininess;
    shape3d(double ambient, double diffuse, double specular, double reflection, int shininess)
    {
        this->ambient = ambient;
        this->diffuse = diffuse;
        this->specular = specular;
        this->reflection = reflection;
        this->shininess = shininess;
    }
    // virtual bool is_ray_intersecting(ray& r) = 0;

    virtual void calculate_hit_distance(ray &r) = 0;

    virtual vector3f normal_at(vector3f &point) = 0; // assumes that the point is always on the shape
    virtual color get_color_at(vector3f point) = 0;  // assumes that the point is always on the shape
    color get_diffuse_and_specular_color(vector3f point, ray& r, std::vector<light *> lights, std::vector<shape3d *> objects);
    color get_reflection_color(vector3f point, ray &r, std::vector<light *> lights, std::vector<shape3d *> objects, int recursions);

    virtual void show() = 0;

    // debug
    virtual void print() = 0;
};


color shape3d::get_diffuse_and_specular_color(vector3f point, ray& r, std::vector<light *> lights, std::vector<shape3d *> objects)
{
    // assumes that the point is always on the shape
    double lambert = 0,
           phong = 0;
    vector3f N = this->normal_at(point);
    vector3f R = r.reflect(point, N).direction;
    
    for (auto light : lights)
    {
        if (!light->is_visible_from(point, r, objects))
        {
            continue;
        }
        vector3f to_source = (light->position - point).normalize();
        double distance = (light->position - point).length();
        double scaling_factor = exp(-1 * distance * distance * light->falloff);

        // diffuse component
        lambert += scaling_factor * std::max(0.0, N.dot(to_source));

        // specular component
        phong += pow(R.dot(to_source), shininess) * scaling_factor;
    }
    color object_color = get_color_at(point);

    double diffuse_component = this->diffuse * lambert;
    double specular_component = this->specular * phong;
    return object_color * (this->ambient + diffuse_component + specular_component);
};

color shape3d::get_reflection_color(vector3f point, ray &r, std::vector<light *> lights, std::vector<shape3d *> objects, int recursions){
    ray current_ray = r;
    double offset = 1.0, multiplier = 1.0;
    color reflected;
    while(--recursions) {
        vector3f reflection_point = current_ray.origin + current_ray.direction * current_ray.hit_info.distance;
        vector3f normal = current_ray.hit_info.normal;
        current_ray = current_ray.reflect(reflection_point, normal);
        current_ray.origin = current_ray.origin + current_ray.direction * offset;
        for (auto object : objects) {
            object->calculate_hit_distance(current_ray);
        }
        if (!current_ray.hit_info.hit){
            return color(0, 0, 0);
        }
        vector3f final_point = current_ray.origin + current_ray.direction * current_ray.hit_info.distance;
        reflected += current_ray.hit_info.object->get_diffuse_and_specular_color(final_point, current_ray, lights, objects) * multiplier;
        multiplier *= current_ray.hit_info.object->reflection;
    }
    // vector3f final_point = current_ray.origin + current_ray.direction *current_ray.hit_info.distance;
    // color reflected_ = current_ray.hit_info.object->get_diffuse_and_specular_color(final_point, current_ray, lights, objects );
    return reflected * this->reflection;
}
