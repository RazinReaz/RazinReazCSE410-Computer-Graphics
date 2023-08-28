#pragma once
#include <iostream>
#include <vector> 
#include "vector3f.h"

#define epsilon 1e-09


class matrix;
matrix create_translation_matrix(double tx, double ty, double tz);
matrix create_scale_matrix(double sx, double sy, double sz);
matrix create_rotation_matrix(double angle, double x, double y, double z);

// homogenous transformation matrix for 3D
class matrix {
public:
    int rows;
    int cols;
    std::vector<std::vector<double>> data;

    matrix(int rows = 4, int cols = 4) {
        this->rows = rows;
        this->cols = cols;
        data.resize(rows);
        for (int i = 0; i < rows; i++) {
            data[i].resize(cols);
            for (int j = 0; j < cols; j++) {
                data[i][j] = (i==j);
            }
        }
    }

    // matrix(vector3f point){
    //     this->cols = 1;
    //     this->rows = 4;
    //     data = new double* [rows];
    //     for (int i = 0; i < rows; i++) {
    //         data[i] = new double[cols];
    //     }
    //     data[0][0] = point.x;
    //     data[1][0] = point.y;
    //     data[2][0] = point.z;
    //     data[3][0] = 1;
    // }

    matrix operator*(matrix m) {
        if(cols != m.rows) {
            std::cout << "Matrix multiplication not possible" << std::endl;
            exit(0);
        }

        matrix result(rows, m.cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < m.cols; j++) {
                result.data[i][j] = 0;
                for (int k = 0; k < cols; k++) {
                    result.data[i][j] += data[i][k] * m.data[k][j];
                }
            }
        }
        return result;
    }

    vector3f operator*(vector3f point) {
        vector3f result;
        result.x = data[0][0] * point.x + data[0][1] * point.y + data[0][2] * point.z + data[0][3] * point.w;
        result.y = data[1][0] * point.x + data[1][1] * point.y + data[1][2] * point.z + data[1][3] * point.w;
        result.z = data[2][0] * point.x + data[2][1] * point.y + data[2][2] * point.z + data[2][3] * point.w;
        result.w = data[3][0] * point.x + data[3][1] * point.y + data[3][2] * point.z + data[3][3] * point.w;

        result.x = result.x / result.w;
        result.y = result.y / result.w;
        result.z = result.z / result.w;
        result.w = 1;
        return result;
    }

    //overload the assignment operator
    matrix& operator=(const matrix& m) {
        if (this == &m) {
            return *this;
        }
        rows = m.rows;
        cols = m.cols;
        data.resize(rows);
        for (int i = 0; i < rows; i++) {
            data[i].resize(cols);
            for (int j = 0; j < cols; j++) {
                data[i][j] = m.data[i][j];
            }
        }
        return *this;
    }

    matrix translate(double tx, double ty, double tz) {
        return (*this) * create_translation_matrix(tx, ty, tz);
    }
    matrix scale(double sx, double sy, double sz) {
        return (*this) * create_scale_matrix(sx, sy, sz);
    }
    matrix rotate(double degree, double ax, double ay, double az) {
        return (*this) * create_rotation_matrix(degree, ax, ay, az);
    }

    friend std::ostream& operator<<(std::ostream& os, matrix m);
};

std::ostream& operator<<(std::ostream& os, matrix m) {
    for (int i = 0; i < m.rows; i++) {
        os << "[ ";
        for (int j = 0; j < m.cols; j++) {
            os << m.data[i][j] << " ";
        }
        os << "]" << std::endl;
    }
    return os;
}

matrix create_translation_matrix(double tx, double ty, double tz) {
    matrix t_matrix;
    t_matrix.data[0][3] = tx;
    t_matrix.data[1][3] = ty;
    t_matrix.data[2][3] = tz;
    return t_matrix;
}

matrix create_scale_matrix(double sx, double sy, double sz) {
    matrix s_matrix;
    s_matrix.data[0][0] = sx;
    s_matrix.data[1][1] = sy;
    s_matrix.data[2][2] = sz;
    return s_matrix;
}

matrix create_rotation_matrix(double degree, double ax, double ay, double az) {
    vector3f i = {1, 0, 0};
    vector3f j = {0, 1, 0};
    vector3f k = {0, 0, 1};
    vector3f axis = {ax, ay, az};
    double radian = degree * PI / 180.0;
    i = i.rotate(radian, axis);
    j = j.rotate(radian, axis);
    k = k.rotate(radian, axis);
    matrix m;
    m.data[0][0] = i.x;     m.data[0][1] = j.x;     m.data[0][2] = k.x;
    m.data[1][0] = i.y;     m.data[1][1] = j.y;     m.data[1][2] = k.y;
    m.data[2][0] = i.z;     m.data[2][1] = j.z;     m.data[2][2] = k.z;
    return m;
}