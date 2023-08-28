#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <stack>
#include "vector3f.h"
#include "matrix.h"

using namespace std;




int main() {
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
    R.data[0][0] = r.x;     R.data[0][1] = r.y;     R.data[0][2] = r.z;
    R.data[1][0] = u.x;     R.data[1][1] = u.y;     R.data[1][2] = u.z;
    R.data[2][0] = -l.x;    R.data[2][1] = -l.y;    R.data[2][2] = -l.z;
    matrix v = R * create_translation_matrix(-eye.x, -eye.y, -eye.z);


    // #############################
    // # PROJECTION TRANSFORMATION #
    // #############################

    cout << near << " " << far << endl;
    cout << fovY << " " << aspect_ratio << endl;

    double fovX = fovY * aspect_ratio;
    double t = near * tan(fovY*PI/360);
    double r1 = near * tan(fovX*PI/360);


    matrix p;
    p.data[0][0] = near/r1; p.data[0][1] = 0;      p.data[0][2] = 0;    p.data[0][3] = 0;
    p.data[1][0] = 0;       p.data[1][1] = near/t; p.data[1][2] = 0;    p.data[1][3] = 0;
    p.data[2][0] = 0;       p.data[2][1] = 0;      p.data[2][2] = -(far+near)/(far-near); p.data[2][3] = -(2*far*near)/(far-near);
    p.data[3][0] = 0;       p.data[3][1] = 0;      p.data[3][2] = -1;   p.data[3][3] = 0;

    cout << "perspective matrix: \n" << p << endl;

    matrix m;
    stack<matrix> s;
    while(getline(scene_file, line)) {
        if( line == "triangle" ) {
            for (int i = 0; i < 3; i++) {
                vector3f point, v_point, p_point;                
                scene_file >> point.x >> point.y >> point.z;

                point = m * point;
                v_point = v * point;
                p_point = p * v_point;

                stage1_file << point.x   << " " << point.y   << " " << point.z   << endl;
                stage2_file << v_point.x << " " << v_point.y << " " << v_point.z << endl;
                stage3_file << p_point.x << " " << p_point.y << " " << p_point.z << endl;
            }
            stage1_file << endl;
            stage2_file << endl;
            stage3_file << endl;
        }
        else if ( line == "translate" ) {
            double tx, ty, tz;
            scene_file >> tx >> ty >> tz;
            m = m.translate(tx, ty, tz);
        }
        else if ( line == "scale" ) {
            double sx, sy, sz;
            scene_file >> sx >> sy >> sz;
            m = m.scale(sx, sy, sz);
        }
        else if ( line == "rotate" ) {
            double angle, rx, ry, rz;
            scene_file >> angle >> rx >> ry >> rz;
            m = m.rotate(angle, rx, ry, rz);
        }
        else if ( line == "push" ) {
            s.push(m);
        }
        else if ( line == "pop" ) {
            m = s.top();
            s.pop();
        }
        else if ( line == "end" ) {
            break;
        }
    }
    return 0;
}