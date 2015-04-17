#include "PointImageObject.h"
#include "Config.h"
#include "Utility.h"
#include "main.h"
#include "Window.h"

#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

static UINT32 count_lines(const char *f_name);

PointImageObject::PointImageObject()
{
    reset();
}

PointImageObject::PointImageObject(std::string file)
{
    n_points = count_lines(file.c_str());
    load_file(file.c_str());
    reset();
}

PointImageObject::~PointImageObject()
{
    free(vertices);
    free(normals);
}

UINT32 count_lines(const char *f_name)
{
    FILE* fs;
    UINT32 lines;
    GLdouble tmp;
    lines = 0;

    if (fopen_s(&fs, f_name, "r") == 0)
        while (fscanf_s(fs, "%lf %lf %lf %lf %lf %lf", &tmp, &tmp, &tmp, &tmp, &tmp, &tmp) != EOF)
            lines++; 
    return lines;
}

BOOLEAN PointImageObject::load_file(const char *f_name)
{
    FILE* fs;
    UINT32 counter;
    GLdouble px, py, pz;
    GLdouble nx, ny, nz;
    vertices = (GLdouble*)malloc(sizeof(GLdouble)* n_points * 3);
    normals = (GLdouble*)malloc(sizeof(GLdouble)* n_points * 3);
    x_min = y_min = z_min = 10e30;
    x_max = y_max = z_max = -10e30;
    counter = 0;

    if (fopen_s(&fs, f_name, "r") == 0)
    {
        for (UINT32 i = 0; i < n_points; ++i)
        {
            fscanf_s(fs, "%lf %lf %lf %lf %lf %lf", &px, &py, &pz, &nx, &ny, &nz);
            x_min = min(x_min, px);
            y_min = min(y_min, py);
            z_min = min(z_min, pz);
            x_max = max(x_max, px);
            y_max = max(y_max, py);
            z_max = max(z_max, pz);
            vertices[counter] = px;
            vertices[counter + 1] = py;
            vertices[counter + 2] = pz;
            normals[counter] = nx;
            normals[counter + 1] = ny;
            normals[counter + 2] = nz;
            counter += 3;
        }
        return TRUE;
    }
    return FALSE;
}

void PointImageObject::render()
{
    Object::render(); 

    /* Rotation of particles to face the camera */
    Vector4 color;
    GLdouble *GL_rotX, *GL_rotY, *GL_rotZ;
    GL_rotX = (GLdouble*)Matrix4().makeRotateX(-angleX).transpose().getPointer();
    GL_rotY = (GLdouble*)Matrix4().makeRotateY(-angleY).transpose().getPointer();
    GL_rotZ = (GLdouble*)Matrix4().makeRotateZ(-angleZ).transpose().getPointer();

    /* Setup */
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glDepthMask(GL_FALSE);

    glColorPointer(3, GL_DOUBLE, 0, normals);
    glVertexPointer(3, GL_DOUBLE, 0, vertices);
    glNormalPointer(GL_DOUBLE, 0, normals);
    
    glMatrixMode(GL_MODELVIEW);
    for (UINT32 i = 0; i < n_points; ++i)
    {
        glPushMatrix();
        glNormal3dv(normals + i * 3);
#if (COLOR_BY_DISTANCE == TRUE)
        color[0] = vertices[i * 3];
        color[1] = vertices[i * 3 + 1];
        color[2] = vertices[i * 3 + 2];
        color.normalize();
#endif

        /* Translate the square to its right place */
        glTranslated(vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]);
        /* Rotate the square to face the camera */
        glMultMatrixd(GL_rotX);
        glMultMatrixd(GL_rotY);
        glMultMatrixd(GL_rotZ);

        glColor3d(color[0], color[1], color[2]);
        glBindTexture(GL_TEXTURE_2D, Globals::texture[0]);

        glDisable(GL_TEXTURE_2D);
        GLdouble PARTICLE_SIZE = 0.001;
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-PARTICLE_SIZE, PARTICLE_SIZE, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-PARTICLE_SIZE, -PARTICLE_SIZE, 0.0f);
        glTexCoord2f(1.0f, 1.0f); 
        glVertex3f(PARTICLE_SIZE, -PARTICLE_SIZE, 0.0f);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(PARTICLE_SIZE, PARTICLE_SIZE, 0.0f);
        glEnd();
        glPopMatrix();
    }

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

void PointImageObject::reset()
{
    Object::reset();
    scale = 2.0 * 20 * tan(degree2rad(15.0 / 2)) / (y_max - y_min + (z_max + z_min) * tan(degree2rad(15.0 / 2.0)));
    camera = Camera(Vector4(0, 0, 20.0), Vector4(0, 0, -1), Vector4(0, 1, 0));
    position[0] = -scale * (x_min + x_max) / 2;
    position[1] = -scale * (y_min + y_max) / 2;
    position[2] = -scale * (z_min + z_max) / 2;
    updateMatrix();
}