#include <cmath>
#include <algorithm>
#include "../include/Scene.h"
#include "../include/glmath.h"

namespace gl_scene
{
    Matrix4f Scene::ProjectionMat()
    {
        float r = (float)windowWidth_ / windowHeight_;
        float f = 1 / tanf(fov_ / 2);
        float a = (far_ + near_) / (far_ - near_);
        float b = -2 * far_ * near_ / (far_ - near_);

        Matrix4f Projection(f / r, 0.0f, 0.0f, 0.0f,
                            0.0f, f, 0.0f, 0.0f,
                            0.0f, 0.0f, -a, b,
                            0.0f, 0.0f, -1.0, 0.0f);

        return Projection;
    }

    Matrix4f Scene::CameraMat()
    {
        Vector3f N = cameraFront_.Normalize();
        Vector3f V = cameraUp_.Normalize();
        Vector3f U = (N.Cross(V)).Normalize();

        Matrix4f CameraMat(U.x, U.y, U.z, -U.Dot(cameraPos_),
                           V.x, V.y, V.z, -V.Dot(cameraPos_),
                           -N.x, -N.y, -N.z, N.Dot(cameraPos_),
                           0.0f, 0.0f, 0.0f, 1.0f);

        return CameraMat;
    }

    Widget *Scene::addWidget(WidgetType widgetType, Vector3f position, Vector3f rotationAxis, float rotationAngle)
    {
        Widget *w = widgetFactory_.CreateWidget(widgetType);
        w->Position() = position;
        w->RotationAxis() = rotationAxis;
        w->RotationAngle() = rotationAngle;

        widgets_.insert(w);

        return w;
    }

    void Scene::removeWidget(Widget *w)
    {
        if (visibleWidgets_.find(w) != visibleWidgets_.end())
            visibleWidgets_.erase(w);

        widgets_.erase(w);

        delete w;
    }

    void Scene::showWidget(Widget *w)
    {
        w->isVisible() = true;
        visibleWidgets_.insert(w);
    }

    void Scene::hideWidget(Widget *w)
    {
        w->isVisible() = false;
        visibleWidgets_.erase(w);
    }

    void Scene::drawWidget(Widget *w, Matrix4f &ProjectionMat, Matrix4f &CameraViewMat)
    {
        glUseProgram(colorShader_);

        w->Rotate();

        Matrix4f matrix = ProjectionMat * CameraViewMat * w->TransformationMat();
        glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &matrix.mat_[0][0]);
        glBindVertexArray(w->VAO());
        glDrawElements(GL_TRIANGLES, 3 * w->TrianglesNumber(), GL_UNSIGNED_INT, 0);
    }

    void Scene::drawWidget(Widget *w, Texture *texture, Matrix4f &ProjectionMat, Matrix4f &CameraViewMat)
    {
        glUseProgram(textureShader_);
        glUniform1i(SamplerLocation, 0);

        texture->Bind(GL_TEXTURE0);

        w->Rotate();

        Matrix4f matrix = ProjectionMat * CameraViewMat * w->TransformationMat();
        glUniformMatrix4fv(WVPLocation, 1, GL_TRUE, &matrix.mat_[0][0]);
        glBindVertexArray(w->VAO());
        glDrawElements(GL_TRIANGLES, 3 * w->TrianglesNumber(), GL_UNSIGNED_INT, 0);
    }

    void Scene::draw()
    {
        Matrix4f CameraViewMat = this->CameraMat();
        Matrix4f ProjectionMat = this->ProjectionMat();

        for (Widget *w : visibleWidgets_)
        {
            if (w->isVisible())
            {
                Texture *texture = textures_[stol(w->id()) % textures_.size()];
                drawWidget(w, texture, ProjectionMat, CameraViewMat);
            }
        }
    }

    void Scene::addTexture(std::string &filename)
    {
        Texture *text = new Texture(filename);
        text->Load();
        textures_.push_back(text);
    }

    void Scene::OnKeyboard(unsigned char key)
    {
        switch (key)
        {
        case GLUT_KEY_UP:
            cameraPos_ += (cameraFront_ * cameraSpeed_);
            break;
        case GLUT_KEY_DOWN:
            cameraPos_ -= (cameraFront_ * cameraSpeed_);
            break;
        case GLUT_KEY_LEFT:
        {
            Vector3f cross = (cameraFront_.Cross(cameraUp_)).Normalize();
            cameraPos_ -= (cross * cameraSpeed_);
        }
        break;
        case GLUT_KEY_RIGHT:
        {
            Vector3f cross = (cameraFront_.Cross(cameraUp_)).Normalize();
            cameraPos_ += (cross * cameraSpeed_);
        }
        break;
        default:
            break;
        }
    }

    void Scene::KeyboardCB(unsigned char key, int mouse_x, int mouse_y)
    {
        switch (key)
        {
        case 'q':
        case 27: // escape key code
            exit(0);

        case '1':
        {
            Widget *w1 = *widgets_.begin();
            if (w1->isVisible())
                hideWidget(w1);
            else
                showWidget(w1);
        }
        break;

        case '2':
        {
            Widget *w2 = *(++widgets_.begin());
            if (w2->isVisible())
                hideWidget(w2);
            else
                showWidget(w2);
        }
        break;
        case '+':
        {
            float z = near_ + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (far_ - near_)));
            float rangeXY = z * tanh(fov_ / 2);
            float x = (-1 + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2))) * rangeXY * ((float)windowWidth_ / windowHeight_);
            float y = (-1 + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2))) * rangeXY;
            Widget *w = addWidget(CUBETEXTURED, cameraPos_ + cameraFront_ * z + cameraUp_ * y + cameraFront_.Cross(cameraUp_) * x, Vector3f(0., 1, 0.), 0.0);
            showWidget(w);
        }
        break;
        case '-':
        {
            if (widgets_.size() > 0)
            {
                Widget *w = *widgets_.rbegin();
                removeWidget(w);
            }
        }
        break;
        }
    }
}