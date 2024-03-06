#include <GL/glew.h>
#include <string>
#include <vector>

namespace gl_scene
{

    class ShaderProgram
    {
    public:
        void AddShader(GLuint ShaderProgram, std::string shaderFilename, GLenum ShaderType);
        GLint GetUniformLocation(const char *uniformName);
        void Compile();
        void Enable();

    protected:
        GLuint shaderProgram_ = 0;
        std::vector<GLuint> shaderObjects_;
    };
}
