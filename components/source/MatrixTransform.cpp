#include "MatrixTransform.h"

MatrixTransform::MatrixTransform() : MatrixTransform(nullptr) {}

MatrixTransform::MatrixTransform(Matrix4 *M)
{
    this->M = M;
}

MatrixTransform::~MatrixTransform()
{
    delete M;
}

void MatrixTransform::setTransform(Matrix4* M)
{
    this->M = M;
}

void MatrixTransform::draw(const Matrix4& C)
{
    Group::draw(C * *M);
}
