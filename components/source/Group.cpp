#include "Group.h"
#include "RenderableObject.h"
#include "Utility.h"

Group::Group()
{
}

Group::~Group()
{
    remove_child();
}

void Group::add_child(Node* child)
{
    children.push_back(child);
}

void Group::remove_child()
{
    foreach(children, child)
    {
        delete (*child);
    }
    children.clear();
}

void Group::draw(const Matrix4& C)
{
    Node *node;

    foreach(children, c)
    {
        node = *c;
        node->draw(C);
    }
}