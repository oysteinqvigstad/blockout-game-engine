#include "Shader.h"
#include "ErrorHandler.h"
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

Shader::Shader(const std::string &vertexSrc, const std::string &fragmentSrc)
    : m_vertexShader(0), m_fragmentShader(0) {

    // Compile shaders and attach to program
    m_shaderProgram = glCreateProgram();
    compileShader(GL_VERTEX_SHADER, vertexSrc);
    compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);
    glLinkProgram(m_shaderProgram);

    // Check for linking errors
    int result = 0;
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &result);
    if (!result)
        gl_parse_linker_error(m_shaderProgram);
    glUseProgram(m_shaderProgram);

}

Shader::~Shader() {
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);
    glDeleteProgram(m_shaderProgram);
}

void Shader::compileShader(GLenum shaderType, const std::string &shaderSrc) {
    GLuint shaderID = glCreateShader(shaderType);
    const GLchar* vss = shaderSrc.c_str();
    GLint result = 0;

    // Generate and compile shader
    glShaderSource(shaderID, 1, &vss, nullptr);
    glCompileShader(shaderID);

    // Check for compilation errors
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
    if (!result)
        gl_parse_shader_error(shaderID);

    if (shaderType == GL_VERTEX_SHADER)
        m_vertexShader = shaderID;
    else
        m_fragmentShader = shaderID;

}

void Shader::bind() const {
    glUseProgram(m_shaderProgram);
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::setUniform(const std::string &name, const GLint value) const {
    glUniform1i(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string &name, const glm::vec2 &vector) const {
    glUniform2f(getUniformLocation(name), vector.x, vector.y);
}

void Shader::setUniform(const std::string &name, const glm::vec4 &vector) const {
    glUniform4f(getUniformLocation(name), vector.x, vector.y, vector.z, vector.w);
}
void Shader::setUniform(const std::string &name, const glm::mat3 &matrix) const {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::setUniform(const std::string &name, const glm::mat4 &matrix) const {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform(const std::string &name, const GLfloat value) const {
    glUniform1f(getUniformLocation(name), value);
}

void Shader::setUniform(const std::string &name, const glm::vec3 &vector) const {
    glUniform3f(getUniformLocation(name), vector.x, vector.y, vector.z);
}

GLint Shader::getUniformLocation(const std::string &name) const {
    bind();
    return glGetUniformLocation(m_shaderProgram, name.c_str());
}

void Shader::uploadSpotlight(const std::string &name) {
    LightManager::Spotlight* spotLight = nullptr;
    auto light = std::find_if(m_spotlights.begin(), m_spotlights.end(),
                              [&name](const auto &spot){ return name == spot->m_name; });
    if (light == m_spotlights.end()) {
        spotLight = LightManager::GetInstance()->getSpotLight(name);
        if (spotLight != nullptr) {
            m_spotlights.push_back(spotLight);
            light = std::find_if(m_spotlights.begin(), m_spotlights.end(),
                                 [&name](const auto &spot){ return name == spot->m_name; });
        }
    }
    if (light == m_spotlights.end())
        raiseException("Could not find light source to update shader with. Check name");

    spotLight = *light;
    auto index = std::to_string(std::distance(m_spotlights.begin(), light));
    setUniform("u_spotLights["+index+"].position", spotLight->m_position);
    setUniform("u_spotLights["+index+"].ambient", spotLight->m_ambient);
    setUniform("u_spotLights["+index+"].diffuse", spotLight->m_diffuse);
    setUniform("u_spotLights["+index+"].specular", spotLight->m_specular);
    setUniform("u_spotLights["+index+"].constant", spotLight->m_constant);
    setUniform("u_spotLights["+index+"].linear", spotLight->m_linear);
    setUniform("u_spotLights["+index+"].quadratic", spotLight->m_quadratic);
    setUniform("u_numOfSpotlights", static_cast<GLint>(m_spotlights.size()));


}


