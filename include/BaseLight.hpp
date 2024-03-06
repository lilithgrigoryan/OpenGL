#pragma once

#include "../include/Vector3f.h"

namespace gl_scene
{
    class BaseLight
    {
    private:
        Vector3f Color_;
        float AmbientIntensity_;

    public:
        BaseLight(Vector3f Color = Vector3f(1.0f, 1.0f, 1.0f), float AmbientIntensity = 0.75f) : Color_(Color), AmbientIntensity_(AmbientIntensity)
        {
            std::cout << Color << std::endl;
        }

        const Vector3f &Color() const
        {
            std::cout << "Color base class" << Color_ << std::endl;
            return Color_;
        }
        const float &AmbientIntensity() const { return AmbientIntensity_; }
    };
}
