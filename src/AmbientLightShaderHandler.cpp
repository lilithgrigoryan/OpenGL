#include "../include/AmbientLightShaderHandler.hpp"
#include <iostream>
#include <stdexcept>
#include "../include/util.h"

namespace gl_scene
{
    void AmbientLightShaderHandler::AddShader(GLuint ShaderProgram, std::string shaderFilename, GLenum ShaderType)
    {
        std::string shaderText;
        if (!ReadFile(shaderFilename.c_str(), shaderText))
        {
            fprintf(stderr, "Error creating shader type %d\n", ShaderType);
            exit(1);
        }

        GLuint ShaderObj = glCreateShader(ShaderType);
        if (ShaderObj == 0)
        {
            fprintf(stderr, "Error creating shader type %d\n", ShaderType);
            exit(1);
        }

        const GLchar *p[1];
        p[0] = shaderText.c_str();

        GLint Lengths[1] = {(GLint)shaderText.size()};

        glShaderSource(ShaderObj, 1, p, Lengths);

        glCompileShader(ShaderObj);

        GLint success;
        glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            GLchar InfoLog[1024];
            glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
            fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
            exit(1);
        }

        glAttachShader(ShaderProgram, ShaderObj);
    }

    void AmbientLightShaderHandler::Compile()
    {
        GLint Success = 0;
        GLchar ErrorLog[1024] = {0};

        glLinkProgram(shaderProgram_);

        glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &Success);
        if (Success == 0)
        {
            glGetProgramInfoLog(shaderProgram_, sizeof(ErrorLog), NULL, ErrorLog);
            fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
            exit(1);
        }

        glValidateProgram(shaderProgram_);
        glGetProgramiv(shaderProgram_, GL_VALIDATE_STATUS, &Success);
        if (!Success)
        {
            glGetProgramInfoLog(shaderProgram_, sizeof(ErrorLog), NULL, ErrorLog);
            fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
            exit(1);
        }
    }

    GLint AmbientLightShaderHandler::GetUniformLocation(const char *uniformName)
    {
        GLint uniformLocation = glGetUniformLocation(shaderProgram_, uniformName);
        if (uniformLocation == -1)
            throw std::runtime_error(std::string("Error getting uniform location of") + uniformName + '\n');

        return uniformLocation;
    }

    AmbientLightShaderHandler::AmbientLightShaderHandler(std::string VertexShader, std::string FragmentShader)
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

    void AmbientLightShaderHandler::SetTransformationMatrix(const Matrix4f &Mat)
    {
        glUniformMatrix4fv(transformMatrixLocation_, 1, GL_TRUE, (const GLfloat *)Mat.mat_);
    }

    void AmbientLightShaderHandler::SetTextureUnit(unsigned int TextureUnit)
    {
        glUniform1i(samplerLocation_, TextureUnit);
    }

    void AmbientLightShaderHandler::SetLight(const BaseLight &Light)
    {
        glUniform3f(lightColorLocation_, Light.Color.x, Light.Color.y, Light.Color.z);
        glUniform1f(lightAmbientIntensityLocation_, Light.AmbientIntensity);
    }

    void AmbientLightShaderHandler::SetMaterial(const Material &material)
    {
        glUniform3f(materialAmbientColorLocation_, material.AmbientColor().x, material.AmbientColor().y, material.AmbientColor().z);
    }

    void AmbientLightShaderHandler::Enable()
    {
        glUseProgram(shaderProgram_);
    }
}
