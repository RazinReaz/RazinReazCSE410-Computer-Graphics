#pragma once

#include <gl/glut.h>
#include <fstream>
#include <iostream>
#include "vector3f.h"
#include "color.h"
#include "interfaces.h"



class checkerboard : public shape3d
{
    const int i_limit = 100, j_limit = 100;
public:
    int size;
    checkerboard(int size, double ambient, double diffuse, double reflection) : shape3d(ambient, diffuse, 0, reflection, 0){
        this->size = size;
    }

    void calculate_hit_distance(ray& r) override {
        double hit_distance = -1.0 * r.origin.y / r.direction.y;
        if (hit_distance < 0)
            return;
        // cout << "hit ditance is not negative" << endl;
        vector3f hit_point = r.origin + r.direction * hit_distance;
        r.set_hit(hit_distance, this, vector3f(0, 1, 0));
    }

    vector3f normal_at(vector3f& point) override {
        // if (camera.get_direction().z > 0)
        //     return vector3f(0, -1, 0);
        return vector3f(0, 1, 0);
    }

    color get_color_at(vector3f &point) override {
        int i = (int) (point.x + i_limit * this->size) / this->size; // don't optimize this
        int j = (int) (point.z + j_limit * this->size) / this->size; // don't optimize this
        if ((i + j) % 2 == 0)
            return color(1, 1, 1);
        else
            return color(0, 0, 0);
    }

    void show() override {
        for (int i = -i_limit; i < i_limit; i++) {
            for (int j = -j_limit; j < j_limit; j++) {
                if ((i + j) % 2 == 0) 
                    glColor3f(1, 1, 1);
                else 
                    glColor3f(0, 0, 0);
                glBegin(GL_QUADS);
                glVertex3f(i * this->size, 0, j * this->size);
                glVertex3f((i + 1) * this->size, 0, j * this->size);
                glVertex3f((i + 1) * this->size, 0, (j + 1) * this->size);
                glVertex3f(i * this->size, 0, (j + 1) * this->size);
                glEnd();
            }
        }
    }

    void print() override {
        std::cout << "Checkerboard: " << std::endl;
        std::cout << "Size: " << this->size << std::endl;
        std::cout << "Ambient: " << this->ambient << std::endl;
        std::cout << "Diffuse: " << this->diffuse << std::endl;
        std::cout << "Reflection: " << this->reflection << std::endl;
    }
};