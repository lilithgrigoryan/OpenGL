#pragma once

#include <iostream>
#include "../include/Vector3f.h"

namespace gl_scene
{
    class Matrix4f
    {
    private:
    public:
        float mat_[4][4];
        Matrix4f();
        Matrix4f(float a11, float a12, float a13, float a14,
                 float a21, float a22, float a23, float a24,
                 float a31, float a32, float a33, float a34,
                 float a41, float a42, float a43, float a44);

        void setZero();
        void setIdentity();

        friend std::ostream &operator<<(std::ostream &stream, const Matrix4f &m)
        {
            for (int row = 0; row < 4; row++)
                stream << m.mat_[row][0] << " " << m.mat_[row][1] << " " << m.mat_[row][2] << " " << m.mat_[row][3] << std::endl;
            stream << std::endl;

            return stream;
        }

        inline struct Matrix4f operator*(const struct Matrix4f &op)
        {
            Matrix4f m;
            for (unsigned int i = 0; i < 4; i++)
            {
                for (unsigned int j = 0; j < 4; j++)
                {
                    m.mat_[i][j] = mat_[i][0] * op.mat_[0][j] +
                                   mat_[i][1] * op.mat_[1][j] +
                                   mat_[i][2] * op.mat_[2][j] +
                                   mat_[i][3] * op.mat_[3][j];
                }
            }

            return m;
        }

        inline Vector3f operator[](int i)
        {
            return Vector3f(mat_[i][0], mat_[i][1], mat_[i][2]);
        }

        inline const Vector3f operator[](int i) const
        {
            return Vector3f(mat_[i][0], mat_[i][1], mat_[i][2]);
        }

        Matrix4f transpose() const;
    };
}