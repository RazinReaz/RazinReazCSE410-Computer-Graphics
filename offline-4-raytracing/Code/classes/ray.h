#pragma once

#include "vector3f.h"
#include "shape3d.h"

class shape3d;

typedef struct Hit_info {
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

    ray(vector3f origin, vector3f direction) {
        this->origin = origin;
        this->direction = direction.normalize();
        this->hit_info.distance = 100000.0;
        this->hit_info.hit = false;
        this->hit_info.normal = vector3f(0, 0, 0);
        this->hit_info.object = NULL;
    }

    ray reflect (const vector3f& reflection_point, vector3f& normal) {
        vector3f reflected_direction = this->direction - normal * 2.0 * this->direction.dot(normal);
        return ray(reflection_point, reflected_direction);
    }

    void set_hit(double hit_distance, shape3d *object, vector3f normal) {
        if (hit_distance < 0)
            return;
        if (this->hit_info.distance <= hit_distance)
            return;
        this->hit_info.distance = hit_distance;
        this->hit_info.hit = true;
        this->hit_info.object = object;
        this->hit_info.normal = normal;
    }

    void show() {
        glPushMatrix();
        glBegin(GL_LINES);
        glColor3f(1, 1, 1);
        glLineWidth(10);
        glVertex3f(origin.x, origin.y, origin.z);
        glVertex3f(origin.x + direction.x, origin.y + direction.y, origin.z + direction.z);
        glEnd();
        glPopMatrix();
    }
};