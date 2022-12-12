#ifndef PROG2002_GLFWAPPLICATION_H
#define PROG2002_GLFWAPPLICATION_H

#include <string>
#include <tclap/CmdLine.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLFWApplication {
protected:
    std::string appName;
    std::string appVersion;
    int winWidth;
    int winHeight;
    TCLAP::CmdLine cmd;

public:
    GLFWwindow* window;
    GLFWApplication(std::string  name, std::string  version);
    ~GLFWApplication();
    virtual void parseArguments(int argc, char **argv);
    virtual unsigned init();
    virtual int run() = 0;
};


#endif //PROG2002_GLFWAPPLICATION_H
