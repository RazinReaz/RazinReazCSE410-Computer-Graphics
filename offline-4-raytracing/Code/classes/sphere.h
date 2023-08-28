#pragma once

#include <cassert>
#include <gl/glut.h>
#include "vector3f.h"
#include "color.h"
#include "interfaces.h"

using namespace std;

class sphere : public shape3d
{
private:
    bool is_ray_inside(ray& r)
    {
        return (this->center - r.origin).length() < this->radius;
    }
    bool is_ray_outside(ray& r)
    {
        return (this->center - r.origin).length() > this->radius;
    }
    double closest_point_on_ray(ray& r)
    {
        return (this->center - r.origin).dot(r.direction);
    }

public:
    vector3f center;
    double radius;
    color clr;
    double specular, shininess;
    sphere(vector3f center, double radius, color c, double ambient, double diffuse, double specular, double reflection, double shininess) : shape3d(ambient, diffuse, specular, reflection, shininess)
    {
        this->center = center;
        this->radius = radius;
        this->clr = c;
    }
    bool is_ray_intersecting(ray& r) {
        bool ray_origin_inside = is_ray_inside(r);
        bool ray_origin_outside = is_ray_outside(r);
        double closest_t_from_center_to_ray = closest_point_on_ray(r);
        if (ray_origin_outside && closest_t_from_center_to_ray < 0)
            return false;
        vector3f otibhuj = r.origin - this->center;
        double distance_from_center_to_ray_squared = otibhuj.dot(otibhuj) - closest_t_from_center_to_ray * closest_t_from_center_to_ray;
        if (distance_from_center_to_ray_squared > this->radius * this->radius)
            return false;
        return true;
    }

    void calculate_hit_distance(ray& r)
    {
        double a = r.direction.dot(r.direction);
        double b = 2 * r.direction.dot(r.origin - this->center);
        double c = (r.origin - this->center).dot(r.origin - this->center) - this->radius * this->radius;
        double discriminant = b * b - 4 * a * c;
        if (discriminant < 0)
            return;
        // cout << "ray hits the sphere " << this->center << endl;
        double t1 = (-b + sqrt(discriminant)) / (2 * a);
        double t2 = (-b - sqrt(discriminant)) / (2 * a);

        double t = min(t1, t2);
        vector3f hit_point = r.origin + r.direction * t;
        vector3f normal = normal_at(hit_point);
        r.set_hit(min(t1,t2), this, normal);
        return;
    }

    vector3f normal_at(vector3f& point)
    {
        return (point - this->center).normalize();
    }

    color get_color_at(vector3f point)
    {
        return this->clr;
    }


    void show()
    {
        // cout << "showing sphere " << this->center << endl;
        int stacks = 100, slices = 100;
        vector3f vertices[stacks + 1][slices + 1];

        for (int j = 0; j <= stacks; j++) {
            double phi = PI / 2 - j * PI / stacks;
            double y = this->center.y + radius * sin(phi);

            for (int i = 0; i <= slices; i++) {
                double theta = i * 2 * PI / slices;
                double x = this->center.x + radius * cos(phi) * cos(theta);
                double z = this->center.z + radius * cos(phi) * sin(theta);
                vertices[j][i] = vector3f(x, y, z);
            }
        }

        for (int j = 0; j < stacks; j++) { 
            glBegin(GL_QUADS);
            for (int i = 0; i < slices; i++) { 
                glColor3f(this->clr.r, this->clr.g, this->clr.b);
                glVertex3f(vertices[j][i].x, vertices[j][i].y, vertices[j][i].z);
                glVertex3f(vertices[j][i + 1].x, vertices[j][i + 1].y, vertices[j][i + 1].z);
                glVertex3f(vertices[j + 1][i + 1].x, vertices[j + 1][i + 1].y, vertices[j + 1][i + 1].z);
                glVertex3f(vertices[j + 1][i].x, vertices[j + 1][i].y, vertices[j + 1][i].z);
            }
            glEnd();
        }
    }

    void print()
    {
        cout << "Sphere: " << endl;
        cout << "Center: " << this->center << endl;
        cout << "Radius: " << this->radius << endl;
        cout << "Color: " << this->clr.r << ", " << this->clr.g << ", " << this->clr.b << endl;
        cout << "Ambient: " << this->ambient << endl;
        cout << "Diffuse: " << this->diffuse << endl;
        cout << "Specular: " << this->specular << endl;
        cout << "Reflection: " << this->reflection << endl;
        cout << "Shininess: " << this->shininess << endl;
    }
    
    void change_color(color c)
    {
        this->clr = c;
    }

    ~sphere()
    {
        cerr << "Sphere destructor called" << endl;
    }
};