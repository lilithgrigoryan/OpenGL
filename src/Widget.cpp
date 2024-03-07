#include "../include/Widget.hpp"
#include "../include/glmath.h"

namespace gl_scene
{
    Matrix4f Widget::ScaleMat() const
    {
        return scale(Scale_);
    }

    Matrix4f Widget::RotationMat() const
    {
        return rotation(RotationAxis_, RotationAngle_);
    }

    Matrix4f Widget::TranslationMatrix() const
    {
        return translation(Position_);
    }

    Matrix4f Widget::TransformationMat() const
    {
        Matrix4f S = ScaleMat();
        Matrix4f T = TranslationMatrix();
        Matrix4f R = RotationMat();

        return T * R * S;
    }

    void Widget::Rotate()
    {
        RotationAngle_ -= RotationSpeed_;
    }
}