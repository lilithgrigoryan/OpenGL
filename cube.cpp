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
#include "include/ColorShaderProgram.hpp"
#include "include/DirectionalLight.hpp"

using namespace gl_scene;

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

Scene *scene;

static void OnKeyboard(int key, int mouse_x, int mouse_y)
{
    scene->OnKeyboard(key);
}

static void OnKeyboardSpecial(unsigned char key, int mouse_x, int mouse_y)
{
    scene->OnKeyboardSpecial(key, mouse_x, mouse_y);
}

void onMouseWheel(int button, int dir, int x, int y)
{
    scene->OnMouseWheel(button, dir, x, y);
}

void onMouse(int button, int state, int x, int y)
{
    scene->OnMouse(button, state, x, y);
}

void onMouseActiveMove(int x, int y)
{
    scene->OnMouseActiveMove(x, y);
}

void RenderDisplayCB()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->draw();

    glutPostRedisplay();
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

    int x = 200;
    int y = 100;
    glutInitWindowPosition(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Cube");
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    GLenum res = glewInit();
    if (res != GLEW_OK)
        fprintf(stderr, "Failed to initialize glew");

    GLclampf r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;
    glClearColor(r, g, b, a);

    glEnable(GL_CULL_FACE);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    float fov = 45.0 / 180.0 * M_PI;
    scene = new Scene(WINDOW_WIDTH, WINDOW_HEIGHT, 45.0 / 180.0 * M_PI, 0.1, 200, 5, 0.0, 0.0);

    PhongShaderProgram *shaderHandlerPhong = new PhongShaderProgram("shaders/shader_phong.vs", "shaders/shader_phong.fs");
    scene->setPhongShader(shaderHandlerPhong);
    ColorShaderProgram *shaderHandlerColor = new ColorShaderProgram("shaders/shader_color.vs", "shaders/shader_color.fs");
    scene->setColorShader(shaderHandlerColor);
    DirectionalLight *light = new DirectionalLight(Vector3f(1.0f, 1.0f, 1.0f), 0.75f, 0.75f, 0.75, Vector3f(0.0f, 0.0f, -1.0f));
    scene->setDirectionalLight(light);

    std::string tex1 = "resources/GlassTexture.jpg";
    std::string tex2 = "resources/MetallicTexture.jpg";
    std::string tex3 = "resources/WoodenTexture.jpg";
    std::string tex4 = "resources/StoneTexture.jpg";
    scene->addTexture(tex1);
    scene->addTexture(tex2);
    scene->addTexture(tex3);
    scene->addTexture(tex4);

    scene->addWidgets(-100.0, 100.0, -100.0, 100.0, 55);

    glutDisplayFunc(RenderDisplayCB);
    glutSpecialFunc(OnKeyboard);
    glutKeyboardFunc(OnKeyboardSpecial);
    glutMouseWheelFunc(onMouseWheel);
    glutMouseFunc(onMouse);
    glutMotionFunc(onMouseActiveMove);
    glutMainLoop();

    delete scene;
    return 0;
}