#include "GLFWApplication.h"
#include <iostream>
#include <utility>
#include <ErrorHandler.h>

GLFWApplication::GLFWApplication(std::string name, std::string version) :
    cmd(appName, ' ', appVersion),
    appName(std::move(name)),
    appVersion(std::move(version)),
    winWidth(0), winHeight(0), window(nullptr) {
}

GLFWApplication::~GLFWApplication() {
    glfwTerminate();
}

void GLFWApplication::parseArguments(int argc, char **argv) {
    TCLAP::ValueArg<int> width("w", "width", "Set the window width", false, 800, "integer", cmd);
    TCLAP::ValueArg<int> height("g", "height", "Set the window height", false, 800, "integer", cmd);
    cmd.parse(argc, argv);
    winWidth = width;
    winHeight = height;
}

unsigned GLFWApplication::init() {

    // Initialize GLFW library
    if (!glfwInit())
        glfw_error_callback(0, "Failed to initialize");
    glfwSetErrorCallback(glfw_error_callback);

    // Set up Window
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
    glfwWindowHint(GLFW_SAMPLES, 4);
    window = glfwCreateWindow(winWidth, winHeight,
                              appName.c_str(), nullptr, nullptr);
    if (!window)
        glfw_error_callback(0, "Failed to create window");

    // Set up GL Context
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        glfw_error_callback(0, "Failed to initialize GLAD");

    // Enable debug
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_error_callback, nullptr);


    // Print out GL information
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << '\n';
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << '\n';
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';


    return 0;
}

