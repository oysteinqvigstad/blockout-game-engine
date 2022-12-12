#ifndef PROG2002_ERRORHANDLER_H
#define PROG2002_ERRORHANDLER_H

#include <glad/glad.h>
#include <string>

void glfw_error_callback(int error, const char* description);
void gl_error_callback( GLenum source,
                        GLenum type,
                        GLuint id,
                        GLenum severity,
                        GLsizei length,
                        const GLchar* message,
                        const void* userParam );
void gl_parse_shader_error(GLuint shaderID);
void gl_parse_linker_error(GLuint programID);
void raiseException(const std::string & str);
#endif //PROG2002_ERRORHANDLER_H
