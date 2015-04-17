#pragma once

#include "Camera.h"
#include "Object.h"

#include "GL/glut.h"

class PointImageObject :
    public Object
{
protected:
    GLdouble x_max, y_max, z_max;
    GLdouble x_min, y_min, z_min;
    Camera camera;
protected:
    GLdouble* vertices;
    GLdouble* normals;
    UINT32 n_points;

    
public:
    PointImageObject();
    PointImageObject(std::string file);
    ~PointImageObject();

    BOOLEAN load_file(const char *f_name);
    virtual void render();
    virtual void reset();
};