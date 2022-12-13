#include "Application.h"
#include "resources/shaders/shader_v.glsl"
#include "resources/shaders/shader_f.glsl"
#include <Keyboard.h>
#include <TextureManager.h>
#include <PerspectiveCamera.h>
#include <LightManager.h>
#include <Model.h>
#include "ActiveBlock.h"

Application::Application(const std::string &name,
                         const std::string &version) :
    GLFWApplication(name, version),
    selectorPos{},
    numSquares{} {
}



void Application::parseArguments(int argc, char **argv) {
    GLFWApplication::parseArguments(argc, argv);
}

unsigned Application::init() {
    GLFWApplication::init();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_MULTISAMPLE);  // 4x
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // s*alpha + d(1-alpha)
    RenderCommands::setClearColor({0.5f, 0.0f, 0.0f, 1.0f});
    return 0;
}

int Application::run() {
    const int row = 5;
    const int col = 5;
    numSquares[0] = row;
    numSquares[1] = col;

    // SHADERS
    auto shader = std::make_shared<Shader>(vertexShaderSrc, shader_f);

    // SETTING UP CAMERA
    PerspectiveCamera camera({45.0f, static_cast<float>(winWidth), static_cast<float>(winHeight), 1.0f, -10.0f}, {2.0f, 2.0f, 15.5f}, {2.0f, 2.0f, 0.0f});

    Model tunnelFarSide(GeometricTools::UnitGridGeometry3DWTCoords<5, 5>(),
                        GeometricTools::UnitGridTopologyTriangles<5, 5>(),
                        BufferLayout({{ShaderDataType::Float3, "position"},
                             {ShaderDataType::Float3, "color"},
                             {ShaderDataType::Float2, "texture"}}));
    tunnelFarSide.setScale({5.0f, 5.0f, 5.0f});
    tunnelFarSide.setTranslation({2.0f, 2.0f, -0.5f});

    Model tunnelSideWall(GeometricTools::UnitGridGeometry3DWTCoords<10, 5>(),
                         GeometricTools::UnitGridTopologyTriangles<10, 5>(),
                         BufferLayout({{ShaderDataType::Float3, "position"},
                                      {ShaderDataType::Float3, "color"},
                                      {ShaderDataType::Float2, "texture"}}));
    tunnelSideWall.setScale({10.0f, 5.0f, 5.0f});



//    Model skybox(GeometricTools::UnitCube3D24WNormals,
//                 GeometricTools::UnitCube3DTopologyTriangles24,
//                 BufferLayout({{ShaderDataType::Float3, "position"},
//                               {ShaderDataType::Float3, "normals"}}));
//    skybox.setScale({100.0f, 100.0f, 100.0f});
//    skybox.setTranslation({0.0f, -10.0f, -10.0f});

    ActiveBlock activeBlock;

    // TEXTURES
    auto textureManager = TextureManager::GetInstance();
    textureManager->LoadTexture2D("floor", "walls.jpg", 0);
    textureManager->LoadCubeMap("cube", {"cube_texture.jpg"}, 1);
    textureManager->LoadCubeMap("active-box", {"transparent-box.png"}, 3);
    textureManager->LoadCubeMap("skybox", {"front.jpg", "back.jpg", "top.jpg", "bottom.jpg", "right.jpg", "left.jpg"}, 2);
    shader->setUniform("u_flatTexture", 0);
    shader->setUniform("u_cubeTexture", 1);


    // Cube
    auto chessPieces = GeometricTools::createPieces<row, col>();
    Model cube(GeometricTools::UnitCube3D24WNormals,
               GeometricTools::UnitCube3DTopologyTriangles24,
               BufferLayout({{ShaderDataType::Float3, "position"},
                             {ShaderDataType::Float3, "normals"}}));

    cube.setScale({1.0f, 1.0f, 1.0f});
    bool squares[10][5][5] = {};



    // KEYBOARD
    auto keyEsc = Keyboard(GLFW_KEY_ESCAPE);
    auto keyUp = Keyboard(GLFW_KEY_UP, 350);
    auto keyDown = Keyboard(GLFW_KEY_DOWN, 350);
    auto keyLeft = Keyboard(GLFW_KEY_LEFT, 350);
    auto keyRight = Keyboard(GLFW_KEY_RIGHT, 350);
    auto keySpace = Keyboard(GLFW_KEY_SPACE, 350);
    auto keyX = Keyboard(GLFW_KEY_X, 350);
    auto keyI = Keyboard(GLFW_KEY_I, 350);  // Toggles illumination
    auto keyT = Keyboard(GLFW_KEY_T, 500);  // Toggles blending
    auto keyQ = Keyboard(GLFW_KEY_Q);       // Quiting the application

    bool blendTexturesWithColor = false;
    bool illuminate = false;
    float elapsedTime, deltaTime, lastTime, timeSinceLastDrop = 0.0f;

    // Lights
//    auto lightPos = glm::vec3(15.0f, 2.0f, 1.0f);
    auto lightManager = LightManager::GetInstance();

    setupAllLights(0.5f, 0.0f, 0.7f);
//    lightManager->addSpotLight({"spot", lightPos, 0.2f, 0.0f, 0.5f});
//    lightManager->moveLight("spot", {-15.0f, 0.0f, 0.0f});
//    shader->uploadSpotlight("spot");

    RenderCommands::setClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        RenderCommands::clear();

        elapsedTime = static_cast<float>(glfwGetTime());
        deltaTime = elapsedTime - lastTime;
        lastTime = elapsedTime;
        timeSinceLastDrop += deltaTime;

        if (keyEsc.isPressed(window) || keyQ.isPressed(window))
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        if (keyUp.isPressed(window))
            activeBlock.moveSideways(squares, 0, 1);
        if (keyDown.isPressed(window))
            activeBlock.moveSideways(squares, 0, -1);
        if (keyLeft.isPressed(window))
            activeBlock.moveSideways(squares, -1, 0);
        if (keyRight.isPressed(window))
            activeBlock.moveSideways(squares, 1, 0);
        if (keyX.isPressed(window))
            activeBlock.goDown(squares, timeSinceLastDrop);
        if (keySpace.isPressed(window))
            activeBlock.goDown(squares, timeSinceLastDrop, true);

        if (timeSinceLastDrop > 2.0f) {
            activeBlock.goDown(squares, timeSinceLastDrop);
        }

        removeLines(squares);

        if (keyI.isPressed(window)) {
            illuminate = !illuminate;
            shader->setUniform("u_illuminate", illuminate);
        }

        if (keyT.isPressed(window)) {
            blendTexturesWithColor = !blendTexturesWithColor;
            shader->setUniform("u_blend", blendTexturesWithColor);
        }

        shader->setUniform("u_projection", camera.GetProjectionMatrix());
        shader->setUniform("u_view", camera.GetViewMatrix());
        shader->setUniform("u_cameraPos", camera.GetPosition());

//        shader->setUniform("u_cubeTexture", 2);
//        shader->setUniform("u_skybox", true);
//        shader->setUniform("u_view", glm::mat4(glm::mat3(camera.GetViewMatrix())));
//        skybox.draw(shader);
//        shader->setUniform("u_skybox", false);
//        shader->setUniform("u_cubeTexture", 1);
//        shader->setUniform("u_view", camera.GetViewMatrix());

//      WALLS
        shader->setUniform("u_walls", true);

        shader->setUniform("u_walls_normal", {0.0f, 0.0f, 1.0f});
        tunnelFarSide.draw(shader);

        // left
        shader->setUniform("u_walls_normal", {1.0f, 0.0f, 0.0f});
        tunnelSideWall.setRotation({0.0f, 1.0f, 0.0f}, -90);
        tunnelSideWall.setTranslation({-0.5f, 2.0f, 4.5f});
        tunnelSideWall.draw(shader);

        // right
        shader->setUniform("u_walls_normal", {-1.0f, 0.0f, 0.0f});
        tunnelSideWall.setTranslation({4.5f, 2.0f, 4.5f});
        tunnelSideWall.draw(shader);

        // top
        shader->setUniform("u_walls_normal", {0.0f, -1.0f, 0.0f});
        tunnelSideWall.setRotation({0.0f, 0.0f, 1.0f}, 90);
        tunnelSideWall.addRotation({0.0f, 1.0f, 0.0f}, 90);
        tunnelSideWall.setTranslation({2.0f, 4.5f, 4.5f});
        tunnelSideWall.draw(shader);

        // bottom
        shader->setUniform("u_walls_normal", {0.0f, 1.0f, 0.0f});
        tunnelSideWall.setTranslation({2.0f, -0.5f, 4.5f});
        tunnelSideWall.draw(shader);

        shader->setUniform("u_walls", false);



        shader->setUniform("u_cubemap", true);
        shader->setUniform("u_cubeTexture", 1);
        drawCubes(squares, cube, shader);
        shader->setUniform("u_cubeTexture", 3);
        activeBlock.draw(shader);
        if (!blendTexturesWithColor)
            activeBlock.draw(shader, true);
        shader->setUniform("u_cubemap", false);


        // light
//        lightManager->setPosition("spot", {2+ 2 * cos(elapsedTime*2), 2 +2 * sin(elapsedTime*2), 5.0f});
//        shader->uploadSpotlight("spot");
        cube.setScale({0.25f, 0.25f, 0.25f});
        setLights(shader);
//        cube.setTranslation(lightManager->getSpotLight("spot")->m_position);
//        cube.draw(shader);

//        lightManager->setPosition("spot2", {2+ 2 * cos(elapsedTime*2), 2 +2 * sin(elapsedTime*2), 7.0f});
//        shader->uploadSpotlight("spot2");
//        cube.setTranslation(lightManager->getSpotLight("spot2")->m_position);
//        cube.draw(shader);
        cube.setScale({1.0f, 1.0f, 1.0f});

        glfwSwapBuffers(window);
    }
    return 0;
}

