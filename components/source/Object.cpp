#include "Object.h"
#include "DataType.h"
#include "Config.h"

#include <iostream>
#include <string>
#include <sstream>

Object::Object()
{
    reset();
    spin_clockwise = FALSE;
    model2world.identity();
}

Object::~Object()
{
}

void Object::update()
{
    force -= velocity * friction;
    acceleration += force / mass;
    velocity += acceleration * dt;
    position += velocity * dt;
    force.set(0, 0, 0, 0);
    acceleration.set(0, 0, 0, 0);
    
    updateMatrix();
}

void Object::render()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMultMatrixd(this->model2world.getPointer());
}

Matrix4& Object::getMatrix()
{
    return model2world;
}

void Object::rotateX(double deg)
{
    angleX += deg;
    if (angleX > 360.0 || angleX < -360.0)
        angleX = 0.0;
}

void Object::rotateY(double deg)   // deg is in degrees
{
    angleY += deg;
    if (angleY > 360.0 || angleY < -360.0)
        angleY = 0.0;
}

void Object::rotateZ(double deg)
{
    angleZ += deg;
    if (angleZ > 360.0 || angleZ < -360.0)
        angleZ = 0.0;
}

Matrix4& Object::getRotationMatrixX()
{
    return m_rotateX;
}

Matrix4& Object::getRotationMatrixY()
{
    return m_rotateY;
}

Matrix4& Object::getRotationMatrixZ()
{
    return m_rotateZ;
}

void Object::addScale(double sca)
{
    scale += sca;
    if (scale <= 0.1)
        scale = 0.1;
}

void Object::updateMatrix()
{
    /* Update translation and scale */
    Matrix4 m_scale, m_translate;
    m_translate.makeTranslate(position[0], position[1], position[2]);
    m_scale.makeScale(scale, scale, scale);
    model2world = m_translate * m_scale;
    
    /* Update rotations */
    m_rotateX.makeRotateX(angleX);
    m_rotateY.makeRotateY(angleY);
    m_rotateZ.makeRotateZ(angleZ);
    
    model2world = model2world * m_rotateX * m_rotateY * m_rotateZ;
}

void Object::spinToggle()
{
    spin_clockwise = !spin_clockwise;
}

void Object::apply_force(const Vector3& force)
{
    this->force += model2world * force;
}

void Object::apply_acceleration(const Vector3& acceleration)
{
    this->acceleration += model2world * acceleration;
}

Vector4& Object::getPosition()
{
    return this->position;
}

double Object::getSpeed() const
{
    return this->velocity.length();
}

void Object::reset()
{
    position.set(0, 0, 0, 1);
    force = velocity = position;
    angleX = 0.0;
    angleY = 0.0;
    angleZ = 0.0;
    scale = 1.0;
    mass = 0.5;
    friction = 4;
}

void Object::print()
{
    std::cout << toString() << std::endl;
}

std::string Object::toString()
{
    std::stringstream buffer;
    buffer << "position: " << this->position.toString() << std::endl;
#if (DEBUG == TRUE)
    buffer << "matrix: " << std::endl << this->model2world.toString();
#endif
    return buffer.str();
}