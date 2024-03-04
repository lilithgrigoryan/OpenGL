#include <GL/glew.h>
#include <random>
#include "../include/WidgetFactory.hpp"

static std::mt19937 rng(std::random_device{}());

namespace gl_scene
{
    Widget *WidgetFactory::CreateWidget(WidgetType widgetType)
    {
        switch (widgetType)
        {
        case CUBE:
        {
            GLuint VAO;
            GLuint VertexVBO;
            GLuint ColorVBO;
            GLuint IBO;

            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            Vector3f vectors[8];
            vectors[0] = Vector3f(-0.5f, -0.5f, -0.5f);
            vectors[1] = Vector3f(0.5f, -0.5f, -0.5f);
            vectors[2] = Vector3f(0.5f, -0.5f, 0.5f);
            vectors[3] = Vector3f(-0.5f, -0.5f, 0.5f);
            vectors[4] = Vector3f(-0.5f, 0.5f, -0.5f);
            vectors[5] = Vector3f(0.5f, 0.5f, -0.5f);
            vectors[6] = Vector3f(0.5f, 0.5f, 0.5f);
            vectors[7] = Vector3f(-0.5f, 0.5f, 0.5f);

            glCreateBuffers(1, &VertexVBO);
            glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vectors), vectors, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            float r = std::generate_canonical<float, 128>(rng);
            float g = std::generate_canonical<float, 128>(rng);
            float b = std::generate_canonical<float, 128>(rng);

            Vector3f color = Vector3f(r, g, b);
            Vector3f colors[8];
            colors[0] = Vector3f(color);
            colors[1] = Vector3f(color);
            colors[2] = Vector3f(color);
            colors[3] = Vector3f(color);
            colors[4] = Vector3f(color);
            colors[5] = Vector3f(color);
            colors[6] = Vector3f(color);
            colors[7] = Vector3f(color);

            glCreateBuffers(1, &ColorVBO);
            glBindBuffer(GL_ARRAY_BUFFER, ColorVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

            int Indices[36] = {
                0, 1, 2,
                0, 2, 3,
                0, 4, 1,
                1, 4, 5,
                1, 5, 6,
                1, 6, 2,
                2, 6, 7,
                2, 7, 3,
                0, 3, 7,
                0, 7, 4,
                4, 7, 5,
                7, 6, 5};

            glGenBuffers(1, &IBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

            glBindVertexArray(0);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            return new Widget(std::to_string(id_++), 12, VAO, VertexVBO, ColorVBO, IBO, widgetType);
        }
        break;

        default:
        {
            std::cout << "Anknown widget type" << std::endl;
            exit(1);
        }
        break;
        }
    }
}