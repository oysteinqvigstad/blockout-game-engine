#ifndef PROG2002_INDEXBUFFER_H
#define PROG2002_INDEXBUFFER_H

#include <glad/glad.h>

class IndexBuffer {
private:
    GLuint m_indexBufferID;
    GLsizei m_count;
public:
    IndexBuffer(const void* data, GLsizei count);
    ~IndexBuffer();
    void bind() const;
    static void unbind() ;
    [[nodiscard]] inline GLsizei getCount() const { return m_count; }
};


#endif //PROG2002_INDEXBUFFER_H
