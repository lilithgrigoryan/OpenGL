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
#include "include/PhongShaderProgram.hpp"
#include "include/DirectionalLight.hpp"

using namespace gl_scene;

GLuint transformationMatrix_;
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

int main(int argc, char **argv)
{
    float fov = 45.0 / 180.0 * M_PI;
    Vector3f cameraPos(0.0f, 0.0f, 4.0f);
    Vector3f cameraFront(0, 0, -1);
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

    GLclampf r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
    glClearColor(r, g, b, a);

    glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    PhongShaderProgram *shaderHandler = new PhongShaderProgram("shaders/shader_phong.vs", "shaders/shader_phong.fs");
    DirectionalLight *light = new DirectionalLight(Vector3f(1.0f, 1.0f, 1.0f), 0.75f, 0.75f, 0.75, Vector3f(0.0f, 0.0f, -1.0f));
    scene.setPhongShader(shaderHandler);
    scene.setDirectionalLight(light);

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