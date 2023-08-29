#define _USE_MATH_DEFINES
#include <windows.h>
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <gl/glut.h>

#include "classes/bitmap_image.hpp"
#include "classes/camera.h"
#include "classes/vector3f.h"
#include "classes/color.h"
#include "classes/interfaces.h"

#include "classes/point_light.h"
#include "classes/sphere.h"
#include "classes/checkerboard.h"

namespace utils {
    std::vector<bool> printed(11, false);
    void progress_report(int i, int j, int image_width, int image_height ) {
        double completed = (i * image_width + j) * 100.0 / (image_width * image_height);
        // print progress once only when completed is a multiple of 10
        if( (int)completed % 10 == 0 && !printed[(int)std::round(completed / 10)] ) {
            printed[(int)std::round(completed / 10)] = true;
            cout << "Rendering " << (int)completed << "% completed" << endl;
        }
        return;
    }
}


using namespace std;

void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
}

Camera camera;
bitmap_image image;


int fovY, aspect_ratio, near_, far_;
int window_size, recursions, checkerboard_size, image_width, image_height;
double checkboard_ambient, checkboard_diffuse, checkboard_reflection;


vector<shape3d *> objects;
vector<light *> lights;
vector<ray> rays;

vector3f x_axis = {1, 0, 0};
vector3f y_axis = {0, 1, 0};
vector3f z_axis = {0, 0, 1};

void draw_line(vector3f start, vector3f end, color c = color(0, 1, 0))
{
    glColor3f(c.r, c.g, c.b);
    glBegin(GL_LINES);
    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);
    glEnd();
}

void draw_checkerboard(int checkerboard_size){
    
}

void draw_axes()
{
    glLineWidth(3);
    draw_line({0, 0, 0}, x_axis * 500);   
    draw_line({0, 0, 0}, y_axis * 500);   
    draw_line({0, 0, 0}, z_axis * 500);   
}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // To operate on Model-View matrix
    glLoadIdentity();           // Reset the model-view matrix
    camera.look();

    for ( auto object: objects ) {
        object->show();
    }
    for ( auto light: lights ) {
        light->show();
    }
    
    draw_axes();
    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height)
{
    if (height == 0) height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity();            // Reset the projection matrix
    gluPerspective(fovY, aspect_ratio, near_, far_);
}

