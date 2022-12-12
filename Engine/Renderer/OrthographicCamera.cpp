#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera(const Frustrum &frustrum,
                                       const glm::vec3 &position,
                                       float rotation) {
    this->CameraFrustrum = frustrum;
    this->m_cameraPos = position;
    this->Rotation = rotation;
}

OrthographicCamera::OrthographicCamera(const OrthographicCamera& camera) : Camera(camera) {
    this->Rotation = camera.Rotation;
    this->CameraFrustrum = camera.CameraFrustrum;
}

void OrthographicCamera::SetRotation(float rotation) {
    this->Rotation = rotation;
    this->RecalculateMatrix();
}

void OrthographicCamera::SetFrustrum(const Frustrum& frustrum) {
    this->CameraFrustrum = frustrum;
    this->RecalculateMatrix();
}
