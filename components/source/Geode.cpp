#include "Geode.h"
#include "main.h"
#include "Config.h"
#include "Utility.h"

#include "GL\glut.h"
#include <cmath>
#include <algorithm>
#include <iostream>

UINT32 Geode::counter = 0;
UINT32 Geode::hiden;

Geode::Geode()
{
    this->is_visible = false;
    Geode::hiden++;
    Geode::counter++;
}

Geode::~Geode()
{
    Geode::counter--;
    if (this->is_visible == FALSE)
        hiden--;
}

void Geode::draw(const Matrix4& C)
{
    static Matrix4 transform;
    Vector4 v;
    double max = -1;
    transform = Globals::camera.getMatrix() * C;

    for (register UINT8 i = 0; i < 3; ++i)
    {
        v = transform * bounding_box[i];
        max = std::max(max, v.length_sq());
    }
    this->bounding_sphere_center[0] = transform[0][3];
    this->bounding_sphere_center[1] = transform[1][3];
    this->bounding_sphere_center[2] = transform[2][3];
    this->bounding_sphere_radius = sqrt(max);

    if (Globals::draw_bounding_sphere)
    {
        this->draw_bounding_sphere();
    }

    if (Globals::culling)
    {
        if (World::is_in_view_frostum(*this))
        {
            if (!is_visible)
                hiden--;
            is_visible = true;
        }
        else
        {
            if (is_visible)
                hiden++;
            is_visible = false;
        }
    }
    else
    {
        is_visible = true;
    }

    if (is_visible)
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glLoadMatrixd(transform.transpose().getPointer());
        this->render();
    }
}