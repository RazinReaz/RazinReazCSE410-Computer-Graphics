#define _USE_MATH_DEFINES
#define PI acos(-1.0)
#include <windows.h>
#include <iostream>
#include <GL/glut.h>
#include "camera.h"
#include "color.h"
struct point
{
    GLfloat x, y, z;
};

void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
}

Camera camera;
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 640;

const int divisions = 20;
const double max_size = 1.0;
const double del_dist = max_size / divisions;
double dist = 0.0;

const vector3f x_axis = {1, 0, 0};
const vector3f y_axis = {0, 1, 0};
const vector3f z_axis = {0, 0, 1};

/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxes()
{
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); // Red
    // X axis
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0); // Green
    // Y axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1); // Blue
    // Z axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);
    glEnd();
}

/* Draw a cube centered at the origin */
void drawCube()
{
    glBegin(GL_QUADS); // Begin drawing the color cube with 6 quads
    // Top face (y = 1.0f)
    // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glColor3f(0.0f, 1.0f, 0.0f); // Green
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.5f, 0.0f); // Orange
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Front face  (z = 1.0f)
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);

    // Left face (x = -1.0f)
    glColor3f(0.0f, 0.0f, 1.0f); // Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);

    // Right face (x = 1.0f)
    glColor3f(1.0f, 0.0f, 1.0f); // Magenta
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd(); // End of drawing color-cube
}

void drawLine(vector3f start, vector3f end, color c = color(1, 0, 0))
{
    glColor3f(c.r, c.g, c.b);
    glBegin(GL_LINES);
    glVertex3f(start.x, start.y, start.z);
    glVertex3f(end.x, end.y, end.z);
    glEnd();
}

// draws a sphere segmetn in the direction of positive x axis
void draw_sphere_segment(double radius, vector3f centre, color c = color(1, 0, 0))
{
    int slices = 100;
    int stacks = 100;
    vector3f vertices[slices + 1][stacks + 1];
    for (int j = 0; j <= stacks; j++)
    {
        vector3f n2 = {0, 1, 0};
        vector3f n1 = {0, 0, 1};
        double phi = PI / 4 - j * PI / (stacks * 2);
        vector3f n2_prime = n2.rotate(phi, {0, 0, 1});

        for (int i = 0; i <= slices; i++)
        {
            double theta = PI / 4 - i * PI / (slices * 2);
            vector3f n1_prime = n1.rotate(theta, {0, 1, 0});
            vector3f r = (n2_prime.cross(n1_prime)).normalize() * radius;
            vertices[j][i] = centre + r;
        }
    }

    glBegin(GL_QUADS);
    for (int j = 0; j < stacks; j++)
    {
        for (int i = 0; i < slices; i++)
        {
            glColor3f(c.r, c.g, c.b);
            glVertex3f(vertices[j][i].x, vertices[j][i].y, vertices[j][i].z);
            glVertex3f(vertices[j][i + 1].x, vertices[j][i + 1].y, vertices[j][i + 1].z);

            glVertex3f(vertices[j + 1][i + 1].x, vertices[j + 1][i + 1].y, vertices[j + 1][i + 1].z);
            glVertex3f(vertices[j + 1][i].x, vertices[j + 1][i].y, vertices[j + 1][i].z);
        }
    }
    glEnd();
}

void drawSphere(double radius, vector3f centre)
{
    vector3f x_axis = {1, 0, 0};
    vector3f y_axis = {0, 1, 0};
    vector3f z_axis = {0, 0, 1};
    glPushMatrix();
    glTranslatef(centre.x, centre.y, centre.z);
    draw_sphere_segment(radius, {0, 0, 0}, color_array[YELLOW]);

    glRotatef(90, z_axis.x, z_axis.y, z_axis.z);
    draw_sphere_segment(radius, {0, 0, 0}, color_array[RED]);

    glRotatef(90, z_axis.x, z_axis.y, z_axis.z);
    draw_sphere_segment(radius, {0, 0, 0}, color_array[PURPLE]);

    glRotatef(90, z_axis.x, z_axis.y, z_axis.z);
    draw_sphere_segment(radius, {0, 0, 0}, color_array[BLUE]);

    glRotatef(90, y_axis.x, y_axis.y, y_axis.z);
    draw_sphere_segment(radius, {0, 0, 0}, color_array[GREEN]);

    glRotatef(180, y_axis.x, y_axis.y, y_axis.z);
    draw_sphere_segment(radius, {0, 0, 0}, color_array[GREEN]);
    glPopMatrix();
}

