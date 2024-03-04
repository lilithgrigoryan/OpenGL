#include "../include/Vector3f.h"
#include <cmath>
#include <assert.h>

namespace gl_scene
{
    Vector3f &Vector3f::Normalize()
    {
        float len = Length();

        assert(len != 0);

        x /= len;
        y /= len;
        z /= len;

        return *this;
    }

    Vector3f Vector3f::Cross(const Vector3f &v) const
    {
        const float _x = y * v.z - z * v.y;
        const float _y = z * v.x - x * v.z;
        const float _z = x * v.y - y * v.x;

        return Vector3f(_x, _y, _z);
    }

    float Vector3f::Dot(const Vector3f &v) const
    {
        float ret = x * v.x + y * v.y + z * v.z;
        return ret;
    }

    float Vector3f::Distance(const Vector3f &v) const
    {
        float delta_x = x - v.x;
        float delta_y = y - v.y;
        float delta_z = z - v.z;
        float distance = sqrtf(delta_x * delta_x + delta_y * delta_y + delta_z * delta_z);
        return distance;
    }

    float Vector3f::Length() const
    {
        float len = sqrtf(x * x + y * y + z * z);
        return len;
    }
}