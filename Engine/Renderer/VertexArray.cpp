#include "VertexArray.h"

VertexArray::VertexArray() : m_vertexArrayID(0) {
    glGenVertexArrays(1, &m_vertexArrayID);
    this->bind();
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_vertexArrayID);
}

void VertexArray::bind() const {
    glBindVertexArray(m_vertexArrayID);
}

void VertexArray::unbind() {
    glBindVertexArray(0);

}

void VertexArray::addBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer,
                            const BufferLayout &layout) {
    this->bind();
    vertexBuffer->bind();
    m_vertexBuffers.push_back(vertexBuffer);
    auto attributes = layout.getAttributes();
    auto stride = layout.getStride();
    for (int i = 0; i < attributes.size(); i++) {
        auto count = attributes[i].m_count;
        auto type = attributes[i].m_gl_type;
        auto norm = attributes[i].m_normalized;
        auto offset = attributes[i].m_offset;
        glVertexAttribPointer(i, count, type, norm, stride, (void*)(size_t)offset);
        glEnableVertexAttribArray(i);
    }


}

void VertexArray::setIndexBuffer(const std::shared_ptr <IndexBuffer> &indexBuffer) {
    this->bind();
    m_indexBuffer = indexBuffer;
    m_indexBuffer->bind();
}

std::shared_ptr<IndexBuffer> VertexArray::getIndexBuffer() {
    return {m_indexBuffer};
}
