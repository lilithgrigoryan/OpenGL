#pragma once

#include "Widget.hpp"
#include "WidgetType.hpp"

namespace gl_scene
{
    class WidgetFactory
    {
    private:
        int id_ = 0;

    public:
        WidgetFactory(){};
        ~WidgetFactory(){};
        Widget *CreateWidget(WidgetType widgetType);
    };
}