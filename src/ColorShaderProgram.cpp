#include "../include/ColorShaderProgram.hpp"
#include <iostream>
#include <stdexcept>
#include "../include/util.h"
#include "../include/DirectionalLight.hpp"
#include "../include/Matrix3f.hpp"
#include "../include/glmath.h"

namespace gl_scene
{

    ColorShaderProgram::ColorShaderProgram(std::string VertexShader, std::string FragmentShader)
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

            lightColorLocation_ = GetUniformLocation("gDirectionalLight.Color");
            lightDirectionLocation_ = GetUniformLocation("gDirectionalLight.Direction");
            lightAmbientIntensityLocation_ = GetUniformLocation("gDirectionalLight.AmbientIntensity");
            lightDiffuseIntensityLocation_ = GetUniformLocation("gDirectionalLight.DiffuseIntensity");
            lightSpecularIntensityLocation_ = GetUniformLocation("gDirectionalLight.SpecularIntensity");

            materialSpecularColorLocation_ = GetUniformLocation("gMaterial.SpecularColor");
            materialAmbientColorLocation_ = GetUniformLocation("gMaterial.AmbientColor");
            materialDiffuseColorLocation_ = GetUniformLocation("gMaterial.DiffuseColor");
            materialSpecularPowerLocation_ = GetUniformLocation("gMaterial.SpecularExp");

            cameraLocationLocation_ = GetUniformLocation("gCameraLocation");
        }
        catch (const std::runtime_error &e)
        {
            std::cerr << e.what() << std::endl;
            exit(1);
        }
    }

    void ColorShaderProgram::SetTransformationMatrix(const Matrix4f &Mat)
    {
        glUniformMatrix4fv(transformMatrixLocation_, 1, GL_TRUE, (const GLfloat *)Mat.mat_);
    }

    void ColorShaderProgram::SetDirectionalLight(const DirectionalLight *Light, Matrix4f &LocalToWorldTrasnform)
    {
        const Vector3f color = Light->Color();
        const Vector3f direction = Light->calculateLocalDirection(LocalToWorldTrasnform);
        glUniform3f(lightColorLocation_, color.x, color.y, color.z);
        glUniform1f(lightAmbientIntensityLocation_, Light->AmbientIntensity());
        glUniform1f(lightDiffuseIntensityLocation_, Light->DiffuseIntensity());
        glUniform1f(lightSpecularIntensityLocation_, Light->SpecularIntensity());
        glUniform3f(lightDirectionLocation_, direction.x, direction.y, direction.z);
    }

    void ColorShaderProgram::SetMaterial(const Material &material)
    {
        Vector3f ambientColor = material.AmbientColor();
        Vector3f diffuseColor = material.DiffuseColor();
        Vector3f specularColor = material.SpecularColor();
        glUniform3f(materialAmbientColorLocation_, ambientColor.x, ambientColor.y, ambientColor.z);
        glUniform3f(materialDiffuseColorLocation_, diffuseColor.x, diffuseColor.y, diffuseColor.z);
        glUniform3f(materialSpecularColorLocation_, specularColor.x, specularColor.y, specularColor.z);
        glUniform1f(materialSpecularPowerLocation_, material.SpecularColorExp());
    }

    void ColorShaderProgram::SetCamera(Vector3f &CameraWorldLocation, Matrix4f &LocalToWorldTransform)
    {
        Vector3f localPos = calculateLocalCameraPosition(CameraWorldLocation, LocalToWorldTransform);
        glUniform3f(cameraLocationLocation_, localPos.x, localPos.y, localPos.z);
    }

    Vector3f ColorShaderProgram::calculateLocalCameraPosition(Vector3f &CameraWorldLocation, Matrix4f &LocalToWorldTransform)
    {
        Matrix3f R(LocalToWorldTransform[0][0], LocalToWorldTransform[0][1], LocalToWorldTransform[0][2],
                   LocalToWorldTransform[1][0], LocalToWorldTransform[1][1], LocalToWorldTransform[1][2],
                   LocalToWorldTransform[2][0], LocalToWorldTransform[2][1], LocalToWorldTransform[2][2]);
        Matrix3f RT = R.transpose();

        Matrix4f Tinv4x4 = translation(CameraWorldLocation);
        Matrix3f Tinv(Tinv4x4[0][0], Tinv4x4[0][1], Tinv4x4[0][2],
                      Tinv4x4[1][0], Tinv4x4[1][1], Tinv4x4[1][2],
                      Tinv4x4[2][0], Tinv4x4[2][1], Tinv4x4[2][2]);

        Vector3f localLocation = RT * Tinv * CameraWorldLocation;

        return localLocation;
    }
}