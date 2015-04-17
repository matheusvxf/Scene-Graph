#pragma once

#include "PointImageObject.h"

class Dragon :
    public PointImageObject
{
public:
    Dragon();
    ~Dragon();

    virtual void reset();
    virtual void update();
    virtual void render();
};

