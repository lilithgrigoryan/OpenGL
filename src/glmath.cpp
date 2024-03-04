#include "../include/glmath.h"

namespace gl_scene
{
    Matrix4f scale(float s)
    {
        Matrix4f res(s, 0.0f, 0.0f, 0.0f,
                     0.0f, s, 0.0f, 0.0f,
                     0.0f, 0.0f, s, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f);

        return res;
    }

    Matrix4f rotationY(float angle)
    {
        float cosA = cosf(angle);
        float sinA = sinf(angle);
        Matrix4f res(cosA, 0.0f, sinA, 0.0f,
                     0.0f, 1.0f, 0.0f, 0.0f,
                     -sinA, 0.0f, cosA, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f);

        return res;
    }

    Matrix4f rotationX(float angle)
    {
        float cosA = cosf(angle);
        float sinA = sinf(angle);
        Matrix4f res(1.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, cosA, -sinA, 0.0f,
                     0.0f, sinA, cosA, 1.0f,
                     0.0f, 0.0f, 0.0f, 1.0f);

        return res;
    }

    Matrix4f rotationZ(float angle)
    {
        float cosA = cosf(angle);
        float sinA = sinf(angle);
        Matrix4f res(cosA, 0.0f, sinA, 0.0f,
                     0.0f, 1.0f, 0.0f, 0.0f,
                     -sinA, 0.0f, cosA, 0.0f,
                     0.0f, 0.0f, 0.0f, 1.0f);

        return res;
    }

    Matrix4f rotation(Vector3f n, float angle)
    {
        float cosA = cosf(angle);
        float sinA = sinf(angle);

        float r11 = cosA + n.x * n.x * (1 - cosA);
        float r12 = n.x * n.y * (1 - cosA) - n.z * sinA;
        float r13 = n.x * n.z * (1 - cosA) + n.y * sinA;
        float r14 = 0.0f;

        float r21 = n.x * n.y * (1 - cosA) + n.z * sinA;
        float r22 = cosA + n.y * n.y * (1 - cosA);
        float r23 = n.y * n.z * (1 - cosA) - n.x * sinA;
        float r24 = 0.0f;

        float r31 = n.z * n.x * (1 - cosA) - n.y * sinA;
        float r32 = n.z * n.y * (1 - cosA) + n.x * sinA;
        float r33 = cosA + n.z * n.z * (1 - cosA);
        float r34 = 0.0f;

        float r41 = 0.0f;
        float r42 = 0.0f;
        float r43 = 0.0f;
        float r44 = 1.0f;

        Matrix4f res(r11, r12, r13, r14,
                     r21, r22, r23, r34,
                     r31, r32, r33, r34,
                     r41, r42, r43, r44);

        return res;
    }

    Matrix4f translation(Vector3f pos)
    {
        Matrix4f Translation(1.0f, 0.0f, 0.0f, pos.x,
                             0.0f, 1.0f, 0.0f, pos.y,
                             0.0f, 0.0f, 1.0f, pos.z,
                             0.0f, 0.0f, 0.0f, 1.0f);

        return Translation;
    }
}