void keyboardListener(unsigned char key, int xx, int yy)
{
    double camera_rotation_rate = 0.1;
    double object_rotation_rate = 1.0;
    double bonus_mark_rotation_rate = 0.4;
    switch (key)
    {
        case '0':
        {
            double height = 2 * near_ * tan(fovY * PI / 360);
            double width = height * aspect_ratio;
            double dw = width / image_width;
            double dh = height / image_height;

            rays.clear();
            image.clear();
            utils::printed = std::vector<bool>(11, false);
            for (int i = 0; i < image_height; i++) {
                for (int j = 0; j < image_width; j++)
                {
                    vector3f pixel_position = camera.get_position() + camera.get_direction() * near_ + camera.get_right() * (j - (image_width-1)/ 2) * dw + camera.get_up() * ((image_height-1)/ 2 - i) * dh;
                    vector3f ray_direction = pixel_position - camera.get_position();
                    ray r(pixel_position, ray_direction);

                    int red = 0, green = 0, blue = 0;
                    for ( auto object : objects) {
                        object->calculate_hit_distance(r);
                    }
                    if(r.hit_info.hit) {
                        vector3f point = r.origin + r.direction * r.hit_info.distance;
                        color ambient_diffuse_specular_component = r.hit_info.object->get_diffuse_and_specular_color(point, r, lights, objects); // here i am passing pointer to r, otherwise it gives circular dependency error
                        color reflection_component = r.hit_info.object->get_reflection_color(point, r, lights, objects, recursions);
                        color c = ambient_diffuse_specular_component + reflection_component;
                        red = c.r * 255;
                        green = c.g * 255;
                        blue = c.b * 255;
                    } else {
                        red = sky_color.r * 255;
                        green = sky_color.g * 255;
                        blue = sky_color.b * 255;
                    }
                    image.set_pixel(j, i, red, green, blue);
                    utils::progress_report(i,j,image_width,image_height);
                }
            }
            cout << "Rendering 100% completed" << endl;
            
            image.save_image("out.bmp");
            cout << "image saved" << endl;
            break;
        }
        case '1':
            camera.rotate_horizontal(camera_rotation_rate);
            break;
        case '2':
            camera.rotate_horizontal(-camera_rotation_rate);
            break;
        case '3':
            camera.rotate_vertical(camera_rotation_rate);
            break;
        case '4':
            camera.rotate_vertical(-camera_rotation_rate);
            break;
        case '5':
            camera.tilt(camera_rotation_rate);
            break;

        case '6':
            camera.tilt(-camera_rotation_rate);
            break;
        case 'a':
            camera.revolve(object_rotation_rate, {0, 1, 0});
            break;
        case 'd':
            camera.revolve(-object_rotation_rate, {0, 1, 0});
            break;
        case 'w':
            camera.move_with_same_target({0, 1, 0}, bonus_mark_rotation_rate);
            break;
        case 's':
            camera.move_with_same_target({0, 1, 0}, -bonus_mark_rotation_rate);
            break;
        case 'i':
            lights[0]->position.y += 1;
            break;
        case 'k':
            lights[0]->position.y -= 1;
            break;
        case 'j':
            lights[0]->position.x -= 1;
            break;
        case 'l': 
            lights[0]->position.x += 1;
            break;
        case 'u':
            lights[0]->position.z -= 1;
            break;
        case 'o':
            lights[0]->position.z += 1;
            break;
        case 'q':
            exit(0);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void specialKeyListener(int key, int x, int y)
{
    double rate = 5.0;
    switch (key)
    {
    case GLUT_KEY_UP:
        camera.move(camera.get_direction(), rate);
        break;
    case GLUT_KEY_DOWN:
        camera.move(camera.get_direction(), -rate);
        break;
    case GLUT_KEY_RIGHT:
        camera.move(camera.get_right(), rate);
        break;
    case GLUT_KEY_LEFT:
        camera.move(camera.get_right(), -rate);
        break;
    case GLUT_KEY_PAGE_UP:
        camera.move(camera.get_up(), rate);
        break;
    case GLUT_KEY_PAGE_DOWN:
        camera.move(camera.get_up(), -rate);
        break;

    case GLUT_KEY_INSERT:
        break;
    case GLUT_KEY_HOME:
        break;
    case GLUT_KEY_END:
        break;

    default:
        break;
    }
    glutPostRedisplay();
}





int main(int argc, char **argv)
{
    ifstream in("in.txt");
    in >> near_ >> far_ >> fovY >> aspect_ratio >> recursions >> window_size;
    image_width = image_height = window_size;
    
    in >> checkerboard_size >> checkboard_ambient >> checkboard_diffuse >> checkboard_reflection;
    checkerboard *board = new checkerboard(checkerboard_size, checkboard_ambient, checkboard_diffuse, checkboard_reflection);
    objects.push_back(board);

    int number_of_objects, number_of_point_lights, number_of_spotlights ;
    in >> number_of_objects;
    for (int i = 0; i < number_of_objects; i++)
    {
        string object_type;
        in >> object_type;
        if (object_type == "sphere") {
            vector3f center;
            double radius;
            color c;
            double ambient, diffuse, specular, reflection, shininess;
            in >> center.x >> center.y >> center.z >> radius >> c.r >> c.g >> c.b;
            in >> ambient >> diffuse >> specular >> reflection >> shininess;
            sphere *s = new sphere(center, radius, c, ambient, diffuse, specular, reflection, shininess);
            objects.push_back(s);
        }
    }
    in >> number_of_point_lights;
    for (int i = 0; i < number_of_point_lights; i++) {
        vector3f position;
        double falloff;
        in >> position.x >> position.y >> position.z >> falloff;
        point_light *l = new point_light(position, falloff);
        lights.push_back(l);
    }
    in.close();
    image.setwidth_height(image_width, image_height);

    glutInit(&argc, argv);                                    // Initialize GLUT
    glutInitWindowSize(window_size, window_size);             // Set the window's initial width & height
    glutInitWindowPosition(50, 50);                           // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color
    glutCreateWindow("Raytracing");                           // Create a window with the given title
    glutDisplayFunc(display);                                 // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);                                 // Register callback handler for window re-shape

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    initGL();       // Our own OpenGL initialization
    glutMainLoop(); // Enter the event-processing loop
    return 0;
}
