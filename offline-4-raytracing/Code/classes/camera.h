#pragma once
#include <iostream>
#include "vector3f.h"
#include <GL\glut.h>

class Camera {
    vector3f position;
    vector3f target;
    vector3f up;
    vector3f direction;
    vector3f right;

public:
    Camera(vector3f position = vector3f(100, 0, -100), vector3f target = vector3f(0, 0, 0)) {
        vector3f up = {0, 1, 0};
        this->position = position;
        this->target = target;
        this->direction = (target - position).normalize();
        this->right = direction.cross(up).normalize();
        this->up = right.cross(direction).normalize();
    }

    vector3f get_direction() {
        return direction;
    }
    vector3f get_position() {
        return position;
    }
    vector3f get_target() {
        return target;
    }
    vector3f get_up() {
        return up;
    }
    vector3f get_right() {
        return right;
    }

    void look() {
        gluLookAt(position.x, position.y, position.z,
                  position.x + direction.x, position.y + direction.y, position.z + direction.z,
                  up.x, up.y, up.z);
    }

    void move_with_same_target(vector3f dir, double del) {
        position = position + dir*del;
        this->direction = (target - position).normalize();
        this->right = direction.cross(up).normalize();
        this->up = right.cross(direction).normalize();
    }
    void move(vector3f dir, double del)
    {
        position = position + dir * del;
    }

    void rotate_horizontal(float del_theta){
        // rotate camera about the up vector
        this->direction = direction.rotate(del_theta, up);
        this->right = right.rotate(del_theta, up);
    }

    void rotate_vertical(float del_phi) {
        // rotate camera about the right vector
        this->direction = direction.rotate(del_phi, right);
        this->up = up.rotate(del_phi, right);
    }

    void tilt( double del_psi) {
        // rotate camera about the direction vector
        this->up = up.rotate(del_psi, direction);
        this->right = right.rotate(del_psi, direction);
    }

    void revolve(double angle, vector3f axis) {
        this->position = position.rotate(angle, axis);
        this->direction = direction.rotate(angle, axis);
        this->up = up.rotate(angle, axis);
        this->right = right.rotate(angle, axis);
    }

    void move_forward(float del) {
        position = position + direction*del;
        target = target + direction*del;
    }
    void move_backward(float del) {
        position = position - direction*del;
        target = target - direction*del;
    }

    void move_left(float del) {
        position = position - right*del;
        target = target - right*del;
    }

    void move_right(float del) {
        position = position + right*del;
        target = target + right*del;
    }

    void move_up(float del) {
        position = position + up*del;
        target = target + up*del;
    }

    void move_down(float del) {
        position = position - up*del;
        target = target - up*del;
    }

    void print() {
        std::cout << "position: " << position << std::endl;
        std::cout << "target: " << target << std::endl;
        std::cout << "up: " << up << std::endl;
        std::cout << "direction: " << direction << std::endl;
        std::cout << "right: " << right << std::endl;
    }
};