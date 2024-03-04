#pragma once

#include <iostream>
#include <cmath>
#include <assert.h>

struct Vector2f
{
    float x;
    float y;

    Vector2f()
    {
    }

    Vector2f(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    friend std::ostream &operator<<(std::ostream &stream, const Vector2f &v)
    {
        stream << v.x << " " << v.y << std::endl;

        return stream;
    }

    float Length() const
    {
        float len = sqrt(x * x + y * y);
        return len;
    }

    void Normalize()
    {
        float len = Length();

        assert(len != 0);

        x /= len;
        y /= len;
    }
};