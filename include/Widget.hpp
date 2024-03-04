#pragma once

#include <string>
#include <GL/glew.h>

#include "WidgetType.hpp"
#include "Vector3f.h"
#include "Matrix4f.h"

namespace gl_scene
{
    class Widget
    {
    private:
        std::string id_;
        int triangleNumber_;
        GLuint VAO_;
        GLuint VertexVB0_;
        GLuint ColorVBO_;
        GLuint IBO_;
        WidgetType type_;
        float Scale_;
        Vector3f Position_;
        Vector3f RotationAxis_;
        float RotationAngle_;
        float RotationSpeed_;
        bool isVisible_;

        Matrix4f ScaleMat() const;
        Matrix4f RotationMat() const;
        Matrix4f TranslationMatrix() const;

    public:
        Widget(std::string id,
               int triangleNumber,
               GLuint &VAO,
               GLuint &VertexVBO,
               GLuint &ColorVBO,
               GLuint &IBO,
               WidgetType WidgetType) : id_(id),
                                        triangleNumber_(triangleNumber),
                                        VAO_(VAO),
                                        VertexVB0_(VertexVB0_),
                                        ColorVBO_(ColorVBO),
                                        type_(WidgetType)
        {
            Scale_ = 1.0f;
            Position_ = Vector3f(0.0f, 0.0f, 0.0f);
            RotationAxis_ = Vector3f(0.0f, 1.0f, 0.0f),
            RotationSpeed_ = 0.02f;
            RotationAngle_ = 0.0f;
            isVisible_ = false;
        };

        const std::string &id() const { return id_; }

        const GLuint &VAO() const { return VAO_; }
        const int &TrianglesNumber() const { return triangleNumber_; }

        float &Scale() { return Scale_; }
        const float &Scale() const { return Scale_; }

        Vector3f &Position() { return Position_; }
        const Vector3f &Position() const { return Position_; }

        Vector3f &RotationAxis() { return RotationAxis_; }
        const Vector3f &RotationAxis() const { return RotationAxis_; }

        float &RotationAngle() { return RotationAngle_; }
        const float &RotationAngle() const { return RotationAngle_; }

        float &RotationSpeed() { return RotationSpeed_; }
        const float &RotationSpeed() const { return RotationSpeed_; }

        bool &isVisible() { return isVisible_; }
        const bool &isVisible() const { return isVisible_; }

        Matrix4f TransformationMat() const;
        void Rotate();

        friend bool operator<(const Widget &lhs, const Widget &rhs) { return rhs.id() < lhs.id(); }
        friend bool operator>(const Widget &lhs, const Widget &rhs) { return rhs.id() > lhs.id(); }
        friend bool operator==(const Widget &lhs, const Widget &rhs) { return rhs.id() == lhs.id(); }

        ~Widget()
        {
            std::cout << "Cleaning up Widget" << std::endl;
            glDeleteVertexArrays(1, &VAO_);
            glDeleteVertexArrays(1, &VertexVB0_);
            glDeleteVertexArrays(1, &ColorVBO_);
            glDeleteVertexArrays(1, &IBO_);
        };
    };
} // namespace gl_scene
