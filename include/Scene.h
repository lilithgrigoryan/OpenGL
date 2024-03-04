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

namespace gl_scene
{
    struct Scene
    {
    private:
        int windowWidth_;
        int windowHeight_;
        float fov_;
        float near_;
        float far_;

        Vector3f cameraPos_;
        Vector3f cameraFront_;
        Vector3f cameraUp_;
        float cameraSpeed_;

        WidgetFactory widgetFactory_;
        std::set<Widget *> widgets_;
        std::set<Widget *> visibleWidgets_;

    public:
        Scene(){};
        Scene(int windowWidth,
              int windowHeight,
              float fov,
              float near,
              float far,
              Vector3f cameraPos = Vector3f(0.0f, 0.0f, 0.0f),
              Vector3f cameraFront = Vector3f(0.0f, 0.0f, -1.0f),
              Vector3f cameraUp = Vector3f(0.0f, 1.0f, 0.0f)) : windowWidth_{windowWidth},
                                                                windowHeight_(windowHeight),
                                                                fov_(fov),
                                                                near_(near),
                                                                far_(far),
                                                                cameraPos_(cameraPos),
                                                                cameraFront_(cameraFront),
                                                                cameraUp_(cameraUp)
        {
            cameraSpeed_ = 1.0f;
        };

        int windowWidth() { return windowWidth_; }
        int windowHeight() { return windowHeight_; }
        const float &fov() const { return fov_; }
        const float &far() const { return far_; }
        const float &near() const { return near_; }

        Matrix4f ProjectionMat();
        Matrix4f CameraMat();

        const std::set<Widget *> &VisibleWidgets() { return visibleWidgets_; };

        Widget *addWidget(WidgetType widgetType, Vector3f position, Vector3f rotationAxis, float rotationAngle);
        void removeWidget(Widget *w);
        void showWidget(Widget *w);
        void hideWidget(Widget *w);

        void OnKeyboard(unsigned char key);
        void KeyboardCB(unsigned char key, int mouse_x, int mouse_y);

        friend std::ostream &operator<<(std::ostream &stream, const Scene &s)
        {
            std::cout << "Window size: " << s.windowWidth_ << "x" << s.windowHeight_ << std::endl;
            std::cout << "Frustum FOV: " << s.fov_ << " near:" << s.near_ << " far:" << s.far_ << std::endl;

            return stream;
        }

        ~Scene()
        {
            std::cout << "Cleaning up" << std::endl;
            for (auto w : widgets_)
                delete w;
        }
    };
}
