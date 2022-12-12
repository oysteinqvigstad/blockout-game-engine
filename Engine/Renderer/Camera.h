#ifndef CAMERA_H_
#define CAMERA_H_

#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
protected:
    glm::mat4 m_projectionMatrix = glm::mat4(1.0f);
    glm::mat4 m_viewMatrix = glm::mat4(1.0f);
    glm::mat4 m_viewProjectionMatrix = glm::mat4(1.0f);
    glm::vec3 m_cameraPos = glm::vec3(0.0f);
    virtual void RecalculateMatrix() = 0;

public:
    Camera()=default;
    explicit Camera(const glm::vec3 &position) : m_cameraPos(position) {}
    Camera(const Camera& camera) {
        this->m_projectionMatrix = camera.m_projectionMatrix;
        this->m_viewMatrix = camera.m_viewMatrix;
        this->m_cameraPos = camera.m_cameraPos;
        this->m_viewProjectionMatrix = camera.m_viewProjectionMatrix;
    }

    ~Camera()=default;

    // Get camera matrices
    [[nodiscard]] const glm::mat4& GetProjectionMatrix() const {
        return this->m_projectionMatrix;
    }

    [[nodiscard]] const glm::mat4& GetViewMatrix() const {
        return this->m_viewMatrix;
    }

    [[nodiscard]] const glm::mat4& GetViewProjectionMatrix() const {
        return this->m_viewProjectionMatrix;
    }

    // Set/Get position
    [[nodiscard]] const glm::vec3& GetPosition() const {
        return this->m_cameraPos;
    }

    void SetPosition(const glm::vec3& pos) {
        this->m_cameraPos = pos;
        this->RecalculateMatrix();
    }
};

#endif // CAMERA_H_
