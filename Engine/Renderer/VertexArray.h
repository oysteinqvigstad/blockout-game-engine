#ifndef PROG2002_VERTEXARRAY_H
#define PROG2002_VERTEXARRAY_H

#include <glad/glad.h>
#include <vector>
#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

class VertexArray {
private:
    GLuint m_vertexArrayID;
    std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
public:
    VertexArray();
    ~VertexArray();
    void bind() const;
    static void unbind() ;
    void addBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer,
                   const BufferLayout &layout);
    void setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer);
    std::shared_ptr<IndexBuffer> getIndexBuffer();
    [[nodiscard]] const std::vector<std::shared_ptr<VertexBuffer>> &getVertexBuffers() const {
        return m_vertexBuffers;
    }

};




#endif //PROG2002_VERTEXARRAY_H
