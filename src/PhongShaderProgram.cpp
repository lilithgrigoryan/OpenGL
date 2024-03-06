#include "../include/PhongShaderProgram.hpp"
#include <iostream>
#include <stdexcept>
#include "../include/util.h"
#include "../include/DirectionalLight.hpp"

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

            lightColorLocation_ = GetUniformLocation("gDirectionalLight.Color");
            lightAmbientIntensityLocation_ = GetUniformLocation("gDirectionalLight.AmbientIntensity");
            lightDiffuseIntensityLocation_ = GetUniformLocation("gDirectionalLight.DiffuseIntensity");
            lightDirectionLocation_ = GetUniformLocation("gDirectionalLight.Direction");

            materialAmbientColorLocation_ = GetUniformLocation("gMaterial.AmbientColor");
            materialAmbientColorLocation_ = GetUniformLocation("gMaterial.DiffuseColor");
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

    void PhongShaderProgram::SetDirectionalLight(const DirectionalLight *Light, Matrix4f LocalToWorldTrasnform)
    {
        std::cout << "Setting dir light" << std::endl;
        const Vector3f color = Light->Color();
        std::cout << "Color: " << color << std::endl;
        const Vector3f direction = Light->calculateLocalDirection(LocalToWorldTrasnform);
        std::cout << "Calculated direction" << std::endl;
        glUniform3f(lightColorLocation_, color.x, color.y, color.z);
        glUniform1f(lightAmbientIntensityLocation_, Light->AmbientIntensity());
        glUniform1f(lightDiffuseIntensityLocation_, Light->DiffuseIntensity());
        glUniform3f(lightDirectionLocation_, direction.x, direction.y, direction.z);
    }

    void PhongShaderProgram::SetMaterial(const Material &material)
    {
        Vector3f ambientColor = material.AmbientColor();
        Vector3f diffuseColor = material.DiffuseColor();
        glUniform3f(materialAmbientColorLocation_, ambientColor.x, ambientColor.y, ambientColor.z);
        glUniform3f(materialDiffuseColorLocation_, diffuseColor.x, diffuseColor.y, diffuseColor.z);
    }
}
