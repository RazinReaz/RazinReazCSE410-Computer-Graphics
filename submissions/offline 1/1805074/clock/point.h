#pragma once
#include <GL\glut.h>
class point {
public:
    double x, y, z;
    double size;
    point(double x = 0, double y = 0, double z = 0) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->size = 1;
    }

    void render() {
        glPointSize(size);
        glBegin(GL_POINTS);
        {
            glVertex3f(x, y, z);
        }
        glEnd();
    }
};