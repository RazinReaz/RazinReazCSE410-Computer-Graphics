#pragma once
#include <cmath>
#include "point.h"
#include <GL\glut.h>

#define PI acos(-1.0)

void circle2D(point centre, double radius, int strokeWidth = 1, bool fill = false, int segments=100)
{
    glLineWidth(strokeWidth);
    if (fill)
        glBegin(GL_POLYGON);
    else
        glBegin(GL_LINE_LOOP);
        
    for (int i = 0; i < segments; i++)
    {
        double theta = 2.0 * PI * double(i) / double(segments);
        double x = radius * cos(theta);
        double y = radius * sin(theta);
        glVertex2f(centre.x + x, centre.y + y);
    }
    glEnd();
}

void line2D(point a, point b, int strokeWidth = 1) {
    glLineWidth(strokeWidth);
    glBegin(GL_LINES);
    {
        glVertex2f(a.x, a.y);
        glVertex2f(b.x, b.y);
    }
    glEnd();
}