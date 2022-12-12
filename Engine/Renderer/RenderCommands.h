#ifndef PROG2002_RENDERCOMMANDS_H
#define PROG2002_RENDERCOMMANDS_H

#include <glad/glad.h>
#include "VertexArray.h"
#include "IndexBuffer.h"

namespace RenderCommands {

    inline void clear(GLuint mode = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT){
        glClear(mode);
    }

    inline void setPolygonMode(GLenum face, GLenum mode) {
        glPolygonMode(face, mode);
    }

    inline void draw(std::shared_ptr<VertexArray> &VAO, std::shared_ptr<IndexBuffer> &EBO, std::shared_ptr<Shader> &shader) {
        VAO->bind();
        EBO->bind();
        shader->bind();
        glDrawElements(GL_TRIANGLES, VAO->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);

    }

    inline void setClearColor(glm::vec4 color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    inline void setWireframeMode(const std::shared_ptr<Shader>& shader) {
        setPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        shader->setUniform("line", true);
    }

    inline void setSolidMode(const std::shared_ptr<Shader>& shader) {
        setPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        shader->setUniform("line", false);
    }

}



#endif //PROG2002_RENDERCOMMANDS_H
