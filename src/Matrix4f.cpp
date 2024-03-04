#include "../include/Matrix4f.h"

namespace gl_scene
{
    Matrix4f::Matrix4f(/* args */)
    {
    }

    Matrix4f::~Matrix4f()
    {
    }

    Matrix4f::Matrix4f(float a11, float a12, float a13, float a14,
                       float a21, float a22, float a23, float a24,
                       float a31, float a32, float a33, float a34,
                       float a41, float a42, float a43, float a44)
    {
        mat_[0][0] = a11;
        mat_[0][1] = a12;
        mat_[0][2] = a13;
        mat_[0][3] = a14;

        mat_[1][0] = a21;
        mat_[1][1] = a22;
        mat_[1][2] = a23;
        mat_[1][3] = a24;

        mat_[2][0] = a31;
        mat_[2][1] = a32;
        mat_[2][2] = a33;
        mat_[2][3] = a34;

        mat_[3][0] = a41;
        mat_[3][1] = a42;
        mat_[3][2] = a43;
        mat_[3][3] = a44;
    }

    void Matrix4f::setIdentity()
    {
        mat_[0][0] = 1.0;
        mat_[0][1] = 0.0;
        mat_[0][2] = 0.0;
        mat_[0][3] = 0.0;

        mat_[1][0] = 0.0;
        mat_[1][1] = 1.0;
        mat_[1][2] = 0.0;
        mat_[1][3] = 0.0;

        mat_[2][0] = 0.0;
        mat_[2][1] = 0.0;
        mat_[2][2] = 1.0;
        mat_[2][3] = 0.0;

        mat_[3][0] = 0.0;
        mat_[3][1] = 0.0;
        mat_[3][2] = 0.0;
        mat_[3][3] = 1.0;
    }

    void Matrix4f::setZero()
    {
        mat_[0][0] = 0.0;
        mat_[0][1] = 0.0;
        mat_[0][2] = 0.0;
        mat_[0][3] = 0.0;

        mat_[1][0] = 0.0;
        mat_[1][1] = 0.0;
        mat_[1][2] = 0.0;
        mat_[1][3] = 0.0;

        mat_[2][0] = 0.0;
        mat_[2][1] = 0.0;
        mat_[2][2] = 0.0;
        mat_[2][3] = 0.0;

        mat_[3][0] = 0.0;
        mat_[3][1] = 0.0;
        mat_[3][2] = 0.0;
        mat_[3][3] = 0.0;
    }

    Matrix4f Matrix4f::transpose() const
    {
        Matrix4f res(mat_[0][0], mat_[1][0], mat_[2][0], mat_[3][0],
                     mat_[0][1], mat_[1][1], mat_[2][1], mat_[3][1],
                     mat_[0][2], mat_[1][2], mat_[2][2], mat_[3][2],
                     mat_[0][3], mat_[1][3], mat_[2][3], mat_[3][3]);

        return res;
    }
}