void Application::moveSelector(int x, int y) {
    // Calculate new position
    int newPosX = selectorPos.x + x;
    int newPosY = selectorPos.y + y;
    // Check if move is legal
    if (newPosX < numSquares[0] && newPosX >= 0 &&
        newPosY < numSquares[1] && newPosY >= 0) {

        // set the color of the current tile
        std::array<float, 3> white{1.0f, 1.0f, 1.0f};
        std::array<float, 3> black{0.0f, 0.0f, 0.0f};
        auto color = ((selectorPos[0] + selectorPos[1]) % 2) ? &white : &black;
        int stride = 8 * sizeof(float);
        int offset = 3 * sizeof(float);
        offset += ((selectorPos.x+1) + selectorPos.y * (numSquares[0]+1)) * stride;
        glBufferSubData(GL_ARRAY_BUFFER, offset, 3*sizeof(float), color);

        // Update new position
        selectorPos.x = newPosX;
        selectorPos.y = newPosY;

        // set color of new tile
        std::array<float, 3> green {0.0f, 1.0f, 0.0f};
        offset = 3 * sizeof(float);
        offset += ((selectorPos.x+1) + selectorPos.y * (numSquares[0]+1)) * stride;
        glBufferSubData(GL_ARRAY_BUFFER, offset, 3*sizeof(float), green.data());

    }
}

