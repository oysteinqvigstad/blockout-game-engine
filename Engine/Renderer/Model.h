#ifndef PROG2002_MODEL_H
#define PROG2002_MODEL_H

#include <glm/glm.hpp>
#include "Shader.h"
#include <string>
#include <vector>
#include <VertexArray.h>

struct Vertex {
    float vx, vy, vz;
    float nx, ny, nz;
    float tx, ty;
};
//
//struct Texture {
//    unsigned int m_id;
//    std::string type;
//};

class Model {
private:
    std::shared_ptr<VertexArray> VAO;
    std::shared_ptr<VertexBuffer> VBO;
    std::shared_ptr<IndexBuffer> EBO;
    glm::mat4 transform;
    glm::mat3 normal;
    glm::mat4 rotation;
    glm::mat4 scale;
    glm::mat4 translation;
    void recalculateTransformMatrix();

public:
    template<typename vArray, typename iArray>
    Model(vArray vertices, iArray indices, const BufferLayout &layout) : transform(1.0f), rotation(1.0f), scale(1.0f), translation(1.0f), normal(1.0f) {
        VAO = std::make_shared<VertexArray>();
        VBO = std::make_shared<VertexBuffer>(vertices.data(), sizeof(vertices));
        EBO = std::make_shared<IndexBuffer>(indices.data(), indices.size());
        VAO->setIndexBuffer(EBO);
        VAO->addBuffer(VBO, layout);
    }



    void draw(const std::shared_ptr<Shader> &shader);
    void addRotation(const glm::vec3 &axis, float degrees);
    void setRotation(const glm::vec3 &axis, float degrees);
    void setTranslation(const glm::vec3 &displacement);
    void setScale(const glm::vec3 &multiplier);
    void bindVBO();
};


#endif //PROG2002_MODEL_H
