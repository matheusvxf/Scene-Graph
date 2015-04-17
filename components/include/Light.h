#pragma once

#include "Geode.h"

class Light :
    public Node
{
public:
    Light();
    virtual ~Light();

    void draw(const Matrix4& C);
    void render();
};

