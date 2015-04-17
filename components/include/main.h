#ifndef _MAIN_H_
#define _MAIN_H_

#include "Camera.h"
#include "Dragon.h"
#include "Bunny.h"
#include "Cube.h"
#include "House.h"
#include "Robot.h"
#include "World.h"
#include "GL\glut.h"

enum tScene
{
    tSpotLight,
    tOneRobot,
    tRobotArm
};

namespace Globals
{
    extern Camera camera;
    extern World* world;
    extern Cube cube;
    extern House house;
    extern Bunny bunny;
    extern Dragon dragon;
    extern Object **curr_obj;
    extern Robot *robot;
    extern GLuint texture[128];
    extern BOOLEAN draw_bounding_sphere;
    extern BOOLEAN culling;
    extern double FOV;
    extern double near;
    extern double far;
    extern double aspect;
    extern tScene scene;
};

#endif