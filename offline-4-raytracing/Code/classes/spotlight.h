#pragma once

#pragma once

#include <vector>
#include <gl/glut.h>
#include "vector3f.h"
#include "interfaces.h"

class spotlight : public light
{
public:
    double cutoff_angle;
    vector3f direction;

    spotlight(vector3f position, double falloff, double cutoff_angle, vector3f direction) : light(position, falloff)
    {
        this->cutoff_angle = cutoff_angle * PI / 180.0;
        this->direction = direction.normalize();
    }

    bool is_visible_from(vector3f point, vector3f &normal, std::vector<shape3d *> objects)
    {
        vector3f to_source_direction = (position - point).normalize();
        vector3f to_object_direction = to_source_direction * -1.0;

        double angle = acos(to_object_direction.dot(this->direction));
        if (angle > cutoff_angle)
            return false;
        

        double distance_to_source = (position - point).length();
        ray to_source = ray(point + normal * offset, to_source_direction);

        for (auto object : objects)
        {
            object->calculate_hit_distance(to_source);
            if (to_source.hit_info.hit && to_source.hit_info.distance < distance_to_source - offset)
            {
                return false;
            }
        }
        return true;
    }

    void show() {
        //draw a cone
        int slices = 19;
        double height = 20;
        double radius = height * tan(cutoff_angle * PI / 180.0);
        int angle0 = (int)(atan(direction.z / direction.x) * 180 / PI);
        int angle1 = (int)(atan(direction.y / sqrt(direction.x * direction.x + direction.z * direction.z)) * 180 / PI);
        glColor3f(1, 1, 0.5);
        glPushMatrix();
            // glTranslatef(position.x, position.y, position.z);
            glPushMatrix();
                // glRotatef(angle0, 0, 1, 0);
                // glRotatef(45, 0, 0, 1);
                glutSolidCone(radius, height, slices, 10);  
            glPopMatrix();
        glPopMatrix();
    }

    void print() {
        std::cout << "Spotlight: " << std::endl;
        std::cout << "Position: " << this->position << std::endl;
        std::cout << "Falloff: " << this->falloff << std::endl;
        std::cout << "Direction: " << this->direction << std::endl;
        std::cout << "Cutoff angle: " << this->cutoff_angle << std::endl;
    }
};