#pragma once

#include "Vector3f.h"

namespace gl_scene
{
    class Material
    {
    private:
        Vector3f AmbientColor_;

    public:
        Material(Vector3f AmbientColor = Vector3f(0.5f, 0.5f, 0.5f)) : AmbientColor_(AmbientColor){};
        ~Material(){};
        const Vector3f &AmbientColor() const { return AmbientColor_; };
        Vector3f &AmbientColor() { return AmbientColor_; }
    };
}