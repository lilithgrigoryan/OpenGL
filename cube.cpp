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
    Matrix4f CameraViewMat = scene.CameraMat();
    Matrix4f ProjectionMat = scene.ProjectionMat();

    static float angle = 0;
    angle += 0.02f;

    for (Widget *w : scene.VisibleWidgets())
    {
        if (w->isVisible())
        {
            w->Rotate();

            Matrix4f matrix = ProjectionMat * CameraViewMat * w->TransformationMat();
            glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &matrix.mat_[0][0]);
            glBindVertexArray(w->VAO());
            glDrawElements(GL_TRIANGLES, 3 * w->TrianglesNumber(), GL_UNSIGNED_INT, 0);
        }
    }

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

const char *pVSFileName = "shader.vs";
const char *pFSFileName = "shader.fs";

static void CompileShaders()
{
    GLuint ShaderProgram = glCreateProgram();

    if (ShaderProgram == 0)
    {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }

    std::string vs, fs;

    if (!ReadFile(pVSFileName, vs))
    {
        exit(1);
    };

    AddShader(ShaderProgram, vs.c_str(), GL_VERTEX_SHADER);

    if (!ReadFile(pFSFileName, fs))
    {
        exit(1);
    };

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

    glUseProgram(ShaderProgram);
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

    CompileShaders();

    glutDisplayFunc(RenderDisplayCB);
    glutSpecialFunc(SpecialKeyboardCB);
    glutKeyboardFunc(KeyboardCB);
    glutMainLoop();

    return 0;
}