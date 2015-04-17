#pragma once
#include "PointImageObject.h"
#include "Camera.h"

class Bunny :
    public PointImageObject
{
public:
    Bunny();
    ~Bunny();

    virtual void update();
    virtual void render();
};

