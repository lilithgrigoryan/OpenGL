#pragma once
#include <iostream>

namespace gl_scene
{
    struct Vector3f
    {
        float x;
        float y;
        float z;

        Vector3f(){};
        Vector3f(float _x, float _y, float _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        Vector3f &Normalize();
        Vector3f Cross(const Vector3f &v) const;
        float Dot(const Vector3f &v) const;
        float Distance(const Vector3f &v) const;
        float Length() const;

        Vector3f &operator+=(const Vector3f &op)
        {
            x += op.x;
            y += op.y;
            z += op.z;

            return *this;
        }

        Vector3f &operator-=(const Vector3f &op)
        {
            x -= op.x;
            y -= op.y;
            z -= op.z;

            return *this;
        }

        Vector3f &operator*=(float f)
        {
            x *= f;
            y *= f;
            z *= f;

            return *this;
        }

        inline Vector3f operator+(const struct Vector3f &op)
        {
            Vector3f v(x + op.x, y + op.y, z + op.z);

            return v;
        }

        inline Vector3f operator-(const struct Vector3f &op)
        {
            Vector3f v(x - op.x, y - op.y, z - op.z);

            return v;
        }

        inline Vector3f operator*(float f)
        {
            Vector3f v(x * f, y * f, z * f);

            return v;
        }

        friend std::ostream &operator<<(std::ostream &stream, const Vector3f &v)
        {
            stream << v.x << " " << v.y << " " << v.z << std::endl;

            return stream;
        }
    };
}