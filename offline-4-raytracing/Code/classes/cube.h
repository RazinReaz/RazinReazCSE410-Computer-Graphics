#pragma once

#include <cassert>
#include <gl/glut.h>
#include <cmath>
#include <iostream>

#include "vector3f.h"
#include "color.h"
#include "interfaces.h"

class cube : public shape3d {
    public:
    vector3f position;
    double size;
    color clr;
    std::vector<face_*> faces;
    
    cube(vector3f position, double size, color clr , double ambient, double diffuse, double specular, double reflection, double shininess) : shape3d(ambient, diffuse, specular, reflection, shininess) {
        this->position = position;
        this->size = size;
        this->clr = clr;
        this->faces.push_back(new face_(vector3f(0, 0, 1), position + vector3f(0, 0, size / 2), size));
        this->faces.push_back(new face_(vector3f(0, 0, -1), position + vector3f(0, 0, -size / 2), size));
        this->faces.push_back(new face_(vector3f(0, 1, 0), position + vector3f(0, size / 2, 0), size));
        this->faces.push_back(new face_(vector3f(0, -1, 0), position + vector3f(0, -size / 2, 0), size));
        this->faces.push_back(new face_(vector3f(1, 0, 0), position + vector3f(size / 2, 0, 0), size));
        this->faces.push_back(new face_(vector3f(-1, 0, 0), position + vector3f(-size / 2, 0, 0), size));
    }

    void calculate_hit_distance(ray &r) override {
        double distance;
        for (auto face : this->faces) {
            if (face->is_hit_by_ray(r, distance)) {
                r.set_hit(distance, this, face->normal);
            }
        }
        return;
    }

    vector3f normal_at(vector3f &point) override {
        for (auto face : faces) {
            if (face->contains_point(point)) {
                return face->normal;
            }
        }
        assert (false);
    }

    color get_color_at(vector3f &point) override {
        return clr;
    }

    void show() override
    {
        // glPushMatrix();
        // glTranslated(position.x, position.y, position.z);
        // glColor3f(clr.r, clr.g, clr.b);
        // glutSolidCube(size);
        // glPopMatrix();
        glColor3f(clr.r, clr.g, clr.b);
        for (auto face : faces) {
            face->show();
        }
    }

    void print() override {
        std::cout << "Cube: " << std::endl;
        for (auto face : faces) {
            std::cout << "face with normal " << face->normal << " and mid point " << face->mid_point << std::endl;
        }
        std::cout << std::endl;
    }
};