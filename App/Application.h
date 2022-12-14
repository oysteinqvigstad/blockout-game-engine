#ifndef PROG2002_APPLICATION_H
#define PROG2002_APPLICATION_H


#include "GLFWApplication.h"
#include <Model.h>
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
    int score = 0;
    static void drawCubes(bool squares[10][5][5], float interpolated[10][5][5], float dt, Model &cube, const std::shared_ptr<Shader> &shader);
    void removeLines(bool squares[10][5][5], float interpolated[10][5][5]);
    static void setLights(const std::shared_ptr<Shader> &shader);
    static void setupAllLights(float constant, float linear, float quadric);
    static float lerp(float start, float end, float pt);
    static float sstep3(float dt);
    static glm::vec2 calcLight2DPos(float time);
    static void drawWalls(Model &farWall, Model &sideWall, const std::shared_ptr<Shader> &shader);
    static glm::vec4 getLevelColor(int level);
public:
    Application(const std::string& name, const std::string& version);
    void parseArguments(int argc, char **argv) override;
    unsigned init() override;
    int run() override;

};

#endif //PROG2002_APPLICATION_H
