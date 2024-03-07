#pragma once

#include "Vector3f.h"

namespace gl_scene
{
    class Material
    {
    private:
        Vector3f AmbientColor_;
        Vector3f DiffuseColor_;
        Vector3f SpecularColor_;
        float SpecularColorExp_;

    public:
        Material(Vector3f AmbientColor = Vector3f(0.5f, 0.5f, 0.5f),
                 Vector3f DiffuseColor = Vector3f(0.5f, 0.5f, 0.5f),
                 Vector3f SpecularColor = Vector3f(0.5f, 0.5f, 0.5f),
                 float SpecularColorExp = 10.0f) : AmbientColor_(AmbientColor),
                                           DiffuseColor_(DiffuseColor),
                                           SpecularColor_(SpecularColor),
                                           SpecularColorExp_(SpecularColorExp){};

        const Vector3f &AmbientColor() const { return AmbientColor_; };
        Vector3f &AmbientColor() { return AmbientColor_; }
        const Vector3f &DiffuseColor() const { return DiffuseColor_; };
        Vector3f &DiffuseColor() { return DiffuseColor_; }
        const Vector3f &SpecularColor() const { return SpecularColor_; };
        Vector3f &SpecularColor() { return SpecularColor_; }
        const float &SpecularColorExp() const { return SpecularColorExp_; }
        float &SpecularColorExp() { return SpecularColorExp_; }
    };
}