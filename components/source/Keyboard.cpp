#include "Object.h"
#include "Keyboard.h"
#include "main.h"
#include "Config.h"
#include "Utility.h"

#include <iostream>
#include "GL/GLU.h"

void Keyboard::keyPressCb(unsigned char key, INT32 x, INT32 y)
{
    Object *object;
    static Vector4 move_left(-10, 0, 0);
    static Vector4 move_right(10, 0, 0);
    static Vector4 move_up(0, 10, 0);
    static Vector4 move_down(0, -10, 0);
    static Vector4 move_in(0, 0, -10);
    static Vector4 move_out(0, 0, 10);
#if (DEBUG == TRUE)
    std::cout << "Press(" << key2str(key) << ")" << std::endl;
#endif
    object = *Globals::curr_obj;
    if (object == nullptr)
    {
#if (DEBUG == TRUE)
        std::cout << "No object!" << std::endl;
#endif
        object = &Globals::cube;
    }
    switch (key)
    {
    case press::b:
        Globals::draw_bounding_sphere = !Globals::draw_bounding_sphere;
        break;
    case press::c:
        Globals::culling = !Globals::culling;
    case press::t:
        object->spinToggle();
        break;
    case press::x:
        object->apply_force(move_left);
        break;
    case press::X:
        object->apply_force(move_right);
        break;
    case press::y:
        object->apply_force(move_down);
        break;
    case press::Y:
        object->apply_force(move_up);
        break;
    case press::z:
        object->apply_force(move_in);
        break;
    case press::Z:
        object->apply_force(move_out);
        break;
    case press::r:
        object->reset();
        Globals::camera.reset();
        break;
    case press::o:
        object->rotateZ(5);
        break;
    case press::O:
        object->rotateZ(-5);
        break;
    case press::p:
        object->rotateX(5);
        break;
    case press::P:
        object->rotateX(-5);
        break;
    case press::l:
        object->rotateY(5);
        break;
    case press::L:
        object->rotateY(-5);
        break;
    case press::s:
        object->addScale(-1);
        break;
    case press::S:
        object->addScale(1);
        break;
    case press::h:
        object->apply_force(Vector4(rand_int(-300, 300), rand_int(0, 300), rand_int(-300, 300)));
        break;
    case press::k1:
        Globals::scene = tScene::tOneRobot;
        Globals::world->change_scene();
        break;
    case press::k2:
        Globals::scene = tScene::tRobotArm;
        Globals::world->change_scene();
        break;
    case press::k3:
        Globals::scene = tScene::tSpotLight;
        Globals::world->change_scene();
        break;
    }
}

void Keyboard::specialFuncCb(int key, int x, int y){
    Object *object = *Globals::curr_obj;
#if (DEBUG == TRUE)
    std::cout << "Press(" << key2str(key) << ")" << std::endl;
#endif
    switch (key)
    {
    case GLUT_KEY_F1:
        object = (Object*)&Globals::cube;
        break;
    case GLUT_KEY_F2:
        object = (Object*)&Globals::house;
        Globals::house.setCamera(Vector4(0, 10, 10), Vector4(0, 0, 0), Vector4(0, 1, 0));
        break;
    case GLUT_KEY_F3:
        object = (Object*)&Globals::house;
        Globals::house.setCamera(Vector4(-15, 5, 10), Vector4(-5, 0, 0), Vector4(0, 1, 0.5));
        break;
    case GLUT_KEY_F4:
        object = (Object*)&Globals::bunny;
        break;
    case GLUT_KEY_F5:
        object = (Object*)&Globals::dragon;
        break;
    }
}

std::string Keyboard::key2str(UINT8 key)
{
    switch (key)
    {
    case press::b: return "b";
    case press::h: return "h";
    case press::l: return "l";
    case press::L: return "L";
    case press::o: return "o";
    case press::O: return "O";
    case press::p: return "p";
    case press::P: return "P";
    case press::r: return "r";
    case press::s: return "s";
    case press::S: return "S";
    case press::t: return "t";
    case press::x: return "x";
    case press::X: return "X";
    case press::z: return "z";
    case press::Z: return "Z";
    case press::y: return "y";
    case press::Y: return "Y";
    case press::k1: return "1";
    case press::k2: return "2";
    case press::k3: return "3";
    case GLUT_KEY_F1: return "F1";
    case GLUT_KEY_F2: return "F2";
    case GLUT_KEY_F3: return "F3";
    case GLUT_KEY_F4: return "F4";
    case GLUT_KEY_F5: return "F5";
    default: return "Unknown key!";
    }
}