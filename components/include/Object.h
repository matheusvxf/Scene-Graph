#pragma once

#include "Vector4.h"
#include "Matrix4.h"
#include "IUpdatable.h"

class Object :
    public IUpdatable
{
public:
    enum ObjectType
    {
        tCube,
        tBall,
        tHouse
    };
    ObjectType obj_type;
private:
    BOOLEAN spin_clockwise;
    Matrix4 m_rotateX;
    Matrix4 m_rotateY;
    Matrix4 m_rotateZ;
    GLdouble* GL_rotateX;
    GLdouble* GL_rotateY;
    GLdouble* GL_rotateZ;


protected:
    double angleX;
    double angleY;                   // rotation angle [degrees]
    double angleZ;
    double scale;

    Matrix4 model2world;            // model matrix (transforms model coordinates to world coordinates)
    Vector4 position;
    Vector4 velocity;
    Vector4 acceleration;
    Vector4 force;
    double mass;
    double friction;

public:
    Object();
    virtual ~Object();

    virtual void render();

    virtual void update();
    virtual void reset();
    virtual void print();
    virtual std::string toString();

    Matrix4& getMatrix();
    Matrix4& getRotationMatrixX();
    Matrix4& getRotationMatrixY();
    Matrix4& getRotationMatrixZ();
    GLdouble* getGLrotateX();
    GLdouble* getGLrotateY();
    GLdouble* getGLrotateZ();
    Vector4& getPosition();
    double getSpeed() const;
    void rotateX(double angle);
    void rotateY(double angle);      // spin cube [degrees]
    void rotateZ(double angle);
    void addScale(double sca);
    void updateMatrix();
    void spinToggle();
    void apply_force(const Vector3& force);
    void apply_acceleration(const Vector3& acceleration);
};

