#pragma once

#include "Vector4.h"
#include "Geode.h"
#include "Group.h"

#include <vector>

class Plane
{
public:
    Plane();
    Plane(const Vector4& normal, const Vector4& center);
	Vector4 normal;
	Vector4 center;
};

class World :
    Group
{
public:
    World();
    ~World();

    void init_robot();
    void init_robot_army();
    void init_spot_light();
    void change_scene();

    void draw(const Matrix4& C);
    void update();
    static void calculate_frustum();
    static BOOLEAN is_in_view_frostum(const Node& object);
};