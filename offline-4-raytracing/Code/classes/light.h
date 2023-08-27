#pragma once

#include <vector>
#include "vector3f.h"
#include "ray.h"
#include "shape3d.h"

class ray;
class shape3d;

using namespace std;
class light {
    public:
    vector3f position;
    double falloff;

    light(vector3f position, double falloff) {
        this->position = position;
        this->falloff = falloff;
    }

    virtual bool is_visible_from(vector3f point, vector<shape3d*> shape) = 0;
    virtual void show() = 0;
};