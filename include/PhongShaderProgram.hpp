#pragma once

#include <GL/glew.h>
#include <string>
#include "../include/ShaderProgram.hpp"
#include "../include/Vector3f.h"
#include "../include/Matrix4f.h"
#include "../include/Material.hpp"
#include "../include/DirectionalLight.hpp"

namespace gl_scene
{
    class PhongShaderProgram : public ShaderProgram
    {
    private:
        std::string vertexShaderFilename_;
        std::string fragmentShaderFilename_;

        GLuint transformMatrixLocation_;
        GLuint samplerLocation_;
        GLuint lightColorLocation_;
        GLuint lightDirectionLocation_;
        GLuint lightAmbientIntensityLocation_;
        GLuint lightDiffuseIntensityLocation_;
        GLuint lightSpecularIntensityLocation_;

        GLuint materialAmbientColorLocation_;
        GLuint materialDiffuseColorLocation_;
        GLuint materialSpecularColorLocation_;
        GLuint materialSpecularPowerLocation_;
        
        GLuint cameraLocationLocation_;

        Vector3f calculateLocalCameraPosition(Vector3f &CameraWorldLocation, Matrix4f &LocalToWorldTransform);

    public:
        PhongShaderProgram(std::string VertexShader, std::string FragmentShader);
        void SetTransformationMatrix(const Matrix4f &WVP);
        void SetTextureUnit(unsigned int TextureUnit);
        void SetDirectionalLight(const DirectionalLight *Light, Matrix4f &LocalToWorldTrasnform);
        void SetCamera(Vector3f &CameraWorldLocation, Matrix4f &WordldToCameraTransform);
        void SetMaterial(const Material &material);
    };
} // namespace gl_scene