void Application::activateSelector(std::list<GeometricTools::chessPiece> &pieces) {
    auto highlightedPiece = std::find_if(pieces.begin(), pieces.end(), [](const auto & piece) { return piece.highlighted; });
    auto selectPiece = std::find_if(pieces.begin(), pieces.end(), [&](const auto & piece) { return selectorPos == piece.squareNumber; });
    if (highlightedPiece != pieces.end()) {
        if (selectPiece == pieces.end())
            highlightedPiece->squareNumber = selectorPos;
        highlightedPiece->highlighted = false;
    } else if (selectPiece != pieces.end()) {
        selectPiece->highlighted = true;
    }
}

void Application::drawCubes(bool (*squares)[5][5],
                            Model cube,
                            const std::shared_ptr<Shader> &shader) {

    glm::vec4 colors[10] = {{1.0f, 0.0f, 0.0f, 1.0f},
                            {0.0f, 1.0f, 0.0f, 1.0f},
                            {0.0f, 0.0f, 1.0f, 1.0f},
                            {1.0f, 0.0f, 0.0f, 1.0f},
                            {0.0f, 1.0f, 0.0f, 1.0f},
                            {0.0f, 0.0f, 1.0f, 1.0f},
                            {1.0f, 0.0f, 0.0f, 1.0f},
                            {0.0f, 1.0f, 0.0f, 1.0f},
                            {0.0f, 0.0f, 1.0f, 1.0f},
                            {1.0f, 0.0f, 0.0f, 1.0f}};

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                if (squares[i][j][k]) {
                    shader->setUniform("u_color", colors[i]);
                    cube.setTranslation({k, j, i});
                    cube.draw(shader);
                }
            }
        }
    }
}

