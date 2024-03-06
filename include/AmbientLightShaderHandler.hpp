#pragma once

#include <GL/glew.h>
#include <string>
#include "../include/Vector3f.h"
#include "../include/Matrix4f.h"
#include "../include/Material.hpp"

namespace gl_scene
{
    class BaseLight
    {
    public:
        Vector3f Color;
        float AmbientIntensity;

        BaseLight()
        {
            Color = Vector3f(1.0f, 1.0f, 1.0f);
            AmbientIntensity = 0.75f;
        }

        BaseLight(Vector3f Color_, float AmbientIntensity_)
        {
            Color = Color_;
            AmbientIntensity = AmbientIntensity_;
        }
    };

    class AmbientLightShaderHandler
    {
    private:
        std::string vertexShaderFilename_;
        std::string fragmentShaderFilename_;

        GLuint shaderProgram_ = 0;

        GLuint transformMatrixLocation_;
        GLuint samplerLocation_;
        GLuint lightColorLocation_;
        GLuint lightAmbientIntensityLocation_;
        GLuint materialAmbientColorLocation_;

    public:
        AmbientLightShaderHandler(std::string VertexShader, std::string FragmentShader);
        void SetTransformationMatrix(const Matrix4f &WVP);
        void SetTextureUnit(unsigned int TextureUnit);
        void SetLight(const BaseLight &Light);
        void SetMaterial(const Material &material);
        void Enable();

    private:
        void AddShader(GLuint ShaderProgram, std::string shaderFilename, GLenum ShaderType);
        GLuint CompileShadersTexture(std::string vertexShaderFilename, std::string fragmentShaderFilename);
        void Compile();
        GLint GetUniformLocation(const char *uniformName);
    };
} // namespace gl_scene