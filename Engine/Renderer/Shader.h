#ifndef PROG2002_SHADER_H
#define PROG2002_SHADER_H

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <LightManager.h>
#include <vector>
class Shader {


private:
    GLuint m_vertexShader;
    GLuint m_fragmentShader;
    GLuint m_shaderProgram;
    std::vector<LightManager::Spotlight*> m_spotlights;
    void compileShader(GLenum shaderType, const std::string & shaderSrc);

public:
    Shader(const std::string & vertexSrc, const std::string & fragmentSrc);
    ~Shader();
    void bind() const;
    static void unbind();
    void uploadSpotlight(const std::string &name);
    void setUniform(const std::string &name, GLint value) const;
    void setUniform(const std::string &name, GLfloat value) const;
    void setUniform(const std::string &name, const glm::vec2 & vector) const;
    void setUniform(const std::string &name, const glm::vec3 &vector) const;
    void setUniform(const std::string &name, const glm::vec4 &vector) const;
    void setUniform(const std::string &name, const glm::mat3 &matrix) const;
    void setUniform(const std::string &name, const glm::mat4x4 &matrix) const;
    [[nodiscard]] GLint getUniformLocation(const std::string & name) const;
};



#endif //PROG2002_SHADER_H
