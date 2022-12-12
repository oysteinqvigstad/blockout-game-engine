#ifndef PERSPECTIVECAMERA_H_
#define PERSPECTIVECAMERA_H_

#include "Camera.h"
#include <array>

class PerspectiveCamera : public Camera {
public:
    struct Frustrum {
        float angle;
        float width;
        float height;
        float near;
        float far;
    };

protected:
    Frustrum m_frustrum;
    glm::vec3 m_worldUp;
    glm::vec3 m_cameraTowards;
    glm::vec3 m_cameraUp;
    glm::vec3 m_cameraRight;
    float m_cameraYaw;
    float m_cameraPitch;
    void RecalculateMatrix() override;

public:
    explicit PerspectiveCamera(
            const Frustrum &frustrum = {45.0f, -1.0f, 1.0f, 1.0f, -1.0f},
            const glm::vec3 &position = glm::vec3(0.0f, 0.0f, 5.0f),
            const glm::vec3 &target = glm::vec3(0.0f, 0.0f, 0.0f),
            const glm::vec3 &worldUp = glm::vec3(0.0f, 1.0f, 0.0f));
    ~PerspectiveCamera()=default;
    void SetFrustrum(const Frustrum& frustrum);
    void SetLookAt(const glm::vec3& lookAt);
    void offsetTarget(float x, float y);
    void offsetPosition(const glm::vec3 &position, float speed);
    void rotate(const glm::vec3 &axis, float rate);
    void zoom(float rate);
    void SetUpVector(const glm::vec3& upVector);

};

#endif // PERSPECTIVECAMERA_H_

