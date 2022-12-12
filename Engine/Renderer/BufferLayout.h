#ifndef PROG2002_BUFFERLAYOUT_H
#define PROG2002_BUFFERLAYOUT_H

#include "ShaderDataType.h"
#include <string>
#include <utility>
#include <vector>

struct BufferAttribute {
    std::string m_name;
    [[maybe_unused]] ShaderDataType m_type;      // e.g. Float2
    GLenum m_gl_type;                            // e.g. GL_FLOAT
    GLuint m_size;                               // In bytes
    GLint  m_count;                              // Number of elements
    GLuint m_offset;                             // In bytes
    GLboolean m_normalized;

    BufferAttribute(ShaderDataType type,
                    std::string name,
                    GLboolean normalized = false) :
                    m_name(std::move(name)),
                    m_type(type),
                    m_gl_type(ShaderDataTypeToOpenGLBaseType(type)),
                    m_size(ShaderDataTypeSize(type)),
                    m_count(ShaderDataTypeComponentCount(type)),
                    m_normalized(normalized),
                    m_offset(0) {
    };
};


class BufferLayout {
private:
    std::vector<BufferAttribute> m_attributes;
    GLsizei m_stride;


    void calculateOffsetAndStride() {
        GLsizei offset = 0;
        m_stride = 0;
        for (auto & attribute : m_attributes) {
            attribute.m_offset = offset;
            offset += static_cast<GLsizei>(attribute.m_size);
            m_stride += static_cast<GLsizei>(attribute.m_size);
        }
    }

public:
    BufferLayout() : m_stride(0) {}

    BufferLayout(const std::initializer_list<BufferAttribute> &attributes)
            : m_attributes(attributes), m_stride(0) {
        calculateOffsetAndStride();
    }

    [[nodiscard]] inline std::vector<BufferAttribute> getAttributes() const {
        return m_attributes;
    }

    [[nodiscard]] inline GLsizei getStride() const {
        return m_stride;
    }

    [[nodiscard]] std::vector<BufferAttribute>::iterator begin() {
        return m_attributes.begin();
    }

    [[nodiscard]] std::vector<BufferAttribute>::iterator end() {
        return m_attributes.end();
    }

    [[nodiscard]] std::vector<BufferAttribute>::const_iterator begin() const {
        return m_attributes.begin();
    }

    [[nodiscard]] std::vector<BufferAttribute>::const_iterator end() const {
        return m_attributes.end();
    }
};


#endif //PROG2002_BUFFERLAYOUT_H
