#pragma once
#include "Node.h"

#include <list>

class Group :
    public Node
{
protected:
    std::list<Node*> children;
public:
    Group();
    virtual ~Group();

    void add_child(Node* child);
    void remove_child();
    virtual void draw(const Matrix4& C);
};

