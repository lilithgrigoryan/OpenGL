#pragma once

#include <iostream>
#include "../include/Vector3f.h"
#include "../include/Matrix4f.h"

namespace gl_scene
{
    class Matrix3f
    {
    private:
    public:
        float mat_[3][3];
        Matrix3f();
        Matrix3f(float a11, float a12, float a13,
                 float a21, float a22, float a23,
                 float a31, float a32, float a33);
        Matrix3f(Matrix4f m4);

        void setZero();
        void setIdentity();

        friend std::ostream &operator<<(std::ostream &stream, const Matrix3f &m)
        {
            for (int row = 0; row < 3; row++)
                stream << m.mat_[row][0] << " " << m.mat_[row][1] << " " << m.mat_[row][2] << std::endl;
            stream << std::endl;

            return stream;
        }

        inline struct Matrix3f operator*(const struct Matrix3f &op)
        {
            Matrix3f m;
            for (unsigned int i = 0; i < 3; i++)
            {
                for (unsigned int j = 0; j < 3; j++)
                {
                    m.mat_[i][j] = mat_[i][0] * op.mat_[0][j] +
                                   mat_[i][1] * op.mat_[1][j] +
                                   mat_[i][2] * op.mat_[2][j];
                }
            }

            return m;
        }

        inline struct Vector3f operator*(const Vector3f &op)
        {
            float x = mat_[0][0] * op[0] + mat_[0][1] * op[1] + mat_[0][2] * op[2];
            float y = mat_[1][0] * op[0] + mat_[1][1] * op[1] + mat_[1][2] * op[2];
            float z = mat_[2][0] * op[0] + mat_[2][1] * op[1] + mat_[2][2] * op[2];

            return Vector3f(x, y, z);
        }

        Matrix3f transpose() const;
    };
}