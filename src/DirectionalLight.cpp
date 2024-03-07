#include "../include/DirectionalLight.hpp"
#include "../include/Matrix3f.hpp"

namespace gl_scene
{
    const Vector3f DirectionalLight::calculateLocalDirection(const Matrix4f &LocalToWorldTrasnform) const
    {
        Matrix3f R(LocalToWorldTrasnform[0][0], LocalToWorldTrasnform[0][1], LocalToWorldTrasnform[0][2],
                   LocalToWorldTrasnform[1][0], LocalToWorldTrasnform[1][1], LocalToWorldTrasnform[1][2],
                   LocalToWorldTrasnform[2][0], LocalToWorldTrasnform[2][1], LocalToWorldTrasnform[2][2]);
        Matrix3f RT = R.transpose();
        Vector3f localDiraction = RT * WorldDirection_;

        return localDiraction.Normalize();
    }
}