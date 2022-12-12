#include "PerspectiveCamera.h"

PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera::Frustrum &frustrum,
                                     const glm::vec3 &position,
                                     const glm::vec3 &target,
                                     const glm::vec3 &worldUp)
                                     : m_frustrum(frustrum),
                                       m_worldUp(worldUp),
                                       m_cameraYaw(-90.0f),
                                       m_cameraPitch(0.0f),
                                       m_cameraTowards(1.0f),
                                       m_cameraUp(1.0f),
                                       m_cameraRight(1.0f),
                                       Camera(position) {

    m_cameraTowards = glm::normalize(-m_cameraPos);
    m_cameraRight = glm::normalize(glm::cross(m_cameraTowards, m_worldUp));
    m_cameraUp = glm::normalize(glm::cross(m_cameraTowards, m_cameraRight));
    m_projectionMatrix = glm::perspectiveFov(glm::radians(m_frustrum.angle),
                                             m_frustrum.width,
                                             m_frustrum.height,
                                             m_frustrum.near,
                                             m_frustrum.far);
    this->m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraTowards, m_worldUp);
}

void PerspectiveCamera::SetFrustrum(const Frustrum& frustrum) {
    this->m_frustrum = frustrum;
    m_projectionMatrix = glm::perspectiveFov(glm::radians(m_frustrum.angle),
                                             m_frustrum.width,
                                             m_frustrum.height,
                                             m_frustrum.near,
                                             m_frustrum.far);
    this->RecalculateMatrix();
}

void PerspectiveCamera::SetLookAt(const glm::vec3& lookAt) {
    this->m_cameraTowards = lookAt - m_cameraPos;
    this->RecalculateMatrix();
}

void PerspectiveCamera::SetUpVector(const glm::vec3& upVector) {
    this->m_worldUp = upVector;
    this->RecalculateMatrix();
}

void PerspectiveCamera::RecalculateMatrix() {
    m_cameraRight = glm::normalize(glm::cross(m_cameraTowards, m_worldUp));
    m_cameraUp = glm::normalize(glm::cross(m_cameraTowards, m_cameraRight));
    this->m_viewMatrix = glm::lookAt(m_cameraPos, m_cameraPos + m_cameraTowards, m_worldUp);
}

void PerspectiveCamera::offsetTarget(const float xOffset, const float yOffset) {
   m_cameraYaw += xOffset;
   m_cameraPitch += yOffset;
   glm::vec3 target = {cos(glm::radians(m_cameraYaw)) * cos(glm::radians(m_cameraPitch)),
                       sin(glm::radians(m_cameraPitch)),
                       sin(glm::radians(m_cameraYaw)) * cos(glm::radians(m_cameraPitch))};
    m_cameraTowards = glm::normalize(target);
   RecalculateMatrix();
}

void PerspectiveCamera::offsetPosition(const glm::vec3 &direction, const float speed) {
    m_cameraPos += direction.x * speed * m_cameraRight;
    m_cameraPos += direction.y * speed * m_cameraUp;
    m_cameraPos += direction.z * speed * m_cameraTowards;
    RecalculateMatrix();
}

void PerspectiveCamera::rotate(const glm::vec3 &axis, const float rate) {
    auto transform = glm::rotate(glm::mat4(1.0f), rate, axis);
    m_cameraPos = transform * glm::vec4(m_cameraPos, 1.0f);
    m_cameraTowards = glm::normalize(-m_cameraPos);
    RecalculateMatrix();
}

void PerspectiveCamera::zoom(const float rate) {
    m_frustrum.angle -= rate * 15.0f;
    if (m_frustrum.angle < 20.0f)
        m_frustrum.angle = 20.0f;
    else if (m_frustrum.angle > 60.0f)
        m_frustrum.angle = 60.0f;
    m_projectionMatrix = glm::perspectiveFov(glm::radians(m_frustrum.angle),
                                             m_frustrum.width,
                                             m_frustrum.height,
                                             m_frustrum.near,
                                             m_frustrum.far);
}
