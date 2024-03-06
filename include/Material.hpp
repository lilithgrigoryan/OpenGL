#pragma once

#include "Vector3f.h"

namespace gl_scene
{
    class Material
    {
    private:
        Vector3f AmbientColor_;
        Vector3f DiffuseColor_;

    public:
        Material(Vector3f AmbientColor = Vector3f(0.5f, 0.5f, 0.5f),
        Vector3f DiffuseColor = Vector3f(0.5f, 0.5f, 0.5f)) : AmbientColor_(AmbientColor), DiffuseColor_(DiffuseColor){};
        ~Material(){};
        const Vector3f &AmbientColor() const { return AmbientColor_; };
        Vector3f &AmbientColor() { return AmbientColor_; }
        const Vector3f &DiffuseColor() const { return DiffuseColor_; };
        Vector3f &DiffuseColor() { return DiffuseColor_; }
    };
}