#pragma once

#include <vector>
#include <gl/glut.h>
#include "vector3f.h"
#include "ray.h"
#include "shape3d.h"
#include "light.h"


class point_light : public light {
    public:
    point_light(vector3f position, double falloff) : light(position, falloff) {
    }

    bool is_visible_from(vector3f point, vector<shape3d*> shapes) {
        vector3f direction = (position - point).normalize();
        const double offset = 0.01;
        ray r = ray(point + direction * offset, direction);
        for(int i = 0; i < shapes.size(); i++) {
            shapes[i]->calculate_hit_distance(r);
            if(r.hit_info.hit) {
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

    ~point_light() {
    }
};