#include "ErrorHandler.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include <map>

void glfw_error_callback(int error, const char *description) {
    throw std::runtime_error("GLFW callback error "
                             + std::to_string(error) + ": " + description);

}

void gl_error_callback(GLenum source,
                       GLenum type,
                       GLuint /* id */,
                       GLenum severity,
                       GLsizei /* length */,
                       const GLchar *message,
                       const void* /* userParam */ ) {

    std::map<GLenum, std::string> typeRepr = {
            {GL_DEBUG_TYPE_ERROR, "ERROR"},
            {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Deprecated call invoked"},
            {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "Undefined behavior"},
            {GL_DEBUG_TYPE_PORTABILITY, "Unportable function called"},
            {GL_DEBUG_TYPE_PERFORMANCE, "Performance issue"},
            {GL_DEBUG_TYPE_MARKER, "Command stream annotation"},
            {GL_DEBUG_TYPE_PUSH_GROUP, "Group pushing"},
            {GL_DEBUG_TYPE_POP_GROUP, "Group popping"},
            {GL_DEBUG_TYPE_OTHER, "Other"}
    };

    std::map<GLenum, std::string> sourceRepr = {
            {GL_DEBUG_SOURCE_API, "OpenGL API"},
            {GL_DEBUG_SOURCE_WINDOW_SYSTEM, "Window-system API"},
            {GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader Compilation"},
            {GL_DEBUG_SOURCE_THIRD_PARTY, "3rd Party Application"},
            {GL_DEBUG_SOURCE_APPLICATION, "User of this application"},
            {GL_DEBUG_SOURCE_OTHER, "Other"}
    };
    std::map<GLenum, std::string> severityRepr = {
            {GL_DEBUG_SEVERITY_HIGH, "High"},
            {GL_DEBUG_SEVERITY_MEDIUM, "Medium"},
            {GL_DEBUG_SEVERITY_LOW, "Low"},
            {GL_DEBUG_SEVERITY_NOTIFICATION, "Other"}
    };
    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION &&
        type != GL_DEBUG_TYPE_OTHER) {
        std::cout << "Debugger:"
                  << "\n\tSeverity  " << severityRepr.at(severity)
                  << "\n\tType      " << typeRepr.at(type)
                  << "\n\tSource    " << sourceRepr.at(source)
                  << "\n\tMessage   " << message
                  << "\n\n";
    }
}

void gl_parse_shader_error(GLuint shaderID) {
    GLint type;         // Type of shader
    glGetShaderiv(shaderID, GL_SHADER_TYPE, &type);
    std::string typeRepr;
    switch (type) {
        case GL_VERTEX_SHADER:   typeRepr = "VERTEX";     break;
        case GL_FRAGMENT_SHADER: typeRepr = "FRAGMENT";   break;
        case GL_GEOMETRY_SHADER: typeRepr = "GEOMETRY";   break;
        default:                 typeRepr = "UNDEFINED";
    }
    auto msg = std::string("Compilation error in ") +
                typeRepr + " shader, see GL_CALLBACK message";
    glfwTerminate();
    throw std::runtime_error(msg);
}

void gl_parse_linker_error(GLuint programID) {
    int length;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*) alloca(length * sizeof(char));
    glGetProgramInfoLog(programID, length, &length, message);
    auto msg = std::string("Failed to link shaders to program ")
               + message;
    glDeleteProgram(programID);
    glfwTerminate();
    throw std::runtime_error(msg);
}

void raiseException(const std::string & str) {
    glfwTerminate();
    throw std::runtime_error(str);
}