void draw_vertices(double radius)
{
    vector3f centre = {0, 0, 0};

    glPushMatrix();
    glTranslatef(centre.x, centre.y, centre.z);
    draw_sphere_segment(radius, {dist, 0, 0}, color_array[BLUE]);

    glRotatef(90, z_axis.x, z_axis.y, z_axis.z);
    draw_sphere_segment(radius, {dist, 0, 0}, color_array[RED]);

    glRotatef(90, z_axis.x, z_axis.y, z_axis.z);
    draw_sphere_segment(radius, {dist, 0, 0}, color_array[BLUE]);

    glRotatef(90, z_axis.x, z_axis.y, z_axis.z);
    draw_sphere_segment(radius, {dist, 0, 0}, color_array[RED]);

    glRotatef(90, y_axis.x, y_axis.y, y_axis.z);
    draw_sphere_segment(radius, {dist, 0, 0}, color_array[GREEN]);

    glRotatef(180, y_axis.x, y_axis.y, y_axis.z);
    draw_sphere_segment(radius, {dist, 0, 0}, color_array[GREEN]);
    glPopMatrix();
}

void draw_triangle(vector3f pointA, vector3f pointB, vector3f pointC, color c = color(1, 0, 0))
{
    glBegin(GL_TRIANGLES);
    glColor3f(c.r, c.g, c.b);
    glVertex3f(pointA.x, pointA.y, pointA.z);
    glVertex3f(pointB.x, pointB.y, pointB.z);
    glVertex3f(pointC.x, pointC.y, pointC.z);
    glEnd();
}

void draw_faces()
{
    double d = dist;
    vector3f pointA = {(1 + 2 * d) / 3, (1 - d) / 3, (1 - d) / 3};
    vector3f pointB = {(1 - d) / 3, (1 + 2 * d) / 3, (1 - d) / 3};
    vector3f pointC = {(1 - d) / 3, (1 - d) / 3, (1 + 2 * d) / 3};

    vector3f pointD = {(1 + 2 * d) / 3, -(1 - d) / 3, (1 - d) / 3};
    vector3f pointE = {(1 - d) / 3, -(1 + 2 * d) / 3, (1 - d) / 3};
    vector3f pointF = {(1 - d) / 3, -(1 - d) / 3, (1 + 2 * d) / 3};

    glPushMatrix();
    draw_triangle(pointA, pointB, pointC, color_array[PURPLE]);
    draw_triangle(pointD, pointE, pointF, color_array[CYAN]);

    glRotatef(90, y_axis.x, y_axis.y, y_axis.z);
    draw_triangle(pointA, pointB, pointC, color_array[CYAN]);
    draw_triangle(pointD, pointE, pointF, color_array[PURPLE]);

    glRotatef(90, y_axis.x, y_axis.y, y_axis.z);
    draw_triangle(pointA, pointB, pointC, color_array[PURPLE]);
    draw_triangle(pointD, pointE, pointF, color_array[CYAN]);

    glRotatef(90, y_axis.x, y_axis.y, y_axis.z);
    draw_triangle(pointA, pointB, pointC, color_array[CYAN]);
    draw_triangle(pointD, pointE, pointF, color_array[PURPLE]);
    glPopMatrix();
}

void draw_cylinder_segment(vector3f top_centre, vector3f bottom_centre,
                           vector3f A, vector3f B,
                           vector3f C, vector3f D,
                           color c = color(1, 1, 0))
{
    vector3f axis = top_centre - bottom_centre;
    vector3f top1 = A - top_centre;
    vector3f top2 = B - top_centre;
    vector3f bottom1 = C - bottom_centre;
    vector3f bottom2 = D - bottom_centre;

    int segments = 100;
    double max_angle = acos(top1.dot(top2) / (top1.length() * top2.length()));

    vector3f top_points[segments + 1];
    vector3f bottom_points[segments + 1];
    for (int i = 0; i <= segments; i++)
    {
        double angle = max_angle * i / segments;
        top_points[i] = top1.rotate(-angle, axis) + top_centre;
        bottom_points[i] = bottom1.rotate(-angle, axis) + bottom_centre;
    }

    glBegin(GL_QUAD_STRIP);
    glColor3f(c.r, c.g, c.b);
    for (int i = 0; i <= segments; i++)
    {
        glVertex3f(top_points[i].x, top_points[i].y, top_points[i].z);
        glVertex3f(bottom_points[i].x, bottom_points[i].y, bottom_points[i].z);
    }
    glEnd();
}

