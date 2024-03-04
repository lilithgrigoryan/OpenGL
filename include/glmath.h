#pragma once

#include <math.h>
#include "Matrix4f.h"
#include "Vector3f.h"

namespace gl_scene
{
    Matrix4f scale(float s);
    Matrix4f rotationY(float angle);
    Matrix4f rotationX(float angle);
    Matrix4f rotationZ(float angle);
    Matrix4f rotation(Vector3f n, float angle);
    Matrix4f translation(Vector3f pos);
}