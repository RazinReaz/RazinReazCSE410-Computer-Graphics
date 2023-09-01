#pragma once

#include <vector>
#include <cassert>
#include <gl/glut.h>
#include "vector3f.h"
#include "interfaces.h"



class point_light : public light {
    public:
    point_light(vector3f position, double falloff) : light(position, falloff) {
    }

    bool is_visible_from(vector3f point, vector3f &normal, std::vector<shape3d*> objects) {
        vector3f direction = (position - point).normalize();
        double distance_to_source = (position - point).length();
        ray to_source = ray(point + normal * offset, direction);
        for(auto object : objects) {
            object->calculate_hit_distance(to_source);
            // assert (to_source.hit_info.object != this);
            if(to_source.hit_info.hit && to_source.hit_info.distance < distance_to_source - offset) {
                return false;
            }
        }
        return true;
    }

    void show() {
        const double light_radius = 5.0;
        int stacks = 50, slices = 50;
        vector3f vertices[stacks + 1][slices + 1];

        for (int j = 0; j <= stacks; j++)
        {
            double phi = PI / 2 - j * PI / stacks;
            double y = light_radius * sin(phi);

            for (int i = 0; i <= slices; i++)
            {
                double theta = i * 2 * PI / slices;
                double x = light_radius * cos(phi) * cos(theta);
                double z = light_radius * cos(phi) * sin(theta);
                vertices[j][i] = vector3f(x, y, z);
            }
        }
        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        for (int j = 0; j < stacks; j++)
        {
            double c = cos(PI / 2 - j * PI / stacks);
            glBegin(GL_QUADS);
            for (int i = 0; i < slices; i++)
            {
                glColor3f(c,c,c);
                glVertex3f(vertices[j][i].x, vertices[j][i].y, vertices[j][i].z);
                glVertex3f(vertices[j][i + 1].x, vertices[j][i + 1].y, vertices[j][i + 1].z);
                glVertex3f(vertices[j + 1][i + 1].x, vertices[j + 1][i + 1].y, vertices[j + 1][i + 1].z);
                glVertex3f(vertices[j + 1][i].x, vertices[j + 1][i].y, vertices[j + 1][i].z);
            }
            glEnd();
        }
        glPopMatrix();
    }

    void print() {
        std::cout << "Point Light: " << std::endl;
        std::cout << "Position: " << this->position << std::endl;
        std::cout << "Falloff: " << this->falloff << std::endl << std::endl;
    }

    ~point_light() {
    }
};