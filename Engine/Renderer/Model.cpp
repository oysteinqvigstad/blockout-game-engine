#include "Model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <iostream>
#include <sstream>

#define TINYOBJLOADER_IMPLEMENTATION

void Model::setTranslation(const glm::vec3 &displacement) {
    translation = glm::translate(glm::mat4(1.0f), displacement);
    recalculateTransformMatrix();
}

void Model::bindVBO() {
    VAO->bind();
    VBO->bind();
}

void Model::setScale(const glm::vec3 &multiplier) {
    scale = glm::scale(glm::mat4(1.0f), multiplier);
    recalculateTransformMatrix();
}

void Model::setRotation(const glm::vec3 &axis, float degrees) {
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(degrees), axis);
    recalculateTransformMatrix();
}

void Model::draw(const std::shared_ptr<Shader> &shader) {
    VAO->bind();
    EBO->bind();
    shader->bind();
    shader->setUniform("u_model", transform);
    shader->setUniform("u_normal", normal);
    glDrawElements(GL_TRIANGLES, VAO->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
}

void Model::recalculateTransformMatrix() {
    transform = translation * rotation * scale;
    normal = glm::inverseTranspose(glm::mat3(transform));
}


void Model::addRotation(const glm::vec3 &axis, float degrees) {
    rotation = glm::rotate(rotation, glm::radians(degrees), axis);
    recalculateTransformMatrix();
}


