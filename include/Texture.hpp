#pragma once

#include <GL/glew.h>
#include <string>

namespace gl_scene
{
    class Texture
    {
    private:
        std::string filename_;
        GLuint textureObj_;

    public:
        void Load();
        void Bind(GLenum TextureUnit = GL_TEXTURE0);
        Texture(std::string &fileName);
        Texture() {};
        ~Texture();
    };
}