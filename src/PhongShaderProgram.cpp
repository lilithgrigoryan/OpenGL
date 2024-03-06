#include "../include/PhongShaderProgram.hpp"
#include <iostream>
#include <stdexcept>
#include "../include/util.h"

namespace gl_scene
{

    PhongShaderProgram::PhongShaderProgram(std::string VertexShader, std::string FragmentShader)
    {
        vertexShaderFilename_ = VertexShader;
        fragmentShaderFilename_ = FragmentShader;

        shaderProgram_ = glCreateProgram();
        if (shaderProgram_ == 0)
            throw std::runtime_error("Error while creating Shader Program.\n");

        AddShader(shaderProgram_, vertexShaderFilename_, GL_VERTEX_SHADER);
        AddShader(shaderProgram_, fragmentShaderFilename_, GL_FRAGMENT_SHADER);

        Compile();

        try
        {
            transformMatrixLocation_ = glGetUniformLocation(shaderProgram_, "gTransformMatrix");
            samplerLocation_ = glGetUniformLocation(shaderProgram_, "gSampler");
            lightColorLocation_ = GetUniformLocation("gLight.Color");
            lightAmbientIntensityLocation_ = GetUniformLocation("gLight.AmbientIntensity");
            materialAmbientColorLocation_ = GetUniformLocation("gMaterial.AmbientColor");
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << e.what() << std::endl;
            exit(1);
        }
    }

    void PhongShaderProgram::SetTransformationMatrix(const Matrix4f &Mat)
    {
        glUniformMatrix4fv(transformMatrixLocation_, 1, GL_TRUE, (const GLfloat *)Mat.mat_);
    }

    void PhongShaderProgram::SetTextureUnit(unsigned int TextureUnit)
    {
        glUniform1i(samplerLocation_, TextureUnit);
    }

    void PhongShaderProgram::SetLight(const BaseLight &Light)
    {
        const Vector3f color = Light.Color();
        glUniform3f(lightColorLocation_, color.x, color.y, color.z);
        glUniform1f(lightAmbientIntensityLocation_, Light.AmbientIntensity());
    }

    void PhongShaderProgram::SetMaterial(const Material &material)
    {
        glUniform3f(materialAmbientColorLocation_, material.AmbientColor().x, material.AmbientColor().y, material.AmbientColor().z);
    }
}
