#pragma once

#include <cmath>
#include <iostream>

class vector3f {
public:
    double x, y, z;
    vector3f(double x = 0, double y = 0, double z = 0);
    double length();
    vector3f normalize();
    vector3f cross(vector3f v);
    double dot(vector3f v);
    vector3f rotate(double angle, vector3f axis);
    vector3f operator+(vector3f v);
    vector3f operator-(vector3f v);
    vector3f operator*(double scalar);
    vector3f operator/(double scalar);
    vector3f operator+=(vector3f v);
    vector3f operator-=(vector3f v);

    friend std::ostream& operator<<(std::ostream& os, vector3f v);
};

vector3f::vector3f(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

double vector3f::length() {
    return sqrt(x*x + y*y + z*z);
}

vector3f vector3f::normalize() {
    double l = length();
    return vector3f(x / l, y / l, z / l);
}

vector3f vector3f::cross(vector3f v) {
    return vector3f(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
}

double vector3f::dot(vector3f v) {
    return x*v.x + y*v.y + z*v.z;
}

vector3f vector3f::rotate(double angle, vector3f axis ) {
    // rotate vector about axis by angle
    // Rodrigues' rotation formula
    vector3f v = *this;
    vector3f k = axis.normalize();
    vector3f v_rot = v*cos(angle) + k.cross(v)*sin(angle) + k*(k.dot(v))*(1 - cos(angle));
    return v_rot;
}

vector3f vector3f::operator+(vector3f v) {
    return vector3f(x + v.x, y + v.y, z + v.z);
}

vector3f vector3f::operator-(vector3f v) {
    return vector3f(x - v.x, y - v.y, z - v.z);
}

vector3f vector3f::operator*(double scalar) {
    return vector3f(x*scalar, y*scalar, z*scalar);
}

vector3f vector3f::operator/(double scalar) {
    return vector3f(x / scalar, y / scalar, z / scalar);
}

vector3f vector3f::operator+=(vector3f v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

vector3f vector3f::operator-=(vector3f v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

std::ostream& operator<<(std::ostream& os, vector3f v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}



