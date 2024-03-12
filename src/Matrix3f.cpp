#include "../include/Matrix3f.hpp"

namespace gl_scene
{
    Matrix3f::Matrix3f()
    {
    }

    Matrix3f::Matrix3f(float a11, float a12, float a13,
                       float a21, float a22, float a23,
                       float a31, float a32, float a33)
    {
        mat_[0][0] = a11;
        mat_[0][1] = a12;
        mat_[0][2] = a13;

        mat_[1][0] = a21;
        mat_[1][1] = a22;
        mat_[1][2] = a23;

        mat_[2][0] = a31;
        mat_[2][1] = a32;
        mat_[2][2] = a33;
    }

    Matrix3f::Matrix3f(Matrix4f m4)
    {
        mat_[0][0] = m4[0][0];
        mat_[0][1] = m4[0][1];
        mat_[0][2] = m4[0][2];

        mat_[1][0] = m4[1][0];
        mat_[1][1] = m4[1][1];
        mat_[1][2] = m4[1][2];

        mat_[2][0] = m4[2][0];
        mat_[2][1] = m4[2][1];
        mat_[2][2] = m4[2][2];
    }

    void Matrix3f::setZero()
    {
        mat_[0][0] = 0.0;
        mat_[0][1] = 0.0;
        mat_[0][2] = 0.0;

        mat_[1][0] = 0.0;
        mat_[1][1] = 0.0;
        mat_[1][2] = 0.0;

        mat_[2][0] = 0.0;
        mat_[2][1] = 0.0;
        mat_[2][2] = 0.0;
    }

    void Matrix3f::setIdentity()
    {
        mat_[0][0] = 1.0;
        mat_[0][1] = 0.0;
        mat_[0][2] = 0.0;

        mat_[1][0] = 0.0;
        mat_[1][1] = 1.0;
        mat_[1][2] = 0.0;

        mat_[2][0] = 0.0;
        mat_[2][1] = 0.0;
        mat_[2][2] = 1.0;
    }

    Matrix3f Matrix3f::transpose() const
    {
        Matrix3f res(mat_[0][0], mat_[1][0], mat_[2][0],
                     mat_[0][1], mat_[1][1], mat_[2][1],
                     mat_[0][2], mat_[1][2], mat_[2][2]);

        return res;
    };
}