void Application::removeLines(bool squares[10][5][5]) {
    int oldScore = score;
    for (int i = 0; i < 10; i++) {
        bool forRemoval[5][5] = {};
        for (int j = 0; j < 5; j++) {
            if (squares[i][j][0] && squares[i][j][1] &&
                squares[i][j][2] && squares[i][j][3] &&
                squares[i][j][4]) {
                for (int k = 0; k < 5; k++)
                    forRemoval[j][k] = true;
            }
            if (squares[i][0][j] && squares[i][1][j] &&
                squares[i][2][j] && squares[i][3][j] &&
                squares[i][4][j]) {
                for (int k = 0; k < 5; k++)
                    forRemoval[k][j] = true;
            }
        }
        for (int j = 0; j < 5; j++) {
            for (int k = 0; k < 5; k++) {
                if (forRemoval[j][k]) {
                    squares[i][j][k] = false;
                    score += 10;
                }
            }
        }
    }
    if (oldScore != score)
        std::cout << "Score: " << score << std::endl;
}

void Application::setLights(const std::shared_ptr<Shader> &shader) {
    auto lightManager = LightManager::GetInstance();
    Model cube(GeometricTools::UnitCube3D24WNormals,
               GeometricTools::UnitCube3DTopologyTriangles24,
               BufferLayout({{ShaderDataType::Float3, "position"},
                             {ShaderDataType::Float3, "normals"}}));
    cube.setScale({0.1f, 0.1f, 0.1f});
    float time = glfwGetTime() * 0.5f;
    std::stringstream ss;
    for (int i = 0; i < 9; i++) {
        ss << "spot" << i+1;
        lightManager->setPosition(ss.str(), {calcLight2DPos(time+i*0.25f), 1.5f+i});
        shader->uploadSpotlight(ss.str());
        ss.str("");
    }
}


glm::vec2 Application::calcLight2DPos(float time) {
    while (time > 4.0f)
        time -= 4;
    if (time > 3) {
        time -= 3;
        return {4.5f, lerp(-0.5f, 4.5f, (time))};
    } else if (time > 2) {
        time -= 2.0f;
        return {lerp(-0.5f, 4.5f, (time)), -0.5f};
    } else if (time > 1) {
        time -= 1.0f;
        return {-0.5, lerp(4.5f, -0.5f, (time))};
    } else {
        return {lerp(4.5f, -0.5f, (time)), 4.5f};
    }

}

float Application::lerp(float start, float end, float pt) {
    return (1.0f - pt) * start + pt * end;
}

float Application::sstep3(float pt) {
    return pt * pt * (3.0f - 2.0f * pt);
}

void Application::setupAllLights(float constant, float linear, float quadric) {
    auto lightManager = LightManager::GetInstance();
    std::stringstream ss;
    for (int i = 1; i < 10; i++) {
        ss << "spot" << i;
        lightManager->addSpotLight({ss.str(), constant, linear, quadric});
        ss.str("");
    }
}


template<typename T>
void Application::setUniformAllShaders(const std::string &str, T value) {
    for (const auto & shader : allShaders)
        shader->setUniform(str, value);
}


