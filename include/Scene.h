#pragma once
#include <iostream>
#include <set>
#include <memory>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Vector3f.h"
#include "Matrix4f.h"
#include "Widget.hpp"
#include "WidgetType.hpp"
#include "WidgetFactory.hpp"
#include "Texture.hpp"
#include "PhongShaderProgram.hpp"
#include "ColorShaderProgram.hpp"

namespace gl_scene
{
    typedef struct CameraUVN
    {
        Vector3f front_;
        Vector3f up_;
        Vector3f left_;
        Vector3f position_;

        friend std::ostream &operator<<(std::ostream &stream, const CameraUVN &c)
        {
            std::cout << "Camera position: " << c.position_;
            std::cout << "Camera front: " << c.front_;
            std::cout << "Camera up: " << c.up_;
            std::cout << "Camera left: " << c.left_;

            return stream;
        }
    } CameraUVN;

    struct Scene
    {
    private:
        int windowWidth_;
        int windowHeight_;
        float fov_;
        float near_;
        float far_;

        float cameraDistance_;
        float theta_;
        float phi_;
        float cameraSpeed_;
        float cameraZoomSpeed_;
        float maxTheta_ = 67.5f;
        float minTheta_ = 22.5f;
        CameraUVN camera;

        WidgetFactory widgetFactory_;
        Widget *playerWidget_ = NULL;
        std::set<Widget *> widgets_;
        std::set<Widget *> visibleWidgets_;

        std::vector<Texture *> textures_;

        GLuint transformationMatrix_;
        GLuint SamplerLocation;

        PhongShaderProgram *phongShaderProgram_;
        ColorShaderProgram *colorShaderProgram_;
        DirectionalLight *light;

        void drawWidget(Widget *w, Matrix4f &ProjectionMat, Matrix4f &CameraViewMat);
        void drawWidget(Widget *w, Texture *texture, Matrix4f &ProjectionMat, Matrix4f &CameraViewMat);

        static int cameraPrevPosX;
        static int cameraPrevPosY;
        static int cameradx;
        static int camerady;

    public:
        // Scene() { playerWidget_ = NULL; };
        Scene(int windowWidth,
              int windowHeight,
              float fov,
              float near,
              float far,
              float cameraDistance_ = 5.0f,
              float theta = 0.0f,
              float phi = 0.0f) : windowWidth_{windowWidth},
                                  windowHeight_(windowHeight),
                                  fov_(fov),
                                  near_(near),
                                  far_(far),
                                  cameraDistance_(cameraDistance_),
                                  theta_(theta),
                                  phi_(phi)
        {
            cameraSpeed_ = 1.0f;
            cameraZoomSpeed_ = 0.5f;
            playerWidget_ = addWidget(CUBECOLORED, Vector3f(0.0, 0.0, 0.0), Vector3f(0.0, 1.0, 0.0), 0.0, false);
            playerWidget_->Scale() = 0.25f;
            
            theta_ = std::max(theta_, minTheta_);
            theta_ = std::min(theta_, maxTheta_);
            camera = CalculateCameraUVN();
        };

        friend std::ostream &operator<<(std::ostream &stream, const Scene &s)
        {
            std::cout << "Window size: " << s.windowWidth_ << "x" << s.windowHeight_ << std::endl;
            std::cout << "Frustum FOV: " << s.fov_ << " near:" << s.near_ << " far:" << s.far_ << std::endl;

            return stream;
        }

        int windowWidth() { return windowWidth_; }
        int windowHeight() { return windowHeight_; }
        const float &fov() const { return fov_; }
        const float &far() const { return far_; }
        const float &near() const { return near_; }

        void OnKeyboard(unsigned char key);
        void OnKeyboardSpecial(unsigned char key, int mouse_x, int mouse_y);
        void OnMouseWheel(int button, int dir, int x, int y);
        void OnMouse(int button, int state, int x, int y);
        void OnMouseActiveMove(int x, int y);

        void setPhongShader(PhongShaderProgram *shaderHadle);
        void enablePhongShader();
        void setColorShader(ColorShaderProgram *shaderHadle);
        void enableColorShader();
        void setDirectionalLight(DirectionalLight *light);
        void addTexture(std::string &filename);
        Texture *Textures(int index) { return textures_[index]; }

        CameraUVN CalculateCameraUVN();

        void addWidgets(float startx, float endx, float startz, float endz, int count);

        void draw();

        ~Scene()
        {
            std::cout << "Cleaning up" << std::endl;
            for (auto w : widgets_)
                delete w;

            delete playerWidget_;
        }

    private:
        Widget *addWidget(WidgetType widgetType, Vector3f position, Vector3f rotationAxis, float rotationAngle, bool insertToWidgets);
        void removeWidget(Widget *w);
        void showWidget(Widget *w);
        void hideWidget(Widget *w);
        Matrix4f ProjectionMat();
        Matrix4f CameraMat();
        const std::set<Widget *> &VisibleWidgets() { return visibleWidgets_; };
    };
}
