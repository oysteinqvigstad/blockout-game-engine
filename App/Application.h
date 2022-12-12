#ifndef PROG2002_APPLICATION_H
#define PROG2002_APPLICATION_H


#include "GLFWApplication.h"
#include <VertexArray.h>
#include <GeometricTools.h>
#include <ShaderDataType.h>
#include <VertexArray.h>
#include <VertexBuffer.h>
#include <IndexBuffer.h>
#include <BufferLayout.h>
#include <Shader.h>
#include <RenderCommands.h>
#include <glm/glm.hpp>
#include <array>

class Application : public GLFWApplication {
private:
    glm::ivec2 selectorPos;       // X and Y pos of the selector in [0,numSq]
    int numSquares[2];            // Number of squares in X and Y direction
    void moveSelector(int x, int y);
    std::vector<std::shared_ptr<Shader>> allShaders;
    void activateSelector(std::list<GeometricTools::chessPiece> &pieces);
    template <typename T>
    void setUniformAllShaders(const std::string &str, T value);
public:
    Application(const std::string& name, const std::string& version);
    void parseArguments(int argc, char **argv) override;
    unsigned init() override;
    int run() override;

};

#endif //PROG2002_APPLICATION_H
