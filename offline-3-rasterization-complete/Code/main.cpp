#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stack>
#include <vector>
#include <cmath>
#include <cassert>

#include "color.h"
#include "bitmap_image.hpp"
#include "utils.h"
#include "vector3f.h"
#include "matrix.h"
#include "triangle.h"

using namespace std;

int main()
{
    // open files
    ifstream scene_file = ifstream("scene.txt");
    ofstream stage1_file = ofstream("stage1.txt");
    ofstream stage2_file = ofstream("stage2.txt");
    ofstream stage3_file = ofstream("stage3.txt");

    // set precision
    cout << std::fixed << std::setprecision(7);
    stage1_file << std::fixed << std::setprecision(7);
    stage2_file << std::fixed << std::setprecision(7);
    stage3_file << std::fixed << std::setprecision(7);

    // taking inputs
    string line;
    vector3f eye, lookat, up;
    double fovY, aspect_ratio, near, far;

    scene_file >> eye.x >> eye.y >> eye.z;
    scene_file >> lookat.x >> lookat.y >> lookat.z;
    scene_file >> up.x >> up.y >> up.z;
    scene_file >> fovY >> aspect_ratio >> near >> far;

    // #########################
    // #  VIEW TRANSFORMATION  #
    // #########################
    matrix R;
    vector3f l = (lookat - eye).normalize();
    vector3f r = (l.cross(up)).normalize();
    vector3f u = r.cross(l);
    R.data[0][0] = r.x;
    R.data[0][1] = r.y;
    R.data[0][2] = r.z;
    R.data[1][0] = u.x;
    R.data[1][1] = u.y;
    R.data[1][2] = u.z;
    R.data[2][0] = -l.x;
    R.data[2][1] = -l.y;
    R.data[2][2] = -l.z;
    matrix v = R * create_translation_matrix(-eye.x, -eye.y, -eye.z);

    // #############################
    // # PROJECTION TRANSFORMATION #
    // #############################

    double fovX = fovY * aspect_ratio;
    double t = near * tan(fovY * PI / 360);
    double r1 = near * tan(fovX * PI / 360);

    matrix p;
    p.data[0][0] = near / r1;
    p.data[0][1] = 0;
    p.data[0][2] = 0;
    p.data[0][3] = 0;
    p.data[1][0] = 0;
    p.data[1][1] = near / t;
    p.data[1][2] = 0;
    p.data[1][3] = 0;
    p.data[2][0] = 0;
    p.data[2][1] = 0;
    p.data[2][2] = -(far + near) / (far - near);
    p.data[2][3] = -(2 * far * near) / (far - near);
    p.data[3][0] = 0;
    p.data[3][1] = 0;
    p.data[3][2] = -1;
    p.data[3][3] = 0;

    matrix m;
    stack<matrix> s;
    while (getline(scene_file, line))
    {
        if (line == "triangle")
        {
            for (int i = 0; i < 3; i++)
            {
                vector3f point, v_point, p_point;
                scene_file >> point.x >> point.y >> point.z;

                point = m * point;
                v_point = v * point;
                p_point = p * v_point;

                stage1_file << point.x << " " << point.y << " " << point.z << endl;
                stage2_file << v_point.x << " " << v_point.y << " " << v_point.z << endl;
                stage3_file << p_point.x << " " << p_point.y << " " << p_point.z << endl;
            }
            stage1_file << endl;
            stage2_file << endl;
            stage3_file << endl;
        }
        else if (line == "translate")
        {
            double tx, ty, tz;
            scene_file >> tx >> ty >> tz;
            m = m.translate(tx, ty, tz);
        }
        else if (line == "scale")
        {
            double sx, sy, sz;
            scene_file >> sx >> sy >> sz;
            m = m.scale(sx, sy, sz);
        }
        else if (line == "rotate")
        {
            double angle, rx, ry, rz;
            scene_file >> angle >> rx >> ry >> rz;
            m = m.rotate(angle, rx, ry, rz);
        }
        else if (line == "push")
        {
            s.push(m);
        }
        else if (line == "pop")
        {
            m = s.top();
            s.pop();
        }
        else if (line == "end")
        {
            break;
        }
    }
    
    stage1_file.close();
    stage2_file.close();
    stage3_file.close();

    ifstream stage3_file2 = ifstream("stage3.txt");
    ifstream config_file = ifstream("config.txt");
    ofstream z_buffer_file = ofstream("z_buffer.txt");

    // initialization
    cout << "reading config file" << endl;
    int width, height;
    config_file >> width >> height;
    const double dx = 2.0 / width;  // the 2.0 is from the range [-1,1]
    const double dy = 2.0 / height; // the 2.0 is from the range [-1,1]
    const double top_y = 1.0 - dy / 2.0;
    const double bottom_y = -1.0 + dy / 2.0;
    const double left_x = -1.0 + dx / 2.0;
    const double right_x = 1.0 - dx / 2.0;
    const double z_max = 2.0;
    vector<vector<double>> z_buffer(height, vector<double>(width, z_max));
    bitmap_image image(width, height);

    // the stage3 file only has triangles. we can assum after every 3 lines there is a new line
    vector3f points[3];
    vector<triangle> triangles;

    cout << "reading stage3 file" << endl;
    while (!stage3_file2.eof())
    {
        stage3_file2 >> points[0].x >> points[0].y >> points[0].z;
        stage3_file2 >> points[1].x >> points[1].y >> points[1].z;
        stage3_file2 >> points[2].x >> points[2].y >> points[2].z;
        if (stage3_file2.eof())
            break;
        stage3_file2.ignore(256, '\n');
        color rand_color = get_random_color();
        triangle t = triangle(points[0], points[1], points[2], rand_color);
        t.set_screen_height(height);
        t.set_screen_width(width);
        triangles.push_back(t);
    }

    cout << "calculating z values" << endl;
    for (int i = 0; i < triangles.size(); i++)
    {
        color c = triangles[i].get_color();
        double max_y = triangles[i].get_top_scanline();
        double min_y = triangles[i].get_bottom_scanline();

        for (double y = max_y; y >= min_y; y -= dy)
        {
            int row = get_row_from_y(y, height);

            vector<double> x_range = triangles[i].get_x_range(y);
            double min_x = x_range[0];
            double max_x = x_range[1];

            for (double x = min_x; x <= max_x; x += dx) {
                int col = get_col_from_x(x, width);

                // cout << "(" << row << ":" << y <<  "," << col << ":" << x << ") " ;
                double z = triangles[i].get_z(x, y);
                if (z < z_buffer[row][col] && z >= -1) {
                    z_buffer[row][col] = z;
                    image.set_pixel(col, row, c.r, c.g, c.b);
                }
            }
            // cout << endl;
        }
    }

    cout << "writing z buffer file" << endl;
    z_buffer_file << std::fixed << std::setprecision(6);
    for ( int i = 0; i < height; i++ ) {
        for ( int j = 0; j < width; j++ ) {
            if ( z_buffer[i][j] < z_max) {
                // cout << z_buffer[i][j] << ":";
                z_buffer_file << z_buffer[i][j] << "\t";
            }
        }
        z_buffer_file << endl;
    }

    stage3_file2.close();
    config_file.close();
    z_buffer_file.close();
    image.save_image("out.bmp");
    cout << "done" << endl;
    return 0;
}