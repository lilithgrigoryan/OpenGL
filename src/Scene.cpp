#include <cmath>
#include <algorithm>
#include "../include/Scene.h"
#include "../include/glmath.h"
#include "../include/Matrix3f.hpp"

namespace gl_scene
{
    int Scene::cameraPrevPosX = -1;
    int Scene::cameraPrevPosY = -1;
    int Scene::cameradx = -1;
    int Scene::camerady = -1;

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

    CameraUVN Scene::CalculateCameraUVN()
    {
        float sint = sinf(theta_ * M_PI / 180.f);
        float cost = cosf(theta_ * M_PI / 180.f);
        float sinp = sinf(phi_ * M_PI / 180.f);
        float cosp = cosf(phi_ * M_PI / 180.f);

        CameraUVN camerauvn;
        float y = cameraDistance_ * cost;
        float x = cameraDistance_ * sint * cosp;
        float z = cameraDistance_ * sint * sinp;
        camerauvn.position_ = playerWidget_->Position() + Vector3f(x, y, z);

        camerauvn.front_ = Vector3f(-sint * cosp, -cost, -sint * sinp).Normalize();
        camerauvn.up_ = Vector3f(-cost * cosp, sint, -cost * sinp).Normalize();
        camerauvn.left_ = Vector3f(sinp, 0, -cosp).Normalize();

        return camerauvn;
    }

    Matrix4f Scene::CameraMat()
    {
        Vector3f N = camera.front_;
        Vector3f V = camera.up_;
        Vector3f U = camera.left_;

        Matrix4f CameraMat(U.x, U.y, U.z, -U.Dot(camera.position_),
                           V.x, V.y, V.z, -V.Dot(camera.position_),
                           -N.x, -N.y, -N.z, N.Dot(camera.position_),
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
        colorShaderProgram_->SetCamera(camera.position_, localToWorld);
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
        phongShaderProgram_->SetCamera(camera.position_, localToWorld);
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
            Vector3f ey = Vector3f(0.0f, 1.0f, 0.0f);
            Vector3f dpos = ey.Cross(camera.left_) * cameraSpeed_;
            playerWidget_->Position() += dpos;
            camera = CalculateCameraUVN();
        }
        break;
        case GLUT_KEY_DOWN:
        {
            Vector3f ey = Vector3f(0.0f, 1.0f, 0.0f);
            Vector3f dpos = ey.Cross(camera.left_) * cameraSpeed_;
            playerWidget_->Position() -= dpos;
            camera = CalculateCameraUVN();
        }
        break;
        case GLUT_KEY_LEFT:
        {
            Vector3f dpos = (camera.left_ * cameraSpeed_);
            playerWidget_->Position() -= dpos;
            camera = CalculateCameraUVN();
        }
        break;
        case GLUT_KEY_RIGHT:
        {
            Vector3f dpos = (camera.left_ * cameraSpeed_);
            playerWidget_->Position() += dpos;
            camera = CalculateCameraUVN();
        }
        break;
        default:
            break;
        }
    }

    void Scene::OnMouseWheel(int button, int dir, int x, int y)
    {
        if (dir > 0)
            cameraDistance_ += cameraZoomSpeed_;
        else
            cameraDistance_ = std::max(0.f, cameraDistance_ - cameraZoomSpeed_);

        camera = CalculateCameraUVN();
    }

    void Scene::OnMouse(int button, int state, int x, int y)
    {
        if (button == GLUT_LEFT_BUTTON)
        {
            if (state == GLUT_DOWN)
            {
                cameraPrevPosX = x;
                cameraPrevPosY = y;
            }
            else
            {
                cameraPrevPosX = -1;
                cameraPrevPosY = -1;
            }
        }
    }

    void Scene::OnMouseActiveMove(int x, int y)
    {
        if (cameraPrevPosX != -1 && cameraPrevPosY != -1)
        {
            cameradx = x - cameraPrevPosX;
            camerady = y - cameraPrevPosY;
            cameraPrevPosX = x;
            cameraPrevPosY = y;

            if (cameradx != 0)
                phi_ += cameradx;

            if (camerady != 0 && std::abs(theta_ - camerady) < maxTheta_ && std::abs(theta_ - camerady) > minTheta_)
                theta_ -= camerady;

            camera = CalculateCameraUVN();
        }
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
            Vector3f widgetPos = camera.position_ + camera.front_ * z + camera.up_ * y + camera.left_ * x;

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