#ifndef ORTHOGRAPHICCAMERA_H_
#define ORTHOGRAPHICCAMERA_H_

#include "Camera.h"
#include <array>


class OrthographicCamera : public Camera {
public:
    struct Frustrum {
        float left;
        float right;
        float bottom;
        float top;
        float near;
        float far;
    };

protected:
    void RecalculateMatrix();
    float Rotation; // in degrees around the Z axis
    Frustrum CameraFrustrum;

public:
    OrthographicCamera(const Frustrum &frustrum = {-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f},
                       const glm::vec3 &position = glm::vec3(0.0f),
                       float rotation = 0.0f);
    ~OrthographicCamera()=default;
    OrthographicCamera(const OrthographicCamera& camera);
    void SetRotation(float rotation);
    void SetFrustrum(const Frustrum& frustrum);

};

#endif // ORTHOGRAPHICCAMERA_H_
