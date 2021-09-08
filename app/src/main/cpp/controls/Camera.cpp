#include "Camera.h"

#include "../modules/glm/glm/ext.hpp"
#include "../graphics/render/MyRenderer.h"

class MyRenderer;

Camera::Camera(vec3 position, float fov) :
    mPosition(position),
    mFov(fov),
    mRotation(1.0f),
    front{},
    up{},
    right{}
{
    updateVectors();
}

void Camera::updateVectors()
{
    // В OpenGL камера смотрит в направлении -Z
    // Сейчас делаем так, чтобы ось X смотрела вправо, а Y - в верх
    front = vec3(mRotation * vec4(0, 0, -1, 1));
    right = vec3(mRotation * vec4(-1, 0, 0, 1));
    up = vec3(mRotation * vec4(0, 1, 0, 1));
}

void Camera::rotate(float x, float y, float z)
{
    mRotation = glm::rotate(mRotation, z, vec3(0, 0, 1));
    mRotation = glm::rotate(mRotation, y, vec3(0, 1, 0));
    mRotation = glm::rotate(mRotation, x, vec3(1, 0, 0));

    updateVectors();
}

mat4 Camera::getProjection() const
{
    float aspect = (float)MyRenderer::mWidth / (float)MyRenderer::mHeight;
    return glm::perspective(mFov, aspect, 0.1f, 15.f);
}

mat4 Camera::getView()
{
    return glm::lookAt(mPosition, mPosition + front, up);
}
