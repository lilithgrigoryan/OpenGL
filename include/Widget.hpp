#pragma once

#include <string>
#include <vector>
#include <GL/glew.h>

#include "WidgetType.hpp"
#include "Vector3f.h"
#include "Matrix4f.h"
#include "Texture.hpp"

namespace gl_scene
{
    class Widget
    {
    private:
        std::string id_;
        int triangleNumber_;

        GLuint VAO_;
        std::vector<GLuint> BufferObjects_;

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
               std::vector<GLuint> BufferObjects,
               WidgetType WidgetType) : id_(id),
                                        triangleNumber_(triangleNumber),
                                        VAO_(VAO),
                                        BufferObjects_(BufferObjects),
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
        std::vector<GLuint> &BufferObjects() { return BufferObjects_; }
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

        friend bool operator<(const Widget &lhs, const Widget &rhs) { return rhs.VAO() < lhs.VAO(); }
        friend bool operator>(const Widget &lhs, const Widget &rhs) { return rhs.VAO() > lhs.VAO(); }
        friend bool operator==(const Widget &lhs, const Widget &rhs) { return rhs.VAO() == lhs.VAO(); }

        ~Widget()
        {
            std::cout << "Cleaning up Widget" << std::endl;

            glDeleteVertexArrays(1, &VAO_);
            for (GLuint buf : BufferObjects_)
                glDeleteBuffers(1, &buf);
        };
    };
} // namespace gl_scene
