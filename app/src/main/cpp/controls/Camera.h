#ifndef ILLEGAL_RACER_CAMERA_H
#define ILLEGAL_RACER_CAMERA_H

#include "../modules/glm/glm/glm.hpp"

using namespace glm;

class Camera
{
public:
    Camera(vec3 position, float fov);
    ~Camera() = default;

    void rotate(float x, float y, float z);

    mat4 getProjection() const;
    mat4 getView();
private:
    float mFov;
    vec3 mPosition;

    vec3 front;
    vec3 up;
    vec3 right;
    mat4 mRotation;

    void updateVectors();
};

#endif
