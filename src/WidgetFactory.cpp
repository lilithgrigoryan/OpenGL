#include <GL/glew.h>
#include <random>
#include <cmath>
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
            GLuint NormalVBO;
            GLuint ColorVBO;
            GLuint IBO;

            std::cout << "in factory" << std::endl;
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);
            std::cout << "in factory1" << std::endl;

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

            Vector3f normals[8];
            normals[0] = Vector3f(-1.0f, -1.0f, -1.0f) * (1.0f / sqrt(3));
            normals[1] = Vector3f(+1.0f, -1.0f, -1.0f) * (1.0f / sqrt(3));
            normals[2] = Vector3f(+1.0f, -1.0f, +1.0f) * (1.0f / sqrt(3));
            normals[3] = Vector3f(-1.0f, -1.0f, +1.0f) * (1.0f / sqrt(3));
            normals[4] = Vector3f(-1.0f, +1.0f, -1.0f) * (1.0f / sqrt(3));
            normals[5] = Vector3f(+1.0f, +1.0f, -1.0f) * (1.0f / sqrt(3));
            normals[6] = Vector3f(+1.0f, +1.0f, +1.0f) * (1.0f / sqrt(3));
            normals[7] = Vector3f(-1.0f, +1.0f, +1.0f) * (1.0f / sqrt(3));

            glGenBuffers(1, &NormalVBO);
            glBindBuffer(GL_ARRAY_BUFFER, NormalVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

            Vector3f color = Vector3f(1.0f, 1.0f, 1.0f);
            Vector3f colors[8];
            colors[0] = color;
            colors[1] = color;
            colors[2] = color;
            colors[3] = color;
            colors[4] = color;
            colors[5] = color;
            colors[6] = color;
            colors[7] = color;

            glGenBuffers(1, &ColorVBO);
            glBindBuffer(GL_ARRAY_BUFFER, ColorVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

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
            glDisableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            Material *material = new Material(Vector3f(1.0, 0.0, 0.0), Vector3f(1.0, 0.0, 0.0), Vector3f(1.0, 0.0, 0.0), 2.f);
            std::cout << "out factory" << std::endl;
            return new Widget(std::to_string(id_++), 12, VAO, {NormalVBO, ColorVBO, IBO}, material, widgetType);
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

            Material *material = new Material();
            return new Widget(std::to_string(id_++), 12, VAO, {TexVBO, IBO}, material, widgetType);
        }
        break;
        case CUBEWITHNORMALS:
        {
            GLuint VAO;
            GLuint VertexVBO;
            GLuint TexVBO;
            GLuint NormalVBO;
            GLuint IBO;

            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            Vector3f vectors[24];
            // Down
            vectors[0] = Vector3f(-0.5f, -0.5f, -0.5f);
            vectors[1] = Vector3f(0.5f, -0.5f, -0.5f);
            vectors[2] = Vector3f(0.5f, -0.5f, 0.5f);
            vectors[3] = Vector3f(-0.5f, -0.5f, 0.5f);

            // Up
            vectors[4] = Vector3f(-0.5f, 0.5f, -0.5f);
            vectors[5] = Vector3f(0.5f, 0.5f, -0.5f);
            vectors[6] = Vector3f(0.5f, 0.5f, 0.5f);
            vectors[7] = Vector3f(-0.5f, 0.5f, 0.5f);

            // Left
            vectors[8] = Vector3f(-0.5f, -0.5f, -0.5f); // 0
            vectors[9] = Vector3f(-0.5f, -0.5f, 0.5f);  // 3
            vectors[10] = Vector3f(-0.5f, 0.5f, 0.5f);  // 7
            vectors[11] = Vector3f(-0.5f, 0.5f, -0.5f); // 4

            // Right
            vectors[12] = Vector3f(0.5f, -0.5f, -0.5f); // 1
            vectors[13] = Vector3f(0.5f, 0.5f, -0.5f);  // 5
            vectors[14] = Vector3f(0.5f, 0.5f, 0.5f);   // 6
            vectors[15] = Vector3f(0.5f, -0.5f, 0.5f);  // 2

            // Near
            vectors[16] = Vector3f(0.5f, -0.5f, 0.5f);  // 2
            vectors[17] = Vector3f(0.5f, 0.5f, 0.5f);   // 6
            vectors[18] = Vector3f(-0.5f, 0.5f, 0.5f);  // 7
            vectors[19] = Vector3f(-0.5f, -0.5f, 0.5f); // 3

            // Far
            vectors[20] = Vector3f(-0.5f, -0.5f, -0.5f); // 0
            vectors[21] = Vector3f(-0.5f, 0.5f, -0.5f);  // 4
            vectors[22] = Vector3f(0.5f, 0.5f, -0.5f);   // 5
            vectors[23] = Vector3f(0.5f, -0.5f, -0.5f);  // 1

            glGenBuffers(1, &VertexVBO);
            glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vectors), vectors, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            Vector2f textures[24];
            // Down
            textures[0] = Vector2f(0.25f, 1.f / 3.f);
            textures[1] = Vector2f(0.25f, 0.0f);
            textures[2] = Vector2f(0.5f, 0.0f);
            textures[3] = Vector2f(0.5f, 1.f / 3.f);

            // Up
            textures[4] = Vector2f(0.25f, 2.f / 3.f);
            textures[5] = Vector2f(0.25f, 1.0f);
            textures[6] = Vector2f(0.5f, 1.0f);
            textures[7] = Vector2f(0.5f, 2.f / 3.f);

            // Left
            textures[8] = Vector2f(0.25f, 1.f / 3.f);  // 0
            textures[9] = Vector2f(0.5f, 1.f / 3.f);   // 3
            textures[10] = Vector2f(0.5f, 2.f / 3.f);  // 7
            textures[11] = Vector2f(0.25f, 2.f / 3.f); // 4

            // Right
            textures[12] = Vector2f(1.f, 1.f / 3.f);   // 1
            textures[13] = Vector2f(1.f, 2.f / 3.f);   // 5
            textures[14] = Vector2f(0.75f, 2.f / 3.f); // 6
            textures[15] = Vector2f(0.75f, 1.f / 3.f); // 2

            // Near
            textures[16] = Vector2f(0.75f, 1.f / 3.f); // 2
            textures[17] = Vector2f(0.75f, 2.f / 3.f); // 6
            textures[18] = Vector2f(0.5f, 2.f / 3.f);  // 7
            textures[19] = Vector2f(0.5f, 1.f / 3.f);  // 3

            // Far
            textures[20] = Vector2f(0.25f, 1.f / 3.f); // 0
            textures[21] = Vector2f(0.25f, 2.f / 3.f); // 4
            textures[22] = Vector2f(0.0f, 2.f / 3.f);  // 5
            textures[23] = Vector2f(0.0f, 1.f / 3.f);  // 1

            glGenBuffers(1, &TexVBO);
            glBindBuffer(GL_ARRAY_BUFFER, TexVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(textures), textures, GL_STATIC_DRAW);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

            Vector3f normals[24];
            Vector3f normal0 = Vector3f(-1.0f, -1.0f, -1.0f) * (1.0f / sqrt(3));
            Vector3f normal1 = Vector3f(+1.0f, -1.0f, -1.0f) * (1.0f / sqrt(3));
            Vector3f normal2 = Vector3f(+1.0f, -1.0f, +1.0f) * (1.0f / sqrt(3));
            Vector3f normal3 = Vector3f(-1.0f, -1.0f, +1.0f) * (1.0f / sqrt(3));

            Vector3f normal4 = Vector3f(-1.0f, +1.0f, -1.0f) * (1.0f / sqrt(3));
            Vector3f normal5 = Vector3f(+1.0f, +1.0f, -1.0f) * (1.0f / sqrt(3));
            Vector3f normal6 = Vector3f(+1.0f, +1.0f, +1.0f) * (1.0f / sqrt(3));
            Vector3f normal7 = Vector3f(-1.0f, +1.0f, +1.0f) * (1.0f / sqrt(3));
            // Down
            normals[0] = normal0; // Vector3f(0.0f, -1.0f, 0.0f);
            normals[1] = normal1; // Vector3f(0.0f, -1.0f, 0.0f);
            normals[2] = normal2; // Vector3f(0.0f, -1.0f, 0.0f);
            normals[3] = normal3; // Vector3f(0.0f, -1.0f, 0.0f);

            // Up
            normals[4] = normal4; // Vector3f(0.0f, 1.0f, 0.0f);
            normals[5] = normal5; // Vector3f(0.0f, 1.0f, 0.0f);
            normals[6] = normal6; // Vector3f(0.0f, 1.0f, 0.0f);
            normals[7] = normal7; // Vector3f(0.0f, 1.0f, 0.0f);

            // Left
            normals[8] = normal0;  // Vector3f(-1.0f, 0.0f, 0.0f);  // 0
            normals[9] = normal3;  // Vector3f(-1.0f, 0.0f, 0.0f);  // 3
            normals[10] = normal7; // Vector3f(-1.0f, 0.0f, 0.0f); // 7
            normals[11] = normal4; // Vector3f(-1.0f, 0.0f, 0.0f); // 4

            // Right
            normals[12] = normal1; // Vector3f(1.0f, 0.0f, 0.0f); // 1
            normals[13] = normal5; // Vector3f(1.0f, 0.0f, 0.0f); // 5
            normals[14] = normal6; // Vector3f(1.0f, 0.0f, 0.0f); // 6
            normals[15] = normal2; // Vector3f(1.0f, 0.0f, 0.0f); // 2

            // Near
            normals[16] = normal2; // Vector3f(0.0f, 0.0f, 1.0f); // 2
            normals[17] = normal6; // Vector3f(0.0f, 0.0f, 1.0f); // 6
            normals[18] = normal7; // Vector3f(0.0f, 0.0f, 1.0f); // 7
            normals[19] = normal3; // Vector3f(0.0f, 0.0f, 1.0f); // 3

            // Far
            normals[20] = normal0; // Vector3f(0.0f, 0.0f, -1.0f); // 0
            normals[21] = normal4; // Vector3f(0.0f, 0.0f, -1.0f); // 4
            normals[22] = normal5; // Vector3f(0.0f, 0.0f, -1.0f); // 5
            normals[23] = normal1; // Vector3f(0.0f, 0.0f, -1.0f); // 1

            glGenBuffers(1, &NormalVBO);
            glBindBuffer(GL_ARRAY_BUFFER, NormalVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

            int Indices[36] = {
                0, 1, 2,
                0, 2, 3,
                20, 21, 22,
                20, 22, 23,
                9, 10, 11,
                9, 11, 8,
                16, 17, 18,
                16, 18, 19,
                12, 13, 14,
                12, 14, 15,
                7, 6, 5,
                7, 5, 4};

            glGenBuffers(1, &IBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

            glBindVertexArray(0);
            glDisableVertexAttribArray(0);
            glDisableVertexAttribArray(1);
            glDisableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            float r = 1.0f; // std::generate_canonical<float, 128>(rng);
            float g = 1.0f; // std::generate_canonical<float, 128>(rng);
            float b = 1.0f; // std::generate_canonical<float, 128>(rng);

            Material *material = new Material(Vector3f(r, g, b), Vector3f(r, g, b));
            return new Widget(std::to_string(id_++), 12, VAO, {TexVBO, NormalVBO, IBO}, material, widgetType);
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