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

    Widget *Scene::addWidget(WidgetType widgetType, Vector3f position, Vector3f rotationAxis, float rotationAngle, bool insertToWidgets)
    {
        Widget *w = widgetFactory_.CreateWidget(widgetType);
        w->Position() = position;
        w->RotationAxis() = rotationAxis;
        w->RotationAngle() = rotationAngle;

        if (insertToWidgets)
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

    void Scene::setPhongShader(PhongShaderProgram *shaderHadle)
    {
        phongShaderProgram_ = shaderHadle;
    }

    void Scene::enablePhongShader()
    {
        phongShaderProgram_->Enable();
    }

    void Scene::setColorShader(ColorShaderProgram *shaderHadle)
    {
        colorShaderProgram_ = shaderHadle;
    }

    void Scene::enableColorShader()
    {
        colorShaderProgram_->Enable();
    }

    void Scene::setDirectionalLight(DirectionalLight *DirectionalLight)
    {
        light = DirectionalLight;
    }

    void Scene::drawWidget(Widget *w, Matrix4f &ProjectionMat, Matrix4f &CameraViewMat)
    {
        Matrix4f localToWorld = w->TransformationMat();
        Matrix4f matrix = ProjectionMat * CameraViewMat * localToWorld;
        colorShaderProgram_->SetMaterial(*w->getMaterial());
        colorShaderProgram_->SetTransformationMatrix(matrix);
        colorShaderProgram_->SetDirectionalLight(light, localToWorld);
        colorShaderProgram_->SetCamera(cameraPos_, localToWorld);
        glBindVertexArray(w->VAO());

        glDrawElements(GL_TRIANGLES, 3 * w->TrianglesNumber(), GL_UNSIGNED_INT, 0);
    }

    void Scene::drawWidget(Widget *w, Texture *texture, Matrix4f &ProjectionMat, Matrix4f &CameraViewMat)
    {
        // glUniform1i(SamplerLocation, 0);

        texture->Bind(GL_TEXTURE0);

        w->Rotate();

        Matrix4f localToWorld = w->TransformationMat();
        Matrix4f matrix = ProjectionMat * CameraViewMat * localToWorld;
        phongShaderProgram_->SetMaterial(*w->getMaterial());
        phongShaderProgram_->SetTransformationMatrix(matrix);
        phongShaderProgram_->SetTextureUnit(0);
        phongShaderProgram_->SetDirectionalLight(light, localToWorld);
        phongShaderProgram_->SetCamera(cameraPos_, localToWorld);
        glBindVertexArray(w->VAO());

        glDrawElements(GL_TRIANGLES, 3 * w->TrianglesNumber(), GL_UNSIGNED_INT, 0);
    }

    void Scene::draw()
    {
        Matrix4f CameraViewMat = this->CameraMat();
        Matrix4f ProjectionMat = this->ProjectionMat();

        enablePhongShader();
        for (Widget *w : visibleWidgets_)
        {
            Texture *texture = textures_[stol(w->id()) % textures_.size()];
            drawWidget(w, texture, ProjectionMat, CameraViewMat);
        }

        enableColorShader();
        drawWidget(playerWidget_, ProjectionMat, CameraViewMat);
    }

    void Scene::addTexture(std::string &filename)
    {
        Texture *text = new Texture(filename);
        text->Load();
        textures_.push_back(text);
    }

    void Scene::addWidgets(float startx, float endx, float startz, float endz, int count)
    {
        float stepx = std::abs(startx - endx) / (float)count;
        float stepz = std::abs(startz - endz) / (float)count;
        float stx = std::min(startx, endx);
        float stz = std::min(startz, endz);

        for (int ix = 0; ix < count; ix++)
        {
            for (int iy = 0; iy < count; iy++)
            {
                Vector3f widgetPos(stx + ix * stepx, 0.0f, stz + iy * stepz);
                std::cout << widgetPos;
                Widget *w = addWidget(CUBEWITHNORMALS, widgetPos, Vector3f(0.0f, 1.0f, 0.0f), 0.0f, true);
                showWidget(w);
            }
        }
    }

    void Scene::OnKeyboard(unsigned char key)
    {
        switch (key)
        {
        case GLUT_KEY_UP:
        {
            Vector3f dpos = (cameraUp_ * cameraSpeed_);
            dpos.y = 0;
            cameraPos_ += dpos;
            playerWidget_->Position() += dpos;

            std::cout << playerWidget_->Position();
        }
        break;
        case GLUT_KEY_DOWN:
        {
            Vector3f dpos = (cameraUp_ * cameraSpeed_);
            dpos.y = 0;
            cameraPos_ -= dpos;
            playerWidget_->Position() -= dpos;

            std::cout << playerWidget_->Position();
        }
        break;
        case GLUT_KEY_LEFT:
        {
            Vector3f cross = (cameraFront_.Cross(cameraUp_)).Normalize();
            Vector3f dpos = (cross * cameraSpeed_);
            dpos.y = 0;
            cameraPos_ -= dpos;
            playerWidget_->Position() -= dpos;

            std::cout << playerWidget_->Position();
        }
        break;
        case GLUT_KEY_RIGHT:
        {
            Vector3f cross = (cameraFront_.Cross(cameraUp_)).Normalize();
            Vector3f dpos = (cross * cameraSpeed_);
            cameraPos_ += dpos;
            playerWidget_->Position() += dpos;

            std::cout << playerWidget_->Position();
        }
        break;
        default:
            break;
        }
    }

    void Scene::OnMouseWheel(int button, int dir, int x, int y)
    {
        if (dir > 0)
            cameraPos_ += (cameraFront_ * cameraZoomSpeed_);
        else
            cameraPos_ -= (cameraFront_ * cameraZoomSpeed_);
    }

    void Scene::OnKeyboardSpecial(unsigned char key, int mouse_x, int mouse_y)
    {
        switch (key)
        {
        case 'q':
        case 27: // escape key code
        {
            glutLeaveMainLoop();
        }
        break;
        case '+':
        {
            float z = near_ + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (far_ - near_)));
            float rangeXY = z * tanh(fov_ / 2);
            float x = (-1 + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2))) * rangeXY * ((float)windowWidth_ / windowHeight_);
            float y = (-1 + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2))) * rangeXY;
            Vector3f widgetPos = cameraPos_ + cameraFront_ * z + cameraUp_ * y + cameraFront_.Cross(cameraUp_) * x;

            Widget *w = addWidget(CUBEWITHNORMALS, widgetPos, Vector3f(0., 1, 0.), 0.0, true);
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