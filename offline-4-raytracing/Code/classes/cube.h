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

    typedef struct face_ {
        // all faces are parallel to either xy, yz or zx plane
        vector3f normal;
        vector3f mid_point;
        double size;

        face_(){
            normal = vector3f(0, 0, 0);
            mid_point = vector3f(0, 0, 0);
            size = 0;
        }

        face_(vector3f normal, vector3f mid_point, double size) {
            this->normal = normal;
            this->mid_point = mid_point;
            this->size = size;
            assert (size > 0);
            assert (normal.length() == 1);
            assert((normal.x == 0 && normal.y == 0) || (normal.y == 0 && normal.z == 0) || (normal.z == 0 && normal.x == 0));
        }

        //assignment operator
        face_ &operator=(const face_ &f) {
            this->normal = f.normal;
            this->mid_point = f.mid_point;
            this->size = f.size;
            return *this;
        }

        inline bool within (const double &q, const double &min, const double &max) {
            return (q >= min && q <= max);
        }

        bool is_hit_by_ray(ray &r, double &distance) {
            distance = (mid_point - r.origin).dot(normal) / r.direction.dot(normal);
            if (distance < 0) return false;
            vector3f point = r.origin + r.direction * distance;
            return contains_point(point);
        }

        bool contains_point(const vector3f &point) {
            if (normal.x != 0) {
                // face parallel to yz plane
                return within(point.y, mid_point.y - size / 2, mid_point.y + size / 2) && within(point.z, mid_point.z - size / 2, mid_point.z + size / 2);
            } else if (normal.y != 0) {
                // face parallel to zx plane
                return within(point.x, mid_point.x - size / 2, mid_point.x + size / 2) && within(point.z, mid_point.z - size / 2, mid_point.z + size / 2);
            } else {
                // face parallel to xy plane
                return within(point.x, mid_point.x - size / 2, mid_point.x + size / 2) && within(point.y, mid_point.y - size / 2, mid_point.y + size / 2);
            }
        }

        void show() {
            // std::cout << "showing face with normal" << normal << std::endl;
            glPushMatrix();
            glTranslatef(mid_point.x, mid_point.y, mid_point.z);
            glBegin(GL_QUADS);
                if(normal.x != 0) {
                    glNormal3f(normal.x, 0, 0);
                    glVertex3f(0, size / 2, size / 2);
                    glVertex3f(0, -size / 2, size / 2);
                    glVertex3f(0, -size / 2, -size / 2);
                    glVertex3f(0, size / 2, -size / 2);
                } else if (normal.y != 0) {
                    glNormal3f(0, normal.y, 0);
                    glVertex3f(size / 2, 0, size / 2);
                    glVertex3f(-size / 2, 0, size / 2);
                    glVertex3f(-size / 2, 0, -size / 2);
                    glVertex3f(size / 2, 0, -size / 2);
                } else {
                    glNormal3f(0, 0, normal.z);
                    glVertex3f(size / 2, size / 2, 0);
                    glVertex3f(-size / 2, size / 2, 0);
                    glVertex3f(-size / 2, -size / 2, 0);
                    glVertex3f(size / 2, -size / 2, 0);
                }
            glEnd();
            glPopMatrix();
        }
    } face_;
    vector3f position;
    double size;
    color clr;
    std::vector<face_> faces;
    
    cube(vector3f position, double size, color clr , double ambient, double diffuse, double specular, double reflection, double shininess) : shape3d(ambient, diffuse, specular, reflection, shininess) {
        this->position = position;
        this->size = size;
        this->clr = clr;
        this->faces.push_back(face_(vector3f(0, 0, 1), position + vector3f(0, 0, size / 2), size));
        this->faces.push_back(face_(vector3f(0, 0, -1), position + vector3f(0, 0, -size / 2), size));
        this->faces.push_back(face_(vector3f(0, 1, 0), position + vector3f(0, size / 2, 0), size));
        this->faces.push_back(face_(vector3f(0, -1, 0), position + vector3f(0, -size / 2, 0), size));
        this->faces.push_back(face_(vector3f(1, 0, 0), position + vector3f(size / 2, 0, 0), size));
        this->faces.push_back(face_(vector3f(-1, 0, 0), position + vector3f(-size / 2, 0, 0), size));
    }

    void calculate_hit_distance(ray &r) override {
        for (auto face : this->faces) {
            double distance;
            if (face.is_hit_by_ray(r, distance)) {
                r.set_hit(distance, this, face.normal);
            }
        }
        return;
    }

    vector3f normal_at(vector3f &point) override {
        for (auto face : faces) {
            if (face.contains_point(point)) {
                return face.normal;
            }
        }
    }

    color get_color_at(vector3f &point) override {
        return clr;
    }

    void show() override
    {
        glPushMatrix();
        glTranslated(position.x, position.y, position.z);
        glColor3f(clr.r, clr.g, clr.b);
        // for (auto face : faces) {
        //     face.show();
        // }
        glutSolidCube(size);
        glPopMatrix();
    }

    void print() override {
        for (auto face : faces) {
            std::cout << "face with normal " << face.normal << " and mid point " << face.mid_point << " and size " << face.size << std::endl;
        }
    }
};