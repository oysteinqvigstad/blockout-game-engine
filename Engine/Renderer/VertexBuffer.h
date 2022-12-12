#ifndef PROG2002_VERTEXBUFFER_H
#define PROG2002_VERTEXBUFFER_H

#include <glad/glad.h>
#include "BufferLayout.h"

class VertexBuffer {
private:
    GLuint m_vertexBufferID{};
    BufferLayout m_layout;
public:
    VertexBuffer(const void *vertices, GLsizei size);
    ~VertexBuffer();
    void bind() const;
    static void unbind();
    [[nodiscard]] const BufferLayout& getLayout() const { return m_layout; }
    void setLayout(const BufferLayout& layout) { m_layout = layout; }
};


#endif //PROG2002_VERTEXBUFFER_H
