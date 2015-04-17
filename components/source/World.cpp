#include "World.h"
#include "Utility.h"
#include "main.h"
#include "MatrixTransform.h"
#include "MotionTransform.h"
#include "IUpdatable.h"
#include "Geode.h"
#include "Wall.h"

#include <chrono>
#include <iostream>

static Plane view_frustum[6];
static double near_plane_height, near_plane_width;

Plane::Plane() {}

Plane::Plane(const Vector4& normal, const Vector4& center)
{
    this->normal = normal;
    this->center = center;
    this->normal.normalize();
}

World::World()
{
    calculate_frustum();
    change_scene();
}

World::~World() {}

void World::init_robot()
{
    MatrixTransform *floor;
    Globals::robot = new Robot();
    floor = new MatrixTransform(new Matrix4(Matrix4::tTranslate, Vector3(-10, 2.3, 0)));
    floor->add_child(Globals::robot);
    this->add_child(floor);
}

void World::init_robot_army()
{
    MatrixTransform *transf;
    for (INT32 x = -80; x < 50; x += 4)
    {
        for (INT32 z = -80; z < 50; z += 4)
        {
            transf = new MatrixTransform(new Matrix4(Matrix4::MatrixType::tTranslate, Vector3(x, 0.0, z)));
            transf->add_child(new Robot());
            this->add_child(transf);
        }
    }
}

void World::init_spot_light()
{
    static UINT32 wall_slices = 1024;
    MatrixTransform *transf;
    Matrix4 *translate, *rotX, *rotY;

    init_robot();

    Globals::camera.set_center_of_proj(Vector4(5, 20, 50));
    Globals::camera.look_at(Vector4(0, 0, -1));
    Globals::camera.set_up(Vector4(0, 1, 0));

    /* Floor */
    transf = new MatrixTransform();
    rotX = new Matrix4();
    rotX->makeRotateX(-90);
    transf->setTransform(rotX);
    transf->add_child(new Wall(50, 50, wall_slices));
    this->add_child(transf);

    /* Front */
    transf = new MatrixTransform();
    translate = new Matrix4();
    translate->makeTranslate(0, 25, -25);
    transf->setTransform(translate);
    transf->add_child(new Wall(50, 50, wall_slices));
    this->add_child(transf);

    /* Left */
    transf = new MatrixTransform();
    translate = new Matrix4();
    rotY = new Matrix4();
    rotY->makeRotateY(90);
    translate->makeTranslate(-25, 25, 0);
    transf->setTransform((*translate * *rotY).clone());
    transf->add_child(new Wall(50, 50, wall_slices));
    this->add_child(transf);

    delete(translate);
    delete(rotY);

    /* Right */
    transf = new MatrixTransform();
    translate = new Matrix4();
    rotY = new Matrix4();
    rotY->makeRotateY(-90);
    translate->makeTranslate(25, 25, 0);
    transf->setTransform((*translate * *rotY).clone());
    transf->add_child(new Wall(50, 50, wall_slices));
    this->add_child(transf);

    delete(translate);
    delete(rotY);
}

void World::change_scene()
{
    Globals::camera.set_center_of_proj(Vector4(5, 10, 20));
    Globals::camera.look_at(Vector4(0, 0, -1));
    Globals::camera.set_up(Vector4(0, 1, 0));

    this->remove_child();
    Globals::robot = nullptr;
    if (Globals::scene == tScene::tOneRobot)
        init_robot();
    else if (Globals::scene == tScene::tRobotArm)
        init_robot_army();
    else if (Globals::scene == tScene::tSpotLight)
        init_spot_light();
}

void World::draw(const Matrix4& C)
{
    static GLfloat light_position[] = { 1.0, 10.0, 1.0, 1.0 };
    static GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
    static GLfloat light_diffuse[] = { 0.3, 0.3, 0.3, 1.0 };
    static time_t now, last_frame;
    double seconds;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glLightfv(GL_LIGHT7, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT7, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT7, GL_POSITION, light_position);
    glEnable(GL_LIGHT7);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);

    Group::draw(C);

    time(&now);
    seconds = difftime(now, last_frame);
    last_frame = now;

