#include "Light.h"
#include "main.h"

#include "GL\glut.h"

Light::Light()
{
}

Light::~Light()
{
}

void Light::draw(const Matrix4& C)
{
    static Matrix4 transformation;
    transformation = Globals::camera.getMatrix() * C;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLoadMatrixd(transformation.transpose().getPointer());
    render();
}

void Light::render()
{
    static GLfloat direction[] = { 0, -1.0, 0 };
    static GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
    static GLfloat light_diffuse[] = { 0.8, 0.4, 0.1, 1.0 };
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}