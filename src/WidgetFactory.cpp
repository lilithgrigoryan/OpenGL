#include <GL/glew.h>
#include <random>
#include "../include/Vector2f.h"
#include "../include/WidgetFactory.hpp"

static std::mt19937 rng(std::random_device{}());

namespace gl_scene
{
    Widget *WidgetFactory::CreateWidget(WidgetType widgetType)
    {
        switch (widgetType)
        {
        case CUBECOLORED:
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

            glGenBuffers(1, &VertexVBO);
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

            glGenBuffers(1, &ColorVBO);
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

            return new Widget(std::to_string(id_++), 12, VAO, {ColorVBO, IBO}, widgetType);
        }
        break;
        case CUBETEXTURED:
        {
            GLuint VAO;
            GLuint VertexVBO;
            GLuint TexVBO;
            GLuint IBO;

            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            Vector3f vectors[14];
            vectors[0] = Vector3f(-0.5f, -0.5f, -0.5f);
            vectors[1] = Vector3f(0.5f, -0.5f, -0.5f);
            vectors[2] = Vector3f(0.5f, -0.5f, 0.5f);
            vectors[3] = Vector3f(-0.5f, -0.5f, 0.5f);
            vectors[4] = Vector3f(-0.5f, 0.5f, -0.5f);
            vectors[5] = Vector3f(0.5f, 0.5f, -0.5f);
            vectors[6] = Vector3f(0.5f, 0.5f, 0.5f);
            vectors[7] = Vector3f(-0.5f, 0.5f, 0.5f);
            vectors[8] = Vector3f(0.5f, -0.5f, -0.5f);  // 1'
            vectors[9] = Vector3f(0.5f, 0.5f, -0.5f);   // 5'
            vectors[10] = Vector3f(0.5f, -0.5f, -0.5f); // 1''
            vectors[11] = Vector3f(0.5f, 0.5f, -0.5f);  // 5''
            vectors[12] = Vector3f(0.5f, 0.5f, 0.5f);   // 6'
            vectors[13] = Vector3f(0.5f, -0.5f, 0.5f);  // 2'

            glGenBuffers(1, &VertexVBO);
            glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vectors), vectors, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            Vector2f textures[14];
            textures[0] = Vector2f(0.25f, 1.f / 3.f);
            textures[1] = Vector2f(0.0f, 1.f / 3.f);
            textures[2] = Vector2f(0.75f, 1.f / 3.f);
            textures[3] = Vector2f(0.5f, 1.f / 3.f);
            textures[4] = Vector2f(0.25f, 2.f / 3.f);
            textures[5] = Vector2f(0.0f, 2.f / 3.f);
            textures[6] = Vector2f(0.75f, 2.f / 3.f);
            textures[7] = Vector2f(0.5f, 2.f / 3.f);
            textures[8] = Vector2f(0.25f, 0.0f);      // 1'
            textures[9] = Vector2f(0.25f, 1.0f);      // 5'
            textures[10] = Vector2f(1.0f, 1.f / 3.f); // 1''
            textures[11] = Vector2f(1.0f, 2.f / 3.f); // 5''
            textures[12] = Vector2f(0.5f, 1.0f);      // 6'
            textures[13] = Vector2f(0.5f, 0.0f);      // 2'

            glGenBuffers(1, &TexVBO);
            glBindBuffer(GL_ARRAY_BUFFER, TexVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(textures), textures, GL_STATIC_DRAW);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

            int Indices[36] = {
                0, 8, 13,
                0, 13, 3,
                0, 4, 1,
                1, 4, 5,
                10, 11, 6,
                10, 6, 2,
                2, 6, 7,
                2, 7, 3,
                0, 3, 7,
                0, 7, 4,
                4, 7, 9,
                7, 12, 9};

            glGenBuffers(1, &IBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

            glBindVertexArray(0);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            return new Widget(std::to_string(id_++), 12, VAO, {TexVBO, IBO}, widgetType);
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