void draw_edges()
{
    /*
          C--D
          |  |
          A--B
    E-----||||
    F-----||||
          G  H
          |  |
          I--J
    */
    double d = dist;
    vector3f X_axis_centre = {d, 0, 0};
    vector3f Y_axis_centre = {0, d, 0};
    vector3f Z_axis_centre = {0, 0, d};
    vector3f minus_Y_axis_centre = {0, -d, 0};
    vector3f A = {(1 + 2 * d) / 3, (1 - d) / 3, (1 - d) / 3};
    vector3f B = {(1 + 2 * d) / 3, (1 - d) / 3, -(1 - d) / 3};
    vector3f C = {(1 - d) / 3, (1 + 2 * d) / 3, (1 - d) / 3};
    vector3f D = {(1 - d) / 3, (1 + 2 * d) / 3, -(1 - d) / 3};
    vector3f E = {(1 - d) / 3, (1 - d) / 3, (1 + 2 * d) / 3};
    vector3f F = {(1 - d) / 3, -(1 - d) / 3, (1 + 2 * d) / 3};
    vector3f G = {(1 + 2 * d) / 3, -(1 - d) / 3, (1 - d) / 3};
    vector3f H = {(1 + 2 * d) / 3, -(1 - d) / 3, -(1 - d) / 3};
    vector3f I = {(1 - d) / 3, -(1 + 2 * d) / 3, (1 - d) / 3};
    vector3f J = {(1 - d) / 3, -(1 + 2 * d) / 3, -(1 - d) / 3};
    glPushMatrix();
    draw_cylinder_segment(X_axis_centre, Y_axis_centre, A, B, C, D);
    draw_cylinder_segment(X_axis_centre, Z_axis_centre, G, A, F, E);
    draw_cylinder_segment(X_axis_centre, minus_Y_axis_centre, H, G, J, I);
    for (int i = 0; i < 3; i++){
        glRotatef(90, 0, 1, 0);
        draw_cylinder_segment(X_axis_centre, Y_axis_centre, A, B, C, D);
        draw_cylinder_segment(X_axis_centre, Z_axis_centre, G, A, F, E);
        draw_cylinder_segment(X_axis_centre, minus_Y_axis_centre, H, G, J, I);
    }
    glPopMatrix();
}

void draw_magic_cube()
{
    double radius = (1 - dist) / sqrt(3);
    draw_vertices(radius);
    draw_faces();
    draw_edges();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // To operate on Model-View matrix
    glLoadIdentity();           // Reset the model-view matrix
    camera.look();

    drawAxes();
    draw_magic_cube();
    glutSwapBuffers(); // Render now
}

void reshape(GLsizei width, GLsizei height)
{
    if (height == 0)
        height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity();            // Reset the projection matrix
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

void keyboardListener(unsigned char key, int xx, int yy)
{
    double camera_rotation_rate = 0.01;
    double object_rotation_rate = 0.2;
    double bonus_mark_rotation_rate = 0.1;
    switch (key)
    {
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
    case '.':
        dist += del_dist;
        if (dist > 1)
            dist = 1;
        break;
    case ',':
        dist -= del_dist;
        if (dist < 0)
            dist = 0;
        break;
    case 'a':
        camera.revolve(object_rotation_rate, {0, 1, 0});
        break;
    case 'd':
        camera.revolve(-object_rotation_rate, {0, 1, 0});
        break;
    case 'w':
        camera.move_with_same_target({0,1,0}, bonus_mark_rotation_rate);
        break;
    case 's':
        camera.move_with_same_target({0,1,0}, -bonus_mark_rotation_rate);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}

void specialKeyListener(int key, int x, int y)
{
    double rate = 0.5;
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
    // camera.print();
    glutInit(&argc, argv);                                    // Initialize GLUT
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);          // Set the window's initial width & height
    glutInitWindowPosition(50, 50);                           // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color
    glutCreateWindow("Magic cube");                           // Create a window with the given title
    glutDisplayFunc(display);                                 // Register display callback handler for window re-paint
    glutReshapeFunc(reshape);                                 // Register callback handler for window re-shape

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);

    initGL();       // Our own OpenGL initialization
    glutMainLoop(); // Enter the event-processing loop
    return 0;
}
