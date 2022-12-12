#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, GLsizei count) : m_count(count), m_indexBufferID(0) {
    glGenBuffers(1, &m_indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(count * sizeof(GLuint)), data, GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &m_indexBufferID);
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_indexBufferID);
}

void IndexBuffer::unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
