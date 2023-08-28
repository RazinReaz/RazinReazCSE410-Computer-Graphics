#include <iostream>
#include <cmath>
#include <chrono>
#include <ctime>

#include "point.h"
#include "draw.h"

using namespace std;
#define PI acos(-1.0)
#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 700

double g = 0.0000015;

double clock_outer_radius = 0.4;
double clock_inner_radius = 0.35;
double clock_hour_handlength = 0.2;
double clock_minute_handlength = 0.3;
double clock_second_handlength = 0.4;
double clock_pendulum_length = 0.5;
double clock_pendulum_radius = 0.05;

double hour_marker_offset = 0.1;
double minute_marker_offset = 0.05;
double hour_hand_width = 3;
double minute_hand_width = 2;
double second_hand_width = 1;

double pendulum_length = 0.5;
double pendulum_radius = 0.1;

double pendulum_theta = PI/20;
double pendulum_theta_velocity = 0.0;
double pendulum_theta_acceleration = (-1 * g * sin(pendulum_theta) )/ pendulum_length;

void draw_hour_markers(point anchor)
{
    for (int i = 0; i < 12; i++)
    {
        point a, b;
        double theta = 2.0 * PI * double(i) / double(12);
        a.x = anchor.x + clock_inner_radius * cos(theta);
        a.y = anchor.y + clock_inner_radius * sin(theta);
        b.x = anchor.x + (clock_inner_radius - hour_marker_offset) * cos(theta);
        b.y = anchor.y + (clock_inner_radius - hour_marker_offset) * sin(theta);
        line2D(a, b);
    }
}

void draw_minute_markers(point anchor) {
    for (int i = 0; i < 60; i++)
    {
        // skip the hour marjers
        if (i % 5 == 0)
            continue;

        point a, b;
        double theta = 2.0 * PI * double(i) / double(60);
        a.x = anchor.x + clock_inner_radius * cos(theta);
        a.y = anchor.y + clock_inner_radius * sin(theta);
        b.x = anchor.x + (clock_inner_radius - minute_marker_offset) * cos(theta);
        b.y = anchor.y + (clock_inner_radius - minute_marker_offset) * sin(theta);
        line2D(a, b);
    }
}

void draw_hour_hand(point anchor, double hour, double minute) {
    double theta = 2.0 * PI * ( int(hour) % 12 + minute / 60.0) / 12.0;
    point b;
    b.x = anchor.x + clock_hour_handlength * sin(theta);
    b.y = anchor.y + clock_hour_handlength * cos(theta);
    line2D(anchor, b, hour_hand_width);
}

void draw_minute_hand(point anchor, double minute, double second) {
    double theta = 2.0 * PI * (minute + second/60.0)/ 60.0;
    point b;
    b.x = anchor.x + clock_minute_handlength * sin(theta);
    b.y = anchor.y + clock_minute_handlength * cos(theta);
    line2D(anchor, b, minute_hand_width);
}

void draw_second_hand(point anchor, double second) {
    double theta = 2.0 * PI * second / 60.0;
    point b;
    b.x = anchor.x + clock_second_handlength * sin(theta);
    b.y = anchor.y + clock_second_handlength * cos(theta);
    line2D(anchor, b, second_hand_width);
}

void draw_pendulum(point anchor, double length, double radius) {
    pendulum_theta_acceleration = (-1 * g * sin(pendulum_theta)) / pendulum_length;
    pendulum_theta_velocity += pendulum_theta_acceleration;
    pendulum_theta += pendulum_theta_velocity;

    point a, b;
    a.x = anchor.x + clock_outer_radius * sin(pendulum_theta);
    a.y = anchor.y - clock_outer_radius * cos(pendulum_theta);
    b.x = anchor.x + (clock_outer_radius + pendulum_length) * sin(pendulum_theta);
    b.y = anchor.y - (clock_outer_radius + pendulum_length) * cos(pendulum_theta);
    line2D(a, b, 3);
    circle2D(b, pendulum_radius, 2, true);
}

void draw_clock(point anchor)
{
    circle2D(anchor, clock_outer_radius, 3);
    circle2D(anchor, clock_inner_radius, 2);
    draw_hour_markers(anchor);
    draw_minute_markers(anchor);

    //get current time
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm *local_time = std::localtime(&now_c);
    int hour = local_time->tm_hour;
    int minute = local_time->tm_min;
    int second = local_time->tm_sec;

    draw_hour_hand(anchor, hour, minute);
    draw_minute_hand(anchor, minute, second);
    draw_second_hand(anchor, second);

    draw_pendulum(anchor, pendulum_length, pendulum_radius);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 1.0);
    draw_clock(point(0, 0.5));
    glFlush();
}

void idle() {
    glutPostRedisplay();
}

int main(int argc, char *argv[])
{
    // Initialize GLUT
    glutInit(&argc, argv);
    // glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Clock");
    glutDisplayFunc(display); // Register display callback handler for window re-paint
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
