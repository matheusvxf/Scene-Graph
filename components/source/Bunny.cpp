#include "Bunny.h"
#include "Utility.h"
#include "Window.h"
#include "Config.h"

#include <cmath>
#include <iostream>

static std::string file = "Resources/bunny.xyz";

Bunny::Bunny() : PointImageObject(file)
{
    reset();
#if (PROJECT_2_PRINT == TRUE)
    getMatrix().print("Bunny translate and scale matrix");
    std::cout << "Max x: " << x_max << " Max y: " << y_max << " Max z: " << z_max << std::endl;
    std::cout << "Min x: " << x_min << " Min y: " << y_min << " Min z: " << z_min << std::endl;
#endif
}

Bunny::~Bunny() {}

void Bunny::update()
{
    Object::update();
}

void Bunny::render()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(15.0, double(Window::width) / (double)Window::height, 1.0, 1000.0); // set perspective projection viewing frustum
    glMultMatrixd(camera.getGLMatrix());
    PointImageObject::render();
}