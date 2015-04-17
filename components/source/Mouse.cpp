#include "Mouse.h"
#include "main.h"
#include "Vector4.h"
#include "Config.h"
#include "Utility.h"
#include "Window.h"

#include "GL\glut.h"
#include <iostream>

#define camera Globals::camera

static Vector4 last_mouse_pos;
static BOOLEAN first_click = true;

Mouse::Mouse()
{
}


Mouse::~Mouse()
{
}

void map(int *x, int *y)
{
    *x = *x - Window::width / 2.0;
    *y = *y - Window::height / 2.0;
    *y = -1 * *y;
}

void Mouse::leftMotion(int x, int y)
{
    Vector3 look_at = camera.d - camera.e;
    Matrix4 rotX, rotY;

    map(&x, &y);

    if (x < 0)
        rotY.makeRotateY(5);
    else
        rotY.makeRotateY(-5);
    if (y < 0)
        rotX.makeRotateX(-5);
    else
        rotX.makeRotateX(5);

    look_at = rotX * rotY * look_at;
    camera.d = look_at + camera.e;

    camera.calculate_camera_matrix();
}

void Mouse::rightMotion(int x, int y)
{
    static Matrix4 rot;
    static Vector4 new_mouse_pos, motion, look_at;

    map(&x, &y);

    new_mouse_pos[0] = x;
    new_mouse_pos[1] = y;

    if (!first_click)
    {
        motion = new_mouse_pos - last_mouse_pos;
        look_at = camera.d - camera.e;
        look_at.normalize();
        
        if (motion[1] > 0)
        {
            camera.d += look_at * 1;
            camera.e += look_at * 1;
        }
        else
        {
            camera.d -= look_at * 1;
            camera.e -= look_at * 1;
        }

        camera.calculate_camera_matrix();
    }
    else
        first_click = false;

    last_mouse_pos[0] = x;
    last_mouse_pos[1] = y;
}

void Mouse::middleMotion(int x, int y)
{
    static Vector4 new_mouse_pos, motion;
    static Matrix4 transf;
    static Vector4 look_at, look_at_prime;

    new_mouse_pos[0] = x;
    new_mouse_pos[1] = y;

    if (!first_click)
    {
        look_at = camera.d - camera.e;
        motion = new_mouse_pos - last_mouse_pos;
        motion[1] = -motion[1];
        motion = motion * 0.05;

        camera.d += motion;
        
        look_at_prime = camera.d - camera.e;
        look_at_prime.normalize();
        camera.d = camera.e + look_at_prime * look_at.length();

        camera.calculate_camera_matrix();
    }
    else
        first_click = false;
    
#if (DEBUG_MOUSE == TRUE)
    std::cout << "look_at: " << camera.d.toString() << std::endl;
    std::cout << "middle motion x:" << x << " y:" << y << std::endl;
#endif

    last_mouse_pos[0] = x;
    last_mouse_pos[1] = y;
}

void Mouse::mouseCallback(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        /* A button is being pressed. Set the correct motion function */
        if (button == GLUT_LEFT_BUTTON)
            glutMotionFunc(leftMotion);
        else if (button == GLUT_RIGHT_BUTTON)
            glutMotionFunc(rightMotion);
    }
}