#if (DEBUG_VIEW_FRUSTUM_CULLING == TRUE)
    char buffer[128];
    sprintf_s(buffer, "Culling %d of %d", Geode::hiden, Geode::counter);
    drawString(GLUT_BITMAP_HELVETICA_18, buffer, 0,0, 0);
#endif
}

void World::update()
{
    static Vector3 robot_move(0, 0, -10.0);
    IUpdatable *object;

#if (DEBUG == TRUE)  
    static std::chrono::system_clock::time_point last_frame;
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = now - last_frame;
    UINT64 microseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();

    std::cout << "fps: " << 1.0 / ((double)microseconds / 1000000000.0) << std::endl;
    last_frame = now;
#endif

    if (Globals::scene == tOneRobot)
        Globals::robot->update();
    else if (Globals::scene == tRobotArm)
    {
        foreach(IUpdatable::list, it)
        {
            object = *it;
            object->apply_force(robot_move);
            object->update();
        }
    }
    else if (Globals::scene == tSpotLight)
    {
        Globals::robot->update();

        /* Robot walking in circle */
        Globals::robot->apply_force(Vector3(0, 0, 40));
        Globals::robot->rotateY(0.6);
    }
}

BOOLEAN World::is_in_view_frostum(const Node& object)
{
    static Vector4 center;
    double distance;
    double distance_module;
    double radius;
    for (register UINT8 i = 0; i < 6; ++i)
    {
        radius = object.get_bounding_sphere_radius();
        center = object.get_bounding_sphere_center();
        distance = distance2plane(view_frustum[i], center);
        distance_module = distance > 0 ? distance : -distance;
        if (distance > radius)
            return FALSE;
    }
    return TRUE;
}

void World::calculate_frustum()
{
    double dx, dy, x, y, z, FOV_w;
    Matrix4 rotX, rotY;
    Vector4 normal;

    near_plane_height = 2.0 * Globals::near * tan(degree2rad(Globals::FOV / 2.0));
    near_plane_width = Globals::aspect * near_plane_height;
    z = -(Globals::near + Globals::far) / 2.0;
    FOV_w = 2 * rad2degree(atan2(near_plane_width / 2.0, Globals::near));
    dy = tan(degree2rad(Globals::FOV / 2.0)) * (-z - Globals::near);
    dx = tan(degree2rad(FOV_w / 2.0)) * (-z - Globals::near);

    /* Near plane */
    view_frustum[0] = Plane(Vector4(0, 0, 1), Vector4(0, 0, -Globals::near));
    /* Far plane */
    view_frustum[1] = Plane(Vector4(0, 0, -1), Vector4(0, 0, -Globals::far));
    /* Right plane */
    rotY.makeRotateY(-FOV_w / 2.0);
    normal.set(1, 0, 0, 0);
    normal = rotY * normal;  
    y = 0;
    x = near_plane_width / 2.0 + dx;
    view_frustum[2] = Plane(normal, Vector4(x, y, z));
    /* Left plane */
    rotY.makeRotateY(FOV_w / 2.0);
    normal.set(-1, 0, 0, 0);
    normal = rotY * normal;
    y = 0;
    x = -x;
    view_frustum[3] = Plane(normal, Vector4(x, y, z));
    /* Up plane */
    y = near_plane_height /2.0 + dy;
    normal.set(0, 1, 0, 0);
    rotX.makeRotateX(Globals::FOV / 2.0);
    normal = rotX * normal;
    view_frustum[4] = Plane(normal, Vector4(0, y, z));
    /* Down plane */
    y = -y;
    normal.set(0, -1, 0, 0);
    rotX.makeRotateX(-Globals::FOV / 2.0);
    normal = rotX * normal;
    view_frustum[5] = Plane(Vector4(normal), Vector4(0, y, z));
}