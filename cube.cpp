#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <string.h>
#include <iostream>

#include "include/Vector3f.h"
#include "include/Matrix4f.h"
#include "include/util.h"
#include "include/glmath.h"
#include "include/Scene.h"

using namespace gl_scene;

GLuint gWorldLocation;
GLuint WVPLocation;
GLuint SamplerLocation;

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

Scene scene;

static void SpecialKeyboardCB(int key, int mouse_x, int mouse_y)
{
    scene.OnKeyboard(key);
}

static void KeyboardCB(unsigned char key, int mouse_x, int mouse_y)
{
    scene.KeyboardCB(key, mouse_x, mouse_y);
}

void RenderDisplayCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene.draw();

    glutPostRedisplay();
    glutSwapBuffers();
}

static void AddShader(GLuint ShaderProgram, const char *pShaderText, GLenum ShaderType)
{
    GLuint ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0)
    {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(1);
    }

    const GLchar *p[1];
    p[0] = pShaderText;

    GLint Lengths[1];
    Lengths[0] = (GLint)strlen(pShaderText);

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

static GLuint CompileShadersColor(std::string vertexShaderFilename, std::string fragmentShaderFilename)
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0)
    {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    std::string vs, fs;

    if (!ReadFile(vertexShaderFilename.c_str(), vs))
    {
        std::cout << "Unable to read shader " << vertexShaderFilename << std::endl;
        exit(1);
    };

    if (!ReadFile(fragmentShaderFilename.c_str(), fs))
    {
        std::cout << "Unable to read shader " << fragmentShaderFilename << std::endl;
        exit(1);
    };

    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = {0};

    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0)
    {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    gWorldLocation = glGetUniformLocation(ShaderProgram, "transform");
    if (gWorldLocation == -1)
    {
        printf("Error getting uniform location of 'transform'\n");
        exit(1);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success)
    {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    return ShaderProgram;
}

static GLuint CompileShadersTexture(std::string vertexShaderFilename, std::string fragmentShaderFilename)
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0)
    {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    std::string vs, fs;

    if (!ReadFile(vertexShaderFilename.c_str(), vs))
    {
        std::cout << "Unable to read shader " << vertexShaderFilename << std::endl;
        exit(1);
    };

    if (!ReadFile(fragmentShaderFilename.c_str(), fs))
    {
        std::cout << "Unable to read shader " << fragmentShaderFilename << std::endl;
        exit(1);
    };

    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);
    AddShader(ShaderProgram, fs.c_str(), GL_FRAGMENT_SHADER);

    GLint Success = 0;
    GLchar ErrorLog[1024] = {0};

    glLinkProgram(ShaderProgram);

    glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &Success);
    if (Success == 0)
    {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    WVPLocation = glGetUniformLocation(ShaderProgram, "gWVP");
    if (WVPLocation == -1)
    {
        printf("Error getting uniform location of 'gWVP'\n");
        exit(1);
    }

    SamplerLocation = glGetUniformLocation(ShaderProgram, "gSampler");
    if (SamplerLocation == -1)
    {
        printf("Error getting uniform location of 'gSampler'\n");
        exit(1);
    }

    glValidateProgram(ShaderProgram);
    glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
    if (!Success)
    {
        glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    return ShaderProgram;
}

int main(int argc, char **argv)
{
    float fov = 45.0 / 180.0 * M_PI;
    Vector3f cameraPos(4.0f, 0.0f, 0.0f);
    Vector3f cameraFront(-1, 0, 0);
    Vector3f cameraUp(0, 1, 0);
    scene = Scene(WINDOW_WIDTH, WINDOW_HEIGHT, 45.0 / 180.0 * M_PI, 3, 20, cameraPos, cameraFront, cameraUp);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(scene.windowWidth(), scene.windowHeight());

    int x = 200;
    int y = 100;
    glutInitWindowPosition(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Cube");

    GLenum res = glewInit();
    if (res != GLEW_OK)
        fprintf(stderr, "Failed to initialize glew");

    GLclampf r = 1.f, g = 1.f, b = 1.f, a = 0.f;
    glClearColor(r, g, b, a);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    std::string vertexShaderColor = "shaders/shader_color.vs";
    std::string fragmentShaderColor = "shaders/shader_color.fs";
    GLuint64 colorShaderProgram = CompileShadersColor(vertexShaderColor, fragmentShaderColor);
    scene.ColorShader() = colorShaderProgram;

    std::string vertexShaderTexture = "shaders/shader_texture.vs";
    std::string fragmentShaderTexture = "shaders/shader_texture.fs";
    GLuint64 textureShaderProgram = CompileShadersTexture(vertexShaderTexture, fragmentShaderTexture);
    scene.TextureShader() = textureShaderProgram;

    std::string tex1 = "resources/GlassTexture.jpg";
    std::string tex2 = "resources/MetallicTexture.jpg";
    std::string tex3 = "resources/WoodenTexture.jpg";
    std::string tex4 = "resources/StoneTexture.jpg";
    scene.addTexture(tex1);
    scene.addTexture(tex2);
    scene.addTexture(tex3);
    scene.addTexture(tex4);

    glutDisplayFunc(RenderDisplayCB);
    glutSpecialFunc(SpecialKeyboardCB);
    glutKeyboardFunc(KeyboardCB);
    glutMainLoop();

    return 0;
}