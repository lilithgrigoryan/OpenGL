#pragma once

#include "BaseLight.hpp"
#include "Matrix4f.h"

namespace gl_scene
{
    class DirectionalLight : public BaseLight
    {
    private:
        float DiffuseIntensity_ = 0.0f;
        Vector3f WorldDirection_ = Vector3f(0.0f, 0.0f, 0.0f);

    public:
        DirectionalLight(Vector3f Color = Vector3f(1.0f, 1.0f, 1.0f),
                         float AmbientIntensity = 0.75f,
                         float DiffuseIntensity = 0.75f,
                         Vector3f WorldDirection = Vector3f(0.0f, 0.0f, 0.0f)) : BaseLight(Color, AmbientIntensity),
                                                                                 DiffuseIntensity_(DiffuseIntensity),
                                                                                 WorldDirection_(WorldDirection)
        {
        }

        const float &DiffuseIntensity() const { return DiffuseIntensity_; }
        const Vector3f calculateLocalDirection(const Matrix4f &LocalToWorldTrasnform) const;
    };
}